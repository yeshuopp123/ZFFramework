/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreTypeDef_ClassType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_ClassType_h_
#define _ZFI_ZFCoreTypeDef_ClassType_h_

#include "ZFCoreEnvDef.h"
#include "ZFCoreTypeDef_CoreType.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief forward declaration of a class type
 */
#define zfclassFwd class
/**
 * @brief shows the class is a POD type
 */
#define zfclassPOD class
/**
 * @brief shows the class is not a POD type,
 *   but you may use it like a POD
 *   except memset it to 0
 */
#define zfclassLikePOD class
/**
 * @brief shows the class is not a POD type,
 *   you should not memset it
 *   or declare it in stack
 *   or copy value by copy constructor or operator =
 */
#define zfclassNotPOD class

/**
 * @brief extends for #zfclassLikePOD
 */
#define zfextendsLikePOD public
/**
 * @brief extends for #zfclassPOD
 */
#define zfextendsPOD public
/**
 * @brief extends for #zfclassNotPOD
 */
#define zfextendsNotPOD public

/**
 * @brief dummy macro shows that method override parent's method
 */
#define zfoverride

/**
 * @brief dummy macro shows that a method or class is designed must not to be overrided
 */
#define zffinal

/**
 * @brief dummy macro shows that a method is pure virtual method
 */
#define zfpurevirtual = 0

/**
 * @brief dummy macro shows the object can not be allocated normally,
 *   marked for reminder
 */
#define ZFCLASS_PRIVATE_ALLOC(hint)

// ============================================================
/**
 * @brief same as dynamic_cast<Type>(obj), require #ZF_ENV_RTTI
 */
#if ZF_ENV_RTTI
    #define ZFCastDynamic(Type, obj) (dynamic_cast<Type>(obj))
#else
    #define ZFCastDynamic(Type, obj) "ZFCastDynamic depends on ZF_ENV_RTTI"
#endif
/**
 * @brief same as static_cast<Type>(obj)
 */
#define ZFCastStatic(Type, obj) (static_cast<Type>(obj))
/**
 * @brief same as reinterpret_cast<Type>(obj)
 */
#define ZFCastReinterpret(Type, obj) (reinterpret_cast<Type>(obj))

// ============================================================
/**
 * @brief a macro to show that a class doesn't allow copy constructor
 *
 * usage:
 * @code
 *   class YourClass
 *   {
 *       ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(YourClass)
 *   };
 * @endcode
 */
#define ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ClassName) \
    private: \
        ClassName(const ClassName &); \
        ClassName &operator =(const ClassName &); \
    public:

// ============================================================
/**
 * @brief same as new defined for future use
 */
#define zfnew(Type, ...) (new Type(__VA_ARGS__))
/**
 * @brief same as new[] defined for future use
 */
#define zfnewArray(Type, size) (new Type[size])
/**
 * @brief placement new defined for future use
 */
#define zfnewPlacement(buf, Type, ...) (new (buf) Type(__VA_ARGS__))
/**
 * @brief same as delete defined for future use
 */
#define zfdelete(instance) delete(instance)
/**
 * @brief same as delete[] defined for future use
 */
#define zfdeleteArray(instance) delete[](instance)
template<typename Type>
inline void _ZFP_zfdeletePlacementAction(Type *obj)
{
    obj->~Type();
}
/**
 * @brief placement delete (instance->~Type()) defined for future use
 */
#define zfdeletePlacement(instance) _ZFP_zfdeletePlacementAction(instance)

/**
 * @brief same as malloc defined for future use
 */
inline void *zfmalloc(zfindex size)
{
    return ::malloc((size_t)size);
}
/**
 * @brief util method to #zfmalloc and #zfmemset to zero
 */
extern ZF_ENV_EXPORT void *zfmallocZero(zfindex size);
/**
 * @brief same as realloc defined for future use
 */
inline void *zfrealloc(void *oldPtr, zfindex newSize)
{
    return ::realloc(oldPtr, (size_t)newSize);
}
/**
 * @brief util method to #zfrealloc and #zfmemset the increased memory to zero
 */
extern ZF_ENV_EXPORT void *zfreallocZero(void *oldPtr, zfindex newSize, zfindex oldSize);
/**
 * @brief same as free defined for future use,
 *   do nothing if ptr is NULL
 */
inline void zffree(void *ptr)
{
    ::free(ptr);
}

/**
 * @brief wrapper to memset
 */
inline void *zfmemset(void *p, zfint ch, zfindex size)
{
    return ::memset(p, ch, (size_t)size);
}
/**
 * @brief wrapper to memcpy
 */
inline void *zfmemcpy(void *dst, const void *src, zfindex size)
{
    return ::memcpy(dst, src, (size_t)size);
}
/**
 * @brief wrapper to memmove
 */
inline void *zfmemmove(void *dst, const void *src, zfindex size)
{
    return ::memmove(dst, src, (size_t)size);
}
/**
 * @brief wrapper to memcmp
 */
inline zfint zfmemcmp(const void *p1, const void *p2, zfindex size)
{
    return (zfint)::memcmp(p1, p2, (size_t)size);
}

/**
 * @brief memmove for common object type, object must support operator =
 *
 * it's all right if dst and src share same memory
 */
template<typename T_Element>
T_Element *zfmemmoveObject(T_Element *dst, const T_Element *src, zfindex count)
{
    if(dst && src && count != 0 && count != zfindexMax)
    {
        if(dst > src && src + count > dst)
        {
            const T_Element *srcEnd = src;
            src += count;
            dst += count;
            while(src > srcEnd)
            {
                --dst;
                --src;
                *dst = *src;
            }
        }
        else
        {
            const T_Element *srcEnd = src + count;
            while(src < srcEnd)
            {
                *dst = *src;
                ++dst;
                ++src;
            }
        }
    }
    return dst;
}

// ============================================================
/**
 * @brief dummy macro that shows the param used as required input
 */
#define ZF_IN
/**
 * @brief dummy macro that shows the param used as optional input
 */
#define ZF_IN_OPT
/**
 * @brief dummy macro that shows the param used as required output
 */
#define ZF_OUT
/**
 * @brief dummy macro that shows the param used as optional output
 */
#define ZF_OUT_OPT
/**
 * @brief dummy macro that shows the param used as required input and output
 */
#define ZF_IN_OUT
/**
 * @brief dummy macro that shows the param used as optional input and output
 */
#define ZF_IN_OUT_OPT

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_ClassType_h_

