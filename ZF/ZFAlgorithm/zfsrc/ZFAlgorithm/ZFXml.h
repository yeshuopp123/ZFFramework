/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFXml.h
 * @brief DOM XML processing
 */

#ifndef _ZFI_ZFXml_h_
#define _ZFI_ZFXml_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/*
 * all types:
 *     XmlNull
 *     XmlElement
 *     XmlAttribute
 *     XmlText
 *     XmlComment
 *     XmlDocument
 *     XmlDeclaration
 *     XmlDOCTYPE
 *     XmlProcessingInstruction
 * has attribute:
 *     XmlElement, XmlDeclaration
 * has child:
 *     XmlElement, XmlDocument
 *
 * a typical xml to test:
    "<?xml type=\"declaration\"?>"
    "<!DOCTYPE TYPE \"DOCTYPE\">"
    "<?pi ProcessingInstruction?>"
    "<element0 attribute0=\"attribute value0\">"
        "<!-- here is a comment -->"
        "<element1 attribute0=\"attribute value0\" />"
        "<element2 attribute0=\"attribute value0\" attribute1=\"attribute value1\""
        " attribute2=\"attribute value2\" attribute3=\"attribute value3\" />"
        "<element3>text0</element3>"
        "<element4><![CDATA[cdata text]]></element4>"
        "<element5>text with escaped chars: &lt; &gt; &amp; &apos; &quot;</element5>"
    "</element0>"
 */
zfclassFwd ZFXmlItem;

// ============================================================
// ZFXmlType
/**
 * @brief xml item type
 */
ZFENUM_BEGIN(ZFXmlType)
    ZFENUM_VALUE(XmlNull)
    ZFENUM_VALUE(XmlElement)
    ZFENUM_VALUE(XmlAttribute)
    ZFENUM_VALUE(XmlText)
    ZFENUM_VALUE(XmlComment)
    ZFENUM_VALUE(XmlDocument)
    ZFENUM_VALUE(XmlDeclaration)
    ZFENUM_VALUE(XmlDOCTYPE)
    ZFENUM_VALUE(XmlProcessingInstruction)
ZFENUM_SEPARATOR(ZFXmlType)
    ZFENUM_VALUE_REGISTER(XmlNull)
    ZFENUM_VALUE_REGISTER(XmlElement)
    ZFENUM_VALUE_REGISTER(XmlAttribute)
    ZFENUM_VALUE_REGISTER(XmlText)
    ZFENUM_VALUE_REGISTER(XmlComment)
    ZFENUM_VALUE_REGISTER(XmlDocument)
    ZFENUM_VALUE_REGISTER(XmlDeclaration)
    ZFENUM_VALUE_REGISTER(XmlDOCTYPE)
    ZFENUM_VALUE_REGISTER(XmlProcessingInstruction)
ZFENUM_END(ZFXmlType)

// ============================================================
/**
 * @brief flags used when parsing document
 */
zfclassPOD ZF_ENV_EXPORT ZFXmlParseFlags
{
};
/**
 * @brief default parse flags
 */
extern ZF_ENV_EXPORT const ZFXmlParseFlags ZFXmlParseFlagsDefault;

// ============================================================
// ZFXmlVisitCallback
/**
 * @brief visit type for #ZFXmlItem::xmlVisit
 */
ZFENUM_BEGIN(ZFXmlVisitType)
    /**
     * @brief entering visit the xml item
     */
    ZFENUM_VALUE(Enter)
    /**
     * @brief exiting visit the xml item
     *
     * for items that have attribute,
     * visit exit would occurred after all attributes visited
     */
    ZFENUM_VALUE(Exit)
    /**
     * @brief exiting visit the xml element after all children visited
     *
     * for xml element only, and the order should like this:\n
     *   1. enter element\n
     *   2.   enter attributes\n
     *   3.   exit attributes\n
     *   4. exit element\n
     *   5.   enter children\n
     *   6.   exit children\n
     *   7. exitChildren element
     */
    ZFENUM_VALUE(ExitChildren)
ZFENUM_SEPARATOR(ZFXmlVisitType)
    ZFENUM_VALUE_REGISTER(Enter)
    ZFENUM_VALUE_REGISTER(Exit)
    ZFENUM_VALUE_REGISTER(ExitChildren)
ZFENUM_END(ZFXmlVisitType)
/**
 * @brief visit data when visiting a xml item, used in #ZFXmlVisitCallback
 */
zfclassLikePOD ZF_ENV_EXPORT ZFXmlVisitData
{
public:
    /**
     * @brief the xml item to visit
     */
    const ZFXmlItem *xmlItem;
    /**
     * @brief visit type
     */
    ZFXmlVisitTypeEnum xmlVisitType;
    /**
     * @brief depth of the xml item, 0 for root item
     *
     * attributes and children has (parent's depth + 1)'s depth
     */
    zfindex depth;
    /**
     * @brief sibling index
     */
    zfindex siblingIndex;
};
/**
 * @brief callback to go through all nodes and attributes in a xml node
 *
 * proto type:
 * -  zfbool xmlVisit(const ZFXmlVisitData &);
 *
 * param:
 * -  (const ZFXmlVisitData &) visit data
 *
 * return:
 * -  (zfbool) if visitEnter, shows that whether we should continue to visit the item's attributes and children\n
 *   if not visitEnter, shows that whether we should continue to visit next siblings
 */
typedef ZFCallbackT<zfbool, const ZFXmlVisitData &> ZFXmlVisitCallback;

// ============================================================
/**
 * @brief token used to output a xml document
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFXmlOutputToken
{
public:
    zfstring xmlNewLineToken; /**< @brief "\n" by default */
    zfstring xmlIndentToken; /**< @brief "    " by default */

    zfstring xmlDeclarationTagLeft; /**< @brief "<?xml" by default */
    zfstring xmlDeclarationTagRight; /**< @brief "?>" by default */
    zfstring xmlDOCTYPETagLeft; /**< @brief "<!DOCTYPE" by default */
    zfstring xmlDOCTYPETagRight; /**< @brief ">" by default */
    zfstring xmlPITagLeft; /**< @brief "<?" by default */
    zfstring xmlPITagRight; /**< @brief "?>" by default */
    zfstring xmlElementBeginTagLeft; /**< @brief "<" by default */
    zfstring xmlElementBeginTagRight; /**< @brief ">" by default */
    zfstring xmlElementEndTagLeft; /**< @brief "</" by default */
    zfstring xmlElementEndTagRight; /**< @brief ">" by default */
    zfstring xmlElementSingleTagLeft; /**< @brief "<" by default */
    zfstring xmlElementSingleTagRight; /**< @brief "/>" by default */
    zfstring xmlAttributeEqualTag; /**< @brief "=" by default */
    zfstring xmlAttributeQuoteTagLeft; /**< @brief "\"" by default */
    zfstring xmlAttributeQuoteTagRight; /**< @brief "\"" by default */
    zfstring xmlAttributeSingleQuoteTagLeft; /**< @brief "\'" by default */
    zfstring xmlAttributeSingleQuoteTagRight; /**< @brief "\'" by default */
    zfstring xmlTextCDATATagLeft; /**< @brief "<![CDATA[" by default */
    zfstring xmlTextCDATATagRight; /**< @brief "]]>" by default */
    zfstring xmlCommentTagLeft; /**< @brief "<!--" by default */
    zfstring xmlCommentTagRight; /**< @brief "-->" by default */

public:
    ZFXmlOutputToken(void)
    : xmlNewLineToken(zfText("\n"))
    , xmlIndentToken(zfText("    "))
    , xmlDeclarationTagLeft(zfText("<?xml"))
    , xmlDeclarationTagRight(zfText("?>"))
    , xmlDOCTYPETagLeft(zfText("<!DOCTYPE"))
    , xmlDOCTYPETagRight(zfText(">"))
    , xmlPITagLeft(zfText("<?"))
    , xmlPITagRight(zfText("?>"))
    , xmlElementBeginTagLeft(zfText("<"))
    , xmlElementBeginTagRight(zfText(">"))
    , xmlElementEndTagLeft(zfText("</"))
    , xmlElementEndTagRight(zfText(">"))
    , xmlElementSingleTagLeft(zfText("<"))
    , xmlElementSingleTagRight(zfText("/>"))
    , xmlAttributeEqualTag(zfText("="))
    , xmlAttributeQuoteTagLeft(zfText("\""))
    , xmlAttributeQuoteTagRight(zfText("\""))
    , xmlAttributeSingleQuoteTagLeft(zfText("\'"))
    , xmlAttributeSingleQuoteTagRight(zfText("\'"))
    , xmlTextCDATATagLeft(zfText("<![CDATA["))
    , xmlTextCDATATagRight(zfText("]]>"))
    , xmlCommentTagLeft(zfText("<!--"))
    , xmlCommentTagRight(zfText("-->"))
    {
    }
};

/**
 * @brief output flags for output a xml
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFXmlOutputFlags
{
public:
    /**
     * @brief xml token to use to output a xml document
     */
    ZFXmlOutputToken xmlToken;

    /**
     * @brief strings added to head of each new line, empty by default
     */
    zfstring xmlGlobalLineBeginToken;

    /**
     * @brief add new line before entering a element if the element isn't single line, zffalse by default
     */
    zfbool xmlElementAddNewLineAtHeadIfNotSingleLine;
    /**
     * @brief how many attributes before add new line, 3 by default
     *
     * e.g. if zfindexMax, never add new line,
     * if 0, add before every attributes,
     * if 1, add before every attributes except first one
     */
    zfindex xmlElementAttributeCountBeforeAddNewLine;
    /**
     * @brief trim "<element></element>" to "<element/>" if no children, zffalse by default
     */
    zfbool xmlElementTrimTagIfNoChildren;
    /**
     * @brief whether put element's end tag in a same line
     *   if no child element or only text children, zftrue by default
     */
    zfbool xmlElementEndTagAtSameLineIfNoChildElement;

    /**
     * @brief use single quote for attribute's value, zffalse by default
     */
    zfbool xmlAttributeUseSingleQuote;

public:
    ZFXmlOutputFlags(void)
    : xmlToken()
    , xmlGlobalLineBeginToken()
    , xmlElementAddNewLineAtHeadIfNotSingleLine(zffalse)
    , xmlElementAttributeCountBeforeAddNewLine(3)
    , xmlElementTrimTagIfNoChildren(zftrue)
    , xmlElementEndTagAtSameLineIfNoChildElement(zftrue)
    , xmlAttributeUseSingleQuote(zffalse)
    {
    }
};
extern ZF_ENV_EXPORT const ZFXmlOutputFlags &_ZFP_ZFXmlOutputFlagsDefault(void);
extern ZF_ENV_EXPORT const ZFXmlOutputFlags &_ZFP_ZFXmlOutputFlagsTrim(void);
extern ZF_ENV_EXPORT const ZFXmlOutputFlags &_ZFP_ZFXmlOutputFlagsDetailed(void);
/**
 * @brief default xml output flags
 */
#define ZFXmlOutputFlagsDefault _ZFP_ZFXmlOutputFlagsDefault()
/**
 * @brief xml output flags with trim format
 */
#define ZFXmlOutputFlagsTrim _ZFP_ZFXmlOutputFlagsTrim()
/**
 * @brief xml output flags with detailed format
 */
#define ZFXmlOutputFlagsDetailed _ZFP_ZFXmlOutputFlagsDetailed()

extern ZF_ENV_EXPORT ZFXmlVisitCallback _ZFP_ZFXmlVisitCallbackForOutput(
    ZF_IN const zfcharA *ownerFilePath,
    ZF_IN const zfcharA *ownerFunctionName,
    ZF_IN zfindex ownerFileLine,
    ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault,
    ZF_IN_OPT const ZFXmlOutputFlags &flags = ZFXmlOutputFlagsDefault);
/**
 * @brief create a ZFXmlVisitCallback to output xml DOM tree
 *
 * takes 0 ~ 2 params:
 * -  (const ZFOutputCallback &) the output callback to use, #ZFOutputCallbackDefault by default
 * -  (const ZFXmlOutputFlags &) output flags to use, #ZFXmlOutputFlagsDefault by default
 */
#define ZFXmlVisitCallbackForOutput(...) \
    _ZFP_ZFXmlVisitCallbackForOutput(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, ##__VA_ARGS__)

extern ZF_ENV_EXPORT ZFXmlVisitCallback &_ZFP_ZFXmlVisitCallbackDefault(void);
/**
 * @brief default visit callback for visiting a xml item,
 *   you can change it at runtime
 */
#define ZFXmlVisitCallbackDefault _ZFP_ZFXmlVisitCallbackDefault()

// ============================================================
// ZFXmlItem
zfclassFwd _ZFP_ZFXmlItemPrivate;
/**
 * @brief xml item that holds the xml DOM
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFXmlItem
{
    // ============================================================
public:
    /** @brief construct a null item */
    ZFXmlItem(void);
    /** @brief construct with specified type */
    ZFXmlItem(ZF_IN ZFXmlTypeEnum xmlType);
    /** @brief retain from other item */
    ZFXmlItem(ZF_IN const ZFXmlItem &ref);
    virtual ~ZFXmlItem(void);

    /** @cond ZFPrivateDoc */
    ZFXmlItem &operator = (ZF_IN const ZFXmlItem &ref);
    zfbool operator == (ZF_IN const ZFXmlItem &ref) const;
    zfbool operator != (ZF_IN const ZFXmlItem &ref) const {return !(this->operator == (ref));}
    /** @endcond */
private:
    zfbool operator == (ZF_IN const zfchar *ref) const;
    zfbool operator != (ZF_IN const zfchar *ref) const;
    zfbool operator == (ZF_IN zfint ref) const;
    zfbool operator != (ZF_IN zfint ref) const;

    // ============================================================
public:
    /** @brief see #ZFObject::objectInfoT */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #ZFObject::objectInfo */
    inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /**
     * @brief get current retain count
     */
    zfindex objectRetainCount(void) const;

    // ============================================================
public:
    /**
     * @brief type of this item
     */
    ZFXmlTypeEnum xmlType(void) const;
    /**
     * @brief true if #xmlType is #ZFXmlType::e_XmlNull
     */
    inline zfbool xmlIsNull(void) const
    {
        return (this->xmlType() == ZFXmlType::e_XmlNull);
    }

    /**
     * @brief access parent of this node
     */
    ZFXmlItem xmlParent(void) const;

public:
    /**
     * @brief name of the node
     *
     * what name stands for depends on the node type:
     * -  document: always null
     * -  declaration: always null
     * -  DOCTYPE: always null
     * -  processing instruction: pi's name
     * -  element: tag name
     * -  attribute: attribute name
     * -  text: always null
     * -  comment: always null
     *
     * note that the name is used for searching to access xml nodes
     */
    void xmlNameSet(ZF_IN const zfchar *name);
    /**
     * @brief see #xmlNameSet
     */
    const zfchar *xmlName(void) const;

    /**
     * @brief value of the node
     *
     * what value stands for depends on the node type:
     * -  document: always null
     * -  declaration: always null
     * -  DOCTYPE: the DOCTYPE without tags
     * -  processing instruction: the processing instruction's content
     * -  element: always null
     * -  attribute: attribute value without quotes
     * -  text: the text itself without tags if CDATA
     * -  comment: the comment itself without tags
     */
    void xmlValueSet(ZF_IN const zfchar *value);
    /**
     * @brief see #xmlValueSet
     */
    const zfchar *xmlValue(void) const;

    zffinal void _ZFP_ZFXml_xmlMemoryPool_xmlNameSet(ZF_IN const zfchar *xmlName, ZF_IN void *token);
    zffinal void _ZFP_ZFXml_xmlMemoryPool_xmlValueSet(ZF_IN const zfchar *xmlValue, ZF_IN void *token);

    // ============================================================
public:
    /**
     * @brief visit the xml item and all of its attributes and children
     *
     * you may use this method to print a xml item:
     * @code
     *   xmlItem->xmlVisit(ZFXmlVisitCallbackForOutput());
     * @endcode
     * or declare your own #ZFXmlVisitCallback to go through all items in xml tree
     */
    void xmlVisit(ZF_IN const ZFXmlVisitCallback &callback) const;

    // ============================================================
public:
    /**
     * @brief return a copy of this node, keep all attribute but ignoring all children
     *
     * the newly created node has no parent
     */
    ZFXmlItem xmlClone(void) const;
    /**
     * @brief return a copy of this node as well as all of its children
     *
     * the newly created tree has no parent
     */
    ZFXmlItem xmlCloneTree(void) const;

    // ============================================================
public:
    /**
     * @brief add attribute before beforeThis or add to tail if beforeThis is null
     */
    void xmlAttributeAdd(ZF_IN const ZFXmlItem &addThis,
                         ZF_IN_OPT const ZFXmlItem *beforeThis = zfnull);
    /**
     * @brief util method to add attribute, do nothing if key or value is null
     */
    void xmlAttributeAdd(ZF_IN const zfchar *key,
                         ZF_IN const zfchar *value,
                         ZF_IN_OPT const ZFXmlItem *beforeThis = zfnull);
    /**
     * @brief remove attribute or do nothing if not in this node
     */
    void xmlAttributeRemove(ZF_IN const ZFXmlItem &removeThis);
    /**
     * @brief remove attribute with name
     */
    void xmlAttributeRemove(ZF_IN const zfchar *name);
    /**
     * @brief remove all attribute
     */
    void xmlAttributeRemoveAll(void);

    /**
     * @brief get attribute with name
     */
    ZFXmlItem xmlAttribute(ZF_IN const zfchar *name) const;
    /**
     * @brief get attribute value with name
     */
    const zfchar *xmlAttributeValue(ZF_IN const zfchar *name) const;

    /**
     * @brief get first attribute
     */
    ZFXmlItem xmlAttributeFirst(void) const;
    /**
     * @brief get last attribute
     */
    ZFXmlItem xmlAttributeLast(void) const;

    /**
     * @brief get next attribute
     */
    ZFXmlItem xmlAttributeNext(void) const;
    /**
     * @brief get prev attribute
     */
    ZFXmlItem xmlAttributePrev(void) const;

    // ============================================================
public:
    /**
     * @brief sort all attribute
     */
    void xmlAttributeSort(void);
    /**
     * @brief sort all attribute as well as all descendants' attribute
     */
    void xmlAttributeSortRecursively(void);

    // ============================================================
public:
    /**
     * @brief add child before beforeThis or add to tail if beforeThis is null
     */
    void xmlChildAdd(ZF_IN const ZFXmlItem &addThis,
                     ZF_IN_OPT const ZFXmlItem *beforeThis = zfnull);
    /**
     * @brief remove child or do nothing if not in this node
     */
    void xmlChildRemove(ZF_IN const ZFXmlItem &removeThis);
    /**
     * @brief remove all child
     */
    void xmlChildRemoveAll(void);

    /**
     * @brief get first child with name after afterThis or null if none
     */
    ZFXmlItem xmlChildFirst(ZF_IN_OPT const zfchar *name = zfnull,
                            ZF_IN_OPT const ZFXmlItem *afterThis = zfnull) const;
    /**
     * @brief get last child with name before beforeThis or null if none
     */
    ZFXmlItem xmlChildLast(ZF_IN_OPT const zfchar *name = zfnull,
                           ZF_IN_OPT const ZFXmlItem *beforeThis = zfnull) const;
    /**
     * @brief util function to #xmlChildFirst, to get first child element
     */
    ZFXmlItem xmlChildElementFirst(ZF_IN_OPT const zfchar *name = zfnull,
                                   ZF_IN_OPT const ZFXmlItem *afterThis = zfnull) const;
    /**
     * @brief util function to #xmlChildLast, to get last child element
     */
    ZFXmlItem xmlChildElementLast(ZF_IN_OPT const zfchar *name = zfnull,
                                  ZF_IN_OPT const ZFXmlItem *beforeThis = zfnull) const;

    /**
     * @brief get next sibling with name or null if none
     */
    ZFXmlItem xmlSiblingNext(ZF_IN const zfchar *name = zfnull) const;
    /**
     * @brief get prev sibling with name or null if none
     */
    ZFXmlItem xmlSiblingPrev(ZF_IN const zfchar *name = zfnull) const;

    /**
     * @brief util function to #xmlSiblingNext, to get next sibling element
     */
    ZFXmlItem xmlSiblingElementNext(ZF_IN const zfchar *name = zfnull) const;
    /**
     * @brief util function to #xmlSiblingPrev, to get prev sibling element
     */
    ZFXmlItem xmlSiblingElementPrev(ZF_IN const zfchar *name = zfnull) const;

    // ============================================================
public:
    /**
     * @brief CDATA or not
     */
    void xmlTextCDATASet(ZF_IN zfbool xmlTextCDATA);
    /**
     * @brief see #xmlTextCDATASet
     */
    zfbool xmlTextCDATA(void) const;

private:
    _ZFP_ZFXmlItemPrivate *d;
private:
    ZFXmlItem(ZF_IN _ZFP_ZFXmlItemPrivate *ref);
};

// ============================================================
/**
 * @brief parse xml document, or return an item with null type if fail
 */
extern ZF_ENV_EXPORT ZFXmlItem ZFXmlFromInput(ZF_IN const ZFInputCallback &callback,
                                              ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault);
/**
 * @brief parse xml document, or return an item with null type if fail
 */
extern ZF_ENV_EXPORT ZFXmlItem ZFXmlFromString(ZF_IN const zfchar *src,
                                               ZF_IN_OPT zfindex size = zfindexMax,
                                               ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault);
/**
 * @brief util method to parse and get first element, or return an item with null type if fail
 */
inline ZFXmlItem ZFXmlParseFirstElement(ZF_IN const ZFInputCallback &callback,
                                        ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault)
{
    return ZFXmlItem(ZFXmlFromInput(callback, flags).xmlChildElementFirst());
}
/**
 * @brief util method to parse and get first element, or return an item with null type if fail
 */
inline ZFXmlItem ZFXmlParseFirstElement(ZF_IN const zfchar *src,
                                        ZF_IN_OPT zfindex size = zfindexMax,
                                        ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault)
{
    return ZFXmlItem(ZFXmlFromString(src, size, flags).xmlChildElementFirst());
}

// ============================================================
/**
 * @brief convert xml to output
 *
 * @note result string is not ensured to be a valid xml string
 *   if source is not valid
 */
extern ZF_ENV_EXPORT zfbool ZFXmlToOutput(ZF_IN_OUT const ZFOutputCallback &output,
                                          ZF_IN const ZFXmlItem &xmlItem,
                                          ZF_IN_OPT const ZFXmlOutputFlags &outputFlags = ZFXmlOutputFlagsDefault);
/**
 * @brief convert xml to string
 *
 * @note result string is not ensured to be a valid xml string
 *   if source is not valid
 */
inline zfbool ZFXmlToString(ZF_IN_OUT zfstring &ret,
                            ZF_IN const ZFXmlItem &xmlItem,
                            ZF_IN_OPT const ZFXmlOutputFlags &outputFlags = ZFXmlOutputFlagsDefault)
{
    return ZFXmlToOutput(ZFOutputCallbackForString(ret), xmlItem, outputFlags);
}
/** @brief see #ZFXmlToString */
inline zfstring ZFXmlToString(ZF_IN const ZFXmlItem &xmlItem,
                              ZF_IN_OPT const ZFXmlOutputFlags &outputFlags = ZFXmlOutputFlagsDefault)
{
    zfstring ret;
    ZFXmlToString(ret, xmlItem, outputFlags);
    return ret;
}

// ============================================================
// escape chars
/**
 * @brief encode xml chars, e.g. convert "&" to "&amp;"
 *
 * support:
 * @code
 *   &lt;               <
 *   &gt;               >
 *   &amp;              &
 *   &apos;             '
 *   &quot;             "
 *   &#0; ~ &#65536;    decimal encoded char
 *   &#x0; ~ &#xFFFF;   heximal encoded char
 * @endcode
 */
extern ZF_ENV_EXPORT void ZFXmlEscapeCharEncode(ZF_OUT zfstring &dst,
                                                ZF_IN const zfchar *src,
                                                ZF_IN_OPT zfindex count = zfindexMax);
/**
 * @brief see #ZFXmlEscapeCharEncode
 */
extern ZF_ENV_EXPORT void ZFXmlEscapeCharEncode(ZF_OUT const ZFOutputCallback &dst,
                                                ZF_IN const zfchar *src,
                                                ZF_IN_OPT zfindex count = zfindexMax);

/**
 * @brief see #ZFXmlEscapeCharEncode
 */
extern ZF_ENV_EXPORT void ZFXmlEscapeCharDecode(ZF_OUT zfstring &dst,
                                                ZF_IN const zfchar *src,
                                                ZF_IN_OPT zfindex count = zfindexMax);
/**
 * @brief see #ZFXmlEscapeCharEncode
 */
extern ZF_ENV_EXPORT void ZFXmlEscapeCharDecode(ZF_OUT const ZFOutputCallback &dst,
                                                ZF_IN const zfchar *src,
                                                ZF_IN_OPT zfindex count = zfindexMax);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFXml_h_

#include "ZFXmlSerializableConverterDef.h"

