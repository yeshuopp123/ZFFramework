/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFTimer.h"
#include "protocol/ZFProtocolZFTimer.h"

#include "ZFThread.h" // for timer thread register
#include "ZFValue.h" // for timer activate count

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFTimerPrivate
zfclassNotPOD _ZFP_ZFTimerPrivate
{
public:
    void *nativeTimer;
    zfbool timerStarted;
    zfindex timerActivatedCount;
    void *timerThreadToken;

public:
    _ZFP_ZFTimerPrivate(void)
    : nativeTimer(zfnull)
    , timerStarted(zffalse)
    , timerActivatedCount(0)
    , timerThreadToken(zfnull)
    {
    }
};

// ============================================================
// ZFTimer
ZFOBJECT_REGISTER(ZFTimer)

ZFOBSERVER_EVENT_REGISTER(ZFTimer, TimerOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFTimer, TimerOnActivate)
ZFOBSERVER_EVENT_REGISTER(ZFTimer, TimerOnStop)

ZFObject *ZFTimer::objectOnInit(ZF_IN const zftimet &timerInterval,
                                ZF_IN_OPT ZFObject *timerParam0 /* = zfnull */,
                                ZF_IN_OPT ZFObject *timerParam1 /* = zfnull */,
                                ZF_IN_OPT const zftimet &timerDelay /* = 0 */,
                                ZF_IN_OPT zfbool timerActivateInMainThread /* = zffalse */)
{
    this->objectOnInit();
    zfself::timerIntervalSet(timerInterval);
    zfself::timerDelaySet(timerDelay);
    zfself::timerParam0Set(timerParam0);
    zfself::timerParam1Set(timerParam1);
    zfself::timerActivateInMainThreadSet(timerActivateInMainThread);
    return this;
}
ZFObject *ZFTimer::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFTimerPrivate);
    d->nativeTimer = ZFPROTOCOL_ACCESS(ZFTimer)->nativeTimerCreate(this);
    return this;
}
void ZFTimer::objectOnDealloc(void)
{
    ZFPROTOCOL_ACCESS(ZFTimer)->nativeTimerDestroy(this, d->nativeTimer);
    d->nativeTimer = zfnull;
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFTimer::objectOnDeallocPrepare(void)
{
    this->timerStop();
    zfsuper::objectOnDeallocPrepare();
}

void *ZFTimer::nativeTimer(void)
{
    return d->nativeTimer;
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFTimer, zftimet, timerInterval)
{
    zfCoreAssert(!this->timerStarted());
    zfCoreAssert(newValue > 0);
    this->timerIntervalSetInternal(newValue);
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFTimer, zftimet, timerDelay)
{
    zfCoreAssert(!this->timerStarted());
    zfCoreAssert(newValue >= 0);
    this->timerDelaySetInternal(newValue);
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFTimer, zfbool, timerActivateInMainThread)
{
    zfCoreAssert(!this->timerStarted());
    this->timerActivateInMainThreadSetInternal(newValue);
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFTimer, ZFObject *, timerParam0)
{
    zfCoreAssert(!this->timerStarted());
    this->timerParam0SetInternal(newValue);
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFTimer, ZFObject *, timerParam1)
{
    zfCoreAssert(!this->timerStarted());
    this->timerParam1SetInternal(newValue);
}

void ZFTimer::timerStart(void)
{
    if(d->timerStarted)
    {
        return ;
    }
    d->timerStarted = zftrue;

    zfRetain(this);

    d->timerActivatedCount = 0;
    ZFPROTOCOL_ACCESS(ZFTimer)->timerStart(this);
}
void ZFTimer::timerStop(void)
{
    if(d->timerStarted)
    {
        d->timerStarted = zffalse;
        ZFPROTOCOL_ACCESS(ZFTimer)->timerStop(this);
    }
}

zfbool ZFTimer::timerStarted(void)
{
    return d->timerStarted;
}

zfindex ZFTimer::timerActivatedCount(void)
{
    return d->timerActivatedCount;
}

void ZFTimer::_ZFP_ZFTimer_timerOnStart(void)
{
    if(ZFThread::currentThread() == zfnull)
    {
        d->timerThreadToken = ZFThread::nativeThreadRegister();
    }
    this->timerOnStart();
}
void ZFTimer::_ZFP_ZFTimer_timerOnActivate(void)
{
    zfRetain(this);
    {
        ++(d->timerActivatedCount);
        this->timerOnActivate();
    }
    zfRelease(this);
}
void ZFTimer::_ZFP_ZFTimer_timerOnStop(void)
{
    this->timerOnStop();
    if(d->timerThreadToken != zfnull)
    {
        ZFThread::nativeThreadUnregister(d->timerThreadToken);
        d->timerThreadToken = zfnull;
    }
    zfRelease(this);
}

// ============================================================
zfautoObject ZFTimerExecute(ZF_IN const ZFTimerExecuteParam &param)
{
    if(param.timerInterval() <= 0 || !param.timerCallback().callbackIsValid())
    {
        return zfautoObjectNull;
    }
    zfblockedAlloc(ZFTimer, timer);
    timer->timerIntervalSet(param.timerInterval());
    timer->timerDelaySet(param.timerDelay());
    timer->timerActivateInMainThreadSet(param.timerActivateInMainThread());
    timer->timerParam0Set(param.timerParam0());
    timer->timerParam1Set(param.timerParam1());
    timer->observerAdd(ZFTimer::EventTimerOnActivate(), param.timerCallback(), param.userData());
    if(param.timerActivateCountMax() > 0)
    {
        ZFLISTENER_LOCAL(timerOnActivate, {
            zfindex timerActivatedCountMax = userData->to<ZFValue *>()->indexValue();
            ZFTimer *timer = listenerData.sender->to<ZFTimer *>();
            if(timer->timerActivatedCount() > timerActivatedCountMax)
            {
                timer->timerStop();
            }
        })
        timer->observerAdd(ZFTimer::EventTimerOnActivate(),
            timerOnActivate,
            ZFValue::indexValueCreate(param.timerActivateCountMax()).toObject());
    }
    timer->timerStart();
    return zfautoObjectCreateMarkCached(timer);
}

ZF_NAMESPACE_GLOBAL_END

