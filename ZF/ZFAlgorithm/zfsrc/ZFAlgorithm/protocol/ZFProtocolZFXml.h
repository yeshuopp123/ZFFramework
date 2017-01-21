/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFXml.h
 * @brief protocol for ZFXml
 */

#ifndef _ZFI_ZFProtocolZFXml_h_
#define _ZFI_ZFProtocolZFXml_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFAlgorithm/ZFXml.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFXml
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFXml)
public:
    /**
     * @brief parse xml document
     *
     * you must parse the DOM tree and save to document,
     * which including:
     * -  name for each xml item
     * -  value for each xml item
     * -  CDATA for text node
     */
    virtual ZFXmlItem xmlParse(ZF_IN const zfchar *src,
                               ZF_IN_OPT zfindex size = zfindexMax,
                               ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault) zfpurevirtual;
    /**
     * @brief parse xml document
     */
    virtual ZFXmlItem xmlParse(ZF_IN const ZFInputCallback &inputCallback,
                               ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault) zfpurevirtual;

    /**
     * @brief for impl to achieve memory pool logic
     *
     * to achieve memory pool logic, impl should:
     * -  supply memory pool token to hold state
     * -  use #xmlMemoryPool_xmlNameSet/#xmlMemoryPool_xmlValueSet to store data
     * -  implement this method to release reference
     */
    virtual void xmlMemoryPoolRelease(ZF_IN void *token, ZF_IN const zfchar *value)
    {
        // no pool logic by default
    }

public:
    /** @brief see #xmlMemoryPoolRelease */
    inline void xmlMemoryPool_xmlNameSet(ZF_IN ZFXmlItem &xmlItem,
                                         ZF_IN const zfchar *xmlName,
                                         ZF_IN void *token)
    {
        xmlItem._ZFP_ZFXml_xmlMemoryPool_xmlNameSet(xmlName, token);
    }
    /** @brief see #xmlMemoryPoolRelease */
    inline void xmlMemoryPool_xmlValueSet(ZF_IN ZFXmlItem &xmlItem,
                                          ZF_IN const zfchar *xmlValue,
                                          ZF_IN void *token)
    {
        xmlItem._ZFP_ZFXml_xmlMemoryPool_xmlValueSet(xmlValue, token);
    }
ZFPROTOCOL_INTERFACE_END(ZFXml)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFXml_h_

