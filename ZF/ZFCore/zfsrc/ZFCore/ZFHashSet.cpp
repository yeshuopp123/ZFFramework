/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFHashSet.h"
#include "ZFHashMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFHashSet
ZFOBJECT_REGISTER(ZFHashSet)

zfbool ZFHashSet::serializableOnSerializeToDataWithRef(ZF_IN_OUT ZFSerializableData &serializableData,
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

ZFObject *ZFHashSet::objectOnInit(ZF_IN ZFContainer *another)
{
    this->objectOnInit();
    zfself::addFrom(another);
    return this;
}
ZFObject *ZFHashSet::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAllocWithoutLeakTest(ZFHashMapEditable);
    return this;
}
void ZFHashSet::objectOnDealloc(void)
{
    zfReleaseWithoutLeakTest(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfindex ZFHashSet::count(void)
{
    return d->count();
}
zfbool ZFHashSet::isEmpty(void)
{
    return d->isEmpty();
}
zfbool ZFHashSet::isContain(ZF_IN ZFObject *obj)
{
    return d->isContain(obj);
}
void ZFHashSet::add(ZF_IN ZFObject *obj)
{
    zfCoreAssertWithMessage(obj != zfnull, zfTextA("insert null object"));
    if(!d->isContain(obj))
    {
        d->set(obj, zfnullObject);
        this->contentOnAdd(obj);
        this->contentOnChange();
    }
}
void ZFHashSet::addFrom(ZF_IN ZFContainer *another)
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

void ZFHashSet::removeElement(ZF_IN ZFObject *obj)
{
    ZFKeyValuePairAutoRelease tmp = d->removeAndGetPair(obj);
    if(tmp.key.toObject() != zfnull)
    {
        this->contentOnRemove(tmp.key.toObject());
        this->contentOnChange();
    }
}
void ZFHashSet::removeAll(void)
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
zfiterator ZFHashSet::iterator(void)
{
    return d->iterator();
}

zfiterator ZFHashSet::iteratorFind(ZF_IN ZFObject *value)
{
    return d->iteratorForKey(value);
}

zfbool ZFHashSet::iteratorIsValid(ZF_IN const zfiterator &it)
{
    return d->iteratorIsValid(it);
}
zfbool ZFHashSet::iteratorIsEqual(ZF_IN const zfiterator &it0,
                                  ZF_IN const zfiterator &it1)
{
    return d->iteratorIsEqual(it0, it1);
}

ZFObject *ZFHashSet::iteratorGet(ZF_IN const zfiterator &it)
{
    return d->iteratorGetKey(it);
}

ZFObject *ZFHashSet::iteratorNext(ZF_IN_OUT zfiterator &it)
{
    return d->iteratorNextKey(it);
}
ZFObject *ZFHashSet::iteratorPrev(ZF_IN_OUT zfiterator &it)
{
    return d->iteratorPrevKey(it);
}

void ZFHashSet::iteratorSet(ZF_IN_OUT zfiterator &it,
                            ZF_IN ZFObject *value)
{
    this->add(value);
}
void ZFHashSet::iteratorRemove(ZF_IN_OUT zfiterator &it)
{
    ZFKeyValuePair tmp = d->iteratorGetPair(it);
    if(tmp.key != zfnull)
    {
        this->contentOnRemove(tmp.key);
        d->iteratorRemove(it);
        this->contentOnChange();
    }
}
void ZFHashSet::iteratorAdd(ZF_IN ZFObject *value,
                            ZF_IN_OPT const zfiterator &it /* = zfiteratorInvalid */)
{
    this->add(value);
}

ZF_NAMESPACE_GLOBAL_END

