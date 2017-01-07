/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFThread.h"
#include "protocol/ZFProtocolZFThread.h"
#include "protocol/ZFProtocolZFThreadTaskRequest.h"
#include "ZFAutoReleasePool.h"
#include "ZFSemaphore.h"
#include "ZFMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFThreadImpl ZFPROTOCOL_ACCESS(ZFThread)

zfclass _ZFP_I_ZFThreadMutex : zfextends ZFMutex
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFThreadMutex, ZFMutex)
};
ZFMutex *_ZFP_ZFThread_mutex = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadMutexHolder, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFThread_mutex = zfAllocWithoutLeakTest(_ZFP_I_ZFThreadMutex);
    _ZFP_ZFThread_mutex->objectDebugInfoSet(zfText("ZFThread internal mutex"));
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadMutexHolder)
{
    zfReleaseWithoutLeakTest(_ZFP_ZFThread_mutex);
    _ZFP_ZFThread_mutex = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFThreadMutexHolder)

// ============================================================
// _ZFP_ZFThreadPrivate
zfclassNotPOD _ZFP_ZFThreadPrivate
{
public:
    ZFAutoReleasePool *autoReleasePool;
    zfbool autoReleasePoolNeedDrain;
    /**
     * no auto-retain, used in ZFThread::threadWait,
     * same as runnable data's semaWait for run task
     */
    ZFSemaphore *semaWaitHolder;
    zfidentity taskId;
    zfbool startFlag;
    zfbool runningFlag;
    zfbool stopRequestedFlag;

public:
    _ZFP_ZFThreadPrivate(void)
    : autoReleasePool(zfnull)
    , autoReleasePoolNeedDrain(zffalse)
    , semaWaitHolder(zfnull)
    , taskId(zfidentityInvalid)
    , startFlag(zffalse)
    , runningFlag(zffalse)
    , stopRequestedFlag(zffalse)
    {
    }
    ~_ZFP_ZFThreadPrivate(void)
    {
        zfRelease(this->autoReleasePool);
    }
};

// ============================================================
// runnable data
typedef enum {
    _ZFP_ZFThreadRunnableTypeExecuteInMainThread,
    _ZFP_ZFThreadRunnableTypeExecuteInNewThread,
    _ZFP_ZFThreadRunnableTypeExecuteInMainThreadAfterDelay,
} _ZFP_ZFThreadRunnableType;
typedef enum {
    _ZFP_ZFThreadRunStatePending,
    _ZFP_ZFThreadRunStateRunning,
    _ZFP_ZFThreadRunStateFinished,
} _ZFP_ZFThreadRunState;
zfclass _ZFP_I_ZFThreadRunnableData : zfextends ZFObject
{
    ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(_ZFP_I_ZFThreadRunnableData, ZFObject)

public:
    zfidentity taskId;
    _ZFP_ZFThreadRunnableType runnableType;
    void *nativeToken;
    ZFListener _runnable;
    ZFThread *ownerZFThread; // no auto-retain, null if main thread
    _ZFP_ZFThreadPrivate *ownerZFThreadPrivate; // null if main thread
    ZFObject *userData; // auto-retain
    ZFObject *param0; // no auto-retain
    ZFObject *param1; // no auto-retain
    ZFSemaphore *semaWait; // not null, no auto-retain, used int ZFThreadExecuteWait and waitUntilDone, also used to notify task observer
    _ZFP_ZFThreadRunState runState;

public:
    inline void runnableSet(ZF_IN const ZFListener &runnable)
    {
        zfassert(runnable.callbackIsValid());
        runnable.callbackOwnerObjectRetain();
        this->_runnable.callbackOwnerObjectRelease();
        this->_runnable = runnable;
    }
    inline ZFListener &runnable(void)
    {
        return this->_runnable;
    }

public:
    virtual void objectOnDeallocPrepare(void)
    {
        this->_runnable.callbackOwnerObjectRelease();
        zfsuper::objectOnDeallocPrepare();
    }

protected:
    _ZFP_I_ZFThreadRunnableData(void)
    : taskId(zfidentityInvalid)
    , runnableType(_ZFP_ZFThreadRunnableTypeExecuteInMainThread)
    , nativeToken(zfnull)
    , _runnable()
    , ownerZFThread(zfnull)
    , ownerZFThreadPrivate(zfnull)
    , userData(zfnull)
    , param0(zfnull)
    , param1(zfnull)
    , semaWait(zfnull)
    , runState(_ZFP_ZFThreadRunStatePending)
    {
    }
};

// ============================================================
// data holder
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadDataHolder, ZFLevelZFFrameworkEssential)
{
}
public:
    ZFCoreArrayPOD<_ZFP_I_ZFThreadRunnableData *> runnableList;
    ZFIdentityGenerator idGenerator;
ZF_GLOBAL_INITIALIZER_END(ZFThreadDataHolder)
#define _ZFP_ZFThread_runnableList (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadDataHolder)->runnableList)
#define _ZFP_ZFThread_idGenerator (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadDataHolder)->idGenerator)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadExecute_AutoCancel, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadExecute_AutoCancel)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }
    ZFCoreArrayPOD<zfidentity> allTaskId;
    const ZFCoreArrayPOD<_ZFP_I_ZFThreadRunnableData *> &runnableList = _ZFP_ZFThread_runnableList;
    for(zfindex i = 0; i < runnableList.count(); ++i)
    {
        allTaskId.add(runnableList[i]->taskId);
    }
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < allTaskId.count(); ++i)
    {
        ZFThreadExecuteCancel(allTaskId[i]);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFThreadExecute_AutoCancel)

// ============================================================
static void _ZFP_ZFThreadRunnableCleanup(ZF_IN _ZFP_I_ZFThreadRunnableData *runnableData);
static zfidentity _ZFP_ZFThreadExecuteInNewThread(ZF_IN const ZFListener &runnable,
                                                  ZF_IN ZFObject *userData,
                                                  ZF_IN ZFObject *param0,
                                                  ZF_IN ZFObject *param1,
                                                  ZF_IN ZFThread *ownerZFThread,
                                                  ZF_IN _ZFP_ZFThreadPrivate *ownerZFThreadPrivate);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadCallback)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    _ZFP_I_ZFThreadRunnableData *runnableData = ZFCastZFObjectUnchecked(_ZFP_I_ZFThreadRunnableData *, listenerData.param0);
    ZFThread *ownerZFThreadFixed = ((runnableData->ownerZFThread == zfnull) ? ZFThread::currentThread() : runnableData->ownerZFThread);

    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }
    if(runnableData->runState == _ZFP_ZFThreadRunStatePending)
    {
        runnableData->runState = _ZFP_ZFThreadRunStateRunning;

        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }

        if(runnableData->ownerZFThreadPrivate != zfnull)
        {
            runnableData->ownerZFThreadPrivate->runningFlag = zftrue;
        }

        // start
        if(runnableData->ownerZFThread != zfnull)
        {
            runnableData->ownerZFThread->_ZFP_ZFThread_threadOnStart(runnableData->param0, runnableData->param1);
        }
        runnableData->semaWait->observerNotifyWithCustomSender(ownerZFThreadFixed, ZFThread::EventThreadOnStart(), runnableData->param0, runnableData->param1);

        // running
        runnableData->runnable().execute(ZFListenerData(zfidentityInvalid, ownerZFThreadFixed, runnableData->param0, runnableData->param1), runnableData->userData);

        // stop
        if(runnableData->ownerZFThread != zfnull)
        {
            runnableData->ownerZFThread->_ZFP_ZFThread_threadOnStop(runnableData->param0, runnableData->param1);
        }
        runnableData->semaWait->observerNotifyWithCustomSender(ownerZFThreadFixed, ZFThread::EventThreadOnStop(), runnableData->param0, runnableData->param1);

        if(lockAvailable)
        {
            zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
        }

        runnableData->runState = _ZFP_ZFThreadRunStateFinished;
    }

    if(runnableData->ownerZFThreadPrivate != zfnull)
    {
        runnableData->ownerZFThreadPrivate->stopRequestedFlag = zffalse;
    }
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }

    _ZFP_ZFThreadRunnableCleanup(runnableData);
}
static void _ZFP_ZFThreadRunnableCleanup(ZF_IN _ZFP_I_ZFThreadRunnableData *runnableData)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }

    {
        zfbool exist = zffalse;
        ZFCoreArrayPOD<_ZFP_I_ZFThreadRunnableData *> &runnableList = _ZFP_ZFThread_runnableList;
        for(zfindex i = 0; i < runnableList.count(); ++i)
        {
            if(runnableList.get(i) == runnableData)
            {
                runnableList.remove(i);
                exist = zftrue;
                break;
            }
        }
        if(!exist)
        {
            if(lockAvailable)
            {
                zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
            }
            return ;
        }
    }

    zfidentity taskIdSaved = runnableData->taskId;

    runnableData->semaWait->observerRemoveAll(ZFThread::EventThreadOnStart());
    runnableData->semaWait->observerRemoveAll(ZFThread::EventThreadOnStop());
    runnableData->semaWait->observerRemoveAll(ZFThread::EventThreadOnCancel());

    if(runnableData->ownerZFThread != zfnull)
    {
        runnableData->ownerZFThread->autoReleasePoolDrain();
    }
    if(runnableData->ownerZFThreadPrivate != zfnull)
    {
        runnableData->ownerZFThreadPrivate->startFlag = zffalse;
        runnableData->ownerZFThreadPrivate->runningFlag = zffalse;
        runnableData->ownerZFThreadPrivate->semaWaitHolder = zfnull;
    }
    switch(runnableData->runnableType)
    {
        case _ZFP_ZFThreadRunnableTypeExecuteInMainThread:
            _ZFP_ZFThreadImpl->executeInMainThreadCleanup(runnableData->taskId, runnableData->nativeToken);
            break;
        case _ZFP_ZFThreadRunnableTypeExecuteInNewThread:
            _ZFP_ZFThreadImpl->executeInNewThreadCleanup(runnableData->taskId, runnableData->nativeToken);
            break;
        case _ZFP_ZFThreadRunnableTypeExecuteInMainThreadAfterDelay:
            _ZFP_ZFThreadImpl->executeInMainThreadAfterDelayCleanup(runnableData->taskId, runnableData->nativeToken);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
    zfRelease(runnableData->ownerZFThread);
    zfRelease(runnableData->param0);
    zfRelease(runnableData->param1);
    zfRelease(runnableData->userData);

    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }

    runnableData->semaWait->semaphoreBroadcastLocked();
    zfReleaseWithoutLeakTest(runnableData->semaWait);
    runnableData->semaWait = zfnull;
    zfReleaseWithoutLeakTest(runnableData);

    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }
    _ZFP_ZFThread_idGenerator.markUnused(taskIdSaved);
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }
}

// ============================================================
// user registered thread
zfclass _ZFP_ZFThreadUserRegisteredThread : zfextends ZFThread
{
    ZFOBJECT_DECLARE(_ZFP_ZFThreadUserRegisteredThread, ZFThread)

public:
    zfoverride
    virtual void threadStart(ZF_IN_OPT ZFObject *userData = zfnull,
                             ZF_IN_OPT ZFObject *param0 = zfnull,
                             ZF_IN_OPT ZFObject *param1 = zfnull)
    {
        zfCoreLogTrim(zfTextA("you must not start a user registered thread"));
    }
    zfoverride
    virtual zfbool threadStarted(void)
    {
        return zftrue;
    }
    zfoverride
    virtual zfbool threadRunning(void)
    {
        return zftrue;
    }
    zfoverride
    virtual void threadStop(void)
    {
        zfCoreLogTrim(zfTextA("you must not stop a user registered thread"));
    }
};

// ============================================================
// ZFThread
ZFOBJECT_REGISTER(ZFThread)

ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadOnCancel)

void *ZFThread::nativeThreadRegister(void)
{
    ZFThread *zfThread = zfAllocWithoutLeakTest(_ZFP_ZFThreadUserRegisteredThread);
    zfThread->_ZFP_ZFThread_d->semaWaitHolder = zfAllocWithoutLeakTest(ZFSemaphore);
    return _ZFP_ZFThreadImpl->nativeThreadRegister(zfThread);
}
void ZFThread::nativeThreadUnregister(ZF_IN void *token)
{
    if(token != zfnull)
    {
        ZFThread *zfThread = ZFPROTOCOL_ACCESS(ZFThread)->threadForToken(token);
        zfCoreAssert(zfThread != zfnull);
        zfThread->_ZFP_ZFThread_d->semaWaitHolder->semaphoreBroadcastLocked();
        _ZFP_ZFThreadImpl->nativeThreadUnregister(token);
        zfReleaseWithoutLeakTest(zfThread->_ZFP_ZFThread_d->semaWaitHolder);
        zfThread->_ZFP_ZFThread_d->semaWaitHolder = zfnull;
        zfReleaseWithoutLeakTest(zfThread);
    }
}

ZFThread *ZFThread::mainThread(void)
{
    return _ZFP_ZFThreadImpl->mainThread();
}
ZFThread *ZFThread::currentThread(void)
{
    return _ZFP_ZFThreadImpl->currentThread();
}

void ZFThread::sleep(ZF_IN const zftimet &miliSecs)
{
    _ZFP_ZFThreadImpl->sleep(miliSecs);
}

// ============================================================
// zfautoRelease
static ZFObject *_ZFP_ZFThread_drainPoolCallbackMethod(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    ZFThread *thread = ZFCastZFObjectUnchecked(ZFThread *, userData);
    thread->autoReleasePoolDrain();
    thread->_ZFP_ZFThreadAutoReleasePoolMarkResolved();
    return zfnull;
}
static ZFListener *_ZFP_ZFThread_drainPoolCallback = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadAutoReleasePoolDrainDataHolder, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFThread_drainPoolCallback = zfnew(ZFListener);
    *_ZFP_ZFThread_drainPoolCallback = ZFCallbackForRawFunction(_ZFP_ZFThread_drainPoolCallbackMethod);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadAutoReleasePoolDrainDataHolder)
{
    zfdelete(_ZFP_ZFThread_drainPoolCallback);
    _ZFP_ZFThread_drainPoolCallback = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFThreadAutoReleasePoolDrainDataHolder)

// ============================================================
// thread instance method
ZFObject *ZFThread::objectOnInit(ZF_IN const ZFListener &runnable)
{
    this->objectOnInit();
    zfself::threadRunnableSet(runnable);
    return this;
}
ZFObject *ZFThread::objectOnInit(void)
{
    zfsuper::objectOnInit();
    _ZFP_ZFThread_d = zfpoolNew(_ZFP_ZFThreadPrivate);
    _ZFP_ZFThread_d->autoReleasePool = zfAllocWithoutLeakTest(ZFAutoReleasePool);
    return this;
}
void ZFThread::objectOnDealloc(void)
{
    zfpoolDelete(_ZFP_ZFThread_d);
    _ZFP_ZFThread_d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFThread::objectOnDeallocPrepare(void)
{
    ZFThreadTaskCancelWithOwner(this);

    if(!this->isMainThread())
    {
        this->threadWait();
    }
    this->autoReleasePoolDrain();
    zfsuper::objectOnDeallocPrepare();
}

void ZFThread::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    if(this->isMainThread())
    {
        ret += zfText(" MainThread");
    }
}

void ZFThread::threadStart(ZF_IN_OPT ZFObject *userData /* = zfnull */,
                           ZF_IN_OPT ZFObject *param0 /* = zfnull */,
                           ZF_IN_OPT ZFObject *param1 /* = zfnull */)
{
    zfidentity taskIdTmp = _ZFP_ZFThreadExecuteInNewThread(
        this->threadRunnable().callbackIsValid()
            ? this->threadRunnable()
            : ZFListener(ZFCallbackForMemberMethod(this, ZFMethodAccess(ZFThread, threadOnRun))),
        userData,
        param0,
        param1,
        this,
        _ZFP_ZFThread_d);
    if(zfidentityIsValid(taskIdTmp))
    {
        _ZFP_ZFThread_d->taskId = taskIdTmp;
    }
}
zfbool ZFThread::threadStarted(void)
{
    return _ZFP_ZFThread_d->startFlag;
}
zfbool ZFThread::threadRunning(void)
{
    return _ZFP_ZFThread_d->runningFlag;
}
void ZFThread::threadStop(void)
{
    if(_ZFP_ZFThread_d->startFlag)
    {
        _ZFP_ZFThread_d->stopRequestedFlag = zftrue;
        ZFThreadExecuteCancel(_ZFP_ZFThread_d->taskId);
    }
}
zfbool ZFThread::threadStopRequested(void)
{
    return _ZFP_ZFThread_d->stopRequestedFlag;
}

void ZFThread::threadWait(void)
{
    if(_ZFP_ZFThread_d->semaWaitHolder != zfnull)
    {
        _ZFP_ZFThread_d->semaWaitHolder->semaphoreWaitLocked();
    }
}
zfbool ZFThread::threadWait(ZF_IN const zftimet &miliSecs)
{
    if(_ZFP_ZFThread_d->semaWaitHolder != zfnull)
    {
        return _ZFP_ZFThread_d->semaWaitHolder->semaphoreWaitLocked(miliSecs);
    }
    return zftrue;
}

zfbool ZFThread::isMainThread(void)
{
    return zffalse;
}

void ZFThread::autoReleasePoolAdd(ZF_IN ZFObject *obj,
                                  ZF_IN_OPT zfbool enableLeakTest /* = zftrue */)
{
    _ZFP_ZFThread_d->autoReleasePool->poolAdd(obj, enableLeakTest);
}

void ZFThread::autoReleasePoolAdd(ZF_IN ZFObject *obj,
                                  ZF_IN const zfcharA *callerFile,
                                  ZF_IN const zfcharA *callerFunction,
                                  ZF_IN zfindex callerLine,
                                  ZF_IN_OPT zfbool enableLeakTest /* = zftrue */)
{
    _ZFP_ZFThread_d->autoReleasePool->poolAdd(obj, callerFile, callerFunction, callerLine, enableLeakTest);
}

void ZFThread::autoReleasePoolDrain(void)
{
    _ZFP_ZFThread_d->autoReleasePool->poolDrain();
}

void ZFThread::_ZFP_ZFThreadAutoReleasePoolAdd(ZF_IN ZFObject *obj,
                                               ZF_IN const zfcharA *callerFile,
                                               ZF_IN const zfcharA *callerFunction,
                                               ZF_IN zfindex callerLine,
                                               ZF_IN zfbool enableLeakTest)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }

    this->autoReleasePoolAdd(obj, callerFile, callerFunction, callerLine, enableLeakTest);
    if(!_ZFP_ZFThread_d->autoReleasePoolNeedDrain)
    {
        _ZFP_ZFThread_d->autoReleasePoolNeedDrain = zftrue;
        ZFThreadTaskRequest(*_ZFP_ZFThread_drainPoolCallback, this);
    }

    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }
}
void ZFThread::_ZFP_ZFThreadAutoReleasePoolMarkResolved(void)
{
    _ZFP_ZFThread_d->autoReleasePoolNeedDrain = zffalse;
}

ZFMETHOD_DEFINE_DETAIL_2(ZFThread, ZFMethodNotConst,
                         void, threadOnRun,
                         const ZFListenerData &, listenerData,
                         ZFObject *, userData)
{
    // nothing to do
}

// ============================================================
// thread execute
zfidentity ZFThreadExecuteInMainThread(ZF_IN const ZFListener &runnable,
                                       ZF_IN_OPT ZFObject *userData /* = zfnull */,
                                       ZF_IN_OPT ZFObject *param0 /* = zfnull */,
                                       ZF_IN_OPT ZFObject *param1 /* = zfnull */,
                                       ZF_IN_OPT zfbool waitUntilDone /* = zffalse */)
{
    if(!runnable.callbackIsValid())
    {
        return zfidentityInvalid;
    }
    ZFThread *curThread = ZFThread::currentThread();
    if(curThread != zfnull && curThread->isMainThread() && waitUntilDone)
    {
        runnable.execute(ZFListenerData(zfidentityInvalid, curThread, param0, param1), userData);
        return zfidentityInvalid;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }
    _ZFP_I_ZFThreadRunnableData *runnableData = zfAllocWithoutLeakTest(_ZFP_I_ZFThreadRunnableData);
    zfidentity taskId = _ZFP_ZFThread_idGenerator.nextMarkUsed();
    runnableData->taskId = taskId;
    runnableData->runnableType = _ZFP_ZFThreadRunnableTypeExecuteInMainThread;
    runnableData->runnableSet(runnable);
    runnableData->param0 = zfRetain(param0);
    runnableData->param1 = zfRetain(param1);
    runnableData->userData = zfRetain(userData);
    runnableData->semaWait = zfAllocWithoutLeakTest(ZFSemaphore);
    _ZFP_ZFThread_runnableList.add(runnableData);
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }

    zfRetainWithoutLeakTest(runnableData);
    zfRetainWithoutLeakTest(runnableData->semaWait);
    runnableData->nativeToken = _ZFP_ZFThreadImpl->executeInMainThread(
        taskId,
        ZFCallbackForRawFunction(_ZFP_ZFThreadCallback),
        runnableData,
        zfnull);

    if(waitUntilDone)
    {
        runnableData->semaWait->semaphoreWaitLocked();
    }
    zfReleaseWithoutLeakTest(runnableData->semaWait);
    zfReleaseWithoutLeakTest(runnableData);

    return taskId;
}

static zfidentity _ZFP_ZFThreadExecuteInNewThread(ZF_IN const ZFListener &runnable,
                                                  ZF_IN ZFObject *userData,
                                                  ZF_IN ZFObject *param0,
                                                  ZF_IN ZFObject *param1,
                                                  ZF_IN ZFThread *ownerZFThread,
                                                  ZF_IN _ZFP_ZFThreadPrivate *ownerZFThreadPrivate)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }
    if(ownerZFThreadPrivate->startFlag)
    {
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
        return zfidentityInvalid;
    }
    ownerZFThreadPrivate->startFlag = zftrue;

    _ZFP_I_ZFThreadRunnableData *runnableData = zfAllocWithoutLeakTest(_ZFP_I_ZFThreadRunnableData);
    zfidentity taskId = _ZFP_ZFThread_idGenerator.nextMarkUsed();
    runnableData->taskId = taskId;
    runnableData->runnableType = _ZFP_ZFThreadRunnableTypeExecuteInNewThread;
    runnableData->runnableSet(runnable);
    runnableData->ownerZFThread = zfRetain(ownerZFThread);
    runnableData->ownerZFThreadPrivate = ownerZFThreadPrivate;
    runnableData->param0 = zfRetain(param0);
    runnableData->param1 = zfRetain(param1);
    runnableData->userData = zfRetain(userData);
    runnableData->semaWait = zfAllocWithoutLeakTest(ZFSemaphore);
    if(runnableData->ownerZFThreadPrivate != zfnull)
    {
        runnableData->ownerZFThreadPrivate->semaWaitHolder = runnableData->semaWait;
    }
    _ZFP_ZFThread_runnableList.add(runnableData);
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }
    runnableData->nativeToken = _ZFP_ZFThreadImpl->executeInNewThread(
        taskId,
        ownerZFThread,
        ZFCallbackForRawFunction(_ZFP_ZFThreadCallback),
        runnableData,
        zfnull);
    return taskId;
}
zfidentity ZFThreadExecuteInNewThread(ZF_IN const ZFListener &runnable,
                                      ZF_IN_OPT ZFObject *userData /* = zfnull */,
                                      ZF_IN_OPT ZFObject *param0 /* = zfnull */,
                                      ZF_IN_OPT ZFObject *param1 /* = zfnull */)
{
    if(!runnable.callbackIsValid())
    {
        return zfidentityInvalid;
    }

    ZFThread *tmpThread = zfAlloc(ZFThread);
    tmpThread->threadRunnableSet(runnable);
    zfidentity taskId = _ZFP_ZFThreadExecuteInNewThread(
        runnable,
        userData,
        param0,
        param1,
        tmpThread,
        tmpThread->_ZFP_ZFThread_d);
    zfRelease(tmpThread);
    return taskId;
}

zfidentity ZFThreadExecuteInMainThreadAfterDelay(ZF_IN zftimet delay,
                                                 ZF_IN const ZFListener &runnable,
                                                 ZF_IN_OPT ZFObject *userData /* = zfnull */,
                                                 ZF_IN_OPT ZFObject *param0 /* = zfnull */,
                                                 ZF_IN_OPT ZFObject *param1 /* = zfnull */)
{
    if(delay <= 0)
    {
        return ZFThreadExecuteInMainThread(runnable, userData, param0, param1);
    }
    if(!runnable.callbackIsValid())
    {
        return zfidentityInvalid;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }
    _ZFP_I_ZFThreadRunnableData *runnableData = zfAllocWithoutLeakTest(_ZFP_I_ZFThreadRunnableData);
    zfidentity taskId = _ZFP_ZFThread_idGenerator.nextMarkUsed();
    runnableData->taskId = taskId;
    runnableData->runnableType = _ZFP_ZFThreadRunnableTypeExecuteInMainThreadAfterDelay;
    runnableData->runnableSet(runnable);
    runnableData->param0 = zfRetain(param0);
    runnableData->param1 = zfRetain(param1);
    runnableData->userData = zfRetain(userData);
    runnableData->semaWait = zfAllocWithoutLeakTest(ZFSemaphore);
    _ZFP_ZFThread_runnableList.add(runnableData);
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }

    zfRetainWithoutLeakTest(runnableData);
    zfRetainWithoutLeakTest(runnableData->semaWait);
    runnableData->nativeToken = _ZFP_ZFThreadImpl->executeInMainThreadAfterDelay(
        taskId,
        delay,
        ZFCallbackForRawFunction(_ZFP_ZFThreadCallback),
        runnableData,
        zfnull);
    zfReleaseWithoutLeakTest(runnableData->semaWait);
    zfReleaseWithoutLeakTest(runnableData);
    return taskId;
}

static void _ZFP_ZFThreadDoCancelTask(ZF_IN _ZFP_I_ZFThreadRunnableData *runnableData)
{
    if(runnableData->runState != _ZFP_ZFThreadRunStatePending)
    {
        return ;
    }
    runnableData->runState = _ZFP_ZFThreadRunStateFinished;
    switch(runnableData->runnableType)
    {
        case _ZFP_ZFThreadRunnableTypeExecuteInMainThread:
            _ZFP_ZFThreadImpl->executeInMainThreadCancel(runnableData->taskId, runnableData->nativeToken);
            break;
        case _ZFP_ZFThreadRunnableTypeExecuteInNewThread:
            _ZFP_ZFThreadImpl->executeInNewThreadCancel(runnableData->taskId, runnableData->nativeToken);
            break;
        case _ZFP_ZFThreadRunnableTypeExecuteInMainThreadAfterDelay:
            _ZFP_ZFThreadImpl->executeInMainThreadAfterDelayCancel(runnableData->taskId, runnableData->nativeToken);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
    if(runnableData->ownerZFThreadPrivate != zfnull)
    {
        if(runnableData->ownerZFThreadPrivate->startFlag)
        {
            runnableData->ownerZFThreadPrivate->stopRequestedFlag = zftrue;
        }
    }
    if(runnableData->ownerZFThread != zfnull)
    {
        runnableData->ownerZFThread->_ZFP_ZFThread_threadOnCancel(runnableData->param0, runnableData->param1);
    }
    runnableData->semaWait->observerNotifyWithCustomSender(
        (runnableData->ownerZFThread == zfnull) ? ZFThread::currentThread() : runnableData->ownerZFThread,
        ZFThread::EventThreadOnCancel(), runnableData->param0, runnableData->param1);

    _ZFP_ZFThreadRunnableCleanup(runnableData);
}
void ZFThreadExecuteCancel(ZF_IN zfidentity taskId)
{
    if(zfidentityIsValid(taskId))
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->taskId == taskId)
            {
                _ZFP_ZFThreadDoCancelTask(runnableData);
                break;
            }
        }
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
    }
}
void ZFThreadExecuteCancel(ZF_IN const ZFListener &runnable,
                           ZF_IN_OPT ZFObject *userData /* = zfnull */,
                           ZF_IN_OPT ZFObject *param0 /* = zfnull */,
                           ZF_IN_OPT ZFObject *param1 /* = zfnull */)
{
    if(runnable.callbackIsValid())
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->ownerZFThread->threadRunnable().objectCompare(runnable) == ZFCompareTheSame
                && ZFObjectCompare(runnableData->userData, userData) == ZFCompareTheSame
                && ZFObjectCompare(runnableData->param0, param0) == ZFCompareTheSame
                && ZFObjectCompare(runnableData->param1, param1) == ZFCompareTheSame)
            {
                _ZFP_ZFThreadDoCancelTask(runnableData);
                break;
            }
        }
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
    }
}
void ZFThreadExecuteCancelByTarget(ZF_IN const ZFListener &runnable)
{
    if(runnable.callbackIsValid())
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->ownerZFThread->threadRunnable().objectCompare(runnable) == ZFCompareTheSame)
            {
                _ZFP_ZFThreadDoCancelTask(runnableData);
            }
        }
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
    }
}

void ZFThreadExecuteWait(ZF_IN zfidentity taskId)
{
    if(zfidentityIsValid(taskId))
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->taskId == taskId)
            {
                if(runnableData->ownerZFThread != zfnull && !runnableData->ownerZFThread->isMainThread())
                {
                    if(lockAvailable)
                    {
                        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
                    }
                    runnableData->semaWait->semaphoreWaitLocked();
                    return ;
                }
                else
                {
                    if(lockAvailable)
                    {
                        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
                    }
                    return ;
                }
            }
        }
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
    }
}
zfbool ZFThreadExecuteWait(ZF_IN zfidentity taskId,
                           ZF_IN const zftimet &miliSecs)
{
    if(zfidentityIsValid(taskId))
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->taskId == taskId)
            {
                if(runnableData->ownerZFThread != zfnull && !runnableData->ownerZFThread->isMainThread())
                {
                    if(lockAvailable)
                    {
                        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
                    }
                    return runnableData->semaWait->semaphoreWaitLocked(miliSecs);
                }
                else
                {
                    if(lockAvailable)
                    {
                        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
                    }
                    return zffalse;
                }
            }
        }
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
    }
    return zffalse;
}

void ZFThreadExecuteObserverAdd(ZF_IN zfidentity taskId,
                                ZF_IN const zfidentity &eventId,
                                ZF_IN const ZFListener &callback)
{
    if(eventId != ZFThread::EventThreadOnStart()
        && eventId != ZFThread::EventThreadOnStop()
        && eventId != ZFThread::EventThreadOnCancel())
    {
        zfCoreCriticalMessage(zfTextA("thread task can only add ZFThread's observer event, event: %s"),
            ZFObserverEventGetName(eventId));
        return ;
    }
    if(zfidentityIsValid(taskId))
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->taskId == taskId)
            {
                runnableData->semaWait->observerAdd(eventId, callback);
                break;
            }
        }
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
    }
}
void ZFThreadExecuteObserverRemove(ZF_IN zfidentity taskId,
                                   ZF_IN const zfidentity &eventId,
                                   ZF_IN const ZFListener &callback)
{
    if(eventId != ZFThread::EventThreadOnStart()
        && eventId != ZFThread::EventThreadOnStop()
        && eventId != ZFThread::EventThreadOnCancel())
    {
        zfCoreCriticalMessage(zfTextA("thread task can only add ZFThread's observer event, event: %s"),
            ZFObserverEventGetName(eventId));
        return ;
    }
    if(zfidentityIsValid(taskId))
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->taskId == taskId)
            {
                runnableData->semaWait->observerRemove(eventId, callback);
                break;
            }
        }
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

