/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreLevelDef.h"
#include "ZFCoreUtilMacroDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFCORETYPE_STRING_CONVERTER_DEFINE(ZFLevel, ZFLevel, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFLevelZFFrameworkStatic,
            ZFTOKEN_ZFLevelZFFrameworkEssential,
            ZFTOKEN_ZFLevelZFFrameworkHigh,
            ZFTOKEN_ZFLevelZFFrameworkNormal,
            ZFTOKEN_ZFLevelZFFrameworkLow,

            ZFTOKEN_ZFLevelAppEssential,
            ZFTOKEN_ZFLevelAppHigh,
            ZFTOKEN_ZFLevelAppNormal,
            ZFTOKEN_ZFLevelAppLow,

            ZFTOKEN_ZFLevelZFFrameworkPostLow,
            ZFTOKEN_ZFLevelZFFrameworkPostNormal,
            ZFTOKEN_ZFLevelZFFrameworkPostHigh,
            ZFTOKEN_ZFLevelZFFrameworkPostEssential,
            ZFTOKEN_ZFLevelZFFrameworkPostStatic,
        });
        zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFLevelAppLow;
        switch(matched)
        {
            case 0:
                v = ZFLevelZFFrameworkStatic;
                return zfnull;
            case 1:
                v = ZFLevelZFFrameworkEssential;
                return zfnull;
            case 2:
                v = ZFLevelZFFrameworkHigh;
                return zfnull;
            case 3:
                v = ZFLevelZFFrameworkNormal;
                return zfnull;
            case 4:
                v = ZFLevelZFFrameworkLow;
                return zfnull;

            case 5:
                v = ZFLevelAppEssential;
                return zfnull;
            case 6:
                v = ZFLevelAppHigh;
                return zfnull;
            case 7:
                v = ZFLevelAppNormal;
                return zfnull;
            case 8:
                v = ZFLevelAppLow;
                return zfnull;

            case 9:
                v = ZFLevelZFFrameworkPostLow;
                return zfnull;
            case 10:
                v = ZFLevelZFFrameworkPostNormal;
                return zfnull;
            case 11:
                v = ZFLevelZFFrameworkPostHigh;
                return zfnull;
            case 12:
                v = ZFLevelZFFrameworkPostEssential;
                return zfnull;
            case 13:
                v = ZFLevelZFFrameworkPostStatic;
                return zfnull;

            default:
                return src;
        }
    }, {
        switch(v)
        {
            case ZFLevelZFFrameworkStatic:
                s += ZFTOKEN_ZFLevelZFFrameworkStatic;
                return ;
            case ZFLevelZFFrameworkEssential:
                s += ZFTOKEN_ZFLevelZFFrameworkEssential;
                return ;
            case ZFLevelZFFrameworkHigh:
                s += ZFTOKEN_ZFLevelZFFrameworkHigh;
                return ;
            case ZFLevelZFFrameworkNormal:
                s += ZFTOKEN_ZFLevelZFFrameworkNormal;
                return ;
            case ZFLevelZFFrameworkLow:
                s += ZFTOKEN_ZFLevelZFFrameworkLow;
                return ;

            case ZFLevelAppEssential:
                s += ZFTOKEN_ZFLevelAppEssential;
                return ;
            case ZFLevelAppHigh:
                s += ZFTOKEN_ZFLevelAppHigh;
                return ;
            case ZFLevelAppNormal:
                s += ZFTOKEN_ZFLevelAppNormal;
                return ;
            case ZFLevelAppLow:
                s += ZFTOKEN_ZFLevelAppLow;
                return ;

            case ZFLevelZFFrameworkPostLow:
                s += ZFTOKEN_ZFLevelZFFrameworkPostLow;
                return ;
            case ZFLevelZFFrameworkPostNormal:
                s += ZFTOKEN_ZFLevelZFFrameworkPostNormal;
                return ;
            case ZFLevelZFFrameworkPostHigh:
                s += ZFTOKEN_ZFLevelZFFrameworkPostHigh;
                return ;
            case ZFLevelZFFrameworkPostEssential:
                s += ZFTOKEN_ZFLevelZFFrameworkPostEssential;
                return ;
            case ZFLevelZFFrameworkPostStatic:
                s += ZFTOKEN_ZFLevelZFFrameworkPostStatic;
                return ;

            default:
                return ;
        }
    })

ZF_NAMESPACE_GLOBAL_END

