/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSerializableDataIO_ZFJson.h
 * @brief ioType for ZFSerializableDataIO
 */

#ifndef _ZFI_ZFSerializableDataIO_ZFJson_h_
#define _ZFI_ZFSerializableDataIO_ZFJson_h_

#include "ZFJson.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFSerializableDataFromIO,
 *   convert by #ZFSerializableDataFromInput/#ZFSerializableDataToOutput
 */
#define ZFSerializableDataIOTypeId_json json
/**
 * @brief see #ZFSerializableDataFromIO,
 *   convert by #ZFSerializableDataFromInput/#ZFSerializableDataToOutput
 */
#define ZFSerializableDataIOType_json ZFM_TOSTRING(ZFSerializableDataIOTypeId_json)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDataIO_ZFJson_h_

