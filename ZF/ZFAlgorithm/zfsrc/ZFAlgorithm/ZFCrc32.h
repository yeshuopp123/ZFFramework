/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCrc32.h
 * @brief CRC32 utility
 */

#ifndef _ZFI_ZFCrc32_h_
#define _ZFI_ZFCrc32_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief value to store crc32
 */
ZFT_INT_STRONG_WITH_BIT(zft_zfuint32, ZFCrc32Value)

/**
 * @brief init value for continous calculation, see #zfCrc32Calc
 */
#define ZFCrc32ValueZero ((ZFCrc32Value)0x0)
/**
 * @brief invalid value for CRC32
 */
#define ZFCrc32ValueInvalid ((ZFCrc32Value)0xFFFFFFFF)

/**
 * @brief calculate CRC32, return #ZFCrc32ValueInvalid if failed
 *
 * prevResult is used for continous calculation for performance,
 * you may separate big buffer to small ones,
 * and calculate separately
 */
extern ZF_ENV_EXPORT ZFCrc32Value zfCrc32Calc(ZF_IN const void *src,
                                              ZF_IN zfindex len,
                                              ZF_IN_OPT ZFCrc32Value prevResult = ZFCrc32ValueZero);
/**
 * @brief calculate CRC32, return #ZFCrc32ValueInvalid if failed
 *
 * prevResult is used for continous calculation for performance,
 * you may separate big buffer to small ones,
 * and calculate separately
 */
extern ZF_ENV_EXPORT ZFCrc32Value zfCrc32Calc(ZF_IN const ZFInputCallback &callback,
                                              ZF_IN_OPT ZFCrc32Value prevResult = ZFCrc32ValueZero);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCrc32_h_

