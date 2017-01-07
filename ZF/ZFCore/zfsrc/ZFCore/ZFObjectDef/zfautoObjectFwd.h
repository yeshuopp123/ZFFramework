/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file zfautoObjectFwd.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_zfautoObjectFwd_h_
#define _ZFI_zfautoObjectFwd_h_

#include "ZFAnyDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfautoObject
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfautoObjectPrivateBase
{
public:
    zfindex _ZFP_refCount;
    ZFObject *_ZFP_obj;
public:
    _ZFP_zfautoObjectPrivateBase(void)
    : _ZFP_refCount(1)
    , _ZFP_obj(zfnull)
    {
    }
    _ZFP_zfautoObjectPrivateBase(ZF_IN ZFObject *obj)
    : _ZFP_refCount(1)
    , _ZFP_obj(obj)
    {
    }
    virtual ~_ZFP_zfautoObjectPrivateBase(void)
    {
    }
public:
    virtual _ZFP_zfautoObjectPrivateBase *onRetain(void) = 0;
    virtual void onRelease(void) = 0;
};
/**
 * @brief a ZFObject holder which would release content object automatically when destroyed
 *
 * must be created by #zfautoObjectCreate\n
 * useful to hold a ZFObject as temp return value:
 * @code
 *   zfautoObject yourFunc(void)
 *   {
 *       // alloc an object for return
 *       zfblockedAlloc(ZFObject, obj);
 *
 *       // use zfautoObject to wrap the returned object
 *       return zfautoObjectCreate(obj);
 *   }
 *
 *   {
 *       zfautoObject value = yourFunc();
 *       ZFObject *obj = value;
 *   } // content obj would be released when zfautoObject destroyed
 * @endcode
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT zfautoObject
{
    /** @cond ZFPrivateDoc */
public:
    ~zfautoObject(void)
    {
        if(d != zfnull)
        {
            --(d->_ZFP_refCount);
            if(d->_ZFP_refCount == 0)
            {
                d->onRelease();
                zfdelete(d);
            }
        }
    }
    zfautoObject(ZF_IN _ZFP_zfautoObjectPrivateBase *d) : d(d) {}
    zfautoObject(void) : d(zfnull) {}
    zfautoObject(ZF_IN zfautoObject const &ref)
    : d(ref.d)
    {
        if(d != zfnull)
        {
            ++(d->_ZFP_refCount);
        }
    }

    zfautoObject &operator = (ZF_IN zfautoObject const &ref)
    {
        _ZFP_zfautoObjectPrivateBase *dTmp = d;
        d = ref.d;
        if(ref.d)
        {
            ++(ref.d->_ZFP_refCount);
        }
        if(dTmp != zfnull)
        {
            --(dTmp->_ZFP_refCount);
            if(dTmp->_ZFP_refCount == 0)
            {
                dTmp->onRelease();
                zfdelete(dTmp);
            }
        }
        return *this;
    }
    template<typename T_ZFObject>
    zfautoObject &operator = (ZF_IN T_ZFObject const &p);

    operator ZFObject *(void) const
    {
        return this->toObject();
    }
    template<typename T_ZFObject>
    operator T_ZFObject (void) const
    {
        return ZFCastZFObject(T_ZFObject, this->toObject());
    }

    zfbool operator == (ZF_IN zfautoObject const &ref) const
    {
        return (this->toObject() == ref.toObject());
    }
    zfbool operator != (ZF_IN zfautoObject const &ref) const
    {
        return (this->toObject() != ref.toObject());
    }
    zfbool operator == (ZF_IN int p) const
    {
        return (this->toObject() == zfnull && p == 0);
    }
    zfbool operator != (ZF_IN int p) const
    {
        return !(this->toObject() == zfnull && p == 0);
    }
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject const &p) const
    {
        return (this->toObject() == ZFCastZFObjectUnchecked(ZFObject *, p));
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject const &p) const
    {
        return (this->toObject() != ZFCastZFObjectUnchecked(ZFObject *, p));
    }
    /** @endcond */

public:
    /**
     * @brief get current retain count
     */
    zfindex objectRetainCount(void) const
    {
        return (d ? d->_ZFP_refCount : 0);
    }

public:
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const
    {
        return (d ? d->_ZFP_obj : zfnull);
    }
    /**
     * @brief get the holded object
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->toObject());
    }
    /**
     * @brief get the holded object
     */
    inline ZFAny toAny(void) const
    {
        return ZFAny(this->toObject());
    }

private:
    _ZFP_zfautoObjectPrivateBase *d;
};

/**
 * @brief global null zfautoObject
 */
extern ZF_ENV_EXPORT const zfautoObject zfautoObjectNull;

// ============================================================
// ZFCastZFObject fix for ZFAny
inline ZFObject *_ZFP_ZFCastZFObjectFromUnknown(zfautoObject const &obj);
inline void _ZFP_ZFCastZFObjectToUnknown(zfautoObject &ret,
                                         ZFObject * const &obj);

inline ZFObject *_ZFP_ZFCastZFObjectFromUnknownUnchecked(zfautoObject const &obj);
inline void _ZFP_ZFCastZFObjectToUnknownUnchecked(zfautoObject &ret,
                                                  ZFObject * const &obj);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfautoObjectFwd_h_

