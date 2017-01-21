/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFPropertyTypeDef.h
 * @brief type define for ZFProperty
 */

#ifndef _ZFI_ZFPropertyTypeDef_h_
#define _ZFI_ZFPropertyTypeDef_h_

#include "ZFPropertyDef.h"
#include "ZFSerializableDef.h"
#include "zfautoObjectFwd.h"
#include "ZFAnyDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief register a property type
 *
 * usually internal use only,
 * ZFFramework would supply most of the types,
 * however you may use this to register your own types\n
 * \n
 * typical code to register a type:
 * @code
 *   // ============================================================
 *   // in h file
 *   // declare your type, name must be unique
 *   / **
 *    * add your Doxygen docs here
 *    * /
 *   ZFPROPERTY_TYPE_DECLARE(YourType, YourType)
 *
 *   // ============================================================
 *   // in cpp file
 *   // register your type
 *   ZFPROPERTY_TYPE_DEFINE(YourType, YourType, {
 *           // serializeFrom callback, proto type:
 *           //   zfbool YourTypeFromSerializableData(
 *           //       ZF_OUT YourType &v,
 *           //       ZF_IN const ZFSerializableData &serializableData,
 *           //       ZF_OUT_OPT zfstring *outErrorHint = zfnull,
 *           //       ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
 *           // you should:
 *           //   * check whether the property's type match the serializableData's type
 *           //   * serialize from serializableData
 *           //   * set the property by the property's setter method
 *       }, {
 *           // serializeTo callback, proto type:
 *           //   zfbool YourTypeToSerializableData(
 *           //       ZF_OUT ZFSerializableData &serializableData,
 *           //       ZF_IN YourType const &v,
 *           //       ZF_OUT_OPT zfstring *outErrorHint = zfnull);
 *           // you should:
 *           //   * get the property's value by the property's getter method
 *           //   * save typeid to the serializableData
 *           //   * save property name to the serializableData
 *           //   * save property value to the serializableData
 *       })
 *
 *   // or, you may use #ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER for short,
 *   // if your type support converter from/to string that suits these proto type:
 *   //   const zfchar *YourTypeFromString(ZF_OUT Type &,
 *   //                                    ZF_IN const zfchar *);
 *   //   zfstring YourTypeToString(ZF_IN Type const &);
 *   ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(YourType, YourType)
 * @endcode
 *
 * once registered, your type can be used as #ZFPROPERTY_ASSIGN
 * which benefits from the powerful automatic serialization logic
 */
#define ZFPROPERTY_TYPE_DECLARE(TypeName, Type) \
    /** \n */ \
    inline const zfchar *ZFPropertyTypeId_##TypeName(void) \
    { \
        return zfText(#TypeName); \
    } \
    /** @brief see #ZFPROPERTY_TYPE_DECLARE */ \
    extern ZF_ENV_EXPORT zfbool TypeName##FromSerializableData(ZF_OUT Type &v, \
                                                               ZF_IN const ZFSerializableData &serializableData, \
                                                               ZF_OUT_OPT zfstring *outErrorHint = zfnull, \
                                                               ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull); \
    /** @brief see #ZFPROPERTY_TYPE_DECLARE */ \
    inline Type TypeName##FromSerializableData(ZF_IN const ZFSerializableData &serializableData, \
                                               ZF_OUT_OPT zfstring *outErrorHint = zfnull, \
                                               ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull) \
    { \
        Type ret; \
        if(TypeName##FromSerializableData(ret, serializableData, outErrorHint, outErrorPos)) \
        { \
            return ret; \
        } \
        else \
        { \
            typedef Type _Type; \
            return _Type();\
        } \
        return ret; \
    } \
    /** @brief see #ZFPROPERTY_TYPE_DECLARE */ \
    extern ZF_ENV_EXPORT zfbool TypeName##ToSerializableData(ZF_OUT ZFSerializableData &serializableData, \
                                                             ZF_IN Type const &v, \
                                                             ZF_OUT_OPT zfstring *outErrorHint = zfnull); \
    /** @brief see #ZFPROPERTY_TYPE_DECLARE */ \
    inline ZFSerializableData TypeName##ToSerializableData(ZF_IN Type const &v, \
                                                           ZF_OUT_OPT zfstring *outErrorHint = zfnull) \
    { \
        ZFSerializableData ret; \
        if(TypeName##ToSerializableData(ret, v, outErrorHint)) \
        { \
            return ret; \
        } \
        else \
        { \
            return ZFSerializableData(); \
        } \
    } \
    _ZFP_ZFPROPERTY_TYPE_ID_DATA_DECLARE(TypeName, Type)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 */
#define ZFPROPERTY_TYPE_DEFINE(TypeName, Type, serializeFromAction, serializeToAction) \
    zfclassNotPOD _ZFP_ZFPropertyTypeRegisterHolder_##TypeName \
    { \
    public: \
        static zfbool serializeFrom(ZF_IN const ZFProperty *propertyInfo, \
                                    ZF_IN ZFObject *ownerObject, \
                                    ZF_IN const ZFSerializableData &serializableData, \
                                    ZF_OUT_OPT zfstring *outErrorHint = zfnull, \
                                    ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull) \
        { \
            if(ZFSerializableUtil::requireSerializableClass(propertyInfo->propertyTypeIdName(), serializableData, outErrorHint, outErrorPos) == zfnull) \
            { \
                return zffalse; \
            } \
            Type v; \
            if(!TypeName##FromSerializableData(v, serializableData, outErrorHint, outErrorPos)) \
            { \
                return zffalse; \
            } \
            propertyInfo->setterMethod()->execute<void, Type const &>(ownerObject, v); \
            return zftrue; \
        } \
        static zfbool serializeTo(ZF_IN const ZFProperty *propertyInfo, \
                                  ZF_IN ZFObject *ownerObject, \
                                  ZF_OUT ZFSerializableData &serializableData, \
                                  ZF_OUT_OPT zfstring *outErrorHint = zfnull) \
        { \
            serializableData.itemClassSet(propertyInfo->propertyTypeIdName()); \
            serializableData.propertyNameSet(propertyInfo->propertyName()); \
            Type const &v = propertyInfo->getterMethod()->execute<Type const &>(ownerObject); \
            return TypeName##ToSerializableData(serializableData, v, outErrorHint); \
        } \
    }; \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPropertyTypeIdHolder_##TypeName, ZFLevelZFFrameworkNormal) \
    { \
        _ZFP_ZFPropertyTypeRegister( \
            ZFPropertyTypeId_##TypeName(), \
            _ZFP_ZFPropertyTypeRegisterHolder_##TypeName::serializeFrom, \
            _ZFP_ZFPropertyTypeRegisterHolder_##TypeName::serializeTo); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFPropertyTypeIdHolder_##TypeName) \
    { \
        _ZFP_ZFPropertyTypeUnregister(ZFPropertyTypeId_##TypeName()); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFPropertyTypeIdHolder_##TypeName) \
    zfbool TypeName##FromSerializableData(ZF_OUT Type &v, \
                                          ZF_IN const ZFSerializableData &serializableData, \
                                          ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
                                          ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */) \
    { \
        serializeFromAction \
    } \
    zfbool TypeName##ToSerializableData(ZF_OUT ZFSerializableData &serializableData, \
                                        ZF_IN Type const &v, \
                                        ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */) \
    { \
        serializeToAction \
    } \
    _ZFP_ZFPROPERTY_TYPE_ID_DATA_DEFINE(TypeName, Type)

/** @brief see #ZFPROPERTY_TYPE_DECLARE */
#define ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(TypeName, Type) \
    ZFPROPERTY_TYPE_DEFINE(TypeName, Type, { \
        if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_##TypeName(), serializableData, outErrorHint, outErrorPos) == zfnull) \
        { \
            return zffalse; \
        } \
        const zfchar *valueString = ZFSerializableUtil::checkPropertyValue(serializableData); \
        if(valueString == zfnull) \
        { \
            typedef Type _Type; \
            v = _Type(); \
            return zftrue; \
        } \
        if(TypeName##FromString(v, valueString) != zfnull) \
        { \
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, \
                zfText("invalid value: \"%s\""), valueString); \
            return zffalse; \
        } \
        serializableData.resolveMark(); \
        return zftrue; \
    }, { \
        serializableData.itemClassSet(ZFPropertyTypeId_##TypeName()); \
        zfstring s; \
        TypeName##ToString(s, v); \
        serializableData.propertyValueSet(s.cString()); \
        return zftrue; \
    })

// ============================================================
/**
 * @brief see #ZFPropertyTypeGetSerializeFromCallback
 */
typedef zfbool (*ZFPropertyTypeSerializeFromCallback)(ZF_IN const ZFProperty *propertyInfo,
                                                      ZF_IN ZFObject *ownerObject,
                                                      ZF_IN const ZFSerializableData &serializableData,
                                                      ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                      ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */);
/**
 * @brief see #ZFPropertyTypeGetSerializeToCallback
 */
typedef zfbool (*ZFPropertyTypeSerializeToCallback)(ZF_IN const ZFProperty *propertyInfo,
                                                    ZF_IN ZFObject *ownerObject,
                                                    ZF_OUT ZFSerializableData &serializableData,
                                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */);
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyTypeRegister(ZF_IN const zfchar *typeIdName,
                                                      ZF_IN ZFPropertyTypeSerializeFromCallback serializeFromCallback,
                                                      ZF_IN ZFPropertyTypeSerializeToCallback serializeToCallback);
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyTypeUnregister(ZF_IN const zfchar *typeIdName);

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * directly access the serialize callback, for performance use only
 */
extern ZF_ENV_EXPORT ZFPropertyTypeSerializeFromCallback ZFPropertyTypeGetSerializeFromCallback(ZF_IN const zfchar *typeIdName);
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * directly access the serialize callback, for performance use only
 */
extern ZF_ENV_EXPORT ZFPropertyTypeSerializeToCallback ZFPropertyTypeGetSerializeToCallback(ZF_IN const zfchar *typeIdName);

/**
 * @brief serialize property from serializable data, see #ZFPROPERTY_TYPE_DECLARE
 */
extern ZF_ENV_EXPORT zfbool ZFPropertySerializeFrom(ZF_IN ZFObject *ownerObject,
                                                    ZF_IN const ZFSerializableData &serializableData,
                                                    ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                    ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
/**
 * @brief serialize property to serializable data, see #ZFPROPERTY_TYPE_DECLARE
 */
extern ZF_ENV_EXPORT zfbool ZFPropertySerializeTo(ZF_IN const ZFProperty *propertyInfo,
                                                  ZF_IN ZFObject *ownerObject,
                                                  ZF_OUT ZFSerializableData &serializableData,
                                                  ZF_OUT_OPT zfstring *outErrorHint = zfnull);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyTypeDef_h_

#include "ZFPropertyTypeDef_CoreType.h"

