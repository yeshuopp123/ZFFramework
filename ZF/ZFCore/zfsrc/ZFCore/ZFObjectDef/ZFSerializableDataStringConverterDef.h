/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSerializableDataStringConverterDef.h
 * @brief convert #ZFSerializableData from and to string
 */

#ifndef _ZFI_ZFSerializableDataStringConverterDef_h_
#define _ZFI_ZFSerializableDataStringConverterDef_h_

#include "ZFSerializableDataDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief convert serializable data from string
 */
extern ZF_ENV_EXPORT zfbool ZFSerializableDataFromString(ZF_OUT ZFSerializableData &serializableData,
                                                         ZF_IN const zfchar *encodedData,
                                                         ZF_IN_OPT zfindex encodedDataLen = zfindexMax,
                                                         ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
/**
 * @brief convert serializable data from string
 */
extern ZF_ENV_EXPORT ZFSerializableData ZFSerializableDataFromString(ZF_IN const zfchar *encodedData,
                                                                     ZF_IN_OPT zfindex encodedDataLen = zfindexMax,
                                                                     ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);

/**
 * @brief convert serializable data from input
 */
extern ZF_ENV_EXPORT zfbool ZFSerializableDataFromInput(ZF_OUT ZFSerializableData &serializableData,
                                                        ZF_IN const ZFInputCallback &input,
                                                        ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
/**
 * @brief convert serializable data from input
 */
extern ZF_ENV_EXPORT ZFSerializableData ZFSerializableDataFromInput(ZF_IN const ZFInputCallback &input,
                                                                    ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);

// ============================================================
/**
 * @brief convert serializable data to string
 */
extern ZF_ENV_EXPORT zfbool ZFSerializableDataToString(ZF_OUT zfstring &result,
                                                       ZF_IN const ZFSerializableData &serializableData,
                                                       ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
/**
 * @brief convert serializable data to string
 */
extern ZF_ENV_EXPORT zfstring ZFSerializableDataToString(ZF_IN const ZFSerializableData &serializableData,
                                                         ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);

/**
 * @brief convert serializable data to output
 */
extern ZF_ENV_EXPORT zfbool ZFSerializableDataToOutput(ZF_IN_OUT const ZFOutputCallback &output,
                                                       ZF_IN const ZFSerializableData &serializableData,
                                                       ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDataStringConverterDef_h_

