/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreTypeDef_CharType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfindex zfcharGetSize(const zfcharA *p)
{
    if((zfbyte)*p <= 0x7F)
    {
        return 1;
    }
    else if((zfbyte)*p <= 0xDF && (zfbyte)p[1] <= 0xBF)
    {
        return 2;
    }
    else if((zfbyte)*p <= 0xEF && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF)
    {
        return 3;
    }
    else if((zfbyte)*p <= 0xF7 && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF)
    {
        return 4;
    }
    else if((zfbyte)*p <= 0xFB && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF)
    {
        return 5;
    }
    else if((zfbyte)*p <= 0xFD && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF && (zfbyte)p[5] <= 0xBF)
    {
        return 6;
    }
    else if((zfbyte)*p <= 0xFE && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF && (zfbyte)p[5] <= 0xBF && (zfbyte)p[6] <= 0xBF)
    {
        return 7;
    }
    else if((zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF && (zfbyte)p[5] <= 0xBF && (zfbyte)p[6] <= 0xBF && (zfbyte)p[7] <= 0xBF)
    {
        return 8;
    }
    else
    {
        return 1;
    }
}

// ============================================================
template<typename T_Char>
zfint _ZFP_zfsicmpTemplate(const T_Char *s1, const T_Char *s2)
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
template<typename T_Char>
zfint _ZFP_zfsnicmpTemplate(const T_Char *s1, const T_Char *s2, zfindex count)
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

zfint _ZFP_zfsicmpA(const zfcharA *s1, const zfcharA *s2)
{
    return _ZFP_zfsicmpTemplate(s1, s2);
}
zfint _ZFP_zfsnicmpA(const zfcharA *s1, const zfcharA *s2, zfindex count)
{
    return _ZFP_zfsnicmpTemplate(s1, s2, count);
}

ZF_ENV_SENSITIVE
#if (defined(ANDROID) || defined(__ANDROID__))
    zfindex _ZFP_zfslenW(const zfcharW *s)
    {
        const zfcharW *p = s;
        while(*p)
        {
            ++p;
        }
        return (p - s);
    }
    zfcharW *_ZFP_zfscpyW(zfcharW *dst, const zfcharW *src)
    {
        zfcharW *ret = dst;
        while((*dst++ = *src++) != 0) {}
        return ret;
    }
    zfint _ZFP_zfscmpW(const zfcharW *s1, const zfcharW *s2)
    {
        while(*s1 && *s2 && (*s1 == *s2))
        {
            ++s1;
            ++s2;
        }
        return (zfint)((zfuint)(*s1) - (zfuint)(*s2));
    }
    zfint _ZFP_zfsicmpW(const zfcharW *s1, const zfcharW *s2)
    {
        return _ZFP_zfsicmpTemplate(s1, s2);
    }
    zfint _ZFP_zfsncmpW(const zfcharW *s1, const zfcharW *s2, zfindex count)
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
        return (zfint)((zfuint)(*s1) - (zfuint)(*s2));
    }
    zfint _ZFP_zfsnicmpW(const zfcharW *s1, const zfcharW *s2, zfindex count)
    {
        return _ZFP_zfsnicmpTemplate(s1, s2, count);
    }
#else
    zfindex _ZFP_zfslenW(const zfcharW *s)
    {
        return (zfindex)::wcslen(s);
    }
    zfcharW *_ZFP_zfscpyW(zfcharW *dst, const zfcharW *src)
    {
        return ::wcscpy(dst, src);
    }
    zfint _ZFP_zfscmpW(const zfcharW *s1, const zfcharW *s2)
    {
        return ::wcscmp(s1, s2);
    }
    zfint _ZFP_zfsicmpW(const zfcharW *s1, const zfcharW *s2)
    {
        return _ZFP_zfsicmpTemplate(s1, s2);
    }
    zfint _ZFP_zfsncmpW(const zfcharW *s1, const zfcharW *s2, zfindex count)
    {
        return ((count == zfindexMax) ? ::wcscmp(s1, s2) : ::wcsncmp(s1, s2, (size_t)count));
    }
    zfint _ZFP_zfsnicmpW(const zfcharW *s1, const zfcharW *s2, zfindex count)
    {
        return _ZFP_zfsnicmpTemplate(s1, s2, count);
    }
#endif // #if (defined(ANDROID) || defined(__ANDROID__))

ZF_NAMESPACE_GLOBAL_END

