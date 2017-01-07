/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFMemPoolDef.h
 * @brief memory pool
 */
#ifndef _ZFI_ZFMemPoolDef_h_
#define _ZFI_ZFMemPoolDef_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief whether enable memory pool, true by default
 */
#ifndef ZFMEMPOOL_ENABLE
    #define ZFMEMPOOL_ENABLE 1
#endif

// ============================================================
/**
 * @brief util macro to log #zfpoolNew, do nothing by default
 */
#ifndef zfpoolNewLogger
    #define zfpoolNewLogger(Type, obj) (obj)
#endif

/**
 * @brief util macro to log #zfpoolDelete, do nothing by default
 */
#ifndef zfpoolDeleteLogger
    #define zfpoolDeleteLogger(obj) (obj)
#endif

// ============================================================
/**
 * @def zfpoolNew
 * @brief internal use only, for allocating internal types for performance
 *
 * @warning no thread-safe
 * @warning pointers passed to zfpoolDelete must be the same with the type you create
 * @def zfpoolDelete
 * @brief see #zfpoolNew
 * @def zfpoolDeclareFriend
 * @brief use to declare friend if your type has non-public constructors
 */
#if ZFMEMPOOL_ENABLE
    #define zfpoolNew(T_Type, ...) zfpoolNewLogger(T_Type, new (_ZFP_zfpoolObjectHolder<T_Type>::poolMalloc()) T_Type(__VA_ARGS__))
    #define zfpoolDelete(obj) _ZFP_zfpoolDelete(zfpoolDeleteLogger(obj))
    #define zfpoolDeclareFriend() \
        template<typename T_Type> \
        friend zfclassFwd ZF_NAMESPACE_GLOBAL::_ZFP_zfpoolObjectHolder;
#else
    #define zfpoolNew(T_Type, ...) zfpoolNewLogger(T_Type, zfnew(T_Type, ##__VA_ARGS__))
    #define zfpoolDelete(obj) zfdelete(zfpoolDeleteLogger(obj))
    #define zfpoolDeclareFriend()
#endif

// ============================================================
// impl
zfclassFwd _ZFP_zfpoolObjectPrivate;
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfpoolObject
{
public:
    void *poolMalloc(void);
    void poolFree(ZF_IN void *obj);
public:
    _ZFP_zfpoolObject(void);
    ~_ZFP_zfpoolObject(void);
    _ZFP_zfpoolObjectPrivate *d;
};
extern ZF_ENV_EXPORT _ZFP_zfpoolObject *_ZFP_zfpoolObjectAccess(ZF_IN zfindex objectSize);

template<typename T_Type>
zfclassNotPOD _ZFP_zfpoolObjectHolder
{
public:
    static void *poolMalloc(void)
    {
        static _ZFP_zfpoolObject &pool = *_ZFP_zfpoolObjectAccess(sizeof(T_Type));
        return pool.poolMalloc();
    }
    static void poolDelete(ZF_IN T_Type *obj)
    {
        static _ZFP_zfpoolObject &pool = *_ZFP_zfpoolObjectAccess(sizeof(T_Type));
        obj->~T_Type();
        pool.poolFree(obj);
    }
};
template<typename T_Type>
inline void _ZFP_zfpoolDelete(ZF_IN T_Type *obj)
{
    if(obj)
    {
        _ZFP_zfpoolObjectHolder<T_Type>::poolDelete(obj);
    }
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFMemPoolDef_h_

