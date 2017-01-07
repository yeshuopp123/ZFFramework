/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreLogDef.h
 * @brief internal log utility
 *
 * used internally, you should use zfLog or zfDebug instead
 */

#ifndef _ZFI_ZFCoreLogDef_h_
#define _ZFI_ZFCoreLogDef_h_

#include "ZFCoreUtilDef.h"
#include "ZFCoreStringUtilDef.h"
#include <assert.h>
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief same as assert defined for future use
 *
 * active only if ZF_ENV_DEBUG is zftrue
 * @see zfCoreAssert
 */
#if ZF_ENV_DEBUG
    #define zfassert(b) assert(b)
#else
    #define zfassert(b) ((void)0)
#endif

/**
 * @brief assert at compile time
 */
#define ZFASSERT_STATIC(cond, name) typedef char assert_failed_##name[(cond) ? 1 : -1];

/**
 * @brief output callback for zfCoreLog
 */
typedef zfindex (*ZFCoreLogOutputCallbackType)(ZF_IN const zfcharA *s);
extern ZF_ENV_EXPORT zfindex _ZFP_ZFCoreLogOutputCallbackDefault(ZF_IN const zfcharA *s);
/**
 * @brief default output callback for zfCoreLog
 */
#define ZFCoreLogOutputCallbackDefault _ZFP_ZFCoreLogOutputCallbackDefault

/**
 * @brief set the output callback for zfCoreLog
 * @warning within the callback, you must not access any other ZFFramework's module,
 *   otherwise dead lock may be occurred
 */
extern ZF_ENV_EXPORT void zfCoreLogOutputCallbackSet(ZF_IN_OPT ZFCoreLogOutputCallbackType callback = ZFCoreLogOutputCallbackDefault);
/**
 * @brief get the output callback for zfCoreLog
 */
extern ZF_ENV_EXPORT ZFCoreLogOutputCallbackType zfCoreLogOutputCallbackGet(void);

extern ZF_ENV_EXPORT void _ZFP_zfCoreLog(ZF_IN const zfcharA *callerFile,
                                         ZF_IN const zfcharA *callerFunction,
                                         ZF_IN zfindex callerLine,
                                         ZF_IN zfbool isAutoEndl,
                                         ZF_IN const zfcharA *format,
                                         ...);
/**
 * @brief log utility used internally
 *
 * quite similar to printf, except that this function would automatically add header info to head,
 * which includes file name, function name and file line info,
 * and an extra '\\n' to tail\n
 * for all the format supported, please refer to #zfstringAppend
 * @warning this function is for internal use,
 *   usually to output critical error messages,
 *   you should not use other log method in your app
 * @warning this method is not thread safe
 * @see zfCoreLogOutputCallbackSet, zfCoreCriticalError
 */
#define zfCoreLog(format, ...) _ZFP_zfCoreLog(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, \
    zftrue, format, ##__VA_ARGS__)
/**
 * @brief same as #zfCoreLog except that no '\\n' is added to tail, see #zfCoreLog
 */
#define zfCoreLogNoEndl(format, ...) _ZFP_zfCoreLog(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, \
    zffalse, format, ##__VA_ARGS__)

/**
 * @brief log without header info, see #zfCoreLog
 */
#define zfCoreLogTrim(format, ...) _ZFP_zfCoreLog(zfnull, zfnull, -1, zftrue, format, ##__VA_ARGS__)
/**
 * @brief log without header info or '\\n' to tail, see #zfCoreLog
 */
#define zfCoreLogTrimNoEndl(format, ...) _ZFP_zfCoreLog(zfnull, zfnull, -1, zffalse, format, ##__VA_ARGS__)

/**
 * @brief log with custom header info, see #zfCoreLog
 */
#define zfCoreLogDetail(callerFile, callerFunction, callerLine, format, ...) _ZFP_zfCoreLog(callerFile, callerFunction, callerLine, zftrue, format, ##__VA_ARGS__)
/**
 * @brief log with custom header info, see #zfCoreLog
 */
#define zfCoreLogDetailNoEndl(callerFile, callerFunction, callerLine, format, ...) _ZFP_zfCoreLog(callerFile, callerFunction, callerLine, zffalse, format, ##__VA_ARGS__)

/**
 * @brief callback which would be called if zfCoreCriticalError is called
 */
typedef void (*ZFCoreCriticalErrorCallback)(ZF_IN const zfchar *callerFile,
                                            ZF_IN const zfchar *callerFunction,
                                            ZF_IN zfindex callerLine);

/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZF_ENV_EXPORT void zfCoreCriticalErrorPrepareCallbackAdd(ZF_IN ZFCoreCriticalErrorCallback callback);
/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZF_ENV_EXPORT void zfCoreCriticalErrorPrepareCallbackRemove(ZF_IN ZFCoreCriticalErrorCallback callback);

/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZF_ENV_EXPORT void zfCoreCriticalErrorCallbackAdd(ZF_IN ZFCoreCriticalErrorCallback callback);
/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZF_ENV_EXPORT void zfCoreCriticalErrorCallbackRemove(ZF_IN ZFCoreCriticalErrorCallback callback);

extern ZF_ENV_EXPORT void _ZFP_zfCoreCriticalErrorPrepare(ZF_IN const zfcharA *callerFile,
                                                          ZF_IN const zfcharA *callerFunction,
                                                          ZF_IN zfindex callerLine);
extern ZF_ENV_EXPORT void _ZFP_zfCoreCriticalError(ZF_IN const zfcharA *callerFile,
                                                   ZF_IN const zfcharA *callerFunction,
                                                   ZF_IN zfindex callerLine);
/**
 * @brief ready to output critical error, see #zfCoreCriticalError
 *
 * usage:
 * @code
 *   zfCoreCriticalErrorPrepare();
 *   zfCoreLog(xxx);
 *   zfCoreCriticalError();
 * @endcode
 * this is useful to redirect #zfCoreLog by #zfCoreLogOutputCallbackSet,
 * to save critical error messages to proper location
 */
#define zfCoreCriticalErrorPrepare() \
    _ZFP_zfCoreCriticalErrorPrepare(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/**
 * @brief used when error occurred, to terminate the application
 *
 * @warning this function is for internal use,
 *   calling this method would cause app to be terminated
 */
#define zfCoreCriticalError() \
    _ZFP_zfCoreCriticalError(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreLogDef_h_

#include "ZFCoreLogDef_CommonLog.h"

