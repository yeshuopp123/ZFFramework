/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPropertySerializableDef.h"
#include "ZFSerializableUtilDef.h"
#include "ZFPropertyTypeDef.h"
#include "ZFCoreType_IODef.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZFPROPERTY_TYPE_DEFINE(ZFProperty, const ZFProperty *)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_FROM_DEFINE(ZFProperty, const ZFProperty *)
{
    if(zfscmpTheSame(serializableData.itemClass(), ZFSerializableKeyword_null))
    {
        result = zfnull;
        serializableData.resolveMark();
        return zftrue;
    }
    if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZFProperty, serializableData, outErrorHintToAppend, outErrorPos) == zfnull)
    {
        return zffalse;
    }

    const zfchar *tmpValue = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFProperty_owner, outErrorHintToAppend, outErrorPos);
    if(tmpValue == zfnull)
    {
        return zffalse;
    }
    const ZFClass *ownerClass = ZFClass::classForName(tmpValue);
    if(ownerClass == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("no such class \"%s\""), tmpValue);
        return zffalse;
    }

    tmpValue = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFProperty_property, outErrorHintToAppend, outErrorPos);
    if(tmpValue == zfnull)
    {
        return zffalse;
    }
    result = ownerClass->propertyForName(tmpValue);
    if(result == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("no such property \"%s\" in class \"%s\""), tmpValue, ownerClass->className());
        return zffalse;
    }

    serializableData.resolveMark();
    return zftrue;
}
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_TO_DEFINE(ZFProperty, const ZFProperty *)
{
    if(v == zfnull)
    {
        serializableData.itemClassSet(ZFSerializableKeyword_null);
        return zftrue;
    }

    serializableData.itemClassSet(ZFPropertyTypeId_ZFProperty);

    serializableData.attributeSet(ZFSerializableKeyword_ZFProperty_owner, v->ownerClass()->className());
    serializableData.attributeSet(ZFSerializableKeyword_ZFProperty_property, v->propertyName());

    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

