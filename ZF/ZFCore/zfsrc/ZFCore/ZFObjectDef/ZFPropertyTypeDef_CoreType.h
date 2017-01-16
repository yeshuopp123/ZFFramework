/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFPropertyTypeDef_CoreType.h
 * @brief type define for #ZFPROPERTY_TYPE_DECLARE
 */

#ifndef _ZFI_ZFPropertyTypeDef_CoreType_h_
#define _ZFI_ZFPropertyTypeDef_CoreType_h_

#include "ZFPropertyTypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfbool value="value">
 *   </zfbool>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zfbool, zfbool)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfbyte value="value">
 *   </zfbyte>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zfbyte, zfbyte)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfchar value="value">
 *   </zfchar>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zfchar, zfchar)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfstring value="value">
 *   </zfstring>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zfstring, zfstring)
/**
 * @brief see #zfstringFromSerializableData, designed for performance
 *
 * returned (const zfchar *) propertyValue is the pointer to serializableData's value,
 * you should ensure it's alive while using the propertyValue
 */
extern ZF_ENV_EXPORT zfbool zfstringFromSerializableData(ZF_OUT const zfchar * &propertyValue,
                                                         ZF_IN const ZFSerializableData &serializableData,
                                                         ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                         ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
/**
 * @brief see #zfstringToSerializableData, designed for performance
 */
extern ZF_ENV_EXPORT zfbool zfstringToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                                                       ZF_IN const zfchar * const &propertyValue,
                                                       ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */);

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfint value="value">
 *   </zfint>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zfint, zfint)
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfuint value="value">
 *   </zfuint>
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFPROPERTY_TYPE_DECLARE(zfuint, zfuint)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfindex value="value">
 *   </zfindex>
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFPROPERTY_TYPE_DECLARE(zfindex, zfindex)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfint8 value="value">
 *   </zfint8>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zfint8, zfint8)
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfuint8 value="value">
 *   </zfuint8>
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFPROPERTY_TYPE_DECLARE(zfuint8, zfuint8)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfint16 value="value">
 *   </zfint16>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zfint16, zfint16)
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfuint16 value="value">
 *   </zfuint16>
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFPROPERTY_TYPE_DECLARE(zfuint16, zfuint16)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfint32 value="value">
 *   </zfint32>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zfint32, zfint32)
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfuint32 value="value">
 *   </zfuint32>
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFPROPERTY_TYPE_DECLARE(zfuint32, zfuint32)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfint64 value="value">
 *   </zfint64>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zfint64, zfint64)
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfuint64 value="value">
 *   </zfuint64>
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFPROPERTY_TYPE_DECLARE(zfuint64, zfuint64)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zffloat value="value">
 *   </zffloat>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zffloat, zffloat)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfdouble value="value">
 *   </zfdouble>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zfdouble, zfdouble)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zflongdouble value="value">
 *   </zflongdouble>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zflongdouble, zflongdouble)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zftimet value="value">
 *   </zftimet>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zftimet, zftimet)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfflags value="value">
 *   </zfflags>
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFPROPERTY_TYPE_DECLARE(zfflags, zfflags)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfidentity value="value">
 *   </zfidentity>
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFPROPERTY_TYPE_DECLARE(zfidentity, zfidentity)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFCompareResult value="value">
 *   </ZFCompereResult>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFCompareResult, ZFCompareResult)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFSeekPos value="value">
 *   </ZFSeekPos>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFSeekPos, ZFSeekPos)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <zfindexRange>
 *       <zfindex name="start" ... />
 *       <zfindex name="count" ... />
 *   </zfindexRange>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(zfindexRange, zfindexRange)
/** @brief keyword for serialize */
#define ZFSerializableKeyword_zfindexRange_start zfText("start")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_zfindexRange_count zfText("count")

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFFilterType value="value">
 *   </ZFFilterType>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFFilterType, ZFFilterType)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFClassFilterType value="value">
 *   </ZFClassFilterType>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFClassFilterType, ZFClassFilterType)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyTypeDef_CoreType_h_

