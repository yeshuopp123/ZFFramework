/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIViewBlinkWhenFocus.h"
#include "ZFUIViewBlink.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, ViewBlinkWhenFocusAutoApplyStart)
ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, ViewBlinkWhenFocusAutoApplyStop)
ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, ViewBlinkWhenFocusAutoApplyPause)
ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, ViewBlinkWhenFocusAutoApplyResume)
ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, ViewBlinkWhenFocusViewBlinkOn)
ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, ViewBlinkWhenFocusViewBlinkOff)

// ============================================================
// ZFUIViewBlinkWhenFocusFilter
ZFFilterForZFObject ZFUIViewBlinkWhenFocusFilter;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocusFilterDataHolder, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocusFilterDataHolder)
{
    ZFUIViewBlinkWhenFocusFilter.filterRemoveAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocusFilterDataHolder)

// ============================================================
zfbool ZFUIViewBlinkWhenFocusAutoApply;
zfautoObject ZFUIViewBlinkWhenFocusMaskImage;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocusAutoApply_settingInit, ZFLevelZFFrameworkLow)
{
    ZFUIViewBlinkWhenFocusAutoApply = zftrue;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocusAutoApply_settingInit)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocusAutoApply_autoStart, ZFLevelAppLow)
{
    if(ZFUIViewBlinkWhenFocusAutoApply)
    {
        ZFUIViewBlinkWhenFocusAutoApplyStart();
    }
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocusAutoApply_autoStart)
{
    ZFUIViewBlinkWhenFocusAutoApplyStop();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocusAutoApply_autoStart)

// ============================================================
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIViewBlinkWhenFocus_focusChange);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIViewBlinkWhenFocus_mouseDown);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIViewBlinkWhenFocus_viewOnDealloc);
static zfbool _ZFP_ZFUIViewBlinkWhenFocus_started = zffalse;
static zfint _ZFP_ZFUIViewBlinkWhenFocus_paused = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocusDataHolder, ZFLevelZFFrameworkNormal)
{
    this->focusChangeListener = ZFCallbackForRawFunction(_ZFP_ZFUIViewBlinkWhenFocus_focusChange);
    this->mouseDownListener = ZFCallbackForRawFunction(_ZFP_ZFUIViewBlinkWhenFocus_mouseDown);
    this->viewOnDeallocListener = ZFCallbackForRawFunction(_ZFP_ZFUIViewBlinkWhenFocus_viewOnDealloc);

    this->viewBlinkOnListener = ZFCallbackForRawFunction(zfself::viewBlinkOn);
    this->viewBlinkOffListener = ZFCallbackForRawFunction(zfself::viewBlinkOff);
    ZFObjectGlobalEventObserverAdd(ZFGlobalEvent::EventViewBlinkOn(), this->viewBlinkOnListener);
    ZFObjectGlobalEventObserverAdd(ZFGlobalEvent::EventViewBlinkOff(), this->viewBlinkOffListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocusDataHolder)
{
    ZFObjectGlobalEventObserverRemove(ZFGlobalEvent::EventViewBlinkOn(), this->viewBlinkOnListener);
    ZFObjectGlobalEventObserverRemove(ZFGlobalEvent::EventViewBlinkOff(), this->viewBlinkOffListener);
    ZFUIViewBlinkWhenFocusMaskImage = zfautoObjectNull;
}
public:
    zfautoObject maskImageCur;
    ZFListener viewBlinkOnListener;
    ZFListener viewBlinkOffListener;
    ZFListener focusChangeListener;
    ZFListener mouseDownListener;
    ZFListener viewOnDeallocListener;
    ZFCoreArrayPOD<ZFUIView *> focusedViews;
    static ZFLISTENER_PROTOTYPE_EXPAND(viewBlinkOn)
    {
        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(
            listenerData.sender, ZFGlobalEvent::EventViewBlinkWhenFocusViewBlinkOn());
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(viewBlinkOff)
    {
        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(
            listenerData.sender, ZFGlobalEvent::EventViewBlinkWhenFocusViewBlinkOff());
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocusDataHolder)

// ============================================================
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIViewBlinkWhenFocus_focusChange)
{
    ZFUIView *view = listenerData.sender->to<ZFUIView *>();

    if(view->objectIsPrivate())
    {
        return ;
    }

    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusDataHolder);

    if(view->viewFocused())
    {
        if(!ZFUIViewBlinkWhenFocusFilter.filterCheckActive(view))
        {
            return ;
        }
        d->focusedViews.add(view);
        view->observerAdd(ZFUIView::EventObjectBeforeDealloc(), d->viewOnDeallocListener);
        view->observerAdd(ZFUIView::EventViewOnEvent(), d->mouseDownListener);
        if(!ZFUIViewBlinkWhenFocusAutoApplyPaused())
        {
            ZFUIViewBlink(view, d->maskImageCur.to<ZFUIImage *>());
        }
    }
    else
    {
        d->focusedViews.removeElement(view);
        view->observerRemove(ZFUIView::EventObjectBeforeDealloc(), d->viewOnDeallocListener);
        view->observerRemove(ZFUIView::EventViewOnEvent(), d->mouseDownListener);
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIViewBlinkWhenFocus_mouseDown)
{
    ZFUIView *view = listenerData.sender->to<ZFUIView *>();
    if(view->objectIsPrivate())
    {
        return ;
    }

    ZFUIMouseEvent *event = ZFCastZFObject(ZFUIMouseEvent *, listenerData.param0);
    if(event == zfnull || event->mouseAction != ZFUIMouseAction::e_MouseDown)
    {
        return ;
    }
    if(!ZFUIViewBlinkWhenFocusFilter.filterCheckActive(view))
    {
        return ;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusDataHolder);
    if(!ZFUIViewBlinkWhenFocusAutoApplyPaused())
    {
        ZFUIViewBlink(view, d->maskImageCur.to<ZFUIImage *>());
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIViewBlinkWhenFocus_viewOnDealloc)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusDataHolder);
    ZFUIView *view = listenerData.sender->to<ZFUIView *>();
    d->focusedViews.removeElement(view);
    view->observerRemove(ZFUIView::EventObjectBeforeDealloc(), d->viewOnDeallocListener);
    view->observerRemove(ZFUIView::EventViewOnEvent(), d->mouseDownListener);
}

// ============================================================
void ZFUIViewBlinkWhenFocusAutoApplyStart(ZF_IN_OPT ZFUIImage *img /* = zfnull */)
{
    ZFUIViewBlinkWhenFocusAutoApplyStop();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusDataHolder);

    _ZFP_ZFUIViewBlinkWhenFocus_started = zftrue;
    d->maskImageCur = zfautoObjectCreateMarkCached((img != zfnull) ? img : ZFUIViewBlinkWhenFocusMaskImage.to<ZFUIImage *>());

    ZFObjectGlobalEventObserverAdd(ZFUIView::EventViewFocusOnChange(), d->focusChangeListener);
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewBlinkWhenFocusAutoApplyStart());
}
void ZFUIViewBlinkWhenFocusAutoApplyStop(void)
{
    ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeCancel();
    if(!_ZFP_ZFUIViewBlinkWhenFocus_started)
    {
        return ;
    }
    _ZFP_ZFUIViewBlinkWhenFocus_started = zffalse;

    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusDataHolder);
    ZFObjectGlobalEventObserverRemove(ZFUIView::EventViewFocusOnChange(), d->focusChangeListener);
    for(zfindex i = 0; i < d->focusedViews.count(); ++i)
    {
        ZFUIView *view = d->focusedViews[i];
        view->observerRemove(ZFUIView::EventObjectBeforeDealloc(), d->viewOnDeallocListener);
        view->observerRemove(ZFUIView::EventViewOnEvent(), d->mouseDownListener);
    }
    d->focusedViews.removeAll();
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewBlinkWhenFocusAutoApplyStop());
}
zfbool ZFUIViewBlinkWhenFocusAutoApplyStarted(void)
{
    return _ZFP_ZFUIViewBlinkWhenFocus_started;
}

void ZFUIViewBlinkWhenFocusAutoApplyPause(void)
{
    ++_ZFP_ZFUIViewBlinkWhenFocus_paused;
    if(_ZFP_ZFUIViewBlinkWhenFocus_paused == 1)
    {
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewBlinkWhenFocusAutoApplyPause());
    }
}
void ZFUIViewBlinkWhenFocusAutoApplyResume(void)
{
    zfCoreAssert(_ZFP_ZFUIViewBlinkWhenFocus_paused > 0);
    --_ZFP_ZFUIViewBlinkWhenFocus_paused;
    if(_ZFP_ZFUIViewBlinkWhenFocus_paused == 0)
    {
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewBlinkWhenFocusAutoApplyResume());
    }
}
zfindex ZFUIViewBlinkWhenFocusAutoApplyPaused(void)
{
    return (!_ZFP_ZFUIViewBlinkWhenFocus_started || _ZFP_ZFUIViewBlinkWhenFocus_paused);
}

// ============================================================
// ZFUIViewBlinkWhenFocusAutoApplyPauseForTime
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder, ZFLevelZFFrameworkNormal)
{
    this->started = zffalse;
    this->delayTimer = zfnull;
    this->doStopListener = ZFCallbackForRawFunction(zfself::doStop);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder)
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
        ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeCancel();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder)

void ZFUIViewBlinkWhenFocusAutoApplyPauseForTime(ZF_IN zftimet time /* = 0 */)
{
    if(ZFUIViewBlinkWhenFocusAutoApplyPaused())
    {
        return ;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder);

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
    ZFUIViewBlinkWhenFocusAutoApplyPause();
    d->started = zftrue;
    d->delayTimer->objectCachedSet(zftrue);
    d->delayTimer->timerStart();
}
void ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeCancel(void)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder);
    if(!d->started)
    {
        return ;
    }
    d->started = zffalse;
    d->delayTimer->timerStop();
    d->delayTimer->objectCachedSet(zffalse);
    ZFUIViewBlinkWhenFocusAutoApplyResume();
}

// ZFUIViewBlinkWhenFocusAutoApplyPauseForTime auto cleanup
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeAutoCleanup, ZFLevelZFFrameworkLow)
{
    // ensure init order
    (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeDataHolder);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeAutoCleanup)
{
    ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeCancel();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeAutoCleanup)

ZF_NAMESPACE_GLOBAL_END

