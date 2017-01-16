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
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfbool, zfbool)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfbyte, zfbyte)

// ============================================================
ZFPROPERTY_TYPE_DEFINE(zfchar, zfchar, {
        if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_zfchar(), serializableData, outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }
        const zfchar *valueString = ZFSerializableUtil::checkPropertyValue(serializableData);
        if(valueString == zfnull)
        {
            v = '\0';
            return zftrue;
        }
        serializableData.resolveMark();
        v = *valueString;
        return zftrue;
    }, {
        serializableData.itemClassSet(ZFPropertyTypeId_zfchar());
        zfchar tmp[2] = {0};
        tmp[0] = v;
        serializableData.propertyValueSet(tmp);
        return zftrue;
    })

// ============================================================
zfbool zfstringFromSerializableData(ZF_OUT const zfchar * &v,
                                    ZF_IN const ZFSerializableData &serializableData,
                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                    ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_zfstring(), serializableData, outErrorHint, outErrorPos) == zfnull)
    {
        return zffalse;
    }
    const zfchar *valueString = ZFSerializableUtil::checkPropertyValue(serializableData);
    if(valueString == zfnull)
    {
        v = zfnull;
        return zftrue;
    }
    serializableData.resolveMark();
    v = valueString;
    return zftrue;
}
zfbool zfstringToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                                  ZF_IN const zfchar * const &v,
                                  ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(v == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("null string"));
        return zffalse;
    }

    serializableData.itemClassSet(ZFPropertyTypeId_zfstring());
    serializableData.propertyValueSet(v);
    return zftrue;
}
ZFPROPERTY_TYPE_DEFINE(zfstring, zfstring, {
        if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_zfstring(), serializableData, outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }
        const zfchar *valueString = ZFSerializableUtil::checkPropertyValue(serializableData);
        if(valueString == zfnull)
        {
            v.removeAll();
            return zftrue;
        }
        serializableData.resolveMark();
        v = valueString;
        return zftrue;
    }, {
        serializableData.itemClassSet(ZFPropertyTypeId_zfstring());
        serializableData.propertyValueSet(v.cString());
        return zftrue;
    })

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfint, zfint)
// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfuint, zfuint)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfindex, zfindex)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfint8, zfint8)
// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfuint8, zfuint8)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfint16, zfint16)
// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfuint16, zfuint16)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfint32, zfint32)
// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfuint32, zfuint32)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfint64, zfint64)
// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfuint64, zfuint64)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zffloat, zffloat)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfdouble, zfdouble)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zflongdouble, zflongdouble)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zftimet, zftimet)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfidentity, zfidentity)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfflags, zfflags)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(ZFCompareResult, ZFCompareResult)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(ZFSeekPos, ZFSeekPos)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(zfindexRange, zfindexRange)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(ZFFilterType, ZFFilterType)

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(ZFClassFilterType, ZFClassFilterType)

ZF_NAMESPACE_GLOBAL_END

