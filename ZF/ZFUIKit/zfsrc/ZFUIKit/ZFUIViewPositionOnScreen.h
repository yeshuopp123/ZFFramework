/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIViewPositionOnScreen.h
 * @brief get view's position on screen
 */

#ifndef _ZFI_ZFUIViewPositionOnScreen_h_
#define _ZFI_ZFUIViewPositionOnScreen_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief get view's position on screen
 *
 * @note result value would be invalid until whole layout step finished,
 *   due to impl's limitation,
 *   we are unable to be notified which time the layout step would finish,
 *   so the best solution to check valid position is using delay,
 *   use #ZFThreadTaskRequest is recommended
 */
extern ZF_ENV_EXPORT void ZFUIViewPositionOnScreen(ZF_OUT ZFUIRect &rect, ZF_IN ZFUIView *view);
/**
 * @brief get view's position on screen
 */
inline ZFUIRect ZFUIViewPositionOnScreen(ZF_IN ZFUIView *view)
{
    ZFUIRect rect = ZFUIRectZero;
    ZFUIViewPositionOnScreen(rect, view);
    return rect;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewPositionOnScreen_h_

