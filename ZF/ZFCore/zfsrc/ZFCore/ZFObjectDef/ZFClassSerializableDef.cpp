/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFClassSerializableDef.h"
#include "ZFSerializableUtilDef.h"
#include "ZFCoreType_IODef.h"
#include "ZFPropertyTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZFPROPERTY_TYPE_DEFINE(ZFClass, const ZFClass *)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_FROM_DEFINE(ZFClass, const ZFClass *)
{
    if(zfscmpTheSame(serializableData.itemClass(), ZFSerializableKeyword_null))
    {
        result = zfnull;
        serializableData.resolveMark();
        return zftrue;
    }
    if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZFClass, serializableData, outErrorHintToAppend, outErrorPos) == zfnull)
    {
        return zffalse;
    }
    const zfchar *className = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFClass_className, outErrorHintToAppend, outErrorPos);
    if(className == zfnull)
    {
        return zffalse;
    }
    result = ZFClass::classForName(className);
    if(result == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("no such class \"%s\""), className);
        return zffalse;
    }

    serializableData.resolveMark();
    return zftrue;
}
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_TO_DEFINE(ZFClass, const ZFClass *)
{
    if(v == zfnull)
    {
        serializableData.itemClassSet(ZFSerializableKeyword_null);
        return zftrue;
    }

    serializableData.itemClassSet(ZFPropertyTypeId_ZFClass);
    serializableData.attributeSet(ZFSerializableKeyword_ZFClass_className, v->className());

    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

