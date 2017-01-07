/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZF2048AppAutoMoveAction_h_
#define _ZFI_ZF2048AppAutoMoveAction_h_

#include "ZFCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_BEGIN(ZF2048AppAutoMoveAction)
    ZFENUM_VALUE(Left)
    ZFENUM_VALUE(Top)
    ZFENUM_VALUE(Right)
    ZFENUM_VALUE(Bottom)
    ZFENUM_VALUE(Random)
ZFENUM_SEPARATOR(ZF2048AppAutoMoveAction)
    ZFENUM_VALUE_REGISTER(Left)
    ZFENUM_VALUE_REGISTER(Top)
    ZFENUM_VALUE_REGISTER(Right)
    ZFENUM_VALUE_REGISTER(Bottom)
    ZFENUM_VALUE_REGISTER(Random)
ZFENUM_END(ZF2048AppAutoMoveAction)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048AppAutoMoveAction_h_

