/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreTypeDef_CoreType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_CoreType_h_
#define _ZFI_ZFCoreTypeDef_CoreType_h_

#include "ZFCoreTypeDef_CoreIntType.h"
#include <stdarg.h>
#include <stdio.h>
#include <new>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief same as NULL, defined for future use
 */
#ifndef zfnull
    #if 1
        #define zfnull NULL
    #elif defined(__cplusplus) && (__cplusplus >= 201103L) && 0
        #define zfnull nullptr
    #else
        #define zfnull NULL
    #endif
#endif

// ============================================================
/**
 * @brief bool type
 */
ZFT_INT_WEAK(_ZFT_zfbool, zfbool);
/**
 * @brief bool true type
 */
#define zftrue _ZFT_zftrue
/**
 * @brief bool false type
 */
#define zffalse _ZFT_zffalse

// ============================================================
/**
 * @brief similar to size_t, used for index and size only
 *
 * we use these number types in ZFFramework:
 * -  zfint, zfuint: normal integer types, size depends on environment
 * -  zfindex: integer types used for index and size
 * -  zffloat/zfdouble/zflongdouble: most widely used float type in ZFFramework, size depends on environment
 * -  zfbyte: for byte types
 * -  zftimet: for time types
 * -  zfflags: for flag types
 * -  zfidentity: for identity types
 *
 *
 * @warning take good care of that, zfindex is not ensured same as size_t,
 *   so it is not ensured that zfindexMax is equal to zfstring::npos,
 *   e.g.:
 *   @code
 *     zfindex n = std::string::npos;
 *     zfbool b0 = (n == std::string::npos); // true
 *     zfbool b1 = (zfindexMax == std::string::npos); // not ensured
 *   @endcode
 * @note all integer types are ensured differs from each other (by #ZFT_INT_STRONG),
 *   so that it's convenient to achieve template specialization\n
 *   however, as the cost, some math operators may have strange behaviors,
 *   use static_cast if necessary\n
 *   we ensure these types are primitive:
 *   -  zfbool
 *   -  zfchar
 *   -  zfbyte
 *   -  zfint
 *   -  zfuint
 *   -  zfindex
 *   -  zffloat
 *
 *   and only primitive types are safe to be passed to va_arg\n
 *   \n
 *   for types wrapped by #ZFT_INT_STRONG,
 *   you may use zft_XXX to access original primitive type
 */
ZFT_INT_WEAK(_ZFT_zfindex, zfindex)
/**
 * @brief (zfindex)-1, indicate a max index value, see #zfindex
 * @see zfuint
 */
#define zfindexMax ((zfindex)-1)
/** @brief zero value */
#define zfindexZero ((zfindex)0)

// ============================================================
/**
 * @brief same as int, see #zfindex
 */
ZFT_INT_WEAK(_ZFT_zfint, zfint);
/** @brief zero value */
#define zfintZero ((zfint)0)
/**
 * @brief same as unsigned int, see #zfindex
 */
ZFT_INT_WEAK(_ZFT_zfuint, zfuint);
/** @brief zero value */
#define zfuintZero ((zfuint)0)

// ============================================================
/**
 * @brief same as float, see #zfindex
 */
ZFT_INT_WEAK(_ZFT_zffloat, zffloat);
/** @brief zero value */
#define zffloatZero ((zffloat)0)

/**
 * @brief same as double, see #zfindex
 */
ZFT_INT_STRONG(double, zfdouble)
/** @brief zero value */
extern ZF_ENV_EXPORT const zfdouble zfdoubleZero;

/**
 * @brief same as long double, see #zfindex
 */
ZFT_INT_STRONG(long double, zflongdouble)
/** @brief zero value */
extern ZF_ENV_EXPORT const zflongdouble zflongdoubleZero;

// ============================================================
/**
 * @brief 8-bit unsigned value, see #zfindex
 */
ZFT_INT_WEAK(_ZFT_zfbyte, zfbyte);
/** @brief zero value */
#define zfbyteZero ((zfbyte)0)

// ============================================================
/**
 * @brief time unit, ensured at least 64 bit, ensured signed
 *
 * time unit of zftimet is not required,
 * but usually use miliseconds in ZFFramework
 */
ZFT_INT_STRONG(zft_zfint64, zftimet)
/** @brief zero value */
extern ZF_ENV_EXPORT const zftimet zftimetZero;

// ============================================================
/**
 * @brief used to hold flags, ensured at least 32 bit, ensured unsigned
 */
ZFT_INT_STRONG_WITH_BIT(zft_zfuint32, zfflags)
/** @brief zero value */
extern ZF_ENV_EXPORT const zfflags zfflagsZero;

// ============================================================
/**
 * @brief identity type, ensured at least 32 bit, ensured unsigned
 */
ZFT_INT_STRONG_WITH_BIT(zft_zfindex, zfidentity)
/** @brief zero value */
extern ZF_ENV_EXPORT const zfidentity zfidentityZero;
/**
 * @brief an invalid id value, ensured ((zfidentity)-1)
 */
extern ZF_ENV_EXPORT const zfidentity zfidentityInvalid;
/**
 * @brief util macro to check whether the id value is valid
 */
#define zfidentityIsValid(value) ((value) != zfidentityInvalid)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_CoreType_h_

