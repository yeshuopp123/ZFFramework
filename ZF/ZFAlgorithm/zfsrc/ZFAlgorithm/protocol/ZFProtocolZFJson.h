/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFJson.h
 * @brief protocol for ZFJson
 */

#ifndef _ZFI_ZFProtocolZFJson_h_
#define _ZFI_ZFProtocolZFJson_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFAlgorithm/ZFJson.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFJson
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFJson)
public:
    /**
     * @brief parse json
     */
    virtual ZFJsonItem jsonParse(ZF_IN const zfchar *src,
                                 ZF_IN_OPT zfindex size = zfindexMax,
                                 ZF_IN_OPT const ZFJsonParseFlags &flags = ZFJsonParseFlagsDefault) zfpurevirtual;
    /**
     * @brief parse json
     */
    virtual ZFJsonItem jsonParse(ZF_IN const ZFInputCallback &inputCallback,
                                 ZF_IN_OPT const ZFJsonParseFlags &flags = ZFJsonParseFlagsDefault) zfpurevirtual;

    /**
     * @brief for impl to achieve memory pool logic
     *
     * to achieve memory pool logic, impl should:
     * -  supply memory pool token to hold state
     * -  use #jsonMemoryPool_jsonValueSet/#jsonMemoryPool_jsonItemSet to store data
     * -  implement this method to release reference
     */
    virtual void jsonMemoryPoolRelease(ZF_IN void *token, ZF_IN const zfchar *value)
    {
        // no pool logic by default
    }

public:
    /** @brief see #jsonMemoryPoolRelease */
    inline void jsonMemoryPool_jsonValueSet(ZF_IN ZFJsonItem &jsonItem,
                                            ZF_IN const zfchar *jsonValue,
                                            ZF_IN void *token)
    {
        jsonItem._ZFP_ZFJson_jsonMemoryPool_jsonValueSet(jsonValue, token);
    }
    /** @brief see #jsonMemoryPoolRelease */
    inline void jsonMemoryPool_jsonItemSet(ZF_IN ZFJsonItem &jsonObject,
                                           ZF_IN const zfchar *jsonKey,
                                           ZF_IN void *token,
                                           ZF_IN const ZFJsonItem &jsonItem)
    {
        jsonObject._ZFP_ZFJson_jsonMemoryPool_jsonItemSet(jsonKey, token, jsonItem);
    }
ZFPROTOCOL_INTERFACE_END(ZFJson)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFJson_h_

