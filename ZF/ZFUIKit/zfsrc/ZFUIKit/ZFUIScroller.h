/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIScroller.h
 * @brief scroller for #ZFUIScrollView
 */

#ifndef _ZFI_ZFUIScroller_h_
#define _ZFI_ZFUIScroller_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIScroller
/**
 * @brief scroller used to achieve scroll logic,
 *   typically #ZFUIScrollerDefault would suit most cases
 *
 * to use a scroller, you must:
 * -  setup scroll animation callback: #scrollAniStartCallback, #scrollAniStopCallback,
 *   which would notify scroll animation events at proper time (#scrollAniOnUpdate)
 * -  change settings by proper method
 * -  notify drag events by #scrollOnDragBegin, #scrollOnDrag, #scrollOnDragEnd
 *
 * \n
 * to implement a scroller, you must:
 * -  implement all necessary methods
 * -  invoke scroll animation at proper time, by #scrollAniStartCallback and #scrollAniStopCallback
 */
zfinterface ZF_ENV_EXPORT ZFUIScroller : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIScroller, ZFInterface)

public:
    /**
     * @brief used to start scroll animation's timer
     */
    ZFCallbackT<zftimet> scrollAniStartCallback;
    /**
     * @brief used to stop scroll animation's timer
     */
    ZFCallbackT<void> scrollAniStopCallback;

public:
    /**
     * @brief called when owner scroll view's size changed,
     *   you should fix content frame to match bounds if necessary
     */
    virtual void scrollOwnerSizeChanged(ZF_IN const zfint &ownerSize) = 0;
    /**
     * @brief whether use bounce
     */
    virtual void scrollBounceChanged(ZF_IN zfbool scrollBounce,
                                     ZF_IN zfbool scrollBounceAlways) = 0;

    /**
     * @brief whether align to page
     */
    virtual void scrollAlignToPageChanged(ZF_IN zfbool scrollAlignToPage) = 0;

    /**
     * @brief used to update scroll content,
     *   you should keep or update previous scroll animation if necessary
     */
    virtual void scrollContentChanged(ZF_IN zfint contentOffset, ZF_IN zfint contentSize) = 0;

public:
    /**
     * @brief cancel over scroll and ensure content offset in range
     */
    virtual void scrollToFitRange(void) = 0;
    /**
     * @brief util method to stop scroll animation
     */
    virtual void scrollStop(void)
    {
        this->scrollWithoutAnimation(this->scrollContentOffset());
    }
    /**
     * @brief scroll without scroll animation,
     *   you should stop old scroll animation if necessary
     */
    virtual void scrollWithoutAnimation(ZF_IN zfint contentOffset) = 0;
    /**
     * @brief see #ZFUIScrollView::scrollByPoint
     */
    virtual void scrollByPoint(ZF_IN zfint point) = 0;
    /**
     * @brief return end position of #scrollByPoint,
     *   must return current content offset if not scrolling
     */
    virtual zfint scrollByPointEndPoint(void) = 0;
    /**
     * @brief see #ZFUIScrollView::scrollBySpeed
     */
    virtual void scrollBySpeed(ZF_IN zfint speed) = 0;
    /**
     * @brief return current speed of #scrollBySpeed
     */
    virtual zfint scrollBySpeedCurrentSpeed(void) = 0;
    /**
     * @brief return end position of #scrollBySpeed,
     *   must return current content offset if not scrolling
     */
    virtual zfint scrollBySpeedEndPointPredicted(void) = 0;

    /**
     * @brief return end position,
     *   must return current content offset if not scrolling
     */
    virtual zfint scrollEndPointPredicted(void) = 0;

    /**
     * @brief whether this direction's scroller want higher priority,
     *   typically true if currently under progress to scroll to align to page
     */
    virtual zfbool scrollRequireFocus(void) = 0;

public:
    /**
     * @brief used to get current content offset
     */
    virtual zfint scrollContentOffset(void) = 0;
    /**
     * @brief used to get current content size
     */
    virtual zfint scrollContentSize(void) = 0;

public:
    /**
     * @brief drag begin
     *
     * note drag events is not ensured paired
     */
    virtual void scrollOnDragBegin(ZF_IN zfint mousePos,
                                   ZF_IN const zftimet &mouseTime) = 0;
    /**
     * @brief drag
     *
     * note drag events is not ensured paired
     */
    virtual void scrollOnDrag(ZF_IN zfint mousePos,
                              ZF_IN const zftimet &mouseTime) = 0;
    /**
     * @brief drag end
     *
     * note drag events is not ensured paired\n
     * \n
     * if scroll animation is needed, use #scrollAniStartCallback to start
     */
    virtual void scrollOnDragEnd(ZF_IN const zftimet &mouseTime,
                                 ZF_IN zfbool needScrollAni) = 0;

public:
    /**
     * @brief called to update scroll animation
     */
    virtual void scrollAniOnUpdate(ZF_IN const zftimet &time) = 0;
};

// ============================================================
/**
 * @brief change default scroller
 *
 * use null to use default scroller
 */
extern ZF_ENV_EXPORT void ZFUIScrollerClassSet(ZF_IN const ZFClass *cls);
/**
 * @brief get the class of default scroller
 */
extern ZF_ENV_EXPORT const ZFClass *ZFUIScrollerClass(void);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIScroller_h_

