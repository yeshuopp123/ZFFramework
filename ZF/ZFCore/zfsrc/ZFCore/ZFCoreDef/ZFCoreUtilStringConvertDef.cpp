/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreUtilStringConvertDef.h"
#include "ZFCoreUtilDef.h"
#include "ZFCoreArrayDef.h"
#include "ZFCoreDataPairSplitDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfindex ZFCoreStringCheckMatch(ZF_IN const zfchar **tokens,
                               ZF_IN zfindex tokenCount,
                               ZF_IN const zfchar *toCompare,
                               ZF_IN_OPT zfindex toCompareLength /* = zfindexMax */)
{
    if(toCompare == zfnull)
    {
        return zfindexMax;
    }

    zfindex tmpLen = 0;
    for(zfindex i = 0; i < tokenCount; ++i)
    {
        tmpLen = zfslen(tokens[i]);
        if(tmpLen < toCompareLength && zfsncmp(tokens[i], toCompare, tmpLen) == 0)
        {
            return i;
        }
    }
    return zfindexMax;
}

// ============================================================
#define _ZFP_zfflagsInvalidToken (zfText("0x0"))
void zfflagsToString(ZF_IN_OUT zfstring &ret,
                     ZF_IN const zfflags *flagList,
                     ZF_IN const zfchar **nameList,
                     ZF_IN zfindex listCount,
                     ZF_IN zfflags const &value,
                     ZF_IN_OPT zfbool includeNotConverted /* = zftrue */,
                     ZF_IN_OPT zfbool exclusiveMode /* = zffalse */,
                     ZF_OUT_OPT zfflags *notConverted /* = zfnull */,
                     ZF_IN_OPT zfchar separatorToken /* = '|' */)
{
    zfflags flagsLeft = value;
    if(exclusiveMode)
    {
        for(zfindex i = 0; i < listCount; ++i)
        {
            if(flagList[i] && ZFBitTest(flagsLeft, flagList[i]))
            {
                ret += nameList[i];
                ret += separatorToken;
                ZFBitUnset(flagsLeft, flagList[i]);
            }
            if(flagsLeft == 0)
            {
                break;
            }
        }
    }
    else
    {
        for(zfindex i = 0; i < listCount; ++i)
        {
            if(flagList[i] && ZFBitTest(value, flagList[i]))
            {
                ret += nameList[i];
                ret += separatorToken;
                ZFBitUnset(flagsLeft, flagList[i]);
            }
        }
    }

    // check flagsLeft and remove tailing separatorToken from ret
    if(flagsLeft == 0)
    {
        if(ret.isEmpty())
        {
            zfindex zeroTokenIndex = zfindexMax;
            for(zfindex i = 0; i < listCount; ++i)
            {
                if(flagList[i] == 0)
                {
                    zeroTokenIndex = i;
                    break;
                }
            }
            if(zeroTokenIndex == zfindexMax)
            {
                ret = _ZFP_zfflagsInvalidToken;
            }
            else
            {
                ret = nameList[zeroTokenIndex];
            }
        }
        else
        {
            ret.remove(ret.length() - 1);
        }
    }
    else if(includeNotConverted)
    {
        ret += zfText("0x");
        zfsFromIntT(ret, flagsLeft,
            zfHint("radix")16,
            zfHint("upper case?")zftrue);
    }

    if(notConverted != zfnull)
    {
        *notConverted = flagsLeft;
    }
}
const zfchar *zfflagsFromString(ZF_OUT zfflags &ret,
                                ZF_IN const zfflags *flagList,
                                ZF_IN const zfchar **nameList,
                                ZF_IN zfindex listCount,
                                ZF_IN const zfchar *src,
                                ZF_IN_OPT zfindex srcLen /* = zfindexMax */,
                                ZF_IN_OPT zfchar separatorToken /* = '|' */)
{
    if(src == zfnull)
    {
        return src;
    }
    ZFCoreArrayPOD<zfindexRange> pos;
    zfstring separatorTokens;
    separatorTokens += separatorToken;
    const zfchar *errPosTmp = zfCoreDataPairSplitString(pos, zfindexMax, src, srcLen, '\0', '\0', separatorTokens);
    if(errPosTmp != zfnull)
    {
        return errPosTmp;
    }
    ret = 0;
    for(zfindex iSrc = 0; iSrc < pos.count(); ++iSrc)
    {
        zfbool recognized = zffalse;
        for(zfindex iList = 0; iList < listCount; ++iList)
        {
            if(zfsncmp(src + pos[iSrc].start, nameList[iList], pos[iSrc].count) == 0)
            {
                recognized = zftrue;
                ZFBitSet(ret, flagList[iList]);
                break;
            }
        }
        if(!recognized)
        {
            if(pos[iSrc].count > 2 && *(src + pos[iSrc].start) == '0' && *(src + pos[iSrc].start + 1) == 'x')
            {
                zfflags tmp = zfflagsZero;
                errPosTmp = zfsToInt(tmp, src + pos[iSrc].start + 2, pos[iSrc].count - 2, 16);
                if(errPosTmp != zfnull)
                {
                    return errPosTmp;
                }
                ZFBitSet(ret, tmp);
            }
            else
            {
                return src + pos[iSrc].start;
            }
        }
    }
    return zfnull;
}

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DEFINE(zfchar, zfchar, {
        if(src == zfnull)
        {
            return src;
        }
        v = *src;
        return zfnull;
    }, {
        s += v;
    })

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DEFINE(zfstring, zfstring, {
        if(src == zfnull)
        {
            return src;
        }
        v.append(src, srcLen);
        return zfnull;
    }, {
        s += v;
    })

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DEFINE(zfbool, zfbool, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_zfbool_zftrue,
            ZFTOKEN_zfbool_zffalse,
        });
        zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = zffalse;
        switch(matched)
        {
            case 0:
                v = zftrue;
                return zfnull;
            case 1:
                v = zffalse;
                return zfnull;
            default:
                return src;
        }
    }, {
        s += (v ? ZFTOKEN_zfbool_zftrue : ZFTOKEN_zfbool_zffalse);
    })

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DEFINE(zfbyte, zfbyte, {
        return zfsToInt(v, src, srcLen, 16, zffalse);
    }, {
        zfsFromIntT(s, v, 16);
    })

// ============================================================
#define _ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_allow_negative(TypeName, Type) \
    ZFCORETYPE_STRING_CONVERTER_DEFINE(TypeName, Type, { \
            return zfsToInt(v, src, srcLen, 10, zftrue); \
        }, { \
            zfsFromIntT(s, v); \
        })
#define _ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_disallow_negative(TypeName, Type) \
    ZFCORETYPE_STRING_CONVERTER_DEFINE(TypeName, Type, { \
            if(srcLen >= 2 && src != zfnull && zfsncmp(src, zfText("-1"), srcLen) == 0) \
            { \
                v = (Type)-1; \
                return zfnull; \
            } \
            else \
            { \
                return zfsToInt(v, src, srcLen, 10, zffalse); \
            } \
        }, { \
            if(v == ((Type)-1)) \
            { \
                s += zfText("-1"); \
            } \
            else \
            { \
                zfsFromIntT(s, v); \
            } \
        })

_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_allow_negative(zfint, zfint)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_disallow_negative(zfuint, zfuint)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_disallow_negative(zfindex, zfindex)

// ============================================================
#define _ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_float(TypeName, Type) \
    ZFCORETYPE_STRING_CONVERTER_DEFINE(TypeName, Type, { \
            return zfsToFloat(v, src, srcLen); \
        }, { \
            zfsFromFloatT(s, v); \
        })

_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_float(zffloat, zffloat)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_float(zfdouble, zfdouble)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_float(zflongdouble, zflongdouble)

// ============================================================
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_allow_negative(zftimet, zftimet)

// ============================================================
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_disallow_negative(zfflags, zfflags)

// ============================================================
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_disallow_negative(zfidentity, zfidentity)

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DEFINE(ZFCompareResult, ZFCompareResult, {
        if(src == zfnull) {return zfnull;}
        if(zfsncmp(src, ZFTOKEN_ZFCompareTheSame, srcLen) == 0)
        {
            v = ZFCompareTheSame;
            return zfnull;
        }
        else if(zfsncmp(src, ZFTOKEN_ZFCompareSmaller, srcLen) == 0)
        {
            v = ZFCompareSmaller;
            return zfnull;
        }
        else if(zfsncmp(src, ZFTOKEN_ZFCompareGreater, srcLen) == 0)
        {
            v = ZFCompareGreater;
            return zfnull;
        }
        else if(zfsncmp(src, ZFTOKEN_ZFCompareUncomparable, srcLen) == 0)
        {
            v = ZFCompareUncomparable;
            return zfnull;
        }
        else
        {
            return src;
        }
    }, {
        switch(v)
        {
            case ZFCompareUncomparable:
                s += ZFTOKEN_ZFCompareUncomparable;
                return ;
            case ZFCompareSmaller:
                s += ZFTOKEN_ZFCompareSmaller;
                return ;
            case ZFCompareTheSame:
                s += ZFTOKEN_ZFCompareTheSame;
                return ;
            case ZFCompareGreater:
                s += ZFTOKEN_ZFCompareGreater;
                return ;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    })

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DEFINE(ZFSeekPos, ZFSeekPos, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFSeekPosBegin,
            ZFTOKEN_ZFSeekPosCur,
            ZFTOKEN_ZFSeekPosCurReversely,
            ZFTOKEN_ZFSeekPosEnd,
        });
        zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFSeekPosBegin;
        switch(matched)
        {
            case 0:
                v = ZFSeekPosBegin;
                return zfnull;
            case 1:
                v = ZFSeekPosCur;
                return zfnull;
            case 2:
                v = ZFSeekPosCurReversely;
                return zfnull;
            case 3:
                v = ZFSeekPosEnd;
                return zfnull;
            default:
                return src;
        }
    }, {
        switch(v)
        {
            case ZFSeekPosBegin:
                s += ZFTOKEN_ZFSeekPosBegin;
                return ;
            case ZFSeekPosCur:
                s += ZFTOKEN_ZFSeekPosCur;
                return ;
            case ZFSeekPosCurReversely:
                s += ZFTOKEN_ZFSeekPosCurReversely;
                return ;
            case ZFSeekPosEnd:
                s += ZFTOKEN_ZFSeekPosEnd;
                return ;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    })

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DEFINE(zfindexRange, zfindexRange, {
        v = zfindexRangeZero;
        ZFCoreArrayPOD<zfindex> pair;
        const zfchar *errPos = zfCoreDataPairSplitInt(pair, 2, src, srcLen);
        if(errPos != zfnull)
        {
            return errPos;
        }
        v.start = pair[0];
        v.count = pair[1];
        return zfnull;
    }, {
        zfstringAppend(s, zfText("(%zi, %zi)"), v.start, v.count);
    })

ZF_NAMESPACE_GLOBAL_END

