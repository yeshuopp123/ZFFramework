/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFMethodDeclareFuncDef.h
 * @brief ZFMethod declaration
 */

#ifndef _ZFI_ZFMethodDeclareFuncDef_h_
#define _ZFI_ZFMethodDeclareFuncDef_h_

#include "ZFMethodDef.h"
#include "ZFVarConvertDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util method to find ZFMethod, return null if method not registered
 */
extern ZF_ENV_EXPORT const ZFMethod *ZFMethodFuncGet(ZF_IN const zfchar *methodNamespace,
                                                     ZF_IN const zfchar *methodName,
                                                     ZF_IN_OPT const zfchar *methodId = zfnull);

// ============================================================
#define _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName, ZFMethodIdOrNoId) \
    (MethodNamespace::_ZFP_ZFMethodFunc_##MethodName##_##ZFMethodIdOrNoId())
/** @brief see #ZFMethod */
#define ZFMethodFuncAccess(MethodNamespace, MethodName) \
    ZFMethodFuncAccessDetail(MethodNamespace, MethodName, ZFMethodNoId)
/** @brief see #ZFMethod */
#define ZFMethodFuncAccessDetail(MethodNamespace, MethodName, ZFMethodIdOrNoId) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName, ZFMethodIdOrNoId)

// ============================================================
extern ZF_ENV_EXPORT zfstring _ZFP_ZFMethodFuncSig(ZF_IN const zfchar *methodNamespace,
                                                   ZF_IN const zfchar *methodName,
                                                   ZF_IN_OPT const zfchar *methodId = zfnull);

// ============================================================
#define _ZFP_ZFMETHOD_FUNC_inlinePrefix_isInline inline
#define _ZFP_ZFMETHOD_FUNC_inlinePrefix_notInline extern ZF_ENV_EXPORT
#define _ZFP_ZFMETHOD_FUNC_inlinePrefix(inlineOrNot) _ZFP_ZFMETHOD_FUNC_inlinePrefix_##inlineOrNot

#define _ZFP_ZFMETHOD_FUNC_inlinePostfix_isInline
#define _ZFP_ZFMETHOD_FUNC_inlinePostfix_notInline ;
#define _ZFP_ZFMETHOD_FUNC_inlinePostfix(inlineOrNot) _ZFP_ZFMETHOD_FUNC_inlinePostfix_##inlineOrNot

#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix_isInline ;
#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix_notInline
#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix(inlineOrNot) _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix_##inlineOrNot

#define _ZFP_ZFMETHOD_FUNC_DECLARE( \
    MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamExpandOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamExpandOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamExpandOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamExpandOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE_( \
        MethodNamespace, ZFMethodInlineOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ParamExpandOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamExpandOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamExpandOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamExpandOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
#define _ZFP_ZFMETHOD_FUNC_DECLARE_( \
    MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamExpandOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamExpandOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamExpandOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamExpandOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    /** @cond ZFPrivateDoc */ \
    _ZFP_ZFMETHOD_FUNC_inlinePrefix(ZFMethodInlineOrNot) ReturnType MethodName( \
        ParamExpandOrEmpty0(            ParamType0 param0 ZFMethodNoDefaultParamOrDefaultParam0) \
        ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1 ZFMethodNoDefaultParamOrDefaultParam1) \
        ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2 ZFMethodNoDefaultParamOrDefaultParam2) \
        ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3 ZFMethodNoDefaultParamOrDefaultParam3) \
        ); \
    zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFMethodFuncHolder_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId \
    { \
    public: \
        _ZFP_ZFMETHOD_GENERIC_INVOKER_DECLARE( \
            ReturnType, genericInvoker \
            , ParamExpandOrEmpty0, ParamType0, param0 \
            , ParamExpandOrEmpty1, ParamType1, param1 \
            , ParamExpandOrEmpty2, ParamType2, param2 \
            , ParamExpandOrEmpty3, ParamType3, param3 \
            ) \
        static const ZFMethod *methodInstance(void) \
        { \
            static _ZFP_ZFMethodInstanceHolder _methodHolder(_ZFP_ZFMethodFuncSig( \
                    ZFM_TOSTRING(MethodNamespace), \
                    ZFM_TOSTRING(MethodName), \
                    ZFM_TOSTRING(ZFMethodIdOrNoId) \
                )); \
            ZFMethod *_method = _methodHolder.method; \
            if(_method->_ZFP_ZFMethodNeedInit) \
            { \
                _method->_ZFP_ZFMethodNeedInit = zffalse; \
                _method->_ZFP_ZFMethod_init( \
                    zffalse, \
                    ZFCastReinterpret(ZFFuncAddrType, \
                        &_ZFP_ZFMethodFuncHolder_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId::methodInvoker), \
                    _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(ReturnType, genericInvoker), \
                    zfText(#MethodName), \
                    zfText(#ZFMethodIdOrNoId), \
                    zfText(#ReturnType), \
                    "" ParamExpandOrEmpty0(#ParamType0), \
                    "" ParamExpandOrEmpty1(#ParamType1), \
                    "" ParamExpandOrEmpty2(#ParamType2), \
                    "" ParamExpandOrEmpty3(#ParamType3), \
                    zfnull \
                    ); \
                _method->_ZFP_ZFMethod_initFuncType( \
                    ZFM_TOSTRING(MethodNamespace) \
                    ); \
            } \
            return _method; \
        } \
        static const ZFMethod *methodAccess(void) \
        { \
            static const ZFMethod *_method = _ZFP_ZFMethodFuncHolder_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId::methodInstance(); \
            return _method; \
        } \
        static inline ReturnType methodInvoker( \
            ZF_IN const ZFMethod *method, \
            ZF_IN ZFObject *obj \
            ParamExpandOrEmpty0(ZFM_COMMA() ParamType0 param0) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
            ) \
        { \
            return MethodNamespace::MethodName( \
                ParamExpandOrEmpty0(            param0) \
                ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                ); \
        } \
    }; \
    /** @endcond */ \
    _ZFP_ZFMETHOD_FUNC_inlinePrefix(ZFMethodInlineOrNot) ReturnType MethodName( \
        ParamExpandOrEmpty0(            ParamType0 param0 ZFMethodNoDefaultParamOrDefaultParam0) \
        ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1 ZFMethodNoDefaultParamOrDefaultParam1) \
        ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2 ZFMethodNoDefaultParamOrDefaultParam2) \
        ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3 ZFMethodNoDefaultParamOrDefaultParam3) \
        ) _ZFP_ZFMETHOD_FUNC_inlinePostfix(ZFMethodInlineOrNot)

#define _ZFP_ZFMETHOD_FUNC_DEFINE(MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamExpandOrEmpty0, ParamType0, param0 \
    , ParamExpandOrEmpty1, ParamType1, param1 \
    , ParamExpandOrEmpty2, ParamType2, param2 \
    , ParamExpandOrEmpty3, ParamType3, param3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE_(MethodNamespace, ZFMethodInlineOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ParamExpandOrEmpty0, ParamType0, param0 \
        , ParamExpandOrEmpty1, ParamType1, param1 \
        , ParamExpandOrEmpty2, ParamType2, param2 \
        , ParamExpandOrEmpty3, ParamType3, param3 \
        )
#define _ZFP_ZFMETHOD_FUNC_DEFINE_(MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamExpandOrEmpty0, ParamType0, param0 \
    , ParamExpandOrEmpty1, ParamType1, param1 \
    , ParamExpandOrEmpty2, ParamType2, param2 \
    , ParamExpandOrEmpty3, ParamType3, param3 \
    ) \
    ZF_STATIC_INITIALIZER_INIT(_ZFP_ZFMethodFuncRegister_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId) \
    { \
        _ZFP_ZFMethodFuncHolder_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId::methodAccess(); \
    } \
    ZF_STATIC_INITIALIZER_END(_ZFP_ZFMethodFuncRegister_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId) \
    ReturnType MethodName( \
        ParamExpandOrEmpty0(            ParamType0 param0) \
        ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
        ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
        ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
        ) _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix(ZFMethodInlineOrNot)

// ============================================================
// 0 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_FUNC_DECLARE_0(MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        MethodNamespace, ZFMethodInlineOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFM_EMPTY, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EMPTY, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EMPTY, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EMPTY, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_FUNC_DEFINE_0(MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        MethodNamespace, ZFMethodInlineOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFM_EMPTY, ParamType0, param0 \
        , ZFM_EMPTY, ParamType1, param1 \
        , ZFM_EMPTY, ParamType2, param2 \
        , ZFM_EMPTY, ParamType3, param3 \
        )

// ============================================================
// 1 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_FUNC_DECLARE_1(MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        MethodNamespace, ZFMethodInlineOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFM_EXPAND, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EMPTY, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EMPTY, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EMPTY, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_FUNC_DEFINE_1(MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        MethodNamespace, ZFMethodInlineOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFM_EXPAND, ParamType0, param0 \
        , ZFM_EMPTY, ParamType1, param1 \
        , ZFM_EMPTY, ParamType2, param2 \
        , ZFM_EMPTY, ParamType3, param3 \
        )

// ============================================================
// 2 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_FUNC_DECLARE_2(MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        MethodNamespace, ZFMethodInlineOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFM_EXPAND, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EXPAND, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EMPTY, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EMPTY, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_FUNC_DEFINE_2(MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0 \
    , ParamType1, param1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        MethodNamespace, ZFMethodInlineOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFM_EXPAND, ParamType0, param0 \
        , ZFM_EXPAND, ParamType1, param1 \
        , ZFM_EMPTY, ParamType2, param2 \
        , ZFM_EMPTY, ParamType3, param3 \
        )

// ============================================================
// 3 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_FUNC_DECLARE_3(MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        MethodNamespace, ZFMethodInlineOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFM_EXPAND, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EXPAND, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EXPAND, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EMPTY, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_FUNC_DEFINE_3(MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        MethodNamespace, ZFMethodInlineOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFM_EXPAND, ParamType0, param0 \
        , ZFM_EXPAND, ParamType1, param1 \
        , ZFM_EXPAND, ParamType2, param2 \
        , ZFM_EMPTY, ParamType3, param3 \
        )

// ============================================================
// 4 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_FUNC_DECLARE_4(MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        MethodNamespace, ZFMethodInlineOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFM_EXPAND, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EXPAND, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EXPAND, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EXPAND, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_FUNC_DEFINE_4(MethodNamespace, ZFMethodInlineOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    , ParamType3, param3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        MethodNamespace, ZFMethodInlineOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFM_EXPAND, ParamType0, param0 \
        , ZFM_EXPAND, ParamType1, param1 \
        , ZFM_EXPAND, ParamType2, param2 \
        , ZFM_EXPAND, ParamType3, param3 \
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodDeclareFuncDef_h_

