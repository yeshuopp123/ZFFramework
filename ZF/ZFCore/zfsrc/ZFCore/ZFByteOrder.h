/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFByteOrder.h
 * @brief byte order define for ZFFramework
 */

#ifndef _ZFI_ZFByteOrder_h_
#define _ZFI_ZFByteOrder_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief byte order define for ZFFramework
 */
ZFENUM_BEGIN(ZFByteOrder)
    ZFENUM_VALUE(LittleEndian)
    ZFENUM_VALUE(BigEndian)
ZFENUM_SEPARATOR(ZFByteOrder)
    ZFENUM_VALUE_REGISTER(LittleEndian)
    ZFENUM_VALUE_REGISTER(BigEndian)
ZFENUM_END(ZFByteOrder)

extern ZF_ENV_EXPORT ZFByteOrderEnum &_ZFP_ZFByteOrderLocalEndian(void);
extern ZF_ENV_EXPORT ZFByteOrderEnum &_ZFP_ZFByteOrderNetEndian(void);

/**
 * @brief local byte order, auto detected
 *
 * you may also change it at runtime, but deprecated
 */
#define ZFByteOrderLocalEndian _ZFP_ZFByteOrderLocalEndian()

/**
 * @brief network transfer byte order, e_BigEndian
 *
 * you may also change it at runtime, but deprecated
 */
#define ZFByteOrderNetEndian _ZFP_ZFByteOrderNetEndian()

/**
 * @brief convert byte order
 *
 * typical usage:
 * @code
 *   zfbyte dst[128];
 *   zfindex count = 3;
 *   zfindex *src = zfnewArray(zfindex, count);
 *   ZFByteOrderChange(
 *       dst,
 *       src,
 *       sizeof(zfindex), // unitSize is type's size
 *       count // count is element's count
 *       );
 * @endcode
 */
extern ZF_ENV_EXPORT void ZFByteOrderChange(ZF_OUT void *dst,
                                            ZF_IN const void *src,
                                            ZF_IN zfindex unitSize,
                                            ZF_IN_OPT zfindex count = 1);

/**
 * @brief encode src to a heximal NULL-terminated string and store to dst
 *
 * for example:
 * @code
 *   zfcharA buf[128] = {0};
 *
 *   zfindex n = 0xABCD;
 *
 *   // buf would be "0000ABCD"
 *   ZFByteOrderEncode(buf, &n, sizeof(zfindex), 1, ZFByteOrder::e_BigEndian);
 *
 *   // buf would be "CDAB0000"
 *   ZFByteOrderEncode(buf, &n, sizeof(zfindex), 1, ZFByteOrder::e_LittleEndian);
 *
 *
 *   const zfchar *s = zfText("ABCD");
 *
 *   // buf would be "0041004200430044"
 *   ZFByteOrderEncode(buf, s, sizeof(zfchar), zfslen(s), ZFByteOrder::e_BigEndian);
 *
 *   // buf would be "4100420043004400"
 *   ZFByteOrderEncode(buf, s, sizeof(zfchar), zfslen(s), ZFByteOrder::e_LittleEndian);
 * @endcode
 * dst's size must be larger than (unitSize*count*2 + 1) bytes\n
 * src's byte order is specified by srcEndian
 * @see ZFByteOrderDecode
 */
extern ZF_ENV_EXPORT zfbool ZFByteOrderEncode(ZF_OUT zfcharA *dst,
                                              ZF_IN const void *src,
                                              ZF_IN zfindex unitSize,
                                              ZF_IN_OPT zfindex count = 1,
                                              ZF_IN_OPT ZFByteOrderEnum srcEndian = ZFByteOrderLocalEndian);
/**
 * @brief see #ZFByteOrderEncode
 *
 * result would be append to dst (dst's existing content won't be cleared)
 */
extern ZF_ENV_EXPORT zfbool ZFByteOrderEncode(ZF_IN_OUT zfstringA &dst,
                                              ZF_IN const void *src,
                                              ZF_IN zfindex unitSize,
                                              ZF_IN_OPT zfindex count = 1,
                                              ZF_IN_OPT ZFByteOrderEnum srcEndian = ZFByteOrderLocalEndian);

/**
 * @brief decode from heximal string to dst
 * @see ZFByteOrderEncode
 */
extern ZF_ENV_EXPORT zfbool ZFByteOrderDecode(ZF_OUT void *dst,
                                              ZF_IN const zfcharA *src,
                                              ZF_IN zfindex unitSize,
                                              ZF_IN zfindex count = 1,
                                              ZF_IN_OPT ZFByteOrderEnum dstEndian = ZFByteOrderLocalEndian);

/**
 * @brief get result byte size or zfindexMax if size not valid
 * @see ZFByteOrderEncode
 */
extern ZF_ENV_EXPORT zfindex ZFByteOrderDecodeGetSize(ZF_IN zfindex encodedByteSize);

/**
 * @brief print a readable hex-string to dst, grouped by unitSize
 *
 * result would be appended to dst instead of clear and set\n
 * memorySizeInByte is the byte size of the memory to print\n
 * unitSize is the unit size of the memory to be grouped,
 * it also affect the endian when output\n
 * groupCountPerLine is how many group should be print before a '\\n'
 */
template<typename T_Str>
ZF_ENV_EXPORT void ZFByteOrderPrintMemory(ZF_OUT T_Str &dst,
                                          ZF_IN const void *src,
                                          ZF_IN zfindex memorySizeInByte,
                                          ZF_IN_OPT zfindex unitSize = 1,
                                          ZF_IN_OPT zfindex groupCountPerLine = zfindexMax,
                                          ZF_IN_OPT ZFByteOrderEnum srcEndian = ZFByteOrderLocalEndian)
{
    if(memorySizeInByte == 0 || unitSize == 0)
    {
        return ;
    }

    const zfbyte *p = ZFCastStatic(const zfbyte *, src);
    if(srcEndian == ZFByteOrder::e_BigEndian || unitSize == 1)
    {
        const zfbyte *pTmp = p;
        const zfbyte *pTmpEnd = p + memorySizeInByte;
        zfindex groupCount = 0;
        while(pTmp < pTmpEnd)
        {
            for(zfindex i = 0; i < unitSize && (pTmp + i) < pTmpEnd; ++i)
            {
                zfbyte c = pTmp[i];
                zfbyte t = (zfbyte)((c >> 4) & 0x0F);
                dst += (zfcharA)(((t < 10) ? ('0' + t) : ('A' + t - 10)));
                t = (zfbyte)(c & 0x0F);
                dst += (zfcharA)(((t < 10) ? ('0' + t) : ('A' + t - 10)));
            }
            pTmp += unitSize;
            if(pTmp < pTmpEnd)
            {
                ++groupCount;
                if(groupCount >= groupCountPerLine)
                {
                    dst += '\n';
                    groupCount = 0;
                }
                else
                {
                    dst += ' ';
                }
            }
        }
    }
    else
    {
        const zfbyte *pTmp = p;
        const zfbyte *pTmpEnd = p + memorySizeInByte;
        zfindex groupCount = 0;
        while(pTmp < pTmpEnd)
        {
            for(zfindex i = 0; i < unitSize && (pTmp + i) < pTmpEnd; ++i)
            {
                zfbyte c = pTmp[unitSize - i - 1];
                zfbyte t = (zfbyte)((c >> 4) & 0x0F);
                dst += (zfcharA)(((t < 10) ? ('0' + t) : ('A' + t - 10)));
                t = (zfbyte)(c & 0x0F);
                dst += (zfcharA)(((t < 10) ? ('0' + t) : ('A' + t - 10)));
            }
            pTmp += unitSize;
            if(pTmp < pTmpEnd)
            {
                ++groupCount;
                if(groupCount >= groupCountPerLine)
                {
                    dst += '\n';
                    groupCount = 0;
                }
                else
                {
                    dst += ' ';
                }
            }
        }
    }
}

/**
 * @brief see #ZFByteOrderPrintMemory
 */
template<typename T_Str>
ZF_ENV_EXPORT T_Str ZFByteOrderPrintMemory(ZF_IN const void *src,
                                           ZF_IN zfindex memorySizeInByte,
                                           ZF_IN_OPT zfindex unitSize = 1,
                                           ZF_IN_OPT zfindex groupCountPerLine = zfindexMax,
                                           ZF_IN_OPT ZFByteOrderEnum srcEndian = ZFByteOrderLocalEndian)
{
    T_Str dst;
    ZFByteOrderPrintMemory(dst, src, memorySizeInByte, unitSize, groupCountPerLine, srcEndian);
    return dst;
}

/**
 * @brief see #ZFByteOrderPrintMemory
 */
extern ZF_ENV_EXPORT zfstring ZFByteOrderPrintMemory(ZF_IN const void *src,
                                                     ZF_IN zfindex memorySizeInByte,
                                                     ZF_IN_OPT zfindex unitSize = 1,
                                                     ZF_IN_OPT zfindex groupCountPerLine = zfindexMax,
                                                     ZF_IN_OPT ZFByteOrderEnum srcEndian = ZFByteOrderLocalEndian);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFByteOrder_h_

