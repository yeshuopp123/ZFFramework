/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFKeyValueContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFKeyValueContainer)

ZFOBSERVER_EVENT_REGISTER(ZFKeyValueContainer, ContentOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFKeyValueContainer, ContentOnAdd)
ZFOBSERVER_EVENT_REGISTER(ZFKeyValueContainer, ContentOnRemove)

zfbool ZFKeyValueContainer::serializableOnCheck(void)
{
    if(!zfsuperI(ZFSerializable)::serializableOnCheck()) {return zffalse;}

    for(zfiterator it = this->iterator(); this->iteratorIsValid(it); this->iteratorNext(it))
    {
        if(!ZFObjectIsSerializable(this->iteratorGetKey(it))
            || !ZFObjectIsSerializable(this->iteratorGetValue(it)))
        {
            return zffalse;
        }
    }

    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                            ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                            ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    this->removeAll();

    zfautoObject key;
    zfautoObject value;
    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = serializableData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfscmpTheSame(category, ZFSerializableKeyword_ZFKeyValueContainer_key))
        {
            if(key != zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    zfText("missing value for key %s (%s)"),
                    key.toObject()->objectInfoOfInstance().cString(),
                    key.toObject()->objectInfo().cString());
                return zffalse;
            }
            if(!ZFObjectFromSerializableData(key, categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(key == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, zfText("null key"));
                return zffalse;
            }
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFKeyValueContainer_value))
        {
            if(key == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    zfText("missing key"));
                return zffalse;
            }
            if(!ZFObjectFromSerializableData(value, categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(value == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, zfText("null value"));
                return zffalse;
            }
            this->iteratorAddKeyValue(key.toObject(), value.toObject());
            key = zfautoObjectNull;
            value = zfautoObjectNull;
        }
    }
    if(key != zfautoObjectNull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            zfText("missing value for key %s (%s)"),
            key.toObject()->objectInfoOfInstance().cString(),
            key.toObject()->objectInfo().cString());
        return zffalse;
    }

    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                          ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                          ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    if(ref == zfnull)
    {
        for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
        {
            ZFKeyValuePair pair = this->iteratorNextPair(it);

            ZFSerializableData keyData;
            if(!ZFObjectToSerializableData(keyData, pair.key, outErrorHint))
            {
                return zffalse;
            }

            ZFSerializableData valueData;
            if(!ZFObjectToSerializableData(valueData, pair.value, outErrorHint))
            {
                return zffalse;
            }

            keyData.categorySet(ZFSerializableKeyword_ZFKeyValueContainer_key);
            serializableData.elementAdd(keyData);
            valueData.categorySet(ZFSerializableKeyword_ZFKeyValueContainer_value);
            serializableData.elementAdd(valueData);
        }
    }
    else
    {
        return this->serializableOnSerializeToDataWithRef(serializableData, ref, outErrorHint);
    }

    return zftrue;
}

void ZFKeyValueContainer::objectOnDeallocPrepare(void)
{
    this->removeAll();
    zfsuper::objectOnDeallocPrepare();
}

void ZFKeyValueContainer::objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                               ZF_IN_OPT zfindex maxCount /* = zfindexMax */,
                                               ZF_IN_OPT const ZFTokenForKeyValueContainer &token /* = ZFTokenForKeyValueContainerDefault */)
{
    zfindex count = 0;
    ret += token.tokenLeft;
    for(zfiterator it = this->iterator(); this->iteratorIsValid(it) && count < maxCount; ++count)
    {
        ZFKeyValuePair pair = this->iteratorNextPair(it);

        if(count > 0)
        {
            ret += token.tokenSeparator;
        }

        ret += token.tokenPairLeft;
        {
            ret += token.tokenKeyLeft;
            pair.key->objectInfoT(ret);
            ret += token.tokenKeyRight;
        }
        ret += token.tokenPairSeparator;
        {
            ret += token.tokenValueLeft;
            pair.value->objectInfoT(ret);
            ret += token.tokenValueRight;
        }
        ret += token.tokenPairRight;
    }
    if(count < this->count())
    {
        if(count > 0)
        {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    ret += token.tokenRight;
}

zfidentity ZFKeyValueContainer::objectHash(void)
{
    zfiterator it = this->iterator();
    if(this->iteratorIsValid(it))
    {
        return (zfidentity)(~((zfidentity)this->count()
            | ((ZFObjectHash(this->iteratorGetKey(it)) << 16) & 0x00FF0000)
            | ((ZFObjectHash(this->iteratorGetValue(it)) << 24) & 0xFF000000)));
    }
    else
    {
        return zfidentityZero;
    }
}
ZFCompareResult ZFKeyValueContainer::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->count() != another->count()
        || this->objectHash() != another->objectHash())
    {
        return ZFCompareUncomparable;
    }

    for(zfiterator it0 = this->iterator(); this->iteratorIsValid(it0);)
    {
        ZFKeyValuePair pair0 = this->iteratorNextPair(it0);
        zfiterator it1 = another->iteratorForKey(pair0.key);
        if(!another->iteratorIsValid(it1))
        {
            return ZFCompareUncomparable;
        }
        if(ZFObjectCompare(pair0.value, another->iteratorNextValue(it1)) != ZFCompareTheSame)
        {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareTheSame;
}

void ZFKeyValueContainer::objectCachedOnChange(void)
{
    zfsuper::objectCachedOnChange();
    zfbool objectCached = this->objectCached();
    for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
    {
        ZFKeyValuePair pair = this->iteratorNextPair(it);
        pair.key->objectCachedSet(objectCached);
        pair.value->objectCachedSet(objectCached);
    }
}

ZF_NAMESPACE_GLOBAL_END

