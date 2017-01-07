/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIScrollView.h"
#include "ZFUIScroller.h"
#include "ZFUIScrollThumb.h"
#include "protocol/ZFProtocolZFUIScrollView.h"

#include "ZFUIViewPositionOnScreen.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFUIScrollView_DEBUG_logEvent 0

ZFENUM_DEFINE(ZFUIScrollViewState)

ZFSTYLE_DEFAULT_DEFINE(ZFUIScrollView)

// ============================================================
typedef enum {
    _ZFP_ZFUIScrollViewActionDragBegin,
    _ZFP_ZFUIScrollViewActionDrag,
    _ZFP_ZFUIScrollViewActionDragEnd,
    _ZFP_ZFUIScrollViewActionScrollBegin,
    _ZFP_ZFUIScrollViewActionScroll,
    _ZFP_ZFUIScrollViewActionScrollEnd,
    _ZFP_ZFUIScrollViewActionUpdate,
} _ZFP_ZFUIScrollViewAction;

// ============================================================
// _ZFP_ZFUIScrollViewPrivate
zfclass _ZFP_ZFUIScrollViewPrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(_ZFP_ZFUIScrollViewPrivate, ZFObject)

public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIScrollView) *impl;
    ZFUIScrollView *pimplOwner;
    ZFUIScrollViewStateEnum state;
    zfbool scrollContentFrameOverrideFlag;
    zfbool scrollAniTimerStarted;
    zftimet scrollAniLastTime;

    ZFUIScroller *xScroll;
    zfbool xScrollEnable;
    zfint xScrollDragPrevPos;
    zfint xScrollDragCurPos;
    zfbool xScrollAniTimerStarted;

    ZFUIScroller *yScroll;
    zfbool yScrollEnable;
    zfint yScrollDragPrevPos;
    zfint yScrollDragCurPos;
    zfbool yScrollAniTimerStarted;

    ZFCoreQueuePOD<_ZFP_ZFUIScrollViewAction> scrollerActions;
    zfbool scrollerActionRunning;

    zfbool scrollThumbNeedUpdate;
    ZFUIMargin scrollAreaMargin;

    const ZFClass *xScrollThumbClass;
    ZFUIScrollThumb *xScrollThumb;

    const ZFClass *yScrollThumbClass;
    ZFUIScrollThumb *yScrollThumb;

    zfint autoScrollSpeedX;
    zfint autoScrollSpeedY;
    zfbool autoScrollStartFlag;

    zfint scrollOverrideFlag;

protected:
    _ZFP_ZFUIScrollViewPrivate(void)
    : impl(ZFPROTOCOL_ACCESS(ZFUIScrollView))
    , pimplOwner(zfnull)
    , state(ZFUIScrollViewState::e_Idle)
    , scrollContentFrameOverrideFlag(zffalse)
    , scrollAniTimerStarted(zffalse)
    , scrollAniLastTime(0)
    , xScroll(zfnull)
    , xScrollEnable(zftrue)
    , xScrollDragPrevPos(0)
    , xScrollDragCurPos(0)
    , xScrollAniTimerStarted(zffalse)
    , yScroll(zfnull)
    , yScrollEnable(zftrue)
    , yScrollDragPrevPos(0)
    , yScrollDragCurPos(0)
    , yScrollAniTimerStarted(zffalse)
    , scrollerActions()
    , scrollerActionRunning(zffalse)
    , scrollThumbNeedUpdate(zftrue)
    , scrollAreaMargin(ZFUIMarginZero)
    , xScrollThumbClass(zfnull)
    , xScrollThumb(zfnull)
    , yScrollThumbClass(zfnull)
    , yScrollThumb(zfnull)
    , autoScrollSpeedX(0)
    , autoScrollSpeedY(0)
    , autoScrollStartFlag(zffalse)
    , scrollOverrideFlag(0)
    {
    }

    // ============================================================
    // scroller logic
public:
    void scrollerInit(void)
    {
        this->xScroll->scrollAniStartCallback = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, x_scrollAniStart));
        this->xScroll->scrollAniStopCallback = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, x_scrollAniStop));
        this->yScroll->scrollAniStartCallback = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, y_scrollAniStart));
        this->yScroll->scrollAniStopCallback = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, y_scrollAniStop));
    }

public:
    void scrollerUpdate(void)
    {
        this->scrollContentFrameUpdate();
        this->scrollerActionRun();
    }

public:
    void scrollBounceChanged(void)
    {
        this->impl->scrollViewScrollBounceSet(this->pimplOwner,
            this->pimplOwner->scrollBounceHorizontal(),
            this->pimplOwner->scrollBounceVertical(),
            this->pimplOwner->scrollBounceHorizontalAlways(),
            this->pimplOwner->scrollBounceVerticalAlways());
        this->xScroll->scrollBounceChanged(this->pimplOwner->scrollBounceHorizontal(), this->pimplOwner->scrollBounceHorizontalAlways());
        this->yScroll->scrollBounceChanged(this->pimplOwner->scrollBounceVertical(), this->pimplOwner->scrollBounceVerticalAlways());
    }

public:
    void scrollAlignToPageChanged(void)
    {
        this->xScroll->scrollAlignToPageChanged(this->pimplOwner->scrollAlignToPageHorizontal());
        this->yScroll->scrollAlignToPageChanged(this->pimplOwner->scrollAlignToPageVertical());
    }

public:
    void processDragBegin(ZF_IN const ZFUIPoint &mousePos,
                          ZF_IN const zftimet &mouseTime)
    {
        // reset state before drag begin
        this->xScrollAniTimerStarted = zffalse;
        this->yScrollAniTimerStarted = zffalse;
        this->notifyScrollAniTimerStop();
        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionDragEnd);

        if(!this->pimplOwner->scrollEnable())
        {
            return ;
        }

        this->xScrollEnable = (this->pimplOwner->scrollBounceHorizontalAlways()
            || this->pimplOwner->scrollContentFrame().size.width > this->pimplOwner->scrollAreaWidth());
        this->yScrollEnable = (this->pimplOwner->scrollBounceVerticalAlways()
            || this->pimplOwner->scrollContentFrame().size.height > this->pimplOwner->scrollAreaHeight());

        if(this->xScroll->scrollRequireFocus())
        {
            this->yScrollEnable = zffalse;
        }
        else if(this->yScroll->scrollRequireFocus())
        {
            this->xScrollEnable = zffalse;
        }

        if(this->xScrollEnable)
        {
            this->xScrollDragPrevPos = mousePos.x;
            this->xScrollDragCurPos = mousePos.x;
            this->xScroll->scrollOnDragBegin(mousePos.x, mouseTime);
        }
        if(this->yScrollEnable)
        {
            this->yScrollDragPrevPos = mousePos.y;
            this->yScrollDragCurPos = mousePos.y;
            this->yScroll->scrollOnDragBegin(mousePos.y, mouseTime);
        }

        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionDragBegin);
        this->scrollerActionRun();
    }
    void processDrag(ZF_IN const ZFUIPoint &mousePos,
                     ZF_IN const zftimet &mouseTime)
    {
        if(this->state != ZFUIScrollViewState::e_Dragging)
        {
            return ;
        }

        zfbool alignToAxis = (this->pimplOwner->scrollAlignToAxis() || this->pimplOwner->scrollAlignToPageHorizontal() || this->pimplOwner->scrollAlignToPageVertical());
        if(alignToAxis && this->xScrollEnable && this->yScrollEnable)
        {
            this->xScrollDragPrevPos = this->xScrollDragCurPos;
            this->xScrollDragCurPos = mousePos.x;
            this->yScrollDragPrevPos = this->yScrollDragCurPos;
            this->yScrollDragCurPos = mousePos.y;

            zfint xOffset = zfmAbs(this->xScrollDragCurPos - this->xScrollDragPrevPos);
            zfint yOffset = zfmAbs(this->yScrollDragCurPos - this->yScrollDragPrevPos);
            if(xOffset > yOffset)
            {
                this->yScrollEnable = zffalse;
            }
            else if(yOffset >= xOffset)
            {
                this->xScrollEnable = zffalse;
            }
        }

        if(this->xScrollEnable)
        {
            this->xScroll->scrollOnDrag(mousePos.x, mouseTime);
        }
        if(this->yScrollEnable)
        {
            this->yScroll->scrollOnDrag(mousePos.y, mouseTime);
        }

        this->scrollContentFrameUpdate();
        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionDrag);
        this->scrollerActionRun();
    }
    void processDragEnd(ZF_IN const zftimet &mouseTime,
                        ZF_IN zfbool needScrollAni)
    {
        if(this->state != ZFUIScrollViewState::e_Dragging)
        {
            // this may be an error state, reset by end scroll
            this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionScrollEnd);
            this->scrollerActionRun();
            return ;
        }

        if(this->xScrollEnable)
        {
            this->xScroll->scrollOnDragEnd(mouseTime, needScrollAni);
        }
        if(this->yScrollEnable)
        {
            this->yScroll->scrollOnDragEnd(mouseTime, needScrollAni);
        }

        this->xScrollEnable = (this->pimplOwner->scrollBounceHorizontalAlways()
            || this->pimplOwner->scrollContentFrame().size.width > this->pimplOwner->scrollAreaWidth());
        this->yScrollEnable = (this->pimplOwner->scrollBounceVerticalAlways()
            || this->pimplOwner->scrollContentFrame().size.height > this->pimplOwner->scrollAreaHeight());

        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionDragEnd);
        this->scrollerActionRun();
    }
    void processScrollAnimation(ZF_IN const zftimet &relativeTimeInMiliseconds)
    {
        this->scrollAniLastTime = relativeTimeInMiliseconds;

        if(this->xScrollAniTimerStarted)
        {
            this->xScroll->scrollAniOnUpdate(relativeTimeInMiliseconds);
        }
        if(this->yScrollAniTimerStarted)
        {
            this->yScroll->scrollAniOnUpdate(relativeTimeInMiliseconds);
        }

        if(this->autoScrollSpeedX != 0)
        {
            if((this->autoScrollSpeedX > 0 && this->xScroll->scrollContentOffset() >= 0)
                || (this->autoScrollSpeedX < 0 && this->xScroll->scrollContentOffset() + this->xScroll->scrollContentSize() <= this->pimplOwner->scrollAreaWidth()))
            {
                this->pimplOwner->scrollOverrideSet(zftrue);
                this->pimplOwner->autoScrollStopX();
                this->pimplOwner->scrollOverrideSet(zffalse);
            }
            else
            {
                this->xScroll->scrollBySpeed(this->autoScrollSpeedX);
            }
        }
        if(this->autoScrollSpeedY != 0)
        {
            if((this->autoScrollSpeedY > 0 && this->yScroll->scrollContentOffset() >= 0)
                || (this->autoScrollSpeedY < 0 && this->yScroll->scrollContentOffset() + this->yScroll->scrollContentSize() <= this->pimplOwner->scrollAreaHeight()))
            {
                this->pimplOwner->scrollOverrideSet(zftrue);
                this->pimplOwner->autoScrollStopY();
                this->pimplOwner->scrollOverrideSet(zffalse);
            }
            else
            {
                this->yScroll->scrollBySpeed(this->autoScrollSpeedY);
            }
        }

        this->scrollContentFrameUpdate();
        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionScroll);
        this->scrollerActionRun();
    }

public:
    void scrollContentFrameUpdateForImpl(void)
    {
        ZFUIRect frame = this->pimplOwner->scrollContentFrame();
        frame.point.x += this->pimplOwner->scrollAreaMargin().left;
        frame.point.y += this->pimplOwner->scrollAreaMargin().top;
        this->impl->scrollViewScrollContentFrameSet(this->pimplOwner, ZFUIRectApplyScale(frame, this->pimplOwner->scaleGetFixed()));
    }
private:
    void scrollContentFrameUpdate(void)
    {
        ZFUIRect frame = ZFUIRectMake(
            this->xScroll->scrollContentOffset(),
            this->yScroll->scrollContentOffset(),
            this->xScroll->scrollContentSize(),
            this->yScroll->scrollContentSize());
        this->scrollContentFrameOverrideFlag = zftrue;
        this->pimplOwner->scrollContentFrameSet(frame);
        this->scrollContentFrameOverrideFlag = zffalse;
        this->scrollContentFrameUpdateForImpl();

        this->notifyScrollContentFrameOnChange();
        this->scrollThumbNeedUpdate = zftrue;
    }

private:
    ZFMETHOD_DECLARE_0(zftimet, x_scrollAniStart)
    {
        if(!this->xScrollAniTimerStarted)
        {
            this->xScrollAniTimerStarted = zftrue;
            this->notifyScrollAniTimerStart();
        }
        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionScrollBegin);
        return this->scrollAniLastTime;
    }
    ZFMETHOD_DECLARE_0(zftimet, y_scrollAniStart)
    {
        if(!this->yScrollAniTimerStarted)
        {
            this->yScrollAniTimerStarted = zftrue;
            this->notifyScrollAniTimerStart();
        }
        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionScrollBegin);
        return this->scrollAniLastTime;
    }
    ZFMETHOD_DECLARE_0(void, x_scrollAniStop)
    {
        if(this->xScrollAniTimerStarted)
        {
            this->xScrollAniTimerStarted = zffalse;
            if(!this->yScrollAniTimerStarted)
            {
                this->notifyScrollAniTimerStop();
                this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionScrollEnd);
            }
        }
    }
    ZFMETHOD_DECLARE_0(void, y_scrollAniStop)
    {
        if(this->yScrollAniTimerStarted)
        {
            this->yScrollAniTimerStarted = zffalse;
            if(!this->xScrollAniTimerStarted)
            {
                this->notifyScrollAniTimerStop();
                this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionScrollEnd);
            }
        }
    }

private:
    void notifyScrollAniTimerStart(void)
    {
        if(!this->scrollAniTimerStarted)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollAniTimerStart");
            #endif
            this->scrollAniTimerStarted = zftrue;
            this->scrollAniLastTime = this->impl->scrollViewScrollAnimationStart(this->pimplOwner);
        }
    }
    void notifyScrollAniTimerStop(void)
    {
        if(this->scrollAniTimerStarted)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollAniTimerStop");
            #endif
            this->scrollAniTimerStarted = zffalse;
            this->impl->scrollViewScrollAnimationStop(this->pimplOwner);
        }
    }
    void notifyScrollOnDragBegin(void)
    {
        if(this->state != ZFUIScrollViewState::e_Dragging)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollOnDragBegin");
            #endif
            this->notifyScrollAniTimerStop();
            this->notifyScrollOnScrollEnd();

            this->state = ZFUIScrollViewState::e_Dragging;
            this->pimplOwner->scrollOnDragBegin();
            this->scrollThumbUpdate();
        }
    }
    void notifyScrollOnDrag(void)
    {
        if(this->state == ZFUIScrollViewState::e_Dragging)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollOnDrag");
            #endif
            this->pimplOwner->scrollOnDrag();
            this->scrollThumbUpdate();
        }
    }
    void notifyScrollOnDragEnd(void)
    {
        if(this->state == ZFUIScrollViewState::e_Dragging)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollOnDragEnd");
            #endif
            this->state = ZFUIScrollViewState::e_Idle;
            this->pimplOwner->scrollOnDragEnd();
            this->scrollThumbNeedUpdate = zftrue;
            this->scrollThumbUpdate();
        }
    }
    void notifyScrollOnScrollBegin(void)
    {
        if(this->state != ZFUIScrollViewState::e_Scrolling)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollOnScrollBegin");
            #endif
            this->notifyScrollOnDragEnd();

            this->state = ZFUIScrollViewState::e_Scrolling;
            this->pimplOwner->scrollOnScrollBegin();
            this->scrollThumbUpdate();
        }
    }
    void notifyScrollOnScroll(void)
    {
        if(this->state == ZFUIScrollViewState::e_Scrolling)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollOnScroll");
            #endif
            this->pimplOwner->scrollOnScroll();
            this->scrollThumbUpdate();
        }
    }
    void notifyScrollOnScrollEnd(void)
    {
        if(this->state == ZFUIScrollViewState::e_Scrolling)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollOnScrollEnd");
            #endif
            this->notifyScrollOnScroll();

            this->state = ZFUIScrollViewState::e_Idle;
            this->notifyScrollAniTimerStop();
            this->pimplOwner->scrollOnScrollEnd();
            this->scrollThumbNeedUpdate = zftrue;
            this->scrollThumbUpdate();
        }
    }
    void notifyScrollContentFrameOnChange(void)
    {
        this->pimplOwner->scrollContentFrameOnChange();
    }
    void notifyScrollAutoScrollOnStart(void)
    {
        if(!this->autoScrollStartFlag)
        {
            this->autoScrollStartFlag = zftrue;

            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollAutoScrollOnStart");
            #endif
            this->pimplOwner->scrollAutoScrollOnStart();
        }
    }
    void notifyScrollAutoScrollOnStop(void)
    {
        if(this->autoScrollStartFlag && this->autoScrollSpeedX == 0 && this->autoScrollSpeedY == 0)
        {
            this->autoScrollStartFlag = zffalse;

            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollAutoScrollOnStop");
            #endif
            this->pimplOwner->scrollAutoScrollOnStop();
        }
    }

private:
    void scrollerActionRun(void)
    {
        if(this->scrollerActionRunning || this->scrollerActions.isEmpty())
        {
            return ;
        }

        this->scrollerActionRunning = zftrue;
        while(!this->scrollerActions.isEmpty())
        {
            _ZFP_ZFUIScrollViewAction scrollerAction = this->scrollerActions.queueTake();
            switch(scrollerAction)
            {
                case _ZFP_ZFUIScrollViewActionDragBegin:
                    this->notifyScrollOnDragBegin();
                    break;
                case _ZFP_ZFUIScrollViewActionDrag:
                    this->notifyScrollOnDrag();
                    break;
                case _ZFP_ZFUIScrollViewActionDragEnd:
                    this->notifyScrollOnDragEnd();
                    break;
                case _ZFP_ZFUIScrollViewActionScrollBegin:
                    this->notifyScrollOnScrollBegin();
                    break;
                case _ZFP_ZFUIScrollViewActionScroll:
                    this->notifyScrollOnScroll();
                    break;
                case _ZFP_ZFUIScrollViewActionScrollEnd:
                    this->notifyScrollOnScrollEnd();
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
        }
        this->scrollerActionRunning = zffalse;
    }
    void scrollerActionAdd(ZF_IN _ZFP_ZFUIScrollViewAction scrollerAction)
    {
        this->scrollerActions.queuePut(scrollerAction);
    }

    // ============================================================
    // scroll thumb logic
public:
    void scrollThumbUpdate(void)
    {
        if(this->scrollThumbNeedUpdate)
        {
            if(this->xScrollThumb != zfnull)
            {
                this->xScrollThumb->scrollThumbUpdate();
                this->xScrollThumb->toObject()->observerNotify(ZFUIScrollThumb::EventScrollThumbOnUpdate());
            }
            if(this->yScrollThumb != zfnull)
            {
                this->yScrollThumb->scrollThumbUpdate();
                this->yScrollThumb->toObject()->observerNotify(ZFUIScrollThumb::EventScrollThumbOnUpdate());
            }
            this->scrollThumbNeedUpdate = zffalse;
        }
    }
};

// ============================================================
// ZFUIScrollView
ZFOBJECT_REGISTER(ZFUIScrollView)

ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnDragBegin)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnDrag)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnDragEnd)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnScrollBegin)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnScroll)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnScrollEnd)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollAreaMarginOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollContentFrameOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollAutoScrollOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollAutoScrollOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnScrolledByUser)

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIScrollView, zfbool, scrollEnable)
{
    this->scrollEnableSetInternal(newValue);
    d->xScrollEnable = this->scrollEnable();
    d->yScrollEnable = this->scrollEnable();
    d->impl->scrollViewScrollEnableSet(this, this->scrollEnable());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIScrollView, zfbool, scrollBounceHorizontal)
{
    this->scrollBounceHorizontalSetInternal(newValue);
    d->scrollBounceChanged();
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIScrollView, zfbool, scrollBounceVertical)
{
    this->scrollBounceVerticalSetInternal(newValue);
    d->scrollBounceChanged();
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIScrollView, zfbool, scrollBounceHorizontalAlways)
{
    this->scrollBounceHorizontalAlwaysSetInternal(newValue);
    d->scrollBounceChanged();
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIScrollView, zfbool, scrollBounceVerticalAlways)
{
    this->scrollBounceVerticalAlwaysSetInternal(newValue);
    d->scrollBounceChanged();
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIScrollView, zfbool, scrollAlignToAxis)
{
    this->scrollAlignToAxisSetInternal(newValue);
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIScrollView, zfbool, scrollAlignToPageHorizontal)
{
    this->scrollAlignToPageHorizontalSetInternal(newValue);
    d->scrollAlignToPageChanged();
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIScrollView, zfbool, scrollAlignToPageVertical)
{
    this->scrollAlignToPageVerticalSetInternal(newValue);
    d->scrollAlignToPageChanged();
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIScrollView, ZFUIRect, scrollContentFrame)
{
    this->scrollContentFrameSetInternal(newValue);
    if(d->xScroll->scrollContentOffset() != newValue.point.x
        || d->xScroll->scrollContentSize() != newValue.size.width)
    {
        d->xScroll->scrollContentChanged(newValue.point.x, newValue.size.width);
    }
    if(d->yScroll->scrollContentOffset() != newValue.point.x
        || d->yScroll->scrollContentSize() != newValue.size.width)
    {
        d->yScroll->scrollContentChanged(newValue.point.y, newValue.size.height);
    }
    if(!d->scrollContentFrameOverrideFlag)
    {
        d->xScroll->scrollWithoutAnimation(newValue.point.x);
        d->yScroll->scrollWithoutAnimation(newValue.point.y);
        d->scrollerUpdate();

        if(d->scrollOverrideFlag == 0)
        {
            this->scrollOnScrolledByUser();
        }
    }
}
void ZFUIScrollView::_ZFP_ZFUIScrollView_scrollContentFrameSetByImpl(ZF_IN const ZFUIRect &rect)
{
    d->scrollContentFrameOverrideFlag = zftrue;
    this->scrollContentFrameSet(rect);
    d->scrollContentFrameOverrideFlag = zffalse;
}

ZFObject *ZFUIScrollView::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfAllocWithoutLeakTest(_ZFP_ZFUIScrollViewPrivate);
    d->pimplOwner = this;
    d->xScroll = zfRetain(ZFCastZFObject(ZFUIScroller *, this->scrollerClass()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE).toObject()));
    d->yScroll = zfRetain(ZFCastZFObject(ZFUIScroller *, this->scrollerClass()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE).toObject()));
    zfCoreAssertWithMessage(d->xScroll != zfnull && d->yScroll != zfnull,
        zfTextA("scrollerClass must return a class type of %s"),
        zfsCoreZ2A(ZFUIScroller::ClassData()->className()));
    zfCoreAssert(d->xScroll != zfnull && d->yScroll != zfnull);
    d->scrollerInit();

    this->scrollThumbHorizontalOnInit();
    this->scrollThumbVerticalOnInit();

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD _ZFP_ZFUIScrollView_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUIScrollView)->nativeScrollViewDestroy(view->to<ZFUIScrollView *>(), nativeImplView);
        }
    };
    this->nativeImplViewSet(
        ZFPROTOCOL_ACCESS(ZFUIScrollView)->nativeScrollViewCreate(this),
        _ZFP_ZFUIScrollView_nativeImplViewDestroy::action);

    return this;
}
void ZFUIScrollView::objectOnDealloc(void)
{
    ZFPropertyChange(d->xScroll, zfnull);
    ZFPropertyChange(d->yScroll, zfnull);

    if(d->xScrollThumb != zfnull)
    {
        d->xScrollThumb->scrollThumbDealloc();
        zfRelease(d->xScrollThumb);
        d->xScrollThumb = zfnull;
    }
    if(d->yScrollThumb != zfnull)
    {
        d->yScrollThumb->scrollThumbDealloc();
        zfRelease(d->yScrollThumb);
        d->yScrollThumb = zfnull;
    }

    zfReleaseWithoutLeakTest(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIScrollView::objectOnDeallocPrepare(void)
{
    // set content frame to stop scroll animation
    this->scrollContentFrameSet(this->scrollContentFrame());
    zfsuper::objectOnDeallocPrepare();
}

void ZFUIScrollView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ret += zfText(" ");
    ZFUIRectToString(ret, this->scrollContentFrame());
    if(!this->scrollEnable())
    {
        ret += zfText(" ScrollDisabled");
    }
}

// ============================================================
// override ZFUIView
void ZFUIScrollView::implChildOnAdd(ZF_IN ZFUIView *child,
                                    ZF_IN zfindex virtualIndex,
                                    ZF_IN ZFUIViewChildLayerEnum childLayer,
                                    ZF_IN zfindex childLayerIndex)
{
    switch(child->viewLayer())
    {
        case ZFUIViewChildLayer::e_Normal:
            d->impl->scrollChildAdd(this, child, childLayerIndex);
            break;
        case ZFUIViewChildLayer::e_Impl:
            zfsuper::implChildOnAdd(child, virtualIndex, childLayer, childLayerIndex);
            break;
        case ZFUIViewChildLayer::e_Background:
            zfsuper::implChildOnAdd(child, virtualIndex, childLayer, childLayerIndex);
            break;
        case ZFUIViewChildLayer::e_Foreground:
            zfsuper::implChildOnAdd(child, virtualIndex - this->childCount(), childLayer, childLayerIndex);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}
void ZFUIScrollView::implChildOnRemove(ZF_IN ZFUIView *child,
                                       ZF_IN zfindex virtualIndex,
                                       ZF_IN ZFUIViewChildLayerEnum childLayer,
                                       ZF_IN zfindex childLayerIndex)
{
    switch(childLayer)
    {
        case ZFUIViewChildLayer::e_Normal:
            d->impl->scrollChildRemove(this, child, childLayerIndex);
            break;
        case ZFUIViewChildLayer::e_Impl:
            zfsuper::implChildOnRemove(child, virtualIndex, childLayer, childLayerIndex);
            break;
        case ZFUIViewChildLayer::e_Background:
            zfsuper::implChildOnRemove(child, virtualIndex, childLayer, childLayerIndex);
            break;
        case ZFUIViewChildLayer::e_Foreground:
            zfsuper::implChildOnRemove(child, virtualIndex - this->childCount(), childLayer, childLayerIndex);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

void ZFUIScrollView::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    if(!ZFUISizeIsEqual(bounds.size, this->layoutedFramePrev().size))
    {
        d->xScroll->scrollOwnerSizeChanged(bounds.size.width - ZFUIMarginGetX(d->scrollAreaMargin));
        d->yScroll->scrollOwnerSizeChanged(bounds.size.height - ZFUIMarginGetY(d->scrollAreaMargin));
        d->scrollerUpdate();
    }

    d->scrollThumbUpdate();
    zfsuper::layoutOnLayoutPrepare(bounds);
}
void ZFUIScrollView::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayout(
        ZFUIRectApplyMargin(
            ZFUIRectGetBounds(this->scrollContentFrame()),
            d->scrollAreaMargin));
}
void ZFUIScrollView::layoutOnLayoutFinish(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayoutFinish(bounds);
    d->scrollContentFrameUpdateForImpl();
}
void ZFUIScrollView::layoutedFrameFixedOnUpdateForChild(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &childFrame)
{
    ret = childFrame;
    ret.point.x += this->scrollContentFrame().point.x;
    ret.point.y += this->scrollContentFrame().point.y;
}

void ZFUIScrollView::viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent)
{
    zfsuper::viewEventOnMouseEvent(mouseEvent);
}
void ZFUIScrollView::viewEventOnWheelEvent(ZF_IN ZFUIWheelEvent *wheelEvent)
{
    zfsuper::viewEventOnWheelEvent(wheelEvent);
    if(wheelEvent->eventResolved()) {
        return ;
    }
    if(!this->scrollEnable())
    {
        return ;
    }

    zfint wheelXSaved = this->scrollByPointEndPoint().x - this->scrollContentFrame().point.x;
    zfint wheelYSaved = this->scrollByPointEndPoint().y - this->scrollContentFrame().point.y;
    const zfint initValue = 60;
    const zfint maxValue = 3000;
    zfint wheelX = wheelEvent->wheelX * initValue;
    zfint wheelY = wheelEvent->wheelY * initValue;
    if(this->scrollAlignToAxis())
    {
        if(zfmAbs(wheelY) >= zfmAbs(wheelX))
        {
            wheelX = 0;
            wheelXSaved = 0;
        }
        else
        {
            wheelY = 0;
            wheelYSaved = 0;
        }
    }

    if(!((wheelXSaved > 0 && wheelX < 0)
        || (wheelXSaved < 0 && wheelX > 0)))
    {
        wheelX = wheelXSaved + wheelX;
        if(wheelX > 0 && wheelX > maxValue)
        {
            wheelX = maxValue;
        }
        else if(wheelX < 0 && wheelX < -maxValue)
        {
            wheelX = -maxValue;
        }
    }

    if(!((wheelYSaved > 0 && wheelY < 0)
        || (wheelYSaved < 0 && wheelY > 0)))
    {
        wheelY = wheelYSaved + wheelY;
        if(wheelY > 0 && wheelY > maxValue)
        {
            wheelY = maxValue;
        }
        else if(wheelY < 0 && wheelY < -maxValue)
        {
            wheelY = -maxValue;
        }
    }

    if(wheelX != 0)
    {
        if(this->scrollAlignToPageHorizontal())
        {
            zfint pageSize = this->scrollAreaWidth();
            if(zfmAbs(wheelX) < zfmAbs(pageSize))
            {
                if(wheelX > 0)
                {
                    wheelX += pageSize / 2;
                }
                else
                {
                    wheelX -= pageSize / 2;
                }
            }
        }
        if((this->scrollContentOffsetLeft() <= 0 && wheelX > 0)
                || (this->scrollContentOffsetRight() <= 0 && wheelX < 0))
        {
            wheelX = 0;
        }
    }

    if(wheelY != 0)
    {
        if(this->scrollAlignToPageVertical())
        {
            zfint pageSize = this->scrollAreaHeight();
            if(zfmAbs(wheelY) < zfmAbs(pageSize))
            {
                if(wheelY > 0)
                {
                    wheelY += pageSize / 2;
                }
                else
                {
                    wheelY -= pageSize / 2;
                }
            }
        }
        if((this->scrollContentOffsetTop() <= 0 && wheelY > 0)
            || (this->scrollContentOffsetBottom() <= 0 && wheelY < 0))
        {
            wheelY = 0;
        }
    }

    if(wheelX != 0 || wheelY != 0)
    {
        this->scrollByPoint(this->scrollContentFrame().point.x + wheelX, this->scrollContentFrame().point.y + wheelY);
        wheelEvent->eventResolvedSet(zftrue);
    }
}

static void _ZFP_ZFUIScrollView_scrollChildToVisible(ZF_OUT zfint &offset,
                                                     ZF_IN zfint childStart,
                                                     ZF_IN zfint childLength,
                                                     ZF_IN zfint selfStart,
                                                     ZF_IN zfint selfLength)
{
    zfint childTail = childStart + childLength;
    zfint selfTail = selfStart + selfLength;
    if(childLength > selfLength)
    {
        if(childStart < selfStart)
        {
            if(childTail > selfTail)
            {
                if(zfmAbs(childStart - selfStart) <= zfmAbs(childTail - selfTail))
                {
                    offset = selfStart - childStart;
                }
                else
                {
                    offset = selfTail - childTail;
                }
            }
            else
            {
                offset = selfTail - childTail;
            }
        }
        else
        {
            offset = selfStart - childStart;
        }
    }
    else
    {
        if(childStart < selfStart)
        {
            offset = selfStart - childStart;
        }
        else if(childTail > selfTail)
        {
            offset = selfTail - childTail;
        }
    }
}
void ZFUIScrollView::scrollToFitRange(void)
{
    d->xScroll->scrollToFitRange();
    d->yScroll->scrollToFitRange();
    d->scrollerUpdate();
}
void ZFUIScrollView::scrollChildToVisible(ZF_IN ZFUIView *child)
{
    if(child == zfnull)
    {
        return ;
    }

    zfint offsetX = 0;
    zfint offsetY = 0;

    ZFUIRect selfRect = ZFUIViewPositionOnScreen(this);
    ZFUIRect childRect = ZFUIViewPositionOnScreen(child);
    _ZFP_ZFUIScrollView_scrollChildToVisible(offsetX, childRect.point.x, childRect.size.width, selfRect.point.x, selfRect.size.width);
    _ZFP_ZFUIScrollView_scrollChildToVisible(offsetY, childRect.point.y, childRect.size.height, selfRect.point.y, selfRect.size.height);

    if(offsetX != 0 || offsetY != 0)
    {
        this->scrollByPoint(this->scrollContentFrame().point.x + offsetX, this->scrollContentFrame().point.y + offsetY);
    }
}

void ZFUIScrollView::scrollAreaMarginAdd(ZF_IN const ZFUIMargin &margin)
{
    if(margin != ZFUIMarginZero)
    {
        ZFUIMarginInc(d->scrollAreaMargin, d->scrollAreaMargin, margin);
        this->layoutRequest();
        this->scrollAreaMarginOnChange();
    }
}
void ZFUIScrollView::scrollAreaMarginRemove(ZF_IN const ZFUIMargin &margin)
{
    if(margin != ZFUIMarginZero)
    {
        ZFUIMarginDec(d->scrollAreaMargin, d->scrollAreaMargin, margin);
        this->layoutRequest();
        this->scrollAreaMarginOnChange();
    }
}
const ZFUIMargin &ZFUIScrollView::scrollAreaMargin(void)
{
    return d->scrollAreaMargin;
}

ZFUIRect ZFUIScrollView::scrollArea(void)
{
    return ZFUIRectApplyMargin(ZFUIRectGetBounds(this->layoutedFrame()), d->scrollAreaMargin);
}
zfint ZFUIScrollView::scrollAreaLeft(void)
{
    return d->scrollAreaMargin.left;
}
zfint ZFUIScrollView::scrollAreaTop(void)
{
    return d->scrollAreaMargin.top;
}
zfint ZFUIScrollView::scrollAreaRight(void)
{
    return this->layoutedFrame().size.width - d->scrollAreaMargin.right;
}
zfint ZFUIScrollView::scrollAreaBottom(void)
{
    return this->layoutedFrame().size.height - d->scrollAreaMargin.bottom;
}
zfint ZFUIScrollView::scrollAreaWidth(void)
{
    return this->layoutedFrame().size.width - ZFUIMarginGetX(d->scrollAreaMargin);
}
zfint ZFUIScrollView::scrollAreaHeight(void)
{
    return this->layoutedFrame().size.height - ZFUIMarginGetY(d->scrollAreaMargin);
}

void ZFUIScrollView::scrollThumbHorizontalClassSet(ZF_IN const ZFClass *cls)
{
    if(cls == zfnull || !cls->classIsTypeOf(ZFUIScrollThumb::ClassData()))
    {
        cls = zfnull;
    }

    d->xScrollThumbClass = cls;
    if(d->xScrollThumb != zfnull)
    {
        d->xScrollThumb->scrollThumbDealloc();
        zfRelease(d->xScrollThumb);
        d->xScrollThumb = zfnull;
    }
    if(cls != zfnull)
    {
        d->xScrollThumb = zfRetain(ZFCastZFObject(ZFUIScrollThumb *, cls->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE).toObject()));
        if(d->xScrollThumb != zfnull)
        {
            d->xScrollThumb->_scrollView = this;
            d->xScrollThumb->_horizontal = zftrue;
            d->xScrollThumb->scrollThumbInit();
        }
    }
}
const ZFClass *ZFUIScrollView::scrollThumbHorizontalClass(void)
{
    return d->xScrollThumbClass;
}
void ZFUIScrollView::scrollThumbVerticalClassSet(ZF_IN const ZFClass *cls)
{
    if(cls == zfnull || !cls->classIsTypeOf(ZFUIScrollThumb::ClassData()))
    {
        cls = zfnull;
    }

    d->yScrollThumbClass = cls;
    if(d->yScrollThumb != zfnull)
    {
        d->yScrollThumb->scrollThumbDealloc();
        zfRelease(d->yScrollThumb);
        d->yScrollThumb = zfnull;
    }
    if(cls != zfnull)
    {
        d->yScrollThumb = zfRetain(ZFCastZFObject(ZFUIScrollThumb *, cls->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE).toObject()));
        if(d->yScrollThumb != zfnull)
        {
            d->yScrollThumb->_scrollView = this;
            d->yScrollThumb->_horizontal = zffalse;
            d->yScrollThumb->scrollThumbInit();
        }
    }
}
const ZFClass *ZFUIScrollView::scrollThumbVerticalClass(void)
{
    return d->yScrollThumbClass;
}

void ZFUIScrollView::scrollThumbHorizontalOnInit(void)
{
    this->scrollThumbHorizontalClassSet(ZFUIScrollThumbHorizontalClass());
}
void ZFUIScrollView::scrollThumbVerticalOnInit(void)
{
    this->scrollThumbVerticalClassSet(ZFUIScrollThumbVerticalClass());
}

// ============================================================
// scroll control
const ZFClass *ZFUIScrollView::scrollerClass(void)
{
    return ZFUIScrollerClass();
}

void ZFUIScrollView::scrollContentFrameSetAnimated(ZF_IN const ZFUIRect &scrollContentFrame)
{
    this->scrollContentFrameSetWhileAnimating(ZFUIRectMake(
        this->scrollContentFrame().point.x,
        this->scrollContentFrame().point.y,
        scrollContentFrame.size.width,
        scrollContentFrame.size.height));
    if(d->state != ZFUIScrollViewState::e_Dragging)
    {
        this->scrollByPoint(scrollContentFrame.point.x, scrollContentFrame.point.y);
    }
    else
    {
        if(d->scrollOverrideFlag == 0)
        {
            this->scrollOnScrolledByUser();
        }
    }
}
void ZFUIScrollView::scrollContentFrameSetWhileAnimating(ZF_IN const ZFUIRect &scrollContentFrame)
{
    d->xScroll->scrollContentChanged(scrollContentFrame.point.x, scrollContentFrame.size.width);
    d->yScroll->scrollContentChanged(scrollContentFrame.point.y, scrollContentFrame.size.height);
    d->scrollerUpdate();
}
void ZFUIScrollView::scrollByPoint(ZF_IN zfint xPos,
                                   ZF_IN zfint yPos)
{
    if(d->state != ZFUIScrollViewState::e_Dragging)
    {
        d->xScroll->scrollByPoint(xPos);
        d->yScroll->scrollByPoint(yPos);
        d->scrollerUpdate();
    }
    this->scrollOverrideSet(zftrue);
    this->autoScrollStopX();
    this->autoScrollStopY();
    this->scrollOverrideSet(zffalse);

    if(d->scrollOverrideFlag == 0)
    {
        this->scrollOnScrolledByUser();
    }
}
ZFUIPoint ZFUIScrollView::scrollByPointEndPoint(void)
{
    return ZFUIPointMake(d->xScroll->scrollByPointEndPoint(), d->yScroll->scrollByPointEndPoint());
}
void ZFUIScrollView::scrollBySpeed(ZF_IN zfint xSpeedInPixelsPerSecond,
                                   ZF_IN zfint ySpeedInPixelsPerSecond)
{
    if(d->state != ZFUIScrollViewState::e_Dragging)
    {
        d->xScroll->scrollBySpeed(xSpeedInPixelsPerSecond);
        d->yScroll->scrollBySpeed(ySpeedInPixelsPerSecond);
        d->scrollerUpdate();
    }

    if(d->scrollOverrideFlag == 0)
    {
        this->scrollOnScrolledByUser();
    }
}
zfint ZFUIScrollView::scrollBySpeedCurrentSpeedX(void)
{
    return d->xScroll->scrollBySpeedCurrentSpeed();
}
zfint ZFUIScrollView::scrollBySpeedCurrentSpeedY(void)
{
    return d->yScroll->scrollBySpeedCurrentSpeed();
}
ZFUIPoint ZFUIScrollView::scrollBySpeedEndPointPredicted(void)
{
    return ZFUIPointMake(d->xScroll->scrollBySpeedEndPointPredicted(), d->yScroll->scrollBySpeedEndPointPredicted());
}

ZFUIPoint ZFUIScrollView::scrollEndPointPredicted(void)
{
    return ZFUIPointMake(d->xScroll->scrollEndPointPredicted(), d->yScroll->scrollEndPointPredicted());
}

void ZFUIScrollView::autoScrollStartX(ZF_IN zfint speedInPixelsPerSecond)
{
    if(d->state != ZFUIScrollViewState::e_Dragging)
    {
        d->autoScrollSpeedX = speedInPixelsPerSecond;
        if(d->xScrollEnable)
        {
            d->xScroll->scrollBySpeed(speedInPixelsPerSecond);
        }
    }

    if(d->scrollOverrideFlag == 0)
    {
        this->scrollOnScrolledByUser();
    }
}
void ZFUIScrollView::autoScrollStartY(ZF_IN zfint speedInPixelsPerSecond)
{
    if(d->state != ZFUIScrollViewState::e_Dragging)
    {
        d->autoScrollSpeedY = speedInPixelsPerSecond;
        if(d->yScrollEnable)
        {
            d->yScroll->scrollBySpeed(speedInPixelsPerSecond);
        }
    }

    if(d->scrollOverrideFlag == 0)
    {
        this->scrollOnScrolledByUser();
    }
}
void ZFUIScrollView::autoScrollStopX(void)
{
    if(d->autoScrollSpeedX != 0)
    {
        d->autoScrollSpeedX = 0;
        d->xScroll->scrollStop();
        d->scrollerUpdate();

        if(d->scrollOverrideFlag == 0)
        {
            this->scrollOnScrolledByUser();
        }
    }
}
void ZFUIScrollView::autoScrollStopY(void)
{
    if(d->autoScrollSpeedY != 0)
    {
        d->autoScrollSpeedY = 0;
        d->yScroll->scrollStop();
        d->scrollerUpdate();

        if(d->scrollOverrideFlag == 0)
        {
            this->scrollOnScrolledByUser();
        }
    }
}
zfint ZFUIScrollView::autoScrollSpeedX(void)
{
    return d->autoScrollSpeedX;
}
zfint ZFUIScrollView::autoScrollSpeedY(void)
{
    return d->autoScrollSpeedY;
}

void ZFUIScrollView::scrollSimulateDragBegin(ZF_IN const ZFUIPoint &mousePos,
                                             ZF_IN const zftimet &mouseTime)
{
    this->_ZFP_ZFUIScrollView_notifyDragBegin(mousePos, mouseTime);
}
void ZFUIScrollView::scrollSimulateDrag(ZF_IN const ZFUIPoint &mousePos,
                                        ZF_IN const zftimet &mouseTime)
{
    this->_ZFP_ZFUIScrollView_notifyDrag(mousePos, mouseTime);
}
void ZFUIScrollView::scrollSimulateDragEnd(ZF_IN const zftimet &mouseTime,
                                           ZF_IN_OPT zfbool needScrollAni /* = zftrue */)
{
    this->_ZFP_ZFUIScrollView_notifyDragEnd(mouseTime, needScrollAni);
}

void ZFUIScrollView::scrollOverrideSet(ZF_IN zfbool scrollOverride)
{
    if(scrollOverride)
    {
        ++(d->scrollOverrideFlag);
    }
    else
    {
        --(d->scrollOverrideFlag);
        zfCoreAssert(d->scrollOverrideFlag >= 0);
    }
}
zfbool ZFUIScrollView::scrollOverride(void)
{
    return (d->scrollOverrideFlag > 0);
}

// ============================================================
ZFUIScrollViewStateEnum ZFUIScrollView::scrollViewState(void)
{
    return d->state;
}

// ============================================================
// scroll callbacks
void ZFUIScrollView::_ZFP_ZFUIScrollView_notifyDragBegin(ZF_IN const ZFUIPoint &mousePos,
                                                         ZF_IN const zftimet &mouseTime)
{
    this->scrollOverrideSet(zftrue);
    this->autoScrollStopX();
    this->autoScrollStopY();
    this->scrollOverrideSet(zffalse);
    d->processDragBegin(mousePos, mouseTime);

    if(d->scrollOverrideFlag == 0)
    {
        this->scrollOnScrolledByUser();
    }
}
void ZFUIScrollView::_ZFP_ZFUIScrollView_notifyDrag(ZF_IN const ZFUIPoint &mousePos,
                                                    ZF_IN const zftimet &mouseTime)
{
    this->scrollOverrideSet(zftrue);
    this->autoScrollStopX();
    this->autoScrollStopY();
    this->scrollOverrideSet(zffalse);
    d->processDrag(mousePos, mouseTime);
}
void ZFUIScrollView::_ZFP_ZFUIScrollView_notifyDragEnd(ZF_IN const zftimet &mouseTime,
                                                       ZF_IN zfbool needScrollAni)
{
    this->scrollOverrideSet(zftrue);
    this->autoScrollStopX();
    this->autoScrollStopY();
    this->scrollOverrideSet(zffalse);
    d->processDragEnd(mouseTime, needScrollAni);
}
void ZFUIScrollView::_ZFP_ZFUIScrollView_notifyScrollAnimation(ZF_IN const zftimet &relativeTimeInMiliseconds)
{
    d->processScrollAnimation(relativeTimeInMiliseconds);
}

ZF_NAMESPACE_GLOBAL_END

