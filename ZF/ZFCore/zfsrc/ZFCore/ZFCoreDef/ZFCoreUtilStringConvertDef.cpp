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
                zfflags tmp = 0;
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
void zfcharToString(ZF_IN_OUT zfstring &ret, ZF_IN zfchar const &value)
{
    ret += value;
}
const zfchar *zfcharFromString(ZF_OUT zfchar &ret,
                               ZF_IN const zfchar *src,
                               ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    if(src == zfnull)
    {
        return src;
    }
    ret = *src;
    return zfnull;
}

// ============================================================
void zfstringToString(ZF_IN_OUT zfstring &ret, ZF_IN zfstring const &value)
{
    ret += value;
}
const zfchar *zfstringFromString(ZF_OUT zfstring &ret,
                                 ZF_IN const zfchar *src,
                                 ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    if(src == zfnull)
    {
        return src;
    }
    ret.append(src, srcLen);
    return zfnull;
}

// ============================================================
void zfboolToString(ZF_IN_OUT zfstring &ret, ZF_IN zfbool const &value)
{
    ret += (value ? ZFTOKEN_zfbool_zftrue : ZFTOKEN_zfbool_zffalse);
}
const zfchar *zfboolFromString(ZF_OUT zfbool &ret,
                               ZF_IN const zfchar *src,
                               ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    const zfchar *tokens[] = {
        ZFTOKEN_zfbool_zftrue,
        ZFTOKEN_zfbool_zffalse,
    };
    zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
    ret = zffalse;
    switch(matched)
    {
        case 0:
            ret = zftrue;
            return zfnull;
        case 1:
            ret = zffalse;
            return zfnull;
        default:
            return src;
    }
}

// ============================================================
void zfbyteToString(ZF_IN_OUT zfstring &ret, ZF_IN zfbyte const &value)
{
    zfsFromIntT(ret, value, 16);
}
const zfchar *zfbyteFromString(ZF_OUT zfbyte &ret,
                               ZF_IN const zfchar *src,
                               ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    return zfsToInt(ret, src, srcLen, 16, zffalse);
}

// ============================================================
#define _ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_allow_negative(TypeName, Type) \
    void TypeName##ToString(ZF_IN_OUT zfstring &ret, ZF_IN Type const &value) \
    { \
        zfsFromIntT(ret, value); \
    } \
    const zfchar *TypeName##FromString(ZF_OUT Type &ret, \
                                       ZF_IN const zfchar *src, \
                                       ZF_IN_OPT zfindex srcLen /* = zfindexMax */) \
    { \
        return zfsToInt(ret, src, srcLen, 10, zftrue); \
    }
#define _ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_disallow_negative(TypeName, Type) \
    void TypeName##ToString(ZF_IN_OUT zfstring &ret, ZF_IN Type const &value) \
    { \
        if(value == ((Type)-1)) \
        { \
            ret += zfText("-1"); \
        } \
        else \
        { \
            zfsFromIntT(ret, value); \
        } \
    } \
    const zfchar *TypeName##FromString(ZF_OUT Type &ret, \
                                       ZF_IN const zfchar *src, \
                                       ZF_IN_OPT zfindex srcLen /* = zfindexMax */) \
    { \
        if(srcLen >= 2 && src != zfnull && zfsncmp(src, zfText("-1"), srcLen) == 0) \
        { \
            ret = (Type)-1; \
            return zfnull; \
        } \
        else \
        { \
            return zfsToInt(ret, src, srcLen, 10, zffalse); \
        } \
    }

_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_allow_negative(zfint, zfint)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_disallow_negative(zfuint, zfuint)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_disallow_negative(zfindex, zfindex)

_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_allow_negative(zfint8, zfint8)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_disallow_negative(zfuint8, zfuint8)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_allow_negative(zfint16, zfint16)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_disallow_negative(zfuint16, zfuint16)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_allow_negative(zfint32, zfint32)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_disallow_negative(zfuint32, zfuint32)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_allow_negative(zfint64, zfint64)
_ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_int_disallow_negative(zfuint64, zfuint64)

// ============================================================
#define _ZFP_ZFCORETYPE_STRING_CONVERTER_DEFINE_float(TypeName, Type) \
    void TypeName##ToString(ZF_IN_OUT zfstring &ret, ZF_IN Type const &value) \
    { \
        zfsFromFloatT(ret, value); \
    } \
    const zfchar *TypeName##FromString(ZF_OUT Type &ret, \
                                       ZF_IN const zfchar *src, \
                                       ZF_IN_OPT zfindex srcLen /* = zfindexMax */) \
    { \
        return zfsToFloat(ret, src, srcLen); \
    }

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
void ZFCompareResultToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFCompareResult const &value)
{
    switch(value)
    {
        case ZFCompareUncomparable:
            ret += ZFTOKEN_ZFCompareUncomparable;
            return ;
        case ZFCompareSmaller:
            ret += ZFTOKEN_ZFCompareSmaller;
            return ;
        case ZFCompareTheSame:
            ret += ZFTOKEN_ZFCompareTheSame;
            return ;
        case ZFCompareGreater:
            ret += ZFTOKEN_ZFCompareGreater;
            return ;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
const zfchar *ZFCompareResultFromString(ZF_OUT ZFCompareResult &ret,
                                        ZF_IN const zfchar *src,
                                        ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    if(src == zfnull) {return zfnull;}
    if(zfsncmp(src, ZFTOKEN_ZFCompareTheSame, srcLen) == 0)
    {
        ret = ZFCompareTheSame;
        return zfnull;
    }
    else if(zfsncmp(src, ZFTOKEN_ZFCompareSmaller, srcLen) == 0)
    {
        ret = ZFCompareSmaller;
        return zfnull;
    }
    else if(zfsncmp(src, ZFTOKEN_ZFCompareGreater, srcLen) == 0)
    {
        ret = ZFCompareGreater;
        return zfnull;
    }
    else if(zfsncmp(src, ZFTOKEN_ZFCompareUncomparable, srcLen) == 0)
    {
        ret = ZFCompareUncomparable;
        return zfnull;
    }
    else
    {
        return src;
    }
}

// ============================================================
void ZFSeekPosToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFSeekPos const &value)
{
    switch(value)
    {
        case ZFSeekPosBegin:
            ret += ZFTOKEN_ZFSeekPosBegin;
            return ;
        case ZFSeekPosCur:
            ret += ZFTOKEN_ZFSeekPosCur;
            return ;
        case ZFSeekPosCurReversely:
            ret += ZFTOKEN_ZFSeekPosCurReversely;
            return ;
        case ZFSeekPosEnd:
            ret += ZFTOKEN_ZFSeekPosEnd;
            return ;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
const zfchar *ZFSeekPosFromString(ZF_OUT ZFSeekPos &ret,
                                  ZF_IN const zfchar *src,
                                  ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFSeekPosBegin,
        ZFTOKEN_ZFSeekPosCur,
        ZFTOKEN_ZFSeekPosCurReversely,
        ZFTOKEN_ZFSeekPosEnd,
    };
    zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
    ret = ZFSeekPosBegin;
    switch(matched)
    {
        case 0:
            ret = ZFSeekPosBegin;
            return zfnull;
        case 1:
            ret = ZFSeekPosCur;
            return zfnull;
        case 2:
            ret = ZFSeekPosCurReversely;
            return zfnull;
        case 3:
            ret = ZFSeekPosEnd;
            return zfnull;
        default:
            return src;
    }
}

// ============================================================
void zfindexRangeToString(ZF_IN_OUT zfstring &ret, ZF_IN zfindexRange const &value)
{
    zfstringAppend(ret, zfText("(%zi, %zi)"), value.start, value.count);
}
const zfchar *zfindexRangeFromString(ZF_OUT zfindexRange &ret,
                                     ZF_IN const zfchar *src,
                                     ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    ret = zfindexRangeZero;
    ZFCoreArrayPOD<zfindex> pair;
    const zfchar *errPos = zfCoreDataPairSplitInt(pair, 2, src, srcLen);
    if(errPos != zfnull)
    {
        return errPos;
    }
    ret.start = pair[0];
    ret.count = pair[1];
    return zfnull;
}

ZF_NAMESPACE_GLOBAL_END

