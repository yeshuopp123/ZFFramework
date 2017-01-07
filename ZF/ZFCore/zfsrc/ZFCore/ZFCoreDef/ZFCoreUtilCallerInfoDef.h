/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreUtilCallerInfoDef.h
 * @brief caller info utils
 */
#ifndef _ZFI_ZFCoreUtilCallerInfoDef_h_
#define _ZFI_ZFCoreUtilCallerInfoDef_h_

#include "ZFCoreUtilCharTypeWrapperDef.h"
#include "ZFCoreStringUtilDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

template<typename T_Char>
ZF_ENV_EXPORT void _ZFP_ZF_CALLER_PATH_TO_NAME(ZF_IN_OUT typename zfwrapCharToString<T_Char>::StringType &ret,
                                               ZF_IN const T_Char *filePath)
{
    if(filePath == zfnull)
    {
        return ;
    }
    const T_Char *p = filePath + zfslenT(filePath);
    while(p > filePath)
    {
        --p;
        if(*p == '/' || *p == '\\')
        {
            ++p;
            break;
        }
    }
    ret += p;
}
template<typename T_Char>
ZF_ENV_EXPORT typename zfwrapCharToString<T_Char>::StringType _ZFP_ZF_CALLER_PATH_TO_NAME(ZF_IN const T_Char *filePath)
{
    typename zfwrapCharToString<T_Char>::StringType ret;
    _ZFP_ZF_CALLER_PATH_TO_NAME(ret, filePath);
    return ret;
}
/**
 * @brief convert file path to file name
 *
 * used by ZF_CALLER_FILE_NAME, you may use this function directly if you want\n
 * the return value would be deleted after line end,
 * you should not save the value for future use,
 * store it in a std::string if necessary
 */
#define ZF_CALLER_PATH_TO_NAME(path) (_ZFP_ZF_CALLER_PATH_TO_NAME(path).cString())
/**
 * @brief see #ZF_CALLER_PATH_TO_NAME
 */
#define ZF_CALLER_PATH_TO_NAME_REF(ret, path) _ZFP_ZF_CALLER_PATH_TO_NAME(ret, path)
/**
 * @brief similar to __FILE__ in C++ world, except that don't include full path
 *
 * note that this macro contains string convertion, take care of performance if necessary\n
 * the return value would be deleted after line end,
 * you should not save the value for future use,
 * store it in a zfstring if necessary
 */
#define ZF_CALLER_FILE_NAME ZF_CALLER_PATH_TO_NAME(__FILE__)

/**
 * @brief same as __FILE__ in C++ world
 *
 * ensured in zfcharA format, you should convert to zfchar type if necessary
 */
#define ZF_CALLER_FILE __FILE__
/**
 * @brief same as __FUNCTION__ in C++ world
 *
 * ensured in zfcharA format, you should convert to zfchar type if necessary
 */
#define ZF_CALLER_FUNCTION __FUNCTION__
/**
 * @brief same as __LINE__ in C++ world
 */
#define ZF_CALLER_LINE __LINE__

template<typename T_Char>
ZF_ENV_EXPORT void _ZFP_CALLER_INFO(ZF_IN_OUT typename zfwrapCharToString<T_Char>::StringType &ret,
                                    ZF_IN const T_Char *callerFile,
                                    ZF_IN const T_Char *callerFunction,
                                    ZF_IN zfindex callerLine)
{
    ret += '[';
    if(callerFile != zfnull)
    {
        ZF_CALLER_PATH_TO_NAME_REF(ret, callerFile);
        ret += ' ';
    }
    if(callerFunction != zfnull)
    {
        ret += callerFunction;
        ret += ' ';
    }
    if(callerLine != zfindexMax)
    {
        ret += '(';
        zfsFromIntT(ret, callerLine);
        ret += ')';
    }
    ret += ']';
}
template<typename T_Char>
ZF_ENV_EXPORT typename zfwrapCharToString<T_Char>::StringType _ZFP_CALLER_INFO(ZF_IN const T_Char *callerFile,
                                                                               ZF_IN const T_Char *callerFunction,
                                                                               ZF_IN zfindex callerLine)
{
    typename zfwrapCharToString<T_Char>::StringType ret;
    _ZFP_CALLER_INFO(ret, callerFile, callerFunction, callerLine);
    return ret;
}
/**
 * @brief convenient macro to create a info looks like "[callerFileName callerFunction (callerLine)]"
 */
#define ZF_CALLER_INFO() \
    ZF_CALLER_INFO_DETAIL(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/**
 * @brief see #ZF_CALLER_INFO
 */
#define ZF_CALLER_INFO_REF(ret) \
    ZF_CALLER_INFO_DETAIL_REF(ret, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/**
 * @brief see #ZF_CALLER_INFO
 */
#define ZF_CALLER_INFO_DETAIL(callerFile, callerFunction, callerLine) \
    (_ZFP_CALLER_INFO(callerFile, callerFunction, callerLine).cString())
/**
 * @brief see #ZF_CALLER_INFO
 */
#define ZF_CALLER_INFO_DETAIL_REF(ret, callerFile, callerFunction, callerLine) \
    _ZFP_CALLER_INFO(ret, callerFile, callerFunction, callerLine)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreUtilCallerInfoDef_h_

