/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFFileBOMDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
void ZFFileBOMToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFFileBOM const &value)
{
    for(zfindex i = 0; value.BOM[i] != 0x00; ++i)
    {
        zfstringAppend(ret, zfText("%02X"), value.BOM[i]);
    }
}
const zfchar *ZFFileBOMFromString(ZF_OUT ZFFileBOM &ret,
                                  ZF_IN const zfchar *src,
                                  ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    if(srcLen == zfindexMax)
    {
        srcLen = zfslen(src);
    }
    if((srcLen % 2) != 0 || srcLen >= ZFFileBOMMaxSize)
    {
        return src;
    }

    zfmemset(ret.BOM, 0, sizeof(ret.BOM));

    for(zfindex i = 0; ; ++i, src += 2)
    {
        if(zfnull != zfsToInt(ret.BOM[i], src, 2, 16))
        {
            return src;
        }
    }

    return zfnull;
}

void ZFFileBOMListToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFCoreArrayPOD<ZFFileBOM> const &value)
{
    ZFFileBOMListToString(ret, value.arrayBuf(), value.count());
}
void ZFFileBOMListToString(ZF_IN_OUT zfstring &ret,
                           ZF_IN const ZFFileBOM *BOMList,
                           ZF_IN zfindex BOMListCount)
{
    ret += zfText("(");
    for(zfindex i = 0; i < BOMListCount; ++i)
    {
        if(i != 0)
        {
            ret += zfText(", ");
        }
        ZFFileBOMToString(ret, BOMList[i]);
    }
    ret += zfText(")");
}
const zfchar *ZFFileBOMListFromString(ZF_OUT ZFCoreArrayPOD<ZFFileBOM> &ret,
                                      ZF_IN const zfchar *src,
                                      ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    ZFCoreArrayPOD<zfindexRange> pos;
    const zfchar *tmp = zfCoreDataPairSplitString(pos, zfindexMax, src, srcLen);
    if(tmp != zfnull)
    {
        return zfnull;
    }

    for(zfindex i = 0; i < pos.count(); ++i)
    {
        ZFFileBOM bom;
        tmp = ZFFileBOMFromString(bom, src + pos[i].start, pos[i].count);
        if(tmp != zfnull)
        {
            return tmp;
        }
        ret.add(bom);
    }

    return zfnull;
}

const ZFFileBOM &_ZFP_ZFFileBOMUTF8(void)
{
    static ZFFileBOM BOM = {{0xEF, 0xBB, 0xBF}};
    return BOM;
}

ZF_NAMESPACE_GLOBAL_END

