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
 *   or use #ZFCOMPARER_DEFAULT_DECLARE_BEGIN for short
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
    static ZFCompareResult comparer(ZF_IN T_Comparable0 const &e0, ZF_IN T_Comparable1 const &e1)
    {
        if(e0 == e1)
        {
            return ZFCompareTheSame;
        }
        else if(e0 < e1)
        {
            return ZFCompareSmaller;
        }
        else if(e0 > e1)
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
inline ZFCompareResult _ZFP_ZFComparerDefault(ZF_IN T_Comparable0 const &e0, ZF_IN T_Comparable1 const &e1)
{
    return ZFComparerDefaultHolder<T_Comparable0, T_Comparable1>::comparer(e0, e1);
}
/**
 * @brief default comparer for common types, see #ZFComparer
 *
 * by default, compare by operator ==, < and >,
 * you may use #ZFCOMPARER_DEFAULT_DECLARE_BEGIN to supply your own type's compare method
 */
#define ZFComparerDefault _ZFP_ZFComparerDefault

/**
 * @brief declare a template specialization for #ZFComparerDefault
 *
 * usage:
 * @code
 *   ZFCOMPARER_DEFAULT_DECLARE_BEGIN(Type0, param0, Type1, param1)
 *   {
 *       return yourCompareMethod(param0, param1);
 *   }
 *   ZFCOMPARER_DEFAULT_DECLARE_END(Type0, param0, Type1, param1)
 * @endcode
 * @note all subclass of the declared type would be specialized by this method,
 *   if you don't want it, use #ZFCOMPARER_DEFAULT_DECLARE_EXPLICIT_BEGIN instead
 */
#define ZFCOMPARER_DEFAULT_DECLARE_BEGIN(T_Comparable0, paramName0, T_Comparable1, paramName1) \
    /** @cond ZFPrivateDoc */ \
    template<typename T0, typename T1> \
    zfclassNotPOD ZFComparerDefaultHolder<T0, T1 \
        , typename zftEnableIf<zftTypeIsTypeOf<T0, T_Comparable0>::TypeIsTypeOf>::EnableIf \
        , typename zftEnableIf<zftTypeIsTypeOf<T1, T_Comparable1>::TypeIsTypeOf>::EnableIf \
        > \
    { \
    public: \
        static ZFCompareResult comparer(ZF_IN T0 const &paramName0, ZF_IN T1 const &paramName1)
/**
 * @brief see #ZFCOMPARER_DEFAULT_DECLARE_BEGIN
 */
#define ZFCOMPARER_DEFAULT_DECLARE_END(T_Comparable0, paramName0, T_Comparable1, paramName1) \
    }; \
    /** @endcond */

/**
 * @brief see #ZFCOMPARER_DEFAULT_DECLARE_BEGIN
 */
#define ZFCOMPARER_DEFAULT_DECLARE_EXPLICIT_BEGIN(T_Comparable0, paramName0, T_Comparable1, paramName1) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFComparerDefaultHolder<T_Comparable0, T_Comparable1> \
    { \
    public: \
        static ZFCompareResult comparer(ZF_IN T_Comparable0 const &paramName0, ZF_IN T_Comparable1 const &paramName1)
/**
 * @brief see #ZFCOMPARER_DEFAULT_DECLARE_EXPLICIT_BEGIN
 */
#define ZFCOMPARER_DEFAULT_DECLARE_EXPLICIT_END(T_Comparable0, paramName0, T_Comparable1, paramName1) \
    }; \
    /** @endcond */

// ============================================================
template<typename T0, typename T1>
inline ZFCompareResult _ZFP_ZFComparerNumeric(ZF_IN T0 const &e0, ZF_IN T1 const &e1)
{
    if(e0 == e1)
    {
        return ZFCompareTheSame;
    }
    else if(e0 < e1)
    {
        return ZFCompareSmaller;
    }
    else if(e0 > e1)
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
inline ZFCompareResult _ZFP_ZFComparerCheckEqualOnly(ZF_IN T_Comparable0 const &e0, ZF_IN T_Comparable1 const &e1)
{
    return ((e0 == e1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
/**
 * @brief compare by operator == only, see #ZFComparer
 *
 * useful for some non-POD types that can only be compared with operator ==
 */
#define ZFComparerCheckEqualOnly _ZFP_ZFComparerCheckEqualOnly

template<typename T_Comparable>
inline ZFCompareResult _ZFP_ZFComparerForPOD(ZF_IN T_Comparable const &e0, ZF_IN T_Comparable const &e1)
{
    return ((zfmemcmp(&e0, &e1, sizeof(T_Comparable)) == 0) ? ZFCompareTheSame : ZFCompareUncomparable);
}
/**
 * @brief default comparer for POD types, compare by memory, see #ZFComparer
 */
#define ZFComparerForPOD _ZFP_ZFComparerForPOD

template<typename T_Comparable>
inline ZFCompareResult _ZFP_ZFComparerDummy(ZF_IN T_Comparable const &e0, ZF_IN T_Comparable const &e1)
{
    return ZFCompareUncomparable;
}
/**
 * @brief dummy comparer that always return ZFCompareUncomparable, see #ZFComparer
 */
#define ZFComparerDummy _ZFP_ZFComparerDummy

// ============================================================
ZFCOMPARER_DEFAULT_DECLARE_BEGIN(zfbool, e0, zfbool, e1)
{
    return ((e0 == e1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCOMPARER_DEFAULT_DECLARE_END(zfbool, e0, zfbool, e1)
ZFCOMPARER_DEFAULT_DECLARE_BEGIN(const zfchar *, e0, const zfchar *, e1)
{
    const zfchar *e0Tmp = ((e0 == zfnull) ? zfText("") : e0);
    const zfchar *e1Tmp = ((e1 == zfnull) ? zfText("") : e1);
    zfint result = zfscmp(e0Tmp, e1Tmp);
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
}
ZFCOMPARER_DEFAULT_DECLARE_END(const zfchar *, e0, const zfchar *, e1)
ZFCOMPARER_DEFAULT_DECLARE_BEGIN(zfstring, e0, zfstring, e1)
{
    zfint result = e0.compare(e1);
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
}
ZFCOMPARER_DEFAULT_DECLARE_END(zfstring, e0, zfstring, e1)

ZFCOMPARER_DEFAULT_DECLARE_BEGIN(ZFCompareResult, e0, ZFCompareResult, e1)
{
    return ((e0 == e1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCOMPARER_DEFAULT_DECLARE_END(ZFCompareResult, e0, ZFCompareResult, e1)
ZFCOMPARER_DEFAULT_DECLARE_BEGIN(ZFSeekPos, e0, ZFSeekPos, e1)
{
    return ((e0 == e1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCOMPARER_DEFAULT_DECLARE_END(ZFSeekPos, e0, ZFSeekPos, e1)
ZFCOMPARER_DEFAULT_DECLARE_BEGIN(zfindexRange, e0, zfindexRange, e1)
{
    return ((e0 == e1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCOMPARER_DEFAULT_DECLARE_END(zfindexRange, e0, zfindexRange, e1)

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

