/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreTypeDef_CoreIntType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_CoreIntType_h_
#define _ZFI_ZFCoreTypeDef_CoreIntType_h_

#include "ZFNamespaceDef.h"
#include "ZFCoreEnvDef.h"

ZF_ENV_SENSITIVE
#if defined(__GNUC__)
    #define _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint 1
#elif defined(_MSC_VER)
    #if _MSC_VER >= 1600
        #define _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint 1
    #else
        #define _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint 0
    #endif
#elif defined(__BORLANDC__)
    #if __BORLANDC__ >= 0x0560
        #define _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint 1
    #else
        #define _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint 0
    #endif
#else
    #define _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint 1
#endif
#if _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint
    #include <stdint.h>

    typedef int8_t _ZFP_zfint8;
    typedef uint8_t _ZFP_zfuint8;

    typedef int16_t _ZFP_zfint16;
    typedef uint16_t _ZFP_zfuint16;

    typedef int32_t _ZFP_zfint32;
    typedef uint32_t _ZFP_zfuint32;

    typedef int64_t _ZFP_zfint64;
    typedef uint64_t _ZFP_zfuint64;
#else
    typedef signed char _ZFP_zfint8;
    typedef unsigned char _ZFP_zfuint8;

    typedef signed short int _ZFP_zfint16;
    typedef unsigned short int _ZFP_zfuint16;

    typedef signed long int _ZFP_zfint32;
    typedef unsigned long int _ZFP_zfuint32;

    typedef signed long long int _ZFP_zfint64;
    typedef unsigned long long int _ZFP_zfuint64;
#endif // #if _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint
#undef _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief 8-bit signed value, see #zfindex
 */
#ifndef _ZFT_zfint8
    typedef _ZFP_zfint8 zfint8;
#else
    typedef _ZFT_zfint8 zfint8;
#endif
/**
 * @brief 8-bit unsigned value, see #zfindex
 */
#ifndef _ZFT_zfuint8
    typedef _ZFP_zfuint8 zfuint8;
#else
    typedef _ZFT_zfuint8 zfuint8;
#endif

/**
 * @brief 16-bit signed value, see #zfindex
 */
#ifndef _ZFT_zfint16
    typedef _ZFP_zfint16 zfint16;
#else
    typedef _ZFT_zfint16 zfint16;
#endif
/**
 * @brief 16-bit unsigned value, see #zfindex
 */
#ifndef _ZFT_zfuint16
    typedef _ZFP_zfuint16 zfuint16;
#else
    typedef _ZFT_zfuint16 zfuint16;
#endif

/**
 * @brief 32-bit signed value, see #zfindex
 */
#ifndef _ZFT_zfint32
    typedef _ZFP_zfint32 zfint32;
#else
    typedef _ZFT_zfint32 zfint32;
#endif
/**
 * @brief 32-bit unsigned value, see #zfindex
 */
#ifndef _ZFT_zfuint32
    typedef _ZFP_zfuint32 zfuint32;
#else
    typedef _ZFT_zfuint32 zfuint32;
#endif

/**
 * @brief 64-bit signed value, see #zfindex
 */
#ifndef _ZFT_zfint64
    typedef _ZFP_zfint64 zfint64;
#else
    typedef _ZFT_zfint64 zfint64;
#endif
/**
 * @brief 64-bit unsigned value, see #zfindex
 */
#ifndef _ZFT_zfuint64
    typedef _ZFP_zfuint64 zfuint64;
#else
    typedef _ZFT_zfuint64 zfuint64;
#endif

/**
 * @brief same as int, see #zfindex
 *
 * ensured no less than 16-bit, maybe 32-bit in 32-bit system
 * or 64-bit in 64-bit system
 * @note size of this type is not fixed,
 *   use zfint8, zfuint8, zfint16, zfuint16, etc, if necessary
 * @see zfuint, zfindex
 */
#ifndef _ZFT_zfint
    typedef int zfint;
#else
    typedef _ZFT_zfint zfint;
#endif
/**
 * @brief same as unsigned int, see #zfindex
 *
 * ensured no less than 16-bit, maybe 32-bit in 32-bit system
 * or 64-bit in 64-bit system
 * @note size of this type is not fixed,
 *   use zfint8, zfuint8, zfint16, zfuint16, etc, if necessary
 * @see zfint, zfindex
 */
#ifndef _ZFT_zfuint
    typedef unsigned int zfuint;
#else
    typedef _ZFT_zfuint zfuint;
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_CoreIntType_h_

