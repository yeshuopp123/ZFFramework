/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZF2048Types.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZF2048Point ZF2048PointZero = {0, 0};

ZFPROPERTY_TYPE_DEFINE(ZF2048Value, ZF2048Value)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_FROM_DEFINE(ZF2048Value, ZF2048Value)
{
    if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZF2048Value, serializableData, outErrorHintToAppend, outErrorPos) == zfnull)
    {
        return zffalse;
    }
    const zfchar *propertyValue = ZFSerializableUtil::checkPropertyValue(serializableData);
    if(propertyValue == zfnull)
    {
        result = 0;
        return zftrue;
    }
    serializableData.resolveMark();
    if(zfintFromString(result, propertyValue) != zfnull)
    {
        return zffalse;
    }
    return zftrue;
}
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_TO_DEFINE(ZF2048Value, ZF2048Value)
{
    serializableData.itemClassSet(ZFPropertyTypeId_ZF2048Value);
    serializableData.propertyValueSet(zfintToString(v));
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

