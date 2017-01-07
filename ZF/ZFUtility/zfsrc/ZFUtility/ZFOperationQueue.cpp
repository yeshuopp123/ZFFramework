/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFOperationQueue.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFOperationQueueChildTaskData)
ZFObject *ZFOperationQueueChildTaskData::objectOnInit(ZF_IN ZFOperation *childOperation,
                                                      ZF_IN ZFOperationStartParam *childTaskData)
{
    this->objectOnInit();
    this->childOperationSet(childOperation);
    this->childTaskDataSet(childTaskData);
    return this;
}
ZFObject *ZFOperationQueueChildTaskData::objectOnInit(void)
{
    return zfsuper::objectOnInit();
}
zfidentity ZFOperationQueueChildTaskData::objectHash(void)
{
    return zfidentityHash(0
        , ZFObjectHash(this->childOperation())
        , ZFObjectHash(this->childTaskData())
        , (zfidentity)this->stopQueueIfCancel()
        , (zfidentity)this->stopQueueIfFail()
        );
}
ZFCompareResult ZFOperationQueueChildTaskData::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(zftrue
        && ZFObjectCompare(this->childOperation(), another->childOperation()) == ZFCompareTheSame
        && ZFObjectCompare(this->childTaskData(), another->childTaskData()) == ZFCompareTheSame
        && this->stopQueueIfCancel() == another->stopQueueIfCancel()
        && this->stopQueueIfFail() == another->stopQueueIfFail()
        )
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

// ============================================================
ZFOBJECT_REGISTER(ZFOperationQueueParam)
zfidentity ZFOperationQueueParam::objectHash(void)
{
    return zfidentityHash(zfsuper::objectHash()
        , ZFObjectHash(this->childTaskDatas())
        , (zfidentity)this->childToStartMax()
        );
}

// ============================================================
ZFOBJECT_REGISTER(ZFOperationQueueResult)
zfidentity ZFOperationQueueResult::objectHash(void)
{
    return zfidentityHash(zfsuper::objectHash()
        , (zfidentity)this->hasCanceledChildTask()
        , (zfidentity)this->hasFailedChildTask()
        , ZFObjectHash(this->childResults())
        , ZFObjectHash(this->childResultsInOrder())
        );
}
ZFCompareResult ZFOperationQueueResult::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(zfsuper::objectCompare(anotherObj) != ZFCompareTheSame)
    {
        return ZFCompareUncomparable;
    }
    zfself *another = ZFCastZFObjectUnchecked(zfself *, anotherObj);
    if(zftrue
       && this->hasCanceledChildTask() == another->hasCanceledChildTask()
       && this->hasFailedChildTask() == another->hasFailedChildTask()
       && ZFObjectCompare(this->childResults(), another->childResults()) == ZFCompareTheSame
       && ZFObjectCompare(this->childResultsInOrder(), another->childResultsInOrder()) == ZFCompareTheSame
       )
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

// ============================================================
ZFOBJECT_REGISTER(ZFOperationQueueProgress)

// ============================================================

// ZFOperationQueueResult, stored in queue's ZFOperationTaskData
// used to store queue's result, to store necessary state while cancel queue
#define _ZFP_ZFOperationQueueKey_queueResult zfText("_ZFP_ZFOperationQueueKey_queueResult")

// ZFOperationQueueProgress, stored in queue's ZFOperationTaskData
// used to store queue's progress
#define _ZFP_ZFOperationQueueKey_queueProgress zfText("_ZFP_ZFOperationQueueKey_queueProgress")

// _ZFP_ZFOperationQueueTaskState, stored in queue's ZFOperationTaskData
// used to store child running state
#define _ZFP_ZFOperationQueueKey_taskState zfText("_ZFP_ZFOperationQueueKey_taskState")

// ZFOperationTaskData, stored in child's ZFOperationTaskData
// used to store child's owner queue
#define _ZFP_ZFOperationQueueKey_ownerQueue zfText("_ZFP_ZFOperationQueueKey_ownerQueue")

// ZFValue with index, stored in child's ZFOperationTaskData
// used to store child's index in it's owner queue
#define _ZFP_ZFOperationQueueKey_childIndex zfText("_ZFP_ZFOperationQueueKey_childIndex")

zfclassFwd _ZFP_ZFOperationQueueTaskState;
static void _ZFP_ZFOperationQueue_checkRunChild(ZF_IN ZFOperationTaskData *queueTaskData, ZF_IN _ZFP_ZFOperationQueueTaskState *taskState);
static void _ZFP_ZFOperationQueue_checkStopQueue(ZF_IN ZFOperationTaskData *queueTaskData, ZF_IN ZFOperationQueueResult *queueResult, ZF_IN _ZFP_ZFOperationQueueTaskState *taskState);

// ============================================================
zfclass _ZFP_ZFOperationQueueTaskState : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFOperationQueueTaskState, ZFObject)

public:
    ZFOperationQueue *ownerQueue;
    zfindex childToStartMaxFixed;
    ZFArrayEditable *childRunning; // ZFOperationQueueChildTaskData
    ZFArrayEditable *childToRun; // ZFOperationQueueChildTaskData
    ZFListener observerChildOnStart;
    ZFListener observerChildOnProgress;
    ZFListener observerChildOnStop;
    ZFValue *taskCategory;

public:
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->ownerQueue = zfnull;
        this->childToStartMaxFixed = 1;
        this->childRunning = zfAllocWithoutLeakTest(ZFArrayEditable);
        this->childToRun = zfAllocWithoutLeakTest(ZFArrayEditable);
        this->observerChildOnStart = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, childOnStart));
        this->observerChildOnProgress = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, childOnProgress));
        this->observerChildOnStop = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, childOnStop));
        this->taskCategory = zfRetain(ZFValue::identityValueCreate(zfidentityCalcPointer(this)).to<ZFValue *>());
        return this;
    }
    virtual void objectOnDealloc(void)
    {
        zfReleaseWithoutLeakTest(this->childRunning);
        this->childRunning = zfnull;
        zfReleaseWithoutLeakTest(this->childToRun);
        this->childToRun = zfnull;
        zfRelease(this->taskCategory);
        this->taskCategory = zfnull;
        zfsuper::objectOnDealloc();
    }

public:
    ZFLISTENER_DECLARE(childOnStart);
    ZFLISTENER_DECLARE(childOnProgress);
    ZFLISTENER_DECLARE(childOnStop);
};

// ============================================================
ZFOBJECT_REGISTER(ZFOperationQueue)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFOperationQueue, ZFOperationQueue, ZFOperationQueue, instance, ZFLevelZFFrameworkNormal)

zfbool ZFOperationQueue::paramIsValid(ZF_IN ZFOperationParam *operationParam)
{
    if(!zfsuper::paramIsValid(operationParam)
        || !operationParam->classData()->classIsSubclassOf(ZFOperationQueueParam::ClassData()))
    {
        return zffalse;
    }
    ZFArray *childTaskDatas = ZFCastZFObjectUnchecked(ZFOperationQueueParam *, operationParam)->childTaskDatas();
    for(zfindex i = 0; i < childTaskDatas->count(); ++i)
    {
        ZFOperationQueueChildTaskData *childTaskData = childTaskDatas->get<ZFOperationQueueChildTaskData *>(i);
        if(childTaskData->childOperation() == zfnull
            || childTaskData->childTaskData() == zfnull
            || childTaskData->childTaskData()->operationTaskData() == zfnull)
        {
            return zffalse;
        }
    }
    return zftrue;
}

void ZFOperationQueue::taskOnStart(ZF_IN ZFOperationTaskData *operationTaskData)
{
    zfsuper::taskOnStart(operationTaskData);

    ZFOperationQueueParam *queueParam = ZFCastZFObject(ZFOperationQueueParam *, operationTaskData->operationParam());

    zfautoObject queueResultTmp = this->createResult();
    ZFOperationQueueResult *queueResult = queueResultTmp.to<ZFOperationQueueResult *>();
    operationTaskData->tagSet(_ZFP_ZFOperationQueueKey_queueResult, queueResult);

    zfautoObject queueProgressTmp = this->createProgress();
    ZFOperationQueueProgress *queueProgress = queueProgressTmp.to<ZFOperationQueueProgress *>();
    operationTaskData->tagSet(_ZFP_ZFOperationQueueKey_queueProgress, queueProgress);

    _ZFP_ZFOperationQueueTaskState *taskState = zfAlloc(_ZFP_ZFOperationQueueTaskState);
    operationTaskData->tagSet(_ZFP_ZFOperationQueueKey_taskState, taskState);
    zfRelease(taskState);

    taskState->ownerQueue = this;
    taskState->childToRun->addFrom(queueParam->childTaskDatas());
    taskState->childToStartMaxFixed = queueParam->childToStartMax();
    if(taskState->childToStartMaxFixed <= 0 || taskState->childToStartMaxFixed > taskState->childToRun->count())
    {
        taskState->childToStartMaxFixed = taskState->childToRun->count();
    }

    for(zfindex i = taskState->childToRun->count() - 1; i != zfindexMax; --i)
    {
        queueResult->childResultsInOrder()->add(zfnullObject);
        queueProgress->childLastProgressDatas()->add(zfnullObject);
        taskState->childToRun->get<ZFOperationQueueChildTaskData *>(i)->childTaskData()->operationTaskData()->tagSet(
            _ZFP_ZFOperationQueueKey_childIndex,
            ZFValue::indexValueCreate(i).toObject());
    }

    _ZFP_ZFOperationQueue_checkRunChild(operationTaskData, taskState);
}
void ZFOperationQueue::taskOnStop(ZF_IN ZFOperationTaskData *operationTaskData)
{
    zfsuper::taskOnStop(operationTaskData);

    ZFOperationQueueResult *queueResult = operationTaskData->tagGet<ZFOperationQueueResult *>(_ZFP_ZFOperationQueueKey_queueResult);
    queueResult->resultTypeSet(operationTaskData->operationResult()->resultType());
    operationTaskData->operationResultSet(queueResult);

    switch(queueResult->resultType())
    {
        case ZFResultType::e_Success:
            break;
        case ZFResultType::e_Fail:
        case ZFResultType::e_Cancel:
        {
            _ZFP_ZFOperationQueueTaskState *taskState = operationTaskData->tagGet<_ZFP_ZFOperationQueueTaskState *>(_ZFP_ZFOperationQueueKey_taskState);
            zfblockedAllocWithoutLeakTest(ZFArrayEditable, childTaskDatasTmp);
            childTaskDatasTmp->addFrom(taskState->childRunning);
            childTaskDatasTmp->addFrom(taskState->childToRun);
            taskState->childRunning->removeAll();
            taskState->childToRun->removeAll();
            for(zfindex i = 0; i < childTaskDatasTmp->count(); ++i)
            {
                ZFOperationQueueChildTaskData *childTaskData = childTaskDatasTmp->get<ZFOperationQueueChildTaskData *>(i);
                ZFOperationTaskData *childOperationTaskData = childTaskData->childTaskData()->operationTaskData();
                zfblockedRelease(zfRetain(childOperationTaskData));
                childTaskData->childOperation()->taskStopForCategory(taskState->taskCategory);

                zfindex childIndex = childOperationTaskData->tagGet<ZFValue *>(_ZFP_ZFOperationQueueKey_childIndex)->indexValue();
                queueResult->childResults()->add(childOperationTaskData);
                queueResult->childResultsInOrder()->set(childIndex, childOperationTaskData);
            }
        }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

static void _ZFP_ZFOperationQueue_checkRunChild(ZF_IN ZFOperationTaskData *queueTaskData, ZF_IN _ZFP_ZFOperationQueueTaskState *taskState)
{
    while(taskState->childRunning->count() < taskState->childToStartMaxFixed && !taskState->childToRun->isEmpty())
    {
        ZFOperationQueueChildTaskData *childTaskData = taskState->childToRun->getFirst<ZFOperationQueueChildTaskData *>();
        taskState->childRunning->add(childTaskData);
        taskState->childToRun->removeFirst();

        childTaskData->childTaskData()->operationTaskData()->tagSet(_ZFP_ZFOperationQueueKey_ownerQueue, queueTaskData);
        childTaskData->childTaskData()->operationTaskData()->taskCategory()->add(taskState->taskCategory);

        ZFOperationObserver *childObserver = childTaskData->childTaskData()->operationTaskData()->operationObserver();
        if(childObserver == zfnull)
        {
            zfautoObject childObserverTmp = childTaskData->childOperation()->createObserver();
            childObserver = childObserverTmp.to<ZFOperationObserver *>();
            childTaskData->childTaskData()->operationTaskData()->operationObserverSet(childObserver);
        }

        childObserver->observerAdd(ZFOperation::EventOperationTaskOnStart(), taskState->observerChildOnStart);
        childObserver->observerAdd(ZFOperation::EventOperationTaskOnProgress(), taskState->observerChildOnProgress);
        childObserver->observerAdd(ZFOperation::EventOperationTaskOnStop(), taskState->observerChildOnStop);

        childTaskData->childOperation()->taskStart(childTaskData->childTaskData());
    }
}
static void _ZFP_ZFOperationQueue_checkStopQueue(ZF_IN ZFOperationTaskData *queueTaskData, ZF_IN ZFOperationQueueResult *queueResult, ZF_IN _ZFP_ZFOperationQueueTaskState *taskState)
{
    if(taskState->childRunning->isEmpty() && taskState->childToRun->isEmpty())
    {
        queueResult->resultTypeSet(ZFResultType::e_Success);
        queueTaskData->ownerOperation()->taskNotifyFinish(queueTaskData->operationId(), queueResult);
    }
}

static void _ZFP_ZFOperationQueue_updateQueueProgress(ZF_IN ZFOperationTaskData *queueTaskData,
                                                      ZF_IN ZFOperationQueueProgress *queueProgress,
                                                      ZF_IN ZFOperationTaskData *childOperationTaskData,
                                                      ZF_IN zfindex childIndex)
{
    queueProgress->childProgressDataSet(childOperationTaskData);

    queueProgress->childLastProgressDatas()->set(childIndex, childOperationTaskData);

    zfdouble totalTask = (zfdouble)queueProgress->childLastProgressDatas()->count();
    zfdouble curTask = 0;
    for(zfindex i = queueProgress->childLastProgressDatas()->count() - 1; i != zfindexMax; --i)
    {
        // zfnullObject or ZFOperationTaskData
        ZFOperationTaskData *childOperationTaskData = ZFCastZFObject(ZFOperationTaskData *, queueProgress->childLastProgressDatas()->get(i));
        if(childOperationTaskData != zfnull)
        {
            if(childOperationTaskData->operationProgress() != zfnull)
            {
                curTask += childOperationTaskData->operationProgress()->taskProgress();
            }
            else if(childOperationTaskData->operationResult() != zfnull)
            {
                curTask += 1;
            }
        }
    }
    if(curTask > totalTask)
    {
        curTask = totalTask;
    }
    if(totalTask > 0)
    {
        zffloat progressTmp = (zffloat)(curTask / totalTask);
        if(zffloatIsGreater(progressTmp, 1.0f))
        {
            progressTmp = 1;
        }
        queueProgress->taskProgressSet(progressTmp);
    }

    queueTaskData->ownerOperation()->taskNotifyProgress(queueTaskData->operationId(), queueProgress);
}
ZFLISTENER_DEFINE(_ZFP_ZFOperationQueueTaskState, childOnStart)
{
    ZFOperationTaskData *childOperationTaskData = ZFCastZFObjectUnchecked(ZFOperationTaskData *, listenerData.param0);
    ZFOperationTaskData *queueTaskData = childOperationTaskData->tagGet<ZFOperationTaskData *>(_ZFP_ZFOperationQueueKey_ownerQueue);
    ZFOperationQueueProgress *queueProgress = queueTaskData->tagGet<ZFOperationQueueProgress *>(_ZFP_ZFOperationQueueKey_queueProgress);
    zfindex childIndex = childOperationTaskData->tagGet<ZFValue *>(_ZFP_ZFOperationQueueKey_childIndex)->indexValue();
    _ZFP_ZFOperationQueue_updateQueueProgress(queueTaskData, queueProgress, childOperationTaskData, childIndex);
}
ZFLISTENER_DEFINE(_ZFP_ZFOperationQueueTaskState, childOnProgress)
{
    ZFOperationTaskData *childOperationTaskData = ZFCastZFObjectUnchecked(ZFOperationTaskData *, listenerData.param0);
    ZFOperationTaskData *queueTaskData = childOperationTaskData->tagGet<ZFOperationTaskData *>(_ZFP_ZFOperationQueueKey_ownerQueue);
    ZFOperationQueueProgress *queueProgress = queueTaskData->tagGet<ZFOperationQueueProgress *>(_ZFP_ZFOperationQueueKey_queueProgress);
    zfindex childIndex = childOperationTaskData->tagGet<ZFValue *>(_ZFP_ZFOperationQueueKey_childIndex)->indexValue();
    _ZFP_ZFOperationQueue_updateQueueProgress(queueTaskData, queueProgress, childOperationTaskData, childIndex);
}
ZFLISTENER_DEFINE(_ZFP_ZFOperationQueueTaskState, childOnStop)
{
    ZFOperationTaskData *childOperationTaskData = ZFCastZFObjectUnchecked(ZFOperationTaskData *, listenerData.param0);
    ZFOperationTaskData *queueTaskData = childOperationTaskData->tagGet<ZFOperationTaskData *>(_ZFP_ZFOperationQueueKey_ownerQueue);
    ZFOperationQueueProgress *queueProgress = queueTaskData->tagGet<ZFOperationQueueProgress *>(_ZFP_ZFOperationQueueKey_queueProgress);
    zfindex childIndex = childOperationTaskData->tagGet<ZFValue *>(_ZFP_ZFOperationQueueKey_childIndex)->indexValue();
    _ZFP_ZFOperationQueue_updateQueueProgress(queueTaskData, queueProgress, childOperationTaskData, childIndex);

    _ZFP_ZFOperationQueueTaskState *taskState = queueTaskData->tagGet<_ZFP_ZFOperationQueueTaskState *>(_ZFP_ZFOperationQueueKey_taskState);
    ZFOperationQueueResult *queueResult = queueTaskData->tagGet<ZFOperationQueueResult *>(_ZFP_ZFOperationQueueKey_queueResult);
    if(!taskState->childToRun->isEmpty() || !taskState->childRunning->isEmpty())
    {
        ZFOperationQueueChildTaskData *childTaskData = ZFCastZFObjectUnchecked(ZFOperationQueueParam *, queueTaskData->operationParam())->childTaskDatas()->get<ZFOperationQueueChildTaskData *>(childIndex);
        taskState->childRunning->removeElement(childTaskData);
        queueResult->childResults()->add(childOperationTaskData);
        queueResult->childResultsInOrder()->set(childIndex, childOperationTaskData);

        switch(childOperationTaskData->operationResult()->resultType())
        {
            case ZFResultType::e_Success:
                _ZFP_ZFOperationQueue_checkRunChild(queueTaskData, taskState);
                _ZFP_ZFOperationQueue_checkStopQueue(queueTaskData, queueResult, taskState);
                break;
            case ZFResultType::e_Fail:
                queueResult->hasFailedChildTaskSet(zftrue);
                if(childTaskData->stopQueueIfFail())
                {
                    queueResult->resultTypeSet(ZFResultType::e_Fail);
                    this->ownerQueue->taskNotifyFinish(queueTaskData->operationId(), queueResult);
                }
                else
                {
                    _ZFP_ZFOperationQueue_checkRunChild(queueTaskData, taskState);
                    _ZFP_ZFOperationQueue_checkStopQueue(queueTaskData, queueResult, taskState);
                }
                break;
            case ZFResultType::e_Cancel:
                queueResult->hasCanceledChildTaskSet(zftrue);
                if(childTaskData->stopQueueIfCancel())
                {
                    queueResult->resultTypeSet(ZFResultType::e_Cancel);
                    this->ownerQueue->taskNotifyFinish(queueTaskData->operationId(), queueResult);
                }
                else
                {
                    _ZFP_ZFOperationQueue_checkRunChild(queueTaskData, taskState);
                    _ZFP_ZFOperationQueue_checkStopQueue(queueTaskData, queueResult, taskState);
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

