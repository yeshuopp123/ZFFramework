/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectCastDef.h
 * @brief ZFCastZFObject declarations
 */

#ifndef _ZFI_ZFObjectCastDef_h_
#define _ZFI_ZFObjectCastDef_h_

#include "ZFObjectClassTypeFwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFClass;
zfclassFwd ZFObject;
zfclassFwd ZFInterface;

// ============================================================
/**
 * @brief safely cast ZFObject types, return null if not desired type
 *
 * types can only be ZFObject or ZFInterface types\n
 * \n
 * ADVANCED:\n
 * -  cast between ZFInterface or it's children types may take much time for dynamic checking,
 *   use with caution
 * -  try not to cast from and to the base ZFInterface * type
 */
#define ZFCastZFObject(T_To, obj) \
    _ZFP_ZFCastZFObject<T_To>::caster(obj)

/**
 * @brief cast ZFObject types without safe check, use only for performance
 *
 * ADVANCED:\n
 * -  this method would have better performance while cast ZFObject types to ZFObject types,
 *   while cast ZFInterface types would always consume some CPU for dynamic check
 * -  try not to cast from and to the base ZFInterface * type
 */
#define ZFCastZFObjectUnchecked(T_To, obj) \
    _ZFP_ZFCastZFObjectUnchecked<T_To>::caster(obj)

#define _ZFP_ZFCastZFObjectInternal(T_To, obj) \
    static_cast<T_To>(obj)


// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_ZFObjectTo, typename T_ZFObjectFrom>
inline T_ZFObjectTo _ZFP_ZFCastZFObjectExplicit(T_ZFObjectFrom const &obj)
{
    return ((obj->classData()->classIsSubclassOf(zftTraitsType<T_ZFObjectTo>::TraitsType::ClassData()))
        ? static_cast<T_ZFObjectTo>(obj)
        : zfnull);
}

template<typename T_Unknown>
inline ZFObject *_ZFP_ZFCastZFObjectFromUnknown(T_Unknown const &obj)
{
    return obj;
}
template<typename T_Unknown>
inline void _ZFP_ZFCastZFObjectToUnknown(T_Unknown &ret,
                                         ZFObject * const &obj)
{
    ret = obj;
}

template<typename T_Unknown>
inline ZFObject *_ZFP_ZFCastZFObjectFromUnknownUnchecked(T_Unknown const &obj)
{
    return obj;
}
template<typename T_Unknown>
inline void _ZFP_ZFCastZFObjectToUnknownUnchecked(T_Unknown &ret,
                                                  ZFObject * const &obj)
{
    ret = obj;
}

ZFM_CLASS_HAS_MEMBER_DECLARE(_ZFP_ZFCastZFObject, ClassData, const ZFClass * (*F)(void))
#define _ZFP_ZFCastZFObject_enableIfNotUnknown(Type) \
    typename zftEnableIf<ZFM_CLASS_HAS_MEMBER(_ZFP_ZFCastZFObject, ClassData, typename zftTraitsType<Type>::TraitsType)>::EnableIf
#define _ZFP_ZFCastZFObject_enableIfUnknown(Type) \
    typename zftEnableIf<!ZFM_CLASS_HAS_MEMBER(_ZFP_ZFCastZFObject, ClassData, typename zftTraitsType<Type>::TraitsType)>::EnableIf
#define _ZFP_ZFCastZFObject_enableIfObject(Type) \
    typename zftEnableIf<zftTypeIsTypeOf<typename zftTraitsType<Type>::TraitsType, ZFObject>::TypeIsTypeOf>::EnableIf
#define _ZFP_ZFCastZFObject_enableIfInterface(Type) \
    typename zftEnableIf<!zftTypeIsTypeOf<typename zftTraitsType<Type>::TraitsType, ZFObject>::TypeIsTypeOf>::EnableIf
/** @endcond */

// ============================================================
// ZFCastZFObject
/** @cond ZFPrivateDoc */
template<typename T_To>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObject
{
public:
    template<typename T_From>
    static inline T_To caster(T_From const &obj);
};
template<typename T_To, typename T_From,
         int T_CanCastDirectly,
         typename T_ToIsUnknown = void, typename T_FromIsUnknown = void,
         typename T_ToIsObject = void, typename T_FromIsObject = void>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectHolder
{
};

// can cast directly
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectHolder<T_To, T_From, 1>
{
public:
    static inline T_To caster(T_From const &obj)
    {
        return static_cast<T_To>(obj);
    }
};
// unknown to object/interface
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfUnknown(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        return ZFCastZFObject(T_To, _ZFP_ZFCastZFObjectFromUnknown(obj));
    }
};
// object/interface to unknown
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        T_To ret;
        _ZFP_ZFCastZFObjectToUnknown(ret, obj);
        return ret;
    }
};
// unknown to unknown
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfUnknown(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        ZFObject *tmp = _ZFP_ZFCastZFObjectFromUnknown(obj);
        T_To ret;
        _ZFP_ZFCastZFObjectToUnknown(ret, tmp);
        return ret;
    }
};
// interface to interface
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_From)
        , _ZFP_ZFCastZFObject_enableIfInterface(T_To)
        , _ZFP_ZFCastZFObject_enableIfInterface(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        return (obj
            ? static_cast<T_To>(obj->classData()->_ZFP_ZFClass_interfaceCast(
                obj->toObject(),
                zftTraitsType<T_To>::TraitsType::ClassData()))
            : zfnull);
    }
};
// object to interface
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_From)
        , _ZFP_ZFCastZFObject_enableIfInterface(T_To)
        , _ZFP_ZFCastZFObject_enableIfObject(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        return (obj
            ? static_cast<T_To>(obj->classData()->_ZFP_ZFClass_interfaceCast(
                obj,
                zftTraitsType<T_To>::TraitsType::ClassData()))
            : zfnull);
    }
};
// interface to object
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_From)
        , _ZFP_ZFCastZFObject_enableIfObject(T_To)
        , _ZFP_ZFCastZFObject_enableIfInterface(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        return (obj
            ? _ZFP_ZFCastZFObjectExplicit<T_To>(obj->toObject())
            : zfnull);
    }
};
// object to object
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_From)
        , _ZFP_ZFCastZFObject_enableIfObject(T_To)
        , _ZFP_ZFCastZFObject_enableIfObject(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        return (obj
            ? _ZFP_ZFCastZFObjectExplicit<T_To>(obj)
            : zfnull);
    }
};
template<typename T_To>
template<typename T_From>
inline T_To _ZFP_ZFCastZFObject<T_To>::caster(T_From const &obj)
{
    return _ZFP_ZFCastZFObjectHolder<T_To, T_From,
            zftTypeIsTypeOf<typename zftTraitsType<T_From>::TraitsType, typename zftTraitsType<T_To>::TraitsType>::TypeIsTypeOf
        >::caster(obj);
}
// direct cast to ZFInterface is not allowed
template<>
template<typename T_From>
inline ZFInterface *_ZFP_ZFCastZFObject<ZFInterface *>::caster(T_From const &obj)
{
    zfCoreCriticalMessageTrim(zfTextA("[ZFCastZFObject] direct cast to ZFInterface is not allowed"));
    return zfnull;
}
/** @endcond */

// ============================================================
// ZFCastZFObjectUnchecked
/** @cond ZFPrivateDoc */
template<typename T_To>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectUnchecked
{
public:
    template<typename T_From>
    static inline T_To caster(T_From const &obj);
};
template<typename T_To, typename T_From,
         int T_CanCastDirectly,
         typename T_ToIsUnknown = void, typename T_FromIsUnknown = void,
         typename T_ToIsObject = void, typename T_FromIsObject = void>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectUncheckedHolder
{
};

// can cast directly
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectUncheckedHolder<T_To, T_From, 1>
{
public:
    static inline T_To caster(T_From const &obj)
    {
        return static_cast<T_To>(obj);
    }
};
// unknown to object/interface
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectUncheckedHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfUnknown(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        return ZFCastZFObjectUnchecked(T_To, _ZFP_ZFCastZFObjectFromUnknownUnchecked(obj));
    }
};
// object/interface to unknown
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectUncheckedHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        T_To ret;
        _ZFP_ZFCastZFObjectToUnknownUnchecked(ret, obj);
        return ret;
    }
};
// unknown to unknown
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectUncheckedHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfUnknown(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        ZFObject *tmp = _ZFP_ZFCastZFObjectFromUnknownUnchecked(obj);
        T_To ret;
        _ZFP_ZFCastZFObjectToUnknownUnchecked(ret, tmp);
        return ret;
    }
};
// interface to interface
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectUncheckedHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_From)
        , _ZFP_ZFCastZFObject_enableIfInterface(T_To)
        , _ZFP_ZFCastZFObject_enableIfInterface(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        return (obj
            ? static_cast<T_To>(obj->classData()->_ZFP_ZFClass_interfaceCast(
                obj->toObject(),
                zftTraitsType<T_To>::TraitsType::ClassData()))
            : zfnull);
    }
};
// object to interface
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectUncheckedHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_From)
        , _ZFP_ZFCastZFObject_enableIfInterface(T_To)
        , _ZFP_ZFCastZFObject_enableIfObject(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        return (obj
            ? static_cast<T_To>(obj->classData()->_ZFP_ZFClass_interfaceCast(
                obj,
                zftTraitsType<T_To>::TraitsType::ClassData()))
            : zfnull);
    }
};
// interface to object
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectUncheckedHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_From)
        , _ZFP_ZFCastZFObject_enableIfObject(T_To)
        , _ZFP_ZFCastZFObject_enableIfInterface(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        return (obj
            ? static_cast<T_To>(obj->toObject())
            : zfnull);
    }
};
// object to object
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCastZFObjectUncheckedHolder<T_To, T_From, 0
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_To)
        , _ZFP_ZFCastZFObject_enableIfNotUnknown(T_From)
        , _ZFP_ZFCastZFObject_enableIfObject(T_To)
        , _ZFP_ZFCastZFObject_enableIfObject(T_From)
    >
{
public:
    static inline T_To caster(T_From const &obj)
    {
        return static_cast<T_To>(obj);
    }
};
template<typename T_To>
template<typename T_From>
inline T_To _ZFP_ZFCastZFObjectUnchecked<T_To>::caster(T_From const &obj)
{
    return _ZFP_ZFCastZFObjectUncheckedHolder<T_To, T_From,
            zftTypeIsTypeOf<typename zftTraitsType<T_From>::TraitsType, typename zftTraitsType<T_To>::TraitsType>::TypeIsTypeOf
        >::caster(obj);
}
// direct cast to ZFInterface is not allowed
template<>
template<typename T_From>
inline ZFInterface *_ZFP_ZFCastZFObjectUnchecked<ZFInterface *>::caster(T_From const &obj)
{
    zfCoreCriticalMessageTrim(zfTextA("[ZFCastZFObjectUnchecked] direct cast to ZFInterface is not allowed"));
    return zfnull;
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectCastDef_h_

