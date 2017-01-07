/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTimer.h
 * @brief timer utility
 */

#ifndef _ZFI_ZFTimer_h_
#define _ZFI_ZFTimer_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFTimerPrivate;
/**
 * @brief timer utility
 *
 * usage:
 * @code
 *   ZFTimer *timer = zfAlloc(ZFTimer, ...);
 *   timer->start();
 *   zfRelease(timer); // safe to release after start, timer would be auto released after stop
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFTimer : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFTimer, ZFObject)

    // ============================================================
    // observer events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer started, ensured in the same thread of timer event\n
     * param0 is the #ZFTimer::timerParam0
     * and param1 is the #ZFTimer::timerParam1
     */
    ZFOBSERVER_EVENT(TimerOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer activated\n
     * param0 is the #ZFTimer::timerParam0
     * and param1 is the #ZFTimer::timerParam1
     * @note on what thread this event is fired,
     *   depends on #timerActivateInMainThread
     */
    ZFOBSERVER_EVENT(TimerOnActivate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer stopped, ensured in the same thread that stop the timer\n
     * param0 is the #ZFTimer::timerParam0
     * and param1 is the #ZFTimer::timerParam1
     */
    ZFOBSERVER_EVENT(TimerOnStop)

public:
    /**
     * @brief init with timer settings
     *
     * you may change them after timer created,
     * but you must not if timer is started
     */
    virtual ZFObject *objectOnInit(ZF_IN const zftimet &timerInterval,
                                   ZF_IN_OPT ZFObject *timerParam0 = zfnull,
                                   ZF_IN_OPT ZFObject *timerParam1 = zfnull,
                                   ZF_IN_OPT const zftimet &timerDelay = 0,
                                   ZF_IN_OPT zfbool timerActivateInMainThread = zffalse);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

public:
    /** @brief for internal use only */
    virtual void *nativeTimer(void);

public:
    /**
     * @brief timer's interval when start a timer in mili seconds, default is 1000
     *
     * assert fail if interval is less than 0
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, timerInterval, ZFPropertyInitValue(1000))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zftimet, timerInterval);

    /**
     * @brief timer's delay when start a timer in mili seconds, default is 0
     *
     * if delay is less than 10, it's treated as 0\n
     * else, first timer event would be fired after (timerDelay + timerInterval)
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, timerDelay, ZFPropertyInitValue(0))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zftimet, timerDelay);

    /**
     * @brief whether timer should be fired in main thread, false by default
     *
     * fired in main thread for convenience but may cause timer to be more inaccurate,
     * use only if necessary
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, timerActivateInMainThread, ZFPropertyInitValue(zffalse))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfbool, timerActivateInMainThread);

    /**
     * @brief timer param, automatically retained
     *
     * the param is alive as long as the timer object is alive,
     * it won't be released after timer stop
     */
    ZFPROPERTY_RETAIN(ZFObject *, timerParam0)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFObject *, timerParam0);

    /**
     * @brief timer param, automatically retained
     *
     * the param is alive as long as the timer object is alive,
     * it won't be released after timer stop
     */
    ZFPROPERTY_RETAIN(ZFObject *, timerParam1)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFObject *, timerParam1);

public:
    /**
     * @brief start the timer
     */
    virtual void timerStart(void);
    /**
     * @brief stop the timer
     */
    virtual void timerStop(void);
    /**
     * @brief true if started
     */
    virtual zfbool timerStarted(void);

public:
    /**
     * @brief get timer's current fired count, with 1 as first fired time's value,
     *   keep after timer stop, but reset before timer start
     */
    virtual zfindex timerActivatedCount(void);

public:
    zffinal void _ZFP_ZFTimer_timerOnStart(void);
    zffinal void _ZFP_ZFTimer_timerOnActivate(void);
    zffinal void _ZFP_ZFTimer_timerOnStop(void);
protected:
    /** @brief see #EventTimerOnStart */
    virtual inline void timerOnStart(void)
    {
        this->observerNotify(ZFTimer::EventTimerOnStart(), this->timerParam0(), this->timerParam1());
    }
    /** @brief see #EventTimerOnActivate */
    virtual inline void timerOnActivate(void)
    {
        this->observerNotify(ZFTimer::EventTimerOnActivate(), this->timerParam0(), this->timerParam1());
    }
    /** @brief see #EventTimerOnStop */
    virtual inline void timerOnStop(void)
    {
        this->observerNotify(ZFTimer::EventTimerOnStop(), this->timerParam0(), this->timerParam1());
    }

private:
    _ZFP_ZFTimerPrivate *d;
};

// ============================================================
// timer util
/**
 * @brief see #ZFTimerExecute
 */
zfclassLikePOD ZF_ENV_EXPORT ZFTimerExecuteParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFTimerExecuteParam)

public:
    /** @brief see #ZFTimer::timerInterval */
    ZFCORE_PARAM_WITH_INIT(zftimet, timerInterval, 1000)
    /** @brief see #ZFTimer::timerDelay */
    ZFCORE_PARAM_WITH_INIT(zftimet, timerDelay, 0)
    /** @brief see #ZFTimer::timerActivateInMainThread */
    ZFCORE_PARAM_WITH_INIT(zfbool, timerActivateInMainThread, zffalse)
    /** @brief see #ZFTimer::timerParam0 */
    ZFCORE_PARAM_RETAIN_WITH_INIT(ZFObject *, timerParam0, zfnull)
    /** @brief see #ZFTimer::timerParam1 */
    ZFCORE_PARAM_RETAIN_WITH_INIT(ZFObject *, timerParam1, zfnull)
    /** @brief see #ZFTimer */
    ZFCORE_PARAM_RETAIN_WITH_INIT(ZFObject *, userData, zfnull)

    /**
     * @brief the timer callback to run
     */
    ZFCORE_PARAM(ZFListener, timerCallback)
    /**
     * @brief automatically stop timer when reach max count, 0 means no limit, 0 by default
     */
    ZFCORE_PARAM_WITH_INIT(zfindex, timerActivateCountMax, 0)
};
/**
 * @brief util method to start a timer
 *
 * return the started timer if success or null otherwise,
 * the started timer would be released automatically when stopped
 */
extern ZF_ENV_EXPORT zfautoObject ZFTimerExecute(ZF_IN const ZFTimerExecuteParam &param);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimer_h_

