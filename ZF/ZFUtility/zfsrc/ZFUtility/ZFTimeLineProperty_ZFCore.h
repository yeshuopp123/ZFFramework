/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTimeLineProperty_ZFCore.h
 * @brief see #ZFTimeLineProperty
 */

#ifndef _ZFI_ZFTimeLineProperty_ZFCore_h_
#define _ZFI_ZFTimeLineProperty_ZFCore_h_

#include "ZFTimeLineProperty.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFTIMELINEPROPERTY_DECLARE_BY_VALUE_DETAIL(ZFTimeLineProperty_zfbool, zfbool, {
    return ((progress <= 0.5f) ? fromValue : toValue);
})
ZFTIMELINEPROPERTY_DECLARE_BY_VALUE(zfint)
ZFTIMELINEPROPERTY_DECLARE_BY_VALUE(zfuint)
ZFTIMELINEPROPERTY_DECLARE_BY_VALUE(zfindex)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLineProperty_ZFCore_h_

