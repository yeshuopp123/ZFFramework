/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreVersionDef.h
 * @brief utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreVersionDef_h_
#define _ZFI_ZFCoreVersionDef_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreUtilComparerDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief get sub version at subVersionIndex
 *
 * e.g.\n
 * check 0 as subVersionIndex from "a.b.c" would result "a"\n
 * check 4 as subVersionIndex from "a.b.c" would result an empty string
 */
extern ZF_ENV_EXPORT void ZFVersionGet(ZF_OUT zfstring &ret,
                                       ZF_IN const zfchar *version,
                                       ZF_IN zfindex subVersionIndex);
/** @brief see #ZFVersionGet */
inline zfstring ZFVersionGet(ZF_IN const zfchar *version,
                             ZF_IN zfindex subVersionIndex)
{
    zfstring ret;
    ZFVersionGet(ret, version, subVersionIndex);
    return ret;
}
/**
 * @brief util method to get int version value of #ZFVersionGet
 */
extern ZF_ENV_EXPORT void ZFVersionGetInt(ZF_OUT zfuint &ret,
                                          ZF_IN const zfchar *version,
                                          ZF_IN zfindex subVersionIndex);
/** @brief see #ZFVersionGetInt */
inline zfuint ZFVersionGetInt(ZF_IN const zfchar *version,
                              ZF_IN zfindex subVersionIndex)
{
    zfuint ret = 0;
    ZFVersionGetInt(ret, version, subVersionIndex);
    return ret;
}
/**
 * @brief set sub version at subVersionIndex
 *
 * e.g.\n
 * set "c" to 0 as subVersionIndex to "a.b" would result "c.b"\n
 * set "d" to 4 as subVersionIndex to "a.b" would result "a.b.0.d"
 */
extern ZF_ENV_EXPORT void ZFVersionSet(ZF_IN_OUT zfstring &version,
                                       ZF_IN zfindex subVersionIndex,
                                       ZF_IN const zfchar *subVersion,
                                       ZF_IN_OPT const zfchar *emptySubVersion = zfText("0"));
/**
 * @brief util method to set int version value of #ZFVersionSet
 */
extern ZF_ENV_EXPORT void ZFVersionSetInt(ZF_IN_OUT zfstring &version,
                                          ZF_IN zfindex subVersionIndex,
                                          ZF_IN zfuint subVersion,
                                          ZF_IN_OPT const zfchar *emptySubVersion = zfText("0"));

// ============================================================
extern ZF_ENV_EXPORT ZFCompareResult _ZFP_ZFComparerForVersion(ZF_IN const zfchar *const &v0, ZF_IN const zfchar *const &v1);
/**
 * @brief default comparer for sub version compare, compared by string's integer value
 *
 * value is treated as 36 radix, so 'a' ~ 'z' letters can be compared normally,
 * while 'a' and 'A' are treated the same (case insensitive)
 */
#define ZFComparerForVersion _ZFP_ZFComparerForVersion

/**
 * @brief compare two version
 *
 * while comparing a version which has subversion,
 * a version would always treated as smaller if it has no sub version
 */
extern ZF_ENV_EXPORT ZFCompareResult ZFVersionCompare(ZF_IN const zfchar *version0,
                                                      ZF_IN const zfchar *version1,
                                                      ZF_IN_OPT ZFComparer<const zfchar *>::Comparer subVersionComparer = ZFComparerForVersion);

// ============================================================
// version index
/**
 * @brief pre-defined subVersionIndex for #ZFVersionSet and #ZFVersionGet
 *
 * a typical version would looks like:
 * @code
 *   11.22.33a.r44
 *
 *   11: main
 *   22: sub
 *   33a: minor
 *   r44: build
 * @endcode
 */
#define ZFVersion_main 0
/** @brief see #ZFVersion_main */
#define ZFVersion_sub 1
/** @brief see #ZFVersion_main */
#define ZFVersion_minor 2
/** @brief see #ZFVersion_main */
#define ZFVersion_build 3

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreVersionDef_h_

