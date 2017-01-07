/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPropertyTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFPropertyTypeData
{
    public:
    ZFPropertyTypeSerializeFromCallback serializeFromCallback;
    ZFPropertyTypeSerializeToCallback serializeToCallback;
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPropertyTypeIdHolder, ZFLevelZFFrameworkNormal)
{
}
public:
    ZFCoreMap propertyTypeMap; // _ZFP_ZFPropertyTypeData
ZF_GLOBAL_INITIALIZER_END(ZFPropertyTypeIdHolder)
#define _ZFP_ZFPropertyTypeMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyTypeIdHolder)->propertyTypeMap)

// ============================================================
void _ZFP_ZFPropertyTypeRegister(ZF_IN const zfchar *typeIdName,
                                 ZF_IN ZFPropertyTypeSerializeFromCallback serializeFromCallback,
                                 ZF_IN ZFPropertyTypeSerializeToCallback serializeToCallback)
{
    zfCoreAssert(typeIdName != zfnull);
    _ZFP_ZFPropertyTypeData *type = zfnew(_ZFP_ZFPropertyTypeData);
    type->serializeFromCallback = serializeFromCallback;
    type->serializeToCallback = serializeToCallback;
    _ZFP_ZFPropertyTypeMap.set(typeIdName, ZFCorePointerForObject<_ZFP_ZFPropertyTypeData *>(type));
}
void _ZFP_ZFPropertyTypeUnregister(ZF_IN const zfchar *typeIdName)
{
    _ZFP_ZFPropertyTypeMap.remove(typeIdName);
}
ZFPropertyTypeSerializeFromCallback ZFPropertyTypeGetSerializeFromCallback(ZF_IN const zfchar *typeIdName)
{
    _ZFP_ZFPropertyTypeData *typeData = _ZFP_ZFPropertyTypeMap.get<_ZFP_ZFPropertyTypeData *>(typeIdName);
    return ((typeData != zfnull) ? typeData->serializeFromCallback : zfnull);
}
ZFPropertyTypeSerializeToCallback ZFPropertyTypeGetSerializeToCallback(ZF_IN const zfchar *typeIdName)
{
    _ZFP_ZFPropertyTypeData *typeData = _ZFP_ZFPropertyTypeMap.get<_ZFP_ZFPropertyTypeData *>(typeIdName);
    return ((typeData != zfnull) ? typeData->serializeToCallback : zfnull);
}

zfbool ZFPropertySerializeFrom(ZF_IN ZFObject *ownerObject,
                               ZF_IN const ZFSerializableData &serializableData,
                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(ownerObject == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("null object"));
        return zffalse;
    }

    const zfchar *propertyName = ZFSerializableUtil::requirePropertyName(serializableData, outErrorHintToAppend, outErrorPos);
    if(propertyName == zfnull)
    {
        return zffalse;
    }
    const ZFProperty *property = ownerObject->classData()->propertyForName(propertyName);
    if(property == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("no property named \"%s\" for object %s"),
            propertyName, ownerObject->objectInfoOfInstance().cString());
        return zffalse;
    }
    if(property->propertyIsRetainProperty())
    {
        zfautoObject obj;
        if(!ZFObjectFromSerializableData(obj, serializableData, outErrorHintToAppend, outErrorPos))
        {
            return zffalse;
        }
        if(obj != zfautoObjectNull && !obj.toObject()->classData()->classIsTypeOf(property->propertyClassOfRetainProperty()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                zfText("object %s not type of %s"),
                obj.toObject()->objectInfoOfInstance().cString(), property->propertyClassOfRetainProperty()->className());
            return zffalse;
        }
        property->callbackRetainSet(property, ownerObject, obj.toObject());
        return zftrue;
    }

    ZFPropertyTypeSerializeFromCallback serializeFromCallback = ZFPropertyTypeGetSerializeFromCallback(serializableData.itemClass());
    if(serializeFromCallback == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("type \"%s\" not registered while serializing \"%s\""),
            serializableData.itemClass(),
            ownerObject->classData()->className());
        return zffalse;
    }
    return serializeFromCallback(property, ownerObject, serializableData, outErrorHintToAppend, outErrorPos);
}
zfbool ZFPropertySerializeTo(ZF_IN const ZFProperty *propertyInfo,
                             ZF_IN ZFObject *ownerObject,
                             ZF_OUT ZFSerializableData &serializableData,
                             ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(ownerObject == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("null object"));
        return zffalse;
    }
    if(propertyInfo == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("null property"));
        return zffalse;
    }
    if(propertyInfo->propertyIsRetainProperty())
    {
        ZFObject *obj = propertyInfo->callbackRetainGet(propertyInfo, ownerObject);
        if(!ZFObjectToSerializableData(serializableData, obj, outErrorHintToAppend))
        {
            return zffalse;
        }
        serializableData.propertyNameSet(propertyInfo->propertyName());
        return zftrue;
    }

    ZFPropertyTypeSerializeToCallback serializeToCallback = ZFPropertyTypeGetSerializeToCallback(propertyInfo->propertyTypeIdName());
    if(serializeToCallback == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("type \"%s\" not registered while serializing \"%s\""),
            propertyInfo->propertyTypeIdName(),
            ownerObject->classData()->className());
        return zffalse;
    }
    return serializeToCallback(propertyInfo, ownerObject, serializableData, outErrorHintToAppend);
}

ZF_NAMESPACE_GLOBAL_END

