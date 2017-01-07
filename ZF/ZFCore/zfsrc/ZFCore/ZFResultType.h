/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFResultType.h
 * @brief result type define
 */

#ifndef _ZFI_ZFResultType_h_
#define _ZFI_ZFResultType_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief result type define
 */
ZFENUM_BEGIN(ZFResultType)
    ZFENUM_VALUE(Success)
    ZFENUM_VALUE(Fail)
    ZFENUM_VALUE(Cancel)
ZFENUM_SEPARATOR(ZFResultType)
    ZFENUM_VALUE_REGISTER(Success)
    ZFENUM_VALUE_REGISTER(Fail)
    ZFENUM_VALUE_REGISTER(Cancel)
ZFENUM_END(ZFResultType)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFResultType_h_

