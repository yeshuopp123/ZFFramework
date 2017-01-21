/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSerializableDataIO.h
 * @brief util to load serializable from different io
 */

#ifndef _ZFI_ZFSerializableDataIO_h_
#define _ZFI_ZFSerializableDataIO_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief uitl method to load serializable data from input according to ioType
 *
 * for custom types, use #ZFSERIALIZABLEDATAIO_DEFINE to register,
 * recommended to use file extension as ioType,
 * recommended to define ioType as macro which named "ZFSerializableDataIOType_YourType"
 */
extern ZF_ENV_EXPORT zfbool ZFSerializableDataFromIO(ZF_OUT ZFSerializableData &serializableData,
                                                     ZF_IN const zfchar *ioType,
                                                     ZF_IN const ZFInputCallback &input,
                                                     ZF_OUT_OPT zfstring *outErrorHint = zfnull);
/** @brief see #ZFSerializableDataFromIO */
inline ZFSerializableData ZFSerializableDataFromIO(ZF_IN const zfchar *ioType,
                                                   ZF_IN const ZFInputCallback &input,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull)
{
    ZFSerializableData ret;
    if(ZFSerializableDataFromIO(ret, ioType, input, outErrorHint))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}
/** @brief see #ZFSerializableDataFromIO */
extern ZF_ENV_EXPORT zfbool ZFSerializableDataToIO(ZF_IN_OUT const ZFOutputCallback &output,
                                                   ZF_IN const zfchar *ioType,
                                                   ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull);

// ============================================================
/** @brief see #ZFSERIALIZABLEDATAIO_DEFINE */
typedef zfbool (*ZFSerializableDataFromIOCallback)(ZF_OUT ZFSerializableData &serializableData,
                                                   ZF_IN const ZFInputCallback &input,
                                                   ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */);
/** @brief see #ZFSERIALIZABLEDATAIO_DEFINE */
typedef zfbool (*ZFSerializableDataToIOCallback)(ZF_IN_OUT const ZFOutputCallback &output,
                                                 ZF_IN const ZFSerializableData &serializableData,
                                                 ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */);

/**
 * @brief register callbacks to load serializable data from input or to output,
 *   see #ZFSerializableDataFromIO
 *
 * recommended to use #ZFSERIALIZABLEDATAIO_DEFINE to register
 */
extern ZF_ENV_EXPORT void ZFSerializableDataIORegister(ZF_IN const zfchar *ioType,
                                                       ZF_IN ZFSerializableDataFromIOCallback fromCallback,
                                                       ZF_IN ZFSerializableDataToIOCallback toCallback);
/** @brief see #ZFSERIALIZABLEDATAIO_DEFINE */
extern ZF_ENV_EXPORT void ZFSerializableDataIOUnregister(ZF_IN const zfchar *ioType);

/** @brief see #ZFSERIALIZABLEDATAIO_DEFINE */
extern ZF_ENV_EXPORT ZFSerializableDataFromIOCallback ZFSerializableDataFromIOCallbackGet(ZF_IN const zfchar *ioType);
/** @brief see #ZFSERIALIZABLEDATAIO_DEFINE */
extern ZF_ENV_EXPORT ZFSerializableDataToIOCallback ZFSerializableDataToIOCallbackGet(ZF_IN const zfchar *ioType);

/**
 * @brief util macro to register custom callback by #ZFSerializableDataIORegister
 *
 * see #ZFSerializableDataFromIOCallback/#ZFSerializableDataToIOCallback for proto type of the callback action
 */
#define ZFSERIALIZABLEDATAIO_DEFINE(ioType_, fromCallbackAction, toCallbackAction) \
    static zfbool _ZFP_ZFSerializableDataFromIOCallback_##ioType_(ZF_OUT ZFSerializableData &serializableData, \
                                                                  ZF_IN const ZFInputCallback &input, \
                                                                  ZF_OUT_OPT zfstring *outErrorHint = zfnull) \
    { \
        fromCallbackAction \
    } \
    static zfbool _ZFP_ZFSerializableDataToIOCallback_##ioType_(ZF_IN_OUT const ZFOutputCallback &output, \
                                                                ZF_IN const ZFSerializableData &serializableData, \
                                                                ZF_OUT_OPT zfstring *outErrorHint = zfnull) \
    { \
        toCallbackAction \
    } \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFSerializableDataIOTypeRegister_##ioType_, ZFLevelZFFrameworkNormal) \
    { \
        ZFSerializableDataIORegister(ZFM_TOSTRING(ioType_), \
            _ZFP_ZFSerializableDataFromIOCallback_##ioType_, \
            _ZFP_ZFSerializableDataToIOCallback_##ioType_); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFSerializableDataIOTypeRegister_##ioType_) \
    { \
        ZFSerializableDataIOUnregister(ZFM_TOSTRING(ioType_)); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFSerializableDataIOTypeRegister_##ioType_)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDataIO_h_

