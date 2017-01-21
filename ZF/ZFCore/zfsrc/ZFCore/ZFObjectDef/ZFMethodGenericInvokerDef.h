/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFMethodGenericInvokerDef.h
 * @brief generic invoker declare for #ZFMethod::methodGenericInvoker
 */

#ifndef _ZFI_ZFMethodGenericInvokerDef_h_
#define _ZFI_ZFMethodGenericInvokerDef_h_

#include "ZFObjectClassTypeFwd.h"
#include "zfautoObjectFwd.h"
#include "ZFPropertyTypeFwdDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFMethod;
/**
 * @brief generic invoker for advanced reflection, see #ZFMethod::methodGenericInvoker
 */
typedef zfbool (*ZFMethodGenericInvoker)(ZF_IN const ZFMethod *ownerMethod,
                                         ZF_IN ZFObject *ownerObjOrNullForStaticFunc,
                                         ZF_OUT_OPT zfautoObject &ret,
                                         ZF_IN_OPT ZFObject *param0,
                                         ZF_IN_OPT ZFObject *param1,
                                         ZF_IN_OPT ZFObject *param2,
                                         ZF_IN_OPT ZFObject *param3);

// ============================================================
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(ParamType, param, N) \
    typedef ParamType _ParamType##N; \
    typedef zftTraitsType<ParamType>::TraitsRemoveReference _ParamTypeTraits##N; \
    _ParamTypeTraits##N _param##N;
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_EXPAND(ParamType, param, N) \
    if(!ZFPropertyTypeIdData<_ParamTypeTraits##N>::propertyConvertFromZFObject(_param##N, param)) \
    { \
        return zffalse; \
    }

// ============================================================
template<typename T_Invoker, typename T_ReturnType>
zfclassNotPOD _ZFP_ZFMethodGenericInvokerWrapper
{
public:
    static zfbool wrap(ZF_IN const ZFMethod *ownerMethod,
                       ZF_IN ZFObject *ownerObj,
                       ZF_OUT_OPT zfautoObject &ret,
                       ZF_IN ZFObject *param0,
                       ZF_IN ZFObject *param1,
                       ZF_IN ZFObject *param2,
                       ZF_IN ZFObject *param3)
    {
        T_Invoker invoker;
        if(!invoker.paramPrepare(param0, param1, param2, param3)) {return zffalse;}
        T_ReturnType retTmp = invoker.invoke(ownerMethod, ownerObj);
        return ZFPropertyTypeIdData<typename zftTraitsType<T_ReturnType>::TraitsRemoveReference>::propertyConvertToZFObject(ret, retTmp);
    }
};
template<typename T_Invoker>
zfclassNotPOD _ZFP_ZFMethodGenericInvokerWrapper<T_Invoker, void>
{
public:
    static zfbool wrap(ZF_IN const ZFMethod *ownerMethod,
                       ZF_IN ZFObject *ownerObj,
                       ZF_OUT_OPT zfautoObject &ret,
                       ZF_IN ZFObject *param0,
                       ZF_IN ZFObject *param1,
                       ZF_IN ZFObject *param2,
                       ZF_IN ZFObject *param3)
    {
        T_Invoker invoker;
        if(!invoker.paramPrepare(param0, param1, param2, param3)) {return zffalse;}
        invoker.invoke(ownerMethod, ownerObj);
        return zftrue;
    }
};

// ============================================================
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_DECLARE( \
    ReturnType, methodSig \
    , ParamExpandOrEmpty0, ParamType0, param0 \
    , ParamExpandOrEmpty1, ParamType1, param1 \
    , ParamExpandOrEmpty2, ParamType2, param2 \
    , ParamExpandOrEmpty3, ParamType3, param3 \
    ) \
    public: \
    zfclassNotPOD _ZFP_ZFMethodGenericInvoker_##methodSig \
    { \
    private: \
         ParamExpandOrEmpty0(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(ParamType0, param0, 0)) \
         ParamExpandOrEmpty1(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(ParamType1, param1, 1)) \
         ParamExpandOrEmpty2(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(ParamType2, param2, 2)) \
         ParamExpandOrEmpty3(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(ParamType3, param3, 3)) \
    public: \
        zfbool paramPrepare(ZF_IN ZFObject *param0, \
                            ZF_IN ZFObject *param1, \
                            ZF_IN ZFObject *param2, \
                            ZF_IN ZFObject *param3) \
        { \
            ParamExpandOrEmpty0(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_EXPAND(ParamType0, param0, 0)) \
            ParamExpandOrEmpty1(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_EXPAND(ParamType1, param1, 1)) \
            ParamExpandOrEmpty2(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_EXPAND(ParamType2, param2, 2)) \
            ParamExpandOrEmpty3(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_EXPAND(ParamType3, param3, 3)) \
            return zftrue; \
        } \
        inline ReturnType invoke(ZF_IN const ZFMethod *ownerMethod, \
                                 ZF_IN ZFObject *ownerObj) \
        { \
            return ownerMethod->execute<ReturnType \
                    ParamExpandOrEmpty0(ZFM_COMMA() ParamType0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                >(ownerObj \
                    ParamExpandOrEmpty0(ZFM_COMMA() _param0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() _param1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() _param2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() _param3) \
                ); \
        } \
    };
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(ReturnType, methodSig) \
    _ZFP_ZFMethodGenericInvokerWrapper<_ZFP_ZFMethodGenericInvoker_##methodSig, ReturnType>::wrap

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodGenericInvokerDef_h_

