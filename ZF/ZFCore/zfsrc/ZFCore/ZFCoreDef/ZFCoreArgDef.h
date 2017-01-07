/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreArgDef.h
 * @brief arg utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreArgDef_h_
#define _ZFI_ZFCoreArgDef_h_

#include "ZFCoreArrayDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief split cmd line to separate item
 *
 * result would be appended to result instead of clear and set
 */
extern ZF_ENV_EXPORT void ZFCoreArgSplit(ZF_OUT ZFCoreArray<zfstring> &result,
                                         ZF_IN const zfchar *cmdLine);
/**
 * @brief split cmd line to separate item
 */
extern ZF_ENV_EXPORT ZFCoreArray<zfstring> ZFCoreArgSplit(ZF_IN const zfchar *cmdLine);
/**
 * @brief see #ZFCoreArgSplit
 *
 * result is appended to result instead of clear and set
 */
extern ZF_ENV_EXPORT void ZFCoreArgMerge(ZF_OUT zfstring &result,
                                         ZF_IN const ZFCoreArray<zfstring> &argList);
/**
 * @brief see #ZFCoreArgMerge
 */
extern ZF_ENV_EXPORT zfstring ZFCoreArgMerge(ZF_IN const ZFCoreArray<zfstring> &argList);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreArgDef_h_

