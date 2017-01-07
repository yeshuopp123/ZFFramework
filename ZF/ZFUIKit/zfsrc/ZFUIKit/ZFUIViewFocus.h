/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIViewFocus.h
 * @brief ZFUIView focus utility
 */

#ifndef _ZFI_ZFUIViewFocus_h_
#define _ZFI_ZFUIViewFocus_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief manually set the next focus target for the from view, set null to remove
 */
extern ZF_ENV_EXPORT void ZFUIViewFocusNextSet(ZF_IN ZFUIView *from, ZF_IN ZFUIView *nextFocus);

#define _ZFP_ZFUIViewFocusNextSetChainEndPtr ((ZFUIView *)-1)
extern ZF_ENV_EXPORT void _ZFP_ZFUIViewFocusNextSetChain(ZF_IN ZFUIView *view0, ZF_IN ZFUIView *view1, ...);
/**
 * @brief util method to chain all view's next focus target by #ZFUIViewFocusNextSet
 */
#define ZFUIViewFocusNextSetChain(view0, view1, ...) \
    _ZFP_ZFUIViewFocusNextSetChain(view0, view1, ##__VA_ARGS__, _ZFP_ZFUIViewFocusNextSetChainEndPtr)

// ============================================================
/**
 * @brief filter to exclude certain view from being focused by #ZFUIViewFocusNextFind, empty by default
 */
extern ZF_ENV_EXPORT ZFFilterForZFObject ZFUIViewFocusNextFilter;

/**
 * @brief param for #ZFUIViewFocusNextFind and #ZFUIViewFocusNextMove
 */
zfclassLikePOD ZF_ENV_EXPORT ZFUIViewFocusNextParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFUIViewFocusNextParam)

    /**
     * @brief direction to find, #ZFUIOrientation::e_Right | #ZFUIOrientation::e_Bottom by default
     */
    ZFCORE_PARAM_WITH_INIT(ZFUIOrientationFlags, focusDirection, (ZFUIOrientation::e_Right | ZFUIOrientation::e_Bottom))
    /**
     * @brief whether find in loop mode, false by default
     */
    ZFCORE_PARAM_WITH_INIT(zfbool, focusLoopMode, zffalse)
    /**
     * @brief only find within this view, null to end with #ZFUIWindow, null by default
     */
    ZFCORE_PARAM(ZFUIView *, focusEndParent)
    /**
     * @brief whether to find internal views, true by default
     */
    ZFCORE_PARAM_WITH_INIT(zfbool, focusInternalViews, zftrue)
};

// ============================================================
/**
 * @brief find next focusable from view (excluding) with direction
 *
 * return first focusable view if found
 */
extern ZF_ENV_EXPORT ZFUIView *ZFUIViewFocusNextFind(ZF_IN ZFUIView *view,
                                                     ZF_IN_OPT const ZFUIViewFocusNextParam &param = ZFUIViewFocusNextParam());

/**
 * @brief move focus to next if available or do nothing otherwise, see #ZFUIViewFocusNextFind
 */
extern ZF_ENV_EXPORT ZFUIView *ZFUIViewFocusNextMove(ZF_IN ZFUIView *view,
                                                     ZF_IN_OPT const ZFUIViewFocusNextParam &param = ZFUIViewFocusNextParam());

// ============================================================
/**
 * @brief util method to resolve key event (such as up/down/left/right keys) and check to move focus, see #ZFUIViewFocusNextFind
 *
 * return true if the event has been resolved,
 * next focused view can be checked by the optional nextFocus param
 */
extern ZF_ENV_EXPORT zfbool ZFUIViewFocusResolveKeyEvent(ZF_IN ZFUIView *view,
                                                         ZF_IN ZFUIKeyEvent *keyEvent,
                                                         ZF_OUT_OPT ZFUIView **nextFocus = zfnull,
                                                         ZF_IN_OPT ZFUIView *endParent = zfnull);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewFocus_h_

