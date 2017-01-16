/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFXmlSerializableConverterDef.h
 * @brief serializable convertion between xml and serializalbe
 */

#ifndef _ZFI_ZFXmlSerializableConverterDef_h_
#define _ZFI_ZFXmlSerializableConverterDef_h_

#include "ZFXml.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFOUTPUT_TYPE(ZFXmlItem, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFXmlItem *, {if(v) {output << v->objectInfo();} else {output << (const void *)zfnull;}})
ZFOUTPUT_TYPE(ZFXmlItem *, {output << (const ZFXmlItem *)v;})

// ============================================================
// xml serializable conversion

/**
 * @brief see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 *
 * format:
 * @code
 *   <Node refType="xml" refData="file descriptor, see ZFInputCallbackForFileDescriptor" />
 * @endcode
 */
#define ZFSerializableDataRefType_xml xml
/** @brief see #ZFSerializableDataRefType_xml */
#define ZFSerializableDataRefTypeName_xml ZFM_TOSTRING(ZFSerializableDataRefType_xml)

/**
 * @brief see #ZFOBJECT_CREATOR_DEFINE
 *
 * data:
 * @code
 *   "file descriptor, see ZFInputCallbackForFileDescriptor"
 * @endcode
 * data is a file descriptor to a xml file,
 * which should contain a serializable object
 */
#define ZFObjectCreatorType_xml xml
/** @brief see #ZFSerializableDataRefType_xml */
#define ZFObjectCreatorTypeName_xml ZFM_TOSTRING(ZFObjectCreatorType_xml)

// ============================================================
/**
 * @brief parse xml formated data to serializable
 */
extern ZF_ENV_EXPORT zfbool ZFXmlParseToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                                                         ZF_IN const ZFXmlItem &xmlElement,
                                                         ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                         ZF_OUT_OPT ZFXmlItem *outErrorPos = zfnull);
/**
 * @brief see #ZFXmlParseToSerializableData
 */
extern ZF_ENV_EXPORT ZFSerializableData ZFXmlParseToSerializableData(ZF_IN const ZFXmlItem &xmlElement,
                                                                     ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                                     ZF_OUT_OPT ZFXmlItem *outErrorPos = zfnull);
/**
 * @brief print serializable to xml formated data
 */
extern ZF_ENV_EXPORT zfbool ZFXmlPrintFromSerializableData(ZF_OUT ZFXmlItem &xmlElement,
                                                           ZF_IN const ZFSerializableData &serializableData,
                                                           ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                           ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
/**
 * @brief see #ZFXmlPrintFromSerializableData
 */
extern ZF_ENV_EXPORT ZFXmlItem ZFXmlPrintFromSerializableData(ZF_IN const ZFSerializableData &serializableData,
                                                              ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                              ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);

// ============================================================
/**
 * @brief util method to convert serializable object to xml format
 */
extern ZF_ENV_EXPORT void ZFXmlPrint(ZF_IN const ZFSerializableData &serializableData,
                                     ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault,
                                     ZF_IN_OPT const ZFXmlOutputFlags &flags = ZFXmlOutputFlagsDefault);
/** @brief see #ZFXmlPrint */
extern ZF_ENV_EXPORT void ZFXmlPrint(ZF_IN ZFObject *obj,
                                     ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault,
                                     ZF_IN_OPT const ZFXmlOutputFlags &flags = ZFXmlOutputFlagsDefault);

// ============================================================
/**
 * @brief util method to parse serializable data from input with xml format
 *
 * this method would automatically setup
 * #ZFSerializableDataTagKeyword_filePath/#ZFSerializableDataTagKeyword_resPath
 */
extern ZF_ENV_EXPORT zfbool ZFXmlParse(ZF_OUT ZFSerializableData &ret, ZF_IN const ZFInputCallback &input);
/** @brief see #ZFXmlParse */
extern ZF_ENV_EXPORT ZFSerializableData ZFXmlParse(ZF_IN const ZFInputCallback &input);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFXmlSerializableConverterDef_h_

