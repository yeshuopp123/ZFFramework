/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreDataEncodeDef.h
 * @brief util to encode data to printable string
 */

#ifndef _ZFI_ZFCoreDataEncodeDef_h_
#define _ZFI_ZFCoreDataEncodeDef_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFCoreDataEncode, default is all numbers and all lower and upper letters
 *
 * here's a list of contents:
 * @code
    // 0x00 ~ 0x0F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x10 ~ 0x1F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x20 ~ 0x2F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x30 ~ 0x3F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    // 0x40 ~ 0x4F
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x50 ~ 0x5F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    // 0x60 ~ 0x6F
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x70 ~ 0x7F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    // 0x80 ~ 0x8F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x90 ~ 0x9F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xA0 ~ 0xAF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xB0 ~ 0xBF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xC0 ~ 0xCF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xD0 ~ 0xDF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xE0 ~ 0xEF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xF0 ~ 0xFF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 * @endcode
 */
extern ZF_ENV_EXPORT const zfchar ZFCoreDataEncodeCharMapDefault[256];

/**
 * @brief util to encode data to printable string
 *
 * use charMap to declare the printable chars,
 * it's a 256 size zfchar array,
 * zero element means the char isn't printable and non-zero means printable\n
 * not printable chars would be escaped by escapeToken
 */
extern ZF_ENV_EXPORT void ZFCoreDataEncode(ZF_OUT zfstring &result,
                                           ZF_IN const zfchar *src,
                                           ZF_IN_OPT zfindex srcLen = zfindexMax,
                                           ZF_IN_OPT const zfchar (&charMap)[256] = ZFCoreDataEncodeCharMapDefault,
                                           ZF_IN_OPT zfchar escapeToken = '%');
/**
 * @brief see #ZFCoreDataEncode
 */
extern ZF_ENV_EXPORT void ZFCoreDataDecode(ZF_OUT zfstring &result,
                                           ZF_IN const zfchar *src,
                                           ZF_IN_OPT zfindex srcLen = zfindexMax,
                                           ZF_IN_OPT zfchar escapeToken = '%');

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreDataEncodeDef_h_

