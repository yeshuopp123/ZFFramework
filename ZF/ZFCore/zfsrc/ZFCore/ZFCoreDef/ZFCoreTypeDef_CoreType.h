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
#include <stddef.h>
#include <stdio.h>
#include <new>

ZF_NAMESPACE_GLOBAL_BEGIN

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
#ifndef _ZFT_zfbool
    typedef bool zfbool;
#else
    typedef _ZFT_zfbool zfbool;
#endif
/**
 * @brief bool true type
 */
#ifndef zftrue
    #define zftrue true
#endif
/**
 * @brief bool false type
 */
#ifndef zffalse
    #define zffalse false
#endif

/**
 * @brief similar to size_t, used for index and size only
 *
 * we use these number types in ZFFramework:
 * -  zfint, zfuint: normal integer types, size depends on environment
 * -  zfindex: integer types used for index and size
 * -  zfint8 ~ zfint64: integer types that needs certain size
 * -  zffloat: most widely used float type in ZFFramework, size depends on environment
 * -  zfdouble, zflongdouble: only used for accuracy, size depends on environment
 * -  zfbyte: for byte types
 * -  zftimet: for time types
 *
 * we ensure that zfindex is not less than zfuint
 * @see zfindexMax
 *
 * @warning take good care of that, zfindex is not ensured same as size_t,
 *   so it is not ensured that zfindexMax is equal to zfstring::npos,
 *   e.g.:
 *   @code
 *     zfindex n = std::string::npos;
 *     zfbool b0 = (n == std::string::npos); // true
 *     zfbool b1 = (zfindexMax == std::string::npos); // not ensured
 *   @endcode
 */
#ifndef _ZFT_zfindex
    typedef size_t zfindex;
#else
    typedef _ZFT_zfindex zfindex;
#endif
/**
 * @brief (zfindex)-1, indicate a max index value, see #zfindex
 * @see zfuint
 */
#define zfindexMax ((zfindex)-1)

/**
 * @brief same as float, see #zfindex
 */
#ifndef _ZFT_zffloat
    typedef float zffloat;
#else
    typedef _ZFT_zffloat zffloat;
#endif

/**
 * @brief same as double, see #zfindex
 */
#ifndef _ZFT_zfdouble
    typedef double zfdouble;
#else
    typedef _ZFT_zfdouble zfdouble;
#endif

/**
 * @brief same as long double, see #zfindex
 */
#ifndef _ZFT_zflongdouble
    typedef long double zflongdouble;
#else
    typedef _ZFT_zflongdouble zflongdouble;
#endif

/**
 * @brief 8-bit unsigned value, see #zfindex
 */
#ifndef _ZFT_zfbyte
    typedef zfuint8 zfbyte;
#else
    typedef _ZFT_zfbyte zfbyte;
#endif

/**
 * @brief time unit, ensured at least 64 bit, ensured signed
 *
 * time unit of zftimet is not required,
 * but usually use miliseconds in ZFFramework
 */
#ifndef _ZFT_zftimet
    typedef zfint64 zftimet;
#else
    typedef _ZFT_zftimet zftimet;
#endif

/**
 * @brief used to hold flags, ensured at least 32 bit, ensured unsigned
 */
#ifndef _ZFT_zfflags
    typedef zfuint32 zfflags;
#else
    typedef _ZFT_zfflags zfflags;
#endif

/**
 * @brief identity type, ensured at least 32 bit, ensured unsigned
 */
#ifndef _ZFT_zfidentity
    typedef zfuint32 zfidentity;
#else
    typedef _ZFT_zfidentity zfidentity;
#endif
/**
 * @brief an invalid id value, ensured ((zfidentity)-1)
 */
#define zfidentityInvalid ((zfidentity)-1)

/**
 * @brief util macro to check whether the id value is valid
 */
#define zfidentityIsValid(value) ((value) != zfidentityInvalid)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_CoreType_h_

