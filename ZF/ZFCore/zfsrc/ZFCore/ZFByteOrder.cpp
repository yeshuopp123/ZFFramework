/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFByteOrder.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFByteOrder)

static ZFByteOrderEnum _ZFP_ZFByteOrderLocalEndianInit(void)
{
    union
    {
        int n;
        zfcharA c;
    } u;
    u.n = 1;
    if(u.c == 1)
    {
        return ZFByteOrder::e_LittleEndian;
    }
    else
    {
        return ZFByteOrder::e_BigEndian;
    }
}
ZFByteOrderEnum &_ZFP_ZFByteOrderLocalEndian(void)
{
    static ZFByteOrderEnum d = _ZFP_ZFByteOrderLocalEndianInit();
    return d;
}
ZFByteOrderEnum &_ZFP_ZFByteOrderNetEndian(void)
{
    static ZFByteOrderEnum d = ZFByteOrder::e_BigEndian;
    return d;
}

void ZFByteOrderChange(ZF_OUT void *dst,
                       ZF_IN const void *src,
                       ZF_IN zfindex unitSize,
                       ZF_IN_OPT zfindex count /* = 1 */)
{
    if(dst == zfnull || src == zfnull || unitSize == 0 || count == 0)
    {
        return ;
    }
    zfbyte *pDst = ZFCastStatic(zfbyte *, dst);
    const zfbyte *pSrc = ZFCastStatic(const zfbyte *, src);
    if(unitSize == 1)
    {
        zfmemcpy(pDst, pSrc, count);
    }
    else
    {
        for(zfindex i = 0; i < count; ++i)
        {
            for(zfindex byteIndex = 0; byteIndex < unitSize; ++byteIndex)
            {
                pDst[byteIndex] = pSrc[unitSize - byteIndex - 1];
            }
            pDst += unitSize;
            pSrc += unitSize;
        }
    }
}

#define _ZFP_ZFByteOrderEncodeEndian ZFByteOrder::e_LittleEndian

static void _ZFP_ZFByteOrderEncodeByte(ZF_OUT zfcharA *dst,
                                       ZF_IN zfbyte src)
{
    unsigned short int n = (src >> 4) & 0x0F;
    dst[0] = ((n < 10) ? ('0' + n) : ('A' + (n - 10)));
    n = src & 0x0F;
    dst[1] = ((n < 10) ? ('0' + n) : ('A' + (n - 10)));
}

zfbool ZFByteOrderEncode(ZF_OUT zfcharA *dst,
                         ZF_IN const void *src,
                         ZF_IN zfindex unitSize,
                         ZF_IN_OPT zfindex count /* = 1 */,
                         ZF_IN_OPT ZFByteOrderEnum srcEndian /* = ZFByteOrderLocalEndian */)
{
    if(dst == zfnull || src == zfnull || unitSize == 0 || count == 0)
    {
        return zffalse;
    }

    zfcharA *pDst = dst;
    const zfbyte *pSrc = ZFCastStatic(const zfbyte *, src);
    if(srcEndian == _ZFP_ZFByteOrderEncodeEndian || unitSize == 1)
    {
        zfindex size = unitSize * count;
        for(zfindex i = 0; i < size; ++i)
        {
            _ZFP_ZFByteOrderEncodeByte(pDst, pSrc[i]);
            pDst += 2;
        }
    }
    else
    {
        for(zfindex i = 0; i < count; ++i)
        {
            for(zfindex byteIndex = 0; byteIndex < unitSize; ++byteIndex)
            {
                _ZFP_ZFByteOrderEncodeByte(pDst, pSrc[unitSize - byteIndex - 1]);
                pDst += 2;
            }
            pSrc += unitSize;
        }
    }
    *pDst = '\0';
    return zftrue;
}
zfbool ZFByteOrderEncode(ZF_IN_OUT zfstringA &dst,
                         ZF_IN const void *src,
                         ZF_IN zfindex unitSize,
                         ZF_IN_OPT zfindex count /* = 1 */,
                         ZF_IN_OPT ZFByteOrderEnum srcEndian /* = ZFByteOrderLocalEndian */)
{
    if(src == zfnull || unitSize == 0 || count == 0)
    {
        return zffalse;
    }

    zfcharA buf[4] = {0};
    const zfbyte *pSrc = ZFCastStatic(const zfbyte *, src);
    if(srcEndian == _ZFP_ZFByteOrderEncodeEndian || unitSize == 1)
    {
        zfindex size = unitSize * count;
        for(zfindex i = 0; i < size; ++i)
        {
            _ZFP_ZFByteOrderEncodeByte(buf, pSrc[i]);
            dst += buf[0];
            dst += buf[1];
        }
    }
    else
    {
        for(zfindex i = 0; i < count; ++i)
        {
            for(zfindex byteIndex = 0; byteIndex < unitSize; ++byteIndex)
            {
                _ZFP_ZFByteOrderEncodeByte(buf, pSrc[unitSize - byteIndex - 1]);
                dst += buf[0];
                dst += buf[1];
            }
            pSrc += unitSize;
        }
    }
    return zftrue;
}

static zfbool _ZFP_ZFByteOrderDecodeByte(ZF_OUT zfbyte *dst,
                                         ZF_IN const zfcharA *src)
{
    unsigned short int n0;
    if(src[0] >= '0' && src[0] <= '9')
    {
        n0 = src[0] - '0';
    }
    else if(src[0] >= 'A' && src[0] <= 'F')
    {
        n0 = 10 + src[0] - 'A';
    }
    else
    {
        return zffalse;
    }

    unsigned short int n1;
    if(src[1] >= '0' && src[1] <= '9')
    {
        n1 = src[1] - '0';
    }
    else if(src[1] >= 'A' && src[1] <= 'F')
    {
        n1 = 10 + src[1] - 'A';
    }
    else
    {
        return zffalse;
    }

    *dst = (zfbyte)(((n0 << 4) & 0xF0) | (n1 & 0x0F));
    return zftrue;
}
zfbool ZFByteOrderDecode(ZF_OUT void *dst,
                         ZF_IN const zfcharA *src,
                         ZF_IN zfindex unitSize,
                         ZF_IN zfindex count /* = 1 */,
                         ZF_IN_OPT ZFByteOrderEnum dstEndian /* = ZFByteOrderLocalEndian */)
{
    if(dst == zfnull || src == zfnull || unitSize == 0 || count == 0)
    {
        return zffalse;
    }

    zfbyte *pDst = ZFCastStatic(zfbyte *, dst);
    const zfcharA *pSrc = src;
    if(dstEndian == _ZFP_ZFByteOrderEncodeEndian || unitSize == 1)
    {
        zfindex size = unitSize * count;
        for(zfindex i = 0; i < size; ++i)
        {
            if(!_ZFP_ZFByteOrderDecodeByte(pDst, pSrc))
            {
                return zffalse;
            }
            ++pDst;
            pSrc += 2;
        }
    }
    else
    {
        for(zfindex i = 0; i < count; ++i)
        {
            for(zfindex byteIndex = 0; byteIndex < unitSize; ++byteIndex)
            {
                if(!_ZFP_ZFByteOrderDecodeByte(pDst, pSrc + (unitSize - byteIndex - 1) * 2))
                {
                    return zffalse;
                }
                ++pDst;
            }
            pSrc += unitSize * 2;
        }
    }
    return zftrue;
}

zfindex ZFByteOrderDecodeGetSize(ZF_IN zfindex encodedByteSize)
{
    if(encodedByteSize % 2 != 0)
    {
        return zfindexMax;
    }
    return ZFCastStatic(zfindex, encodedByteSize / 2);
}

zfstring ZFByteOrderPrintMemory(ZF_IN const void *src,
                                ZF_IN zfindex memorySizeInByte,
                                ZF_IN_OPT zfindex unitSize /* = 1 */,
                                ZF_IN_OPT zfindex groupCountPerLine /* = zfindexMax */,
                                ZF_IN_OPT ZFByteOrderEnum srcEndian /* = ZFByteOrderLocalEndian */)
{
    zfstring dst;
    ZFByteOrderPrintMemory(dst, src, memorySizeInByte, unitSize, groupCountPerLine, srcEndian);
    return dst;
}

ZF_NAMESPACE_GLOBAL_END

