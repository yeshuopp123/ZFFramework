/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectCoreRetainDef.h
 * @brief retain count logic for ZFFramework
 */

#ifndef _ZFI_ZFObjectCoreRetainDef_h_
#define _ZFI_ZFObjectCoreRetainDef_h_

#include "ZFObjectDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfAlloc
template<typename T_ZFObject, int valid>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFObjectAllocChecker
{
};
template<typename T_ZFObject>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFObjectAllocChecker<T_ZFObject, 1>
{
public:
    typedef T_ZFObject CanAlloc;
};
template<typename T_ZFObject>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFObjectAllocChecker<T_ZFObject, 0>
{
};
#define _ZFP_zfAlloc(T_ZFObject, ...) \
    _ZFP_ZFCastZFObjectInternal(T_ZFObject *, _ZFP_ZFCastZFObjectInternal(T_ZFObject *, \
        (ZFObject::_ZFP_ZFObjectAlloc( \
            _ZFP_ZFObjectAllocChecker<T_ZFObject, T_ZFObject::_ZFP_ZFObjectCanAlloc>::CanAlloc::_ZFP_ZFObject_constructor())) \
        )->objectOnInit(__VA_ARGS__)->_ZFP_ZFObjectCheckOnInit())
/**
 * @brief won't be logged by ZFLeakTest, use only if necessary
 * @see zfAlloc, ZFLeakTestBegin
 */
#define zfAllocWithoutLeakTest(T_ZFObject, ...) \
    (ZFCoreMutexLockerHolder(), zflockfree_zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__))
/** @brief no lock version of #zfAllocWithoutLeakTest, use with causion */
#define zflockfree_zfAllocWithoutLeakTest(T_ZFObject, ...) \
    _ZFP_zfAlloc(T_ZFObject, ##__VA_ARGS__)

// ============================================================
// zfRetain
inline void _ZFP_zfRetainAction(ZF_IN ZFObject *obj)
{
    obj->objectOnRetain();
}
template<typename T_ZFObject, int isZFObjectType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfRetainHolder {};
template<typename T_ZFObject>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfRetainHolder<T_ZFObject, 1>
{
public:
    static inline void action(ZF_IN T_ZFObject *obj)
    {
        _ZFP_zfRetainAction(obj);
    }
};
template<typename T_ZFObject>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfRetainHolder<T_ZFObject, 0>
{
public:
    static inline void action(ZF_IN T_ZFObject *obj)
    {
        _ZFP_zfRetainAction(obj->toObject());
    }
};
template<typename T_ZFObject>
inline T_ZFObject *_ZFP_zfRetain(ZF_IN T_ZFObject *obj)
{
    if(obj)
    {
        _ZFP_zfRetainHolder<T_ZFObject, zftTypeIsTypeOf<T_ZFObject, ZFObject>::TypeIsTypeOf>::action(obj);
    }
    return obj;
}
inline const ZFAny &_ZFP_zfRetain(ZF_IN const ZFAny &any)
{
    _ZFP_zfRetain(any.toObject());
    return any;
}
template<typename T_ZFObject>
inline const ZFAnyT<T_ZFObject *> &_ZFP_zfRetain(ZF_IN const ZFAnyT<T_ZFObject *> &any)
{
    _ZFP_zfRetain(any.toObjectT());
    return any;
}
/**
 * @brief won't be logged by ZFLeakTest, use only if necessary
 * @see zfRetain, ZFLeakTestBegin
 */
#define zfRetainWithoutLeakTest(obj) \
    (ZFCoreMutexLockerHolder(), zflockfree_zfRetainWithoutLeakTest(obj))
/** @brief no lock version of #zfRetainWithoutLeakTest, use with causion */
#define zflockfree_zfRetainWithoutLeakTest(obj) \
    _ZFP_zfRetain(obj)

// ============================================================
// zfRelease
inline void _ZFP_zfReleaseAction(ZF_IN ZFObject *obj)
{
    obj->objectOnRelease();
    if(obj->objectRetainCount() == 0)
    {
        ZFObject::_ZFP_ZFObjectDealloc(obj);
    }
}
template<typename T_ZFObject, int isZFObjectType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfReleaseHolder {};
template<typename T_ZFObject>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfReleaseHolder<T_ZFObject, 1>
{
public:
    static inline void action(ZF_IN T_ZFObject *obj)
    {
        _ZFP_zfReleaseAction(obj);
    }
};
template<typename T_ZFObject>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfReleaseHolder<T_ZFObject, 0>
{
public:
    static inline void action(ZF_IN T_ZFObject *obj)
    {
        _ZFP_zfReleaseAction(obj->toObject());
    }
};
template<typename T_ZFObject>
inline void _ZFP_zfRelease(ZF_IN T_ZFObject *obj)
{
    if(obj)
    {
        _ZFP_zfReleaseHolder<T_ZFObject, zftTypeIsTypeOf<T_ZFObject, ZFObject>::TypeIsTypeOf>::action(obj);
    }
}
inline void _ZFP_zfRelease(ZF_IN const ZFAny &any)
{
    _ZFP_zfRelease(any.toObject());
}
template<typename T_ZFObject>
inline void _ZFP_zfRelease(ZF_IN const ZFAnyT<T_ZFObject *> &any)
{
    _ZFP_zfRelease(any.toObject());
}
/**
 * @brief won't be logged by ZFLeakTest, use only if necessary
 * @see zfRelease, ZFLeakTestBegin
 */
#define zfReleaseWithoutLeakTest(obj) \
    (ZFCoreMutexLockerHolder(), zflockfree_zfReleaseWithoutLeakTest(obj))
/** @brief no lock version of #zfReleaseWithoutLeakTest, use with causion */
#define zflockfree_zfReleaseWithoutLeakTest(obj) \
    _ZFP_zfRelease(obj)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectCoreRetainDef_h_

