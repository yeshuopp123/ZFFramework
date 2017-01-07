/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFContainer)

ZFOBSERVER_EVENT_REGISTER(ZFContainer, ContentOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFContainer, ContentOnAdd)
ZFOBSERVER_EVENT_REGISTER(ZFContainer, ContentOnRemove)

zfbool ZFContainer::serializableOnCheck(void)
{
    if(!zfsuperI(ZFSerializable)::serializableOnCheck()) {return zffalse;}
    for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
    {
        if(!ZFObjectIsSerializable(this->iteratorNext(it)))
        {
            return zffalse;
        }
    }
    return zftrue;
}
zfbool ZFContainer::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                    ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                    ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHintToAppend, outErrorPos)) {return zffalse;}

    this->removeAll();

    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = serializableData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfscmpTheSame(category, ZFSerializableKeyword_ZFContainer_element))
        {
            zfautoObject element;
            if(!ZFObjectFromSerializableData(element, categoryData, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            if(element == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                    zfText("null element"));
                return zffalse;
            }
            this->iteratorAdd(element.toObject());

            categoryData.resolveMark();
        }
    }
    return zftrue;
}
zfbool ZFContainer::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                  ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                  ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHintToAppend)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    if(ref == zfnull)
    {
        for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
        {
            ZFSerializableData elementData;
            if(!ZFObjectToSerializableData(elementData, this->iteratorNext(it), outErrorHintToAppend))
            {
                return zffalse;
            }
            elementData.categorySet(ZFSerializableKeyword_ZFContainer_element);
            serializableData.elementAdd(elementData);
        }
    }
    else
    {
        return this->serializableOnSerializeToDataWithRef(serializableData, ref, outErrorHintToAppend);
    }

    return zftrue;
}

void ZFContainer::objectOnDeallocPrepare(void)
{
    this->removeAll();
    zfsuper::objectOnDeallocPrepare();
}

void ZFContainer::objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                       ZF_IN_OPT zfindex maxCount /* = zfindexMax */,
                                       ZF_IN_OPT const ZFTokenForContainer &token /* = ZFTokenForContainerDefault */)
{
    zfindex count = 0;
    ret += token.tokenLeft;
    zfiterator it = this->iterator();
    for(; this->iteratorIsValid(it) && count < maxCount; ++count)
    {
        if(count > 0)
        {
            ret += token.tokenSeparator;
        }
        ret += token.tokenValueLeft;
        this->iteratorNext(it)->objectInfoT(ret);
        ret += token.tokenValueRight;
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

zfidentity ZFContainer::objectHash(void)
{
    ZFObject *first = zfnull;
    {
        zfiterator it = this->iterator();
        if(this->iteratorIsValid(it))
        {
            first = this->iteratorGet(it);
        }
    }
    if(first != zfnull)
    {
        return zfidentityHash(this->count(), first->objectHash());
    }
    else
    {
        return 0;
    }
}
ZFCompareResult ZFContainer::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->count() != another->count()
        || this->objectHash() != another->objectHash())
    {
        return ZFCompareUncomparable;
    }
    for(zfiterator it = this->iterator(), itRef = another->iterator(); this->iteratorIsValid(it);)
    {
        if(ZFObjectCompare(this->iteratorNext(it), another->iteratorNext(itRef)) != ZFCompareTheSame)
        {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareTheSame;
}

void ZFContainer::objectCachedOnChange(void)
{
    zfsuper::objectCachedOnChange();
    zfbool objectCached = this->objectCached();
    for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
    {
        this->iteratorNext(it)->objectCachedSet(objectCached);
    }
}

ZF_NAMESPACE_GLOBAL_END

