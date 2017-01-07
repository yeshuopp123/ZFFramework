/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFIteratorDef.h
 * @brief iterator in ZFFramework
 */

#ifndef _ZFI_ZFIteratorDef_h_
#define _ZFI_ZFIteratorDef_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief iterator for ZFFramework
 *
 * zfiterator is an abstract iterator holder,
 * which simply stores an void * pointer for implementations
 * to achieve iterator logic\n
 * \n
 * typically, you may use iterators like this:
 * @code
 *   for(zfiterator it = someIterable.iterator();
 *       someIterable.iteratorIsValid(it);)
 *   {
 *       // get current value located by iterator,
 *       // then move iterator to next
 *       const SomeType &value = someIterable.iteratorNext(it);
 *
 *       // or you may simply want to access without move iterator
 *       // const SomeType &value = someIterable.iteratorGet(it);
 *   }
 * @endcode
 * for key-value container,
 * access by iterator should return value,
 * and should supply key-value version of iterator access:
 * @code
 *   for(zfiterator it = keyValueIterable.iteratorForKey(key);
 *       keyValueIterable.iteratorIsValid(it);)
 *   {
 *       // access key and value
 *       KeyType key = keyValueIterable.iteratorGetKey(it);
 *       ValueType value = keyValueIterable.iteratorGetValue(it);
 *
 *       // access by iterator should return value type
 *       ValueType value2 = keyValueIterable.iteratorGet(it);
 *
 *       // move to next
 *       keyValueIterable.iteratorNext(it);
 *   }
 * @endcode
 * \n
 * for implementations,
 * you should construct a zfiterator with a void *pointer,
 * a delete callback and a copy callback,
 * then you may use #data to access the stored void *pointer
 * to achieve your implementation
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT zfiterator
{
public:
    /**
     * @brief delete callback for implementation
     */
    typedef void (*DeleteCallback)(ZF_IN void *data);
    /**
     * @brief copy callback for implementation
     */
    typedef void *(*CopyCallback)(ZF_IN void *data);

public:
    /**
     * @brief helper method to compare two iterator, for implementation only
     */
    template<typename T_Data>
    static zfbool iteratorIsEqual(ZF_IN zfiterator const &it0, ZF_IN zfiterator const &it1)
    {
        T_Data d0 = it0.data<T_Data>();
        T_Data d1 = it1.data<T_Data>();
        return ((d0 == zfnull && d1 == zfnull) || (d0 != zfnull && d1 != zfnull && *d0 == *d1));
    }

public:
    /**
     * @brief create a dummy iterator
     */
    zfiterator(void)
    {
        zfmemset(&d, 0, sizeof(zfiterator::_Data));
    }
    /**
     * @brief implementations of iterables must use this to create an iterator,
     *   see #zfiterator
     */
    zfiterator(ZF_IN void *data,
               ZF_IN zfiterator::DeleteCallback deleteCallback,
               ZF_IN zfiterator::CopyCallback copyCallback)
    {
        d.data = data;
        d.deleteCallback = deleteCallback;
        d.copyCallback = copyCallback;
    }
    /**
     * @brief implementations may use this method to access data passed from constructor,
     *   see #zfiterator
     */
    inline void *data(void) const
    {
        return d.data;
    }
    /**
     * @brief see #data
     */
    template<typename T_Data>
    inline T_Data data(void) const
    {
        return (T_Data)(d.data);
    }
    /**
     * @brief for implementation to change the internal data
     */
    inline void iteratorImplDataChange(ZF_IN void *newData)
    {
        if(d.data)
        {
            d.deleteCallback(d.data);
        }
        d.data = newData;
    }

public:
    /** @cond ZFPrivateDoc */
    zfiterator(ZF_IN const zfiterator &ref)
    {
        if(ref.d.data)
        {
            d.data = ref.d.copyCallback(ref.d.data);
            d.deleteCallback = ref.d.deleteCallback;
            d.copyCallback = ref.d.copyCallback;
        }
        else
        {
            zfmemset(&d, 0, sizeof(zfiterator::_Data));
        }
    }
    ~zfiterator(void)
    {
        if(d.data)
        {
            d.deleteCallback(d.data);
        }
    }
    zfiterator &operator = (ZF_IN const zfiterator &ref)
    {
        if(this != &ref)
        {
            if(d.data)
            {
                d.deleteCallback(d.data);
            }
            if(ref.d.data)
            {
                d.data = ref.d.copyCallback(ref.d.data);
                d.deleteCallback = ref.d.deleteCallback;
                d.copyCallback = ref.d.copyCallback;
            }
            else
            {
                zfmemset(&d, 0, sizeof(zfiterator::_Data));
            }
        }
        return *this;
    }
    /** @endcond */
private:
    zfbool operator == (ZF_IN const zfiterator &ref);
    zfbool operator != (ZF_IN const zfiterator &ref);
private:
    zfclassPOD _Data
    {
    public:
        void *data;
        zfiterator::DeleteCallback deleteCallback;
        zfiterator::CopyCallback copyCallback;
    };
    _Data d;
};

/**
 * @brief invalid iterator
 * @note compare another iterator with this value is not safe,
 *   always use the check method that container supplys is recommended,
 *   such as aArray.iteratorIsValid(it)
 */
extern ZF_ENV_EXPORT const zfiterator zfiteratorInvalid;

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFIteratorDef_h_

