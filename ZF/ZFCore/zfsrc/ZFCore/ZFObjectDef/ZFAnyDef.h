/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFAnyDef.h
 * @brief util class to cast ZFObject types freely
 */

#ifndef _ZFI_ZFAnyDef_h_
#define _ZFI_ZFAnyDef_h_

#include "ZFObjectCastDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFObject;
zfclassFwd ZFInterface;

// ============================================================
// ZFAny
template<typename T_ZFObjectBase>
zfclassFwd ZFAnyT;
/**
 * @brief util method to cast ZFObject types freely
 *
 * usage:
 * @code
 *   YourObjectType *obj = ...;
 *   ZFAny any = obj;
 *
 *   // cast to any other object type, return null if not able to cast
 *   AnotherObjectType *another = any;
 *
 *   // or use toObject
 *   ZFObject *orig = any.toObject();
 * @endcode
 */
zfclassLikePOD ZF_ENV_EXPORT ZFAny
{
    /** @cond ZFPrivateDoc */
public:
    ZFAny(void)
    : _obj(zfnull)
    {
    }
    template<typename T_ZFObject>
    ZFAny(ZF_IN T_ZFObject *obj);
    template<typename T_ZFObjectBase>
    ZFAny(ZF_IN const ZFAnyT<T_ZFObjectBase *> &ref);
public:
    template<typename T_ZFObject>
    ZFAny &operator =(ZF_IN T_ZFObject *obj);
    ZFAny &operator =(ZF_IN const ZFAny &ref)
    {
        this->_obj = ref.toObject();
        return *this;
    }
    template<typename T_ZFObjectBase>
    ZFAny &operator =(ZF_IN const ZFAnyT<T_ZFObjectBase *> &ref);

public:
    template<typename T_ZFObject>
    inline operator T_ZFObject *(void) const;
    inline ZFObject *toObject(void) const
    {
        return this->_obj;
    }
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const;
    /** @endcond */

private:
    ZFObject *_obj;
};

// ============================================================
// ZFAnyT
/**
 * @brief util method to cast ZFObject types freely, see #ZFAny
 *
 * usage:
 * @code
 *   YourObjectType *obj = ...;
 *   ZFAnyT<BaseObjectType *> any = obj;
 *
 *   // cast to any other object type
 *   // AnotherObjectType must be child of BaseObjectType
 *   // else, compile error
 *   AnotherObjectType *another = any;
 *
 *   // or use toObject
 *   ZFObject *orig = any.toObject();
 * @endcode
 */
template<typename T_ZFObjectBase>
zfclassLikePOD ZF_ENV_EXPORT ZFAnyT
{
    /** @cond ZFPrivateDoc */
public:
    ZFAnyT(void)
    : _obj(zfnull)
    {
    }
    template<typename T_ZFObject>
    ZFAnyT(ZF_IN T_ZFObject *obj)
    : _obj(obj)
    {
    }
    ZFAnyT(ZF_IN const ZFAnyT<T_ZFObjectBase> &ref)
    : _obj(ref.toObjectT())
    {
    }
    template<typename T_ZFObject>
    ZFAnyT(ZF_IN const ZFAnyT<T_ZFObject *> &ref)
    : _obj(ref.toObjectT())
    {
    }
    ZFAnyT(ZF_IN const ZFAny &ref);
public:
    template<typename T_ZFObject>
    ZFAnyT<T_ZFObjectBase> &operator =(ZF_IN T_ZFObject *obj)
    {
        this->_obj = obj;
        return *this;
    }
    ZFAnyT<T_ZFObjectBase> &operator =(ZF_IN const ZFAnyT<T_ZFObjectBase> &ref)
    {
        this->_obj = ref.toObjectT();
        return *this;
    }
    template<typename T_ZFObject>
    ZFAnyT<T_ZFObjectBase> &operator =(ZF_IN const ZFAnyT<T_ZFObject *> &ref)
    {
        this->_obj = ref.toObjectT();
        return *this;
    }
    ZFAnyT<T_ZFObjectBase> &operator =(ZF_IN const ZFAny &ref);

public:
    template<typename T_ZFObject>
    inline operator T_ZFObject *(void) const;
    inline ZFObject *toObject(void) const;
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const;
    inline T_ZFObjectBase toObjectT(void) const
    {
        return this->_obj;
    }
    /** @endcond */

private:
    template<typename T_ZFObject, int canCast>
    zfclassNotPOD _ZFP_ZFAnyTypeChecker
    {
    };
    template<typename T_ZFObject>
    zfclassNotPOD _ZFP_ZFAnyTypeChecker<T_ZFObject, 1>
    {
    public:
        typedef T_ZFObject TypeMatched;
    };
private:
    T_ZFObjectBase _obj;
};

// ============================================================
// ZFCastZFObject fix for ZFAny
inline ZFObject *_ZFP_ZFCastZFObjectFromUnknown(ZFAny const &obj)
{
    return obj;
}
inline void _ZFP_ZFCastZFObjectToUnknown(ZFAny &ret,
                                         ZFObject * const &obj)
{
    ret = obj;
}

inline ZFObject *_ZFP_ZFCastZFObjectFromUnknownUnchecked(ZFAny const &obj)
{
    return obj;
}
inline void _ZFP_ZFCastZFObjectToUnknownUnchecked(ZFAny &ret,
                                                  ZFObject * const &obj)
{
    ret = obj;
}

// ============================================================
// ZFCastZFObject fix for ZFAnyT
template<typename T_ZFObject>
inline ZFObject *_ZFP_ZFCastZFObjectFromUnknown(ZFAnyT<T_ZFObject *> const &obj)
{
    return obj.toObject();
}
template<typename T_ZFObject>
inline void _ZFP_ZFCastZFObjectToUnknown(ZFAnyT<T_ZFObject *> &ret,
                                         ZFObject * const &obj)
{
    ret = ZFCastZFObject(T_ZFObject *, obj);
}

template<typename T_ZFObject>
inline ZFObject *_ZFP_ZFCastZFObjectFromUnknownUnchecked(ZFAnyT<T_ZFObject *> const &obj)
{
    return obj.toObject();
}
template<typename T_ZFObject>
inline void _ZFP_ZFCastZFObjectToUnknownUnchecked(ZFAnyT<T_ZFObject *> &ret,
                                                  ZFObject * const &obj)
{
    ret = ZFCastZFObjectUnchecked(T_ZFObject *, obj);
}

// ============================================================
// ZFAny impl
/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
ZFAny::ZFAny(ZF_IN T_ZFObject *obj)
: _obj(ZFCastZFObjectUnchecked(ZFObject *, obj))
{
}
template<typename T_ZFObjectBase>
ZFAny::ZFAny(ZF_IN const ZFAnyT<T_ZFObjectBase *> &ref)
: _obj(ref.toObject())
{
}
template<typename T_ZFObject>
ZFAny &ZFAny::operator =(ZF_IN T_ZFObject *obj)
{
    this->_obj = ZFCastZFObjectUnchecked(ZFObject *, obj);
    return *this;
}
template<typename T_ZFObjectBase>
ZFAny &ZFAny::operator =(ZF_IN const ZFAnyT<T_ZFObjectBase *> &ref)
{
    this->_obj = ref.toObject();
    return *this;
}
template<typename T_ZFObject>
inline ZFAny::operator T_ZFObject *(void) const
{
    return ZFCastZFObject(T_ZFObject *, this->_obj);
}
template<typename T_ZFObject>
inline T_ZFObject ZFAny::to(void) const
{
    return ZFCastZFObjectUnchecked(T_ZFObject, this->_obj);
}
/** @endcond */

// ============================================================
// ZFAny impl
/** @cond ZFPrivateDoc */
template<typename T_ZFObjectBase>
ZFAnyT<T_ZFObjectBase>::ZFAnyT(ZF_IN const ZFAny &ref)
: _obj(ZFCastZFObject(T_ZFObjectBase, ref.toObject()))
{
}
template<typename T_ZFObjectBase>
ZFAnyT<T_ZFObjectBase> &ZFAnyT<T_ZFObjectBase>::operator =(ZF_IN const ZFAny &ref)
{
    this->_obj = ZFCastZFObject(T_ZFObjectBase, ref.toObject());
    return *this;
}

template<typename T_ZFObjectBase>
template<typename T_ZFObject>
inline ZFAnyT<T_ZFObjectBase>::operator T_ZFObject *(void) const
{
    return ZFCastZFObjectUnchecked(
        typename _ZFP_ZFAnyTypeChecker<T_ZFObject * ZFM_COMMA()
                zftTypeIsTypeOf<
                        typename zftTraitsType<T_ZFObject>::TraitsType ZFM_COMMA()
                        typename zftTraitsType<T_ZFObjectBase>::TraitsType
                    >::TypeIsTypeOf
            >::TypeMatched,
        this->_obj);
}
template<typename T_ZFObjectBase>
inline ZFObject *ZFAnyT<T_ZFObjectBase>::toObject(void) const
{
    return ZFCastZFObjectUnchecked(ZFObject *, this->_obj);
}
template<typename T_ZFObjectBase>
template<typename T_ZFObject>
inline T_ZFObject ZFAnyT<T_ZFObjectBase>::to(void) const
{
    return ZFCastZFObjectUnchecked(
        typename _ZFP_ZFAnyTypeChecker<T_ZFObject ZFM_COMMA()
                zftTypeIsTypeOf<
                        typename zftTraitsType<T_ZFObject>::TraitsType ZFM_COMMA()
                        typename zftTraitsType<T_ZFObjectBase>::TraitsType
                    >::TypeIsTypeOf
            >::TypeMatched,
        this->_obj);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAnyDef_h_

