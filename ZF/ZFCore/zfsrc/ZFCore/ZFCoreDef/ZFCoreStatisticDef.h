/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreStatisticDef.h
 * @brief utilities to log function's invoke time
 */

#ifndef _ZFI_ZFCoreStatisticDef_h_
#define _ZFI_ZFCoreStatisticDef_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

/** @brief see #ZFCoreStatistic::invokeCountGet */
extern ZF_ENV_EXPORT void invokeCountLog(ZF_IN const zfchar *key);
/** @brief see #ZFCoreStatistic::invokeCountGet */
extern ZF_ENV_EXPORT void invokeCountRemove(ZF_IN const zfchar *key);
/** @brief see #ZFCoreStatistic::invokeCountGet */
extern ZF_ENV_EXPORT void invokeCountRemoveAll(void);
/**
 * @brief get #ZFCoreStatistic::invokeCountLog's invoke count
 *
 * for debug performance use only, used to log a method's invoke count, usage:
 * @code
 *   void yourFuncToTest(void)
 *   {
 *       ZFCoreStatistic::invokeCountLog(yourKeyOrNull);
 *   }
 *   void YourTestFunc(void)
 *   {
 *       // this is yourFuncToTest's invoke times
 *       zfindex yourFuncCalledTime = ZFCoreStatistic::invokeCountGet(yourKeyOrNull);
 *       // if you want to reset it:
 *       ZFCoreStatistic::invokeCountReset(yourKeyOrNull);
 *   }
 * @endcode
 * @note this method is not thread-safe
 */
extern ZF_ENV_EXPORT zfindex invokeCountGet(ZF_IN const zfchar *key);

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreStatisticDef_h_

