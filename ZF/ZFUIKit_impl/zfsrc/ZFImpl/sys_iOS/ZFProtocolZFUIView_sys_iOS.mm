/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIKeyboardState.h"

#define _ZFP_ZFUIViewImpl_sys_iOS_VIEW_TREE_DEBUG ZF_ENV_DEBUG

#if ZF_ENV_sys_iOS

#import <objc/runtime.h>

@interface _ZFP_ZFUIViewImpl_sys_iOS_View : UIView
#if _ZFP_ZFUIViewImpl_sys_iOS_VIEW_TREE_DEBUG
@property (nonatomic, strong) NSString *text;
#endif
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *_ZFP_impl;
@property (nonatomic, assign) ZFUIView *_ZFP_ownerZFUIView;
@property (nonatomic, assign) CGRect _ZFP_frame;
@property (nonatomic, strong) UIView *_ZFP_nativeImplView;
@property (nonatomic, assign) BOOL _ZFP_uiEnable;
@property (nonatomic, assign) BOOL _ZFP_uiEnableTree;
@property (nonatomic, assign) BOOL _ZFP_layoutRequested;
@property (nonatomic, assign) ZFCoreArrayPOD<UITouch *> _ZFP_mouseRecords;
@property (nonatomic, assign) BOOL _ZFP_ZFUIViewFocus_viewFocusable;
@end
@implementation _ZFP_ZFUIViewImpl_sys_iOS_View
// ============================================================
// init and dealloc
- (id)init
{
    self = [super init];

    self.autoresizesSubviews = NO;
    self.multipleTouchEnabled = YES;
    self.exclusiveTouch = YES;
    self.clipsToBounds = YES;

    // status init
    self._ZFP_uiEnable = YES;
    self._ZFP_uiEnableTree = YES;
    self._ZFP_layoutRequested = NO;

    self._ZFP_ZFUIViewFocus_viewFocusable = NO;

    return self;
}
- (void)dealloc
{
    zfCoreAssert(self._ZFP_nativeImplView == nil);

#if _ZFP_ZFUIViewImpl_sys_iOS_VIEW_TREE_DEBUG
    self.text = nil;
#endif
}

#if _ZFP_ZFUIViewImpl_sys_iOS_VIEW_TREE_DEBUG
- (void)set_ZFP_ownerZFUIView:(ZFUIView *)newOwnerZFUIView ZFImpl_sys_iOS_overrideProperty
{
    ZFLISTENER_LOCAL(viewIdListener, {
        const ZFProperty *property = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFProperty *>();
        if(property == ZFPropertyAccess(ZFUIView, viewId))
        {
            ZFUIView *view = ZFCastZFObjectUnchecked(ZFUIView *, listenerData.sender);
            const zfstring &viewId = view->viewId();
            ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).text
                = (viewId.isEmpty() ? nil : [NSString stringWithUTF8String:view->viewId().cString()]);
        }
    })
    if(self->__ZFP_ownerZFUIView != zfnull)
    {
        self->__ZFP_ownerZFUIView->observerRemove(ZFObject::EventObjectPropertyValueAfterChange(), viewIdListener);
    }
    self->__ZFP_ownerZFUIView = newOwnerZFUIView;
    if(newOwnerZFUIView != zfnull)
    {
        newOwnerZFUIView->observerAdd(ZFObject::EventObjectPropertyValueAfterChange(), viewIdListener);
    }
}
#endif

// ============================================================
// ui and tree enable
- (void)set_ZFP_uiEnableTree:(BOOL)newUIEnableTree ZFImpl_sys_iOS_overrideProperty
{
    self->__ZFP_uiEnableTree = newUIEnableTree;
    self.userInteractionEnabled = newUIEnableTree;
}
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    UIView *tmp = [super hitTest:point withEvent:event];
    if(!self._ZFP_uiEnable && tmp == self)
    {
        return nil;
    }
    else
    {
        return tmp;
    }
}

// ============================================================
// frame and layout
- (void)set_ZFP_frame:(CGRect)newFrame ZFImpl_sys_iOS_overrideProperty
{
    self->__ZFP_frame = newFrame;
    self.frame = newFrame;
}
- (void)setNeedsLayout
{
    if(!self._ZFP_layoutRequested)
    {
        self._ZFP_layoutRequested = YES;
        if(self._ZFP_ownerZFUIView != zfnull && self._ZFP_ownerZFUIView->viewParent() == zfnull)
        {
            self._ZFP_impl->notifyNeedLayout(self._ZFP_ownerZFUIView);
        }
    }
    [super setNeedsLayout];
}
- (CGSize)sizeThatFits:(CGSize)size
{
    return self._ZFP_frame.size;
}
- (void)layoutSubviews
{
    self._ZFP_layoutRequested = NO;
    [super layoutSubviews];
    if(self._ZFP_ownerZFUIView != zfnull && self._ZFP_ownerZFUIView->viewParent() == zfnull)
    {
        self._ZFP_impl->notifyLayoutRootView(self._ZFP_ownerZFUIView, ZFImpl_sys_iOS_ZFUIKit_ZFUIRectFromCGRect(self.frame));
    }

    NSArray *children = self.subviews;
    for(NSUInteger i = 0; i < [children count]; ++i)
    {
        UIView *child = [children objectAtIndex:i];
        if([child isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
        {
            child.frame = ((_ZFP_ZFUIViewImpl_sys_iOS_View *)child)._ZFP_frame;
        }
        else if(child == self._ZFP_nativeImplView)
        {
            ZFUIRect rect;
            self._ZFP_impl->notifyLayoutNativeImplView(self._ZFP_ownerZFUIView, rect);
            child.frame = ZFImpl_sys_iOS_ZFUIKit_ZFUIRectToCGRect(rect);
        }
        else
        {
            child.frame = self.bounds;
        }
    }
}

// ============================================================
// touches
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesBegan:touches withEvent:event];

    if(self._ZFP_ownerZFUIView == zfnull)
    {
        return ;
    }

    for(UITouch *touch in touches)
    {
        self._ZFP_mouseRecords.add(touch);

        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, ev);
        ev->mouseId = (zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_MouseDown;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint([touch locationInView:self]);
        self._ZFP_impl->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesMoved:touches withEvent:event];

    if(self._ZFP_ownerZFUIView == zfnull)
    {
        return ;
    }

    for(UITouch *touch in touches)
    {
        if(self._ZFP_mouseRecords.find(touch, ZFComparerCheckEqualOnly) == zfindexMax)
        {
            self._ZFP_mouseRecords.add(touch);
        }
    }

    for(zfindex i = 0; i < self._ZFP_mouseRecords.count(); ++i)
    {
        UITouch *touch = self._ZFP_mouseRecords.get(i);

        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, ev);
        ev->mouseId = (zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_MouseMove;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint([touch locationInView:self]);
        self._ZFP_impl->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesEnded:touches withEvent:event];

    if(self._ZFP_ownerZFUIView == zfnull)
    {
        return ;
    }

    for(UITouch *touch in touches)
    {
        zfindex index = self._ZFP_mouseRecords.find(touch, ZFComparerCheckEqualOnly);
        if(index != zfindexMax)
        {
            self._ZFP_mouseRecords.remove(index);
        }

        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, ev);
        ev->mouseId = (zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_MouseUp;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint([touch locationInView:self]);
        self._ZFP_impl->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesCancelled:touches withEvent:event];

    if(self._ZFP_ownerZFUIView == zfnull)
    {
        return ;
    }

    for(UITouch *touch in touches)
    {
        zfindex index = self._ZFP_mouseRecords.find(touch, ZFComparerCheckEqualOnly);
        if(index != zfindexMax)
        {
            self._ZFP_mouseRecords.remove(index);
        }

        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, ev);
        ev->mouseId = (zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_MouseCancel;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint([touch locationInView:self]);
        self._ZFP_impl->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}

// ============================================================
// ZFUIViewFocus
- (BOOL)canBecomeFirstResponder
{
    return self._ZFP_ZFUIViewFocus_viewFocusable;
}
- (BOOL)becomeFirstResponder
{
    BOOL ret = NO;
    if(self._ZFP_nativeImplView != nil && !self._ZFP_nativeImplView.isFirstResponder)
    {
        ret = [self._ZFP_nativeImplView becomeFirstResponder];
    }
    else if(!self.isFirstResponder)
    {
        ret = [super becomeFirstResponder];
    }
    return ret;
}
- (BOOL)resignFirstResponder
{
    BOOL ret = NO;
    if(self._ZFP_nativeImplView != nil && self._ZFP_nativeImplView.isFirstResponder)
    {
        ret = [self._ZFP_nativeImplView resignFirstResponder];
    }
    else if(self.isFirstResponder)
    {
        ret = [super resignFirstResponder];
    }
    return ret;
}
@end

static void _ZFP_ZFUIViewImpl_sys_iOS_notifyViewFocusChanged(ZF_IN UIView *nativeImplView)
{
    _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = nil;
    if([nativeImplView isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
    {
        nativeView = (_ZFP_ZFUIViewImpl_sys_iOS_View *)nativeImplView;
    }
    else if([nativeImplView.superview isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
    {
        nativeView = (_ZFP_ZFUIViewImpl_sys_iOS_View *)nativeImplView.superview;
    }
    else if([nativeImplView.superview.superview isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
    {
        nativeView = (_ZFP_ZFUIViewImpl_sys_iOS_View *)nativeImplView.superview.superview;
    }

    if(nativeView._ZFP_ownerZFUIView != zfnull)
    {
        ZFProtocolZFUIViewFocus_notifyViewFocusChanged(nativeView._ZFP_ownerZFUIView);
    }
}

@implementation UIView (_ZFP_ZFUIViewImpl_sys_iOS_MethodSwizzling)
- (BOOL)_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_becomeFirstResponder
{
    BOOL old = self.isFirstResponder;
    BOOL ret = [self _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_becomeFirstResponder];
    if(!old && self.isFirstResponder)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_notifyViewFocusChanged(self);
    }
    return ret;
}
- (BOOL)_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_resignFirstResponder
{
    BOOL old = self.isFirstResponder;
    BOOL ret = [self _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_resignFirstResponder];
    if(old && !self.isFirstResponder)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_notifyViewFocusChanged(self);
    }
    return ret;
}
@end
static void _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzlePrepare(void)
{
    {
        Method methodOrg =  class_getInstanceMethod([UIView class], @selector(becomeFirstResponder));
        Method methodNew = class_getInstanceMethod([UIView class], @selector(_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_becomeFirstResponder));
        method_exchangeImplementations(methodOrg, methodNew);
    }
    {
        Method methodOrg =  class_getInstanceMethod([UIView class], @selector(resignFirstResponder));
        Method methodNew = class_getInstanceMethod([UIView class], @selector(_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_resignFirstResponder));
        method_exchangeImplementations(methodOrg, methodNew);
    }
}

ZF_NAMESPACE_GLOBAL_BEGIN
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_iOS, ZFUIView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UIView"))

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        static zfbool _needMethodSwizzling = zftrue;
        if(_needMethodSwizzling)
        {
            _needMethodSwizzling = zffalse;
            _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzlePrepare();
        }
    }
    zfoverride
    virtual void protocolOnInitFinish(void)
    {
        zfsuper::protocolOnInitFinish();
        ZFUIKeyboardStateBuiltinImplRegister();
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        ZFUIKeyboardStateBuiltinImplUnregister();
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = [_ZFP_ZFUIViewImpl_sys_iOS_View new];
        nativeView._ZFP_impl = this;
        nativeView._ZFP_ownerZFUIView = view;
        return (__bridge_retained void *)nativeView;
    }
    virtual void nativeViewDestroy(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeView)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeViewTmp = (__bridge_transfer _ZFP_ZFUIViewImpl_sys_iOS_View *)nativeView;
        nativeViewTmp._ZFP_ownerZFUIView = zfnull;
        nativeViewTmp = nil;
    }

    virtual void nativeImplViewSet(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeImplViewOld,
                                   ZF_IN void *nativeImplView,
                                   ZF_IN zfindex virtualIndex)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        if(nativeView._ZFP_nativeImplView != nil)
        {
            [nativeView._ZFP_nativeImplView removeFromSuperview];
        }
        nativeView._ZFP_nativeImplView = (__bridge UIView *)nativeImplView;
        if(nativeView._ZFP_nativeImplView != nil)
        {
            [nativeView insertSubview:nativeView._ZFP_nativeImplView atIndex:virtualIndex];
        }
    }
    virtual zffloat nativeViewScaleForImpl(ZF_IN void *nativeView)
    {
        return 1;
    }
    virtual zffloat nativeViewScaleForPhysicalPixel(ZF_IN void *nativeView)
    {
        UIScreen *ownerScreen = ((__bridge UIView *)nativeView).window.screen;
        return (zffloat)((ownerScreen == nil) ? [UIScreen mainScreen].scale : ownerScreen.scale);
    }

    // ============================================================
    // properties
public:
    virtual void viewVisibleSet(ZF_IN ZFUIView *view,
                                ZF_IN zfbool viewVisible)
    {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).hidden = !viewVisible;
    }
    virtual void viewAlphaSet(ZF_IN ZFUIView *view,
                              ZF_IN zffloat viewAlpha)
    {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).alpha = viewAlpha;
    }
    virtual void viewUIEnableSet(ZF_IN ZFUIView *view,
                                 ZF_IN zfbool viewUIEnable)
    {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView())._ZFP_uiEnable = viewUIEnable;
    }
    virtual void viewUIEnableTreeSet(ZF_IN ZFUIView *view,
                                     ZF_IN zfbool viewUIEnableTree)
    {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView())._ZFP_uiEnableTree = viewUIEnableTree;
    }
    virtual void viewBackgroundColorSet(ZF_IN ZFUIView *view,
                                        ZF_IN const ZFUIColor &viewBackgroundColor)
    {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).backgroundColor = ZFImpl_sys_iOS_ZFUIKit_ZFUIColorToUIColor(viewBackgroundColor);
    }

public:
    virtual void childAdd(ZF_IN ZFUIView *parent,
                          ZF_IN ZFUIView *child,
                          ZF_IN zfindex virtualIndex,
                          ZF_IN ZFUIViewChildLayerEnum childLayer,
                          ZF_IN zfindex childLayerIndex)
    {
        [(__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)parent->nativeView()
            insertSubview:(__bridge UIView *)child->nativeView()
            atIndex:virtualIndex];
    }
    virtual void childRemove(ZF_IN ZFUIView *parent,
                             ZF_IN ZFUIView *child,
                             ZF_IN zfindex virtualIndex,
                             ZF_IN ZFUIViewChildLayerEnum childLayer,
                             ZF_IN zfindex childLayerIndex)
    {
        [(__bridge UIView *)child->nativeView() removeFromSuperview];
    }

public:
    virtual void viewFrameSet(ZF_IN ZFUIView *view,
                              ZF_IN const ZFUIRect &rect)
    {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView())._ZFP_frame =
            ZFImpl_sys_iOS_ZFUIKit_ZFUIRectToCGRect(rect);
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view)
    {
        // iOS needs to setNeedsLayout recursively
        UIView *nativeView = (__bridge UIView *)view->nativeView();
        do
        {
            [nativeView setNeedsLayout];
            nativeView = nativeView.superview;
        } while(nativeView != nil);
    }

    virtual void measureNativeView(ZF_OUT ZFUISize &ret,
                                   ZF_IN void *nativeView,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
    {
        ret = ZFUISizeMake(zfmMax(sizeHint.width, 0), zfmMax(sizeHint.height, 0));
        ret = ZFImpl_sys_iOS_ZFUIKit_ZFUISizeFromCGSize(
            [(__bridge UIView *)nativeView sizeThatFits:ZFImpl_sys_iOS_ZFUIKit_ZFUISizeToCGSize(ret)]);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewImpl_sys_iOS)

// ============================================================
// ZFUIViewFocus
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewFocusImpl_sys_iOS, ZFUIViewFocus, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UIView"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("iOS:UIView"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewFocusableSet(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocusable)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        nativeView._ZFP_ZFUIViewFocus_viewFocusable = viewFocusable;
        if(!viewFocusable)
        {
            [nativeView resignFirstResponder];
        }
    }
    virtual zfbool viewFocused(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        return (nativeView.isFirstResponder
            || (nativeView._ZFP_nativeImplView != nil && nativeView._ZFP_nativeImplView.isFirstResponder));
    }
    virtual void viewFocusRequest(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocus)
    {
        UIView *nativeView = (__bridge UIView *)view->nativeView();
        if(viewFocus)
        {
            [nativeView becomeFirstResponder];
        }
        else
        {
            [nativeView resignFirstResponder];
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewFocusImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewFocusImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

