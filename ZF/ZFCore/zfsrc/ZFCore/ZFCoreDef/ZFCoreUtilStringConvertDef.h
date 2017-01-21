/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreUtilStringConvertDef.h
 * @brief utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreUtilStringConvertDef_h_
#define _ZFI_ZFCoreUtilStringConvertDef_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief check whether the toCompare matches the tokens, return the index in tokens or zfindexMax if not matched
 */
extern ZF_ENV_EXPORT zfindex ZFCoreStringCheckMatch(ZF_IN const zfchar **tokens,
                                                    ZF_IN zfindex tokenCount,
                                                    ZF_IN const zfchar *toCompare,
                                                    ZF_IN_OPT zfindex toCompareLength = zfindexMax);

// ============================================================
/**
 * @brief util macro to declare type string converter for ZFFramework
 */
#define ZFCORETYPE_STRING_CONVERTER_DECLARE(TypeName, Type) \
    /** @brief util method to convert TypeName from string, return null if success or error position if fail */ \
    extern ZF_ENV_EXPORT const zfchar *TypeName##FromString(ZF_OUT Type &v, \
                                                            ZF_IN const zfchar *src, \
                                                            ZF_IN_OPT zfindex srcLen = zfindexMax); \
    /** @brief util method to convert TypeName to string */ \
    extern ZF_ENV_EXPORT void TypeName##ToString(ZF_IN_OUT zfstring &s, ZF_IN Type const &v); \
    /** @brief util method to convert TypeName to string */ \
    inline zfstring TypeName##ToString(ZF_IN Type const &v) \
    { \
        zfstring s; \
        TypeName##ToString(s, v); \
        return s; \
    }
/**
 * @brief util macro to declare type string converter for ZFFramework
 *
 * @code
 *   ZFCORETYPE_STRING_CONVERTER_DEFINE(YourTypeName, YourType, {
 *           // proto type:
 *           //   const zfchar *YourTypeNameFromString(ZF_OUT YourType &v,
 *           //       ZF_IN const zfchar *src,
 *           //       ZF_IN_OPT zfindex srcLen = zfindexMax);
 *           // return null if success, or error position if fail
 *       }, {
 *           // proto type:
 *           //   void YourTypeNameToString(ZF_IN_OUT zfstring &s,
 *           //       ZF_IN YourType const &v);
 *           // append result to s
 *       })
 * @endcode
 */
#define ZFCORETYPE_STRING_CONVERTER_DEFINE(TypeName, Type, convertFromStringAction, convertToStringAction) \
    const zfchar *TypeName##FromString(ZF_OUT Type &v, \
                                       ZF_IN const zfchar *src, \
                                       ZF_IN_OPT zfindex srcLen /* = zfindexMax */) \
    { \
        convertFromStringAction \
    } \
    void TypeName##ToString(ZF_IN_OUT zfstring &s, ZF_IN Type const &v) \
    { \
        convertToStringAction \
    }

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DECLARE(zfchar, zfchar)

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DECLARE(zfstring, zfstring)
/** @cond ZFPrivateDoc */
inline void zfstringToString(ZF_IN_OUT zfstring &s,
                             ZF_IN const zfchar *v)
{
    s += v;
}
inline zfstring zfstringToString(ZF_IN const zfchar *v)
{
    return zfstring(v);
}
inline const zfchar *zfstringFromString(ZF_OUT const zfchar *&v,
                                        ZF_IN const zfchar *src,
                                        ZF_IN_OPT zfindex srcLen = zfindexMax)
{
    if(srcLen != zfindexMax && src[srcLen] != '\0')
    {
        return src + srcLen;
    }
    v = src;
    return zfnull;
}
/** @endcond */

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DECLARE(zfbool, zfbool)

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DECLARE(zfbyte, zfbyte)

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DECLARE(zfint, zfint)
// ============================================================
/**
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFCORETYPE_STRING_CONVERTER_DECLARE(zfuint, zfuint)

// ============================================================
/**
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFCORETYPE_STRING_CONVERTER_DECLARE(zfindex, zfindex)

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DECLARE(zffloat, zffloat)

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DECLARE(zfdouble, zfdouble)

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DECLARE(zflongdouble, zflongdouble)

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DECLARE(zftimet, zftimet)

// ============================================================
/**
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFCORETYPE_STRING_CONVERTER_DECLARE(zfflags, zfflags)

// ============================================================
/**
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFCORETYPE_STRING_CONVERTER_DECLARE(zfidentity, zfidentity)

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFCompareResult, ZFCompareResult)

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFSeekPos, ZFSeekPos)

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DECLARE(zfindexRange, zfindexRange)

// ============================================================
// flags and string conversion
/**
 * @brief convert flags to string using flagList and nameList
 *
 * result would be like "Flag0|Flag1|0x123ABC"\n
 * if flags is 0 and there's no 0 value in flagList, or error occurred, "0x0" would be returned
 * params:
 * -  includeNotConverted
 *   if true, values not recognized would be appended to result as HEX value
 * -  exclusiveMode if true, when a flag recognized,
 *   flags would be unset and can't be recognized any more\n
 *   for example, if we have two flag 0101 and 0011 in flagList which shares 0001 bit,
 *   then if we are converting 0111, since 0101 would be recognized first
 *   and leave 0010, 0011 won't be recognized any more
 * -  notConverted if all value recognized, notConverted would be set to 0,
 *   otherwise, the not converted value would be set
 */
extern ZF_ENV_EXPORT void zfflagsToString(ZF_IN_OUT zfstring &ret,
                                          ZF_IN const zfflags *flagList,
                                          ZF_IN const zfchar **nameList,
                                          ZF_IN zfindex listCount,
                                          ZF_IN zfflags const &value,
                                          ZF_IN_OPT zfbool includeNotConverted = zftrue,
                                          ZF_IN_OPT zfbool exclusiveMode = zffalse,
                                          ZF_OUT_OPT zfflags *notConverted = zfnull,
                                          ZF_IN_OPT zfchar separatorToken = '|');
/** @brief see #zfflagsToString */
inline zfstring zfflagsToString(ZF_IN const zfflags *flagList,
                                ZF_IN const zfchar **nameList,
                                ZF_IN zfindex listCount,
                                ZF_IN zfflags const &value,
                                ZF_IN_OPT zfbool includeNotConverted = zftrue,
                                ZF_IN_OPT zfbool exclusiveMode = zffalse,
                                ZF_OUT_OPT zfflags *notConverted = zfnull,
                                ZF_IN_OPT zfchar separatorToken = '|')
{
    zfstring ret;
    zfflagsToString(ret, flagList, nameList, listCount, value, includeNotConverted, exclusiveMode, notConverted, separatorToken);
    return ret;
}

/**
 * @brief convert string to zfflags, return null if success or error position if failed
 *
 * params:
 * -  pErrPos pointer to first char where error occurred,
 *   a non converted format is not regarded as error
 */
extern ZF_ENV_EXPORT const zfchar *zfflagsFromString(ZF_OUT zfflags &ret,
                                                     ZF_IN const zfflags *flagList,
                                                     ZF_IN const zfchar **nameList,
                                                     ZF_IN zfindex listCount,
                                                     ZF_IN const zfchar *src,
                                                     ZF_IN_OPT zfindex srcLen = zfindexMax,
                                                     ZF_IN_OPT zfchar separatorToken = '|');

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreUtilStringConvertDef_h_

