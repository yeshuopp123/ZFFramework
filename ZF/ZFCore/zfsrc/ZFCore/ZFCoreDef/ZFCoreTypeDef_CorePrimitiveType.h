/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreTypeDef_CorePrimitiveType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_CorePrimitiveType_h_
#define _ZFI_ZFCoreTypeDef_CorePrimitiveType_h_

#include "ZFNamespaceDef.h"
#include "ZFCoreEnvDef.h"
#include <stddef.h>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// must ensure these types differs from each other
/** @cond ZFPrivateDoc */
#define _ZFT_zftrue true
#define _ZFT_zffalse false
typedef bool                    _ZFT_zfbool;
typedef char                    _ZFT_zfcharA;
typedef wchar_t                 _ZFT_zfcharW;
typedef unsigned char           _ZFT_zfbyte;
typedef int                     _ZFT_zfint;
typedef unsigned int            _ZFT_zfuint;
typedef unsigned long int       _ZFT_zfindex;
typedef float                   _ZFT_zffloat;
/** @endcond */
// must ensure these types differs from each other
// ============================================================

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_CorePrimitiveType_h_

