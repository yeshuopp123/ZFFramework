/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSerializableDataIO_ZFXml.h
 * @brief ioType for ZFSerializableDataIO
 */

#ifndef _ZFI_ZFSerializableDataIO_ZFXml_h_
#define _ZFI_ZFSerializableDataIO_ZFXml_h_

#include "ZFXml.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFSerializableDataFromIO,
 *   convert by #ZFSerializableDataFromInput/#ZFSerializableDataToOutput
 */
#define ZFSerializableDataIOTypeId_xml xml
/**
 * @brief see #ZFSerializableDataFromIO,
 *   convert by #ZFSerializableDataFromInput/#ZFSerializableDataToOutput
 */
#define ZFSerializableDataIOType_xml ZFM_TOSTRING(ZFSerializableDataIOTypeId_xml)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDataIO_ZFXml_h_

