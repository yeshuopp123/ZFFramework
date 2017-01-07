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
 * @brief init value for continous calculation, see #zfCrc32Calc
 */
#define ZFCrc32InitValue 0x0L
/**
 * @brief invalid value for CRC32
 */
#define ZFCrc32Invalid 0xFFFFFFFFL

/**
 * @brief calculate CRC32, return #ZFCrc32Invalid if failed
 *
 * prevResult is used for continous calculation for performance,
 * you may separate big buffer to small ones,
 * and calculate separately
 */
extern ZF_ENV_EXPORT zfuint32 zfCrc32Calc(ZF_IN const void *src,
                                          ZF_IN zfindex len,
                                          ZF_IN_OPT zfuint32 prevResult = ZFCrc32InitValue);
/**
 * @brief calculate CRC32, return #ZFCrc32Invalid if failed
 *
 * prevResult is used for continous calculation for performance,
 * you may separate big buffer to small ones,
 * and calculate separately
 */
extern ZF_ENV_EXPORT zfuint32 zfCrc32Calc(ZF_IN const ZFInputCallback &callback,
                                          ZF_IN_OPT zfuint32 prevResult = ZFCrc32InitValue);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCrc32_h_

