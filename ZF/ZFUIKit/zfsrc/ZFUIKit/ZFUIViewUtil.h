/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIViewUtil.h
 * @brief utils for ZFUIView
 */

#ifndef _ZFI_ZFUIViewUtil_h_
#define _ZFI_ZFUIViewUtil_h_

#include "ZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFUIViewUtil)

/**
 * @brief recursive find the root ZFUIView parent of this view,
 *   may return self if no parent
 */
extern ZF_ENV_EXPORT ZFUIView *viewRoot(ZF_IN ZFUIView *view);

/**
 * @brief check whether this view is child of specified view (recursively)
 *
 * true if view equals to this view
 */
extern ZF_ENV_EXPORT zfbool viewIsChildOf(ZF_IN ZFUIView *view, ZF_IN ZFUIView *parentToCheck);

/**
 * @brief recursively check child at position (relative to the view)
 *
 * auto exclude disabled view\n
 * may be invalid if layout step not finished
 */
extern ZF_ENV_EXPORT ZFUIView *viewChildAt(ZF_IN ZFUIView *view,
                                           ZF_IN const ZFUIPoint &pos,
                                           ZF_IN_OPT zfbool filterDisabledView = zftrue,
                                           ZF_IN_OPT const ZFFilterForZFObject *filter = zfnull);

/**
 * @brief check rect of view relative to parent,
 *   return #ZFUIRectZero if not child of parent or invalid
 *
 * result value would be invalid until whole layout step finished\n
 * unlike #ZFUIViewPositionOnScreen,
 * this method does not depends on impl,
 * however, result may be invalid if view's parent contains #ZFUINativeViewWrapper\n
 * this method would calculate rect depends on #ZFUIView::layoutedFrameFixed recursively
 */
extern ZF_ENV_EXPORT void viewRectToParent(ZF_OUT ZFUIRect &rect, ZF_IN ZFUIView *view, ZF_IN ZFUIView *parent);
/** @brief see #viewRectToParent */
inline ZFUIRect viewRectToParent(ZF_IN ZFUIView *view, ZF_IN ZFUIView *parent)
{
    ZFUIRect ret = ZFUIRectZero;
    ZFUIViewUtil::viewRectToParent(ret, view, parent);
    return ret;
}

ZF_NAMESPACE_END(ZFUIViewUtil)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewUtil_h_

