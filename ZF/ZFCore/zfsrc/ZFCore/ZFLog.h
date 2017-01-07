/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFLog.h
 * @brief thread-safe log utility
 */

#ifndef _ZFI_ZFLog_h_
#define _ZFI_ZFLog_h_

#include "ZFDebug.h"
#include "ZFMutex.h"
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT ZFMutex *_ZFP_ZFLogMutex(void);
/**
 * @brief mutex used by ZFLog,
 *   you may use this lock to prevent your log actions from being interrupted
 */
#define ZFLogMutex _ZFP_ZFLogMutex()

zfclassLikePOD ZF_ENV_EXPORT _ZFP_zfLogTAccess
{
public:
    ZFDebugCallback *debugCallback;

public:
    ZFDebugCallback &debugCallbackGet(void) const
    {
        return *debugCallback;
    }

public:
    _ZFP_zfLogTAccess(void);
    ~_ZFP_zfLogTAccess(void);
};
#define _ZFP_zfLogT() (_ZFP_zfLogTAccess().debugCallbackGet())

/**
 * @brief a null log callback that do nothing while appending data
 */
#define zfLogTNull zfDebugTNull

// ============================================================
/**
 * @brief a convenient debug header string
 *
 * typically looks like this (not ensured):\n
 *   "timestamp [file scope::function (line)] "
 */
#define ZFLOG_HEADER_STRING (zfstringWithFormat(zfText("%s %s "), \
    _ZFP_zfLogCurTimeString().cString(), \
    ZFDEBUG_HEADER_STRING) \
    .cString())

/**
 * @brief similar to zfDebug
 *
 * @code
 *   zfLog(format, ...);
 * @endcode
 */
#define zfLog(format, ...) (void)(_ZFP_zfLogT().addPrefix(ZFLOG_HEADER_STRING) \
    << zfstringWithFormat(format, ##__VA_ARGS__).cString())

/**
 * @brief similar to zfDebugT
 *
 * @code
 *   zfLogT() << something;
 * @endcode
 */
#define zfLogT() _ZFP_zfLogT().addPrefix(ZFLOG_HEADER_STRING)

/**
 * @brief log without ZFLog's header string
 * @see zfLog
 */
#define zfLogTrim(format, ...) (void)(_ZFP_zfLogT() \
    << zfstringWithFormat(format, ##__VA_ARGS__).cString())

/**
 * @brief log without ZFLog's header string
 * @see zfLogT
 */
#define zfLogTrimT() _ZFP_zfLogT()

// ============================================================
// other convenient method
extern ZF_ENV_EXPORT zfstring _ZFP_zfLogCurTimeString(void);
/**
 * @brief get a string contains current time with format HH:mm:ss.SSS
 */
#define zfLogCurTimeString() (_ZFP_zfLogCurTimeString().cString())

// ============================================================
/**
 * @brief see #ZFDebugLevel
 */
extern ZF_ENV_EXPORT void zfLogLevelSet(ZF_IN ZFDebugLevelEnum level);
/**
 * @brief see #ZFDebugLevel
 */
extern ZF_ENV_EXPORT ZFDebugLevelEnum zfLogLevelGet(void);
/**
 * @brief see #ZFDebugLevel
 */
extern ZF_ENV_EXPORT zfbool zfLogIsActive(ZF_IN ZFDebugLevelEnum level);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLog_h_

#include "ZFLog_StackTrace.h"

