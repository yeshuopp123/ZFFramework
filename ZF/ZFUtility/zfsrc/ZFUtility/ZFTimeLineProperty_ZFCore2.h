/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTimeLineProperty_ZFCore2.h
 * @brief see #ZFTimeLineProperty
 */
#ifndef _ZFI_ZFTimeLineProperty_ZFCore2_h_
#define _ZFI_ZFTimeLineProperty_ZFCore2_h_

#include "ZFTimeLineProperty.h"
// ZFDoxygenBugFix
// this file was separated from ZFTimeLineProperty_ZFCore.h
// because of doxygen can not parse large macro generated file
// the original error message was:
//   "input buffer overflow, can't enlarge buffer because scanner uses REJECT"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFTIMELINEPROPERTY_DECLARE_COMMON(zffloat)
ZFTIMELINEPROPERTY_DECLARE_COMMON(zfdouble)
ZFTIMELINEPROPERTY_DECLARE_COMMON(zflongdouble)
ZFTIMELINEPROPERTY_DECLARE_COMMON(zftimet)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLineProperty_ZFCore2_h_

