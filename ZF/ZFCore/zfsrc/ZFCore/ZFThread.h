/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFThread.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_h_
#define _ZFI_ZFThread_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFThreadPrivate;
/**
 * @brief thread utility
 *
 * you can use thread by one of these method:
 * -  inherit from ZFThread and override ZFThread::threadOnRun
 * -  declare a callback and set as thread's run loop by #ZFThread::threadRunnableSet
 *
 * @note try to use ZFThreadExecuteInNewThread is recommended, since it may have thread pool
 */
zfclass ZF_ENV_EXPORT ZFThread : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFThread, ZFObject)

public:
    // ============================================================
    // observer events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0/param1 is the params passed to the thread,
     * called in the same thread of the thread task
     */
    ZFOBSERVER_EVENT(ThreadOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0/param1 is the params passed to the thread,
     * called in the same thread of the thread task
     */
    ZFOBSERVER_EVENT(ThreadOnStop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0/param1 is the params passed to the thread,
     * called in the same thread that cancel the task
     */
    ZFOBSERVER_EVENT(ThreadOnCancel)

    // ============================================================
    // thread control
public:
    /**
     * @brief register a thread which is not started by ZFThread, assert fail if already registered
     *
     * return a token for #nativeThreadUnregister to unregister\n
     * \n
     * you must call nativeThreadUnregister before the thread ends\n
     * \n
     * you must take good care when calling this method\n
     * main thread has no need to register
     */
    static void *nativeThreadRegister(void);
    /**
     * @brief see #nativeThreadRegister
     *
     * it's safe to unregister in different thread using token
     */
    static void nativeThreadUnregister(ZF_IN void *token);

public:
    /**
     * @brief return main thread
     */
    static ZFThread *mainThread(void);
    /**
     * @brief return current thread,
     *   or zfnull if thread isn't started or registered by ZFThread
     *   or not registered by #ZFThread::nativeThreadRegister
     */
    static ZFThread *currentThread(void);

    /**
     * @brief make current thread sleep for miliSecs,
     *   note this method may be not accurate
     */
    static void sleep(ZF_IN const zftimet &miliSecs);

    // ============================================================
    // thread instance method
public:
    /**
     * @brief init from listener
     */
    virtual ZFObject *objectOnInit(ZF_IN const ZFListener &runnable);
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief the main run loop
     *
     * default is invalid callback, meaning the builtin #threadOnRun would be run\n
     * changing this callback to a valid runnable would cause the builtin #threadOnRun being replaced\n
     * you must make sure thread isn't running when setting new runnable\n
     * this value take effects only if you start thread by #threadStart
     * @see threadStart, threadOnRun, threadWait
     */
    ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE(ZFListener, threadRunnable)

    /**
     * @brief start thread, do nothing if already started
     */
    virtual void threadStart(ZF_IN_OPT ZFObject *userData = zfnull,
                             ZF_IN_OPT ZFObject *param0 = zfnull,
                             ZF_IN_OPT ZFObject *param1 = zfnull);
    /**
     * @brief return true if start is called and hasn't been stopped or end
     */
    virtual zfbool threadStarted(void);
    /**
     * @brief return true only if thread is running
     */
    virtual zfbool threadRunning(void);
    /**
     * @brief request stop the thread, may not be stopped immediately
     *
     * if thread is started but not running, this function will cancel the thread's run\n
     * otherwise, whether the thread is continue running or stopped, is up to the runnable
     * @see threadStopRequested
     */
    virtual void threadStop(void);
    /**
     * @brief return true if stop is called after start
     */
    virtual zfbool threadStopRequested(void);

    /**
     * @brief wait until thread finished running
     *
     * return immediately if thread isn't running
     */
    virtual void threadWait(void);

    /**
     * @brief wait until thread finished running, or timeout
     *
     * return immediately if thread isn't running
     * @return true if wait success or thread isn't running\n
     *         false if wait timeout or error
     */
    virtual zfbool threadWait(ZF_IN const zftimet &miliSecs);

    /**
     * @brief return true if current thread is main thread
     *
     * subclass should not override this method
     */
    virtual zfbool isMainThread(void);
    /**
     * @brief add object to auto release pool attached to this thread
     *
     * usually this method is called by #zfautoRelease
     * @see ZFAutoReleasePool
     */
    virtual void autoReleasePoolAdd(ZF_IN ZFObject *obj,
                                    ZF_IN_OPT zfbool enableLeakTest = zftrue);
    /**
     * @brief add object to auto release pool attached to this thread with location info,
     *   used by ZFLeakTest
     * @see ZFAutoReleasePool
     */
    virtual void autoReleasePoolAdd(ZF_IN ZFObject *obj,
                                    ZF_IN const zfcharA *callerFile,
                                    ZF_IN const zfcharA *callerFunction,
                                    ZF_IN zfindex callerLine,
                                    ZF_IN_OPT zfbool enableLeakTest = zftrue);
    /**
     * @brief manually drain auto release pool
     *
     * must ensure that all objects in pool are safe to release\n
     * called automatically after each time that runnable ends
     * @see ZFAutoReleasePool
     */
    virtual void autoReleasePoolDrain(void);
    zffinal void _ZFP_ZFThreadAutoReleasePoolAdd(ZF_IN ZFObject *obj,
                                                 ZF_IN const zfcharA *callerFile,
                                                 ZF_IN const zfcharA *callerFunction,
                                                 ZF_IN zfindex callerLine,
                                                 ZF_IN zfbool enableLeakTest);
    zffinal void _ZFP_ZFThreadAutoReleasePoolMarkResolved(void);

protected:
    /**
     * @brief internal runnable, do nothing by default, you may override it
     *
     * see #threadRunnable
     */
    ZFMETHOD_DECLARE_DETAIL_2(protected, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst,
                              void, threadOnRun, ZFMethodNoId,
                              const ZFListenerData &, listenerData, ZFMethodNoDefaultParam,
                              ZFObject *, userData, ZFMethodNoDefaultParam);

public:
    zffinal void _ZFP_ZFThread_threadOnStart(ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        this->threadOnStart(param0, param1);
    }
    zffinal void _ZFP_ZFThread_threadOnStop(ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        this->threadOnStop(param0, param1);
    }
    zffinal void _ZFP_ZFThread_threadOnCancel(ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        this->threadOnCancel(param0, param1);
    }
protected:
    /** @brief see #EventThreadOnStart */
    virtual inline void threadOnStart(ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        this->observerNotify(ZFThread::EventThreadOnStart(), param0, param1);
    }
    /** @brief see #EventThreadOnStop */
    virtual inline void threadOnStop(ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        this->observerNotify(ZFThread::EventThreadOnStop(), param0, param1);
    }
    /** @brief see #EventThreadOnCancel */
    virtual inline void threadOnCancel(ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        this->observerNotify(ZFThread::EventThreadOnCancel(), param0, param1);
    }

public:
    _ZFP_ZFThreadPrivate *_ZFP_ZFThread_d;
};
zfclassFwd ZFMutex;
extern ZF_ENV_EXPORT ZFMutex *_ZFP_ZFThread_mutex;

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_h_

#include "ZFThread_execute.h"
#include "ZFThread_observerNotifyInMainThread.h"
#include "ZFThread_taskRequest.h"
#include "ZFThread_zfautoRelease.h"

