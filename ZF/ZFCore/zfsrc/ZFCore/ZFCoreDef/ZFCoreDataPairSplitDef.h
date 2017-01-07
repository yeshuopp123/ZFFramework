/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreDataPairSplitDef.h
 * @brief util to split data pairs from a string
 */

#ifndef _ZFI_ZFCoreDataPairSplitDef_h_
#define _ZFI_ZFCoreDataPairSplitDef_h_

#include "ZFCoreArrayDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief split data pair such as "(a, b, c)" from a string,
 *   return null if success or error position if failed
 *
 * return splited sub-string's index range\n
 * spaces and tabs would be ignored between elements and tokens,
 * spaces and tabs in elements would be reserved\n
 * only all format matched would result success,
 * missing or extra tokens would result fail\n
 * embeded data pair is allowed,
 * such as "(a, (b, c), d)" can be splited to "a", "(b, c)", "d"\n
 * \n
 * leftToken or rightToken can be disabled by setting to '\0'\n
 * if leftToken has been set, then rightToken must also be set\n
 * if rightToken not set, src can only be stopped by reach end
 */
extern ZF_ENV_EXPORT const zfchar *zfCoreDataPairSplitString(ZF_OUT ZFCoreArray<zfindexRange> &outData,
                                                             ZF_IN zfindex desiredCountOrIndexMax,
                                                             ZF_IN const zfchar *src,
                                                             ZF_IN_OPT zfindex srcLen = zfindexMax,
                                                             ZF_IN_OPT zfchar leftToken = '(',
                                                             ZF_IN_OPT zfchar rightToken = ')',
                                                             ZF_IN_OPT const zfstring &separatorTokens = zfText(","),
                                                             ZF_IN_OPT zfbool allowEmptyItem = zffalse);

/**
 * @brief split data to int array format, see #zfCoreDataPairSplitString
 */
template<typename T_int>
const zfchar *zfCoreDataPairSplitInt(ZF_OUT ZFCoreArray<T_int> &outData,
                                     ZF_IN zfindex desiredCountOrIndexMax,
                                     ZF_IN const zfchar *src,
                                     ZF_IN_OPT zfindex srcLen = zfindexMax,
                                     ZF_IN_OPT zfchar leftToken = '(',
                                     ZF_IN_OPT zfchar rightToken = ')',
                                     ZF_IN_OPT const zfstring &separatorTokens = zfText(","),
                                     ZF_IN_OPT zfbool allowEmptyItem = zffalse)
{
    ZFCoreArrayPOD<zfindexRange> splited;
    const zfchar *errPos = zfCoreDataPairSplitString(
        splited,
        desiredCountOrIndexMax,
        src, srcLen,
        leftToken, rightToken, separatorTokens, allowEmptyItem);
    if(errPos != zfnull)
    {
        return errPos;
    }
    for(zfindex i = 0; i < splited.count(); ++i)
    {
        T_int tmp = 0;
        errPos = zfsToInt(tmp, src + splited[i].start, splited[i].count);
        if(errPos != zfnull)
        {
            break;
        }
        outData.add(tmp);
    }
    return errPos;
}

/**
 * @brief split data to float array format, see #zfCoreDataPairSplitString
 */
template<typename T_float>
const zfchar *zfCoreDataPairSplitFloat(ZF_OUT ZFCoreArray<T_float> &outData,
                                       ZF_IN zfindex desiredCountOrIndexMax,
                                       ZF_IN const zfchar *src,
                                       ZF_IN_OPT zfindex srcLen = zfindexMax,
                                       ZF_IN_OPT zfchar leftToken = '(',
                                       ZF_IN_OPT zfchar rightToken = ')',
                                       ZF_IN_OPT const zfstring &separatorTokens = zfText(","),
                                       ZF_IN_OPT zfbool allowEmptyItem = zffalse)
{
    ZFCoreArrayPOD<zfindexRange> splited;
    const zfchar *errPos = zfCoreDataPairSplitString(
        splited,
        desiredCountOrIndexMax,
        src, srcLen,
        leftToken, rightToken, separatorTokens, allowEmptyItem);
    if(errPos != zfnull)
    {
        return errPos;
    }
    for(zfindex i = 0; i < splited.count(); ++i)
    {
        T_float tmp = 0;
        errPos = zfsToFloat(tmp, src + splited[i].start, splited[i].count);
        if(errPos != zfnull)
        {
            break;
        }
        outData.add(tmp);
    }
    return errPos;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreDataPairSplitDef_h_

