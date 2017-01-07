/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFArray.h"
#include "ZFSTLWrapper/zfstl_deque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFArrayPrivate
zfclassNotPOD _ZFP_ZFArrayPrivate
{
public:
    zfstldeque<ZFObject *> data;
};

// ============================================================
// ZFArray
ZFOBJECT_REGISTER(ZFArray)

zfbool ZFArray::serializableOnSerializeToDataWithRef(ZF_IN_OUT ZFSerializableData &serializableData,
                                                     ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                     ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);
    if(ref == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("%s not type of %s"),
            referencedOwnerOrNull->toObject()->objectInfoOfInstance().cString(), zfself::ClassData()->className());
        return zffalse;
    }
    zfbool success = zftrue;
    if(this->count() != ref->count())
    {
        success = zffalse;
    }
    else
    {
        for(zfiterator it = this->iterator(), itRef = ref->iterator(); this->iteratorIsValid(it); )
        {
            if(ZFObjectCompare(this->iteratorGet(it), ref->iteratorGet(itRef)) != ZFCompareTheSame)
            {
                success = zffalse;
                break;
            }
        }
    }
    if(!success)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("array contents mismatch, this: %s, ref: %s"),
            this->objectInfoOfContent().cString(), ref->objectInfoOfContent().cString());
        return zffalse;
    }
    return zftrue;
}

ZFObject *ZFArray::objectOnInit(ZF_IN ZFContainer *another)
{
    this->objectOnInit();
    zfself::addFrom(another);
    return this;
}
ZFObject *ZFArray::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFArrayPrivate);
    return this;
}
void ZFArray::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfindex ZFArray::count(void)
{
    return (zfindex)(d->data.size());
}
zfbool ZFArray::isEmpty(void)
{
    return d->data.empty();
}
ZFObject *ZFArray::get(ZF_IN zfindex index)
{
    if(index >= d->data.size())
    {
        zfCoreCriticalIndexOutOfRange(index, d->data.size());
        return zfnull;
    }
    return d->data[index];
}
ZFObject *ZFArray::getFirst(void)
{
    if(d->data.empty())
    {
        return zfnull;
    }
    return d->data[0];
}
ZFObject *ZFArray::getLast(void)
{
    if(d->data.empty())
    {
        return zfnull;
    }
    return d->data[d->data.size() - 1];
}
zfbool ZFArray::isContain(ZF_IN ZFObject *obj,
                          ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer /* = ZFComparerCheckEqualOnly */)
{
    return (this->find(obj, comparer) != zfindexMax);
}
zfindex ZFArray::find(ZF_IN ZFObject *obj,
                      ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer /* = ZFComparerCheckEqualOnly */)
{
    if(comparer == zfnull)
    {
        return zfindexMax;
    }
    for(zfstlsize i = 0; i < d->data.size(); ++i)
    {
        if(comparer(d->data[i], obj) == ZFCompareTheSame)
        {
            return (zfindex)i;
        }
    }
    return zfindexMax;
}
zfindex ZFArray::findReversely(ZF_IN ZFObject *obj,
                               ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer /* = ZFComparerCheckEqualOnly */)
{
    if(comparer == zfnull)
    {
        return zfindexMax;
    }
    for(zfstlsize i = d->data.size() - 1; i != (zfstlsize)-1; --i)
    {
        if(comparer(d->data[i], obj) == ZFCompareTheSame)
        {
            return (zfindex)i;
        }
    }
    return zfindexMax;
}
void ZFArray::add(ZF_IN zfindex indexAddTo,
                  ZF_IN ZFObject *obj)
{
    zfCoreAssertWithMessage(obj != zfnull, zfTextA("insert null object"));
    zfRetain(obj);
    d->data.insert(d->data.begin() + indexAddTo, obj);

    this->contentOnAdd(obj);
    this->contentOnChange();
}
void ZFArray::add(ZF_IN ZFObject *obj)
{
    zfCoreAssertWithMessage(obj != zfnull, zfTextA("insert null object"));
    zfRetain(obj);
    d->data.push_back(obj);

    this->contentOnAdd(obj);
    this->contentOnChange();
}
void ZFArray::addFrom(ZF_IN ZFContainer *another)
{
    if(another == zfnull)
    {
        return ;
    }
    if(this == another)
    {
        zfstldeque<ZFObject *> tmp = d->data;
        ZFObject *obj = zfnull;
        for(zfstldeque<ZFObject *>::const_iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            obj = *it;
            zfRetain(obj);
            d->data.push_back(obj);
            this->contentOnAdd(obj);
        }
        if(obj != zfnull)
        {
            this->contentOnChange();
        }
    }
    else
    {
        ZFObject *obj = zfnull;
        for(zfiterator it = another->iterator(); another->iteratorIsValid(it);)
        {
            obj = another->iteratorNext(it);
            zfRetain(obj);
            d->data.push_back(obj);
            this->contentOnAdd(obj);
        }
        if(obj != zfnull)
        {
            this->contentOnChange();
        }
    }
}

void ZFArray::set(ZF_IN zfindex index,
                  ZF_IN ZFObject *obj)
{
    zfCoreAssertWithMessage(obj != zfnull, zfTextA("set null object"));
    zfRetain(obj);

    ZFObject *old = d->data[index];
    d->data[index] = obj;

    this->contentOnRemove(old);
    this->contentOnAdd(obj);
    this->contentOnChange();

    zfRelease(old);
}

zfbool ZFArray::removeElement(ZF_IN ZFObject *obj)
{
    if(obj)
    {
        for(zfstldeque<ZFObject *>::iterator it = d->data.begin(); it != d->data.end(); )
        {
            if((*it)->objectCompare(obj) == ZFCompareTheSame)
            {
                ZFObject *toRelease = *it;
                it = d->data.erase(it);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
                this->contentOnChange();
                return zftrue;
            }
            else
            {
                ++it;
            }
        }
    }
    return zffalse;
}
zfbool ZFArray::removeElement(ZF_IN ZFObject *obj, ZF_IN ZFComparer<ZFObject *>::Comparer comparer)
{
    if(obj && comparer)
    {
        for(zfstldeque<ZFObject *>::iterator it = d->data.begin(); it != d->data.end(); )
        {
            if(comparer(*it, obj) == ZFCompareTheSame)
            {
                ZFObject *toRelease = *it;
                it = d->data.erase(it);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
                this->contentOnChange();
                return zftrue;
            }
            else
            {
                ++it;
            }
        }
    }
    return zffalse;
}
zfbool ZFArray::removeElementRevsersely(ZF_IN ZFObject *obj)
{
    if(obj)
    {
        for(zfstlsize i = d->data.size() - 1; i != (zfstlsize)-1; --i)
        {
            if(d->data[i]->objectCompare(obj) == ZFCompareTheSame)
            {
                ZFObject *toRelease = d->data[i];
                d->data.erase(d->data.begin() + i);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
                this->contentOnChange();
                return zftrue;
            }
        }
    }
    return zffalse;
}
zfbool ZFArray::removeElementRevsersely(ZF_IN ZFObject *obj, ZF_IN ZFComparer<ZFObject *>::Comparer comparer)
{
    if(obj && comparer)
    {
        for(zfstlsize i = d->data.size() - 1; i != (zfstlsize)-1; --i)
        {
            if(comparer(d->data[i], obj) == ZFCompareTheSame)
            {
                ZFObject *toRelease = d->data[i];
                d->data.erase(d->data.begin() + i);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
                this->contentOnChange();
                return zftrue;
            }
        }
    }
    return zffalse;
}
zfindex ZFArray::removeElementAll(ZF_IN ZFObject *obj)
{
    zfindex removedCount = 0;
    if(obj)
    {
        for(zfstldeque<ZFObject *>::iterator it = d->data.begin(); it != d->data.end(); )
        {
            if((*it)->objectCompare(obj) == ZFCompareTheSame)
            {
                ++removedCount;
                ZFObject *toRelease = *it;
                it = d->data.erase(it);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
            }
            else
            {
                ++it;
            }
        }
    }
    if(removedCount > 0)
    {
        this->contentOnChange();
    }
    return removedCount;
}
zfindex ZFArray::removeElementAll(ZF_IN ZFObject *obj, ZF_IN ZFComparer<ZFObject *>::Comparer comparer)
{
    zfindex removedCount = 0;
    if(obj && comparer)
    {
        for(zfstldeque<ZFObject *>::iterator it = d->data.begin(); it != d->data.end(); )
        {
            if(comparer(*it, obj) == ZFCompareTheSame)
            {
                ++removedCount;
                ZFObject *toRelease = *it;
                it = d->data.erase(it);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
            }
            else
            {
                ++it;
            }
        }
    }
    if(removedCount > 0)
    {
        this->contentOnChange();
    }
    return removedCount;
}

void ZFArray::remove(ZF_IN zfindex index,
                     ZF_IN_OPT zfindex count /* = 1 */)
{
    if(count == 1)
    {
        ZFObject *tmp = d->data[index];
        d->data.erase(d->data.begin() + index);
        zfRelease(tmp);

        this->contentOnRemove(tmp);
        this->contentOnChange();
    }
    else if(count > 1)
    {
        if(count > this->count() - index)
        {
            count = this->count() - index;
        }
        zfstldeque<ZFObject *> tmp(
            d->data.begin() + index,
            d->data.begin() + (index + count));
        d->data.erase(d->data.begin() + index, d->data.begin() + (index + count));
        for(zfstldeque<ZFObject *>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            this->contentOnRemove(*it);
            zfRelease(*it);
        }

        if(!tmp.empty())
        {
            this->contentOnChange();
        }
    }
}
void ZFArray::removeFirst(void)
{
    if(!d->data.empty())
    {
        ZFObject *tmp = d->data[0];
        d->data.pop_front();
        zfRelease(tmp);

        this->contentOnRemove(tmp);
        this->contentOnChange();
    }
}
void ZFArray::removeLast(void)
{
    if(!d->data.empty())
    {
        ZFObject *tmp = d->data[d->data.size() - 1];
        d->data.pop_back();
        zfRelease(tmp);

        this->contentOnRemove(tmp);
        this->contentOnChange();
    }
}
void ZFArray::removeAll(void)
{
    if(!d->data.empty())
    {
        zfstldeque<ZFObject *> tmp;
        tmp.swap(d->data);

        this->contentOnChange();

        for(zfstldeque<ZFObject *>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            this->contentOnRemove(*it);
            zfRelease(*it);
        }
    }
}

void ZFArray::move(ZF_IN zfindex fromIndex, ZF_IN zfindex toIndexOrIndexMax)
{
    if(fromIndex >= (zfindex)d->data.size())
    {
        zfCoreCriticalIndexOutOfRange(fromIndex, (zfindex)d->data.size());
        return ;
    }
    if(toIndexOrIndexMax == zfindexMax)
    {
        toIndexOrIndexMax = (zfindex)(d->data.size() - 1);
    }
    if(toIndexOrIndexMax >= (zfindex)d->data.size())
    {
        zfCoreCriticalIndexOutOfRange(toIndexOrIndexMax, (zfindex)d->data.size());
        return ;
    }
    if(fromIndex == toIndexOrIndexMax)
    {
        return ;
    }
    ZFObject *t = d->data[fromIndex];
    if(fromIndex < toIndexOrIndexMax)
    {
        for(zfindex i = fromIndex; i < toIndexOrIndexMax; ++i)
        {
            d->data[i] = d->data[i + 1];
        }
    }
    else
    {
        for(zfindex i = fromIndex; i > toIndexOrIndexMax; --i)
        {
            d->data[i] = d->data[i - 1];
        }
    }
    d->data[toIndexOrIndexMax] = t;

    this->contentOnChange();
}

void ZFArray::sort(ZF_IN_OPT zfbool ascending /* = zftrue */,
                   ZF_IN_OPT zfindex start /* = 0 */,
                   ZF_IN_OPT zfindex count /* = zfindexMax */,
                   ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer /* = ZFComparerCheckEqualOnly */)
{
    if(d->data.size() > 0 && start + 1 < d->data.size() && count > 1)
    {
        zfmSort<ZFObject *>(
            d->data,
            comparer,
            start,
            (count > d->data.size() - start) ? (d->data.size() - 1) : (start + count - 1),
            ascending);

        this->contentOnChange();
    }
}

// ============================================================
static void _ZFP_ZFArray_iteratorDeleteCallback(ZF_IN void *data)
{
    zfdelete((zfindex *)data);
}
static void *_ZFP_ZFArray_iteratorCopyCallback(ZF_IN void *data)
{
    return zfnew(zfindex, *(zfindex *)data);
}

zfiterator ZFArray::iteratorForIndex(ZF_IN zfindex index)
{
    return zfiterator(zfnew(zfindex, index),
        _ZFP_ZFArray_iteratorDeleteCallback,
        _ZFP_ZFArray_iteratorCopyCallback);
}
zfiterator ZFArray::iteratorFind(ZF_IN ZFObject *value,
                                 ZF_IN ZFComparer<ZFObject *>::Comparer comparer)
{
    return this->iteratorForIndex(this->find(value, comparer));
}
zfiterator ZFArray::iteratorFindReversely(ZF_IN ZFObject *value,
                                          ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer /* = ZFComparerCheckEqualOnly */)
{
    return this->iteratorForIndex(this->findReversely(value, comparer));
}

zfiterator ZFArray::iterator(void)
{
    return zfiterator(zfnew(zfindex, 0),
        _ZFP_ZFArray_iteratorDeleteCallback,
        _ZFP_ZFArray_iteratorCopyCallback);
}

zfiterator ZFArray::iteratorFind(ZF_IN ZFObject *value)
{
    return this->iteratorForIndex(this->find(value));
}

zfbool ZFArray::iteratorIsValid(ZF_IN const zfiterator &it)
{
    zfindex *index = it.data<zfindex *>();
    return (index != zfnull && *index < d->data.size());
}
zfbool ZFArray::iteratorIsEqual(ZF_IN const zfiterator &it0,
                                ZF_IN const zfiterator &it1)
{
    return zfiterator::iteratorIsEqual<zfindex *>(it0, it1);
}

ZFObject *ZFArray::iteratorGet(ZF_IN const zfiterator &it)
{
    zfindex *index = it.data<zfindex *>();
    if(index != zfnull && *index < d->data.size())
    {
        return d->data[*index];
    }
    return zfnull;
}

ZFObject *ZFArray::iteratorNext(ZF_IN_OUT zfiterator &it)
{
    zfindex *index = it.data<zfindex *>();
    if(index != zfnull && *index < d->data.size())
    {
        ++(*index);
        return d->data[*index - 1];
    }
    return zfnull;
}
ZFObject *ZFArray::iteratorPrev(ZF_IN_OUT zfiterator &it)
{
    zfindex *index = it.data<zfindex *>();
    if(index != zfnull && *index < d->data.size())
    {
        --(*index);
        return d->data[*index + 1];
    }
    return zfnull;
}

void ZFArray::iteratorSet(ZF_IN_OUT zfiterator &it,
                          ZF_IN ZFObject *value)
{
    zfindex *index = it.data<zfindex *>();
    if(index != zfnull && *index < d->data.size())
    {
        this->set(*index, value);
    }
}
void ZFArray::iteratorRemove(ZF_IN_OUT zfiterator &it)
{
    zfindex *index = it.data<zfindex *>();
    if(index != zfnull && *index < d->data.size())
    {
        this->remove(*index);
        if(*index >= (zfindex)(d->data.size()))
        {
            *index = (zfindex)(d->data.size()) - 1;
        }
    }
}
void ZFArray::iteratorAdd(ZF_IN ZFObject *value,
                          ZF_IN_OPT const zfiterator &it /* = zfiteratorInvalid */)
{
    zfindex *index = it.data<zfindex *>();
    if(index != zfnull && *index < d->data.size())
    {
        this->add((*index)++, value);
    }
    else
    {
        this->add(value);
    }
}

ZF_NAMESPACE_GLOBAL_END

