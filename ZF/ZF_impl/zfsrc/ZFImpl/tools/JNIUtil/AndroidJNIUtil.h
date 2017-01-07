/**
 * @file AndroidJNIUtil.h
 * @brief Android's JNI utilities
 */

#ifndef _AndroidJNIUtil_h_
#define _AndroidJNIUtil_h_

// need -llog if use logs in Android

#include "JNIUtil.h"
#if (defined(ANDROID) || defined(__ANDROID__))
#include <jni.h>
#include <android/log.h>
#include <string>
namespace AndroidJNIUtil {

/** @brief verbose log level */
#define AndroidLogLevelV ANDROID_LOG_VERBOSE
/** @brief debug log level */
#define AndroidLogLevelD ANDROID_LOG_DEBUG
/** @brief info log level */
#define AndroidLogLevelI ANDROID_LOG_INFO
/** @brief warning log level */
#define AndroidLogLevelW ANDROID_LOG_WARN
/** @brief error log level */
#define AndroidLogLevelE ANDROID_LOG_ERROR
/** @brief error log level */
#define AndroidLogLevelA ANDROID_LOG_FATAL
/** @brief default log level, could be changed by adding to pre-compiler */
#ifndef AndroidLogLevelDefault
    #define AndroidLogLevelDefault AndroidLogLevelV
#endif

/** @cond ZFPrivateDoc */
extern ZF_ENV_EXPORT std::string &_AndroidLogTagDefault(void);
/** @endcond */
/**
 * @brief default log tag, could be changed at runtime
 */
#define AndroidLogTagDefault (AndroidJNIUtil::_AndroidLogTagDefault().c_str())
/**
 * @brief change the default log tag
 */
extern ZF_ENV_EXPORT void AndroidLogTagDefaultSet(const char *s);

/**
 * @brief log with level, tag and fmt
 */
#define AndroidLogDetail(level, tag, fmt, ...) \
    ((void)__android_log_print(level, tag, fmt, ##__VA_ARGS__))

/**
 * @brief log with default level, default tag and fmt
 */
#define AndroidLog(fmt, ...) \
    AndroidLogDetail(AndroidLogLevelDefault, AndroidLogTagDefault, fmt, ##__VA_ARGS__)

}
#endif // #if (defined(ANDROID) || defined(__ANDROID__))
#endif // #ifndef _AndroidJNIUtil_h_
