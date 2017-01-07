/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFThread_zfautoRelease.h"
#include "ZFAutoReleasePool.h"
#include "protocol/ZFProtocolZFThread.h"
#include "protocol/ZFProtocolZFThreadTaskRequest.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfautoRelease
static ZFObject *_ZFP_zfautoRelease_poolDrain(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData);
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfautoRelease_poolDrainDataHolder, ZFLevelZFFrameworkEssential)
{
    this->drainTask = ZFCallbackForRawFunction(_ZFP_zfautoRelease_poolDrain);
    this->drainTaskRequested = zffalse;
}
public:
    ZFListener drainTask;
    zfbool drainTaskRequested;
ZF_GLOBAL_INITIALIZER_END(zfautoRelease_poolDrainDataHolder)
static ZFObject *_ZFP_zfautoRelease_poolDrain(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(zfautoRelease_poolDrainDataHolder)->drainTaskRequested = zffalse;
    ZFAutoReleasePool::instance()->poolDrain();
    return zfnull;
}

void _ZFP_zfautoReleaseAction(ZF_IN ZFObject *obj,
                              ZF_IN const zfcharA *callerFile,
                              ZF_IN const zfcharA *callerFunction,
                              ZF_IN zfindex callerLine,
                              ZF_IN zfbool enableLeakTest)
{
    if(ZFPROTOCOL_IS_AVAILABLE(ZFThread))
    {
        ZFThread *threadCur = ZFThread::currentThread();
        if(threadCur == zfnull)
        {
            zfCoreCriticalMessageTrim(zfTextA("%s current thread is null, make sure the thread is started or registered by ZFThread"),
                ZF_CALLER_INFO_DETAIL(callerFile, callerFunction, callerLine));
            return ;
        }
        threadCur->_ZFP_ZFThreadAutoReleasePoolAdd(obj, callerFile, callerFunction, callerLine, enableLeakTest);
    }
    else
    {
        ZFAutoReleasePool::instance()->poolAdd(obj, callerFile, callerFunction, callerLine, enableLeakTest);

        if(ZFPROTOCOL_IS_AVAILABLE(ZFThreadTaskRequest))
        {
            ZF_GLOBAL_INITIALIZER_CLASS(zfautoRelease_poolDrainDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfautoRelease_poolDrainDataHolder);
            if(!d->drainTaskRequested)
            {
                d->drainTaskRequested = zftrue;
                ZFThreadTaskRequest(d->drainTask);
            }
        }
        else
        {
            zfCoreLogTrim(zfTextA("%s zfautoRelease called while no auto drain logic support, object %s would not be released normally"),
                ZF_CALLER_INFO_DETAIL(callerFile, callerFunction, callerLine),
                zfsCoreZ2A(obj->objectInfoOfInstance().cString()));
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

