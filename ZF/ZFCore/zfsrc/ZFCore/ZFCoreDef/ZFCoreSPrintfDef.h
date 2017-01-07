/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreSPrintfDef.h
 * @brief string utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreSPrintfDef_h_
#define _ZFI_ZFCoreSPrintfDef_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreUtilCharTypeWrapperDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT void _ZFP_zfstringAppendV(ZF_OUT ZFCoreStringA &s, ZF_IN const zfcharA *fmt, va_list vaList);
inline void _ZFP_zfstringAppendV(ZF_OUT_OPT ZFCoreStringA *s, ZF_IN const zfcharA *fmt, va_list vaList)
{
    if(s)
    {
        _ZFP_zfstringAppendV(*s, fmt, vaList);
    }
}
extern ZF_ENV_EXPORT void _ZFP_zfstringAppendV(ZF_OUT ZFCoreStringW &s, ZF_IN const zfcharW *fmt, va_list vaList);
inline void _ZFP_zfstringAppendV(ZF_OUT_OPT ZFCoreStringW *s, ZF_IN const zfcharW *fmt, va_list vaList)
{
    if(s)
    {
        _ZFP_zfstringAppendV(*s, fmt, vaList);
    }
}

template<typename T_Char, typename T_Str>
void _ZFP_zfstringAppend(ZF_OUT T_Str &s,
                         ZF_IN const T_Char *fmt,
                         ...)
{
    va_list vaList;
    va_start(vaList, fmt);
    _ZFP_zfstringAppendV(s, fmt, vaList);
    va_end(vaList);
}

/**
 * @brief a light-weight sprintf-like string formatter
 *
 * s to append could be any types that support:
 * -  operator+=(CharType)
 * -  operator+=(const CharType *)
 *
 * supported format tokens:
 * -  "b": zfbool (e.g. true)
 * -  "zi": zfindex (e.g. 13579)
 * -  "d" or "i": zfint (e.g. -13579)
 * -  "u": zfuint (e.g. 13579)
 * -  "o": zfuint, in octal (e.g. 01234567)
 * -  "x": zfuint, in heximal, lowercase (e.g. 0x1234abcd)
 * -  "X": zfuint, in heximal, uppercase (e.g. 0x1234ABCD)
 * -  "f": zffloat, accuracy is not ensured (e.g. 13.579)
 * -  "lf": zfdouble, accuracy is not ensured (e.g. 13.579001)
 * -  "llf" or "Lf": zflongdouble, accuracy is not ensured (e.g. 13.579000001)
 * -  "p": pointer value (e.g. 0x1234ABCD)
 * -  "c" or "C": zfchar/zfcharA/zfcharW depends on fmt's type
 * -  "s" or "S": zfchar/zfcharA/zfcharW string depends on fmt's type
 * -  "%": % itself
 *
 * extra format tokens can be added before any of those listed above (except "%"),
 * similar to printf, they are:
 * -  "-": left justify
 * -  "0": leading zero fill
 * -  "+": print plus if positive number
 * -  " ": print space if positive number
 * -  "m": minimum width
 * -  "*": same as "m" but scaned from vaList as a zfindex
 * -  ".n": decimal width for float types or max width for string types
 * -  ".*": same as ".n" but scaned from vaList as a zfindex
 *
 * note:
 * -  for special types such as zfint8 or zfint64 or zftimet,
 *   you should use zfsFromInt series to make sure type size
 *   doesn't affect the print result
 * -  zfindex may or may not have the same size with zfint/zfuint,
 *   you must take care of it when printing zfuint and zfindex types,
 *   for example, this may cause trouble:
 *   @code
 *     zfindex n = 1;
 *     const zfchar *s = zfText("1");
 *     zfstring str;
 *     // since zfuint is usually 32 bit and zfindex may be 64 bit,
 *     // when scanning %u from a zfindex,
 *     // the left 32 bit would be passed and scaned as %s,
 *     // the app may crash at runtime
 *     zfstringAppend(str, zfText("%u %s"), n, s);
 *   @endcode
 */
#define zfstringAppend(s, fmt, ...) \
    _ZFP_zfstringAppend(s, fmt, ##__VA_ARGS__)
/** @brief see #zfstringAppend */
#define zfstringAppendV(s, fmt, vaList) \
    _ZFP_zfstringAppendV(s, fmt, vaList)

/**
 * @brief util to create a zfstring from format
 */
template<typename T_Char>
typename zfwrapCharToString<T_Char>::StringType zfstringWithFormat(ZF_IN const T_Char *fmt, ...)
{
    typename zfwrapCharToString<T_Char>::StringType ret;
    va_list vaList;
    va_start(vaList, fmt);
    zfstringAppendV(ret, fmt, vaList);
    va_end(vaList);
    return ret;
}
/**
 * @brief util to create a zfstring from format
 */
template<typename T_Char>
typename zfwrapCharToString<T_Char>::StringType zfstringWithFormatV(ZF_IN const T_Char *fmt, ZF_IN va_list vaList)
{
    typename zfwrapCharToString<T_Char>::StringType ret;
    zfstringAppendV(ret, fmt, vaList);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreSPrintfDef_h_

