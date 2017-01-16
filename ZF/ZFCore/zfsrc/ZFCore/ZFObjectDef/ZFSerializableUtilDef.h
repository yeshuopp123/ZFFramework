/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSerializableUtilDef.h
 * @brief utilities for ZFSerializable's implementation
 */

#ifndef _ZFI_ZFSerializableUtilDef_h_
#define _ZFI_ZFSerializableUtilDef_h_

#include "ZFSerializableDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFSerializableUtil)

extern ZF_ENV_EXPORT ZFCoreArray<ZFOutputCallback> &_ZFP_ZFSerializableUtilErrorOutputCallbacks(void);
/**
 * @brief global output callback that would be called when any serialization error occurred,
 *   empty by default, usually for debug use only
 *
 * note: add this may cause performance issue, recommended used for debug only
 */
#define ZFSerializableUtilErrorOutputCallbacks ZFSerializableUtil::_ZFP_ZFSerializableUtilErrorOutputCallbacks()

/**
 * @brief util method to set error
 */
extern ZF_ENV_EXPORT void errorOccurred(ZF_OUT_OPT zfstring *outErrorHint,
                                        ZF_OUT_OPT ZFSerializableData *outErrorPos,
                                        ZF_IN const ZFSerializableData &errorPos,
                                        ZF_IN const zfchar *fmt,
                                        ...);

/**
 * @brief util method to set error
 */
extern ZF_ENV_EXPORT void errorOccurred(ZF_OUT_OPT zfstring *outErrorHint,
                                        ZF_IN const zfchar *fmt,
                                        ...);

/**
 * @brief util method to set error
 */
extern ZF_ENV_EXPORT void errorOccurredWhile(ZF_OUT_OPT zfstring *outErrorHint,
                                             ZF_OUT_OPT ZFSerializableData *outErrorPos,
                                             ZF_IN const ZFSerializableData &errorPos,
                                             ZF_IN const zfchar *serializingName,
                                             ZF_IN const zfchar *errorValue);
/**
 * @brief util method to set error
 */
extern ZF_ENV_EXPORT void errorOccurredWhile(ZF_OUT_OPT zfstring *outErrorHint,
                                             ZF_IN const zfchar *serializingName,
                                             ZF_IN const zfchar *errorValue);

/**
 * @brief util to check whether the serializable data contains desired attribute
 *
 * use null desiredClass shows that any class name is allowed except empty\n
 * return null if no match
 */
extern ZF_ENV_EXPORT const zfchar *checkSerializableClass(ZF_IN const zfchar *desiredClass,
                                                          ZF_IN const ZFSerializableData &serializableData);
/**
 * @brief see #checkSerializableClass, output error hint if failed
 */
extern ZF_ENV_EXPORT const zfchar *requireSerializableClass(ZF_IN const zfchar *desiredClass,
                                                            ZF_IN const ZFSerializableData &serializableData,
                                                            ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                            ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);

/**
 * @brief util to check whether the serializable data contains desired attribute,
 *   auto mark as resolved
 *
 * return null if not exist
 */
extern ZF_ENV_EXPORT const zfchar *checkAttribute(ZF_IN const ZFSerializableData &serializableData,
                                                  ZF_IN const zfchar *desiredAttribute);
/**
 * @brief see #checkAttribute, output error hint if failed,
 *   auto mark as resolved
 */
extern ZF_ENV_EXPORT const zfchar *requireAttribute(ZF_IN const ZFSerializableData &serializableData,
                                                    ZF_IN const zfchar *desiredAttribute,
                                                    ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                    ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);

/**
 * @brief util to check whether the serializable data contains desired element,
 *   auto skip resolved data,
 *   auto mark name as resolved
 */
extern ZF_ENV_EXPORT const ZFSerializableData *checkElementByName(ZF_IN const ZFSerializableData &serializableData,
                                                                  ZF_IN const zfchar *desiredElementName);
/**
 * @brief see #checkElementByName, output error hint if failed,
 *   auto skip resolved data,
 *   auto mark name as resolved
 */
extern ZF_ENV_EXPORT const ZFSerializableData *requireElementByName(ZF_IN const ZFSerializableData &serializableData,
                                                                    ZF_IN const zfchar *desiredElementName,
                                                                    ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                                    ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);

/**
 * @brief util to check whether the serializable data contains desired element,
 *   auto skip resolved data,
 *   auto mark category as resolved
 */
extern ZF_ENV_EXPORT const ZFSerializableData *checkElementByCategory(ZF_IN const ZFSerializableData &serializableData,
                                                                      ZF_IN const zfchar *desiredElementCategory);
/**
 * @brief see #checkElementByCategory, output error hint if failed,
 *   auto skip resolved data,
 *   auto mark category as resolved
 */
extern ZF_ENV_EXPORT const ZFSerializableData *requireElementByCategory(ZF_IN const ZFSerializableData &serializableData,
                                                                        ZF_IN const zfchar *desiredElementCategory,
                                                                        ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                                        ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);

/**
 * @brief util to check whether the serializable data contains property name attribute,
 *   auto mark as resolved
 */
inline const zfchar *checkPropertyName(ZF_IN const ZFSerializableData &serializableData)
{
    return ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_name);
}
/**
 * @brief see #checkPropertyName, output error hint if failed,
 *   auto mark as resolved
 */
inline const zfchar *requirePropertyName(ZF_IN const ZFSerializableData &serializableData,
                                         ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                         ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull)
{
    return ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_name, outErrorHint, outErrorPos);
}

/**
 * @brief util to check whether the serializable data contains property value attribute,
 *   auto mark as resolved
 */
inline const zfchar *checkPropertyValue(ZF_IN const ZFSerializableData &serializableData)
{
    return ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_value);
}
/**
 * @brief see #checkPropertyValue, output error hint if failed,
 *   auto mark as resolved
 */
inline const zfchar *requirePropertyValue(ZF_IN const ZFSerializableData &serializableData,
                                          ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                          ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull)
{
    return ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_value, outErrorHint, outErrorPos);
}

/**
 * @brief util to check whether the serializable data contains category,
 *   auto mark as resolved
 */
inline const zfchar *checkCategory(ZF_IN const ZFSerializableData &serializableData)
{
    return ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_category);
}
/**
 * @brief see #checkCategory, output error hint if failed,
 *   auto mark as resolved
 */
inline const zfchar *requireCategory(ZF_IN const ZFSerializableData &serializableData,
                                     ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                     ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull)
{
    return ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_category, outErrorHint, outErrorPos);
}

/**
 * @brief util method to print resolve status of ZFSerializableData, for debug use only,
 *   output nothing if all resolved
 *
 * return true if anything not resolved, or false if all resolved
 */
extern ZF_ENV_EXPORT zfbool printResolveStatus(ZF_IN const ZFSerializableData &serializableData,
                                               ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault);

// ============================================================
// util macro for impl
#define _ZFP_ZFSerializableUtilSerializeFromData_check(outErrorHint, outErrorPos)
#define _ZFP_ZFSerializableUtilSerializeFromData_require(outErrorHint, outErrorPos) , outErrorHint, outErrorPos
#define _ZFP_ZFSerializableUtilSerializeFromData_(check_or_require, outErrorHint, outErrorPos) \
    _ZFP_ZFSerializableUtilSerializeFromData_##check_or_require(outErrorHint, outErrorPos)
#define _ZFP_ZFSerializableUtilSerializeFromData(check_or_require, outErrorHint, outErrorPos) \
    _ZFP_ZFSerializableUtilSerializeFromData_(check_or_require, outErrorHint, outErrorPos)

/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos, \
    check_or_require, key, TypeName, value) \
    do \
    { \
        const zfchar *valueString = ZFSerializableUtil::check_or_require##Attribute(serializableData, key \
            _ZFP_ZFSerializableUtilSerializeFromData(check_or_require, outErrorHint, outErrorPos)); \
        if(valueString != zfnull) \
        { \
            if(TypeName##FromString(value, valueString) != zfnull) \
            { \
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, \
                    zfText("failed to convert from \"%s\""), valueString); \
                return zffalse; \
            } \
        } \
    } while(zffalse)
/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHint, ref, \
    key, TypeName, thisValue, refValue, defaultValue) \
    do \
    { \
        if((ref == zfnull && ZFComparerDefault(thisValue, defaultValue) != ZFCompareTheSame) \
            || (ref != zfnull && ZFComparerDefault(thisValue, refValue) != ZFCompareTheSame)) \
        { \
            zfstring valueString; \
            TypeName##ToString(valueString, thisValue); \
            serializableData.attributeSet(key, valueString); \
        } \
    } while(zffalse)

// ============================================================
/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos, \
    check_or_require, key, TypeName, value) \
    do \
    { \
        const ZFSerializableData *valueData = ZFSerializableUtil::check_or_require##ElementByCategory(serializableData, key \
            _ZFP_ZFSerializableUtilSerializeFromData(check_or_require, outErrorHint, outErrorPos)); \
        if(valueData != zfnull) \
        { \
            if(!TypeName##FromSerializableData(value, *valueData, outErrorHint, outErrorPos)) \
            { \
                return zffalse; \
            } \
            if(valueData->referenceInfoExistRecursively()) \
            { \
                ZFSerializableData referenceInfo; \
                referenceInfo.copyFrom(*valueData); \
                this->referenceInfoForCategorySet(key, &referenceInfo); \
            } \
        } \
    } while(zffalse)
/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeCategoryToData(serializableData, outErrorHint, ref, \
    key, TypeName, thisValue, refValue, defaultValue) \
    do \
    { \
        const ZFSerializableData *referenceInfo = this->referenceInfoForCategory(key); \
        if(referenceInfo != zfnull \
            || (ref == zfnull && ZFComparerDefault(thisValue, defaultValue) != ZFCompareTheSame) \
            || (ref != zfnull && ZFComparerDefault(thisValue, refValue) != ZFCompareTheSame)) \
        { \
            ZFSerializableData categoryData; \
            if(!TypeName##ToSerializableData(categoryData, thisValue, outErrorHint)) \
            { \
                return zffalse; \
            } \
            if(referenceInfo != zfnull) \
            { \
                categoryData.referenceInfoRestore(*referenceInfo); \
            } \
            categoryData.categorySet(key); \
            serializableData.elementAdd(categoryData); \
        } \
    } while(zffalse)

ZF_NAMESPACE_END(ZFSerializableUtil)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableUtilDef_h_

