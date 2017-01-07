/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFAlgorithm.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFXml.h"

#include "ZFImpl/_repo/pugixml/pugixml.hpp"

ZF_NAMESPACE_GLOBAL_BEGIN
ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)

zfclassNotPOD _ZFP_ZFXmlImpl_default_MemoryPoolHolder
{
public:
    zfindex refCount;
    ZFBuffer buf;
    pugi::xml_document implXmlDoc;
public:
    _ZFP_ZFXmlImpl_default_MemoryPoolHolder(void)
    : refCount(0)
    , buf()
    , implXmlDoc()
    {
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFXmlImpl_default, ZFXml, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("pugixml"))
public:
    virtual ZFXmlItem xmlParse(ZF_IN const zfchar *src,
                               ZF_IN_OPT zfindex size = zfindexMax,
                               ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault)
    {
        ZFBuffer buf;
        buf.bufferCopy(src, size);
        return this->xmlParse(buf, flags);
    }
    virtual ZFXmlItem xmlParse(ZF_IN const ZFInputCallback &inputCallback,
                               ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault)
    {
        ZFBuffer buf = ZFInputCallbackReadToBuffer(inputCallback);
        return this->xmlParse(buf, flags);
    }
    virtual void xmlMemoryPoolRelease(ZF_IN void *token, ZF_IN const zfchar *value)
    {
        _ZFP_ZFXmlImpl_default_MemoryPoolHolder *docHolder = (_ZFP_ZFXmlImpl_default_MemoryPoolHolder *)token;
        --(docHolder->refCount);
        if(docHolder->refCount == 0)
        {
            zfdelete(docHolder);
        }
    }
private:
    ZFXmlItem xmlParse(ZF_IN_OUT ZFBuffer &buf,
                       ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault)
    {
        if(buf.buffer() == zfnull)
        {
            return ZFXmlItem();
        }
        _ZFP_ZFXmlImpl_default_MemoryPoolHolder *docHolder = zfnew(_ZFP_ZFXmlImpl_default_MemoryPoolHolder);
        docHolder->buf = buf;
        pugi::xml_parse_result implResult = docHolder->implXmlDoc.load_buffer_inplace(buf.buffer(), buf.bufferSize(), pugi::parse_full);
        if(implResult.status != pugi::status_ok)
        {
            return ZFXmlItem();
        }
        ZFXmlItem doc(ZFXmlType::e_XmlDocument);
        this->translateChildren(docHolder->implXmlDoc, doc, docHolder);
        if(docHolder->refCount == 0)
        {
            zfdelete(docHolder);
        }
        return doc;
    }
    void translateAttribute(ZF_IN const pugi::xml_node &implXmlItem,
                            ZF_OUT ZFXmlItem &zfXmlItem,
                            ZF_IN _ZFP_ZFXmlImpl_default_MemoryPoolHolder *docHolder)
    {
        pugi::xml_attribute implXmlAttribute = implXmlItem.first_attribute();
        while(implXmlAttribute)
        {
            ZFXmlItem zfXmlAttribute(ZFXmlType::e_XmlAttribute);
            ++(docHolder->refCount);
            this->xmlMemoryPool_xmlNameSet(zfXmlAttribute, implXmlAttribute.name(), docHolder);
            ++(docHolder->refCount);
            this->xmlMemoryPool_xmlValueSet(zfXmlAttribute, implXmlAttribute.value(), docHolder);
            zfXmlItem.xmlAttributeAdd(zfXmlAttribute);

            implXmlAttribute = implXmlAttribute.next_attribute();
        }
    }
    void translateChildren(ZF_IN const pugi::xml_node &implXmlItem,
                           ZF_OUT ZFXmlItem &zfXmlItem,
                           ZF_IN _ZFP_ZFXmlImpl_default_MemoryPoolHolder *docHolder)
    {
        pugi::xml_node implXmlChild = implXmlItem.first_child();
        while(implXmlChild != zfnull)
        {
            switch(implXmlChild.type())
            {
                case pugi::node_document:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
                case pugi::node_element:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlElement);
                    this->translateAttribute(implXmlChild, zfXmlChild, docHolder);
                    this->translateChildren(implXmlChild, zfXmlChild, docHolder);

                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlNameSet(zfXmlChild, implXmlChild.name(), docHolder);
                    zfXmlItem.xmlChildAdd(zfXmlChild);
                    break;
                }
                case pugi::node_pcdata:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlText);

                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlNameSet(zfXmlChild, implXmlChild.name(), docHolder);
                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlValueSet(zfXmlChild, implXmlChild.value(), docHolder);
                    zfXmlItem.xmlChildAdd(zfXmlChild);
                    break;
                }
                case pugi::node_cdata:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlText);
                    zfXmlChild.xmlTextCDATASet(zftrue);

                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlNameSet(zfXmlChild, implXmlChild.name(), docHolder);
                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlValueSet(zfXmlChild, implXmlChild.value(), docHolder);
                    zfXmlItem.xmlChildAdd(zfXmlChild);
                    break;
                }
                case pugi::node_comment:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlComment);

                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlNameSet(zfXmlChild, implXmlChild.name(), docHolder);
                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlValueSet(zfXmlChild, implXmlChild.value(), docHolder);
                    zfXmlItem.xmlChildAdd(zfXmlChild);
                    break;
                }
                case pugi::node_declaration:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlDeclaration);
                    this->translateAttribute(implXmlChild, zfXmlChild, docHolder);

                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlNameSet(zfXmlChild, implXmlChild.name(), docHolder);
                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlValueSet(zfXmlChild, implXmlChild.value(), docHolder);
                    zfXmlItem.xmlChildAdd(zfXmlChild);
                    break;
                }
                case pugi::node_doctype:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlDOCTYPE);

                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlNameSet(zfXmlChild, implXmlChild.name(), docHolder);
                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlValueSet(zfXmlChild, implXmlChild.value(), docHolder);
                    zfXmlItem.xmlChildAdd(zfXmlChild);
                    break;
                }
                case pugi::node_pi:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlProcessingInstruction);

                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlNameSet(zfXmlChild, implXmlChild.name(), docHolder);
                    ++(docHolder->refCount);
                    this->xmlMemoryPool_xmlValueSet(zfXmlChild, implXmlChild.value(), docHolder);
                    zfXmlItem.xmlChildAdd(zfXmlChild);
                    break;
                }
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }

            implXmlChild = implXmlChild.next_sibling();
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFXmlImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFXmlImpl_default)

ZF_NAMESPACE_GLOBAL_END

