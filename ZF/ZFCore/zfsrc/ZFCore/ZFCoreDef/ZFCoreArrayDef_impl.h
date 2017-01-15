/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreArrayDef_impl.h
 * @brief light weight array
 */

#ifndef _ZFI_ZFCoreArrayDef_impl_h_
#define _ZFI_ZFCoreArrayDef_impl_h_

#include "ZFCoreArrayDef.h"

#define _ZFP_ZFCoreArrayMinCapacity 8

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
template<typename T_Element>
static void _ZFP_ZFCoreArray_objCreate(ZF_IN T_Element *p, ZF_IN T_Element *pEnd, ZF_IN zfbool PODType)
{
    if(!PODType)
    {
        while(p != pEnd)
        {
            zfnewPlacement(p, T_Element);
            ++p;
        }
    }
}
template<typename T_Element>
static void _ZFP_ZFCoreArray_objCreate(ZF_IN T_Element *p, ZF_IN T_Element *pEnd,
                                       ZF_IN const T_Element *src,
                                       ZF_IN zfbool PODType)
{
    if(PODType)
    {
        zfmemcpy(p, src, (pEnd - p) * sizeof(T_Element));
    }
    else
    {
        while(p != pEnd)
        {
            zfnewPlacement(p, T_Element, *src);
            ++p;
            ++src;
        }
    }
}
template<typename T_Element>
static void _ZFP_ZFCoreArray_objMove(ZF_IN T_Element *dst,
                                     ZF_IN const T_Element *src,
                                     ZF_IN zfindex count,
                                     ZF_IN zfbool PODType)
{
    if(PODType)
    {
        zfmemmove(dst, src, count * sizeof(T_Element));
    }
    else
    {
        zfmemmoveObject(dst, src, count);
    }
}
template<typename T_Element>
static void _ZFP_ZFCoreArray_objDestroy(ZF_IN T_Element *p, ZF_IN T_Element *pEnd,
                                        ZF_IN zfbool PODType)
{
    if(!PODType)
    {
        while(p != pEnd)
        {
            zfdeletePlacement(p);
            ++p;
        }
    }
}
template<typename T_Element>
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCoreArrayPrivate
{
public:
    zfindex refCount;
    zfbool PODType;
    T_Element *buf;
    T_Element *bufEnd;
    T_Element *contentEnd;
    typename ZFCoreInfoGetter<T_Element>::InfoGetter customInfoGetter;
public:
    _ZFP_ZFCoreArrayPrivate(void)
    : refCount(1)
    , PODType(zffalse)
    , buf(zfnull)
    , bufEnd(zfnull)
    , contentEnd(zfnull)
    , customInfoGetter(zfnull)
    {
    }
    ~_ZFP_ZFCoreArrayPrivate(void)
    {
        this->cleanup();
    }
    void cleanup(void)
    {
        if(this->buf)
        {
            _ZFP_ZFCoreArray_objDestroy(this->buf, this->contentEnd, this->PODType);
            zffree(this->buf);
            this->buf = this->bufEnd = this->contentEnd = zfnull;
        }
    }
};

// ============================================================
template<typename T_Element>
ZFCoreArray<T_Element>::ZFCoreArray(void)
{
    d = zfnew(_ZFP_ZFCoreArrayPrivate<T_Element>);
}
template<typename T_Element>
ZFCoreArray<T_Element>::ZFCoreArray(ZF_IN zfindex capacity)
{
    d = zfnew(_ZFP_ZFCoreArrayPrivate<T_Element>);
    this->capacitySet(capacity);
}
template<typename T_Element>
ZFCoreArray<T_Element>::ZFCoreArray(ZF_IN const ZFCoreArray<T_Element> &ref)
: d(ref.d)
{
    ++(d->refCount);
}
template<typename T_Element>
ZFCoreArray<T_Element>::~ZFCoreArray(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}
template<typename T_Element>
ZFCoreArray<T_Element> &ZFCoreArray<T_Element>::operator =(ZF_IN const ZFCoreArray<T_Element> &ref)
{
    _ZFP_ZFCoreArrayPrivate<T_Element> *dTmp = d;
    d = ref.d;
    ++(ref.d->refCount);
    --(dTmp->refCount);
    if(dTmp->refCount == 0)
    {
        zfdelete(dTmp);
    }
    return *this;
}
template<typename T_Element>
zfbool ZFCoreArray<T_Element>::operator ==(ZF_IN const ZFCoreArray<T_Element> &ref) const
{
    return (d == ref.d);
}
template<typename T_Element>
zfbool ZFCoreArray<T_Element>::operator !=(ZF_IN const ZFCoreArray<T_Element> &ref) const
{
    return (d != ref.d);
}
template<typename T_Element>
void ZFCoreArray<T_Element>::copyFrom(ZF_IN const ZFCoreArray<T_Element> &ref)
{
    if(d != ref.d)
    {
        d->cleanup();
        this->capacitySet(ref.count());
        _ZFP_ZFCoreArray_objCreate(d->buf, d->buf + ref.count(), ref.arrayBuf(), d->PODType);
        d->contentEnd = d->buf + ref.count();
    }
}
template<typename T_Element>
inline zfindex ZFCoreArray<T_Element>::objectRetainCount(void) const
{
    return d->refCount;
}

template<typename T_Element>
void ZFCoreArray<T_Element>::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    this->objectInfoOfContentT(ret, zfnull, 10);
}

template<typename T_Element>
void ZFCoreArray<T_Element>::objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                                  ZF_IN typename ZFCoreInfoGetter<T_Element>::InfoGetter elementInfoGetter,
                                                  ZF_IN_OPT zfindex maxCount /* = zfindexMax */,
                                                  ZF_IN_OPT const ZFTokenForContainer &token /* = ZFTokenForContainerDefault */) const
{
    zfindex count = 0;
    ret += token.tokenLeft;
    for(; count < this->count() && count < maxCount; ++count)
    {
        if(count > 0)
        {
            ret += token.tokenSeparator;
        }
        ret += token.tokenValueLeft;
        if(d->customInfoGetter != zfnull)
        {
            d->customInfoGetter(ret, this->get(count));
        }
        else if(elementInfoGetter != zfnull)
        {
            elementInfoGetter(ret, this->get(count));
        }
        else
        {
            ret += ZFTOKEN_ZFCoreInfoGetterNotAvailable;
        }
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

template<typename T_Element>
void ZFCoreArray<T_Element>::customInfoGetterSet(ZF_IN typename ZFCoreInfoGetter<T_Element>::InfoGetter customInfoGetter)
{
    d->customInfoGetter = customInfoGetter;
}
template<typename T_Element>
typename ZFCoreInfoGetter<T_Element>::InfoGetter ZFCoreArray<T_Element>::customInfoGetter(void) const
{
    return d->customInfoGetter;
}

template<typename T_Element>
void ZFCoreArray<T_Element>::capacitySet(ZF_IN zfindex newCapacity)
{
    zfindex fixedCapacity = zfmMax<zfindex>(this->capacity(), _ZFP_ZFCoreArrayMinCapacity);
    while(fixedCapacity < newCapacity)
    {
        fixedCapacity *= 2;
    }
    this->capacityDoChange(fixedCapacity);
}
template<typename T_Element>
void ZFCoreArray<T_Element>::capacityTrim(void)
{
    if(this->capacity() > _ZFP_ZFCoreArrayMinCapacity
        && this->capacity() > this->count() * 2)
    {
        zfindex fixedCapacity = this->capacity();
        zfindex end = zfmMax<zfindex>(this->count(), _ZFP_ZFCoreArrayMinCapacity) * 2;
        while(fixedCapacity >= end) {fixedCapacity /= 2;}
        this->capacityDoChange(fixedCapacity);
    }
}
template<typename T_Element>
inline zfindex ZFCoreArray<T_Element>::capacity(void) const
{
    return (d->bufEnd - d->buf);
}

template<typename T_Element>
inline void ZFCoreArray<T_Element>::add(ZF_IN T_Element const &e)
{
    T_Element t = e;
    this->capacitySet(this->count() + 1);
    _ZFP_ZFCoreArray_objCreate(d->contentEnd, d->contentEnd + 1, &t, d->PODType);
    ++(d->contentEnd);
}
template<typename T_Element>
inline void ZFCoreArray<T_Element>::add(ZF_IN zfindex index,
                                        ZF_IN T_Element const &e)
{
    if(index > this->count())
    {
        zfCoreCriticalIndexOutOfRange(index, this->count() + 1);
        return ;
    }
    T_Element t = e;
    this->capacitySet(this->count() + 1);
    _ZFP_ZFCoreArray_objCreate(d->contentEnd, d->contentEnd + 1, &t, d->PODType);
    T_Element *pos = d->buf + index;
    _ZFP_ZFCoreArray_objMove(pos + 1, pos, this->count() - index, d->PODType);
    ++(d->contentEnd);
    *pos = e;
}
template<typename T_Element>
void ZFCoreArray<T_Element>::addFrom(ZF_IN const T_Element *src,
                                     ZF_IN zfindex count)
{
    if(src == zfnull || count == 0)
    {
        return ;
    }
    if(src < d->buf || src >= d->buf + this->count())
    {
        this->capacitySet(this->count() + count);
        _ZFP_ZFCoreArray_objCreate(d->contentEnd, d->contentEnd + count, src, d->PODType);
        d->contentEnd += count;
    }
    else
    {
        ZFCoreArray<T_Element> tmp(count);
        tmp.addFrom(src, count);
        this->addFrom(tmp.arrayBuf(), count);
    }
}

template<typename T_Element>
inline void ZFCoreArray<T_Element>::addFrom(ZF_IN const ZFCoreArray<T_Element> &ref)
{
    this->addFrom(ref.arrayBuf(), ref.count());
}

template<typename T_Element>
zfindex ZFCoreArray<T_Element>::find(ZF_IN T_Element const &e,
                                     ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer /* = ZFComparerCheckEqualOnly */) const
{
    for(T_Element *p = d->buf; p < d->contentEnd; ++p)
    {
        if(comparer(*p, e) == ZFCompareTheSame)
        {
            return (p - d->buf);
        }
    }
    return zfindexMax;
}
template<typename T_Element>
zfindex ZFCoreArray<T_Element>::findReversely(ZF_IN T_Element const &e,
                                              ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer /* = ZFComparerCheckEqualOnly */) const
{
    if(d->buf)
    {
        for(T_Element *p = d->contentEnd - 1; p >= d->buf; --p)
        {
            if(comparer(*p, e) == ZFCompareTheSame)
            {
                return (p - d->buf);
            }
        }
    }
    return zfindexMax;
}
template<typename T_Element>
template<typename T_Another>
zfindex ZFCoreArray<T_Element>::find(ZF_IN T_Another const &e,
                                     ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer) const
{
    for(T_Element *p = d->buf; p < d->contentEnd; ++p)
    {
        if(comparer(*p, e) == ZFCompareTheSame)
        {
            return (p - d->buf);
        }
    }
    return zfindexMax;
}
template<typename T_Element>
template<typename T_Another>
zfindex ZFCoreArray<T_Element>::findReversely(ZF_IN T_Another const &e,
                                              ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer) const
{
    if(d->buf)
    {
        for(T_Element *p = d->contentEnd - 1; p >= d->buf; --p)
        {
            if(comparer(*p, e) == ZFCompareTheSame)
            {
                return (p - d->buf);
            }
        }
    }
    return zfindexMax;
}

template<typename T_Element>
zfbool ZFCoreArray<T_Element>::removeElement(ZF_IN T_Element const &e,
                                             ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer /* = ZFComparerCheckEqualOnly */)
{
    for(T_Element *p = d->buf; p < d->contentEnd; ++p)
    {
        if(comparer(*p, e) == ZFCompareTheSame)
        {
            this->remove(p - d->buf);
            return zftrue;
        }
    }
    return zffalse;
}
template<typename T_Element>
template<typename T_Another>
zfbool ZFCoreArray<T_Element>::removeElement(ZF_IN T_Another const &e,
                                             ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer)
{
    for(T_Element *p = d->buf; p < d->contentEnd; ++p)
    {
        if(comparer(*p, e) == ZFCompareTheSame)
        {
            this->remove(p - d->buf);
            return zftrue;
        }
    }
    return zffalse;
}
template<typename T_Element>
zfbool ZFCoreArray<T_Element>::removeElementReversely(ZF_IN T_Element const &e,
                                                      ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer /* = ZFComparerCheckEqualOnly */)
{
    if(d->buf)
    {
        for(T_Element *p = d->contentEnd - 1; p >= d->buf; --p)
        {
            if(comparer(*p, e) == ZFCompareTheSame)
            {
                this->remove(p - d->buf);
                return zftrue;
            }
        }
    }
    return zffalse;
}
template<typename T_Element>
template<typename T_Another>
zfbool ZFCoreArray<T_Element>::removeElementReversely(ZF_IN T_Another const &e,
                                                      ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer)
{
    if(d->buf)
    {
        for(T_Element *p = d->contentEnd - 1; p >= d->buf; --p)
        {
            if(comparer(*p, e) == ZFCompareTheSame)
            {
                this->remove(p - d->buf);
                return zftrue;
            }
        }
    }
    return zffalse;
}
template<typename T_Element>
zfindex ZFCoreArray<T_Element>::removeElementAll(ZF_IN T_Element const &e,
                                                 ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer /* = ZFComparerCheckEqualOnly */)
{
    zfindex removedCount = 0;
    for(T_Element *p = d->buf; p < d->contentEnd; ++p)
    {
        if(comparer(*p, e) == ZFCompareTheSame)
        {
            ++removedCount;
            this->remove(p - d->buf);
            --p;
        }
    }
    return removedCount;
}
template<typename T_Element>
template<typename T_Another>
zfindex ZFCoreArray<T_Element>::removeElementAll(ZF_IN T_Another const &e,
                                                 ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer)
{
    zfindex removedCount = 0;
    for(T_Element *p = d->buf; p < d->contentEnd; ++p)
    {
        if(comparer(*p, e) == ZFCompareTheSame)
        {
            ++removedCount;
            this->remove(p - d->buf);
            --p;
        }
    }
    return removedCount;
}

template<typename T_Element>
inline void ZFCoreArray<T_Element>::remove(ZF_IN zfindex index)
{
    if(index >= this->count())
    {
        zfCoreCriticalIndexOutOfRange(index, this->count());
        return ;
    }
    _ZFP_ZFCoreArray_objMove(d->buf + index, d->buf + index + 1, this->count() - index - 1, d->PODType);
    _ZFP_ZFCoreArray_objDestroy(d->contentEnd - 1, d->contentEnd, d->PODType);
    --(d->contentEnd);
    this->capacityTrim();
}
template<typename T_Element>
inline void ZFCoreArray<T_Element>::remove(ZF_IN zfindex index,
                                           ZF_IN zfindex count)
{
    if(index >= this->count())
    {
        zfCoreCriticalIndexOutOfRange(index, this->count());
        return ;
    }
    if(count > this->count() - index)
    {
        count = this->count() - index;
    }
    _ZFP_ZFCoreArray_objMove(d->buf + index, d->buf + index + count, this->count() - (index + count), d->PODType);
    _ZFP_ZFCoreArray_objDestroy(d->contentEnd - count, d->contentEnd, d->PODType);
    d->contentEnd -= count;
    this->capacityTrim();
}
template<typename T_Element>
inline void ZFCoreArray<T_Element>::removeFirst(void)
{
    if(!this->isEmpty())
    {
        this->remove(0);
    }
}
template<typename T_Element>
inline T_Element ZFCoreArray<T_Element>::removeFirstAndGet(void)
{
    zfCoreAssertWithMessage(!this->isEmpty(), zfTextA("removeFirstAndGet an empty array"));
    T_Element t = *(d->buf);
    this->remove(0);
    return t;
}
template<typename T_Element>
inline void ZFCoreArray<T_Element>::removeLast(void)
{
    if(!this->isEmpty())
    {
        this->remove(this->count() - 1);
    }
}
template<typename T_Element>
inline T_Element ZFCoreArray<T_Element>::removeLastAndGet(void)
{
    zfCoreAssertWithMessage(!this->isEmpty(), zfTextA("removeLastAndGet an empty array"));
    T_Element t = *(d->contentEnd - 1);
    this->remove(this->count() - 1);
    return t;
}
template<typename T_Element>
inline void ZFCoreArray<T_Element>::removeAll(void)
{
    d->cleanup();
}

template<typename T_Element>
void ZFCoreArray<T_Element>::move(ZF_IN zfindex fromIndex, ZF_IN zfindex toIndexOrIndexMax)
{
    if(fromIndex >= this->count())
    {
        zfCoreCriticalIndexOutOfRange(fromIndex, this->count());
        return ;
    }
    if(toIndexOrIndexMax == zfindexMax)
    {
        toIndexOrIndexMax = this->count() - 1;
    }
    if(toIndexOrIndexMax >= this->count())
    {
        zfCoreCriticalIndexOutOfRange(toIndexOrIndexMax, this->count());
        return ;
    }
    if(fromIndex == toIndexOrIndexMax)
    {
        return ;
    }
    T_Element t = d->buf[fromIndex];
    if(fromIndex < toIndexOrIndexMax)
    {
        _ZFP_ZFCoreArray_objMove(d->buf + fromIndex, d->buf + fromIndex + 1, toIndexOrIndexMax - fromIndex, d->PODType);
    }
    else
    {
        _ZFP_ZFCoreArray_objMove(d->buf + toIndexOrIndexMax + 1, d->buf + toIndexOrIndexMax, fromIndex - toIndexOrIndexMax, d->PODType);
    }
    d->buf[toIndexOrIndexMax] = t;
}

template<typename T_Element>
inline void ZFCoreArray<T_Element>::set(ZF_IN zfindex index, ZF_IN T_Element const &e)
{
    if(index >= this->count())
    {
        zfCoreCriticalIndexOutOfRange(index, this->count());
    }
    d->buf[index] = e;
}

template<typename T_Element>
inline T_Element &ZFCoreArray<T_Element>::get(ZF_IN zfindex index)
{
    if(index >= this->count())
    {
        zfCoreCriticalIndexOutOfRange(index, this->count());
    }
    return d->buf[index];
}
template<typename T_Element>
inline T_Element const &ZFCoreArray<T_Element>::get(ZF_IN zfindex index) const
{
    if(index >= this->count())
    {
        zfCoreCriticalIndexOutOfRange(index, this->count());
    }
    return d->buf[index];
}
template<typename T_Element>
inline T_Element &ZFCoreArray<T_Element>::operator[] (ZF_IN zfindex index)
{
    if(index >= this->count())
    {
        zfCoreCriticalIndexOutOfRange(index, this->count());
    }
    return d->buf[index];
}
template<typename T_Element>
inline T_Element const &ZFCoreArray<T_Element>::operator[] (ZF_IN zfindex index) const
{
    if(index >= this->count())
    {
        zfCoreCriticalIndexOutOfRange(index, this->count());
    }
    return d->buf[index];
}
template<typename T_Element>
inline T_Element const &ZFCoreArray<T_Element>::getFirst(void) const
{
    if(this->count() == 0)
    {
        zfCoreCriticalIndexOutOfRange(0, this->count());
    }
    return *(d->buf);
}
template<typename T_Element>
inline T_Element const &ZFCoreArray<T_Element>::getLast(void) const
{
    if(this->count() == 0)
    {
        zfCoreCriticalIndexOutOfRange(0, this->count());
    }
    return *(d->contentEnd - 1);
}

template<typename T_Element>
inline T_Element *ZFCoreArray<T_Element>::arrayBuf(void)
{
    return d->buf;
}
template<typename T_Element>
inline const T_Element *ZFCoreArray<T_Element>::arrayBuf(void) const
{
    return d->buf;
}

template<typename T_Element>
inline zfindex ZFCoreArray<T_Element>::count(void) const
{
    return (d->contentEnd - d->buf);
}
template<typename T_Element>
inline zfbool ZFCoreArray<T_Element>::isEmpty(void) const
{
    return (d->contentEnd == d->buf);
}

template<typename T_Element>
inline void ZFCoreArray<T_Element>::sort(ZF_IN typename ZFComparer<T_Element>::Comparer elementComparer,
                                         ZF_IN_OPT zfbool ascending /* = zftrue */,
                                         ZF_IN_OPT zfindex start /* = 0 */,
                                         ZF_IN_OPT zfindex count /* = zfindexMax */)
{
    if(!this->isEmpty() && start + 1 < this->count() && count > 1)
    {
        zfmSort<T_Element>(
            d->buf,
            elementComparer,
            start,
            (count > this->count() - start) ? (this->count() - 1) : (start + count - 1),
            ascending);
    }
}

// ============================================================
// iterator
template<typename T_Element>
void ZFCoreArray<T_Element>::_ZFP_ZFCoreArray_iteratorDeleteCallback(ZF_IN void *data)
{
}
template<typename T_Element>
void *ZFCoreArray<T_Element>::_ZFP_ZFCoreArray_iteratorCopyCallback(ZF_IN void *data)
{
    return data;
}

template<typename T_Element>
inline zfiterator ZFCoreArray<T_Element>::iteratorForIndex(ZF_IN zfindex index) const
{
    return zfiterator(d->buf + index,
        _ZFP_ZFCoreArray_iteratorDeleteCallback,
        _ZFP_ZFCoreArray_iteratorCopyCallback);
}

template<typename T_Element>
inline zfiterator ZFCoreArray<T_Element>::iteratorFind(ZF_IN T_Element const &e,
                                                       ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer /* = ZFComparerCheckEqualOnly */) const
{
    return this->iteratorForIndex(this->find(e, comparer));
}
template<typename T_Element>
inline zfiterator ZFCoreArray<T_Element>::iteratorFindReversely(ZF_IN T_Element const &e,
                                                                ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer /* = ZFComparerCheckEqualOnly */) const
{
    return this->iteratorForIndex(this->findReversely(e, comparer));
}

template<typename T_Element>
inline zfiterator ZFCoreArray<T_Element>::iterator(void) const
{
    return zfiterator(d->buf,
        _ZFP_ZFCoreArray_iteratorDeleteCallback,
        _ZFP_ZFCoreArray_iteratorCopyCallback);
}

template<typename T_Element>
inline zfbool ZFCoreArray<T_Element>::iteratorIsValid(ZF_IN const zfiterator &it) const
{
    T_Element *p = it.data<T_Element *>();
    return (p != zfnull && p >= d->buf && p < d->contentEnd);
}
template<typename T_Element>
inline zfbool ZFCoreArray<T_Element>::iteratorIsEqual(ZF_IN const zfiterator &it0,
                                                      ZF_IN const zfiterator &it1) const
{
    return ((it0.data() == it1.data()) || (!this->iteratorIsValid(it0) && !this->iteratorIsValid(it1)));
}

template<typename T_Element>
inline T_Element &ZFCoreArray<T_Element>::iteratorGet(ZF_IN_OUT zfiterator &it)
{
    T_Element *p = it.data<T_Element *>();
    if(p != zfnull && p >= d->buf && p < d->contentEnd)
    {
        return *p;
    }
    else
    {
        return d->buf[0];
    }
}
template<typename T_Element>
inline T_Element const &ZFCoreArray<T_Element>::iteratorGet(ZF_IN_OUT zfiterator &it) const
{
    T_Element *p = it.data<T_Element *>();
    if(p != zfnull && p >= d->buf && p < d->contentEnd)
    {
        return *p;
    }
    else
    {
        return d->buf[0];
    }
}

template<typename T_Element>
inline T_Element &ZFCoreArray<T_Element>::iteratorNext(ZF_IN_OUT zfiterator &it)
{
    T_Element *p = it.data<T_Element *>();
    if(p != zfnull && p >= d->buf && p < d->contentEnd)
    {
        it.iteratorImplDataChange(p + 1);
        return *p;
    }
    else
    {
        return d->buf[0];
    }
}
template<typename T_Element>
inline T_Element const &ZFCoreArray<T_Element>::iteratorNext(ZF_IN_OUT zfiterator &it) const
{
    T_Element *p = it.data<T_Element *>();
    if(p != zfnull && p >= d->buf && p < d->contentEnd)
    {
        it.iteratorImplDataChange(p + 1);
        return *p;
    }
    else
    {
        return d->buf[0];
    }
}

template<typename T_Element>
inline T_Element &ZFCoreArray<T_Element>::iteratorPrev(ZF_IN_OUT zfiterator &it)
{
    T_Element *p = it.data<T_Element *>();
    if(p != zfnull && p >= d->buf && p < d->contentEnd)
    {
        it.iteratorImplDataChange(p - 1);
        return *p;
    }
    else
    {
        return d->buf[0];
    }
}
template<typename T_Element>
inline T_Element const &ZFCoreArray<T_Element>::iteratorPrev(ZF_IN_OUT zfiterator &it) const
{
    T_Element *p = it.data<T_Element *>();
    if(p != zfnull && p >= d->buf && p < d->contentEnd)
    {
        it.iteratorImplDataChange(p - 1);
        return *p;
    }
    else
    {
        return d->buf[0];
    }
}

template<typename T_Element>
inline void ZFCoreArray<T_Element>::iteratorSet(ZF_IN_OUT zfiterator &it,
                                                ZF_IN T_Element const &e)
{
    T_Element *p = it.data<T_Element *>();
    if(p != zfnull && p >= d->buf && p < d->contentEnd)
    {
        *p = e;
    }
}
template<typename T_Element>
inline void ZFCoreArray<T_Element>::iteratorRemove(ZF_IN_OUT zfiterator &it)
{
    T_Element *p = it.data<T_Element *>();
    if(p != zfnull && p >= d->buf && p < d->contentEnd)
    {
        this->remove(p - d->buf);
        if(p >= d->contentEnd)
        {
            it.iteratorImplDataChange(d->contentEnd - 1);
        }
    }
}

template<typename T_Element>
inline void ZFCoreArray<T_Element>::iteratorAdd(ZF_IN T_Element const &e,
                                                ZF_IN_OPT const zfiterator &it /* = zfiteratorInvalid */)
{
    T_Element *p = it.data<T_Element *>();
    if(p != zfnull && p >= d->buf && p < d->contentEnd)
    {
        zfindex oldIndex = p - d->buf;
        this->add(oldIndex, e);
        it.iteratorImplDataChange(d->buf + oldIndex + 1);
    }
    else
    {
        this->add(e);
    }
}

template<typename T_Element>
void ZFCoreArray<T_Element>::_ZFP_PODTypeSet(void)
{
    d->PODType = zftrue;
}
template<typename T_Element>
void ZFCoreArray<T_Element>::capacityDoChange(ZF_IN zfindex newCapacity)
{
    if(newCapacity != this->capacity())
    {
        if(newCapacity == 0)
        {
            d->cleanup();
        }
        else
        {
            T_Element *oldBuf = d->buf;
            T_Element *oldContentEnd = d->contentEnd;

            T_Element *newBuf = (T_Element *)zfmalloc(newCapacity * sizeof(T_Element));
            _ZFP_ZFCoreArray_objCreate(newBuf, newBuf + (oldContentEnd - oldBuf), oldBuf, d->PODType);

            d->buf = newBuf;
            d->bufEnd = newBuf + newCapacity;
            d->contentEnd = newBuf + (oldContentEnd - oldBuf);

            _ZFP_ZFCoreArray_objDestroy(oldBuf, oldContentEnd, d->PODType);
            zffree(oldBuf);
        }
    }
}

// ============================================================
template<typename T_Element>
ZFCoreArrayPOD<T_Element>::ZFCoreArrayPOD(void)
: ZFCoreArray<T_Element>()
{
    this->_ZFP_PODTypeSet();
}
template<typename T_Element>
ZFCoreArrayPOD<T_Element>::ZFCoreArrayPOD(ZF_IN zfindex capacity)
: ZFCoreArray<T_Element>(capacity)
{
    this->_ZFP_PODTypeSet();
}
template<typename T_Element>
inline ZFCoreArrayPOD<T_Element> &ZFCoreArrayPOD<T_Element>::operator =(ZF_IN const ZFCoreArray<T_Element> &ref)
{
    ZFCoreArray<T_Element>::operator = (ref);
    return *this;
}
template<typename T_Element>
inline ZFCoreArrayPOD<T_Element> &ZFCoreArrayPOD<T_Element>::operator =(ZF_IN const ZFCoreArrayPOD<T_Element> &ref)
{
    ZFCoreArray<T_Element>::operator = (ref);
    return *this;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreArrayDef_impl_h_

