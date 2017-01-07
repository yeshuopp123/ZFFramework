/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreStringUtilDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

template<typename T_Char>
zfindex _ZFP_zfstringFind(ZF_IN const T_Char *src, ZF_IN zfindex srcLen, ZF_IN const T_Char *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax)
        {
            srcLen = zfslenT(src);
        }
        if(findLen == zfindexMax)
        {
            findLen = zfslenT(find);
        }
        if(srcLen >= findLen)
        {
            const T_Char *p = src;
            const T_Char *pEnd = src + srcLen - findLen;
            do
            {
                if(zfsncmpT(p, find, findLen) == 0)
                {
                    return (p - src);
                }
                ++p;
            } while(p <= pEnd);
        }
    }
    return zfindexMax;
}
template<typename T_Char>
zfindex _ZFP_zfstringFindReversely(ZF_IN const T_Char *src, ZF_IN zfindex srcLen, ZF_IN const T_Char *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax)
        {
            srcLen = zfslenT(src);
        }
        if(findLen == zfindexMax)
        {
            findLen = zfslenT(find);
        }
        if(srcLen >= findLen)
        {
            const T_Char *p = src + srcLen - findLen;
            const T_Char *pEnd = src;
            do
            {
                if(zfsncmpT(p, find, findLen) == 0)
                {
                    return (p - src);
                }
                --p;
            } while(p >= pEnd);
        }
    }
    return zfindexMax;
}
template<typename T_Char>
zfindex _ZFP_zfstringFindCaseInsensitive(ZF_IN const T_Char *src, ZF_IN zfindex srcLen, ZF_IN const T_Char *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax)
        {
            srcLen = zfslenT(src);
        }
        if(findLen == zfindexMax)
        {
            findLen = zfslenT(find);
        }
        if(srcLen >= findLen)
        {
            const T_Char *p = src;
            const T_Char *pEnd = src + srcLen - findLen;
            do
            {
                if(zfsnicmpT(p, find, findLen) == 0)
                {
                    return (p - src);
                }
                ++p;
            } while(p <= pEnd);
        }
    }
    return zfindexMax;
}
template<typename T_Char>
zfindex _ZFP_zfstringFindCaseInsensitiveReversely(ZF_IN const T_Char *src, ZF_IN zfindex srcLen, ZF_IN const T_Char *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax)
        {
            srcLen = zfslenT(src);
        }
        if(findLen == zfindexMax)
        {
            findLen = zfslenT(find);
        }
        if(srcLen >= findLen)
        {
            const T_Char *p = src + srcLen - findLen;
            const T_Char *pEnd = src;
            do
            {
                if(zfsnicmpT(p, find, findLen) == 0)
                {
                    return (p - src);
                }
                --p;
            } while(p >= pEnd);
        }
    }
    return zfindexMax;
}
template<typename T_Char>
zfindex _ZFP_zfstringFindFirstOf(ZF_IN const T_Char *src, ZF_IN zfindex srcLen, ZF_IN const T_Char *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax)
        {
            srcLen = zfslenT(src);
        }
        if(findLen == zfindexMax)
        {
            findLen = zfslenT(find);
        }
        const T_Char *p = src;
        const T_Char *pEnd = src + srcLen;
        while(p < pEnd)
        {
            for(zfindex i = 0; i < findLen; ++i)
            {
                if(*p == find[i])
                {
                    return (p - src);
                }
            }
            ++p;
        }
    }
    return zfindexMax;
}
template<typename T_Char>
zfindex _ZFP_zfstringFindFirstNotOf(ZF_IN const T_Char *src, ZF_IN zfindex srcLen, ZF_IN const T_Char *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax)
        {
            srcLen = zfslenT(src);
        }
        if(findLen == zfindexMax)
        {
            findLen = zfslenT(find);
        }
        const T_Char *p = src;
        const T_Char *pEnd = src + srcLen;
        while(p < pEnd)
        {
            zfindex i = 0;
            for( ; i < findLen; ++i)
            {
                if(*p == find[i])
                {
                    break;
                }
            }
            if(i >= findLen)
            {
                return (p - src);
            }
            ++p;
        }
    }
    return zfindexMax;
}
template<typename T_Char>
zfindex _ZFP_zfstringFindLastOf(ZF_IN const T_Char *src, ZF_IN zfindex srcLen, ZF_IN const T_Char *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax)
        {
            srcLen = zfslenT(src);
        }
        if(findLen == zfindexMax)
        {
            findLen = zfslenT(find);
        }
        const T_Char *p = src + srcLen - 1;
        const T_Char *pEnd = src - 1;
        while(p > pEnd)
        {
            for(zfindex i = 0; i < findLen; ++i)
            {
                if(*p == find[i])
                {
                    return (p - src);
                }
            }
            --p;
        }
    }
    return zfindexMax;
}
template<typename T_Char>
zfindex _ZFP_zfstringFindLastNotOf(ZF_IN const T_Char *src, ZF_IN zfindex srcLen, ZF_IN const T_Char *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax)
        {
            srcLen = zfslenT(src);
        }
        if(findLen == zfindexMax)
        {
            findLen = zfslenT(find);
        }
        const T_Char *p = src + srcLen - 1;
        const T_Char *pEnd = src - 1;
        while(p > pEnd)
        {
            zfindex i = 0;
            for( ; i < findLen; ++i)
            {
                if(*p == find[i])
                {
                    break;
                }
            }
            if(i >= findLen)
            {
                return (p - src);
            }
            --p;
        }
    }
    return zfindexMax;
}

// ============================================================
zfindex zfstringFind(ZF_IN const zfcharA *src, ZF_IN zfindex srcLen, ZF_IN const zfcharA *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFind(src, srcLen, find, findLen);
}
zfindex zfstringFindReversely(ZF_IN const zfcharA *src, ZF_IN zfindex srcLen, ZF_IN const zfcharA *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindReversely(src, srcLen, find, findLen);
}
zfindex zfstringFindCaseInsensitive(ZF_IN const zfcharA *src, ZF_IN zfindex srcLen, ZF_IN const zfcharA *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindCaseInsensitive(src, srcLen, find, findLen);
}
zfindex zfstringFindCaseInsensitiveReversely(ZF_IN const zfcharA *src, ZF_IN zfindex srcLen, ZF_IN const zfcharA *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindCaseInsensitiveReversely(src, srcLen, find, findLen);
}
zfindex zfstringFindFirstOf(ZF_IN const zfcharA *src, ZF_IN zfindex srcLen, ZF_IN const zfcharA *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindFirstOf(src, srcLen, find, findLen);
}
zfindex zfstringFindFirstNotOf(ZF_IN const zfcharA *src, ZF_IN zfindex srcLen, ZF_IN const zfcharA *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindFirstNotOf(src, srcLen, find, findLen);
}
zfindex zfstringFindLastOf(ZF_IN const zfcharA *src, ZF_IN zfindex srcLen, ZF_IN const zfcharA *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindLastOf(src, srcLen, find, findLen);
}
zfindex zfstringFindLastNotOf(ZF_IN const zfcharA *src, ZF_IN zfindex srcLen, ZF_IN const zfcharA *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindLastNotOf(src, srcLen, find, findLen);
}

// ============================================================
zfindex zfstringFind(ZF_IN const zfcharW *src, ZF_IN zfindex srcLen, ZF_IN const zfcharW *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFind(src, srcLen, find, findLen);
}
zfindex zfstringFindReversely(ZF_IN const zfcharW *src, ZF_IN zfindex srcLen, ZF_IN const zfcharW *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindReversely(src, srcLen, find, findLen);
}
zfindex zfstringFindCaseInsensitive(ZF_IN const zfcharW *src, ZF_IN zfindex srcLen, ZF_IN const zfcharW *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindCaseInsensitive(src, srcLen, find, findLen);
}
zfindex zfstringFindCaseInsensitiveReversely(ZF_IN const zfcharW *src, ZF_IN zfindex srcLen, ZF_IN const zfcharW *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindCaseInsensitiveReversely(src, srcLen, find, findLen);
}
zfindex zfstringFindFirstOf(ZF_IN const zfcharW *src, ZF_IN zfindex srcLen, ZF_IN const zfcharW *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindFirstOf(src, srcLen, find, findLen);
}
zfindex zfstringFindFirstNotOf(ZF_IN const zfcharW *src, ZF_IN zfindex srcLen, ZF_IN const zfcharW *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindFirstNotOf(src, srcLen, find, findLen);
}
zfindex zfstringFindLastOf(ZF_IN const zfcharW *src, ZF_IN zfindex srcLen, ZF_IN const zfcharW *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindLastOf(src, srcLen, find, findLen);
}
zfindex zfstringFindLastNotOf(ZF_IN const zfcharW *src, ZF_IN zfindex srcLen, ZF_IN const zfcharW *find, ZF_IN zfindex findLen /* = zfindexMax */)
{
    return _ZFP_zfstringFindLastNotOf(src, srcLen, find, findLen);
}

ZF_NAMESPACE_GLOBAL_END

