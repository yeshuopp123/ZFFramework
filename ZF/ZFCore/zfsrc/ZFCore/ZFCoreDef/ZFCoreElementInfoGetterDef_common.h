/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreElementInfoGetterDef_common.h
 * @brief commone template type util for element access
 */

#ifndef _ZFI_ZFCoreElementInfoGetterDef_common_h_
#define _ZFI_ZFCoreElementInfoGetterDef_common_h_

#include "ZFCoreElementInfoGetterDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @cond ZFPrivateDoc */
template<>
zfclassNotPOD ZF_ENV_EXPORT ZFCoreElementTokenGetter<zfchar>
{
public:
    static void elementTokenGetterLeft(ZF_IN_OUT zfstring &ret, ZF_IN zfchar const &v)
    {
        ret += '\'';
    }
    static void elementTokenGetterRight(ZF_IN_OUT zfstring &ret, ZF_IN zfchar const &v)
    {
        ret += '\'';
    }
};
template<>
zfclassNotPOD ZF_ENV_EXPORT ZFCoreElementTokenGetter<zfchar *>
{
public:
    static void elementTokenGetterLeft(ZF_IN_OUT zfstring &ret, ZF_IN zfchar * const &v)
    {
        if(v != zfnull)
        {
            ret += '\"';
        }
    }
    static void elementTokenGetterRight(ZF_IN_OUT zfstring &ret, ZF_IN zfchar * const &v)
    {
        if(v != zfnull)
        {
            ret += '\"';
        }
    }
};
template<>
zfclassNotPOD ZF_ENV_EXPORT ZFCoreElementTokenGetter<const zfchar *>
{
public:
    static void elementTokenGetterLeft(ZF_IN_OUT zfstring &ret, ZF_IN const zfchar * const &v)
    {
        if(v != zfnull)
        {
            ret += '\"';
        }
    }
    static void elementTokenGetterRight(ZF_IN_OUT zfstring &ret, ZF_IN const zfchar * const &v)
    {
        if(v != zfnull)
        {
            ret += '\"';
        }
    }
};
template<>
zfclassNotPOD ZF_ENV_EXPORT ZFCoreElementTokenGetter<zfstring>
{
public:
    static void elementTokenGetterLeft(ZF_IN_OUT zfstring &ret, ZF_IN zfstring const &v)
    {
        ret += '\"';
    }
    static void elementTokenGetterRight(ZF_IN_OUT zfstring &ret, ZF_IN zfstring const &v)
    {
        ret += '\"';
    }
};
template<>
zfclassNotPOD ZF_ENV_EXPORT ZFCoreElementTokenGetter<zfstring *>
{
public:
    static void elementTokenGetterLeft(ZF_IN_OUT zfstring &ret, ZF_IN zfstring * const &v)
    {
        if(v != zfnull)
        {
            ret += '\"';
        }
    }
    static void elementTokenGetterRight(ZF_IN_OUT zfstring &ret, ZF_IN zfstring * const &v)
    {
        if(v != zfnull)
        {
            ret += '\"';
        }
    }
};
template<>
zfclassNotPOD ZF_ENV_EXPORT ZFCoreElementTokenGetter<const zfstring *>
{
public:
    static void elementTokenGetterLeft(ZF_IN_OUT zfstring &ret, ZF_IN const zfstring * const &v)
    {
        if(v != zfnull)
        {
            ret += '\"';
        }
    }
    static void elementTokenGetterRight(ZF_IN_OUT zfstring &ret, ZF_IN const zfstring * const &v)
    {
        if(v != zfnull)
        {
            ret += '\"';
        }
    }
};
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreElementInfoGetterDef_common_h_

