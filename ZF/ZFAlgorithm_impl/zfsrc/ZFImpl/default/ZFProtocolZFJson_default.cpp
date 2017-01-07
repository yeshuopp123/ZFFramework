/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFAlgorithm.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFJson.h"

#include "ZFImpl/_repo/rapidjson/document.h"
#include "ZFImpl/_repo/rapidjson/reader.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)

zfclassNotPOD _ZFP_ZFJsonImpl_default_MemoryPoolHolder
{
public:
    zfindex refCount;
    ZFBuffer buf;
    rapidjson::Document implJsonDoc;
public:
    _ZFP_ZFJsonImpl_default_MemoryPoolHolder(void)
    : refCount(0)
    , buf()
    , implJsonDoc()
    {
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFJsonImpl_default, ZFJson, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("rapidjson"))
public:
    virtual ZFJsonItem jsonParse(ZF_IN const zfchar *src,
                                 ZF_IN_OPT zfindex size = zfindexMax,
                                 ZF_IN_OPT const ZFJsonParseFlags &flags = ZFJsonParseFlagsDefault)
    {
        ZFBuffer buf;
        buf.bufferCopy(src, size);
        return this->jsonParse(buf, flags);
    }
    virtual ZFJsonItem jsonParse(ZF_IN const ZFInputCallback &inputCallback,
                                 ZF_IN_OPT const ZFJsonParseFlags &flags = ZFJsonParseFlagsDefault)
    {
        ZFBuffer buf = ZFInputCallbackReadToBuffer(inputCallback);
        return this->jsonParse(buf, flags);
    }
    virtual void jsonMemoryPoolRelease(ZF_IN void *token, ZF_IN const zfchar *value)
    {
        _ZFP_ZFJsonImpl_default_MemoryPoolHolder *docHolder = (_ZFP_ZFJsonImpl_default_MemoryPoolHolder *)token;
        --(docHolder->refCount);
        if(docHolder->refCount == 0)
        {
            zfdelete(docHolder);
        }
    }
private:
    ZFJsonItem jsonParse(ZF_IN_OUT ZFBuffer &buf,
                         ZF_IN_OPT const ZFJsonParseFlags &flags = ZFJsonParseFlagsDefault)
    {
        if(buf.buffer() == zfnull)
        {
            return ZFJsonItem();
        }
        _ZFP_ZFJsonImpl_default_MemoryPoolHolder *docHolder = zfnew(_ZFP_ZFJsonImpl_default_MemoryPoolHolder);
        docHolder->buf = buf;
        docHolder->implJsonDoc.ParseInsitu<rapidjson::kParseNumbersAsStringsFlag>(buf.bufferAsString());
        if(docHolder->implJsonDoc.HasParseError())
        {
            return ZFJsonItem();
        }
        ZFJsonItem ret = this->jsonConvert(docHolder->implJsonDoc, docHolder);
        if(docHolder->refCount == 0)
        {
            zfdelete(docHolder);
        }
        return ret;
    }
private:
    ZFJsonItem jsonConvert(ZF_IN const rapidjson::Value &implJsonItem,
                           ZF_IN _ZFP_ZFJsonImpl_default_MemoryPoolHolder *docHolder)
    {
        switch(implJsonItem.GetType())
        {
            case rapidjson::kNullType:
                return ZFJsonItem();
            case rapidjson::kFalseType:
            case rapidjson::kTrueType:
            case rapidjson::kStringType:
            case rapidjson::kNumberType:
            {
                ZFJsonItem jsonValue(ZFJsonType::e_JsonValue);
                ++(docHolder->refCount);
                this->jsonMemoryPool_jsonValueSet(jsonValue, implJsonItem.GetString(), docHolder);
                return jsonValue;
            }
            case rapidjson::kArrayType:
            {
                ZFJsonItem jsonArray(ZFJsonType::e_JsonArray);
                for(rapidjson::Value::ConstValueIterator it = implJsonItem.Begin(); it != implJsonItem.End(); ++it)
                {
                    ZFJsonItem jsonChild = this->jsonConvert(*it, docHolder);
                    if(jsonChild.jsonIsNull())
                    {
                        return ZFJsonItem();
                    }
                    jsonArray.jsonObjectAdd(jsonChild);
                }
                return jsonArray;
            }
            case rapidjson::kObjectType:
            {
                ZFJsonItem jsonObject(ZFJsonType::e_JsonObject);
                for(rapidjson::Value::ConstMemberIterator it = implJsonItem.MemberBegin(); it != implJsonItem.MemberEnd(); ++it)
                {
                    ZFJsonItem jsonChild = this->jsonConvert(it->value, docHolder);
                    if(jsonChild.jsonIsNull())
                    {
                        return ZFJsonItem();
                    }
                    ++(docHolder->refCount);
                    this->jsonMemoryPool_jsonItemSet(jsonObject, it->name.GetString(), docHolder, jsonChild);
                }
                return jsonObject;
            }
            default:
                zfCoreCriticalShouldNotGoHere();
                return ZFJsonItem();
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFJsonImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFJsonImpl_default)

ZF_NAMESPACE_GLOBAL_END

