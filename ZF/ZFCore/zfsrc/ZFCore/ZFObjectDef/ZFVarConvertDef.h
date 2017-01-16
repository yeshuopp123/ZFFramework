/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFVarConvertDef.h
 * @brief util to convert types from and to ZFObject,
 *   for advanced reflection
 */

#ifndef _ZFI_ZFVarConvertDef_h_
#define _ZFI_ZFVarConvertDef_h_

#include "ZFObjectClassTypeFwd.h"
#include "zfautoObjectFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFVarConvertType_default {};
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFVarConvertType_ZFObject {};

template<typename T_Var, typename T_TypeFix, int T_SpecLoop = 0>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFVarConvertHolder
{
public:
    static zfbool convertFrom(ZF_OUT T_Var &v, ZF_IN ZFObject *obj)
    {
        return _ZFP_ZFVarConvertHolder<T_Var, T_TypeFix, T_SpecLoop + 1>::convertFrom(v, obj);
    }
    static zfbool convertTo(ZF_OUT zfautoObject &obj, ZF_IN T_Var const &v)
    {
        return _ZFP_ZFVarConvertHolder<T_Var, T_TypeFix, T_SpecLoop + 1>::convertTo(obj, v);
    }
    static zfbool convertAvailable(void)
    {
        return _ZFP_ZFVarConvertHolder<T_Var, T_TypeFix, T_SpecLoop + 1>::convertAvailable();
    }
};
template<typename T_Var, typename T_TypeFix>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFVarConvertHolder<T_Var, T_TypeFix, zftCounterMax>
{
public:
    static zfbool convertFrom(ZF_OUT T_Var &v, ZF_IN ZFObject *obj)
    {
        return zffalse;
    }
    static zfbool convertTo(ZF_OUT zfautoObject &obj, ZF_IN T_Var const &v)
    {
        return zffalse;
    }
    static zfbool convertAvailable(void)
    {
        return zffalse;
    }
};

/** @brief see #ZFVarConvertFromZFObject */
#define ZFVAR_CONVERT_DECLARE(Type, convertFromAction, convertToAction) \
    zftCounterInc(Type) \
    template<> \
    zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFVarConvertHolder<Type, _ZFP_ZFVarConvertType_default, zftCounterGet(Type)> \
    { \
    public: \
        static zfbool convertFrom(ZF_OUT Type &v, ZF_IN ZFObject *obj) \
        { \
            convertFromAction \
            return zftrue; \
        } \
        static zfbool convertTo(ZF_OUT zfautoObject &obj, ZF_IN Type const &v) \
        { \
            convertToAction \
            return zftrue; \
        } \
        static zfbool convertAvailable(void) \
        { \
            return zftrue; \
        } \
    };
/** @brief see #ZFVarConvertFromZFObject */
#define ZFVAR_CONVERT_DECLARE_TEMPLATE(typenameList, Type, convertFromAction, convertToAction) \
    zftCounterInc(Type) \
    template<typenameList> \
    zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFVarConvertHolder<Type, _ZFP_ZFVarConvertType_default, zftCounterGet(Type)> \
    { \
    public: \
        static zfbool convertFrom(ZF_OUT Type &v, ZF_IN ZFObject *obj) \
        { \
            convertFromAction \
            return zftrue; \
        } \
        static zfbool convertTo(ZF_OUT zfautoObject &obj, ZF_IN Type const &v) \
        { \
            convertToAction \
            return zftrue; \
        } \
        static zfbool convertAvailable(void) \
        { \
            return zftrue; \
        } \
    };

extern ZF_ENV_EXPORT void _ZFP_ZFVarConvertHolderFwdFix(ZF_OUT zfautoObject &obj, ZF_IN ZFObject *v);
template<typename T_Var>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFVarConvertHolder<T_Var, _ZFP_ZFVarConvertType_ZFObject, zftCounterMax>
{
public:
    static zfbool convertFrom(ZF_OUT T_Var &v, ZF_IN ZFObject *obj)
    {
        v = ZFCastZFObject(T_Var, obj);
        return ((obj == zfnull) == (v == zfnull));
    }
    static zfbool convertTo(ZF_OUT zfautoObject &obj, ZF_IN T_Var const &v)
    {
        _ZFP_ZFVarConvertHolderFwdFix(obj, ZFCastZFObject(ZFObject *, v));
        return ((v == zfnull) == (obj.toObject() == zfnull));
    }
    static zfbool convertAvailable(void)
    {
        return zftrue;
    }
};

template<typename T_Var, typename T_TypeFix = void>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFVarConvertTypeFix
{
public:
    typedef _ZFP_ZFVarConvertType_default Type;
};
template<typename T_Var>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFVarConvertTypeFix<T_Var,
        typename zftEnableIf<(zftTypeIsTypeOf<T_Var, ZFObject>::TypeIsTypeOf || zftTypeIsTypeOf<T_Var, ZFInterface>::TypeIsTypeOf)>::EnableIf
    >
{
public:
    typedef _ZFP_ZFVarConvertType_ZFObject Type;
};

/**
 * @brief util to convert types from and to ZFObject,
 *   for advanced reflection
 *
 * to use, you must first ensure #ZFVAR_CONVERT_DECLARE has been declared before using this macro,
 * example:
 * @code
 *   ZFVAR_CONVERT_DECLARE(MyType, {
 *           v = myConvertFrom(obj);
 *       }, {
 *           obj = myConvertTo(v);
 *       })
 *
 *   // proto type for the conversion method:
 *   //   static zfbool convertFrom(ZF_OUT T_Var &v, ZF_IN ZFObject *obj);
 *   //   static zfbool convertTo(ZF_OUT zfautoObject &obj, ZF_IN T_Var const &v);
 *
 *   // or, for template types:
 *   ZFVAR_CONVERT_DECLARE_TEMPLATE(
 *       typename T0 ZFM_COMMA() typename T1,
 *       MyType<T0 ZFM_COMMA() T1>,
 *       {
 *           v = myConvertFrom(obj);
 *       }, {
 *           obj = myConvertTo(v);
 *       })
 * @endcode
 * \n
 * once declared, you may use it to convert types:
 * @code
 *   MyType var;
 *   ZFObject *obj;
 *   zfautoObject objHolder;
 *   zfbool success;
 *
 *   success = ZFVarConvertFromZFObject(MyType, var, obj);
 *   success = ZFVarConvertToZFObject(MyType, objHolder, var);
 *
 *   // or, for template types:
 *   success = ZFVarConvertFromZFObject(MyType<T0 ZFM_COMMA() T1>, var, obj);
 *   success = ZFVarConvertToZFObject(MyType<T0 ZFM_COMMA() T1>, objHolder, var);
 * @endcode
 * \n
 * REMARKS:\n
 * this is usefull when combined with templates,
 * any type can be converted from and to ZFObject types without knowing its actual type\n
 * currently, it's used by #ZFMethod::methodGenericInvoker,
 * to achive automatic script binding by reflection
 */
#define ZFVarConvertFromZFObject(T_Var, v, obj) \
    _ZFP_ZFVarConvertHolder< \
            typename zftTraitsType<T_Var>::TraitsRemoveReference, \
            typename _ZFP_ZFVarConvertTypeFix<typename zftTraitsType<T_Var>::TraitsType>::Type \
        >::convertFrom(v, obj)
/** @brief see #ZFVarConvertFromZFObject */
#define ZFVarConvertToZFObject(T_Var, obj, v) \
    _ZFP_ZFVarConvertHolder< \
            typename zftTraitsType<T_Var>::TraitsRemoveReference, \
            typename _ZFP_ZFVarConvertTypeFix<typename zftTraitsType<T_Var>::TraitsType>::Type \
        >::convertTo(obj, v)
/** @brief see #ZFVarConvertFromZFObject */
#define ZFVarConvertAvailable(T_Var) \
    _ZFP_ZFVarConvertHolder< \
            typename zftTraitsType<T_Var>::TraitsRemoveReference, \
            typename _ZFP_ZFVarConvertTypeFix<typename zftTraitsType<T_Var>::TraitsType>::Type \
        >::convertAvailable()

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFVarConvertDef_h_

