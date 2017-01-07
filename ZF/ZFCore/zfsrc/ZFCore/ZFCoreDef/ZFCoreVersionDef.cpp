/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreVersionDef.h"
#include "ZFCoreDataPairSplitDef.h"
#include "ZFCoreStringUtilDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFVersionGet(ZF_OUT zfstring &ret,
                  ZF_IN const zfchar *version,
                  ZF_IN zfindex subVersionIndex)
{
    ZFCoreArrayPOD<zfindexRange> pos;
    if(*version != '\0' && zfCoreDataPairSplitString(pos, zfindexMax, version, zfindexMax, '\0', '\0', zfText(".")) != zfnull)
    {
        return ;
    }
    if(pos.count() <= subVersionIndex)
    {
        return ;
    }
    ret.append(version + pos[subVersionIndex].start, pos[subVersionIndex].count);
}
void ZFVersionGetInt(ZF_OUT zfuint &ret,
                     ZF_IN const zfchar *version,
                     ZF_IN zfindex subVersionIndex)
{
    ret = zfsToInt(ZFVersionGet(version, subVersionIndex).cString());
}
void ZFVersionSet(ZF_IN_OUT zfstring &version,
                  ZF_IN zfindex subVersionIndex,
                  ZF_IN const zfchar *subVersion,
                  ZF_IN_OPT const zfchar *emptySubVersion /* = zfText("0") */)
{
    ZFCoreArrayPOD<zfindexRange> pos;
    if(!version.isEmpty() && zfCoreDataPairSplitString(pos, zfindexMax, version, zfindexMax, '\0', '\0', zfText(".")) != zfnull)
    {
        return ;
    }

    if(pos.count() <= subVersionIndex)
    {
        for(zfindex i = pos.count(); i < subVersionIndex; ++i)
        {
            if(i != 0)
            {
                version += '.';
            }
            version += emptySubVersion;
        }
        version += '.';
        version += subVersion;
    }
    else
    {
        version.replace(pos[subVersionIndex].start, pos[subVersionIndex].count, subVersion);
    }
}
void ZFVersionSetInt(ZF_IN_OUT zfstring &version,
                     ZF_IN zfindex subVersionIndex,
                     ZF_IN zfuint subVersion,
                     ZF_IN_OPT const zfchar *emptySubVersion /* = zfText("0") */)
{
    ZFVersionSet(version, subVersionIndex, zfsFromInt(subVersion), emptySubVersion);
}

ZFCompareResult _ZFP_ZFComparerForVersion(ZF_IN const zfchar *const &v0, ZF_IN const zfchar *const &v1)
{
    zfuint64 n0 = 0;
    zfuint64 n1 = 0;
    if(zfsToInt(n0, v0, zfindexMax, zfHint("radix")36, zfHint("allowNegative")zffalse) != zfnull
       || zfsToInt(n1, v1, zfindexMax, zfHint("radix")36, zfHint("allowNegative")zffalse) != zfnull)
    {
        return ZFCompareUncomparable;
    }
    return ZFComparerDefault(n0, n1);
}
ZFCompareResult ZFVersionCompare(ZF_IN const zfchar *version0,
                                 ZF_IN const zfchar *version1,
                                 ZF_IN_OPT ZFComparer<const zfchar *>::Comparer subVersionComparer /* = ZFComparerForVersion */)
{
    ZFCoreArrayPOD<zfindexRange> pos0;
    ZFCoreArrayPOD<zfindexRange> pos1;
    if(zfCoreDataPairSplitString(pos0, zfindexMax, version0, zfindexMax, '\0', '\0', zfText(".")) != zfnull
       || zfCoreDataPairSplitString(pos1, zfindexMax, version1, zfindexMax, '\0', '\0', zfText(".")) != zfnull)
    {
        return ZFCompareUncomparable;
    }

    zfindex count = zfmMin(pos0.count(), pos1.count());
    for(zfindex i = 0; i < count; ++i)
    {
        ZFCompareResult cmp = subVersionComparer(
            zfstring(version0 + pos0[i].start, pos0[i].count).cString()
            ,
            zfstring(version1 + pos1[i].start, pos1[i].count).cString()
            );
        switch(cmp)
        {
            case ZFCompareUncomparable:
            case ZFCompareSmaller:
            case ZFCompareGreater:
                return cmp;
            case ZFCompareTheSame:
                continue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ZFCompareUncomparable;
        }
    }
    if(pos0.count() > pos1.count())
    {
        return ZFCompareGreater;
    }
    else if(pos0.count() < pos1.count())
    {
        return ZFCompareSmaller;
    }
    else
    {
        return ZFCompareTheSame;
    }
}

ZF_NAMESPACE_GLOBAL_END

