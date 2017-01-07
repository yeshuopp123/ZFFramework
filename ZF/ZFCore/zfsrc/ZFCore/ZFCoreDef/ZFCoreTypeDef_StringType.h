/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreTypeDef_StringType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_StringType_h_
#define _ZFI_ZFCoreTypeDef_StringType_h_

#include "ZFCoreTypeDef_CoreType.h"
#include "ZFCoreTypeDef_CharType.h"
#include "ZFCoreStringDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFCoreString_impl */
#ifndef _ZFT_zfstringA
    typedef ZFCoreStringA zfstringA;
#else
    typedef _ZFT_zfstringA zfstringA;
#endif

/** @brief see #ZFCoreString_impl */
#ifndef _ZFT_zfstringW
    typedef ZFCoreStringW zfstringW;
#else
    typedef _ZFT_zfstringW zfstringW;
#endif

// ============================================================
#if 1 // you must change these if change zfchar's type

/** @brief see #ZFCoreString_impl */
#ifndef _ZFT_zfstring
    typedef ZFCoreString zfstring;
#else
    typedef _ZFT_zfstring zfstring;
#endif

#endif // you must change these if change zfchar's type
// ============================================================

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_StringType_h_

