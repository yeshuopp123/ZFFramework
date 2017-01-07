/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreUtilCharTypeWrapperDef.h
 * @brief char type wrapper between string and char types
 */
#ifndef _ZFI_ZFCoreUtilCharTypeWrapperDef_h_
#define _ZFI_ZFCoreUtilCharTypeWrapperDef_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief wrapper to convert string types to char types
 *
 * usage:
 * @code
 *   zfwrapStringToChar<zfstring>::CharType c; // zfchar type
 *   zfwrapStringToChar<zfstringA>::CharType cA; // zfcharA type
 *   zfwrapStringToChar<zfstringW>::CharType cW; // zfcharW type
 * @endcode
 */
template<typename T_Str>
struct ZF_ENV_EXPORT zfwrapStringToChar
{
public:
    /** @brief see #zfwrapStringToChar */
    typedef void CharType;
};
/**
 * @brief wrapper to convert char types to string types
 *
 * usage:
 * @code
 *   zfwrapCharToString<zfchar>::StringType s; // zfstring type
 *   zfwrapCharToString<zfcharA>::StringType sA; // zfstringA type
 *   zfwrapCharToString<zfcharW>::StringType sW; // zfstringW type
 * @endcode
 */
template<typename T_Char>
struct ZF_ENV_EXPORT zfwrapCharToString
{
public:
    /** @brief see #zfwrapCharToString */
    typedef void StringType;
};

// ============================================================
/** @cond ZFPrivateDoc */
template<>
struct ZF_ENV_EXPORT zfwrapStringToChar<zfstring>
{
public:
    typedef zfchar CharType;
};
template<>
struct ZF_ENV_EXPORT zfwrapCharToString<zfchar>
{
public:
    typedef zfstring StringType;
};
#if !ZFCHAR_TYPE_SAME_AS_CHAR_A
    template<>
    struct ZF_ENV_EXPORT zfwrapStringToChar<zfstringA>
    {
    public:
        typedef zfcharA CharType;
    };
    template<>
    struct ZF_ENV_EXPORT zfwrapCharToString<zfcharA>
    {
    public:
        typedef zfstringA StringType;
    };
#endif // #if !ZFCHAR_TYPE_SAME_AS_CHAR_A
#if !ZFCHAR_TYPE_SAME_AS_CHAR_W
    template<>
    struct ZF_ENV_EXPORT zfwrapStringToChar<zfstringW>
    {
    public:
        typedef zfcharW CharType;
    };
    template<>
    struct ZF_ENV_EXPORT zfwrapCharToString<zfcharW>
    {
    public:
        typedef zfstringW StringType;
    };
#endif // #if !ZFCHAR_TYPE_SAME_AS_CHAR_W
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreUtilCharTypeWrapperDef_h_

