/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFBase64.h
 * @brief base64 encoder and decoder
 */

#ifndef _ZFI_ZFBase64_h_
#define _ZFI_ZFBase64_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief char table for ZFBase64
 */
#define ZFBase64TableDefault zfText("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/")
/**
 * @brief pad token for ZFBase64
 */
#define ZFBase64PadDefault '='
/**
 * @brief standard line break position for ZFBase64
 */
#define ZFBase64LineBreakPosStandard 76
/**
 * @brief no line break for ZFBase64
 */
#define ZFBase64LineBreakPosNone zfindexMax

// ============================================================
// encode
/**
 * @brief calculate required size to store result,
 *   ensured big enough but not ensure exactly the same
 *
 * return zfindexMax if error
 */
extern ZF_ENV_EXPORT zfindex ZFBase64EncodeCalcSize(ZF_IN zfindex srcLen,
                                                    ZF_IN_OPT zfindex lineBreakPos = ZFBase64LineBreakPosNone);

/**
 * @brief encode base64
 */
extern ZF_ENV_EXPORT zfbool ZFBase64Encode(ZF_OUT zfchar *buf,
                                           ZF_IN const void *src,
                                           ZF_IN_OPT zfindex srcLen = zfindexMax,
                                           ZF_OUT_OPT zfindex *outResultSize = zfnull,
                                           ZF_IN_OPT const zfchar *table = ZFBase64TableDefault,
                                           ZF_IN_OPT zfchar pad = ZFBase64PadDefault,
                                           ZF_IN_OPT zfindex lineBreakPos = ZFBase64LineBreakPosNone);
/**
 * @brief encode base64, return byte size written even if error occurred
 */
extern ZF_ENV_EXPORT zfbool ZFBase64Encode(ZF_IN_OUT const ZFOutputCallback &outputCallback,
                                           ZF_IN const ZFInputCallback &inputCallback,
                                           ZF_OUT_OPT zfindex *outResultSize = zfnull,
                                           ZF_IN_OPT const zfchar *table = ZFBase64TableDefault,
                                           ZF_IN_OPT zfchar pad = ZFBase64PadDefault,
                                           ZF_IN_OPT zfindex lineBreakPos = ZFBase64LineBreakPosNone);

// ============================================================
// decode
/**
 * @brief calculate required size to store result,
 *   ensured big enough but not ensure exactly the same
 *
 * return zfindexMax if error
 */
extern ZF_ENV_EXPORT zfindex ZFBase64DecodeCalcSize(ZF_IN zfindex srcLen,
                                                    ZF_IN_OPT zfindex lineBreakPos = ZFBase64LineBreakPosNone);
/**
 * @brief encode base64, return byte size written even if error occurred
 *
 * extra space, tab, '\\r', '\\n' is allowed,
 * missing pad is allowed
 */
extern ZF_ENV_EXPORT zfbool ZFBase64Decode(ZF_OUT void *buf,
                                           ZF_IN const zfchar *src,
                                           ZF_IN_OPT zfindex srcLen = zfindexMax,
                                           ZF_OUT_OPT zfindex *outResultSize = zfnull,
                                           ZF_IN_OPT const zfchar *table = ZFBase64TableDefault,
                                           ZF_IN_OPT zfchar pad = ZFBase64PadDefault);
/**
 * @brief encode base64, return byte size written even if error occurred
 */
extern ZF_ENV_EXPORT zfbool ZFBase64Decode(ZF_IN_OUT const ZFOutputCallback &outputCallback,
                                           ZF_IN const ZFInputCallback &inputCallback,
                                           ZF_OUT_OPT zfindex *outResultSize = zfnull,
                                           ZF_IN_OPT const zfchar *table = ZFBase64TableDefault,
                                           ZF_IN_OPT zfchar pad = ZFBase64PadDefault);

// ============================================================
/**
 * @brief util class to process base64
 */
zfclass ZF_ENV_EXPORT ZFBase64 : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(ZFBase64, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

public:
    /** @brief see #ZFBase64TableDefault */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfstring, base64Table, ZFBase64TableDefault)
    /** @brief see #ZFBase64PadDefault */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfchar, base64Pad, ZFBase64PadDefault)
    /** @brief see #ZFBase64LineBreakPosNone */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, base64LineBreakPos, ZFBase64LineBreakPosNone)

public:
    /** @brief see #ZFBase64EncodeCalcSize */
    virtual inline zfindex base64EncodeCalcSize(ZF_IN zfindex srcLen)
    {
        return ZFBase64EncodeCalcSize(srcLen, this->base64LineBreakPos());
    }
    /** @brief see #ZFBase64Encode */
    virtual inline zfbool base64Encode(ZF_OUT zfchar *buf,
                                       ZF_IN const void *src,
                                       ZF_IN_OPT zfindex srcLen = zfindexMax,
                                       ZF_OUT_OPT zfindex *outResultSize = zfnull)
    {
        return ZFBase64Encode(buf, src, srcLen, outResultSize, this->base64Table(), this->base64Pad(), this->base64LineBreakPos());
    }
    /** @brief see #ZFBase64Encode */
    virtual inline zfbool base64Encode(ZF_IN_OUT const ZFOutputCallback &output,
                                       ZF_IN const ZFInputCallback &input,
                                       ZF_OUT_OPT zfindex *outResultSize = zfnull)
    {
        return ZFBase64Encode(output, input, outResultSize, this->base64Table(), this->base64Pad(), this->base64LineBreakPos());
    }
public:
    /** @brief see #ZFBase64DecodeCalcSize */
    virtual inline zfindex base64DecodeCalcSize(ZF_IN zfindex srcLen)
    {
        return ZFBase64DecodeCalcSize(srcLen, this->base64LineBreakPos());
    }
    /** @brief see #ZFBase64Decode */
    virtual inline zfbool base64Decode(ZF_OUT void *buf,
                                       ZF_IN const zfchar *src,
                                       ZF_IN_OPT zfindex srcLen = zfindexMax,
                                       ZF_OUT_OPT zfindex *outResultSize = zfnull)
    {
        return ZFBase64Decode(buf, src, srcLen, outResultSize, this->base64Table(), this->base64Pad());
    }
    /** @brief see #ZFBase64Decode */
    virtual inline zfbool base64Decode(ZF_IN_OUT const ZFOutputCallback &output,
                                       ZF_IN const ZFInputCallback &input,
                                       ZF_OUT_OPT zfindex *outResultSize = zfnull)
    {
        return ZFBase64Decode(output, input, outResultSize, this->base64Table(), this->base64Pad());
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFBase64_h_

