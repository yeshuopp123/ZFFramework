/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFHashMap.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFHashMap_h_
#define _ZFI_ZFHashMap_h_

#include "ZFKeyValueContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFHashMapPrivate;
/**
 * @brief container of ZFObject, see #ZFKeyValueContainer
 */
zfclass ZF_ENV_EXPORT ZFHashMap: zfextends ZFKeyValueContainer
{
    ZFOBJECT_DECLARE(ZFHashMap, ZFKeyValueContainer)

protected:
    zfoverride
    virtual zfbool serializableOnSerializeToDataWithRef(ZF_IN_OUT ZFSerializableData &serializableData,
                                                        ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                        ZF_OUT_OPT zfstring *outErrorHint = zfnull);

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->removeAll();
        this->addFrom(ZFCastZFObjectUnchecked(zfself *, anotherObj));
    }

public:
    /**
     * @brief init from a existing container
     */
    virtual ZFObject *objectOnInit(ZF_IN ZFKeyValueContainer *another);
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
     * @brief return true if contain pKey
     */
    virtual zfbool isContain(ZF_IN ZFObject *pKey);

    /**
     * @brief get element by key or null if no such key
     */
    virtual ZFObject *get(ZF_IN ZFObject *pKey);
    /**
     * @brief get element pair, return all null if not find,
     *   otherwise, original key and value's pair would be returned
     */
    virtual ZFKeyValuePair getPair(ZF_IN ZFObject *pKey);

    /**
     * @brief get a copy of all key
     */
    virtual void allKey(ZF_OUT ZFCoreArray<ZFObject *> &ret);
    /** @brief see #allKey */
    virtual inline ZFCoreArrayPOD<ZFObject *> allKey(void)
    {
        ZFCoreArrayPOD<ZFObject *> ret;
        this->allKey(ret);
        return ret;
    }

    /**
     * @brief get a copy of all value
     */
    virtual void allValue(ZF_OUT ZFCoreArray<ZFObject *> &ret);
    /** @brief see #allValue */
    virtual inline ZFCoreArrayPOD<ZFObject *> allValue(void)
    {
        ZFCoreArrayPOD<ZFObject *> ret;
        this->allValue(ret);
        return ret;
    }

    /**
     * @brief get a copy of all key value pair
     */
    virtual void allPair(ZF_OUT ZFCoreArray<ZFKeyValuePair> &ret);
    /** @brief see #allPair */
    virtual inline ZFCoreArrayPOD<ZFKeyValuePair> allPair(void)
    {
        ZFCoreArrayPOD<ZFKeyValuePair> ret;
        this->allPair(ret);
        return ret;
    }

public:
    /**
     * @brief util method to get and cast to desired type,
     *   no type safe check and type must be valid
     */
    template<typename T_ZFObject>
    T_ZFObject get(ZF_IN ZFObject *pKey)
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->get(pKey));
    }

    /**
     * @brief util method to get and cast to desired type,
     *   no type safe check and type must be valid
     */
    template<typename T_ZFObject>
    void allKey(ZF_OUT ZFCoreArray<T_ZFObject> &ret)
    {
        ret.capacitySet(ret.count() + this->count());
        for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
        {
            ret.add(this->iteratorNextKey(it)->to<T_ZFObject>());
        }
    }
    /**
     * @brief util method to get and cast to desired type,
     *   no type safe check and type must be valid
     */
    template<typename T_ZFObject>
    ZFCoreArrayPOD<T_ZFObject> allKey(void)
    {
        ZFCoreArrayPOD<T_ZFObject> ret;
        this->allKey(ret);
        return ret;
    }

    /**
     * @brief util method to get and cast to desired type,
     *   no type safe check and type must be valid
     */
    template<typename T_ZFObject>
    void allValue(ZF_OUT ZFCoreArray<T_ZFObject> &ret)
    {
        ret.capacitySet(ret.count() + this->count());
        for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
        {
            ret.add(this->iteratorNextValue(it)->to<T_ZFObject>());
        }
    }
    /**
     * @brief util method to get and cast to desired type,
     *   no type safe check and type must be valid
     */
    template<typename T_ZFObject>
    ZFCoreArrayPOD<T_ZFObject> allValue(void)
    {
        ZFCoreArrayPOD<T_ZFObject> ret;
        this->allValue(ret);
        return ret;
    }

protected:
    /**
     * @brief add data from another container
     */
    virtual void addFrom(ZF_IN ZFKeyValueContainer *another);

    /**
     * @brief set a key value pair
     *
     * create new one if not exist,
     * replace old one if already exist\n
     * null key is not allowed,
     * use null value to remove the pair
     */
    virtual void set(ZF_IN ZFObject *pKey,
                     ZF_IN ZFObject *pValue);

    /**
     * @brief remove value associated with pKey
     */
    virtual void remove(ZF_IN ZFObject *pKey);
    /**
     * @brief remove and return removed value or null if not exist
     *
     * use this method for performance, instead of "get then remove",
     * since the latter one have two search step
     */
    virtual zfautoObject removeAndGet(ZF_IN ZFObject *pKey);
    /**
     * @brief remove and get pair
     */
    virtual ZFKeyValuePairAutoRelease removeAndGetPair(ZF_IN ZFObject *pKey);
    /**
     * @brief remove all content
     */
    virtual void removeAll(void);

protected:
    /**
     * @brief util method to get and cast to desired type
     */
    template<typename T_ZFObject>
    T_ZFObject removeAndGet(ZF_IN ZFObject *pKey)
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->removeAndGet(pKey));
    }

    // ============================================================
    // ZFIterable
public:
    /** @brief see #zfiterator */
    virtual zfiterator iterator(void);

    /** @brief see #zfiterator */
    virtual zfiterator iteratorFind(ZF_IN ZFObject *value)
    {
        return this->iteratorForKey(value);
    }

    /** @brief see #zfiterator */
    virtual zfbool iteratorIsValid(ZF_IN const zfiterator &it);
    /** @brief see #zfiterator */
    virtual zfbool iteratorIsEqual(ZF_IN const zfiterator &it0,
                                   ZF_IN const zfiterator &it1);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorGet(ZF_IN const zfiterator &it)
    {
        return this->iteratorGetValue(it);
    }

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorNext(ZF_IN_OUT zfiterator &it)
    {
        return this->iteratorNextValue(it);
    }

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorPrev(ZF_IN_OUT zfiterator &it)
    {
        return this->iteratorPrevValue(it);
    }

protected:
    /** @brief see #zfiterator */
    virtual void iteratorSet(ZF_IN_OUT zfiterator &it,
                             ZF_IN ZFObject *value);
    /** @brief see #zfiterator */
    virtual void iteratorRemove(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    virtual void iteratorAdd(ZF_IN ZFObject *value,
                             ZF_IN_OPT const zfiterator &it = zfiteratorInvalid)
    {
        zfCoreCriticalNotSupported();
    }

    // ============================================================
    // ZFIterableKeyValue
public:
    /** @brief see #zfiterator */
    virtual zfiterator iteratorForKey(ZF_IN ZFObject *key);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorGetKey(ZF_IN const zfiterator &it);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorGetValue(ZF_IN const zfiterator &it);

    /** @brief see #zfiterator */
    virtual ZFKeyValuePair iteratorGetPair(ZF_IN const zfiterator &it);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorNextKey(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorNextValue(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    virtual ZFKeyValuePair iteratorNextPair(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorPrevKey(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    virtual ZFObject *iteratorPrevValue(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    virtual ZFKeyValuePair iteratorPrevPair(ZF_IN_OUT zfiterator &it);

protected:
    /** @brief see #zfiterator */
    virtual void iteratorAddKeyValue(ZF_IN ZFObject *key,
                                     ZF_IN ZFObject *value,
                                     ZF_IN_OPT const zfiterator &it = zfiteratorInvalid);

private:
    _ZFP_ZFHashMapPrivate *d;
};

// ============================================================
/**
 * @brief editable container
 */
zfclass ZF_ENV_EXPORT ZFHashMapEditable : zfextends ZFHashMap, zfimplements ZFIterableEditable, zfimplements ZFIterableKeyValueEditable
{
    ZFOBJECT_DECLARE(ZFHashMapEditable, ZFHashMap)
    ZFIMPLEMENTS_DECLARE(ZFIterableEditable, ZFIterableKeyValueEditable)

public:
    zfoverride
    virtual void addFrom(ZF_IN ZFKeyValueContainer *another)
    {
        zfsuper::addFrom(another);
    }

    zfoverride
    virtual void set(ZF_IN ZFObject *pKey,
                     ZF_IN ZFObject *pValue)
    {
        zfsuper::set(pKey, pValue);
    }

    zfoverride
    virtual void remove(ZF_IN ZFObject *pKey)
    {
        zfsuper::remove(pKey);
    }
    zfoverride
    virtual zfautoObject removeAndGet(ZF_IN ZFObject *pKey)
    {
        return zfsuper::removeAndGet(pKey);
    }
    zfoverride
    virtual ZFKeyValuePairAutoRelease removeAndGetPair(ZF_IN ZFObject *pKey)
    {
        return zfsuper::removeAndGetPair(pKey);
    }
    zfoverride
    virtual void removeAll(void)
    {
        zfsuper::removeAll();
    }

public:
    /**
     * @brief util method to get and cast to desired type
     */
    template<typename T_ZFObject>
    T_ZFObject removeAndGet(ZF_IN ZFObject *pKey)
    {
        return zfsuper::removeAndGet<T_ZFObject>(pKey);
    }

    // ============================================================
    // ZFIterable
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

    // ============================================================
    // ZFIterableKeyValue
public:
    /** @brief see #zfiterator */
    virtual void iteratorAddKeyValue(ZF_IN ZFObject *key,
                                     ZF_IN ZFObject *value,
                                     ZF_IN_OPT const zfiterator &it = zfiteratorInvalid)
    {
        zfsuper::iteratorAddKeyValue(key, value, it);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFHashMap_h_

