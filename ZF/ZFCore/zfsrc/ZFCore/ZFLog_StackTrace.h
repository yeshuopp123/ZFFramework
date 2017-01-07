/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFLog_StackTrace.h
 * @brief utility to log stack
 */

#ifndef _ZFI_ZFLog_StackTrace_h_
#define _ZFI_ZFLog_StackTrace_h_

#include "ZFLog.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief true if #zfLogStackTrace and #zfLogCallerInfo is available
 */
extern ZF_ENV_EXPORT zfbool zfLogStackTraceAvailable(void);

/**
 * @brief get the function call stack
 *
 * output may be different depending on the OS and implementation
 */
extern ZF_ENV_EXPORT void zfLogStackTrace(ZF_OUT zfstring &ret,
                                          ZF_IN_OPT const zfchar *prefix = zfnull,
                                          ZF_IN_OPT zfindex ignoreLevel = 0,
                                          ZF_IN_OPT zfindex maxLevel = 20);
/** @brief see #zfLogStackTrace */
extern ZF_ENV_EXPORT zfstring zfLogStackTrace(ZF_IN_OPT const zfchar *prefix = zfnull,
                                              ZF_IN_OPT zfindex ignoreLevel = 0,
                                              ZF_IN_OPT zfindex maxLevel = 20);

/**
 * @brief get the caller's info
 *
 * see #zfLogStackTrace for how to make it available
 */
extern ZF_ENV_EXPORT void zfLogCallerInfo(ZF_OUT zfstring &ret, ZF_IN_OPT zfindex ignoreLevel = 0);
/** @brief see zfLogCallerInfo */
extern ZF_ENV_EXPORT zfstring zfLogCallerInfo(ZF_IN_OPT zfindex ignoreLevel = 0);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLog_StackTrace_h_

