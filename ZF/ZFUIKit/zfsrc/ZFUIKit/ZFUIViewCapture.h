/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIViewCapture.h
 * @brief capture ZFUIView to ZFUIImage
 */

#ifndef _ZFI_ZFUIViewCapture_h_
#define _ZFI_ZFUIViewCapture_h_

#include "ZFUIView.h"
#include "ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief capture ZFUIView to ZFUIImage
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIViewCapture(ZF_IN ZFUIView *view);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewCapture_h_

