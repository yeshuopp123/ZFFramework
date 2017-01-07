/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIOnScreenKeyboardAutoResize.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// start how many times for the same window
#define _ZFP_ZFUIOnScreenKeyboardAutoResize_tag_startCount zfText("_ZFP_ZFUIOnScreenKeyboardAutoResize_tag_startCount")
// holds a ZFTypeHolder to a ZFUIMargin
#define _ZFP_ZFUIOnScreenKeyboardAutoResize_tag_layoutMarginSaved zfText("_ZFP_ZFUIOnScreenKeyboardAutoResize_tag_layoutMarginSaved")

static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStart(ZF_IN ZFUIWindow *window);
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStop(ZF_IN ZFUIWindow *window);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_onScreenKeyboardStateChange);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnShow);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowLayoutMarginChange);

void ZFUIOnScreenKeyboardAutoResizeStart(ZF_IN ZFUIWindow *window)
{
    if(window == zfnull)
    {
        return ;
    }

    ZFCoreMutexLocker();

    ZFValueEditable *count = window->tagGet<ZFValueEditable *>(_ZFP_ZFUIOnScreenKeyboardAutoResize_tag_startCount);
    if(count != zfnull)
    {
        count->indexValueSet(count->indexValue() + 1);
        return ;
    }
    zfautoObject countHolder = ZFValueEditable::indexValueCreate(1);
    window->tagSetMarkCached(_ZFP_ZFUIOnScreenKeyboardAutoResize_tag_startCount, countHolder.toObject());

    _ZFP_ZFUIOnScreenKeyboardAutoResize_doStart(window);
}
void ZFUIOnScreenKeyboardAutoResizeStop(ZF_IN ZFUIWindow *window)
{
    if(window == zfnull)
    {
        return ;
    }

    ZFCoreMutexLocker();

    ZFValueEditable *count = window->tagGet<ZFValueEditable *>(_ZFP_ZFUIOnScreenKeyboardAutoResize_tag_startCount);
    if(count == zfnull)
    {
        return ;
    }
    if(count->indexValue() > 1)
    {
        count->indexValueSet(count->indexValue() - 1);
        return ;
    }
    window->tagRemove(_ZFP_ZFUIOnScreenKeyboardAutoResize_tag_startCount);

    _ZFP_ZFUIOnScreenKeyboardAutoResize_doStop(window);
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIOnScreenKeyboardAutoResizeDataHolder, ZFLevelZFFrameworkHigh)
{
    this->onScreenKeyboardStateChangeListener = ZFCallbackForRawFunction(_ZFP_ZFUIOnScreenKeyboardAutoResize_onScreenKeyboardStateChange);
    this->windowOnShowListener = ZFCallbackForRawFunction(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnShow);
    this->windowLayoutMarginChangeListener = ZFCallbackForRawFunction(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowLayoutMarginChange);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardAutoResizeDataHolder)
{
    zfCoreAssertWithMessageTrim(this->windowList.isEmpty(),
        zfText("ZFUIOnScreenKeyboardAutoResizeStart/ZFUIOnScreenKeyboardAutoResizeStop mismatch, have you forgot to stop?"));
}
public:
    ZFCoreArrayPOD<ZFUIWindow *> windowList;
    ZFListener onScreenKeyboardStateChangeListener;
    ZFListener windowOnShowListener;
    ZFListener windowLayoutMarginChangeListener;
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardAutoResizeDataHolder)

static void _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(ZF_IN ZFUIWindow *window, ZF_IN ZFUIOnScreenKeyboardState *state)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    ZFTypeHolder *layoutMarginSaved = window->windowLayoutParam()->tagGet<ZFTypeHolder *>(_ZFP_ZFUIOnScreenKeyboardAutoResize_tag_layoutMarginSaved);
    ZFUIMargin layoutMarginOld = (layoutMarginSaved ? layoutMarginSaved->holdedDataRef<ZFUIMargin &>() : window->windowLayoutParam()->layoutMargin());
    if(state->keyboardShowing() && window->windowShowing())
    {
        ZFUIMargin margin = layoutMarginOld;
        ZFUIRect windowFrame = ZFUIRectApplyMargin(ZFUIViewPositionOnScreen(window->viewParent()), margin);
        ZFUIRect clientFrame = ZFUIRectZero;
        state->keyboardFixClientFrame(clientFrame);
        if(ZFUIRectGetLeft(clientFrame) > ZFUIRectGetLeft(windowFrame))
        {
            margin.left += ZFUIRectGetLeft(clientFrame) - ZFUIRectGetLeft(windowFrame);
        }
        if(ZFUIRectGetTop(clientFrame) > ZFUIRectGetTop(windowFrame))
        {
            margin.top += ZFUIRectGetTop(clientFrame) - ZFUIRectGetTop(windowFrame);
        }
        if(ZFUIRectGetRight(clientFrame) < ZFUIRectGetRight(windowFrame))
        {
            margin.right += ZFUIRectGetRight(windowFrame) - ZFUIRectGetRight(clientFrame);
        }
        if(ZFUIRectGetBottom(clientFrame) < ZFUIRectGetBottom(windowFrame))
        {
            margin.bottom += ZFUIRectGetBottom(windowFrame) - ZFUIRectGetBottom(clientFrame);
        }

        if(layoutMarginSaved == zfnull)
        {
            window->windowLayoutParam()->tagSetMarkCached(_ZFP_ZFUIOnScreenKeyboardAutoResize_tag_layoutMarginSaved,
                zflineAlloc(ZFTypeHolder, zfnew(ZFUIMargin, layoutMarginOld), ZFTypeHolder::DeleteObject<ZFUIMargin *>));
        }

        window->windowLayoutParam()->observerRemove(
            ZFObject::EventObjectPropertyValueAfterChange(),
            d->windowLayoutMarginChangeListener);
        window->windowLayoutParam()->layoutMarginSet(margin);
        window->windowLayoutParam()->observerAdd(
            ZFObject::EventObjectPropertyValueAfterChange(),
            d->windowLayoutMarginChangeListener);
    }
    else
    {
        window->windowLayoutParam()->observerRemove(
            ZFObject::EventObjectPropertyValueAfterChange(),
            d->windowLayoutMarginChangeListener);
        window->windowLayoutParam()->tagRemove(_ZFP_ZFUIOnScreenKeyboardAutoResize_tag_layoutMarginSaved);
        window->windowLayoutParam()->layoutMarginSet(layoutMarginOld);
    }
}

static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStart(ZF_IN ZFUIWindow *window)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    if(d->windowList.isEmpty())
    {
        ZFObjectGlobalEventObserverAdd(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), d->onScreenKeyboardStateChangeListener);
    }
    d->windowList.add(window);

    window->observerAdd(ZFUIWindow::EventWindowOnShow(), d->windowOnShowListener);
    _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(window, ZFUIOnScreenKeyboardState::instanceForView(window));
}
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStop(ZF_IN ZFUIWindow *window)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);

    window->observerRemove(ZFUIWindow::EventWindowOnShow(), d->windowOnShowListener);
    window->windowLayoutParam()->observerRemove(ZFObject::EventObjectPropertyValueAfterChange(), d->windowLayoutMarginChangeListener);
    window->layoutRequest();

    ZFTypeHolder *layoutMarginSaved = window->windowLayoutParam()->tagGet<ZFTypeHolder *>(_ZFP_ZFUIOnScreenKeyboardAutoResize_tag_layoutMarginSaved);
    if(layoutMarginSaved != zfnull)
    {
        ZFUIMargin margin = layoutMarginSaved->holdedDataRef<ZFUIMargin &>();
        window->windowLayoutParam()->tagRemove(_ZFP_ZFUIOnScreenKeyboardAutoResize_tag_layoutMarginSaved);
        window->windowLayoutParam()->layoutMarginSet(margin);
    }

    d->windowList.removeElement(window);
    if(d->windowList.isEmpty())
    {
        ZFObjectGlobalEventObserverRemove(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), d->onScreenKeyboardStateChangeListener);
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_onScreenKeyboardStateChange)
{
    ZFUIOnScreenKeyboardState *state = listenerData.sender->to<ZFUIOnScreenKeyboardState *>();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    for(zfindex i = 0; i < d->windowList.count(); ++i)
    {
        ZFUIWindow *window = d->windowList[i];
        if(window->ownerSysWindow() != state->ownerSysWindow())
        {
            continue;
        }

        _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(window, state);
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnShow)
{
    ZFUIWindow *window = listenerData.sender->to<ZFUIWindow *>();
    _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(window, ZFUIOnScreenKeyboardState::instanceForView(window));
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowLayoutMarginChange)
{
    const ZFProperty *property = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFProperty *>();
    if(property != ZFPropertyAccess(ZFUIViewLayoutParam, layoutMargin))
    {
        return ;
    }

    ZFUIViewLayoutParam *layoutParam = listenerData.sender->to<ZFUIViewLayoutParam *>();
    ZFTypeHolder *layoutMarginSaved = layoutParam->tagGet<ZFTypeHolder *>(_ZFP_ZFUIOnScreenKeyboardAutoResize_tag_layoutMarginSaved);
    if(layoutMarginSaved != zfnull)
    {
        layoutMarginSaved->holdedDataRef<ZFUIMargin &>() = layoutParam->layoutMargin();
    }
}

ZF_NAMESPACE_GLOBAL_END

