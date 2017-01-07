/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFArray.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFArray_h_
#define _ZFI_ZFArray_h_

#include "ZFContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFArrayPrivate;
/**
 * @brief container of ZFObject, see #ZFContainer
 */
zfclass ZF_ENV_EXPORT ZFArray : zfextends ZFContainer
{
    ZFOBJECT_DECLARE(ZFArray, ZFContainer)

protected:
    /**
     * @brief see #ZFContainer::serializableOnSerializeToDataWithRef
     *
     * for array container,
     * contents must be all the same with the referenced object with same order,
     * otherwise we would treat as failed for safe
     */
    zfoverride
    virtual zfbool serializableOnSerializeToDataWithRef(ZF_IN_OUT ZFSerializableData &serializableData,
                                                        ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                        ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->removeAll();
        this->addFrom(ZFCastZFObjectUnchecked(zfself *, anotherObj));
    }

public:
    /** @brief see #ZFObject::objectOnInit */
    virtual ZFObject *objectOnInit(ZF_IN ZFContainer *another);
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    /**
     * @brief return number of content
     */
    virtual zfindex count(void);

    /**
     * @brief return true if empty or false if not empty
     */
    virtual zfbool isEmpty(void);

    /**
     * @brief return object at index, assert failure if out of range
     */
    virtual ZFObject *get(ZF_IN zfindex index);

    /**
     * @brief return first object or zfnull if empty
     */
    virtual ZFObject *getFirst(void);

    /**
     * @brief return last object or zfnull if empty
     */
    virtual ZFObject *getLast(void);

    /**
     * @brief return true if contains the object,
     *   compared by #ZFObject::objectCompare by default
     */
    virtual zfbool isContain(ZF_IN ZFObject *obj,
                             ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerCheckEqualOnly);

    /**
     * @brief find element, compared by #ZFObject::objectCompare by default
     */
    virtual zfindex find(ZF_IN ZFObject *obj,
                         ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerCheckEqualOnly);
    /**
     * @brief find element, compared by #ZFObject::objectCompare by default
     */
    virtual zfindex findReversely(ZF_IN ZFObject *obj,
                                  ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerCheckEqualOnly);

public:
    /**
     * @brief util getter to get and cast to desired type
     */
    template<typename T_ZFObject>
    T_ZFObject get(ZF_IN zfindex index)
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->get(index));
    }
    /**
     * @brief util getter to get and cast to desired type
     */
    template<typename T_ZFObject>
    T_ZFObject getFirst(void)
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->getFirst());
    }
    /**
     * @brief util getter to get and cast to desired type
     */
    template<typename T_ZFObject>
    T_ZFObject getLast(void)
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->getLast());
    }

protected:
    /**
     * @brief insert object at index, assert failure if obj = zfnull
     *   or index out of range [0, count]
     * @note null object is not allowed
     */
    virtual void add(ZF_IN zfindex indexAddTo,
                     ZF_IN ZFObject *obj);
    /**
     * @brief insert object to last, assert failure if obj = zfnull, see #add
     */
    virtual void add(ZF_IN ZFObject *obj);
    /**
     * @brief add objects from another container
     */
    virtual void addFrom(ZF_IN ZFContainer *another);

    /**
     * @brief replace object at index, assert fail if index out of range
     */
    virtual void set(ZF_IN zfindex index,
                     ZF_IN ZFObject *obj);

    /**
     * @brief remove first matched object, return whether the element removed
     */
    virtual zfbool removeElement(ZF_IN ZFObject *obj);
    /**
     * @brief remove first matched object, return whether the element removed
     */
    virtual zfbool removeElement(ZF_IN ZFObject *obj, ZF_IN ZFComparer<ZFObject *>::Comparer comparer);
    /**
     * @brief remove last matched object, return whether the element removed
     */
    virtual zfbool removeElementRevsersely(ZF_IN ZFObject *obj);
    /**
     * @brief remove last matched object, return whether the element removed
     */
    virtual zfbool removeElementRevsersely(ZF_IN ZFObject *obj, ZF_IN ZFComparer<ZFObject *>::Comparer comparer);
    /**
     * @brief remove all matched object, return number of element removed
     */
    virtual zfindex removeElementAll(ZF_IN ZFObject *obj);
    /**
     * @brief remove all matched object, return number of element removed
     */
    virtual zfindex removeElementAll(ZF_IN ZFObject *obj, ZF_IN ZFComparer<ZFObject *>::Comparer comparer);

    /**
     * @brief remove object at index, assert failure if out of range
     */
    virtual void remove(ZF_IN zfindex index,
                        ZF_IN_OPT zfindex count = 1);
    /**
     * @brief remove first object or do nothing if out of range
     */
    virtual void removeFirst(void);
    /**
     * @brief remove last object or do nothing if out of range
     */
    virtual void removeLast(void);
    /**
     * @brief remove all object
     */
    virtual void removeAll(void);

    /**
     * @brief move element
     */
    virtual void move(ZF_IN zfindex fromIndex, ZF_IN zfindex toIndexOrIndexMax);

    /**
     * @brief sort content in range [start, start + count), all of content must be comparable
     */
    virtual void sort(ZF_IN_OPT zfbool ascending = zftrue,
                      ZF_IN_OPT zfindex start = 0,
                      ZF_IN_OPT zfindex count = zfindexMax,
                      ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerCheckEqualOnly);

    // ============================================================
    // extra iterable
public:
    /** @brief see #zfiterator */
    virtual zfiterator iteratorForIndex(ZF_IN zfindex index);
    /** @brief see #zfiterator */
    virtual zfiterator iteratorFind(ZF_IN ZFObject *value,
                                    ZF_IN ZFComparer<ZFObject *>::Comparer comparer);
    /** @brief see #zfiterator */
    virtual zfiterator iteratorFindReversely(ZF_IN ZFObject *value,
                                             ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerCheckEqualOnly);

    // ============================================================
    // ZFIterable
public:
    /** @brief see #zfiterator */
    virtual zfiterator iterator(void);

    /** @brief see #zfiterator */
    virtual zfiterator iteratorFind(ZF_IN ZFObject *value);

    /** @brief see #zfiterator */
    virtual zfbool iteratorIsValid(ZF_IN const zfiterator &it);
    /** @brief see #zfiterator */
    virtual zfbool iteratorIsEqual(ZF_IN const zfiterator &it0,
                                   ZF_IN const zfiterator &it1);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorGet(ZF_IN const zfiterator &it);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorNext(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorPrev(ZF_IN_OUT zfiterator &it);

protected:
    /** @brief see #zfiterator */
    virtual void iteratorSet(ZF_IN_OUT zfiterator &it,
                             ZF_IN ZFObject *value);
    /** @brief see #zfiterator */
    virtual void iteratorRemove(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    virtual void iteratorAdd(ZF_IN ZFObject *value,
                             ZF_IN_OPT const zfiterator &it = zfiteratorInvalid);

private:
    _ZFP_ZFArrayPrivate *d;
};

// ============================================================
/**
 * @brief editable container of ZFObject
 */
zfclass ZF_ENV_EXPORT ZFArrayEditable : zfextends ZFArray, zfimplements ZFIterableEditable
{
    ZFOBJECT_DECLARE(ZFArrayEditable, ZFArray)
    ZFIMPLEMENTS_DECLARE(ZFIterableEditable)

public:
    zfoverride
    virtual void add(ZF_IN zfindex indexAddTo,
                     ZF_IN ZFObject *obj)
    {
        zfsuper::add(indexAddTo, obj);
    }
    zfoverride
    virtual void add(ZF_IN ZFObject *obj)
    {
        zfsuper::add(obj);
    }
    zfoverride
    virtual void addFrom(ZF_IN ZFContainer *another)
    {
        zfsuper::addFrom(another);
    }

    zfoverride
    virtual void set(ZF_IN zfindex index,
                     ZF_IN ZFObject *obj)
    {
        zfsuper::set(index, obj);
    }

    zfoverride
    virtual zfbool removeElement(ZF_IN ZFObject *obj)
    {
        return zfsuper::removeElement(obj);
    }
    zfoverride
    virtual zfbool removeElement(ZF_IN ZFObject *obj, ZF_IN ZFComparer<ZFObject *>::Comparer comparer)
    {
        return zfsuper::removeElement(obj, comparer);
    }
    zfoverride
    virtual zfbool removeElementRevsersely(ZF_IN ZFObject *obj)
    {
        return zfsuper::removeElementRevsersely(obj);
    }
    zfoverride
    virtual zfbool removeElementRevsersely(ZF_IN ZFObject *obj, ZF_IN ZFComparer<ZFObject *>::Comparer comparer)
    {
        return zfsuper::removeElementRevsersely(obj, comparer);
    }
    zfoverride
    virtual zfindex removeElementAll(ZF_IN ZFObject *obj)
    {
        return zfsuper::removeElementAll(obj);
    }
    zfoverride
    virtual zfindex removeElementAll(ZF_IN ZFObject *obj, ZF_IN ZFComparer<ZFObject *>::Comparer comparer)
    {
        return zfsuper::removeElementAll(obj, comparer);
    }

    zfoverride
    virtual void remove(ZF_IN zfindex index,
                        ZF_IN_OPT zfindex count = 1)
    {
        zfsuper::remove(index, count);
    }
    zfoverride
    virtual void removeFirst(void)
    {
        zfsuper::removeFirst();
    }
    zfoverride
    virtual void removeLast(void)
    {
        zfsuper::removeLast();
    }
    zfoverride
    virtual void removeAll(void)
    {
        zfsuper::removeAll();
    }

    zfoverride
    virtual void move(ZF_IN zfindex fromIndex, ZF_IN zfindex toIndexOrIndexMax)
    {
        zfsuper::move(fromIndex, toIndexOrIndexMax);
    }

    zfoverride
    virtual void sort(ZF_IN_OPT zfbool ascending = zftrue,
                      ZF_IN_OPT zfindex start = 0,
                      ZF_IN_OPT zfindex count = zfindexMax,
                      ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerCheckEqualOnly)
    {
        zfsuper::sort(ascending, start, count, comparer);
    }

public:
    /** @brief see #zfiterator */
    virtual void iteratorSet(ZF_IN_OUT zfiterator &it,
                             ZF_IN ZFObject *value)
    {
        zfsuper::iteratorSet(it, value);
    }
    /** @brief see #zfiterator */
    virtual void iteratorRemove(ZF_IN_OUT zfiterator &it)
    {
        zfsuper::iteratorRemove(it);
    }

    /** @brief see #zfiterator */
    virtual void iteratorAdd(ZF_IN ZFObject *value,
                             ZF_IN_OPT const zfiterator &it = zfiteratorInvalid)
    {
        zfsuper::iteratorAdd(value, it);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFArray_h_

