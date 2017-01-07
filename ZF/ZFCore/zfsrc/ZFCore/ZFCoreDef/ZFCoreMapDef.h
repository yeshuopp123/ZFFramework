/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreMapDef.h
 * @brief core map type for private use only
 */

#ifndef _ZFI_ZFCoreMapDef_h_
#define _ZFI_ZFCoreMapDef_h_

#include "ZFCorePointerDef.h"
#include "ZFCoreArrayDef.h"
#include "ZFIteratorDef.h"
#include "ZFTokenDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief key value pair for #ZFCoreMap
 */
zfclassPOD ZF_ENV_EXPORT ZFCoreMapPair
{
public:
    const zfchar *key; /**< @brief key of map */
    ZFCorePointerBase *value; /**< @brief value of map, ensured not null as smart pointer */
};
/**
 * @brief an empty pair
 */
extern ZF_ENV_EXPORT const ZFCoreMapPair ZFCoreMapPairZero;

zfclassFwd _ZFP_ZFCoreMapPrivate;
/**
 * @brief core map type for private use only
 *
 * used to reduce dependency of stl\n
 * use string as key,
 * null key is considered same as empty string\n
 * use #ZFCorePointerBase as value,
 * which use retain logic and can hold many types
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFCoreMap
{
public:
    /**
     * @brief construct an empty map
     */
    ZFCoreMap(void);
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    ZFCoreMap(ZF_IN const ZFCoreMap &ref);
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    zffinal ZFCoreMap &operator =(ZF_IN const ZFCoreMap &ref);
    ~ZFCoreMap(void);

public:
    /**
     * @brief get a short info
     */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #objectInfoT */
    zffinal zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /**
     * @brief return a string describe the content
     */
    zffinal void objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                      ZF_IN_OPT zfindex maxCount = zfindexMax,
                                      ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault) const;
    /** @brief see #objectInfoOfContentT */
    zffinal zfstring objectInfoOfContent(ZF_IN_OPT zfindex maxCount = zfindexMax,
                                         ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault) const
    {
        zfstring ret;
        this->objectInfoOfContentT(ret, maxCount, token);
        return ret;
    }

public:
    /**
     * @brief copy all contents from ref, remove all before copy
     */
    zffinal void copyFrom(ZF_IN const ZFCoreMap &ref);

    /**
     * @brief get current retain count
     */
    zffinal zfindex objectRetainCount(void) const;

public:
    /**
     * @brief return count
     */
    zffinal zfindex count(void) const;

    /**
     * @brief true if empty
     */
    zffinal zfbool isEmpty(void) const;

    /**
     * @brief true if contains the key
     */
    zffinal zfbool isContain(ZF_IN const zfchar *key) const;

    /**
     * @brief add elements from ref
     */
    zffinal void addFrom(ZF_IN const ZFCoreMap &ref);

    /**
     * @brief change value or create if not exist,
     *   value would be retained by this method
     *
     * null key is considered same as empty string\n
     * assert fail if value is null (use an empty smart pointer to store null value)
     */
    zffinal void set(ZF_IN const zfchar *key,
                     ZF_IN const ZFCorePointerBase &value);
    /**
     * @brief get value or null if not exist
     */
    zffinal ZFCorePointerBase *get(ZF_IN const zfchar *key) const;
    /**
     * @brief get value or null if not exist
     */
    template<typename T_Element>
    T_Element get(ZF_IN const zfchar *key) const
    {
        ZFCorePointerBase *t = this->get(key);
        if(t != zfnull)
        {
            return t->pointerValueT<T_Element>();
        }
        return zfnull;
    }

    /**
     * @brief return a copy of all keys
     */
    zffinal void allKey(ZF_OUT ZFCoreArray<const zfchar *> &ret) const;
    /** @brief see #allKey */
    inline ZFCoreArrayPOD<const zfchar *> allKey(void) const
    {
        ZFCoreArrayPOD<const zfchar *> ret;
        this->allKey(ret);
        return ret;
    }

    /**
     * @brief return a copy of all values
     */
    zffinal void allValue(ZF_OUT ZFCoreArray<ZFCorePointerBase *> &ret) const;
    /** @brief see #allValue */
    inline ZFCoreArrayPOD<ZFCorePointerBase *> allValue(void) const
    {
        ZFCoreArrayPOD<ZFCorePointerBase *> ret;
        this->allValue(ret);
        return ret;
    }

    /**
     * @brief return a copy of all keys and values
     */
    zffinal void allPair(ZF_OUT ZFCoreArray<ZFCoreMapPair> &ret) const;
    /** @brief see #allPair */
    inline ZFCoreArrayPOD<ZFCoreMapPair> allPair(void) const
    {
        ZFCoreArrayPOD<ZFCoreMapPair> ret;
        this->allPair(ret);
        return ret;
    }

    /**
     * @brief return a copy of all values
     *
     * template T_Element must be pointer type
     */
    template<typename T_Element>
    void allValue(ZF_OUT ZFCoreArray<T_Element> &ret) const
    {
        ret.capacitySet(ret.count() + this->count());
        for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
        {
            ret.add(this->iteratorNextValue<T_Element>(it));
        }
    }

    /**
     * @brief remove or do nothing if not exist
     */
    zffinal void remove(ZF_IN const zfchar *key);

    /**
     * @brief remove all content
     */
    zffinal void removeAll(void);

    // ============================================================
    // iterator access
public:
    /** @brief see #zfiterator */
    zffinal zfiterator iterator(void) const;

    /** @brief see #zfiterator */
    zffinal zfiterator iteratorForKey(ZF_IN const zfchar *key) const;

    /** @brief see #zfiterator */
    zffinal zfbool iteratorIsValid(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal zfbool iteratorIsEqual(ZF_IN const zfiterator &it0,
                                   ZF_IN const zfiterator &it1) const;

    /** @brief see #zfiterator */
    zffinal ZFCorePointerBase *iteratorGet(ZF_IN const zfiterator &it) const
    {
        return this->iteratorGetValue(it);
    }

    /** @brief see #zfiterator */
    zffinal ZFCorePointerBase *iteratorNext(ZF_IN_OUT zfiterator &it) const
    {
        return this->iteratorNextValue(it);
    }
    /** @brief see #zfiterator */
    zffinal ZFCorePointerBase *iteratorPrev(ZF_IN_OUT zfiterator &it) const
    {
        return this->iteratorPrevValue(it);
    }

    /** @brief see #zfiterator */
    zffinal void iteratorSet(ZF_IN_OUT zfiterator &it,
                             ZF_IN const ZFCorePointerBase &newValue);
    /** @brief see #zfiterator */
    zffinal void iteratorRemove(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    zffinal const zfchar *iteratorGetKey(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal ZFCorePointerBase *iteratorGetValue(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal ZFCoreMapPair iteratorGetPair(ZF_IN const zfiterator &it) const;

    /** @brief see #zfiterator */
    zffinal const zfchar *iteratorNextKey(ZF_IN_OUT zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal ZFCorePointerBase *iteratorNextValue(ZF_IN_OUT zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal ZFCoreMapPair iteratorNextPair(ZF_IN_OUT zfiterator &it) const;

    /** @brief see #zfiterator */
    zffinal const zfchar *iteratorPrevKey(ZF_IN_OUT zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal ZFCorePointerBase *iteratorPrevValue(ZF_IN_OUT zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal ZFCoreMapPair iteratorPrevPair(ZF_IN_OUT zfiterator &it) const;

    /** @brief see #zfiterator */
    zffinal void iteratorAddKeyValue(ZF_IN const zfchar *key,
                                     ZF_IN const ZFCorePointerBase &value,
                                     ZF_IN_OPT const zfiterator &it = zfiteratorInvalid);

    /** @brief see #zfiterator */
    template<typename T_Element>
    T_Element iteratorGetValue(ZF_IN const zfiterator &it) const
    {
        ZFCorePointerBase *t = this->iteratorGetValue(it);
        if(t != zfnull)
        {
            return t->pointerValueT<T_Element>();
        }
        return zfnull;
    }
    /** @brief see #zfiterator */
    template<typename T_Element>
    T_Element iteratorNextValue(ZF_IN_OUT zfiterator &it) const
    {
        ZFCorePointerBase *t = this->iteratorNextValue(it);
        if(t != zfnull)
        {
            return t->pointerValueT<T_Element>();
        }
        return zfnull;
    }
    /** @brief see #zfiterator */
    template<typename T_Element>
    T_Element iteratorPrevValue(ZF_IN_OUT zfiterator &it) const
    {
        ZFCorePointerBase *t = this->iteratorPrevValue(it);
        if(t != zfnull)
        {
            return t->pointerValueT<T_Element>();
        }
        return zfnull;
    }

private:
    _ZFP_ZFCoreMapPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreMapDef_h_

