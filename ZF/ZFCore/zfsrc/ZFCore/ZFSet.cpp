/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFSet.h"
#include "ZFMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFSet
ZFOBJECT_REGISTER(ZFSet)

zfbool ZFSet::serializableOnSerializeToDataWithRef(ZF_IN_OUT ZFSerializableData &serializableData,
                                                   ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                   ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFContainer *ref = ZFCastZFObject(ZFContainer *, referencedOwnerOrNull);
    if(ref == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("%s not type of %s"),
            referencedOwnerOrNull->toObject()->objectInfoOfInstance().cString(), ZFContainer::ClassData()->className());
        return zffalse;
    }

    zfblockedAllocWithoutLeakTest(zfself, tmp, ref);
    for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
    {
        ZFObject *element = this->iteratorNext(it);
        zfiterator itTmp = tmp->iteratorFind(element);
        if(ref->iteratorIsValid(itTmp))
        {
            tmp->iteratorRemove(itTmp);
            continue;
        }

        ZFSerializableData elementData;
        if(!ZFObjectToSerializableData(elementData, element, outErrorHint))
        {
            return zffalse;
        }
        elementData.categorySet(ZFSerializableKeyword_ZFContainer_element);
        serializableData.elementAdd(elementData);
    }

    if(!tmp->isEmpty())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("missing elements from referenced container: %s"), tmp->objectInfoOfContent().cString());
        return zffalse;
    }

    return zftrue;
}

ZFObject *ZFSet::objectOnInit(ZF_IN ZFContainer *another)
{
    this->objectOnInit();
    zfself::addFrom(another);
    return this;
}
ZFObject *ZFSet::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAllocWithoutLeakTest(ZFMapEditable);
    return this;
}
void ZFSet::objectOnDealloc(void)
{
    zfReleaseWithoutLeakTest(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfindex ZFSet::count(void)
{
    return d->count();
}
zfbool ZFSet::isEmpty(void)
{
    return d->isEmpty();
}
zfbool ZFSet::isContain(ZF_IN ZFObject *obj)
{
    return d->isContain(obj);
}
void ZFSet::add(ZF_IN ZFObject *obj)
{
    zfCoreAssertWithMessage(obj != zfnull, zfTextA("insert null object"));
    if(!d->isContain(obj))
    {
        d->set(obj, zfnullObject);
        this->contentOnAdd(obj);
        this->contentOnChange();
    }
}
void ZFSet::addFrom(ZF_IN ZFContainer *another)
{
    if(another == this || another == zfnull)
    {
        return ;
    }

    for(zfiterator it = another->iterator(); another->iteratorIsValid(it);)
    {
        ZFObject *obj = another->iteratorNext(it);
        if(!d->isContain(obj))
        {
            d->set(obj, zfnullObject);
            this->contentOnAdd(obj);
        }
    }

    this->contentOnChange();
}

void ZFSet::removeElement(ZF_IN ZFObject *obj)
{
    ZFKeyValuePairAutoRelease tmp = d->removeAndGetPair(obj);
    if(tmp.key.toObject() != zfnull)
    {
        this->contentOnRemove(tmp.key.toObject());
        this->contentOnChange();
    }
}
void ZFSet::removeAll(void)
{
    if(!d->isEmpty())
    {
        for(zfiterator it = d->iterator(); d->iteratorIsValid(it); )
        {
            this->contentOnRemove(d->iteratorNextKey(it));
        }
        d->removeAll();

        this->contentOnChange();
    }
}

// ============================================================
zfiterator ZFSet::iterator(void)
{
    return d->iterator();
}

zfiterator ZFSet::iteratorFind(ZF_IN ZFObject *value)
{
    return d->iteratorForKey(value);
}

zfbool ZFSet::iteratorIsValid(ZF_IN const zfiterator &it)
{
    return d->iteratorIsValid(it);
}
zfbool ZFSet::iteratorIsEqual(ZF_IN const zfiterator &it0,
                              ZF_IN const zfiterator &it1)
{
    return d->iteratorIsEqual(it0, it1);
}

ZFObject *ZFSet::iteratorGet(ZF_IN const zfiterator &it)
{
    return d->iteratorGetKey(it);
}

ZFObject *ZFSet::iteratorNext(ZF_IN_OUT zfiterator &it)
{
    return d->iteratorNextKey(it);
}
ZFObject *ZFSet::iteratorPrev(ZF_IN_OUT zfiterator &it)
{
    return d->iteratorPrevKey(it);
}

void ZFSet::iteratorSet(ZF_IN_OUT zfiterator &it,
                        ZF_IN ZFObject *value)
{
    this->add(value);
}
void ZFSet::iteratorRemove(ZF_IN_OUT zfiterator &it)
{
    ZFKeyValuePair tmp = d->iteratorGetPair(it);
    if(tmp.key != zfnull)
    {
        this->contentOnRemove(tmp.key);
        d->iteratorRemove(it);
        this->contentOnChange();
    }
}
void ZFSet::iteratorAdd(ZF_IN ZFObject *value,
                        ZF_IN_OPT const zfiterator &it /* = zfiteratorInvalid */)
{
    this->add(value);
}

ZF_NAMESPACE_GLOBAL_END

