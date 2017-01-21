/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZF2048Types_h_
#define _ZFI_ZF2048Types_h_

#include "ZFCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassPOD ZF2048Point
{
public:
    zfindex x;
    zfindex y;
};
inline ZF2048Point ZF2048PointMake(ZF_IN zfindex x, ZF_IN zfindex y)
{
    ZF2048Point ret = {x, y};
    return ret;
}
extern const ZF2048Point ZF2048PointZero;
ZFCORE_POD_COMPARER_DECLARE(ZF2048Point)

/**
 * +-----x----->
 * | 0  4  8  12
 * y 1  5  9  13
 * | 2  6  10 14
 * v 3  7  11 15
 */
inline void ZF2048PointFromIndex(ZF_OUT ZF2048Point &pos,
                                 ZF_IN zfindex index,
                                 ZF_IN zfindex width,
                                 ZF_IN zfindex height)
{
    pos.x = (index / height);
    pos.y = (index % height);
}
inline ZF2048Point ZF2048PointFromIndex(ZF_IN zfindex index,
                                        ZF_IN zfindex width,
                                        ZF_IN zfindex height)
{
    return ZF2048PointMake(
        index / height,
        index % height);
}
inline zfindex ZF2048PointToIndex(ZF_IN const ZF2048Point &pos,
                                  ZF_IN zfindex width,
                                  ZF_IN zfindex height)
{
    return pos.x * height + pos.y;
}
inline zfindex ZF2048PointToIndex(ZF_IN zfindex posX, ZF_IN zfindex posY,
                                  ZF_IN zfindex width,
                                  ZF_IN zfindex height)
{
    return posX * height + posY;
}

ZFT_INT_STRONG(zft_zfint, ZF2048Value)
ZFPROPERTY_TYPE_DECLARE(ZF2048Value, ZF2048Value)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048Types_h_

