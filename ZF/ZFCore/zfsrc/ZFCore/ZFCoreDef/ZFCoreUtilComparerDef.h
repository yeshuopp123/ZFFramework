/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreUtilComparerDef.h
 * @brief common comparer for ZFFramework
 */

#ifndef _ZFI_ZFCoreUtilComparerDef_h_
#define _ZFI_ZFCoreUtilComparerDef_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreUtilTemplateDef.h"
#include "ZFCoreUtilMacroDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief comparer holder fo #ZFComparerDefault,
 *   you may specialize this class to supply your custom type's comparation,
 *   or use #ZFCOMPARER_DEFAULT_DECLARE for short
 */
template<typename T_Comparable0, typename T_Comparable1,
         typename TypeFix0 = void, typename TypeFix1 = void,
         typename T_ReservedFix0 = void, typename T_ReservedFix1 = void>
zfclassNotPOD ZF_ENV_EXPORT ZFComparerDefaultHolder
{
public:
    /**
     * @brief default comparer, compare by value
     *
     * element must support operator = and operator < and operator >
     */
    static ZFCompareResult comparer(ZF_IN T_Comparable0 const &v0, ZF_IN T_Comparable1 const &v1)
    {
        if(v0 == v1)
        {
            return ZFCompareTheSame;
        }
        else if(v0 < v1)
        {
            return ZFCompareSmaller;
        }
        else if(v0 > v1)
        {
            return ZFCompareGreater;
        }
        else
        {
            return ZFCompareUncomparable;
        }
    }
};
template<typename T_Comparable0, typename T_Comparable1>
inline ZFCompareResult _ZFP_ZFComparerDefault(ZF_IN T_Comparable0 const &v0, ZF_IN T_Comparable1 const &v1)
{
    return ZFComparerDefaultHolder<T_Comparable0, T_Comparable1>::comparer(v0, v1);
}
/**
 * @brief default comparer for common types, see #ZFComparer
 *
 * by default, compare by operator ==, < and >,
 * you may use #ZFCOMPARER_DEFAULT_DECLARE to supply your own type's compare method
 */
#define ZFComparerDefault _ZFP_ZFComparerDefault

/**
 * @brief declare a template specialization for #ZFComparerDefault
 *
 * usage:
 * @code
 *   ZFCOMPARER_DEFAULT_DECLARE(Type0, Type1, {
 *           return yourCompareMethod(v0, v1);
 *       })
 * @endcode
 * @note all subclass of the declared type would be specialized by this method,
 *   if you don't want it, use #ZFCOMPARER_DEFAULT_DECLARE_EXPLICIT instead
 */
#define ZFCOMPARER_DEFAULT_DECLARE(T_Comparable0, T_Comparable1, compareAction) \
    /** @cond ZFPrivateDoc */ \
    template<typename T0, typename T1> \
    zfclassNotPOD ZFComparerDefaultHolder<T0, T1 \
        , typename zftEnableIf<zftTypeIsTypeOf<T0, T_Comparable0>::TypeIsTypeOf>::EnableIf \
        , typename zftEnableIf<zftTypeIsTypeOf<T1, T_Comparable1>::TypeIsTypeOf>::EnableIf \
        > \
    { \
    public: \
        static ZFCompareResult comparer(ZF_IN T0 const &v0, ZF_IN T1 const &v1) \
        { \
            compareAction \
        } \
    }; \
    /** @endcond */

/**
 * @brief see #ZFCOMPARER_DEFAULT_DECLARE
 */
#define ZFCOMPARER_DEFAULT_DECLARE_EXPLICIT(T_Comparable0, T_Comparable1, compareAction) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFComparerDefaultHolder<T_Comparable0, T_Comparable1> \
    { \
    public: \
        static ZFCompareResult comparer(ZF_IN T_Comparable0 const &v0, ZF_IN T_Comparable1 const &v1) \
        { \
            compareAction \
        } \
    }; \
    /** @endcond */

// ============================================================
template<typename T0, typename T1>
inline ZFCompareResult _ZFP_ZFComparerNumeric(ZF_IN T0 const &v0, ZF_IN T1 const &v1)
{
    if(v0 == v1)
    {
        return ZFCompareTheSame;
    }
    else if(v0 < v1)
    {
        return ZFCompareSmaller;
    }
    else if(v0 > v1)
    {
        return ZFCompareGreater;
    }
    else
    {
        return ZFCompareUncomparable;
    }
}
/**
 * @brief numeric comparer compare by operator ==, < and >, see #ZFComparer
 */
#define ZFComparerNumeric _ZFP_ZFComparerNumeric

// ============================================================
template<typename T_Comparable0, typename T_Comparable1>
inline ZFCompareResult _ZFP_ZFComparerCheckEqualOnly(ZF_IN T_Comparable0 const &v0, ZF_IN T_Comparable1 const &v1)
{
    return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
/**
 * @brief compare by operator == only, see #ZFComparer
 *
 * useful for some non-POD types that can only be compared with operator ==
 */
#define ZFComparerCheckEqualOnly _ZFP_ZFComparerCheckEqualOnly

template<typename T_Comparable>
inline ZFCompareResult _ZFP_ZFComparerForPOD(ZF_IN T_Comparable const &v0, ZF_IN T_Comparable const &v1)
{
    return ((zfmemcmp(&v0, &v1, sizeof(T_Comparable)) == 0) ? ZFCompareTheSame : ZFCompareUncomparable);
}
/**
 * @brief default comparer for POD types, compare by memory, see #ZFComparer
 */
#define ZFComparerForPOD _ZFP_ZFComparerForPOD

template<typename T_Comparable>
inline ZFCompareResult _ZFP_ZFComparerDummy(ZF_IN T_Comparable const &v0, ZF_IN T_Comparable const &v1)
{
    return ZFCompareUncomparable;
}
/**
 * @brief dummy comparer that always return ZFCompareUncomparable, see #ZFComparer
 */
#define ZFComparerDummy _ZFP_ZFComparerDummy

// ============================================================
ZFCOMPARER_DEFAULT_DECLARE(zfbool, zfbool, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })
ZFCOMPARER_DEFAULT_DECLARE(const zfchar *, const zfchar *, {
        const zfchar *v0Tmp = ((v0 == zfnull) ? zfText("") : v0);
        const zfchar *v1Tmp = ((v1 == zfnull) ? zfText("") : v1);
        zfint result = zfscmp(v0Tmp, v1Tmp);
        if(result < 0)
        {
            return ZFCompareSmaller;
        }
        else if(result > 0)
        {
            return ZFCompareGreater;
        }
        else
        {
            return ZFCompareTheSame;
        }
    })
ZFCOMPARER_DEFAULT_DECLARE(zfstring, zfstring, {
        zfint result = v0.compare(v1);
        if(result < 0)
        {
            return ZFCompareSmaller;
        }
        else if(result > 0)
        {
            return ZFCompareGreater;
        }
        else
        {
            return ZFCompareTheSame;
        }
    })

ZFCOMPARER_DEFAULT_DECLARE(ZFCompareResult, ZFCompareResult, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })
ZFCOMPARER_DEFAULT_DECLARE(ZFSeekPos, ZFSeekPos, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })
ZFCOMPARER_DEFAULT_DECLARE(zfindexRange, zfindexRange, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

// ============================================================
/**
 * @brief util method to compare two POD type
 */
template<typename T_Element>
inline zfbool zfcmpPODTheSame(ZF_IN T_Element const &v0, ZF_IN T_Element const &v1)
{
    return (zfmemcmp(&v0, &v1, sizeof(T_Element)) == 0);
}

/**
 * @brief utile macro to declare a operator== comparer of POD type
 */
#define ZFCORE_POD_COMPARER_DECLARE(Type) \
    /** @brief compare by value */ \
    inline zfbool Type##IsEqual(ZF_IN Type const &v0, ZF_IN Type const &v1) \
    { \
        return zfcmpPODTheSame(v0, v1); \
    } \
    /** @brief compare by value */ \
    inline zfbool operator == (ZF_IN Type const &v0, ZF_IN Type const &v1) \
    { \
        return zfcmpPODTheSame(v0, v1); \
    } \
    /** @brief compare by value */ \
    inline zfbool operator != (ZF_IN Type const &v0, ZF_IN Type const &v1) \
    { \
        return !zfcmpPODTheSame(v0, v1); \
    }

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreUtilComparerDef_h_

