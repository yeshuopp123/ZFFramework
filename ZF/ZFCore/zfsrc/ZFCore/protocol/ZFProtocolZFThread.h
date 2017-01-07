/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFThread.h
 * @brief protocol for ZFThread
 */

#ifndef _ZFI_ZFProtocolZFThread_h_
#define _ZFI_ZFProtocolZFThread_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFThread
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFThread)
public:
    /**
     * @brief see #ZFThread::nativeThreadUnregister
     *
     * you should not retain or release ownerZFThread
     */
    virtual void *nativeThreadRegister(ZF_IN ZFThread *ownerZFThread) = 0;
    /**
     * @brief see #ZFThread::nativeThreadUnregister, #nativeThreadRegister
     *
     * you should not retain or release ownerZFThread
     */
    virtual void nativeThreadUnregister(ZF_IN void *token) = 0;
    /**
     * @brief get thread from token created by nativeThreadRegister
     */
    virtual ZFThread *threadForToken(ZF_IN void *token) = 0;

    /**
     * @brief see #ZFThread::mainThread
     */
    virtual ZFThread *mainThread(void) = 0;

    /**
     * @brief see #ZFThread::currentThread
     */
    virtual ZFThread *currentThread(void) = 0;

    /**
     * @brief see #ZFThread::sleep
     */
    virtual void sleep(ZF_IN const zftimet &miliSecs) = 0;

    /**
     * @brief see #ZFThreadExecuteInMainThread
     *
     * you should not retain or release runnable or params\n
     * you should queue it to run even if current thread is main thread
     */
    virtual void *executeInMainThread(ZF_IN zfidentity taskId,
                                      ZF_IN const ZFListener &runnable,
                                      ZF_IN ZFObject *param0,
                                      ZF_IN ZFObject *param1) = 0;
    /**
     * @brief cancel execute task
     *
     * you could do nothing if task can't be canceled\n
     * this function is designed for performance,
     * that you may cancel execute task if it hasn't started to run
     */
    virtual void executeInMainThreadCancel(ZF_IN zfidentity taskId,
                                           ZF_IN void *nativeToken)
    {
    }
    /**
     * @brief used to cleanup nativeToken
     */
    virtual void executeInMainThreadCleanup(ZF_IN zfidentity taskId,
                                            ZF_IN void *nativeToken)
    {
    }

    /**
     * @brief see #ZFThreadExecuteInNewThread
     *
     * you should not retain or release runnable or params\n
     * you should not retain or release ownerZFThread\n
     * you should call runnable in new thread,
     * also, remember to register so that currentThread should work
     */
    virtual void *executeInNewThread(ZF_IN zfidentity taskId,
                                     ZF_IN ZFThread *ownerZFThread,
                                     ZF_IN const ZFListener &runnable,
                                     ZF_IN ZFObject *param0,
                                     ZF_IN ZFObject *param1) = 0;
    /**
     * @brief cancel execute task
     *
     * you could do nothing if task can't be canceled\n
     * this function is designed for performance,
     * that you may cancel execute task if it hasn't started to run
     */
    virtual void executeInNewThreadCancel(ZF_IN zfidentity taskId,
                                          ZF_IN void *nativeToken)
    {
    }
    /**
     * @brief used to cleanup nativeToken
     */
    virtual void executeInNewThreadCleanup(ZF_IN zfidentity taskId,
                                           ZF_IN void *nativeToken)
    {
    }

    /**
     * @brief see #ZFThreadExecuteInMainThreadAfterDelay
     *
     * you should not retain or release runnable or params\n
     * you should call runnable in main thread after specified delay\n
     * since delay may be interrupted,
     * you must implement #executeInMainThreadAfterDelayCancel to support task's cancel
     */
    virtual void *executeInMainThreadAfterDelay(ZF_IN zfidentity taskId,
                                                ZF_IN zftimet delay,
                                                ZF_IN const ZFListener &runnable,
                                                ZF_IN ZFObject *param0,
                                                ZF_IN ZFObject *param1) = 0;
    /**
     * @brief cancel execute task
     */
    virtual void executeInMainThreadAfterDelayCancel(ZF_IN zfidentity taskId,
                                                     ZF_IN void *nativeToken) = 0;
    /**
     * @brief used to cleanup nativeToken
     */
    virtual void executeInMainThreadAfterDelayCleanup(ZF_IN zfidentity taskId,
                                                      ZF_IN void *nativeToken)
    {
    }
ZFPROTOCOL_INTERFACE_END(ZFThread)

/** @brief see #ZFThread */
zfclass ZF_ENV_EXPORT ZFThreadMainThread : zfextends ZFThread
{
    ZFOBJECT_DECLARE(ZFThreadMainThread, ZFThread)

public:
    zfoverride
    virtual void threadStart(ZF_IN_OPT ZFObject *userData = zfnull,
                             ZF_IN_OPT ZFObject *param0 = zfnull,
                             ZF_IN_OPT ZFObject *param1 = zfnull)
    {
        zfCoreLogTrim(zfTextA("you must not start main thread"));
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
        zfCoreLogTrim(zfTextA("you must not stop main thread"));
    }
    zfoverride
    virtual void threadWait(void)
    {
        zfCoreLogTrim(zfTextA("you must not wait main thread"));
    }
    zfoverride
    virtual zfbool threadWait(ZF_IN const zftimet &miliSecs)
    {
        zfCoreLogTrim(zfTextA("you must not wait main thread"));
        return zffalse;
    }
    zfoverride
    virtual zfbool isMainThread(void)
    {
        return zftrue;
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFThread_h_

