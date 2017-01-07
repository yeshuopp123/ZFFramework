/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCallbackDef_template.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFCallbackDef_template_h_
#define _ZFI_ZFCallbackDef_template_h_

#include "ZFCallbackDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief template version of #ZFCallback */
template<typename T_Return, typename T_Param0 = void, typename T_Param1 = void, typename T_Param2 = void, typename T_Param3 = void, typename T_Param4 = void>
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT : zfextendsLikePOD ZFCallback {};

/** @cond ZFPrivateDoc */
template<typename T_Return, typename T_Param0, typename T_Param1, typename T_Param2, typename T_Param3>
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return, T_Param0, T_Param1, T_Param2, T_Param3, void> : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(ZF_IN T_Param0 param0,
                            ZF_IN T_Param1 param1,
                            ZF_IN T_Param2 param2,
                            ZF_IN T_Param3 param3) const
    {
        return ZFCallback::executeExact<T_Return, T_Param0, T_Param1, T_Param2, T_Param3>(param0, param1, param2, param3);
    }
    inline T_Return operator()(ZF_IN T_Param0 param0,
                               ZF_IN T_Param1 param1,
                               ZF_IN T_Param2 param2,
                               ZF_IN T_Param3 param3) const
    {
        return ZFCallback::executeExact<T_Return, T_Param0, T_Param1, T_Param2, T_Param3>(param0, param1, param2, param3);
    }
};

template<typename T_Return, typename T_Param0, typename T_Param1, typename T_Param2>
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return, T_Param0, T_Param1, T_Param2, void> : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(ZF_IN T_Param0 param0,
                            ZF_IN T_Param1 param1,
                            ZF_IN T_Param2 param2) const
    {
        return ZFCallback::executeExact<T_Return, T_Param0, T_Param1, T_Param2>(param0, param1, param2);
    }
    inline T_Return operator()(ZF_IN T_Param0 param0,
                               ZF_IN T_Param1 param1,
                               ZF_IN T_Param2 param2) const
    {
        return ZFCallback::executeExact<T_Return, T_Param0, T_Param1, T_Param2>(param0, param1, param2);
    }
};

template<typename T_Return, typename T_Param0, typename T_Param1>
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return, T_Param0, T_Param1, void, void> : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(ZF_IN T_Param0 param0,
                            ZF_IN T_Param1 param1) const
    {
        return ZFCallback::executeExact<T_Return, T_Param0, T_Param1>(param0, param1);
    }
    inline T_Return operator()(ZF_IN T_Param0 param0,
                               ZF_IN T_Param1 param1) const
    {
        return ZFCallback::executeExact<T_Return, T_Param0, T_Param1>(param0, param1);
    }
};

template<typename T_Return, typename T_Param0>
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return, T_Param0, void, void, void> : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(ZF_IN T_Param0 param0) const
    {
        return ZFCallback::executeExact<T_Return, T_Param0>(param0);
    }
    inline T_Return operator()(ZF_IN T_Param0 param0) const
    {
        return ZFCallback::executeExact<T_Return, T_Param0>(param0);
    }
};

template<typename T_Return>
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return, void, void, void, void> : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(void) const
    {
        return ZFCallback::executeExact<T_Return>();
    }
    inline T_Return operator()(void) const
    {
        return ZFCallback::executeExact<T_Return>();
    }
};
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallbackDef_template_h_

