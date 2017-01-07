/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFMd5.h
 * @brief MD5 utility
 */

#ifndef _ZFI_ZFMd5_h_
#define _ZFI_ZFMd5_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief calculate MD5, return empty string if failed
 */
extern ZF_ENV_EXPORT void zfMd5Calc(ZF_IN_OUT zfstring &ret,
                                    ZF_IN const void *src,
                                    ZF_IN zfindex len,
                                    ZF_IN_OPT zfbool upperCase = zftrue);
/** @brief see #zfMd5Calc */
inline zfstring zfMd5Calc(ZF_IN const void *src,
                          ZF_IN zfindex len,
                          ZF_IN_OPT zfbool upperCase = zftrue)
{
    zfstring ret;
    zfMd5Calc(ret, src, len, upperCase);
    return ret;
}
/** @brief see #zfMd5Calc */
extern ZF_ENV_EXPORT void zfMd5Calc(ZF_IN_OUT zfstring &ret,
                                    ZF_IN const ZFInputCallback &callback,
                                    ZF_IN_OPT zfbool upperCase = zftrue);
/** @brief see #zfMd5Calc */
inline zfstring zfMd5Calc(ZF_IN const ZFInputCallback &callback,
                          ZF_IN_OPT zfbool upperCase = zftrue)
{
    zfstring ret;
    zfMd5Calc(ret, callback, upperCase);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMd5_h_

