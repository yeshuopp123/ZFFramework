/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPropertyTypeDef_CoreType.h"
#include "ZFClassSerializableDef.h"
#include "ZFMethodSerializableDef.h"
#include "ZFPropertySerializableDef.h"
#include "ZFCoreType_IODef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfbool, zfbool)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfbool, zfbool, ZFPropertyTypeId_zfbool)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfbyte, zfbyte)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfbyte, zfbyte, ZFPropertyTypeId_zfbyte)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfchar, zfchar)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_FROM_DEFINE(zfchar, zfchar)
{
    if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_zfchar, serializableData, outErrorHintToAppend, outErrorPos) == zfnull)
    {
        return zffalse;
    }
    const zfchar *propertyValue = ZFSerializableUtil::checkPropertyValue(serializableData);
    if(propertyValue == zfnull)
    {
        result = '\0';
        return zftrue;
    }
    serializableData.resolveMark();
    result = *propertyValue;
    return zftrue;
}
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_TO_DEFINE(zfchar, zfchar)
{
    serializableData.itemClassSet(ZFPropertyTypeId_zfchar);
    zfchar tmp[2] = {0};
    tmp[0] = v;
    serializableData.propertyValueSet(tmp);
    return zftrue;
}

// ============================================================
zfbool zfstringFromSerializableData(ZF_OUT const zfchar * &result,
                                    ZF_IN const ZFSerializableData &serializableData,
                                    ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                    ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_zfstring, serializableData, outErrorHintToAppend, outErrorPos) == zfnull)
    {
        return zffalse;
    }
    const zfchar *propertyValue = ZFSerializableUtil::checkPropertyValue(serializableData);
    if(propertyValue == zfnull)
    {
        result = zfnull;
        return zftrue;
    }
    serializableData.resolveMark();
    result = propertyValue;
    return zftrue;
}
zfbool zfstringToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                                  ZF_IN const zfchar * const &v,
                                  ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(v == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("null string"));
        return zffalse;
    }

    serializableData.itemClassSet(ZFPropertyTypeId_zfstring);
    serializableData.propertyValueSet(v);
    return zftrue;
}
ZFPROPERTY_TYPE_DEFINE(zfstring, zfstring)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_FROM_DEFINE(zfstring, zfstring)
{
    if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_zfstring, serializableData, outErrorHintToAppend, outErrorPos) == zfnull)
    {
        return zffalse;
    }
    const zfchar *propertyValue = ZFSerializableUtil::checkPropertyValue(serializableData);
    if(propertyValue == zfnull)
    {
        result.removeAll();
        return zftrue;
    }
    serializableData.resolveMark();
    result = propertyValue;
    return zftrue;
}
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_TO_DEFINE(zfstring, zfstring)
{
    serializableData.itemClassSet(ZFPropertyTypeId_zfstring);
    serializableData.propertyValueSet(v.cString());
    return zftrue;
}

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfint, zfint)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfint, zfint, ZFPropertyTypeId_zfint)
// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfuint, zfuint)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfuint, zfuint, ZFPropertyTypeId_zfuint)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfindex, zfindex)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfindex, zfindex, ZFPropertyTypeId_zfindex)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfint8, zfint8)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfint8, zfint8, ZFPropertyTypeId_zfint8)
// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfuint8, zfuint8)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfuint8, zfuint8, ZFPropertyTypeId_zfuint8)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfint16, zfint16)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfint16, zfint16, ZFPropertyTypeId_zfint16)
// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfuint16, zfuint16)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfuint16, zfuint16, ZFPropertyTypeId_zfuint16)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfint32, zfint32)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfint32, zfint32, ZFPropertyTypeId_zfint32)
// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfuint32, zfuint32)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfuint32, zfuint32, ZFPropertyTypeId_zfuint32)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfint64, zfint64)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfint64, zfint64, ZFPropertyTypeId_zfint64)
// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfuint64, zfuint64)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfuint64, zfuint64, ZFPropertyTypeId_zfuint64)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zffloat, zffloat)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zffloat, zffloat, ZFPropertyTypeId_zffloat)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfdouble, zfdouble)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfdouble, zfdouble, ZFPropertyTypeId_zfdouble)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zflongdouble, zflongdouble)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zflongdouble, zflongdouble, ZFPropertyTypeId_zflongdouble)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zftimet, zftimet)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zftimet, zftimet, ZFPropertyTypeId_zftimet)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfidentity, zfidentity)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfidentity, zfidentity, ZFPropertyTypeId_zfidentity)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfflags, zfflags)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfflags, zfflags, ZFPropertyTypeId_zfflags)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(ZFCompareResult, ZFCompareResult)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(ZFCompareResult, ZFCompareResult, ZFPropertyTypeId_ZFCompareResult)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(ZFSeekPos, ZFSeekPos)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(ZFSeekPos, ZFSeekPos, ZFPropertyTypeId_ZFSeekPos)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfindexRange, zfindexRange)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(zfindexRange, zfindexRange, ZFPropertyTypeId_zfindexRange)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(ZFFilterType, ZFFilterType)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(ZFFilterType, ZFFilterType, ZFPropertyTypeId_ZFFilterType)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(ZFClassFilterType, ZFClassFilterType)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(ZFClassFilterType, ZFClassFilterType, ZFPropertyTypeId_ZFClassFilterType)

ZF_NAMESPACE_GLOBAL_END

