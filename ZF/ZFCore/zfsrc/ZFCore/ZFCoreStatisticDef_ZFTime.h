/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreStatisticDef_ZFTime.h
 * @brief util to log function's invoke time
 */

#ifndef _ZFI_ZFCoreStatisticDef_ZFTime_h_
#define _ZFI_ZFCoreStatisticDef_ZFTime_h_

#include "ZFTime.h"
ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

// ============================================================
/**
 * @brief util to log function's invoke time
 */
extern ZF_ENV_EXPORT void invokeTimeLogBegin(ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeLogEnd(ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeRemove(ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeRemoveAll(void);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT zfindex invokeTimeGetInvokeCount(ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT zftimet invokeTimeGetAverageTime(ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT zftimet invokeTimeGetTotalTime(ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeGetSummary(ZF_OUT zfstring &ret, ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
inline zfstring invokeTimeGetSummary(ZF_IN_OPT const zfchar *key = zfnull)
{
    zfstring ret;
    ZFCoreStatistic::invokeTimeGetSummary(ret, key);
    return ret;
}

zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFCoreStatisticInvokeTimeLogger
{
public:
    _ZFP_ZFCoreStatisticInvokeTimeLogger(ZF_IN const zfchar *key)
    : key(key)
    {
        ZFCoreStatistic::invokeTimeLogBegin(key);
    }
    ~_ZFP_ZFCoreStatisticInvokeTimeLogger(void)
    {
        ZFCoreStatistic::invokeTimeLogEnd(key);
    }
private:
    zfstring key;
};
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
#define ZFCoreStatisticInvokeTimeLogger(key) \
    ZFCoreStatistic::_ZFP_ZFCoreStatisticInvokeTimeLogger ZFUniqueName(ZFCoreStatisticInvokeTimeLogger_v)(key); \
    ZFUNUSED(ZFUniqueName(ZFCoreStatisticInvokeTimeLogger_v))

// ============================================================
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeAccurateLogBegin(ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeAccurateLogEnd(ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeAccurateRemove(ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeAccurateRemoveAll(void);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT zfindex invokeTimeAccurateGetInvokeCount(ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT ZFTimeValue invokeTimeAccurateGetAverageTime(ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT ZFTimeValue invokeTimeAccurateGetTotalTime(ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeAccurateGetSummary(ZF_OUT zfstring &ret, ZF_IN_OPT const zfchar *key = zfnull);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
inline zfstring invokeTimeAccurateGetSummary(ZF_IN_OPT const zfchar *key = zfnull)
{
    zfstring ret;
    ZFCoreStatistic::invokeTimeAccurateGetSummary(ret, key);
    return ret;
}

zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFCoreStatisticInvokeTimeAccurateLogger
{
public:
    _ZFP_ZFCoreStatisticInvokeTimeAccurateLogger(ZF_IN const zfchar *key)
    : key(key)
    {
        ZFCoreStatistic::invokeTimeAccurateLogBegin(key);
    }
    ~_ZFP_ZFCoreStatisticInvokeTimeAccurateLogger(void)
    {
        ZFCoreStatistic::invokeTimeAccurateLogEnd(key);
    }
private:
    zfstring key;
};
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
#define ZFCoreStatisticInvokeTimeAccurateLogger(key) \
    ZFCoreStatistic::_ZFP_ZFCoreStatisticInvokeTimeAccurateLogger ZFUniqueName(ZFCoreStatisticInvokeTimeAccurateLogger_v)(key); \
    ZFUNUSED(ZFUniqueName(ZFCoreStatisticInvokeTimeAccurateLogger_v))

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreStatisticDef_ZFTime_h_

