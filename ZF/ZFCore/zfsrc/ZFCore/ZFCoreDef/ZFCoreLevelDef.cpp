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

void ZFLevelToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFLevel const &value)
{
    switch(value)
    {
        case ZFLevelZFFrameworkStatic:
            ret += ZFTOKEN_ZFLevelZFFrameworkStatic;
            return ;
        case ZFLevelZFFrameworkEssential:
            ret += ZFTOKEN_ZFLevelZFFrameworkEssential;
            return ;
        case ZFLevelZFFrameworkHigh:
            ret += ZFTOKEN_ZFLevelZFFrameworkHigh;
            return ;
        case ZFLevelZFFrameworkNormal:
            ret += ZFTOKEN_ZFLevelZFFrameworkNormal;
            return ;
        case ZFLevelZFFrameworkLow:
            ret += ZFTOKEN_ZFLevelZFFrameworkLow;
            return ;

        case ZFLevelAppEssential:
            ret += ZFTOKEN_ZFLevelAppEssential;
            return ;
        case ZFLevelAppHigh:
            ret += ZFTOKEN_ZFLevelAppHigh;
            return ;
        case ZFLevelAppNormal:
            ret += ZFTOKEN_ZFLevelAppNormal;
            return ;
        case ZFLevelAppLow:
            ret += ZFTOKEN_ZFLevelAppLow;
            return ;

        case ZFLevelZFFrameworkPostLow:
            ret += ZFTOKEN_ZFLevelZFFrameworkPostLow;
            return ;
        case ZFLevelZFFrameworkPostNormal:
            ret += ZFTOKEN_ZFLevelZFFrameworkPostNormal;
            return ;
        case ZFLevelZFFrameworkPostHigh:
            ret += ZFTOKEN_ZFLevelZFFrameworkPostHigh;
            return ;
        case ZFLevelZFFrameworkPostEssential:
            ret += ZFTOKEN_ZFLevelZFFrameworkPostEssential;
            return ;
        case ZFLevelZFFrameworkPostStatic:
            ret += ZFTOKEN_ZFLevelZFFrameworkPostStatic;
            return ;

        default:
            return ;
    }
}
const zfchar *ZFLevelFromString(ZF_OUT ZFLevel &ret,
                                ZF_IN const zfchar *src,
                                ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    const zfchar *tokens[] = {
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
    };
    zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
    ret = ZFLevelAppLow;
    switch(matched)
    {
        case 0:
            ret = ZFLevelZFFrameworkStatic;
            return zfnull;
        case 1:
            ret = ZFLevelZFFrameworkEssential;
            return zfnull;
        case 2:
            ret = ZFLevelZFFrameworkHigh;
            return zfnull;
        case 3:
            ret = ZFLevelZFFrameworkNormal;
            return zfnull;
        case 4:
            ret = ZFLevelZFFrameworkLow;
            return zfnull;

        case 5:
            ret = ZFLevelAppEssential;
            return zfnull;
        case 6:
            ret = ZFLevelAppHigh;
            return zfnull;
        case 7:
            ret = ZFLevelAppNormal;
            return zfnull;
        case 8:
            ret = ZFLevelAppLow;
            return zfnull;

        case 9:
            ret = ZFLevelZFFrameworkPostLow;
            return zfnull;
        case 10:
            ret = ZFLevelZFFrameworkPostNormal;
            return zfnull;
        case 11:
            ret = ZFLevelZFFrameworkPostHigh;
            return zfnull;
        case 12:
            ret = ZFLevelZFFrameworkPostEssential;
            return zfnull;
        case 13:
            ret = ZFLevelZFFrameworkPostStatic;
            return zfnull;

        default:
            return src;
    }
}

ZF_NAMESPACE_GLOBAL_END

