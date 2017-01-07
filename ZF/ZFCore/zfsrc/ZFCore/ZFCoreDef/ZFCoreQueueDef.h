/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreQueueDef.h
 * @brief core queue type for private use only
 */

#ifndef _ZFI_ZFCoreQueueDef_h_
#define _ZFI_ZFCoreQueueDef_h_

#include "ZFCoreArrayDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFCoreQueuePODBuiltinBufSize 8

/**
 * @brief core queue type for performance and for private use only
 *
 * @warning can hold POD types only
 */
template<typename T_POD>
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFCoreQueuePOD
{
public:
    /**
     * @brief main constructor
     */
    ZFCoreQueuePOD(void)
    : _bufBuiltin()
    , _bufHead(_bufBuiltin)
    , _bufTail(_bufBuiltin + _ZFP_ZFCoreQueuePODBuiltinBufSize)
    , _pHead(_bufHead)
    , _pTail(_pHead)
    , _customInfoGetter(zfnull)
    {
    }
    virtual ~ZFCoreQueuePOD(void)
    {
        if(_bufHead != _bufBuiltin)
        {
            zffree(_bufHead);
        }
    }
private:
    // we don't allow copy
    ZFCoreQueuePOD(ZF_IN const ZFCoreQueuePOD<T_POD> &ref);
    ZFCoreQueuePOD<T_POD> &operator =(ZF_IN const ZFCoreQueuePOD<T_POD> &ref);

public:
    /**
     * @brief get a short info about this object
     */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const
    {
        this->objectInfoOfContentT(ret, zfnull, 10);
    }
    /** @brief see #objectInfoT */
    zffinal inline zfstring objectInfo(void) const
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
                                      ZF_IN typename ZFCoreInfoGetter<T_POD>::InfoGetter elementInfoGetter,
                                      ZF_IN_OPT zfindex maxCount = zfindexMax,
                                      ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault) const
    {
        zfindex count = 0;
        ret += token.tokenLeft;
        for(T_POD *p = _pHead; p != _pTail && count < maxCount; ++count, _loopNext(p))
        {
            if(count > 0)
            {
                ret += token.tokenSeparator;
            }
            ret += token.tokenValueLeft;
            if(_customInfoGetter != zfnull)
            {
                _customInfoGetter(ret, *p);
            }
            else if(elementInfoGetter != zfnull)
            {
                elementInfoGetter(ret, *p);
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
    /** @brief see #objectInfoOfContentT */
    zffinal zfstring objectInfoOfContent(ZF_IN typename ZFCoreInfoGetter<T_POD>::InfoGetter elementInfoGetter,
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
    zffinal void customInfoGetterSet(ZF_IN typename ZFCoreInfoGetter<T_POD>::InfoGetter customInfoGetter)
    {
        _customInfoGetter = customInfoGetter;
    }
    /**
     * @brief see #customInfoGetterSet
     */
    zffinal typename ZFCoreInfoGetter<T_POD>::InfoGetter customInfoGetter(void) const
    {
        return _customInfoGetter;
    }

public:
    /**
     * @brief get current capacity
     *
     * capacity would be increased automatically during adding elements
     */
    inline zfindex capacity(void) const
    {
        return (_bufTail - _bufHead);
    }
    /**
     * @brief trim the buffer, call only if necessary
     */
    zffinal void queueTrim(void)
    {
        zfindex capacityNew = this->count();
        if(capacityNew < _ZFP_ZFCoreQueuePODBuiltinBufSize)
        {
            _bufChange(_bufBuiltin, _bufBuiltin + _ZFP_ZFCoreQueuePODBuiltinBufSize, zffalse);
        }
        else
        {
            capacityNew = ((capacityNew / _ZFP_ZFCoreQueuePODBuiltinBufSize) + 1) * _ZFP_ZFCoreQueuePODBuiltinBufSize;
            T_POD *bufHeadNew = (T_POD *)zfmalloc(sizeof(T_POD) * capacityNew);
            _bufChange(bufHeadNew, bufHeadNew + capacityNew, zftrue);
        }
    }

public:
    /**
     * @brief push element at tail of the queue,
     *   auto increase capacity if necessary
     */
    inline void queuePut(ZF_IN T_POD const &e)
    {
        *_pTail = e;
        _loopNext(_pTail);
        if(_pTail == _pHead)
        {
            zfindex capacityNew = this->capacity() * 2;
            T_POD *bufHeadNew = (T_POD *)zfmalloc(sizeof(T_POD) * capacityNew);
            _bufChange(bufHeadNew, bufHeadNew + capacityNew, zftrue);
        }
    }
    /**
     * @brief take element at head of the queue, assert fail if empty
     */
    inline T_POD queueTake(void)
    {
        zfCoreAssertWithMessage(_pHead != _pTail, zfTextA("take from an empty queue"));
        T_POD *ret = _pHead;
        _loopNext(_pHead);
        return *ret;
    }

public:
    /**
     * @brief element count of this array
     */
    inline zfindex count(void) const
    {
        return ((_pTail >= _pHead)
            ? (_pTail - _pHead)
            : ((_bufTail - _pHead) + (_pTail - _bufHead)));
    }
    /**
     * @brief true if empty
     */
    inline zfbool isEmpty(void) const
    {
        return (_pHead == _pTail);
    }

public:
    /**
     * @brief remove all contents
     */
    void removeAll(void)
    {
        _pHead = _bufHead;
        _pTail = _bufHead;
    }

public:
    /**
     * @brief copy contents to array
     */
    void toArray(ZF_IN_OUT ZFCoreArray<T_POD> &array) const
    {
        array.capacitySet(array.capacity() + this->count());
        T_POD *p = _pHead;
        while(p != _pTail)
        {
            array.add(*p);
            _loopNext(p);
        }
    }
    /**
     * @brief copy contents to array
     */
    ZFCoreArrayPOD<T_POD> toArray(void) const
    {
        ZFCoreArrayPOD<T_POD> ret;
        this->toArray(ret);
        return ret;
    }

    // ============================================================
    // iterator
private:
    static void _ZFP_ZFCoreQueuePOD_iteratorDeleteCallback(ZF_IN void *data)
    {
    }
    static void *_ZFP_ZFCoreQueuePOD_iteratorCopyCallback(ZF_IN void *data)
    {
        return data;
    }

public:
    /** @brief #zfiterator */
    inline zfiterator iterator(void) const
    {
        return zfiterator(_pHead,
            _ZFP_ZFCoreQueuePOD_iteratorDeleteCallback,
            _ZFP_ZFCoreQueuePOD_iteratorCopyCallback);
    }

    /** @brief see #iterator */
    inline zfbool iteratorIsValid(ZF_IN const zfiterator &it) const
    {
        T_POD *data = it.data<T_POD *>();
        return ((_pTail >= _pHead)
            ? (data >= _pHead && data < _pTail)
            : ((data >= _pHead && data < _bufTail) || (data >= _bufHead && data < _pTail)));
    }
    /** @brief see #zfiterator */
    inline zfbool iteratorIsEqual(ZF_IN const zfiterator &it0,
                                  ZF_IN const zfiterator &it1) const
    {
        return zfiterator::iteratorIsEqual<T_POD *>(it0, it1);
    }

    /** @brief see #iterator */
    inline T_POD &iteratorGet(ZF_IN_OUT zfiterator &it)
    {
        if(this->iteratorIsValid(it))
        {
            return *(it.data<T_POD *>());
        }
        else
        {
            return *_pHead;
        }
    }
    /** @brief see #iterator */
    inline T_POD const &iteratorGet(ZF_IN_OUT zfiterator &it) const
    {
        if(this->iteratorIsValid(it))
        {
            return *(it.data<T_POD *>());
        }
        else
        {
            return *_pHead;
        }
    }

    /** @brief see #iterator */
    inline T_POD &iteratorNext(ZF_IN_OUT zfiterator &it)
    {
        if(this->iteratorIsValid(it))
        {
            T_POD *ret = it.data<T_POD *>();
            T_POD *t = ret;
            _loopNext(t);
            it.iteratorImplDataChange(t);
            return *ret;
        }
        else
        {
            return *_pHead;
        }
    }
    /** @brief see #iterator */
    inline T_POD const &iteratorNext(ZF_IN_OUT zfiterator &it) const
    {
        if(this->iteratorIsValid(it))
        {
            T_POD *ret = it.data<T_POD *>();
            T_POD *t = ret;
            _loopNext(t);
            it.iteratorImplDataChange(t);
            return *ret;
        }
        else
        {
            return *_pHead;
        }
    }

    /** @brief see #iterator */
    inline void iteratorSet(ZF_IN_OUT zfiterator &it,
                            ZF_IN T_POD const &e)
    {
        if(this->iteratorIsValid(it))
        {
            *(it.data<T_POD *>()) = e;
        }
    }
    /** @brief see #iterator */
    void iteratorRemove(ZF_IN_OUT zfiterator &it)
    {
        if(this->iteratorIsValid(it))
        {
            T_POD *p = it.data<T_POD *>();
            T_POD *p2 = p;
            _loopNext(p2);
            while(p2 != _pTail)
            {
                *p = *p2;
                p = p2;
                _loopNext(p2);
            }
        }
    }

    /** @brief see #zfiterator */
    inline void iteratorAdd(ZF_IN T_POD const &e,
                            ZF_IN_OPT const zfiterator &it = zfiteratorInvalid)
    {
        this->queuePut(e);
    }

private:
    T_POD _bufBuiltin[_ZFP_ZFCoreQueuePODBuiltinBufSize];
    T_POD *_bufHead;
    T_POD *_bufTail;
    T_POD *_pHead;
    T_POD *_pTail;
    typename ZFCoreInfoGetter<T_POD>::InfoGetter _customInfoGetter;
private:
    void _loopNext(ZF_IN_OUT T_POD *&p) const
    {
        ++p;
        if(p >= _bufTail)
        {
            p = _bufHead;
        }
    }
    void _bufChange(ZF_IN T_POD *bufHeadNew, ZF_IN T_POD *bufTailNew, ZF_IN zfbool bufFull)
    {
        T_POD *pNew = bufHeadNew;
        T_POD *pOld = _pHead;
        if(bufFull)
        {
            *pNew = *pOld;
            _loopNext(pOld);
            ++pNew;
        }
        while(pOld != _pTail)
        {
            *pNew = *pOld;
            _loopNext(pOld);
            ++pNew;
        }
        if(_bufHead != _bufBuiltin)
        {
            zffree(_bufHead);
        }
        _bufHead = bufHeadNew;
        _bufTail = bufTailNew;
        _pHead = _bufHead;
        _pTail = pNew;
    }
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreQueueDef_h_

