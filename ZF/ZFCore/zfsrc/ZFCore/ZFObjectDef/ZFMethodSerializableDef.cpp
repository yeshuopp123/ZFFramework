/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFMethodSerializableDef.h"
#include "ZFSerializableUtilDef.h"
#include "ZFCoreType_IODef.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZFPROPERTY_TYPE_DEFINE(ZFMethod, const ZFMethod *)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_FROM_DEFINE(ZFMethod, const ZFMethod *)
{
    if(zfscmpTheSame(serializableData.itemClass(), ZFSerializableKeyword_null))
    {
        result = zfnull;
        serializableData.resolveMark();
        return zftrue;
    }
    if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZFMethod, serializableData, outErrorHintToAppend, outErrorPos) == zfnull)
    {
        return zffalse;
    }
    const zfchar *ownerName = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFMethod_owner, outErrorHintToAppend, outErrorPos);
    if(ownerName == zfnull)
    {
        return zffalse;
    }

    const zfchar *methodName = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFMethod_method, outErrorHintToAppend, outErrorPos);
    if(methodName == zfnull)
    {
        return zffalse;
    }
    const zfchar *methodId = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFMethod_methodId);

    const ZFClass *ownerCls = ZFClass::classForName(ownerName);
    if(ownerCls == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData, zfText("no class named \"%s\""), ownerName);
        return zffalse;
    }

    result = ownerCls->methodForName(methodName, methodId);

    if(result == zfnull)
    {
        zfstring errorHint;
        zfstringAppend(errorHint, zfText("class \"%s\" has no method named \"%s\""), ownerName, methodName);
        if(methodId != zfnull)
        {
            zfstringAppend(errorHint, zfText(" with method id \"%s\""), methodId);
        }
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData, errorHint.cString());
        return zffalse;
    }

    serializableData.resolveMark();
    return zftrue;
}
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_TO_DEFINE(ZFMethod, const ZFMethod *)
{
    if(v == zfnull)
    {
        serializableData.itemClassSet(ZFSerializableKeyword_null);
        return zftrue;
    }

    serializableData.itemClassSet(ZFPropertyTypeId_ZFMethod);

    if(v->ownerClass() != zfnull)
    {
        serializableData.attributeSet(ZFSerializableKeyword_ZFMethod_owner, v->ownerClass()->className());
    }
    else
    {
        zfCoreCriticalShouldNotGoHere();
        return zffalse;
    }
    serializableData.attributeSet(ZFSerializableKeyword_ZFMethod_method, v->methodName());
    serializableData.attributeSet(ZFSerializableKeyword_ZFMethod_methodId, v->methodId());
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

