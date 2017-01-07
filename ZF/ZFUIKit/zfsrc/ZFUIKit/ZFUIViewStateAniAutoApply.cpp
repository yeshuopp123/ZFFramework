/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIViewStateAniAutoApply.h"
#include "ZFUIViewBlink.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, ViewStateAniAutoApplyStart)
ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, ViewStateAniAutoApplyStop)
ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, ViewStateAniAutoApplyPause)
ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, ViewStateAniAutoApplyResume)

// ============================================================
zfbool ZFUIViewStateAniAutoApply;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniAutoApply_settingInit, ZFLevelZFFrameworkLow)
{
    ZFUIViewStateAniAutoApply = zffalse;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniAutoApply_settingInit)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniAutoApply_autoStart, ZFLevelAppLow)
{
    if(ZFUIViewStateAniAutoApply)
    {
        ZFUIViewStateAniAutoApplyStart();
    }
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniAutoApply_autoStart)
{
    ZFUIViewStateAniAutoApplyStop();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniAutoApply_autoStart)

// ============================================================
static zfbool _ZFP_ZFUIViewStateAniAutoApply_started = zffalse;
static zfint _ZFP_ZFUIViewStateAniAutoApply_paused = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniAutoApplyDataHolder, ZFLevelZFFrameworkNormal)
{
    this->taskStartListener = ZFCallbackForRawFunction(zfself::taskStart);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniAutoApplyDataHolder)
{
}
public:
    ZFListener taskStartListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(taskStart)
    {
        if(_ZFP_ZFUIViewStateAniAutoApply_paused == 0)
        {
            ZFUIViewStateAniStart();
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniAutoApplyDataHolder)

// ============================================================
void ZFUIViewStateAniAutoApplyStart(void)
{
    ZFUIViewStateAniAutoApplyStop();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniAutoApplyDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniAutoApplyDataHolder);

    _ZFP_ZFUIViewStateAniAutoApply_started = zftrue;

    ZFObjectGlobalEventObserverAdd(ZFUIView::EventViewLayoutOnLayoutRequest(), d->taskStartListener);
    ZFObjectGlobalEventObserverAdd(ZFUIView::EventViewOnAddToParent(), d->taskStartListener);
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewStateAniAutoApplyStart());
}
void ZFUIViewStateAniAutoApplyStop(void)
{
    ZFUIViewStateAniAutoApplyPauseForTimeCancel();
    if(!_ZFP_ZFUIViewStateAniAutoApply_started)
    {
        return ;
    }
    _ZFP_ZFUIViewStateAniAutoApply_started = zffalse;

    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniAutoApplyDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniAutoApplyDataHolder);
    ZFObjectGlobalEventObserverRemove(ZFUIView::EventViewLayoutOnLayoutRequest(), d->taskStartListener);
    ZFObjectGlobalEventObserverRemove(ZFUIView::EventViewOnAddToParent(), d->taskStartListener);
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewStateAniAutoApplyStop());
}
zfbool ZFUIViewStateAniAutoApplyStarted(void)
{
    return _ZFP_ZFUIViewStateAniAutoApply_started;
}

void ZFUIViewStateAniAutoApplyPause(void)
{
    ++_ZFP_ZFUIViewStateAniAutoApply_paused;
    if(_ZFP_ZFUIViewStateAniAutoApply_paused == 1)
    {
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewStateAniAutoApplyPause());
    }
}
void ZFUIViewStateAniAutoApplyResume(void)
{
    zfCoreAssert(_ZFP_ZFUIViewStateAniAutoApply_paused > 0);
    --_ZFP_ZFUIViewStateAniAutoApply_paused;
    if(_ZFP_ZFUIViewStateAniAutoApply_paused == 0)
    {
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewStateAniAutoApplyResume());
    }
}
zfindex ZFUIViewStateAniAutoApplyPaused(void)
{
    return (!_ZFP_ZFUIViewStateAniAutoApply_started || _ZFP_ZFUIViewStateAniAutoApply_paused);
}

// ============================================================
// ZFUIViewStateAniAutoApplyPauseForTime
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder, ZFLevelZFFrameworkNormal)
{
    this->started = zffalse;
    this->delayTimer = zfnull;
    this->doStopListener = ZFCallbackForRawFunction(zfself::doStop);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder)
{
    zfReleaseWithoutLeakTest(this->delayTimer);
}
public:
    zfbool started;
    ZFTimer *delayTimer;
    zftimet endTime;
    ZFListener doStopListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(doStop)
    {
        ZFUIViewStateAniAutoApplyPauseForTimeCancel();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder)

void ZFUIViewStateAniAutoApplyPauseForTime(ZF_IN zftimet time /* = 0 */)
{
    if(ZFUIViewStateAniAutoApplyPaused())
    {
        return ;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder);

    time = zfmMax<zftimet>(time, 100);
    zftimet endTime = ZFTime::timestamp() + time;
    if(d->started && endTime <= d->endTime + 100)
    {
        return ;
    }

    if(d->delayTimer == zfnull)
    {
        d->delayTimer = zfAllocWithoutLeakTest(ZFTimer);
        d->delayTimer->observerAdd(ZFTimer::EventTimerOnActivate(), d->doStopListener);
        d->delayTimer->timerActivateInMainThreadSet(zftrue);
    }
    d->delayTimer->timerStop();

    d->endTime = endTime;
    d->delayTimer->timerIntervalSet(time);
    ZFUIViewStateAniAutoApplyPause();
    d->started = zftrue;
    d->delayTimer->objectCachedSet(zftrue);
    d->delayTimer->timerStart();
}
void ZFUIViewStateAniAutoApplyPauseForTimeCancel(void)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder);
    if(!d->started)
    {
        return ;
    }
    d->started = zffalse;
    d->delayTimer->timerStop();
    d->delayTimer->objectCachedSet(zffalse);
    ZFUIViewStateAniAutoApplyResume();
}

// ZFUIViewStateAniAutoApplyPauseForTime auto cleanup
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniAutoApplyPauseForTimeAutoCleanup, ZFLevelZFFrameworkLow)
{
    // ensure init order
    (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniAutoApplyPauseForTimeAutoCleanup)
{
    ZFUIViewStateAniAutoApplyPauseForTimeCancel();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniAutoApplyPauseForTimeAutoCleanup)

ZF_NAMESPACE_GLOBAL_END

