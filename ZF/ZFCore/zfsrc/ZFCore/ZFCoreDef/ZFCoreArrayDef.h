/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreArrayDef.h
 * @brief light weight array
 */

#ifndef _ZFI_ZFCoreArrayDef_h_
#define _ZFI_ZFCoreArrayDef_h_

#include "ZFCoreLogDef.h"
#include "ZFIteratorDef.h"
#include "ZFCoreUtilComparerDef.h"
#include "ZFCoreUtilMathDef.h"
#include "ZFTokenDef.h"
#include <stdlib.h>

ZF_NAMESPACE_GLOBAL_BEGIN

template<typename T_Element>
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCoreArrayPrivate;

// ============================================================
/**
 * @brief light weight array
 *
 * @warning element must support:
 *   constructor with no argument,
 *   copy constructor to create from existing value,
 *   operator = to copy value
 *
 * REMARKS:
 * -  ZFCoreArray is designed to reduce dependency of STL or other third-party containers,
 *   aimed for light weight
 * -  use other containers such as ZFArray in your app,
 *   if performance is really a matter,
 *   choose any other containers you like,
 *   but try not to abuse them,
 *   since it really bricks the portability of your code
 */
template<typename T_Element>
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFCoreArray
{
public:
    /**
     * @brief main constructor
     */
    ZFCoreArray(void);
    /**
     * @brief main constructor
     */
    ZFCoreArray(ZF_IN zfindex capacity);
    /**
     * @brief construct from another array
     */
    ZFCoreArray(ZF_IN const ZFCoreArray<T_Element> &ref);
    virtual ~ZFCoreArray(void);
    /**
     * @brief retain the array, if you want to copy, use #copyFrom instead
     */
    virtual ZFCoreArray<T_Element> &operator =(ZF_IN const ZFCoreArray<T_Element> &ref);
    /**
     * @brief compare by pointer
     */
    virtual zfbool operator ==(ZF_IN const ZFCoreArray<T_Element> &ref) const;
    /**
     * @brief compare by pointer
     */
    virtual zfbool operator !=(ZF_IN const ZFCoreArray<T_Element> &ref) const;
    /**
     * @brief copy all settings and contents from another array
     */
    void copyFrom(ZF_IN const ZFCoreArray<T_Element> &ref);
    /**
     * @brief get retain count
     */
    zfindex objectRetainCount(void) const;

public:
    /**
     * @brief get a short info about this object
     */
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #objectInfoT */
    virtual inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /**
     * @brief return a string describe the content
     */
    virtual void objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                      ZF_IN typename ZFCoreInfoGetter<T_Element>::InfoGetter elementInfoGetter,
                                      ZF_IN_OPT zfindex maxCount = zfindexMax,
                                      ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault) const;
    /** @brief see #objectInfoOfContentT */
    virtual zfstring objectInfoOfContent(ZF_IN typename ZFCoreInfoGetter<T_Element>::InfoGetter elementInfoGetter,
                                         ZF_IN_OPT zfindex maxCount = zfindexMax,
                                         ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault) const
    {
        zfstring ret;
        this->objectInfoOfContentT(ret, elementInfoGetter, maxCount, token);
        return ret;
    }

public:
    /**
     * @brief custom info getter for #objectInfoOfContent, null to disable
     */
    void customInfoGetterSet(ZF_IN typename ZFCoreInfoGetter<T_Element>::InfoGetter customInfoGetter);
    /**
     * @brief see #customInfoGetterSet
     */
    typename ZFCoreInfoGetter<T_Element>::InfoGetter customInfoGetter(void) const;

public:
    /**
     * @brief whether the array contains POD type
     */
    zfbool isPODType(void) const;
    /**
     * @brief change capacity to hold at least newCapacity
     *
     * do nothing if newCapacity not changed or less than current capacity
     */
    void capacitySet(ZF_IN zfindex newCapacity);
    /**
     * @brief trim current capacity
     *
     * do nothing if not necessary to trim\n
     * this method would be called automatically when remove elements
     */
    void capacityTrim(void);
    /**
     * @brief get capacity
     */
    zfindex capacity(void) const;

public:
    /**
     * @brief add element
     */
    void add(ZF_IN T_Element const &e);
    /**
     * @brief add element at index
     */
    void add(ZF_IN zfindex index,
             ZF_IN T_Element const &e);
    /**
     * @brief add elements, src can be part of this array's buffer
     */
    void addFrom(ZF_IN const T_Element *src,
                 ZF_IN zfindex count);

    /**
     * @brief add from another array
     */
    void addFrom(ZF_IN const ZFCoreArray<T_Element> &ref);

    /**
     * @brief find element
     */
    zfindex find(ZF_IN T_Element const &e,
                 ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqualOnly) const;
    /**
     * @brief find element reversely
     */
    zfindex findReversely(ZF_IN T_Element const &e,
                          ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqualOnly) const;
    /**
     * @brief find element
     */
    template<typename T_Another>
    zfindex find(ZF_IN T_Another const &e,
                 ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer) const;
    /**
     * @brief find element reversely
     */
    template<typename T_Another>
    zfindex findReversely(ZF_IN T_Another const &e,
                          ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer) const;

    /**
     * @brief remove first matched element, return whether the element removed
     */
    zfbool removeElement(ZF_IN T_Element const &e,
                         ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqualOnly);
    /**
     * @brief remove first matched element, return whether the element removed
     */
    template<typename T_Another>
    zfbool removeElement(ZF_IN T_Another const &e,
                         ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer);
    /**
     * @brief remove last matched element, return whether the element removed
     */
    zfbool removeElementReversely(ZF_IN T_Element const &e,
                                  ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqualOnly);
    /**
     * @brief remove last matched element, return whether the element removed
     */
    template<typename T_Another>
    zfbool removeElementReversely(ZF_IN T_Another const &e,
                                  ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer);
    /**
     * @brief remove all matched element, return number of removed element
     */
    zfindex removeElementAll(ZF_IN T_Element const &e,
                             ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqualOnly);
    /**
     * @brief remove all matched element, return number of removed element
     */
    template<typename T_Another>
    zfindex removeElementAll(ZF_IN T_Another const &e,
                             ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer);

    /**
     * @brief remove element at index with count, assert fail if out of range
     */
    void remove(ZF_IN zfindex index);
    /**
     * @brief remove element at index with count, assert fail if out of range
     */
    void remove(ZF_IN zfindex index,
                ZF_IN zfindex count);
    /**
     * @brief remove first or do nothing if empty
     */
    void removeFirst(void);
    /**
     * @brief remove first and return the removed value,
     *   or assert fail if empty
     */
    T_Element removeFirstAndGet(void);
    /**
     * @brief remove last or do nothing if empty
     */
    void removeLast(void);
    /**
     * @brief remove last and return the removed value,
     *   or assert fail if empty
     */
    T_Element removeLastAndGet(void);
    /**
     * @brief remove all content
     */
    void removeAll(void);

    /**
     * @brief move element
     */
    void move(ZF_IN zfindex fromIndex, ZF_IN zfindex toIndexOrIndexMax);

public:
    /**
     * @brief set element at index, or assert fail if index out of range
     */
    void set(ZF_IN zfindex index, ZF_IN T_Element const &e);

public:
    /**
     * @brief get element's reference at index
     */
    T_Element &get(ZF_IN zfindex index);
    /**
     * @brief get element's const reference at index
     */
    T_Element const &get(ZF_IN zfindex index) const;
    /**
     * @brief get element's reference at index
     */
    T_Element &operator[] (ZF_IN zfindex index);
    /**
     * @brief get element's const reference at index
     */
    T_Element const &operator[] (ZF_IN zfindex index) const;
    /**
     * @brief try to get first element, assert fail if empty
     */
    T_Element const &getFirst(void) const;
    /**
     * @brief try to get first element, assert fail if empty
     */
    T_Element const &getLast(void) const;

    /**
     * @brief directly access the array
     *
     * @warning the returned pointer may changed after modify the array
     * @warning return null if current capacity is 0
     */
    T_Element *arrayBuf(void);
    /**
     * @brief see #arrayBuf
     */
    const T_Element *arrayBuf(void) const;

    /**
     * @brief element count of this array
     */
    zfindex count(void) const;
    /**
     * @brief true if empty
     */
    zfbool isEmpty(void) const;

public:
    /**
     * @brief sort element
     */
    void sort(ZF_IN typename ZFComparer<T_Element>::Comparer elementComparer,
              ZF_IN_OPT zfbool ascending = zftrue,
              ZF_IN_OPT zfindex start = 0,
              ZF_IN_OPT zfindex count = zfindexMax);

    // ============================================================
    // iterator
private:
    static void _ZFP_ZFCoreArray_iteratorDeleteCallback(ZF_IN void *data);
    static void *_ZFP_ZFCoreArray_iteratorCopyCallback(ZF_IN void *data);

public:
    /** @brief #zfiterator */
    zfiterator iteratorForIndex(ZF_IN zfindex index) const;

    /** @brief #zfiterator */
    zfiterator iteratorFind(ZF_IN T_Element const &e,
                            ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqualOnly) const;
    /** @brief #zfiterator */
    zfiterator iteratorFindReversely(ZF_IN T_Element const &e,
                                     ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqualOnly) const;

public:
    /** @brief #zfiterator */
    zfiterator iterator(void) const;

    /** @brief see #iterator */
    zfbool iteratorIsValid(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zfbool iteratorIsEqual(ZF_IN const zfiterator &it0,
                           ZF_IN const zfiterator &it1) const;

    /** @brief see #iterator */
    T_Element &iteratorGet(ZF_IN_OUT zfiterator &it);
    /** @brief see #iterator */
    T_Element const &iteratorGet(ZF_IN_OUT zfiterator &it) const;

    /** @brief see #iterator */
    T_Element &iteratorNext(ZF_IN_OUT zfiterator &it);
    /** @brief see #iterator */
    T_Element const &iteratorNext(ZF_IN_OUT zfiterator &it) const;

    /** @brief see #iterator */
    T_Element &iteratorPrev(ZF_IN_OUT zfiterator &it);
    /** @brief see #iterator */
    T_Element const &iteratorPrev(ZF_IN_OUT zfiterator &it) const;

    /** @brief see #iterator */
    void iteratorSet(ZF_IN_OUT zfiterator &it,
                     ZF_IN T_Element const &e);
    /** @brief see #iterator */
    void iteratorRemove(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    void iteratorAdd(ZF_IN T_Element const &e,
                     ZF_IN_OPT const zfiterator &it = zfiteratorInvalid);

protected:
    void _ZFP_PODTypeSet(void);
private:
    _ZFP_ZFCoreArrayPrivate<T_Element> *d;
    void capacityDoChange(ZF_IN zfindex newCapacity);
};

// ============================================================
/**
 * @brief POD version of #ZFCoreArray
 *
 * typically has higher performance than non-POD version
 * @warning you should ensure the content type is POD type
 */
template<typename T_Element>
zfclassLikePOD ZF_ENV_EXPORT ZFCoreArrayPOD : zfextendsLikePOD ZFCoreArray<T_Element>
{
public:
    /**
     * @brief see #ZFCoreArray
     */
    ZFCoreArrayPOD(void);
    /**
     * @brief see #ZFCoreArray
     */
    ZFCoreArrayPOD(ZF_IN zfindex capacity);
    /**
     * @brief see #ZFCoreArray
     */
    virtual ZFCoreArrayPOD<T_Element> &operator =(ZF_IN const ZFCoreArray<T_Element> &ref);
    /**
     * @brief see #ZFCoreArray
     */
    virtual ZFCoreArrayPOD<T_Element> &operator =(ZF_IN const ZFCoreArrayPOD<T_Element> &ref);
};

template<typename T_Element>
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFCoreArrayCreate
{
public:
    inline _ZFP_ZFCoreArrayCreate<T_Element> &add(ZF_IN T_Element const &v)
    {
        this->array.add(v);
        return *this;
    }

public:
    ZFCoreArray<T_Element> array;
};
template<typename T_Element>
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFCoreArrayPODCreate
{
public:
    inline _ZFP_ZFCoreArrayPODCreate<T_Element> &add(ZF_IN T_Element const &v)
    {
        this->array.add(v);
        return *this;
    }

public:
    ZFCoreArrayPOD<T_Element> array;
};
#define _ZFP_ZFCoreArrayCreate_action_expand(value, index, total) .add(value)
#define _ZFP_ZFCoreArrayCreate_action(CreatorType, values, ...) \
    CreatorType() ZFM_FIX_PARAM(_ZFP_ZFCoreArrayCreate_action_expand, ZFM_EMPTY, values, ##__VA_ARGS__) .array
/**
 * @brief util macro to create a temp array for short
 *
 * usage:
 * @code
 *   ZFCoreArray array = ZFCoreArrayCreate(v0, v1, ...); // any element count
 * @endcode
 */
#define ZFCoreArrayCreate(ElementType, values, ...) _ZFP_ZFCoreArrayCreate_action(_ZFP_ZFCoreArrayCreate<ElementType>, values, ##__VA_ARGS__)
/**
 * @brief create POD version, see #ZFCoreArrayCreate
 */
#define ZFCoreArrayPODCreate(ElementType, values, ...) _ZFP_ZFCoreArrayCreate_action(_ZFP_ZFCoreArrayPODCreate<ElementType>, values, ##__VA_ARGS__)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreArrayDef_h_

#include "ZFCoreArrayDef_impl.h"

