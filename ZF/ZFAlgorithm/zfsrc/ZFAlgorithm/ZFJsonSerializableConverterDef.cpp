/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFJsonSerializableConverterDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/*
 * <ZFString myAttr="myAttrValue" >
 *     <zfstring category="value" value="123" />
 *     <zfstring refType="123" refData="123" />
 * </ZFString>
 *
 * {
 *     "@ZFString" : [
 *         {
 *            "@zfstring" : [],
 *            "category" : "value",
 *            "value" : "123"
 *         },
 *         {
 *            "@zfstring" : [],
 *            "refType" : "123",
 *            "refData" : "123"
 *         }
 *     ],
 *     "myAttr" : "myAttrValue"
 * }
 */

#define _ZFP_ZFJsonSerializeKey_classPrefix '@'

ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(ZFSerializableDataRefType_json)
{
    ZFJsonItem jsonObject = ZFJsonFromInput(ZFInputCallbackForFileDescriptor(refData));
    if(jsonObject.jsonIsNull())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("failed to load json object from \"%s\""), refData);
        return zffalse;
    }
    return ZFJsonParseToSerializableData(serializableData, jsonObject);
}

ZFOBJECT_CREATOR_DEFINE(ZFObjectCreatorType_json, data)
{
    ZFJsonItem jsonObject = ZFJsonFromInput(ZFInputCallbackForFileDescriptor(data));
    if(jsonObject.jsonIsNull())
    {
        return zfautoObjectNull;
    }
    ZFSerializableData serializableData;
    if(ZFJsonParseToSerializableData(serializableData, jsonObject))
    {
        return ZFObjectFromSerializableData(serializableData);
    }
    return zfautoObjectNull;
}

static zfbool _ZFP_ZFJsonParseToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                                                 ZF_IN const ZFJsonItem &jsonObject,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                 ZF_OUT_OPT ZFJsonItem *outErrorPos = zfnull)
{
    if(jsonObject.jsonIsNull())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, zfText("null json object"));
        if(outErrorPos != zfnull)
        {
            *outErrorPos = jsonObject;
        }
        return zffalse;
    }

    ZFJsonItem elementJsonArray;
    for(zfiterator jsonItemIt = jsonObject.jsonItemIterator(); jsonObject.jsonItemIteratorIsValid(jsonItemIt); jsonObject.jsonItemIteratorNext(jsonItemIt))
    {
        const zfchar *key = jsonObject.jsonItemIteratorGetKey(jsonItemIt);
        ZFJsonItem jsonItem = jsonObject.jsonItemIteratorGet(jsonItemIt);
        if(*key == _ZFP_ZFJsonSerializeKey_classPrefix)
        {
            serializableData.itemClassSet(key + 1);

            if(jsonItem.jsonType() != ZFJsonType::e_JsonArray)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    zfText("json item %s not type of %s"),
                    jsonItem.objectInfo().cString(),
                    ZFJsonType::EnumNameForValue(ZFJsonType::e_JsonArray));
                if(outErrorPos != zfnull)
                {
                    *outErrorPos = jsonItem;
                }
                return zffalse;
            }
            elementJsonArray = jsonItem;
        }
        else
        {
            if(jsonItem.jsonType() != ZFJsonType::e_JsonValue)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    zfText("json item %s not type of %s"),
                    jsonItem.objectInfo().cString(),
                    ZFJsonType::EnumNameForValue(ZFJsonType::e_JsonValue));
                if(outErrorPos != zfnull)
                {
                    *outErrorPos = jsonItem;
                }
                return zffalse;
            }
            const zfchar *value = jsonItem.jsonValue();

            if(zfscmpTheSame(key, ZFSerializableKeyword_refType))
            {
                serializableData.referenceRefTypeSet(value);
            }
            else if(zfscmpTheSame(key, ZFSerializableKeyword_refData))
            {
                serializableData.referenceRefDataSet(value);
            }
            else
            {
                serializableData.attributeSet(zfstring(key).cString(), value);
            }
        }
    }

    if(serializableData.itemClass() == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, zfText("missing class node (which looks like \"@ClassName\")"));
        if(outErrorPos != zfnull)
        {
            *outErrorPos = jsonObject;
        }
        return zffalse;
    }

    if(!elementJsonArray.jsonIsNull())
    {
        for(zfindex i = 0; i < elementJsonArray.jsonObjectCount(); ++i)
        {
            ZFSerializableData childData;
            if(!_ZFP_ZFJsonParseToSerializableData(childData, elementJsonArray.jsonObjectAtIndex(i), outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            serializableData.elementAdd(childData);
        }
    }

    return zftrue;
}
zfbool ZFJsonParseToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                                     ZF_IN const ZFJsonItem &jsonObject,
                                     ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                     ZF_OUT_OPT ZFJsonItem *outErrorPos /* = zfnull */)
{
    return _ZFP_ZFJsonParseToSerializableData(serializableData, jsonObject, outErrorHint, outErrorPos)
        && serializableData.referenceInfoLoad();
}
ZFSerializableData ZFJsonParseToSerializableData(ZF_IN const ZFJsonItem &jsonObject,
                                                 ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                 ZF_OUT_OPT ZFJsonItem *outErrorPos /* = zfnull */)
{
    ZFSerializableData ret;
    if(ZFJsonParseToSerializableData(ret, jsonObject, outErrorHint, outErrorPos))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}
zfbool ZFJsonPrintFromSerializableData(ZF_OUT ZFJsonItem &jsonObject,
                                       ZF_IN const ZFSerializableData &serializableData,
                                       ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                       ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    jsonObject = ZFJsonPrintFromSerializableData(serializableData, outErrorHint, outErrorPos);
    return !jsonObject.jsonIsNull();
}
ZFJsonItem ZFJsonPrintFromSerializableData(ZF_IN const ZFSerializableData &serializableData,
                                           ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                           ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(serializableData.itemClass() == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, zfText("missing serializable class"));
        return ZFJsonItem();
    }

    ZFJsonItem ret(ZFJsonType::e_JsonObject);

    if(serializableData.referenceRefType() != zfnull)
    {
        ret.jsonItemValueSet(ZFSerializableKeyword_refType, serializableData.referenceRefType());
    }
    if(serializableData.referenceRefData() != zfnull)
    {
        ret.jsonItemValueSet(ZFSerializableKeyword_refData, serializableData.referenceRefData());
    }

    for(zfiterator it = serializableData.attributeIterator();
        serializableData.attributeIteratorIsValid(it);
        serializableData.attributeIteratorNext(it))
    {
        ret.jsonItemValueSet(serializableData.attributeIteratorGetKey(it),
            serializableData.attributeIteratorGet(it));
    }

    ZFJsonItem elementJsonArray(ZFJsonType::e_JsonArray);
    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        ZFJsonItem child = ZFJsonPrintFromSerializableData(serializableData.elementAtIndex(i), outErrorHint, outErrorPos);
        if(child.jsonType() == ZFJsonType::e_JsonNull)
        {
            return ZFJsonItem();
        }
        elementJsonArray.jsonObjectAdd(child);
    }
    zfstring t;
    t += _ZFP_ZFJsonSerializeKey_classPrefix;
    t += serializableData.itemClass();
    ret.jsonItemSet(
        t.cString(),
        elementJsonArray);

    return ret;
}

void ZFJsonPrint(ZF_IN const ZFSerializableData &serializableData,
                 ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */,
                 ZF_IN_OPT const ZFJsonOutputFlags &flags /* = ZFJsonOutputFlagsDefault */)
{
    ZFJsonItem jsonObject = ZFJsonPrintFromSerializableData(serializableData);
    if(!jsonObject.jsonIsNull())
    {
        ZFJsonToOutput(outputCallback, jsonObject, flags);
        outputCallback.execute(zfText("\n"));
    }
}
void ZFJsonPrint(ZF_IN ZFObject *obj,
                 ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */,
                 ZF_IN_OPT const ZFJsonOutputFlags &flags /* = ZFJsonOutputFlagsDefault */)
{
    ZFJsonPrint(ZFObjectToSerializableData(obj), outputCallback, flags);
}

// ============================================================
zfbool ZFJsonParse(ZF_OUT ZFSerializableData &ret, ZF_IN const ZFInputCallback &input)
{
    if(!input.callbackIsValid())
    {
        return zffalse;
    }
    ZFJsonItem jsonElement = ZFJsonFromInput(input);
    if(jsonElement.jsonType() == ZFJsonType::e_JsonNull)
    {
        return zffalse;
    }
    if(!ZFJsonParseToSerializableData(ret, jsonElement))
    {
        return zffalse;
    }

    {
        ZFString *v = input.callbackTagGet<ZFString *>(ZFCallbackTagKeyword_resPath);
        if(v != zfnull)
        {
            ret.serializableDataTagSetMarkCached(ZFSerializableDataTagKeyword_resPath, v);
        }
        else
        {
            v = input.callbackTagGet<ZFString *>(ZFCallbackTagKeyword_filePath);
            if(v != zfnull)
            {
                ret.serializableDataTagSetMarkCached(ZFSerializableDataTagKeyword_filePath, v);
            }
        }
    }
    return zftrue;
}
ZFSerializableData ZFJsonParse(ZF_IN const ZFInputCallback &input)
{
    ZFSerializableData ret;
    if(ZFJsonParse(ret, input))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}

ZF_NAMESPACE_GLOBAL_END

