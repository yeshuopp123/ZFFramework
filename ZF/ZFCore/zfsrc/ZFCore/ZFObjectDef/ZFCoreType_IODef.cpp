/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreType_IODef.h"
#include "ZFPropertyDef.h"
#include "ZFClassDef.h"
#include "ZFObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

template<typename T_Int>
zfbool _ZFP_zfintFromInput(ZF_OUT T_Int &v,
                           ZF_IN const ZFInputCallback &input,
                           ZF_IN_OPT zfindex radix = 10,
                           ZF_IN_OPT zfbool allowNegative = zftrue)
{
    v = 0;
    if(radix < 2 || radix > 36)
    {
        return zffalse;
    }

    zfchar p = '\0';
    zfbool negative = zffalse;

    p = ZFInputCallbackSkipChars(input);
    if(p == '-')
    {
        if(input.execute(&p, 1) != 1) {return zffalse;}
        if(p == '1')
        {
            if(input.execute(&p, 1) != 1
                || !(
                     (p >= '0' && p <= '9')
                     || (p >= 'a' && p <= 'z')
                     || (p >= 'A' && p <= 'Z')
                ))
            {
                v = ((T_Int)-1);
                return zftrue;
            }
            else
            {
                return zffalse;
            }
        }

        if(!allowNegative)
        {
            return zffalse;
        }
        negative = zftrue;
    }
    else if(p == '+')
    {
        p = ZFInputCallbackSkipChars(input);
    }

    do
    {
        zfuint tmp = 0;
        if(p >= '0' && p <= '9') {tmp = p - '0';}
        else if(p >= 'a' && p <= 'z') {tmp = 10 + p - 'a';}
        else if(p >= 'A' && p <= 'Z') {tmp = 10 + p - 'A';}
        else {input.ioSeek(1, ZFSeekPosCurReversely); break;}
        if(tmp >= radix) {input.ioSeek(1, ZFSeekPosCurReversely); break;}
        v = (T_Int)(v * radix + tmp);
        if(input.execute(&p, 1) != 1) {break;}
    } while(zftrue);
    if(negative)
    {
        v = (T_Int)0 - v;
    }
    return zftrue;
}

template<typename T_Float>
zfbool _ZFP_zffloatFromInput(ZF_OUT T_Float &v,
                             ZF_IN const ZFInputCallback &input)
{
    v = 0;
    zfchar p = '\0';
    zfbool negative = zffalse;

    p = ZFInputCallbackSkipChars(input);
    if(p == '-')
    {
        negative = zftrue;
        p = ZFInputCallbackSkipChars(input);
    }
    else if(p == '+')
    {
        p = ZFInputCallbackSkipChars(input);
    }

    do
    {
        zfuint tmp = 0;
        if(p >= '0' && p <= '9') {tmp = p - '0';}
        else {if(p != '.') {input.ioSeek(1, ZFSeekPosCurReversely);} break;}
        v = v * 10 + tmp;
        if(input.execute(&p, 1) != 1) {break;}
    } while(zftrue);
    if(p == '.' && input.execute(&p, 1) == 1)
    {
        unsigned long e = 10;
        do
        {
            if(!(p >= '0' && p <= '9')) {input.ioSeek(1, ZFSeekPosCurReversely); break;}
            v += (T_Float)(p - '0') / e;
            e *= 10;
            if(input.execute(&p, 1) != 1) {break;}
        } while(zftrue);
    }
    if(negative)
    {
        v = (T_Float)0 - v;
    }
    return zftrue;
}

// ============================================================
// common output for basic types
#define _ZFP_ZFCoreType_IODef_int_DEFINE(T_Int, allowNegative) \
    ZFOUTPUT_TYPE_DEFINE(T_Int, \
    { \
        const zfintSettingForOutput &setting = zfintSettingForOutputGet(output); \
        if(!allowNegative && v == ((T_Int)-1)) \
        { \
            output.execute(zfText("-1")); \
        } \
        else \
        { \
            output.execute(zfsFromInt(v, setting.radix(), setting.upperCase()).cString()); \
        } \
    }) \
    zfbool zfintFromInput(ZF_OUT T_Int &v, \
                          ZF_IN const ZFInputCallback &input, \
                          ZF_IN_OPT zfindex radix /* = 10 */) \
    { \
        return _ZFP_zfintFromInput(v, input, radix, allowNegative); \
    } \
    const ZFInputCallback &operator >> (ZF_IN_OUT const ZFInputCallback &input, ZF_OUT T_Int &v) \
    { \
        const zfintSettingForInput &setting = zfintSettingForInputGet(input); \
        zfintFromInput(v, input, setting.radix()); \
        return input; \
    }
_ZFP_ZFCoreType_IODef_int_DEFINE(short, zftrue)
_ZFP_ZFCoreType_IODef_int_DEFINE(unsigned short, zffalse)
_ZFP_ZFCoreType_IODef_int_DEFINE(int, zftrue)
_ZFP_ZFCoreType_IODef_int_DEFINE(unsigned int, zffalse)
_ZFP_ZFCoreType_IODef_int_DEFINE(long, zftrue)
_ZFP_ZFCoreType_IODef_int_DEFINE(unsigned long, zffalse)
_ZFP_ZFCoreType_IODef_int_DEFINE(long long, zftrue)
_ZFP_ZFCoreType_IODef_int_DEFINE(unsigned long long, zffalse)

#define _ZFP_ZFCoreType_IODef_float_DEFINE(T_Float) \
    ZFOUTPUT_TYPE_DEFINE(T_Float, \
    { \
        const zffloatSettingForOutput &setting = zffloatSettingForOutputGet(output); \
        output.execute(zfsFromFloat(v, setting.precision()).cString()); \
    }) \
    zfbool zffloatFromInput(ZF_OUT T_Float &v, \
                          ZF_IN const ZFInputCallback &input) \
    { \
        return _ZFP_zffloatFromInput(v, input); \
    } \
    const ZFInputCallback &operator >> (ZF_IN_OUT const ZFInputCallback &input, ZF_OUT T_Float &v) \
    { \
        zffloatFromInput(v, input); \
        return input; \
    }
_ZFP_ZFCoreType_IODef_float_DEFINE(float)
_ZFP_ZFCoreType_IODef_float_DEFINE(double)
_ZFP_ZFCoreType_IODef_float_DEFINE(long double)

static zfbool _ZFP_zfboolFromInput(ZF_OUT zfbool &v,
                                   ZF_IN const ZFInputCallback &input)
{
    const zfchar *tokens[] = {
        ZFTOKEN_zfbool_zftrue,
        ZFTOKEN_zfbool_zffalse,
    };
    zfindex matched = ZFInputCallbackCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), input);
    v = zffalse;
    switch(matched)
    {
        case 0:
            v = zftrue;
            return zftrue;
        case 1:
            v = zffalse;
            return zftrue;
        default:
            return zffalse;
    }
}
ZFINPUT_TYPE_DEFINE(zfbool, zfbool, {return _ZFP_zfboolFromInput(v, input);})

ZFINPUT_TYPE_DEFINE(zfchar, zfchar, {
    v = '\0';
    if(input.execute(&v, 1) != 1) {return zffalse;}
    return zftrue;
})

static zfbool _ZFP_zfstringFromInput(ZF_OUT zfstring &v, ZF_IN const ZFInputCallback &input)
{
    const zfstringSettingForInput &setting = zfstringSettingForInputGet(input);

    zfchar p = ZFInputCallbackSkipChars(input, setting.skipToken());
    if(p == '\0') {return zffalse;}
    zfindex quoteCount = 0;
    zfbool success = zffalse;
    zfbool first = zftrue;

    do {
        if(p == setting.escapeToken())
        {
            if(input.execute(&p, 1) != 1 || p == '\0')
            {
                v += setting.escapeToken();
                success = zftrue;
                break;
            }
            if(zfstringFindFirstOf(setting.endToken(), p) != zfindexMax
                || p == setting.escapeToken()
                || p == setting.quoteTokenLeft()
                || p == setting.quoteTokenRight())
            {
                v += p;
            }
            else
            {
                v += setting.escapeToken();
                v += p;
            }
        }
        else if(p == setting.quoteTokenLeft() || p == setting.quoteTokenRight())
        {
            if(setting.quoteTokenLeft() == setting.quoteTokenRight())
            {
                if(quoteCount == 0)
                { // left token
                    if(!first)
                    {
                        break;
                    }
                    ++quoteCount;
                }
                else
                { // right token
                    success = (quoteCount == 1);
                    break;
                }
            }
            else
            {
                if(p == setting.quoteTokenLeft())
                { // left token
                    if(!setting.quoteAllowEmbeded() && !first)
                    {
                        break;
                    }
                    ++quoteCount;
                }
                else
                { // right token
                    if(quoteCount == 0)
                    {
                        break;
                    }
                    --quoteCount;
                    if(quoteCount == 0)
                    {
                        success = zftrue;
                        break;
                    }
                }
            }
        }
        else if(zfstringFindFirstOf(setting.endToken(), p) != zfindexMax)
        {
            if(quoteCount == 0)
            {
                success = zftrue;
                input.ioSeek(1, ZFSeekPosCurReversely);
                break;
            }
            v += p;
        }
        else
        {
            v += p;
        }

        if(input.execute(&p, 1) != 1 || p == '\0')
        {
            success = (quoteCount == 0);
            break;
        }
        first = zffalse;
    } while(zftrue);

    return success;
}
ZFINPUT_TYPE_DEFINE(zfstring, zfstring, {
    return _ZFP_zfstringFromInput(v, input);
})

// ============================================================
// common output for core types
static zfbool _ZFP_ZFCompareResultFromInput(ZF_OUT ZFCompareResult &v, ZF_IN const ZFInputCallback &input)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFCompareUncomparable,
        ZFTOKEN_ZFCompareSmaller,
        ZFTOKEN_ZFCompareTheSame,
        ZFTOKEN_ZFCompareGreater,
    };
    zfindex matched = ZFInputCallbackCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), input);
    v = ZFCompareUncomparable;
    switch(matched)
    {
        case 0:
            v = ZFCompareUncomparable;
            return zftrue;
        case 1:
            v = ZFCompareSmaller;
            return zftrue;
        case 2:
            v = ZFCompareTheSame;
            return zftrue;
        case 3:
            v = ZFCompareGreater;
            return zftrue;
        default:
            return zffalse;
    }
}
ZFINPUT_TYPE_DEFINE(ZFCompareResult, ZFCompareResult, {return _ZFP_ZFCompareResultFromInput(v, input);})

static zfbool _ZFP_ZFSeekPosFromInput(ZF_OUT ZFSeekPos &v, ZF_IN const ZFInputCallback &input)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFSeekPosBegin,
        ZFTOKEN_ZFSeekPosCur,
        ZFTOKEN_ZFSeekPosCurReversely,
        ZFTOKEN_ZFSeekPosEnd,
    };
    zfindex matched = ZFInputCallbackCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), input);
    v = ZFSeekPosBegin;
    switch(matched)
    {
        case 0:
            v = ZFSeekPosBegin;
            return zftrue;
        case 1:
            v = ZFSeekPosCur;
            return zftrue;
        case 2:
            v = ZFSeekPosCurReversely;
            return zftrue;
        case 3:
            v = ZFSeekPosEnd;
            return zftrue;
        default:
            return zffalse;
    }
}
ZFINPUT_TYPE_DEFINE(ZFSeekPos, ZFSeekPos, {return _ZFP_ZFSeekPosFromInput(v, input);})

ZFINPUT_TYPE_DEFINE(zfindexRange, zfindexRange, {
    v = zfindexRangeZero;
    ZFCoreArrayPOD<zfindex> pair;
    if(!ZFInputCallbackReadDataPairInt(pair, input, 2))
    {
        return zffalse;
    }
    v.start = pair[0];
    v.count = pair[1];
    return zftrue;
})

// ============================================================
// common output for ZFObject types
static zfbool _ZFP_ZFFilterTypeFromInput(ZF_OUT ZFFilterType &v, ZF_IN const ZFInputCallback &input)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFFilterTypeInclude,
        ZFTOKEN_ZFFilterTypeExclude,
    };
    zfindex matched = ZFInputCallbackCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), input);
    v = ZFFilterTypeExclude;
    switch(matched)
    {
        case 0:
            v = ZFFilterTypeInclude;
            return zftrue;
        case 1:
            v = ZFFilterTypeExclude;
            return zftrue;
        default:
            return zffalse;
    }
}
ZFINPUT_TYPE_DEFINE(ZFFilterType, ZFFilterType, {return _ZFP_ZFFilterTypeFromInput(v, input);})

static zfbool _ZFP_ZFFilterCallbackResultFromInput(ZF_OUT ZFFilterCallbackResult &v, ZF_IN const ZFInputCallback &input)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFFilterCallbackResultNotSpecified,
        ZFTOKEN_ZFFilterCallbackResultActive,
        ZFTOKEN_ZFFilterCallbackResultNotActive,
    };
    zfindex matched = ZFInputCallbackCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), input);
    v = ZFFilterCallbackResultNotSpecified;
    switch(matched)
    {
        case 0:
            v = ZFFilterCallbackResultNotSpecified;
            return zftrue;
        case 1:
            v = ZFFilterCallbackResultActive;
            return zftrue;
        case 2:
            v = ZFFilterCallbackResultNotActive;
            return zftrue;
        default:
            return zffalse;
    }
}
ZFINPUT_TYPE_DEFINE(ZFFilterCallbackResult, ZFFilterCallbackResult, {return _ZFP_ZFFilterCallbackResultFromInput(v, input);})

static zfbool _ZFP_ZFClassFilterTypeFromInput(ZF_OUT ZFClassFilterType &v, ZF_IN const ZFInputCallback &input)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFClassFilterTypeInclude,
        ZFTOKEN_ZFClassFilterTypeExclude,
        ZFTOKEN_ZFClassFilterTypeIncludeChildOf,
        ZFTOKEN_ZFClassFilterTypeIncludeChildTypeOf,
        ZFTOKEN_ZFClassFilterTypeIncludeParentOf,
        ZFTOKEN_ZFClassFilterTypeIncludeParentTypeOf,
        ZFTOKEN_ZFClassFilterTypeExcludeChildOf,
        ZFTOKEN_ZFClassFilterTypeExcludeChildTypeOf,
        ZFTOKEN_ZFClassFilterTypeExcludeParentOf,
        ZFTOKEN_ZFClassFilterTypeExcludeParentTypeOf,
    };
    zfindex matched = ZFInputCallbackCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), input);
    v = ZFClassFilterTypeExclude;
    switch(matched)
    {
        case 0:
            v = ZFClassFilterTypeInclude;
            return zftrue;
        case 1:
            v = ZFClassFilterTypeExclude;
            return zftrue;
        case 2:
            v = ZFClassFilterTypeIncludeChildOf;
            return zftrue;
        case 3:
            v = ZFClassFilterTypeIncludeChildTypeOf;
            return zftrue;
        case 4:
            v = ZFClassFilterTypeIncludeParentOf;
            return zftrue;
        case 5:
            v = ZFClassFilterTypeIncludeParentTypeOf;
            return zftrue;
        case 6:
            v = ZFClassFilterTypeExcludeChildOf;
            return zftrue;
        case 7:
            v = ZFClassFilterTypeExcludeChildTypeOf;
            return zftrue;
        case 8:
            v = ZFClassFilterTypeExcludeParentOf;
            return zftrue;
        case 9:
            v = ZFClassFilterTypeExcludeParentTypeOf;
            return zftrue;
        default:
            return zffalse;
    }
}
ZFINPUT_TYPE_DEFINE(ZFClassFilterType, ZFClassFilterType, {return _ZFP_ZFClassFilterTypeFromInput(v, input);})

static zfbool _ZFP_ZFMethodPrivilegeTypeFromInput(ZF_OUT ZFMethodPrivilegeType &v, ZF_IN const ZFInputCallback &input)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFMethodPrivilegeTypePublic,
        ZFTOKEN_ZFMethodPrivilegeTypeProtected,
        ZFTOKEN_ZFMethodPrivilegeTypePrivate,
    };
    zfindex matched = ZFInputCallbackCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), input);
    v = ZFMethodPrivilegeTypePublic;
    switch(matched)
    {
        case 0:
            v = ZFMethodPrivilegeTypePublic;
            return zftrue;
        case 1:
            v = ZFMethodPrivilegeTypeProtected;
            return zftrue;
        case 2:
            v = ZFMethodPrivilegeTypePrivate;
            return zftrue;
        default:
            return zffalse;
    }
}
ZFINPUT_TYPE_DEFINE(ZFMethodPrivilegeType, ZFMethodPrivilegeType, {return _ZFP_ZFMethodPrivilegeTypeFromInput(v, input);})

ZF_NAMESPACE_GLOBAL_END

