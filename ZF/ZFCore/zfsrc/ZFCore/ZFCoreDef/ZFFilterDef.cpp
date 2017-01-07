/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFFilterDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
void ZFFilterTypeToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFFilterType const &value)
{
    switch(value)
    {
        case ZFFilterTypeInclude:
            ret += ZFTOKEN_ZFFilterTypeInclude;
            return ;
        case ZFFilterTypeExclude:
            ret += ZFTOKEN_ZFFilterTypeExclude;
            return ;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
const zfchar *ZFFilterTypeFromString(ZF_OUT ZFFilterType &ret,
                                     ZF_IN const zfchar *src,
                                     ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFFilterTypeInclude,
        ZFTOKEN_ZFFilterTypeExclude,
    };
    zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
    ret = ZFFilterTypeInclude;
    switch(matched)
    {
        case 0:
            ret = ZFFilterTypeInclude;
            return zfnull;
        case 1:
            ret = ZFFilterTypeExclude;
            return zfnull;
        default:
            return src;
    }
}

// ============================================================
void ZFFilterCallbackResultToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFFilterCallbackResult const &value)
{
    switch(value)
    {
        case ZFFilterCallbackResultNotSpecified:
            ret += ZFTOKEN_ZFFilterCallbackResultNotSpecified;
            return ;
        case ZFFilterCallbackResultActive:
            ret += ZFTOKEN_ZFFilterCallbackResultActive;
            return ;
        case ZFFilterCallbackResultNotActive:
            ret += ZFTOKEN_ZFFilterCallbackResultNotActive;
            return ;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
const zfchar *ZFFilterCallbackResultFromString(ZF_OUT ZFFilterCallbackResult &ret,
                                               ZF_IN const zfchar *src,
                                               ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFFilterCallbackResultNotSpecified,
        ZFTOKEN_ZFFilterCallbackResultActive,
        ZFTOKEN_ZFFilterCallbackResultNotActive,
    };
    zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
    ret = ZFFilterCallbackResultNotSpecified;
    switch(matched)
    {
        case 0:
            ret = ZFFilterCallbackResultNotSpecified;
            return zfnull;
        case 1:
            ret = ZFFilterCallbackResultActive;
            return zfnull;
        case 2:
            ret = ZFFilterCallbackResultNotActive;
            return zfnull;
        default:
            return src;
    }
}

ZF_NAMESPACE_GLOBAL_END

