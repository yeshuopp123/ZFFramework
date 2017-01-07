/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIScrollView.h"

#if ZF_ENV_sys_iOS

static zftimet _ZFP_ZFUIScrollViewImpl_sys_iOS_timestamp(void)
{
    return (zftimet)([[NSDate date] timeIntervalSince1970] * 1000);
}

@interface _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView : UIScrollView<UIScrollViewDelegate>
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUIScrollView) *_ZFP_impl;
@property (nonatomic, assign) ZFUIScrollView *_ZFP_ownerZFUIScrollView;
@property (nonatomic, assign) CGPoint _ZFP_mouseDrag;
@property (nonatomic, assign) CGPoint _ZFP_mouseDragPrevPos;
@property (nonatomic, assign) zfbool _ZFP_mouseDragOverride;
@property (nonatomic, assign) ZFUIRect _ZFP_scrollContentFrame;
@property (nonatomic, assign) zfbool _ZFP_scrollContentFrameOverrideFlag;
@property (nonatomic, strong) NSTimer *_ZFP_scrollImplScrollAnimationTimer;
- (void)_ZFP_scrollContentFrameFix;
- (void)_ZFP_scrollImplScrollAnimationTimerEvent:(id)dummy;
@end
@implementation _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView
- (id)init
{
    self = [super init];

    self.delegate = self;

    self.bounces = YES;
    self.alwaysBounceHorizontal = NO;
    self.alwaysBounceVertical = NO;

    self.showsHorizontalScrollIndicator = NO;
    self.showsVerticalScrollIndicator = NO;

    self.exclusiveTouch = YES;

    return self;
}

- (void)setFrame:(CGRect)frame
{
    self._ZFP_scrollContentFrameOverrideFlag = zftrue;
    [super setFrame:frame];
    self._ZFP_scrollContentFrameOverrideFlag = zffalse;
}
- (void)setContentOffset:(CGPoint)contentOffset
{
    if(!self._ZFP_scrollContentFrameOverrideFlag)
    {
        [super setContentOffset:contentOffset];
    }
}

- (void)setContentOffset:(CGPoint)contentOffset animated:(BOOL)animated
{
    if(!self._ZFP_scrollContentFrameOverrideFlag)
    {
        [super setContentOffset:contentOffset animated:animated];
    }
}

- (void)set_ZFP_scrollContentFrame:(ZFUIRect)newScrollContentFrame ZFImpl_sys_iOS_overrideProperty
{
    self->__ZFP_scrollContentFrame = newScrollContentFrame;

    self._ZFP_mouseDragOverride = zftrue;
    self.contentSize = ZFImpl_sys_iOS_ZFUIKit_ZFUISizeToCGSize(newScrollContentFrame.size);
    [self setContentOffset:CGPointMake(-newScrollContentFrame.point.x, -newScrollContentFrame.point.y) animated:NO];
    self._ZFP_mouseDragPrevPos = self.contentOffset;
    self._ZFP_mouseDragOverride = zffalse;
}

// scroll logic
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    self._ZFP_mouseDrag = point;
    UITouch *touch = [[event allTouches] anyObject];
    if(touch.phase == UITouchPhaseBegan)
    {
        self._ZFP_ownerZFUIScrollView->scrollContentFrameSet(self._ZFP_ownerZFUIScrollView->scrollContentFrame());
    }
    return [super hitTest:point withEvent:event];
}
- (BOOL)touchesShouldCancelInContentView:(UIView *)view
{
    return NO;
}
- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
    [self _ZFP_scrollContentFrameFix];
    self._ZFP_mouseDragPrevPos = self.contentOffset;
    self._ZFP_mouseDragOverride = zffalse;
    self._ZFP_impl->notifyScrollViewDragBegin(
        self._ZFP_ownerZFUIScrollView,
        ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint(self._ZFP_mouseDrag),
        _ZFP_ZFUIScrollViewImpl_sys_iOS_timestamp());
}
- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if(self.dragging)
    {
        if(!self._ZFP_mouseDragOverride)
        {
            self._ZFP_mouseDrag = CGPointMake(
                self._ZFP_mouseDrag.x - (self.contentOffset.x - self._ZFP_mouseDragPrevPos.x),
                self._ZFP_mouseDrag.y - (self.contentOffset.y - self._ZFP_mouseDragPrevPos.y));

            self._ZFP_impl->notifyScrollViewDrag(
                self._ZFP_ownerZFUIScrollView,
                ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint(self._ZFP_mouseDrag),
                _ZFP_ZFUIScrollViewImpl_sys_iOS_timestamp());
        }
    }
    self._ZFP_mouseDragPrevPos = self.contentOffset;
}
- (void)scrollViewWillEndDragging:(UIScrollView *)scrollView withVelocity:(CGPoint)velocity targetContentOffset:(inout CGPoint *)targetContentOffset
{
    *targetContentOffset = self.contentOffset;
    // iOS9 or later would have strange decerlating logic,
    // which would cause shake when bounce back,
    // force update contentOffset once, to prevent shake
    dispatch_async(dispatch_get_main_queue(), ^{
        [self setContentOffset:self.contentOffset animated:NO];
    });
}
- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
{
    self._ZFP_impl->notifyScrollViewDragEnd(self._ZFP_ownerZFUIScrollView, _ZFP_ZFUIScrollViewImpl_sys_iOS_timestamp(), zftrue);
}
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    [self _ZFP_scrollContentFrameFix];
}
- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)scrollView
{
    [self _ZFP_scrollContentFrameFix];
}
- (void)_ZFP_scrollContentFrameFix
{
    self._ZFP_impl->scrollViewScrollContentFrameSetForImpl(
        self._ZFP_ownerZFUIScrollView,
        ZFUIRectMake(
            zfmRound(-self.contentOffset.x),
            zfmRound(-self.contentOffset.y),
            zfmRound(self.contentSize.width),
            zfmRound(self.contentSize.height)));
}
- (void)_ZFP_scrollImplScrollAnimationTimerEvent:(id)dummy
{
    if(self._ZFP_scrollImplScrollAnimationTimer != nil)
    {
        self._ZFP_impl->notifyScrollViewScrollAnimation(self._ZFP_ownerZFUIScrollView, _ZFP_ZFUIScrollViewImpl_sys_iOS_timestamp());
    }
}

- (BOOL)scrollViewShouldScrollToTop:(UIScrollView *)scrollView
{
    return NO;
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIScrollViewImpl_sys_iOS, ZFUIScrollView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UIScrollView"))

public:
    virtual void *nativeScrollViewCreate(ZF_IN ZFUIScrollView *scrollView)
    {
        _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView *nativeScrollView = [_ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView new];
        nativeScrollView._ZFP_impl = this;
        nativeScrollView._ZFP_ownerZFUIScrollView = scrollView;
        return (__bridge_retained void *)nativeScrollView;
    }
    virtual void nativeScrollViewDestroy(ZF_IN ZFUIScrollView *scrollView,
                                         ZF_IN void *nativeScrollView)
    {
        UIView *tmp = (__bridge_transfer UIView *)nativeScrollView;
        tmp = nil;
    }

public:
    virtual void scrollViewScrollEnableSet(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN zfbool scrollEnable)
    {
        ((__bridge _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView *)scrollView->nativeImplView()).scrollEnabled = scrollEnable;
    }
    virtual void scrollViewScrollBounceSet(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN zfbool scrollBounceHorizontal,
                                           ZF_IN zfbool scrollBounceVertical,
                                           ZF_IN zfbool scrollBounceHorizontalAlways,
                                           ZF_IN zfbool scrollBounceVerticalAlways)
    {
        _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView *nativeScrollView = (__bridge _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView *)scrollView->nativeImplView();
        nativeScrollView.bounces = (scrollBounceHorizontal || scrollBounceVertical);
        nativeScrollView.alwaysBounceHorizontal = scrollBounceHorizontalAlways;
        nativeScrollView.alwaysBounceVertical = scrollBounceVerticalAlways;
    }
    virtual void scrollViewScrollContentFrameSet(ZF_IN ZFUIScrollView *scrollView,
                                                 ZF_IN const ZFUIRect &frame)
    {
        _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView *nativeScrollView = (__bridge _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView *)scrollView->nativeImplView();
        nativeScrollView._ZFP_scrollContentFrame = frame;
    }
    virtual zftimet scrollViewScrollAnimationStart(ZF_IN ZFUIScrollView *scrollView)
    {
        _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView *nativeScrollView = (__bridge _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView *)scrollView->nativeImplView();
        nativeScrollView._ZFP_scrollImplScrollAnimationTimer = [NSTimer timerWithTimeInterval:0.03f target:nativeScrollView selector:@selector(_ZFP_scrollImplScrollAnimationTimerEvent:) userInfo:nil repeats:YES];
        [[NSRunLoop mainRunLoop] addTimer:nativeScrollView._ZFP_scrollImplScrollAnimationTimer forMode:NSRunLoopCommonModes];
        return _ZFP_ZFUIScrollViewImpl_sys_iOS_timestamp();
    }
    virtual void scrollViewScrollAnimationStop(ZF_IN ZFUIScrollView *scrollView)
    {
        _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView *nativeScrollView = (__bridge _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView *)scrollView->nativeImplView();
        [nativeScrollView._ZFP_scrollImplScrollAnimationTimer invalidate];
        nativeScrollView._ZFP_scrollImplScrollAnimationTimer = nil;
    }

public:
    virtual void scrollChildAdd(ZF_IN ZFUIScrollView *parent,
                                ZF_IN ZFUIView *child,
                                ZF_IN zfindex atIndex)
    {
        _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView *nativeScrollView = (__bridge _ZFP_ZFUIScrollViewImpl_sys_iOS_ScrollView *)parent->nativeImplView();
        UIView *nativeChildView = (__bridge UIView *)child->nativeView();
        [nativeScrollView insertSubview:nativeChildView atIndex:atIndex];
    }
    virtual void scrollChildRemove(ZF_IN ZFUIScrollView *parent,
                                   ZF_IN ZFUIView *child,
                                   ZF_IN zfindex atIndex)
    {
        [(__bridge UIView *)child->nativeView() removeFromSuperview];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIScrollViewImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIScrollViewImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

