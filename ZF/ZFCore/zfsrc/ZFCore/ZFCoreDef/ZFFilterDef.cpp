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
ZFCORETYPE_STRING_CONVERTER_DEFINE(ZFFilterType, ZFFilterType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFFilterTypeInclude,
            ZFTOKEN_ZFFilterTypeExclude,
        });
        zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFFilterTypeInclude;
        switch(matched)
        {
            case 0:
                v = ZFFilterTypeInclude;
                return zfnull;
            case 1:
                v = ZFFilterTypeExclude;
                return zfnull;
            default:
                return src;
        }
    }, {
        switch(v)
        {
            case ZFFilterTypeInclude:
                s += ZFTOKEN_ZFFilterTypeInclude;
                return ;
            case ZFFilterTypeExclude:
                s += ZFTOKEN_ZFFilterTypeExclude;
                return ;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    })

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DEFINE(ZFFilterCallbackResult, ZFFilterCallbackResult, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFFilterCallbackResultNotSpecified,
            ZFTOKEN_ZFFilterCallbackResultActive,
            ZFTOKEN_ZFFilterCallbackResultNotActive,
        });
        zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFFilterCallbackResultNotSpecified;
        switch(matched)
        {
            case 0:
                v = ZFFilterCallbackResultNotSpecified;
                return zfnull;
            case 1:
                v = ZFFilterCallbackResultActive;
                return zfnull;
            case 2:
                v = ZFFilterCallbackResultNotActive;
                return zfnull;
            default:
                return src;
        }
    }, {
        switch(v)
        {
            case ZFFilterCallbackResultNotSpecified:
                s += ZFTOKEN_ZFFilterCallbackResultNotSpecified;
                return ;
            case ZFFilterCallbackResultActive:
                s += ZFTOKEN_ZFFilterCallbackResultActive;
                return ;
            case ZFFilterCallbackResultNotActive:
                s += ZFTOKEN_ZFFilterCallbackResultNotActive;
                return ;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    })

ZF_NAMESPACE_GLOBAL_END

