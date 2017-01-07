/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"
#include "ZFImpl/sys_iOS/ZFMainEntry_sys_iOS.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow : UIViewController
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUISysWindow) *impl;
@property (nonatomic, assign) ZFUISysWindow *ownerZFUISysWindow;
@property (nonatomic, assign) ZFUIOrientationEnum windowOrientation;
@property (nonatomic, assign) ZFUIOrientationFlags windowOrientationFlags;
- (void)_ZFP_updateLayout;
@end
@implementation _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow
// ============================================================
- (void)_ZFP_updateLayout
{
    if(self.ownerZFUISysWindow != zfnull)
    {
        UIView *nativeRootView = (__bridge UIView *)self.ownerZFUISysWindow->rootView()->nativeView();
        nativeRootView.frame = ZFImpl_sys_iOS_ZFUIKit_ZFUIRectToCGRect(self.impl->notifyMeasureWindow(
            self.ownerZFUISysWindow, ZFImpl_sys_iOS_ZFUIKit_ZFUIRectFromCGRect(self.view.bounds)));
    }
}
// ============================================================
// init and dealloc
- (id)init
{
    self = [super init];
    if(self)
    {
        self.windowOrientation = ZFUIOrientation::e_Top;
        self.windowOrientationFlags = ZFUIOrientation::e_Top;
    }
    return self;
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    [super willAnimateRotationToInterfaceOrientation:toInterfaceOrientation duration:duration];
    switch(toInterfaceOrientation)
    {
        case UIInterfaceOrientationPortrait:
            self.windowOrientation = ZFUIOrientation::e_Top;
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            self.windowOrientation = ZFUIOrientation::e_Bottom;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            self.windowOrientation = ZFUIOrientation::e_Left;
            break;
        case UIInterfaceOrientationLandscapeRight:
            self.windowOrientation = ZFUIOrientation::e_Right;
            break;
        default:
            self.windowOrientation = ZFUIOrientation::e_Top;
            break;
    }
    if(self.ownerZFUISysWindow != zfnull)
    {
        self.impl->notifyOnRotate(self.ownerZFUISysWindow);
    }
}
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    switch(toInterfaceOrientation)
    {
        case UIInterfaceOrientationPortrait:
            return ZFBitTest(self.windowOrientation, ZFUIOrientation::e_Top);
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            return ZFBitTest(self.windowOrientation, ZFUIOrientation::e_Bottom);
            break;
        case UIInterfaceOrientationLandscapeLeft:
            return ZFBitTest(self.windowOrientation, ZFUIOrientation::e_Left);
            break;
        case UIInterfaceOrientationLandscapeRight:
            return ZFBitTest(self.windowOrientation, ZFUIOrientation::e_Right);
        default:
            return NO;
    }
}

- (void)viewWillLayoutSubviews
{
    [super viewWillLayoutSubviews];
    [self _ZFP_updateLayout];
}
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    if(self.ownerZFUISysWindow != zfnull)
    {
        self.impl->notifyOnResume(self.ownerZFUISysWindow);
    }
}
- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    if(self.ownerZFUISysWindow != zfnull)
    {
        self.impl->notifyOnPause(self.ownerZFUISysWindow);
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUISysWindowImpl_sys_iOS, ZFUISysWindow, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UIViewController"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("iOS:UIView"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        this->_mainWindow = zfnull;
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        this->mainWindowOnCleanup();
        zfsuper::protocolOnDeallocPrepare();
    }
public:
    virtual ZFUISysWindow *mainWindow(void)
    {
        if(this->_mainWindow == zfnull)
        {
            this->_mainWindow = zfRetain(ZFUISysWindow::ClassData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE).to<ZFUISysWindow *>());
            _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *nativeWindow = [_ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow new];
            nativeWindow.ownerZFUISysWindow = this->_mainWindow;

            this->notifyOnCreate(this->_mainWindow, (__bridge void *)nativeWindow);

            ZFImpl_sys_iOS_rootWindow().rootViewController = nativeWindow;
            [ZFImpl_sys_iOS_rootWindow() makeKeyAndVisible];
        }
        return this->_mainWindow;
    }
    virtual void mainWindowOnCleanup(void)
    {
        if(this->_mainWindow != zfnull)
        {
            _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *nativeWindow = (__bridge_transfer _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)this->_mainWindow->nativeWindow();
            nativeWindow.ownerZFUISysWindow = zfnull;
            ZFImpl_sys_iOS_rootWindow().rootViewController = nil;

            if(this->_mainWindow->nativeWindowIsResumed())
            {
                this->notifyOnPause(this->_mainWindow);
            }
            zfblockedRelease(this->_mainWindow);
            this->notifyOnDestroy(this->_mainWindow);
            nativeWindow = nil;
        }
    }
    virtual void mainWindowOnDestroy(void)
    {
        this->_mainWindow = zfnull;
    }

    virtual void nativeWindowOnRootViewAdd(ZF_IN ZFUISysWindow *window)
    {
        _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *nativeWindow = (__bridge _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)window->nativeWindow();

        ZFUIView::nativeViewNotifyBeforeAdd(window->rootView(), (__bridge void *)nativeWindow.view);

        UIView *nativeRootView = (__bridge UIView *)window->rootView()->nativeView();
        [nativeWindow.view addSubview:nativeRootView];
        [nativeWindow _ZFP_updateLayout];
    }
    virtual void nativeWindowOnRootViewRemove(ZF_IN ZFUISysWindow *window)
    {
        UIView *nativeRootView = (__bridge UIView *)window->rootView()->nativeView();
        [nativeRootView removeFromSuperview];

        ZFUIView::nativeViewNotifyAfterRemove(window->rootView());
    }

    virtual ZFUISysWindow *modalWindowShow(ZF_IN ZFUISysWindow *ownerWindow)
    {
        ZFUISysWindow *modalWindow = zfRetain(ZFUISysWindow::ClassData()->newInstance(
            ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE).to<ZFUISysWindow *>());
        _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *nativeModalWindow = [_ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow new];
        nativeModalWindow.ownerZFUISysWindow = modalWindow;

        [(__bridge UIViewController *)ownerWindow->nativeWindow() presentViewController:nativeModalWindow animated:YES completion:nil];
        this->notifyOnCreate(modalWindow, (__bridge void *)nativeModalWindow);

        return modalWindow;
    }
    virtual void modalWindowFinish(ZF_IN ZFUISysWindow *ownerWindow,
                                   ZF_IN ZFUISysWindow *windowToFinish)
    {
        this->notifyOnDestroy(windowToFinish);
        [(__bridge UIViewController *)ownerWindow->nativeWindow() dismissViewControllerAnimated:YES completion:nil];
    }

    virtual void updateSuggestedWindowLayoutParam(ZF_IN ZFUISysWindow *window)
    {
        // default is fill parent, nothing to do
    }
    virtual void windowLayoutParamOnChange(ZF_IN ZFUISysWindow *window)
    {
        [(__bridge _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)window->nativeWindow() _ZFP_updateLayout];
    }

    virtual ZFUIOrientationEnum windowOrientation(ZF_IN ZFUISysWindow *window)
    {
        return ((__bridge _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)window->nativeWindow()).windowOrientation;
    }
    virtual void windowOrientationFlagsSet(ZF_IN ZFUISysWindow *window,
                                           ZF_IN const ZFUIOrientationFlags &flags)
    {
        ((__bridge _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)window->nativeWindow()).windowOrientationFlags = flags;
    }

private:
    ZFUISysWindow *_mainWindow;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUISysWindowImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUISysWindowImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

