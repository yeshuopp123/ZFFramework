/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFOperation.h"

#include "ZFCore/ZFSTLWrapper/zfstl_hashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFOperationPrivate
zfclassFwd _ZFP_I_ZFOperationPrivateTaskData;
zfclass _ZFP_I_ZFOperationPrivateTaskObserverData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFOperationPrivateTaskObserverData, ZFObject)

public:
    _ZFP_I_ZFOperationPrivateTaskData *ownerTaskData; // assign only
    ZFOperationTaskData *operationTaskData;
    zfbool taskIsDuplicate; // used when remove taskObserverData before taskOnStop, to mark whether it's duplicated task
    zftimet cacheExpireTime;

public:
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->ownerTaskData = zfnull;
        this->operationTaskData = zfnull;
        this->taskIsDuplicate = zffalse;
        this->cacheExpireTime = ZFOperationCacheExpireTimeUnspecified;
        return this;
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfRelease(this->operationTaskData);
        zfsuper::objectOnDealloc();
    }
};
zfclass _ZFP_I_ZFOperationPrivateTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFOperationPrivateTaskData, ZFObject)
public:
    ZFOperationParam *operationParam; // auto release
    ZFArrayEditable *taskObserverDatas; // array of _ZFP_I_ZFOperationPrivateTaskObserverData, auto create and release

public:
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->operationParam = zfnull;
        this->taskObserverDatas = zfAllocWithoutLeakTest(ZFArrayEditable);
        return this;
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfRelease(operationParam);
        zfReleaseWithoutLeakTest(this->taskObserverDatas);
        zfsuper::objectOnDealloc();
    }
};
zfclassNotPOD _ZFP_ZFOperationPrivate
{
public:
    ZFOperation *pimplOwner;
    const ZFClass *classForOperationParam;
    const ZFClass *classForOperationResult;
    const ZFClass *classForOperationObserver;
    const ZFClass *classForOperationCache;
    const ZFClass *classForOperationProgress;
    const ZFClass *classForOperationTaskData;
    ZFArrayEditable *tasks; // array of _ZFP_I_ZFOperationPrivateTaskData, newer task appends to tail
    ZFArrayEditable *tasksQueued; // array of _ZFP_I_ZFOperationPrivateTaskData, newer task appends to tail
    ZFArrayEditable *caches; // array of ZFOperationCache, newer cache appends to tail
    ZFIdentityGenerator idGenerator;
    zfbool cacheNeedSave;
    zfbool cacheNeedRestore;
    zfbool startFirstFlag;
    ZFOperationParam *dummyParam;

public:
    ZFOperationParam *dummyParamAccess(void)
    {
        if(this->dummyParam)
        {
            this->dummyParam = zfRetainWithoutLeakTest(this->pimplOwner->createParam().to<ZFOperationParam *>());
        }
        return this->dummyParam;
    }

public:
    _ZFP_I_ZFOperationPrivateTaskData *findTaskDataForOperationParam(ZF_IN ZFOperationParam *operationParam,
                                                                     ZF_IN_OPT zfbool includeQueued = zftrue,
                                                                     ZF_OUT_OPT zfbool *isInQueue = zfnull)
    {
        for(zfindex iTaskData = 0; iTaskData < this->tasks->count(); ++iTaskData)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = this->tasks->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
            if(taskData->operationParam->paramIsEqual(operationParam))
            {
                if(isInQueue != zfnull)
                {
                    *isInQueue = zffalse;
                }
                return taskData;
            }
        }
        if(includeQueued)
        {
            for(zfindex iTaskData = 0; iTaskData < this->tasksQueued->count(); ++iTaskData)
            {
                _ZFP_I_ZFOperationPrivateTaskData *taskData = this->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
                if(taskData->operationParam->paramIsEqual(operationParam))
                {
                    if(isInQueue != zfnull)
                    {
                        *isInQueue = zftrue;
                    }
                    return taskData;
                }
            }
        }
        return zfnull;
    }
    _ZFP_I_ZFOperationPrivateTaskObserverData *findTaskObserverDataForOperationId(ZF_IN zfidentity operationId,
                                                                                  ZF_IN_OPT zfbool includeQueued = zftrue,
                                                                                  ZF_OUT_OPT zfbool *isInQueue = zfnull)
    {
        if(isInQueue != zfnull)
        {
            *isInQueue = zffalse;
        }
        for(zfindex iTaskData = 0; iTaskData < this->tasks->count(); ++iTaskData)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = this->tasks->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
            for(zfindex iTaskObserverData = 0; iTaskObserverData < taskData->taskObserverDatas->count(); ++iTaskObserverData)
            {
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iTaskObserverData);
                if(taskObserverData->operationTaskData->operationId() == operationId)
                {
                    return taskObserverData;
                }
            }
        }
        if(includeQueued)
        {
            for(zfindex iTaskData = 0; iTaskData < this->tasksQueued->count(); ++iTaskData)
            {
                _ZFP_I_ZFOperationPrivateTaskData *taskData = this->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
                for(zfindex iTaskObserverData = 0; iTaskObserverData < taskData->taskObserverDatas->count(); ++iTaskObserverData)
                {
                    _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iTaskObserverData);
                    if(taskObserverData->operationTaskData->operationId() == operationId)
                    {
                        if(isInQueue != zfnull)
                        {
                            *isInQueue = zftrue;
                        }
                        return taskObserverData;
                    }
                }
            }
        }
        return zfnull;
    }

public:
    void checkAddCache(ZF_IN ZFOperationParam *operationParam,
                       ZF_IN ZFOperationResult *operationResult,
                       ZF_IN zftimet cacheExpireTime)
    {
        if(cacheExpireTime != ZFOperationCacheExpireTimeDisable && operationResult->resultType() == ZFResultType::e_Success)
        {
            if(cacheExpireTime > 0)
            {
                this->pimplOwner->cacheAdd(this->pimplOwner->createCache(operationParam, operationResult, cacheExpireTime).to<ZFOperationCache *>());
                this->pimplOwner->cacheSaveRequest();
            }
            else if(this->pimplOwner->cacheExpireTimeDefault() > 0)
            {
                this->pimplOwner->cacheAdd(this->pimplOwner->createCache(operationParam, operationResult).to<ZFOperationCache *>());
                this->pimplOwner->cacheSaveRequest();
            }
        }
        this->pimplOwner->cacheSave();
    }

public:
    void doStart(ZF_IN _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData,
                 ZF_IN zfbool isDuplicatedTask)
    {
        taskObserverData->taskIsDuplicate = isDuplicatedTask;
        this->idGenerator.markUsed(taskObserverData->operationTaskData->operationId());
        if(!this->startFirstFlag)
        {
            this->startFirstFlag = zftrue;
            this->pimplOwner->taskOnStartFirst();
        }
        if(!isDuplicatedTask)
        {
            this->pimplOwner->taskOnStart(taskObserverData->operationTaskData);
            this->pimplOwner->operationTaskOnStart(taskObserverData->operationTaskData);
        }
        else
        {
            this->pimplOwner->taskOnStartDuplicated(taskObserverData->operationTaskData);
        }
    }

public:
    void prepareStopAll(ZF_IN ZFArrayEditable *taskArray,
                        ZF_IN_OUT ZFArrayEditable *toNotifyTaskObserverDatas)
    {
        for(zfindex iTask = 0;
            iTask < taskArray->count();
            ++iTask)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = taskArray->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTask);
            if(taskArray == this->tasks)
            {
                for(zfindex iObserver = taskData->taskObserverDatas->count() - 1; iObserver != 0; --iObserver)
                {
                    taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iObserver)->taskIsDuplicate = zftrue;
                }
            }

            toNotifyTaskObserverDatas->addFrom(taskData->taskObserverDatas);
        }
        taskArray->removeAll();
    }
    void prepareStopForOperationId(ZF_IN ZFArrayEditable *taskArray,
                                   ZF_IN_OUT ZFArrayEditable *toNotifyTaskObserverDatas,
                                   ZF_IN zfidentity operationId)
    {
        for(zfindex iTask = 0;
            iTask < taskArray->count();
            ++iTask)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = taskArray->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTask);
            for(zfindex iObserver = 0;
                iObserver < taskData->taskObserverDatas->count();
                ++iObserver)
            {
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iObserver);
                if(taskObserverData->operationTaskData->operationId() == operationId)
                {
                    toNotifyTaskObserverDatas->add(taskObserverData);
                    taskData->taskObserverDatas->remove(iObserver);
                    --iObserver;

                    if(taskData->taskObserverDatas->count() == 0)
                    {
                        taskArray->remove(iTask);
                        taskObserverData->taskIsDuplicate = zffalse;
                        --iTask;
                    }
                    else
                    {
                        taskObserverData->taskIsDuplicate = (taskArray == this->tasks);
                    }

                    // id is unique, stop on first match
                    return ;
                }
            }
        }
    }
    void prepareStopForOperationParam(ZF_IN ZFArrayEditable *taskArray,
                                      ZF_IN_OUT ZFArrayEditable *toNotifyTaskObserverDatas,
                                      ZF_IN ZFOperationParam *operationParam,
                                      ZF_IN_OPT zfbool stopOnFirstMatch = zftrue)
    {
        if(!this->pimplOwner->paramIsValid(operationParam))
        {
            return ;
        }
        for(zfindex iTask = 0;
            iTask < taskArray->count();
            ++iTask)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = taskArray->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTask);
            if(taskData->operationParam->paramIsEqual(operationParam))
            {
                if(taskArray == this->tasks)
                {
                    for(zfindex iObserver = taskData->taskObserverDatas->count() - 1; iObserver != 0; --iObserver)
                    {
                        taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iObserver)->taskIsDuplicate = zftrue;
                    }
                }

                toNotifyTaskObserverDatas->addFrom(taskData->taskObserverDatas);
                taskArray->remove(iTask);
                --iTask;

                if(stopOnFirstMatch)
                {
                    return ;
                }
            }
        }
    }
    void prepareStopForCategory(ZF_IN ZFArrayEditable *taskArray,
                                ZF_IN_OUT ZFArrayEditable *toNotifyTaskObserverDatas,
                                ZF_IN ZFObject *category)
    {
        if(category == zfnull)
        {
            return ;
        }

        for(zfindex iTask = 0;
            iTask < taskArray->count();
            ++iTask)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = taskArray->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTask);
            for(zfindex iObserver = 0;
                iObserver < taskData->taskObserverDatas->count();
                ++iObserver)
            {
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iObserver);
                if(taskObserverData->operationTaskData->taskCategory()->isContain(category))
                {
                    toNotifyTaskObserverDatas->add(taskObserverData);
                    taskData->taskObserverDatas->remove(iObserver);
                    --iObserver;

                    if(taskData->taskObserverDatas->count() == 0)
                    {
                        taskArray->remove(iTask);
                        taskObserverData->taskIsDuplicate = zffalse;
                        --iTask;
                    }
                    else
                    {
                        taskObserverData->taskIsDuplicate = (taskArray == this->tasks);
                    }
                }
            }
        }
    }
    void doStop(ZF_IN ZFArrayEditable *toNotifyTaskObserverDatas, // array of _ZFP_I_ZFOperationPrivateTaskObserverData *
                ZF_IN ZFOperationResult *operationResult)
    {
        for(zfindex i = 0; i < toNotifyTaskObserverDatas->count(); ++i)
        {
            _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = toNotifyTaskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i);
            taskObserverData->operationTaskData->operationResultSet(operationResult);
            if(taskObserverData->taskIsDuplicate)
            {
                this->pimplOwner->taskOnStopDuplicated(taskObserverData->operationTaskData);
            }
            else
            {
                this->pimplOwner->taskOnStop(taskObserverData->operationTaskData);
                this->pimplOwner->operationTaskOnStop(taskObserverData->operationTaskData);
            }
            if(i == toNotifyTaskObserverDatas->count() - 1 && this->tasks->isEmpty() && this->tasksQueued->isEmpty()
                && this->startFirstFlag)
            {
                this->startFirstFlag = zffalse;
                this->pimplOwner->taskOnStopLast();
            }

            this->idGenerator.markUnused(taskObserverData->operationTaskData->operationId());
        }

        // check start queued task
        while(this->tasks->count() < this->pimplOwner->taskToStartMax()
            && !this->tasksQueued->isEmpty())
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = this->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(0);
            this->tasks->add(taskData);
            this->tasksQueued->remove(0);
            for(zfindex i = 0; i < taskData->taskObserverDatas->count(); ++i)
            {
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i);
                this->doStart(taskObserverData, (i > 0));
            }
        }
    }

public:
    void notifyEventOperationTaskOnProgress(ZF_IN ZFOperationTaskData *operationTaskData,
                                            ZF_IN ZFOperationProgress *operationProgress)
    {
        operationTaskData->operationProgressSet(operationProgress);
        this->pimplOwner->taskOnProgress(operationTaskData);
        this->pimplOwner->operationTaskOnProgress(operationTaskData);
    }

public:
    _ZFP_ZFOperationPrivate(void)
    : pimplOwner(zfnull)
    , classForOperationParam(zfnull)
    , classForOperationResult(zfnull)
    , classForOperationObserver(zfnull)
    , classForOperationCache(zfnull)
    , classForOperationProgress(zfnull)
    , classForOperationTaskData(zfnull)
    , tasks(zfnull)
    , tasksQueued(zfnull)
    , caches(zfnull)
    , idGenerator()
    , cacheNeedSave(zftrue)
    , cacheNeedRestore(zftrue)
    , startFirstFlag(zffalse)
    , dummyParam(zfnull)
    {
        this->tasks = zfAllocWithoutLeakTest(ZFArrayEditable);
        this->tasksQueued = zfAllocWithoutLeakTest(ZFArrayEditable);
        this->caches = zfAllocWithoutLeakTest(ZFArrayEditable);
    }
    ~_ZFP_ZFOperationPrivate(void)
    {
        zfReleaseWithoutLeakTest(this->tasks);
        zfReleaseWithoutLeakTest(this->tasksQueued);
        zfReleaseWithoutLeakTest(this->caches);

        zfReleaseWithoutLeakTest(this->dummyParam);
    }
};

// ============================================================
// cacheTrim
typedef zfstlhashmap<ZFOperation *, zfbool,
        zfpointer_zfstlHasher<ZFOperation *>, zfpointer_zfstlHashComparer<ZFOperation *>
    > _ZFP_ZFOperationMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFOperationDataHolder, ZFLevelZFFrameworkHigh)
{
    this->cacheTrimListener = ZFCallbackForRawFunction(zfself::cacheTrim);
}
public:
    ZFListener cacheTrimListener;
    _ZFP_ZFOperationMapType attachedObject;
public:
    static ZFLISTENER_PROTOTYPE_EXPAND(cacheTrim)
    {
        const _ZFP_ZFOperationMapType &attachedObject = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFOperationDataHolder)->attachedObject;
        for(_ZFP_ZFOperationMapType::const_iterator it = attachedObject.begin(); it != attachedObject.end(); ++it)
        {
            it->first->cacheTrim();
        }
    }
    void cacheTrimListenerSetup(ZF_IN ZFOperation *operation, ZF_IN zfbool autoTrim)
    {
        ZFCoreMutexLocker();

        if(autoTrim)
        {
            this->attachedObject[operation] = zftrue;
        }
        else
        {
            this->attachedObject.erase(operation);
        }

        if(autoTrim && this->attachedObject.size() == 1)
        {
            ZFObjectGlobalEventObserverAdd(
                ZFGlobalEvent::EventAppOnReceiveMemoryWarning(),
                this->cacheTrimListener);
        }
        else if(!autoTrim && this->attachedObject.size() == 0)
        {
            ZFObjectGlobalEventObserverRemove(
                ZFGlobalEvent::EventAppOnReceiveMemoryWarning(),
                this->cacheTrimListener);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFOperationDataHolder)

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFOperation, zfbool, cacheTrimWhenReceiveMemoryWarning)
{
    this->cacheTrimWhenReceiveMemoryWarningSetInternal(newValue);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFOperationDataHolder)->cacheTrimListenerSetup(this, this->cacheTrimWhenReceiveMemoryWarning());
}

void ZFOperation::cacheTrim(void)
{
    this->cacheTrimBySize(this->cacheTrimThreshold());
}

void ZFOperation::cacheTrimBySize(ZF_IN zfindex size)
{
    zfsynchronizedObject(this);
    if(size >= d->caches->count())
    {
        return ;
    }
    zfblockedAllocWithoutLeakTest(ZFArrayEditable, tmp);
    for(zfindex i = size; i < d->caches->count(); ++i)
    {
        tmp->add(d->caches->get(i));
    }
    d->caches->remove(size, zfindexMax);
    for(zfindex i = 0; i < tmp->count(); ++i)
    {
        this->cacheOnRemove(tmp->get<ZFOperationCache *>(i));
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFOperation)

ZFOBSERVER_EVENT_REGISTER(ZFOperation, OperationCacheOnMatch)
ZFOBSERVER_EVENT_REGISTER(ZFOperation, OperationCacheOnInvalid)
ZFOBSERVER_EVENT_REGISTER(ZFOperation, OperationCacheOnExpire)
ZFOBSERVER_EVENT_REGISTER(ZFOperation, OperationTaskOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFOperation, OperationTaskOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFOperation, OperationTaskOnProgress)

ZFObject *ZFOperation::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFOperationPrivate);
    d->pimplOwner = this;
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFOperationDataHolder)->cacheTrimListenerSetup(this, this->cacheTrimWhenReceiveMemoryWarning());
    return this;
}
void ZFOperation::objectOnDealloc(void)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFOperationDataHolder)->cacheTrimListenerSetup(this, zffalse);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}
void ZFOperation::objectOnDeallocPrepare(void)
{
    this->taskStopAll();
    for(zfindex i = 0; i < d->caches->count(); ++i)
    {
        ZFOperationCache *cache = d->caches->get<ZFOperationCache *>(i);
        cache->objectCachedSet(zffalse);
    }
    d->caches->removeAll();
    zfsuper::objectOnDeallocPrepare();
}

// ============================================================
// type create
zfautoObject ZFOperation::createParam(void)
{
    return this->classForOperationParam()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
}
zfautoObject ZFOperation::createResult(void)
{
    return this->classForOperationResult()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
}
zfautoObject ZFOperation::createResult(ZF_IN ZFResultTypeEnum resultType)
{
    zfautoObject operationResultTmp = this->createResult();
    ZFOperationResult *operationResult = operationResultTmp.to<ZFOperationResult *>();
    operationResult->resultTypeSet(resultType);
    return operationResultTmp;
}
zfautoObject ZFOperation::createObserver(void)
{
    return this->classForOperationObserver()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
}
zfautoObject ZFOperation::createCache(void)
{
    return this->classForOperationCache()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
}
zfautoObject ZFOperation::createCache(ZF_IN ZFOperationParam *operationParam,
                                      ZF_IN ZFOperationResult *operationResult,
                                      ZF_IN_OPT const zftimet &cacheExpireTime /* = zftimetZero */,
                                      ZF_IN_OPT const zftimet &cacheTime /* = zftimetZero */)
{
    zfautoObject operationCacheTmp = this->createCache();
    ZFOperationCache *operationCache = operationCacheTmp.to<ZFOperationCache *>();
    operationCache->operationParamSet(operationParam);
    operationCache->operationResultSet(operationResult);
    operationCache->cacheExpireTimeSet((cacheExpireTime <= 0) ? this->cacheExpireTimeDefault() : cacheExpireTime);
    operationCache->cacheTimeSet((cacheTime == 0) ? ZFTime::currentTimeValue().sec : cacheTime);
    return operationCacheTmp;
}
zfautoObject ZFOperation::createProgress(void)
{
    return this->classForOperationProgress()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
}
zfautoObject ZFOperation::createTaskData(void)
{
    return this->classForOperationTaskData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
}
zfautoObject ZFOperation::createTaskData(ZF_IN ZFOperationParam *operationParam,
                                         ZF_IN_OPT ZFOperationResult *operationResult /* = zfnull */,
                                         ZF_IN_OPT ZFOperationObserver *operationObserver /* = zfnull */,
                                         ZF_IN_OPT ZFOperationCache *operationCache /* = zfnull */,
                                         ZF_IN_OPT zfidentity operationId /* = zfidentityInvalid */,
                                         ZF_IN_OPT ZFOperationProgress *operationProgress /* = zfnull */)
{
    zfautoObject operationTaskDataTmp = this->createTaskData();
    ZFOperationTaskData *operationTaskData = operationTaskDataTmp.to<ZFOperationTaskData *>();
    operationTaskData->operationParamSet(operationParam);
    operationTaskData->operationResultSet(operationResult);
    operationTaskData->operationObserverSet(operationObserver);
    operationTaskData->operationCacheSet(operationCache);
    operationTaskData->operationIdSet(operationId);
    operationTaskData->operationProgressSet(operationProgress);
    return operationTaskDataTmp;
}

static const ZFClass *_ZFP_ZFOperation_findTypeClass(ZF_IN const ZFClass *operationClass,
                                                     ZF_IN const zfchar *desiredTypeName,
                                                     ZF_IN const ZFClass *endTypeClass)
{
    const ZFClass *operationClassToCheck = operationClass;
    while(operationClassToCheck != zfnull && operationClassToCheck != ZFOperation::ClassData())
    {
        const ZFClass *tmp = ZFClass::classForName(zfsConnectLineFree(operationClassToCheck->className(), desiredTypeName));
        if(tmp != zfnull)
        {
            return tmp;
        }
        operationClassToCheck = operationClassToCheck->parentClass();
    }
    return endTypeClass;
}
const ZFClass *ZFOperation::classForOperationParam(void)
{
    if(d->classForOperationParam == zfnull)
    {
        d->classForOperationParam = _ZFP_ZFOperation_findTypeClass(this->classData(), zfText("Param"), ZFOperationParam::ClassData());
    }
    return d->classForOperationParam;
}
const ZFClass *ZFOperation::classForOperationResult(void)
{
    if(d->classForOperationResult == zfnull)
    {
        d->classForOperationResult = _ZFP_ZFOperation_findTypeClass(this->classData(), zfText("Result"), ZFOperationResult::ClassData());
    }
    return d->classForOperationResult;
}
const ZFClass *ZFOperation::classForOperationObserver(void)
{
    if(d->classForOperationObserver == zfnull)
    {
        d->classForOperationObserver = _ZFP_ZFOperation_findTypeClass(this->classData(), zfText("Observer"), ZFOperationObserver::ClassData());
    }
    return d->classForOperationObserver;
}
const ZFClass *ZFOperation::classForOperationCache(void)
{
    if(d->classForOperationCache == zfnull)
    {
        d->classForOperationCache = _ZFP_ZFOperation_findTypeClass(this->classData(), zfText("Cache"), ZFOperationCache::ClassData());
    }
    return d->classForOperationCache;
}
const ZFClass *ZFOperation::classForOperationProgress(void)
{
    if(d->classForOperationProgress == zfnull)
    {
        d->classForOperationProgress = _ZFP_ZFOperation_findTypeClass(this->classData(), zfText("Progress"), ZFOperationProgress::ClassData());
    }
    return d->classForOperationProgress;
}
const ZFClass *ZFOperation::classForOperationTaskData(void)
{
    if(d->classForOperationTaskData == zfnull)
    {
        d->classForOperationTaskData = _ZFP_ZFOperation_findTypeClass(this->classData(), zfText("TaskData"), ZFOperationTaskData::ClassData());
    }
    return d->classForOperationTaskData;
}

// ============================================================
// operation control
zfidentity ZFOperation::taskStart(ZF_IN_OPT ZFOperationParam *operationParam /* = zfnull */,
                                  ZF_IN_OPT ZFOperationObserver *operationObserver /* = zfnull */)
{
    zfblockedAllocWithoutLeakTest(ZFOperationStartParam, startParam);
    startParam->operationTaskDataSet(this->createTaskData(operationParam, zfnull, operationObserver).to<ZFOperationTaskData *>());
    return this->taskStart(startParam);
}
zfidentity ZFOperation::taskStart(ZF_IN ZFOperationStartParam *startParam)
{
    if(startParam == zfnull || startParam->operationTaskData() == zfnull)
    {
        zfCoreLog(zfTextA("invalid start param"));
        return zfidentityInvalid;
    }

    zfsynchronizedObject(this);

    ZFOperationTaskData *operationTaskData = startParam->operationTaskData();
    operationTaskData->ownerOperationSet(this);

    ZFOperationParam *operationParam = operationTaskData->operationParam();
    if(operationParam == zfnull)
    {
        if(this->taskAllowDummyParam())
        {
            operationParam = d->dummyParamAccess();
        }
        else
        {
            zfCoreLog(zfTextA("dummy param is not allowed"));
            return zfidentityInvalid;
        }
    }
    if(!this->paramIsValid(operationParam))
    {
        return zfidentityInvalid;
    }
    if(!this->taskOnCheckNeedStart(operationTaskData))
    {
        operationTaskData->operationResultSet(this->createResult(ZFResultType::e_Cancel).to<ZFOperationResult *>());
        this->operationTaskOnStart(operationTaskData);
        this->operationTaskOnStop(operationTaskData);
        return zfidentityInvalid;
    }

    this->cacheRestore();

    zfidentity operationId = d->idGenerator.next();
    operationTaskData->operationIdSet(operationId);

    // search for cache
    zfindex cacheIndex = d->caches->count() - 1;
    while(cacheIndex != zfindexMax)
    {
        if(d->caches->get<ZFOperationCache *>(cacheIndex)->operationParam()->paramIsEqual(operationParam))
        {
            break;
        }
        --cacheIndex;
    }
    if(cacheIndex != zfindexMax)
    {
        this->cacheSaveRequest();

        ZFOperationCache *operationCache = d->caches->get<ZFOperationCache *>(cacheIndex);
        zfRetain(operationCache);
        zfblockedRelease(operationCache);
        d->caches->remove(cacheIndex);

        zfbool cacheValid = this->cacheIsValid(operationCache);
        zfbool cacheIsExpired = operationCache->cacheIsExpired();
        if(!cacheValid)
        {
            this->cacheOnInvalid(operationCache);
            this->operationCacheOnInvalid(operationCache);
        }
        if(cacheIsExpired)
        {
            this->cacheOnExpire(operationCache);
            this->operationCacheOnExpire(operationCache);
        }
        if(cacheValid)
        {
            {
                operationTaskData->operationCacheSet(operationCache);
                this->cacheOnMatch(operationCache);
                this->operationCacheOnMatch(operationTaskData);
                zfRelease(operationTaskData);
            }

            ZFOperationCacheMatchActionEnum cacheMatchAction = startParam->cacheMatchAction();
            if(cacheMatchAction == ZFOperationCacheMatchAction::e_Unspecified)
            {
                cacheMatchAction = this->cacheMatchAction();
            }
            if(cacheMatchAction == ZFOperationCacheMatchAction::e_Unspecified)
            {
                cacheMatchAction = ZFOperationCacheMatchAction::e_NotifyFinish;
            }
            switch(cacheMatchAction)
            {
                case ZFOperationCacheMatchAction::e_NotifyFinish:
                {
                    if(!cacheIsExpired)
                    {
                        d->caches->add(operationCache);
                        operationTaskData->operationResultSet(operationCache->operationResult());
                        operationTaskData->operationCacheSet(operationCache);
                        this->operationTaskOnStart(operationTaskData);
                        this->operationTaskOnStop(operationTaskData);
                        return zfidentityInvalid;
                    }
                    else
                    {
                        this->cacheOnRemove(operationCache);
                        break;
                    }
                }
                case ZFOperationCacheMatchAction::e_NotifyFinishThenRemove:
                {
                    if(!cacheIsExpired)
                    {
                        operationTaskData->operationResultSet(operationCache->operationResult());
                        operationTaskData->operationCacheSet(operationCache);
                        this->operationTaskOnStart(operationTaskData);
                        this->operationTaskOnStop(operationTaskData);
                        this->cacheRemove(operationParam);
                        return zfidentityInvalid;
                    }
                    else
                    {
                        this->cacheOnRemove(operationCache);
                        break;
                    }
                }
                case ZFOperationCacheMatchAction::e_StartNew:
                {
                    if(!cacheIsExpired)
                    {
                        d->caches->add(operationCache);
                    }
                    else
                    {
                        this->cacheOnRemove(operationCache);
                    }
                    break;
                }
                case ZFOperationCacheMatchAction::e_StartNewThenRemove:
                {
                    this->cacheOnRemove(operationCache);
                    break;
                }
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zfidentityInvalid;
            }
        }
        else
        {
            this->cacheOnRemove(operationCache);
        }
    }

    // find duplicate operation
    ZFOperationTaskDuplicateActionEnum taskDuplicateAction = startParam->taskDuplicateAction();
    if(taskDuplicateAction == ZFOperationTaskDuplicateAction::e_Unspecified)
    {
        taskDuplicateAction = this->taskDuplicateAction();
    }
    if(taskDuplicateAction == ZFOperationTaskDuplicateAction::e_Unspecified)
    {
        taskDuplicateAction = ZFOperationTaskDuplicateAction::e_Merge;
    }
    if(taskDuplicateAction != ZFOperationTaskDuplicateAction::e_NewTask)
    {
        zfbool isInQueue = zffalse;
        _ZFP_I_ZFOperationPrivateTaskData *dupTaskData = d->findTaskDataForOperationParam(operationParam, zftrue, &isInQueue);
        if(dupTaskData != zfnull)
        {
            if(taskDuplicateAction == ZFOperationTaskDuplicateAction::e_Merge)
            {
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = zfAllocWithoutLeakTest(_ZFP_I_ZFOperationPrivateTaskObserverData);
                taskObserverData->ownerTaskData = dupTaskData;
                taskObserverData->operationTaskData = operationTaskData;
                taskObserverData->cacheExpireTime = startParam->cacheExpireTime();
                dupTaskData->taskObserverDatas->add(taskObserverData);
                if(!isInQueue)
                {
                    d->doStart(taskObserverData, zftrue);
                }
                return operationId;
            }
            else
            {
                operationTaskData->operationResultSet(this->createResult(ZFResultType::e_Cancel).to<ZFOperationResult *>());
                return zfidentityInvalid;
            }
        }
    }

    zfblockedAllocWithoutLeakTest(_ZFP_I_ZFOperationPrivateTaskData, taskData);
    ZFPropertyChange(taskData->operationParam, operationParam);

    zfblockedAllocWithoutLeakTest(_ZFP_I_ZFOperationPrivateTaskObserverData, taskObserverData);
    taskData->taskObserverDatas->add(taskObserverData);
    taskObserverData->ownerTaskData = taskData;
    taskObserverData->operationTaskData = operationTaskData;
    taskObserverData->cacheExpireTime = startParam->cacheExpireTime();

    if(d->tasks->count() > 10000 || d->tasksQueued->count() > 10000)
    {
        zfCoreLog(zfTextA("[%s] too many operations attached, do you forget taskNotifyFinish?"), zfsCoreZ2A(this->objectInfoOfInstance().cString()));
    }

    if(d->tasks->count() < this->taskToStartMax())
    {
        d->tasks->add(taskData);
        d->doStart(taskObserverData, zffalse);
    }
    else
    {
        d->tasksQueued->add(taskData);
    }

    return operationId;
}
void ZFOperation::taskStop(ZF_IN zfidentity operationId)
{
    if(zfidentityIsValid(operationId))
    {
        zfsynchronizedObjectLock(this);
        zfblockedAllocWithoutLeakTest(ZFArrayEditable, toNotifyTaskObserverDatas);
        d->prepareStopForOperationId(d->tasks, toNotifyTaskObserverDatas, operationId);
        if(toNotifyTaskObserverDatas->isEmpty())
        {
            d->prepareStopForOperationId(d->tasksQueued, toNotifyTaskObserverDatas, operationId);
        }
        d->doStop(toNotifyTaskObserverDatas, this->createResult(ZFResultType::e_Cancel).to<ZFOperationResult *>());
        zfsynchronizedObjectUnlock(this);
    }
}
zfautoObject ZFOperation::taskStopAndGetResult(ZF_IN zfidentity operationId)
{
    if(zfidentityIsValid(operationId))
    {
        zfsynchronizedObject(this);
        zfautoObject operationResultTmp = this->createResult(ZFResultType::e_Cancel);
        ZFOperationResult *operationResult = operationResultTmp.to<ZFOperationResult *>();
        zfblockedAllocWithoutLeakTest(ZFArrayEditable, toNotifyTaskObserverDatas);
        d->prepareStopForOperationId(d->tasks, toNotifyTaskObserverDatas, operationId);
        if(toNotifyTaskObserverDatas->isEmpty())
        {
            d->prepareStopForOperationId(d->tasksQueued, toNotifyTaskObserverDatas, operationId);
        }
        d->doStop(toNotifyTaskObserverDatas, operationResult);

        if(toNotifyTaskObserverDatas->count() == 1)
        {
            operationResult = toNotifyTaskObserverDatas->getFirst<_ZFP_I_ZFOperationPrivateTaskObserverData *>()->operationTaskData->operationResult();
            return zfautoObjectCreate(operationResult);
        }
    }
    return zfautoObjectNull;
}
void ZFOperation::taskStop(ZF_IN_OPT ZFOperationParam *operationParam /* = zfnull */)
{
    zfsynchronizedObject(this);
    if(operationParam == zfnull)
    {
        operationParam = d->dummyParamAccess();
    }
    if(!this->paramIsValid(operationParam))
    {
        return ;
    }
    zfblockedAllocWithoutLeakTest(ZFArrayEditable, toNotifyTaskObserverDatas);
    d->prepareStopForOperationParam(d->tasks, toNotifyTaskObserverDatas, operationParam, zffalse);
    d->prepareStopForOperationParam(d->tasksQueued, toNotifyTaskObserverDatas, operationParam, zffalse);
    d->doStop(toNotifyTaskObserverDatas, this->createResult(ZFResultType::e_Cancel).to<ZFOperationResult *>());
}
void ZFOperation::taskStopForCategory(ZF_IN ZFObject *category)
{
    if(category != zfnull)
    {
        zfsynchronizedObjectLock(this);
        zfblockedAllocWithoutLeakTest(ZFArrayEditable, toNotifyTaskObserverDatas);
        d->prepareStopForCategory(d->tasks, toNotifyTaskObserverDatas, category);
        d->prepareStopForCategory(d->tasksQueued, toNotifyTaskObserverDatas, category);
        d->doStop(toNotifyTaskObserverDatas, this->createResult(ZFResultType::e_Cancel).to<ZFOperationResult *>());
        zfsynchronizedObjectUnlock(this);
    }
}
void ZFOperation::taskStopAll(void)
{
    zfsynchronizedObjectLock(this);
    zfblockedAllocWithoutLeakTest(ZFArrayEditable, toNotifyTaskObserverDatas);
    d->prepareStopAll(d->tasks, toNotifyTaskObserverDatas);
    d->prepareStopAll(d->tasksQueued, toNotifyTaskObserverDatas);
    d->doStop(toNotifyTaskObserverDatas, this->createResult(ZFResultType::e_Cancel).to<ZFOperationResult *>());
    zfsynchronizedObjectUnlock(this);
}

void ZFOperation::taskStopObserver(ZF_IN zfidentity operationId)
{
    if(zfidentityIsValid(operationId))
    {
        zfsynchronizedObject(this);
        _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = d->findTaskObserverDataForOperationId(operationId);
        if(taskObserverData != zfnull)
        {
            taskObserverData->operationTaskData->operationObserverSet(zfnull);
        }
    }
}
void ZFOperation::taskStopObserver(ZF_IN_OPT ZFOperationParam *operationParam /* = zfnull */)
{
    zfsynchronizedObjectLock(this);
    if(operationParam == zfnull)
    {
        operationParam = d->dummyParamAccess();
    }
    if(!this->paramIsValid(operationParam))
    {
        return ;
    }
    for(zfindex i = 0; i < d->tasks->count(); ++i)
    {
        _ZFP_I_ZFOperationPrivateTaskData *taskData = d->tasks->get<_ZFP_I_ZFOperationPrivateTaskData *>(i);
        if(taskData->operationParam->paramIsEqual(operationParam))
        {
            for(zfindex iObserver = 0; iObserver < taskData->taskObserverDatas->count(); ++iObserver)
            {
                taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i)->operationTaskData->operationObserverSet(zfnull);
            }
        }
    }
    for(zfindex i = 0; i < d->tasksQueued->count(); ++i)
    {
        _ZFP_I_ZFOperationPrivateTaskData *taskData = d->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(i);
        if(taskData->operationParam->paramIsEqual(operationParam))
        {
            for(zfindex iObserver = 0; iObserver < taskData->taskObserverDatas->count(); ++iObserver)
            {
                taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i)->operationTaskData->operationObserverSet(zfnull);
            }
        }
    }
}
void ZFOperation::taskStopObserverForCategory(ZF_IN ZFObject *category)
{
    if(category != zfnull)
    {
        zfsynchronizedObject(this);

        for(zfindex i = 0; i < d->tasks->count(); ++i)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = d->tasks->get<_ZFP_I_ZFOperationPrivateTaskData *>(i);
            for(zfindex iObserver = 0; iObserver < taskData->taskObserverDatas->count(); ++iObserver)
            {
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i);
                if(taskObserverData->operationTaskData->taskCategory()->isContain(category))
                {
                    taskObserverData->operationTaskData->operationObserverSet(zfnull);
                }
            }
        }
        for(zfindex i = 0; i < d->tasksQueued->count(); ++i)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = d->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(i);
            for(zfindex iObserver = 0; iObserver < taskData->taskObserverDatas->count(); ++iObserver)
            {
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i);
                if(taskObserverData->operationTaskData->taskCategory()->isContain(category))
                {
                    taskObserverData->operationTaskData->operationObserverSet(zfnull);
                }
            }
        }
    }
}

zfindex ZFOperation::taskCount(void)
{
    zfsynchronizedObject(this);
    return d->tasks->count();
}
zfindex ZFOperation::taskQueuedCount(void)
{
    zfsynchronizedObject(this);
    return d->tasksQueued->count();
}
zfbool ZFOperation::taskRunning(void)
{
    zfsynchronizedObject(this);
    return (!d->tasks->isEmpty() || !d->tasksQueued->isEmpty());
}

zfbool ZFOperation::taskIsAlive(ZF_IN ZFOperationParam *operationParam)
{
    zfsynchronizedObject(this);
    return (d->findTaskDataForOperationParam(operationParam) != zfnull);
}
zfbool ZFOperation::taskIsAlive(ZF_IN zfidentity operationId)
{
    zfsynchronizedObject(this);
    return (d->findTaskObserverDataForOperationId(operationId) != zfnull);
}

zfbool ZFOperation::paramIsValid(ZF_IN ZFOperationParam *operationParam)
{
    return zftrue;
}
void ZFOperation::taskNotifyFinish(ZF_IN ZFOperationParam *operationParam,
                                   ZF_IN ZFOperationResult *operationResult)
{
    zfsynchronizedObject(this);

    if(operationParam == zfnull)
    {
        operationParam = d->dummyParamAccess();
    }
    zfCoreAssertWithMessage(this->paramIsValid(operationParam) && operationResult != zfnull, zfTextA("invalid param or result"));

    zfblockedAllocWithoutLeakTest(ZFArrayEditable, toNotifyTaskObserverDatas);
    d->prepareStopForOperationParam(d->tasks, toNotifyTaskObserverDatas, operationParam);

    if(toNotifyTaskObserverDatas->isEmpty())
    { // notify a canceled task, not an error, simply return
        return ;
    }
    d->doStop(toNotifyTaskObserverDatas, operationResult);

    d->checkAddCache(
        operationParam,
        operationResult,
        toNotifyTaskObserverDatas->getLast<_ZFP_I_ZFOperationPrivateTaskObserverData *>()->cacheExpireTime);
}
void ZFOperation::taskNotifyFinish(ZF_IN zfidentity operationId,
                                   ZF_IN ZFOperationResult *operationResult)
{
    zfCoreAssertWithMessage(operationResult != zfnull, zfTextA("result is null"));

    if(!zfidentityIsValid(operationId))
    {
        return ;
    }

    zfsynchronizedObject(this);

    zfblockedAllocWithoutLeakTest(ZFArrayEditable, toNotifyTaskObserverDatas);
    d->prepareStopForOperationId(d->tasks, toNotifyTaskObserverDatas, operationId);

    if(toNotifyTaskObserverDatas->isEmpty())
    { // notify a canceled task, not an error, simply return
        return ;
    }
    d->doStop(toNotifyTaskObserverDatas, operationResult);

    d->checkAddCache(
        toNotifyTaskObserverDatas->getFirst<_ZFP_I_ZFOperationPrivateTaskObserverData *>()->ownerTaskData->operationParam,
        operationResult,
        toNotifyTaskObserverDatas->getLast<_ZFP_I_ZFOperationPrivateTaskObserverData *>()->cacheExpireTime);
}
void ZFOperation::taskNotifyProgress(ZF_IN ZFOperationParam *operationParam,
                                     ZF_IN_OPT ZFOperationProgress *operationProgress /* = zfnull */)
{
    zfsynchronizedObject(this);
    if(operationParam == zfnull)
    {
        operationParam = d->dummyParamAccess();
    }
    zfCoreAssertWithMessage(this->paramIsValid(operationParam), zfTextA("invalid param"));
    _ZFP_I_ZFOperationPrivateTaskData *taskData = d->findTaskDataForOperationParam(operationParam, zffalse);
    if(taskData != zfnull)
    {
        for(zfindex i = 0; i < taskData->taskObserverDatas->count(); ++i)
        {
            d->notifyEventOperationTaskOnProgress(
                taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i)->operationTaskData,
                operationProgress);
        }
    }
}
void ZFOperation::taskNotifyProgress(ZF_IN zfidentity operationId,
                                     ZF_IN_OPT ZFOperationProgress *operationProgress /* = zfnull */)
{
    if(zfidentityIsValid(operationId))
    {
        zfsynchronizedObject(this);
        _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = d->findTaskObserverDataForOperationId(operationId, zffalse);
        if(taskObserverData != zfnull)
        {
            for(zfindex i = 0; i < taskObserverData->ownerTaskData->taskObserverDatas->count(); ++i)
            {
                d->notifyEventOperationTaskOnProgress(
                    taskObserverData->ownerTaskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i)->operationTaskData,
                    operationProgress);
            }
        }
    }
}

void ZFOperation::taskOnStart(ZF_IN ZFOperationTaskData *operationTaskData)
{
    // nothing to do
}

// ============================================================
// observer
void ZFOperation::operationCacheOnMatch(ZF_IN ZFOperationTaskData *operationTaskData)
{
    if(operationTaskData->operationObserver() != zfnull)
    {
        operationTaskData->operationObserver()->observerNotify(
            ZFOperation::EventOperationCacheOnMatch(),
            operationTaskData);
    }
    this->observerNotify(ZFOperation::EventOperationCacheOnMatch(), operationTaskData);
}
void ZFOperation::operationCacheOnInvalid(ZF_IN ZFOperationCache *operationCache)
{
    this->observerNotify(ZFOperation::EventOperationCacheOnInvalid(), operationCache);
}
void ZFOperation::operationCacheOnExpire(ZF_IN ZFOperationCache *operationCache)
{
    this->observerNotify(ZFOperation::EventOperationCacheOnExpire(), operationCache);
}
void ZFOperation::operationTaskOnStart(ZF_IN ZFOperationTaskData *operationTaskData)
{
    if(operationTaskData->operationObserver() != zfnull)
    {
        operationTaskData->operationObserver()->observerNotify(
            ZFOperation::EventOperationTaskOnStart(),
            operationTaskData);
    }
    this->observerNotify(ZFOperation::EventOperationTaskOnStart(), operationTaskData);
}
void ZFOperation::operationTaskOnStop(ZF_IN ZFOperationTaskData *operationTaskData)
{
    if(operationTaskData->operationObserver() != zfnull)
    {
        operationTaskData->operationObserver()->observerNotify(
            ZFOperation::EventOperationTaskOnStop(),
            operationTaskData);
    }
    this->observerNotify(ZFOperation::EventOperationTaskOnStop(), operationTaskData);
}
void ZFOperation::operationTaskOnProgress(ZF_IN ZFOperationTaskData *operationTaskData)
{
    if(operationTaskData->operationObserver() != zfnull)
    {
        operationTaskData->operationObserver()->observerNotify(
            ZFOperation::EventOperationTaskOnProgress(),
            operationTaskData);
    }
    this->observerNotify(ZFOperation::EventOperationTaskOnProgress(), operationTaskData);
}

// ============================================================
// advance operation control
ZFOperationTaskData *ZFOperation::operationGetTaskData(ZF_IN zfidentity operationId)
{
    zfsynchronizedObject(this);
    _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = d->findTaskObserverDataForOperationId(operationId);
    if(taskObserverData != zfnull)
    {
        return taskObserverData->operationTaskData;
    }
    return zfnull;
}
void ZFOperation::operationGetTaskList(ZF_IN_OUT ZFArrayEditable *taskDatas,
                                       ZF_IN zfidentity operationId)
{
    if(taskDatas != zfnull)
    {
        zfsynchronizedObject(this);
        _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = d->findTaskObserverDataForOperationId(operationId);
        if(taskObserverData != zfnull)
        {
            ZFArray *taskObserverDatas = taskObserverData->ownerTaskData->taskObserverDatas;
            for(zfindex i = 0; i < taskObserverDatas->count(); ++i)
            {
                taskDatas->add(taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i)->operationTaskData);
            }
        }
    }
}
void ZFOperation::operationGetTaskList(ZF_IN_OUT ZFArrayEditable *taskDatas)
{
    if(taskDatas != zfnull)
    {
        zfsynchronizedObject(this);
        for(zfindex iTaskData = d->tasks->count(); iTaskData < d->tasks->count(); ++iTaskData)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = d->tasks->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
            for(zfindex iTaskObserverData = 0; iTaskObserverData < taskData->taskObserverDatas->count(); ++iTaskObserverData)
            {
                taskDatas->add(taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iTaskObserverData)->operationTaskData);
            }
        }
    }
}
void ZFOperation::operationGetTaskListQueued(ZF_IN_OUT ZFArrayEditable *taskDatas)
{
    if(taskDatas != zfnull)
    {
        zfsynchronizedObject(this);
        for(zfindex iTaskData = d->tasksQueued->count(); iTaskData < d->tasksQueued->count(); ++iTaskData)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = d->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
            for(zfindex iTaskObserverData = 0; iTaskObserverData < taskData->taskObserverDatas->count(); ++iTaskObserverData)
            {
                taskDatas->add(taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iTaskObserverData)->operationTaskData);
            }
        }
    }
}

// ============================================================
// cache control
void ZFOperation::cacheAdd(ZF_IN ZFOperationCache *operationCache)
{
    zfCoreAssertWithMessage(this->cacheIsValid(operationCache), zfTextA("adding a invalid cache"));
    if(operationCache->cacheIsExpired())
    {
        return ;
    }
    zfbool found = zffalse;
    for(zfindex i = 0; i < d->caches->count(); ++i)
    {
        if(d->caches->get<ZFOperationCache *>(i)->operationParam()->paramIsEqual(operationCache->operationParam()))
        {
            this->cacheOnRemove(d->caches->get<ZFOperationCache *>(i));
            d->caches->set(i, operationCache);
            found = zftrue;
            break;
        }
    }
    if(!found)
    {
        d->caches->add(operationCache);
        this->cacheOnAdd(operationCache);
    }
    else
    {
        this->cacheOnAdd(operationCache);
    }
    while(d->caches->count() > this->cacheMaxSize())
    {
        ZFOperationCache *cacheTmp = zfRetain(d->caches->get<ZFOperationCache *>(0));
        d->caches->remove(0);
        this->cacheOnRemove(cacheTmp);
        zfRelease(cacheTmp);
    }
}
void ZFOperation::cacheRemove(ZF_IN ZFOperationParam *operationParam)
{
    zfsynchronizedObject(this);
    if(operationParam == zfnull)
    {
        operationParam = d->dummyParamAccess();
    }
    if(!this->paramIsValid(operationParam))
    {
        return ;
    }

    for(zfindex i = 0; i < d->caches->count(); ++i)
    {
        if(d->caches->get<ZFOperationCache *>(i)->operationParam()->paramIsEqual(operationParam))
        {
            ZFOperationCache *cache = zfRetain(d->caches->get<ZFOperationCache *>(i));
            d->caches->remove(i);
            this->cacheOnRemove(cache);
            zfRelease(cache);
            return ;
        }
    }
}
void ZFOperation::cacheRemoveAll(void)
{
    ZFArrayEditable *tmpArray = zfAllocWithoutLeakTest(ZFArrayEditable, d->caches);
    d->caches->removeAll();
    for(zfindex i = 0; i < tmpArray->count(); ++i)
    {
        this->cacheOnRemove(tmpArray->get<ZFOperationCache *>(i));
    }
    zfReleaseWithoutLeakTest(tmpArray);
}
zfbool ZFOperation::cacheIsValid(ZF_IN ZFOperationCache *operationCache)
{
    return (operationCache != zfnull
        && this->paramIsValid(operationCache->operationParam())
        && operationCache->operationResult()->resultType() == ZFResultType::e_Success);
}

void ZFOperation::cacheSaveRequest(void)
{
    d->cacheNeedSave = zftrue;
}
void ZFOperation::cacheSave(void)
{
    if(d->cacheNeedSave)
    {
        d->cacheNeedSave = zffalse;

        this->cacheOnSave(d->caches);
    }
}
void ZFOperation::cacheRestoreRequest(void)
{
    d->cacheNeedRestore = zftrue;
}
void ZFOperation::cacheRestore(void)
{
    if(d->cacheNeedRestore)
    {
        d->cacheNeedRestore = zffalse;

        this->cacheRemoveAll();
        zfblockedAllocWithoutLeakTest(ZFArrayEditable, tmpArray);
        this->cacheOnRestore(tmpArray);
        for(zfindex i = 0; i < tmpArray->count(); ++i)
        {
            this->cacheAdd(tmpArray->get<ZFOperationCache *>(i));
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

