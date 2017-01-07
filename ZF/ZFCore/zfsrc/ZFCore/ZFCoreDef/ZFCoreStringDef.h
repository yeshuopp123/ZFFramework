/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreStringDef.h
 * @brief tricks to wrap std::string
 */

#ifndef _ZFI_ZFCoreStringDef_h_
#define _ZFI_ZFCoreStringDef_h_

#include "ZFCoreStringDef_impl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFCoreString_impl */
#ifndef _ZFT_ZFCoreStringA
    typedef ZFCoreStringA_impl ZFCoreStringA;
#else
    typedef _ZFT_ZFCoreStringA ZFCoreStringA;
#endif

/** @brief see #ZFCoreString_impl */
#ifndef _ZFT_ZFCoreStringW
    typedef ZFCoreStringW_impl ZFCoreStringW;
#else
    typedef _ZFT_ZFCoreStringW ZFCoreStringW;
#endif

// ============================================================
#if 1 // you must change these if change zfchar's type
/**
 * @brief string types used in ZFFramework
 */
#ifndef _ZFT_ZFCoreString
    typedef ZFCoreStringA ZFCoreString;
#else
    typedef _ZFT_ZFCoreString ZFCoreString;
#endif
#endif // you must change these if change zfchar's type
// ============================================================

/** @cond ZFPrivateDoc */
inline zfbool operator == (ZF_IN ZFCoreStringA const &s0, ZF_IN ZFCoreStringA const s1)
{
    return (s0.compare(s1) == 0);
}
inline zfbool operator != (ZF_IN ZFCoreStringA const &s0, ZF_IN ZFCoreStringA const s1)
{
    return (s0.compare(s1) != 0);
}
// ============================================================
inline zfbool operator == (ZF_IN ZFCoreStringW const &s0, ZF_IN ZFCoreStringW const s1)
{
    return (s0.compare(s1) == 0);
}
inline zfbool operator != (ZF_IN ZFCoreStringW const &s0, ZF_IN ZFCoreStringW const s1)
{
    return (s0.compare(s1) != 0);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreStringDef_h_

