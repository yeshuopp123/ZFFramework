/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUISysWindow.h"
#include "protocol/ZFProtocolZFUISysWindow.h"
#include "ZFUIRootView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFUISysWindowPrivate
{
public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFUISysWindow) *impl;
    ZFUISysWindow::NativeWindowEmbedImpl *embedImpl;
    void *nativeWindow;
    ZFUIRootView *windowRootView;
    ZFUIOrientationFlags windowOrientationFlags;
    ZFUISysWindow *modalWindowOwner;
    ZFUISysWindow *modalWindowShowing;
    ZFUIViewLayoutParam *windowLayoutParam;
    ZFListener windowLayoutParamOnChangeListener;
    zfbool nativeWindowCreated;
    zfbool nativeWindowResumed;

public:
    _ZFP_ZFUISysWindowPrivate(void)
    : impl(ZFPROTOCOL_ACCESS(ZFUISysWindow))
    , embedImpl(zfnull)
    , nativeWindow(zfnull)
    , windowRootView(zfnull)
    , windowOrientationFlags(ZFUIOrientation::e_Top)
    , modalWindowOwner(zfnull)
    , modalWindowShowing(zfnull)
    , windowLayoutParam(zfnull)
    , windowLayoutParamOnChangeListener(ZFCallbackForRawFunction(_ZFP_ZFUISysWindowPrivate::windowLayoutParamOnChange))
    , nativeWindowCreated(zffalse)
    , nativeWindowResumed(zffalse)
    {
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(windowLayoutParamOnChange)
    {
        ZFUISysWindow *sysWindow = userData->to<ZFObjectHolder *>()->holdedObj;
        if(sysWindow->nativeWindowEmbedImpl() != zfnull)
        {
            sysWindow->nativeWindowEmbedImpl()->windowLayoutParamOnChange();
        }
        else
        {
            ZFPROTOCOL_ACCESS(ZFUISysWindow)->windowLayoutParamOnChange(sysWindow);
        }
    }
};

ZFOBJECT_REGISTER(ZFUISysWindow)

ZFOBSERVER_EVENT_REGISTER(ZFUISysWindow, SysWindowOnCreate)
ZFOBSERVER_EVENT_REGISTER(ZFUISysWindow, SysWindowOnDestroy)
ZFOBSERVER_EVENT_REGISTER(ZFUISysWindow, SysWindowOnResume)
ZFOBSERVER_EVENT_REGISTER(ZFUISysWindow, SysWindowOnPause)
ZFOBSERVER_EVENT_REGISTER(ZFUISysWindow, SysWindowOnRotate)

zfautoObject ZFUISysWindow::nativeWindowEmbed(ZF_IN ZFUISysWindow::NativeWindowEmbedImpl *impl)
{
    zfautoObject tmp = ZFUISysWindow::ClassData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    ZFUISysWindow *ret = tmp.to<ZFUISysWindow *>();
    ret->d->embedImpl = impl;
    impl->_ZFP_ownerZFUISysWindow = ret;
    return zfautoObjectCreateMarkCached(ret);
}
ZFUISysWindow::NativeWindowEmbedImpl *ZFUISysWindow::nativeWindowEmbedImpl(void)
{
    return d->embedImpl;
}

static ZFUISysWindow *_ZFP_ZFUISysWindow_mainWindowRegistered = zfnull;
static ZFUISysWindow *_ZFP_ZFUISysWindow_mainWindow = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUISysWindowMainWindowCleanup, ZFLevelZFFrameworkLow)
{
    _ZFP_ZFUISysWindow_mainWindowRegistered = zfnull;
    _ZFP_ZFUISysWindow_mainWindow = zfnull;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUISysWindowMainWindowCleanup)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUISysWindow) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUISysWindow);
    if(impl != zfnull)
    {
        impl->mainWindowOnCleanup();
    }
    _ZFP_ZFUISysWindow_mainWindowRegistered = zfnull;
    _ZFP_ZFUISysWindow_mainWindow = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFUISysWindowMainWindowCleanup)
void ZFUISysWindow::mainWindowRegister(ZF_IN ZFUISysWindow *window)
{
    zfCoreAssertWithMessage(_ZFP_ZFUISysWindow_mainWindow == zfnull, zfTextA("mainWindowRegister must be called before accessing mainWindow"));
    _ZFP_ZFUISysWindow_mainWindowRegistered = window;
}

ZFUISysWindow *ZFUISysWindow::mainWindow(void)
{
    if(_ZFP_ZFUISysWindow_mainWindow == zfnull)
    {
        if(_ZFP_ZFUISysWindow_mainWindowRegistered != zfnull)
        {
            _ZFP_ZFUISysWindow_mainWindow = _ZFP_ZFUISysWindow_mainWindowRegistered;
        }
        else
        {
            _ZFP_ZFUISysWindow_mainWindow = ZFPROTOCOL_ACCESS(ZFUISysWindow)->mainWindow();
        }
    }
    return _ZFP_ZFUISysWindow_mainWindow;
}

ZFObject *ZFUISysWindow::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUISysWindowPrivate);
    d->windowRootView = zfAlloc(ZFUIRootView);
    d->windowLayoutParam = zfAllocWithoutLeakTest(ZFUIViewLayoutParam);
    d->windowLayoutParam->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
    d->impl->updateSuggestedWindowLayoutParam(this);
    d->windowLayoutParam->observerAdd(ZFUILayoutParam::EventLayoutParamOnChange(), d->windowLayoutParamOnChangeListener, this->objectHolder());
    return this;
}
void ZFUISysWindow::objectOnDealloc(void)
{
    if(d->embedImpl != zfnull)
    {
        d->embedImpl->nativeWindowOnCleanup();
    }
    zfReleaseWithoutLeakTest(d->windowLayoutParam);
    zfRelease(d->windowRootView);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void *ZFUISysWindow::nativeWindow(void)
{
    return d->nativeWindow;
}

zfbool ZFUISysWindow::nativeWindowIsCreated(void)
{
    return d->nativeWindowCreated;
}
zfbool ZFUISysWindow::nativeWindowIsResumed(void)
{
    return d->nativeWindowResumed;
}

ZFUIOrientationEnum ZFUISysWindow::windowOrientation(void)
{
    if(d->embedImpl != zfnull)
    {
        return d->embedImpl->windowOrientation();
    }
    else
    {
        return d->impl->windowOrientation(this);
    }
}
void ZFUISysWindow::windowOrientationFlagsSet(ZF_IN const ZFUIOrientationFlags &windowOrientationFlags)
{
    ZFEnumValue tmp = 0;
    if(ZFBitTest(windowOrientationFlags.enumValue(), ZFUIOrientation::e_Left))
    {
        ZFBitSet(tmp, ZFUIOrientation::e_Left);
    }
    if(ZFBitTest(windowOrientationFlags.enumValue(), ZFUIOrientation::e_Top))
    {
        ZFBitSet(tmp, ZFUIOrientation::e_Top);
    }
    if(ZFBitTest(windowOrientationFlags.enumValue(), ZFUIOrientation::e_Right))
    {
        ZFBitSet(tmp, ZFUIOrientation::e_Right);
    }
    if(ZFBitTest(windowOrientationFlags.enumValue(), ZFUIOrientation::e_Bottom))
    {
        ZFBitSet(tmp, ZFUIOrientation::e_Bottom);
    }
    if(d->windowOrientationFlags != tmp)
    {
        d->windowOrientationFlags = tmp;
        if(d->embedImpl != zfnull)
        {
            d->embedImpl->windowOrientationFlagsSet(tmp);
        }
        else
        {
            if(this->nativeWindow() != zfnull)
            {
                d->impl->windowOrientationFlagsSet(this, tmp);
            }
        }
    }
}
const ZFUIOrientationFlags &ZFUISysWindow::windowOrientationFlags(void)
{
    return d->windowOrientationFlags;
}

ZFUISysWindow *ZFUISysWindow::modalWindowShow(void)
{
    zfCoreAssertWithMessage(d->modalWindowShowing == zfnull, zfTextA("already has a showing modal window, you must finish it first"));
    zfCoreAssertWithMessage(this->nativeWindow() != zfnull, zfTextA("you can only create modal window after ZFUISysWindow created, see ZFUISysWindow::nativeWindowIsCreated"));

    ZFUISysWindow *modalWindow = d->impl->modalWindowShow(this);
    d->modalWindowShowing = modalWindow;
    modalWindow->d->modalWindowOwner = this;
    return modalWindow;
}
void ZFUISysWindow::modalWindowFinish(void)
{
    zfCoreAssertWithMessage(d->modalWindowOwner != zfnull, zfTextA("not a modal window"));

    d->impl->modalWindowFinish(d->modalWindowOwner, this);
    d->modalWindowOwner->d->modalWindowShowing = zfnull;
    d->modalWindowOwner = zfnull;

    zfRelease(this);
}
ZFUISysWindow *ZFUISysWindow::modalWindowGetShowing(void)
{
    return d->modalWindowShowing;
}
ZFUISysWindow *ZFUISysWindow::modalWindowGetOwner(void)
{
    return d->modalWindowOwner;
}

ZFUIViewLayoutParam *ZFUISysWindow::windowLayoutParam(void)
{
    return d->windowLayoutParam;
}
ZFUIRootView *ZFUISysWindow::rootView(void)
{
    return d->windowRootView;
}

ZFUIRect ZFUISysWindow::_ZFP_ZFUISysWindow_measureWindow(ZF_IN const ZFUIRect &rootRefRect)
{
    ZFUIRect ret = ZFUIRectApplyScale(ZFUIViewLayoutParam::layoutParamApply(
            ZFUIRectApplyScaleReversely(rootRefRect, this->rootView()->scaleGetFixed()),
            this->rootView(),
            this->windowLayoutParam()),
        this->rootView()->scaleGetFixed());
    return ret;
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onCreate(ZF_IN void *nativeWindow)
{
    zfCoreAssertWithMessage(!d->nativeWindowCreated, zfTextA("window already created"));

    d->nativeWindow = nativeWindow;
    d->nativeWindowCreated = zftrue;

    if(d->embedImpl != zfnull)
    {
        d->embedImpl->windowOrientationFlagsSet(d->windowOrientationFlags);
        d->embedImpl->nativeWindowOnRootViewAdd();
    }
    else
    {
        d->impl->windowOrientationFlagsSet(this, d->windowOrientationFlags);
        d->impl->nativeWindowOnRootViewAdd(this);
    }

    this->observerNotify(ZFUISysWindow::EventSysWindowOnCreate());
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onDestroy(void)
{
    zfCoreAssertWithMessage(d->nativeWindowCreated, zfTextA("window not created"));
    zfCoreAssertWithMessage(!d->nativeWindowResumed, zfTextA("window still resumed"));
    d->nativeWindowCreated = zffalse;

    if(d->embedImpl != zfnull)
    {
        d->embedImpl->nativeWindowOnRootViewRemove();
    }
    else
    {
        d->impl->nativeWindowOnRootViewRemove(this);
    }

    this->observerNotify(ZFUISysWindow::EventSysWindowOnDestroy());

    if(this == _ZFP_ZFUISysWindow_mainWindow)
    {
        d->impl->mainWindowOnDestroy();
    }
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onResume(void)
{
    zfCoreAssertWithMessage(d->nativeWindowCreated, zfTextA("window not created"));
    zfCoreAssertWithMessage(!d->nativeWindowResumed, zfTextA("window already resumed"));

    d->nativeWindowResumed = zftrue;
    this->observerNotify(ZFUISysWindow::EventSysWindowOnResume());
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onPause(void)
{
    zfCoreAssertWithMessage(d->nativeWindowCreated, zfTextA("window not created"));
    zfCoreAssertWithMessage(d->nativeWindowResumed, zfTextA("window already paused"));

    d->nativeWindowResumed = zffalse;
    this->observerNotify(ZFUISysWindow::EventSysWindowOnPause());
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onRotate(void)
{
    zfCoreAssertWithMessage(d->nativeWindowCreated, zfTextA("window not created"));
    zfCoreAssertWithMessage(d->nativeWindowResumed, zfTextA("window not resumed"));
    this->observerNotify(ZFUISysWindow::EventSysWindowOnRotate());
}

ZF_NAMESPACE_GLOBAL_END

