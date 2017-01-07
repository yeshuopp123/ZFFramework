/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFLangUtil_ZFSerializableHelper.h
 * @brief language util for localization
 */

#ifndef _ZFI_ZFLangUtil_ZFSerializableHelper_h_
#define _ZFI_ZFLangUtil_ZFSerializableHelper_h_

#include "ZFLang.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 *
 * format:
 * @code
 *   <zfstring refType="lang_zfstring" refData="langKey used by zfLang" />
 * @endcode
 */
#define ZFSerializableDataRefType_lang_zfstring lang_zfstring
/** @brief see #ZFSerializableDataRefType_lang_zfstring */
#define ZFSerializableDataRefTypeName_lang_zfstring ZFM_TOSTRING(ZFSerializableDataRefType_lang_zfstring)

/**
 * @brief see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 *
 * format:
 * @code
 *   <ZFString refType="lang_ZFString" refData="langKey used by zfLang" />
 * @endcode
 */
#define ZFSerializableDataRefType_lang_ZFString lang_ZFString
/** @brief see #ZFSerializableDataRefType_lang_ZFString */
#define ZFSerializableDataRefTypeName_lang_ZFString ZFM_TOSTRING(ZFSerializableDataRefType_lang_ZFString)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLangUtil_ZFSerializableHelper_h_

