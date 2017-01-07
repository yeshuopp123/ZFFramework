/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZF2048UISkin_h_
#define _ZFI_ZF2048UISkin_h_

#include "ZFUIKit.h"
#include "../ZF2048Core/ZF2048Types.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * resPath/block/block_bg.png
 * resPath/block/block_na.png
 * resPath/block/block_(2, 4, 8, ...).png
 * resPath/frame/frame_bg.xml
 */
extern void ZF2048UISkinLoad(ZF_IN const zfchar *resPath = zfText("ZF2048/skin/default"));

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048UISkin_h_

