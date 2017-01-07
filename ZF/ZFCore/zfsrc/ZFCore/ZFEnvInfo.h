/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFEnvInfo.h
 * @brief info of native environment
 */

#ifndef _ZFI_ZFEnvInfo_h_
#define _ZFI_ZFEnvInfo_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFEnvInfo::envSummaryCallbackRegister */
typedef void (*ZFEnvSummaryCallback)(ZF_IN_OUT zfstring &ret);

ZF_NAMESPACE_BEGIN(ZFEnvInfo)

// ============================================================
/**
 * @brief util method to get summary info
 */
extern ZF_ENV_EXPORT void envSummary(ZF_IN_OUT zfstring &ret);
/**
 * @brief name of native system, e.g. "iOS", empty if not available
 */
inline zfstring envSummary(void)
{
    zfstring ret;
    ZFEnvInfo::envSummary(ret);
    return ret;
}

/**
 * @brief register a callback to output custom env info, see #envSummary
 */
extern ZF_ENV_EXPORT void envSummaryCallbackRegister(ZF_IN const zfchar *name, ZF_IN ZFEnvSummaryCallback callback);
/** @brief see #envSummaryCallbackRegister */
extern ZF_ENV_EXPORT void envSummaryCallbackUnregister(ZF_IN const zfchar *name);

// ============================================================
/**
 * @brief name of native system, e.g. "iOS", empty if not available
 */
extern ZF_ENV_EXPORT void systemName(ZF_IN_OUT zfstring &ret);
/**
 * @brief name of native system, e.g. "iOS", empty if not available
 */
inline zfstring systemName(void)
{
    zfstring ret;
    ZFEnvInfo::systemName(ret);
    return ret;
}
/**
 * @brief version of native system, e.g. "9.0.1", empty if not available
 */
extern ZF_ENV_EXPORT void systemVersion(ZF_IN_OUT zfstring &ret);
/**
 * @brief version of native system, e.g. "9.0.1", empty if not available
 */
inline zfstring systemVersion(void)
{
    zfstring ret;
    ZFEnvInfo::systemVersion(ret);
    return ret;
}

// ============================================================
/**
 * @brief name of native framework, e.g. "Qt", empty if not available
 */
extern ZF_ENV_EXPORT void frameworkName(ZF_IN_OUT zfstring &ret);
/**
 * @brief name of native framework, e.g. "Qt", empty if not available
 */
inline zfstring frameworkName(void)
{
    zfstring ret;
    ZFEnvInfo::frameworkName(ret);
    return ret;
}
/**
 * @brief version of native framework, e.g. "9.0.1", empty if not available
 */
extern ZF_ENV_EXPORT void frameworkVersion(ZF_IN_OUT zfstring &ret);
/**
 * @brief version of native framework, e.g. "9.0.1", empty if not available
 */
inline zfstring frameworkVersion(void)
{
    zfstring ret;
    ZFEnvInfo::frameworkVersion(ret);
    return ret;
}

ZF_NAMESPACE_END(ZFEnvInfo)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnvInfo_h_

