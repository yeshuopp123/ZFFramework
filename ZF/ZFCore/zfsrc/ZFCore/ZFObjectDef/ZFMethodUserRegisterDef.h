/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFMethodUserRegisterDef.h
 * @brief user registered ZFMethod
 */

#ifndef _ZFI_ZFMethodUserRegisterDef_h_
#define _ZFI_ZFMethodUserRegisterDef_h_

#include "ZFMethodDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFMethodUserRegister(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamExpandOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamExpandOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamExpandOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamExpandOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    const ZFMethod *resultMethod = zfnull; \
    { \
        zfclassNotPOD _ZFP_ZFMethodUserRegisterHolder \
        { \
        public: \
            const ZFClass *_ownerClass; \
            zfstring _methodName; \
            zfstring _methodId; \
            zfstring _methodInternalId; \
        public: \
            _ZFP_ZFMethodUserRegisterHolder(ZF_IN const ZFClass *ownerClass_, \
                                            ZF_IN const zfchar *methodName_, \
                                            ZF_IN const zfchar *methodId_) \
            : _ownerClass(ownerClass_) \
            , _methodName(methodName_) \
            , _methodId(methodId_) \
            , _methodInternalId() \
            { \
                zfCoreAssert(_ownerClass != zfnull); \
                zfCoreAssert(!_methodName.isEmpty()); \
                zfstringAppend(_methodInternalId, zfText("%s::%s_%s"), \
                    _ownerClass->className(), \
                    _methodName.cString(), \
                    _methodId.cString() \
                ); \
            } \
        public: \
            typedef ReturnType (*MethodTypeChecker)(const ZFMethod *, ZFObject * \
                    ParamExpandOrEmpty0(ZFM_COMMA() ParamType0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                ); \
            _ZFP_ZFMETHOD_GENERIC_INVOKER_DECLARE( \
                ReturnType, _ \
                , ParamExpandOrEmpty0, ParamType0, param0 \
                , ParamExpandOrEmpty1, ParamType1, param1 \
                , ParamExpandOrEmpty2, ParamType2, param2 \
                , ParamExpandOrEmpty3, ParamType3, param3 \
                ) \
            const ZFMethod *methodRegister(void) \
            { \
                ZFCoreMutexLocker(); \
                _ZFP_ZFMethodUserRegisterHolder::MethodTypeChecker fn = funcAddr; \
                zfCoreAssert(fn != zfnull); \
                \
                ZFMethod *_method = _ZFP_ZFMethodInstanceFind(_methodInternalId); \
                zfCoreAssertWithMessageTrim(_method == zfnull, \
                    zfTextA("[ZFMethodUserRegister] registering a method that already registered, class: %s, methodName: %s, methodId: %s"), \
                    zfsCoreZ2A(_ownerClass->className()), \
                    zfsCoreZ2A(_methodName.cString()), \
                    zfsCoreZ2A(_methodId.cString())); \
                _method = _ZFP_ZFMethodInstanceAccess(_methodInternalId); \
                \
                if(_method->_ZFP_ZFMethodNeedInit) \
                { \
                    _method->_ZFP_ZFMethodNeedInit = zffalse; \
                    _method->_ZFP_ZFMethod_init( \
                        zftrue, \
                        ZFCastReinterpret(ZFFuncAddrType, fn), \
                        _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(ReturnType, _), \
                        _methodName, \
                        _methodId, \
                        zfText(#ReturnType), \
                        "" ParamExpandOrEmpty0(#ParamType0), \
                        "" ParamExpandOrEmpty1(#ParamType1), \
                        "" ParamExpandOrEmpty2(#ParamType2), \
                        "" ParamExpandOrEmpty3(#ParamType3), \
                        zfnull \
                        ); \
                    _method->_ZFP_ZFMethod_initClassMemberType( \
                        _ownerClass, \
                        _ZFP_ZFMethod_initClassMemberType_privilege(PublicOrProtectedOrPrivate), \
                        _ZFP_ZFMethod_initClassMemberType_static(ZFMethodStaticOrNot), \
                        _ZFP_ZFMethod_initClassMemberType_virtual(ZFMethodVirtualOrNot), \
                        _ZFP_ZFMethod_initClassMemberType_const(ZFMethodConstOrNot) \
                        ); \
                    _ownerClass->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_methodRegister(_method); \
                } \
                return _method; \
            } \
        }; \
        const ZFMethod *_##resultMethod = _ZFP_ZFMethodUserRegisterHolder(ownerClass, methodNameString, methodIdString).methodRegister(); \
        resultMethod = _##resultMethod; \
    } \
    ZFUNUSED(resultMethod)
#define _ZFP_ZFMETHOD_USER_REGISTER(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamExpandOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamExpandOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamExpandOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamExpandOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(_ZFP_ZFMethodUserRegisterAutoRegister_##registerSig, ZFLevelZFFrameworkNormal) \
    { \
        ZFMethodUserRegister(result, funcAddr, ownerClass, \
            PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
            ReturnType, methodNameString, methodIdString \
            , ParamExpandOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
            , ParamExpandOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
            , ParamExpandOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
            , ParamExpandOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        ); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(_ZFP_ZFMethodUserRegisterAutoRegister_##registerSig) \
    { \
        ZFMethodUserUnregister(ownerClass, methodNameString, methodIdString); \
    } \
    ZF_GLOBAL_INITIALIZER_END(_ZFP_ZFMethodUserRegisterAutoRegister_##registerSig)

// ============================================================
/**
 * @brief register a custom method to existing class, for advanced reflection use only
 *
 * example:
 * @code
 *   // declare your method invoker
 *   static ReturnType myInvoker(const ZFMethod *method, ZFObject *obj, ParamType0 param0, ParamType1 param0)
 *   {
 *      // ...
 *   }
 *
 *   // register it, using ZF_GLOBAL_INITIALIZER_INIT is recommended
 *   ZF_GLOBAL_INITIALIZER_INIT(MyMethodRegister)
 *   {
 *       ZFMethodUserRegister_2(
 *           resultMethod, myInvoker, MyClassToAttachTheMethod::ClassData(),
 *           public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst,
 *           ReturnType, zfText("methodName"), zfnull
 *           , ParamType0, param0, ZFMethodNoDefaultParam
 *           , ParamType1, param1, ZFMethodNoDefaultParam);
 *       zfLogTrimT() << resultMethod;
 *   }
 *   ZF_GLOBAL_INITIALIZER_DESTROY(MyMethodRegister)
 *   {
 *       ZFMethodUserUnregister(MyClassToAttachTheMethod::ClassData(), zfText("methodName"), zfnull);
 *   }
 *   ZF_GLOBAL_INITIALIZER_END(MyMethodRegister)
 *
 *   // or, you may use this macro for short, at cpp files only
 *   ZFMETHOD_USER_REGISTER_2(
 *       myInvoker, myInvoker, MyClassToAttachTheMethod::ClassData(),
 *       public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst,
 *       ReturnType, zfText("methodName"), zfnull
 *       , ParamType0, param0, ZFMethodNoDefaultParam
 *       , ParamType1, param1, ZFMethodNoDefaultParam)
 * @endcode
 *
 * note:
 * -  if there is already a method exist with same method name and method id,
 *   register would fail
 * -  the registered method must be unregister manually at proper time,
 *   use #ZF_GLOBAL_INITIALIZER_INIT is recommeded
 * -  once registered, the method can be reflected by #ZFClass::methodForName,
 *   but there's no way to invoke the method by object directly,
 *   it can only be invoked by #ZFMethod::execute
 */
#define ZFMethodUserRegister(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamExpandOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamExpandOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamExpandOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamExpandOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamExpandOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamExpandOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamExpandOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamExpandOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    )
/** @brief see #ZFMethodUserRegister */
#define ZFMETHOD_USER_REGISTER(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamExpandOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamExpandOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamExpandOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamExpandOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamExpandOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamExpandOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamExpandOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamExpandOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    )

// ============================================================
/** @brief see #ZFMethodUserRegister */
#define ZFMethodUserRegister_0(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
    ) \
    ZFMethodUserRegister(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ZFM_EMPTY, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EMPTY, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EMPTY, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EMPTY, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    )
/** @brief see #ZFMethodUserRegister */
#define ZFMETHOD_USER_REGISTER_0(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
    ) \
    ZFMETHOD_USER_REGISTER(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ZFM_EMPTY, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EMPTY, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EMPTY, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EMPTY, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    )
/** @brief see #ZFMethodUserRegister */
#define ZFMethodUserRegister_1(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    ) \
    ZFMethodUserRegister(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ZFM_EXPAND, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EMPTY,  ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EMPTY,  ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EMPTY,  ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    )
/** @brief see #ZFMethodUserRegister */
#define ZFMETHOD_USER_REGISTER_1(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    ) \
    ZFMETHOD_USER_REGISTER(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ZFM_EXPAND, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EMPTY,  ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EMPTY,  ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EMPTY,  ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    )
/** @brief see #ZFMethodUserRegister */
#define ZFMethodUserRegister_2(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    ) \
    ZFMethodUserRegister(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ZFM_EXPAND, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EXPAND, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EMPTY,  ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EMPTY,  ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    )
/** @brief see #ZFMethodUserRegister */
#define ZFMETHOD_USER_REGISTER_2(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    ) \
    ZFMETHOD_USER_REGISTER(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ZFM_EXPAND, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EXPAND, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EMPTY,  ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EMPTY,  ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    )
/** @brief see #ZFMethodUserRegister */
#define ZFMethodUserRegister_3(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    ) \
    ZFMethodUserRegister(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ZFM_EXPAND, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EXPAND, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EXPAND, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EMPTY,  ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    )
/** @brief see #ZFMethodUserRegister */
#define ZFMETHOD_USER_REGISTER_3(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    ) \
    ZFMETHOD_USER_REGISTER(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ZFM_EXPAND, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EXPAND, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EXPAND, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EMPTY,  ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    )
/** @brief see #ZFMethodUserRegister */
#define ZFMethodUserRegister_4(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    ZFMethodUserRegister(resultMethod, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ZFM_EXPAND, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EXPAND, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EXPAND, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EXPAND, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    )
/** @brief see #ZFMethodUserRegister */
#define ZFMETHOD_USER_REGISTER_4(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    ZFMETHOD_USER_REGISTER(registerSig, funcAddr, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, methodNameString, methodIdString \
        , ZFM_EXPAND, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFM_EXPAND, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFM_EXPAND, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFM_EXPAND, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    )

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_ZFMethodUserUnregister(ZF_IN const ZFClass *cls,
                                                      ZF_IN const zfchar *methodName,
                                                      ZF_IN const zfchar *methodId);
/** @brief see #ZFMethodUserRegister */
#define ZFMethodUserUnregister(ownerClass, methodNameString, methodIdString) \
    _ZFP_ZFMethodUserUnregister(ownerClass, methodNameString, methodIdString)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodUserRegisterDef_h_

