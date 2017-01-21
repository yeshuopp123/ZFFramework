/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFIterable.h
 * @brief interface shows a ZFObject is iterable
 */

#ifndef _ZFI_ZFIterable_h_
#define _ZFI_ZFIterable_h_

#include "ZFObject.h"
#include "ZFKeyValuePair.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief iterable, see #zfiterator
 */
zfinterface ZF_ENV_EXPORT ZFIterable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFIterable, ZFInterface)

public:
    /**
     * @brief return total count of this iterable
     */
    virtual zfindex count(void) zfpurevirtual;
    /**
     * @brief return a iterator, see #zfiterator
     */
    virtual zfiterator iterator(void) zfpurevirtual;

    /**
     * @brief find element
     */
    virtual zfiterator iteratorFind(ZF_IN ZFObject *value) zfpurevirtual;

    /**
     * @brief true if the iterator is valid, see #zfiterator
     *
     * you may access value by #iteratorGet,
     * or move to next by #iteratorNext
     */
    virtual zfbool iteratorIsValid(ZF_IN const zfiterator &it) zfpurevirtual;

    /**
     * @brief true if two iterator is considered as equal, see #zfiterator
     *
     * two invalid iterator is considered as equal
     */
    virtual zfbool iteratorIsEqual(ZF_IN const zfiterator &it0,
                                   ZF_IN const zfiterator &it1) zfpurevirtual;

    /**
     * @brief get value by iterator, see #zfiterator
     *
     * iterator must be valid
     */
    virtual ZFObject *iteratorGet(ZF_IN const zfiterator &it) zfpurevirtual;

    /**
     * @brief return current value and move to next, see #zfiterator
     *
     * iterator must be valid
     */
    virtual ZFObject *iteratorNext(ZF_IN_OUT zfiterator &it) zfpurevirtual;

    /**
     * @brief return current value and move to prev, see #zfiterator
     *
     * iterator must be valid and this iterable must be able to move prev
     */
    virtual ZFObject *iteratorPrev(ZF_IN_OUT zfiterator &it) zfpurevirtual;

protected:
    /**
     * @brief set value at iterator, see #zfiterator
     */
    virtual void iteratorSet(ZF_IN_OUT zfiterator &it,
                             ZF_IN ZFObject *value) zfpurevirtual;
    /**
     * @brief remove value at iterator, see #zfiterator
     */
    virtual void iteratorRemove(ZF_IN_OUT zfiterator &it) zfpurevirtual;
    /**
     * @brief remove all contents of this iterable
     */
    virtual void removeAll(void) zfpurevirtual;

    /**
     * @brief add value before iterator,
     *   see #zfiterator
     *
     * use an invalid iterator would add value to tail
     */
    virtual void iteratorAdd(ZF_IN ZFObject *value,
                             ZF_IN_OPT const zfiterator &it = zfiteratorInvalid) zfpurevirtual;
};

// ============================================================
/**
 * @brief iterable that can be edited, see #zfiterator
 */
zfinterface ZF_ENV_EXPORT ZFIterableEditable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFIterableEditable, ZFIterable)

public:
    /**
     * @brief see #ZFIterable::iteratorSet
     */
    virtual void iteratorSet(ZF_IN_OUT zfiterator &it,
                             ZF_IN ZFObject *value) zfpurevirtual;
    /**
     * @brief see #ZFIterable::iteratorRemove
     */
    virtual void iteratorRemove(ZF_IN_OUT zfiterator &it) zfpurevirtual;
    /**
     * @brief see #ZFIterable::removeAll
     */
    virtual void removeAll(void) zfpurevirtual;

    /**
     * @brief see #ZFIterable::iteratorAdd
     */
    virtual void iteratorAdd(ZF_IN ZFObject *value,
                             ZF_IN_OPT const zfiterator &it = zfiteratorInvalid) zfpurevirtual;
};

// ============================================================
/**
 * @brief iterable that with key value pair, see #zfiterator
 */
zfinterface ZF_ENV_EXPORT ZFIterableKeyValue : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFIterableKeyValue, ZFIterable)

public:
    /**
     * @brief return iterator with key, see #zfiterator
     */
    virtual zfiterator iteratorForKey(ZF_IN ZFObject *key) zfpurevirtual;

    /**
     * @brief get key with iterator, see #zfiterator
     */
    virtual ZFObject *iteratorGetKey(ZF_IN const zfiterator &it) zfpurevirtual;

    /**
     * @brief get value with iterator, see #zfiterator
     */
    virtual ZFObject *iteratorGetValue(ZF_IN const zfiterator &it) zfpurevirtual;

    /**
     * @brief get key value pair with iterator, see #zfiterator
     */
    virtual ZFKeyValuePair iteratorGetPair(ZF_IN const zfiterator &it) zfpurevirtual;

    /**
     * @brief return current key and move to next, see #zfiterator
     */
    virtual ZFObject *iteratorNextKey(ZF_IN_OUT zfiterator &it) zfpurevirtual;

    /**
     * @brief return current value and move to next, see #zfiterator
     */
    virtual ZFObject *iteratorNextValue(ZF_IN_OUT zfiterator &it) zfpurevirtual;

    /**
     * @brief return current key value pair and move to next, see #zfiterator
     */
    virtual ZFKeyValuePair iteratorNextPair(ZF_IN_OUT zfiterator &it) zfpurevirtual;

    /**
     * @brief return current key and move to prev, see #zfiterator
     */
    virtual ZFObject *iteratorPrevKey(ZF_IN_OUT zfiterator &it) zfpurevirtual;

    /**
     * @brief return current value and move to prev, see #zfiterator
     */
    virtual ZFObject *iteratorPrevValue(ZF_IN_OUT zfiterator &it) zfpurevirtual;

    /**
     * @brief return current key value pair and move to prev, see #zfiterator
     */
    virtual ZFKeyValuePair iteratorPrevPair(ZF_IN_OUT zfiterator &it) zfpurevirtual;

protected:
    /**
     * @brief add key value before iterator, see #zfiterator
     *
     * for most key value container,
     * this is same as set key with value
     */
    virtual void iteratorAddKeyValue(ZF_IN ZFObject *key,
                                     ZF_IN ZFObject *value,
                                     ZF_IN_OPT const zfiterator &it = zfiteratorInvalid) zfpurevirtual;
};

// ============================================================
/**
 * @brief editable version of #ZFIterableKeyValue, see #zfiterator
 */
zfinterface ZF_ENV_EXPORT ZFIterableKeyValueEditable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFIterableKeyValueEditable, ZFIterable, ZFIterableEditable)

public:
    /**
     * @brief see #ZFIterableKeyValue::iteratorAddKeyValue
     */
    virtual void iteratorAddKeyValue(ZF_IN ZFObject *key,
                                     ZF_IN ZFObject *value,
                                     ZF_IN_OPT const zfiterator &it = zfiteratorInvalid) zfpurevirtual;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIterable_h_

#include "ZFIterableUtil.h"

