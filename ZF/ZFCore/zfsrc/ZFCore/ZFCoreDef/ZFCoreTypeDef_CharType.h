/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreTypeDef_CharType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_CharType_h_
#define _ZFI_ZFCoreTypeDef_CharType_h_

#include "ZFCoreTypeDef_CoreType.h"
#include <wchar.h>
#include <string.h>
#include <stdlib.h>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfcharA types
/** @brief char wrapper */
ZFT_INT_WEAK(_ZFT_zfcharA, zfcharA);
/** @brief char string wrapper */
#define zfTextA(s) s

// ============================================================
// zfcharW types
/** @brief wchar_t wrapper */
ZFT_INT_WEAK(_ZFT_zfcharW, zfcharW);
/** @brief wchar_t string wrapper */
#define zfTextW(s) L##s

// ============================================================
#if 1 // you must change these if change zfchar's type

/**
 * @brief true if zfchar is same as zfcharA (ignore encoding)
 */
#define ZFCHAR_TYPE_SAME_AS_CHAR_A 1
/**
 * @brief true if zfchar is same as zfcharW (ignore encoding)
 */
#define ZFCHAR_TYPE_SAME_AS_CHAR_W 0

/**
 * @brief char wrapper
 *
 * remarks:
 * if you want to change the type of zfchar,
 * you must also change:
 * -  zfchar, zfText, etc
 * -  zfsCoreZ2A
 */
ZFT_INT_WEAK(zft_zfcharA, zfchar);
/** @brief char string wrapper */
#define zfText(s) zfTextA(s)

/**
 * @brief get logical char's size,
 *   see #zfcharMoveNext
 */
extern ZF_ENV_EXPORT zfindex zfcharGetSize(const zfcharA *p);
/**
 * @brief get logical char's size,
 *   see #zfcharMoveNext
 */
inline zfindex zfcharGetSize(const zfcharW *p)
{
    return 1;
}

/**
 * @brief move to next logical char
 *
 * since zfchar's encoding may take more than one char unit
 * to store a logical char, you can't simply use "++p"\n
 * this macro take one param,
 * which should be a pointer var of (zfchar *)
 * or (const zfchar *),
 * and this macro would move the pointer to next logical char
 */
template<typename T_CharString>
inline void zfcharMoveNext(T_CharString &p)
{
    p += zfcharGetSize(p);
}

/**
 * @brief append logical char from p to s,
 *   then move p to next char,
 *   see #zfcharMoveNext
 */
template<typename T_Str, typename T_CharString>
void zfcharAppendAndMoveNext(T_Str &s, T_CharString &p)
{
    switch(zfcharGetSize(p))
    {
        case 2:
            s.append(p, 2);
            p += 2;
            break;
        case 3:
            s.append(p, 3);
            p += 3;
            break;
        case 4:
            s.append(p, 4);
            p += 4;
            break;
        case 5:
            s.append(p, 5);
            p += 5;
            break;
        case 6:
            s.append(p, 6);
            p += 6;
            break;
        case 7:
            s.append(p, 7);
            p += 7;
            break;
        case 8:
            s.append(p, 8);
            p += 8;
            break;
        case 1:
        default:
            s.append(p, 1);
            p += 1;
            break;
    }
}

/**
 * @brief true if 'a' ~ 'z'
 */
#define zfcharIsLetterLowerCase(c) \
    ((c) >= 'a' && (c) <= 'z')
/**
 * @brief true if 'A' ~ 'Z'
 */
#define zfcharIsLetterUpperCase(c) \
    ((c) >= 'A' && (c) <= 'Z')
/**
 * @brief true if '0' ~ '9'
 */
#define zfcharIsNumber(c) \
    ((c) >= '0' && (c) <= '9')
/**
 * @brief true if '0' ~ '9' or 'a' ~ 'f'
 */
#define zfcharIsHexNumberLowerCase(c) \
    (zfcharIsNumber(c) || ((c) >= 'a' && (c) <= 'f'))
/**
 * @brief true if '0' ~ '9' or 'A' ~ 'F'
 */
#define zfcharIsHexNumberUpperCase(c) \
    (zfcharIsNumber(c) || ((c) >= 'A' && (c) <= 'F'))
/**
 * @brief true if '0' ~ '9' or 'a' ~ 'f' or 'A' ~ 'F'
 */
#define zfcharIsHexNumber(c) \
    (zfcharIsNumber(c) || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))
/**
 * @brief true if char is space or tab
 */
#define zfcharIsSpace(c) \
    ((c) == ' ' || (c) == '\t')
/**
 * @brief true if char is '\\r' or '\\n'
 */
#define zfcharIsEndl(c) \
    ((c) == '\r' || (c) == '\n')
/**
 * @brief move p to skip spaces and tabs, up to end
 */
#define zfcharSkipSpace(p, end) \
    while(p < end && zfcharIsSpace(*p)) ++p
/**
 * @brief move p to skip spaces and tabs, up to end
 */
#define zfcharSkipEndl(p, end) \
    while(p < end && zfcharIsEndl(*p)) ++p
/**
 * @brief move p to skip spaces and tabs, up to end
 */
#define zfcharSkipSpaceAndEndl(p, end) \
    while(p < end && (zfcharIsSpace(*p) || zfcharIsEndl(*p))) ++p
#endif // you must change these if change zfchar's type
// ============================================================

// ============================================================
// zfcharA types
/** @brief strlen wrapper as zfcharA type */
#define zfslenA(s) _ZFP_zfslenA(s)
/** @brief strcpy wrapper as zfcharA type */
#define zfscpyA(dst, src) _ZFP_zfscpyA(dst, src)
/** @brief strcmp wrapper as zfcharA type */
#define zfscmpA(s1, s2) _ZFP_zfscmpA(s1, s2)
/** @brief stricmp wrapper as zfcharA type, no locale supported */
#define zfsicmpA(s1, s2) _ZFP_zfsicmpA(s1, s2)
/** @brief strncmp wrapper as zfcharA type */
#define zfsncmpA(s1, s2, count) _ZFP_zfsncmpA(s1, s2, count)
/** @brief strnicmp wrapper as zfcharA type, no locale supported */
#define zfsnicmpA(s1, s2, count) _ZFP_zfsnicmpA(s1, s2, count)

// ============================================================
// zfcharW types
/** @brief wcslen wrapper as zfcharW type */
#define zfslenW(s) _ZFP_zfslenW(s)
/** @brief wcscpy wrapper as zfcharW type */
#define zfscpyW(dst, src) _ZFP_zfscpyW(dst, src)
/** @brief wcscmp wrapper as zfcharW type */
#define zfscmpW(s1, s2) _ZFP_zfscmpW(s1, s2)
/** @brief wcsicmp wrapper as zfcharW type, no locale supported */
#define zfsicmpW(s1, s2) _ZFP_zfsicmpW(s1, s2)
/** @brief wcsncmp wrapper as zfcharW type */
#define zfsncmpW(s1, s2, count) _ZFP_zfsncmpW(s1, s2, count)
/** @brief wcsnicmp wrapper as zfcharW type, no locale supported */
#define zfsnicmpW(s1, s2, count) _ZFP_zfsnicmpW(s1, s2, count)

// ============================================================
#if 1 // you must change these if change zfchar's type

/** @brief strlen wrapper */
#define zfslen(s) zfslenA(s)
/** @brief strcpy wrapper */
#define zfscpy(dst, src) zfscpyA(dst, src)
/** @brief strcmp wrapper */
#define zfscmp(s1, s2) zfscmpA(s1, s2)
/** @brief stricmp wrapper, no locale supported */
#define zfsicmp(s1, s2) zfsicmpA(s1, s2)
/** @brief strncmp wrapper */
#define zfsncmp(s1, s2, count) zfsncmpA(s1, s2, count)
/** @brief strnicmp wrapper, no locale supported */
#define zfsnicmp(s1, s2, count) zfsnicmpA(s1, s2, count)

#endif // you must change these if change zfchar's type
// ============================================================

// ============================================================
inline zfindex _ZFP_zfslenA(const zfcharA *s)
{
    return (zfindex)::strlen(s);
}
inline zfcharA *_ZFP_zfscpyA(zfcharA *dst, const zfcharA *src)
{
    return ::strcpy(dst, src);
}
inline zfint _ZFP_zfscmpA(const zfcharA *s1, const zfcharA *s2)
{
    return (zfint)::strcmp(s1, s2);
}
extern ZF_ENV_EXPORT zfint _ZFP_zfsicmpA(const zfcharA *s1, const zfcharA *s2);
inline zfint _ZFP_zfsncmpA(const zfcharA *s1, const zfcharA *s2, zfindex count)
{
    return ((count == zfindexMax) ? ::strcmp(s1, s2) : ::strncmp(s1, s2, (size_t)count));
}
extern ZF_ENV_EXPORT zfint _ZFP_zfsnicmpA(const zfcharA *s1, const zfcharA *s2, zfindex count);

extern ZF_ENV_EXPORT zfindex _ZFP_zfslenW(const zfcharW *s);
extern ZF_ENV_EXPORT zfcharW *_ZFP_zfscpyW(zfcharW *dst, const zfcharW *src);
extern ZF_ENV_EXPORT zfint _ZFP_zfscmpW(const zfcharW *s1, const zfcharW *s2);
extern ZF_ENV_EXPORT zfint _ZFP_zfsicmpW(const zfcharW *s1, const zfcharW *s2);
extern ZF_ENV_EXPORT zfint _ZFP_zfsncmpW(const zfcharW *s1, const zfcharW *s2, zfindex count);
extern ZF_ENV_EXPORT zfint _ZFP_zfsnicmpW(const zfcharW *s1, const zfcharW *s2, zfindex count);

/** @cond ZFPrivateDoc */
template<typename T_Char>
zfindex zfslenT(const T_Char *s)
{
    const T_Char *p = s;
    while(*p) {++p;}
    return (p - s);
}
inline zfindex zfslenT(const zfcharA *s)
{
    return zfslenA(s);
}
inline zfindex zfslenT(const zfcharW *s)
{
    return zfslenW(s);
}

template<typename T_Char>
T_Char *zfscpyT(T_Char *dst, const T_Char *src)
{
    T_Char *saved = dst;
    while(*src) {*dst++ = *src++;}
    return saved;
}
inline zfcharA *zfscpyT(zfcharA *dst, const zfcharA *src)
{
    return zfscpyA(dst, src);
}
inline zfcharW *zfscpyT(zfcharW *dst, const zfcharW *src)
{
    return zfscpyW(dst, src);
}

template<typename T_Char>
zfint zfscmpT(const T_Char *s1, const T_Char *s2)
{
    while(*s1 && *s2 && (*s1 == *s2))
    {
        ++s1;
        ++s2;
    }
    return (((zfint)(*s1)) - ((zfint)(*s2)));
}
inline zfint zfscmpT(const zfcharA *s1, const zfcharA *s2)
{
    return zfscmpA(s1, s2);
}
inline zfint zfscmpT(const zfcharW *s1, const zfcharW *s2)
{
    return zfscmpW(s1, s2);
}

template<typename T_Char>
zfint zfsicmpT(const T_Char *s1, const T_Char *s2)
{
    while(*s1 && *s2 && (
        (*s1 == *s2)
        || (*s1 >= 'A' && *s1 <= 'Z' && *s1 + 32 == *s2)
        || (*s1 >= 'a' && *s1 <= 'a' && *s1 - 32 == *s2)))
    {
        ++s1;
        ++s2;
    }
    return (((zfint)(*s1)) - ((zfint)(*s2)));
}
inline zfint zfsicmpT(const zfcharA *s1, const zfcharA *s2)
{
    return zfsicmpA(s1, s2);
}
inline zfint zfsicmpT(const zfcharW *s1, const zfcharW *s2)
{
    return zfsicmpW(s1, s2);
}

template<typename T_Char>
zfint zfsncmpT(const T_Char *s1, const T_Char *s2, zfindex count)
{
    if(count == 0)
    {
        return 0;
    }
    while(--count && *s1 && *s2 && (*s1 == *s2))
    {
        ++s1;
        ++s2;
    }
    return (((zfint)(*s1)) - ((zfint)(*s2)));
}
inline zfint zfsncmpT(const zfcharA *s1, const zfcharA *s2, zfindex count)
{
    return zfsncmpA(s1, s2, count);
}
inline zfint zfsncmpT(const zfcharW *s1, const zfcharW *s2, zfindex count)
{
    return zfsncmpW(s1, s2, count);
}

template<typename T_Char>
zfint zfsnicmpT(const T_Char *s1, const T_Char *s2, zfindex count)
{
    if(count == 0)
    {
        return 0;
    }
    while(--count && *s1 && *s2 && (
        (*s1 == *s2)
        || (*s1 >= 'A' && *s1 <= 'Z' && *s1 + 32 == *s2)
        || (*s1 >= 'a' && *s1 <= 'a' && *s1 - 32 == *s2)))
    {
        ++s1;
        ++s2;
    }
    return (((zfint)(*s1)) - ((zfint)(*s2)));
}
inline zfint zfsnicmpT(const zfcharA *s1, const zfcharA *s2, zfindex count)
{
    return zfsnicmpA(s1, s2, count);
}
inline zfint zfsnicmpT(const zfcharW *s1, const zfcharW *s2, zfindex count)
{
    return zfsnicmpW(s1, s2, count);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_CharType_h_

