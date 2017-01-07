/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIOnScreenKeyboardAutoResize.h
 * @brief auto fit #ZFUIWindow accorrding to #ZFUIOnScreenKeyboardState
 */

#ifndef _ZFI_ZFUIOnScreenKeyboardAutoResize_h_
#define _ZFI_ZFUIOnScreenKeyboardAutoResize_h_

#include "ZFUIWidgetDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief auto fit #ZFUIWindow accorrding to #ZFUIOnScreenKeyboardState
 *
 * adjust window's layout param when on screen keyboard show or hide\n
 * you must not modify window's layout param's #ZFUIViewLayoutParam::layoutMargin
 * while the auto fit is activated,
 * it would be restored during #ZFUIOnScreenKeyboardAutoResizeStop
 * and your modification would be lost
 */
extern ZF_ENV_EXPORT void ZFUIOnScreenKeyboardAutoResizeStart(ZF_IN ZFUIWindow *window);
/** @brief see #ZFUIOnScreenKeyboardAutoResizeStart */
extern ZF_ENV_EXPORT void ZFUIOnScreenKeyboardAutoResizeStop(ZF_IN ZFUIWindow *window);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIOnScreenKeyboardAutoResize_h_

