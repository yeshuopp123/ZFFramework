/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFMethodDef.h
 * @brief reflectable method definination for ZFFramework
 */

#ifndef _ZFI_ZFMethodDef_h_
#define _ZFI_ZFMethodDef_h_

#include "ZFObjectClassTypeFwd.h"
#include "ZFMethodGenericInvokerDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief access type for ZFMethod
 */
typedef enum
{
    ZFMethodPrivilegeTypePublic, /**< @brief public */
    ZFMethodPrivilegeTypeProtected, /**< @brief protected */
    ZFMethodPrivilegeTypePrivate, /**< @brief private */
} ZFMethodPrivilegeType;
/** @brief string tokens */
#define ZFTOKEN_ZFMethodPrivilegeTypePublic zfText("public")
/** @brief string tokens */
#define ZFTOKEN_ZFMethodPrivilegeTypeProtected zfText("protected")
/** @brief string tokens */
#define ZFTOKEN_ZFMethodPrivilegeTypePrivate zfText("private")

ZFCOMPARER_DEFAULT_DECLARE_BEGIN(ZFMethodPrivilegeType, e0, ZFMethodPrivilegeType, e1)
{
    return ((e0 == e1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCOMPARER_DEFAULT_DECLARE_END(ZFMethodPrivilegeType, e0, ZFMethodPrivilegeType, e1)

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFMethodPrivilegeType, ZFMethodPrivilegeType)

// ============================================================
zfclassFwd ZFObject;
zfclassFwd ZFClass;
#define _ZFP_ZFMETHOD_INVOKER(N) \
    /** @brief see #ZFMethod */ \
    template<typename T_ReturnType ZFM_REPEAT(N, ZFMARCO_REPEAT_TEMPLATE, ZFM_COMMA)> \
    inline T_ReturnType execute(ZFObject *obj ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA)) const \
    { \
        return ZFCastReinterpret( \
                T_ReturnType (*)(const ZFMethod *, ZFObject * ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA)), \
                this->_ZFP_ZFMethod_invoker) \
            (this, obj ZFM_REPEAT(N, ZFM_REPEAT_NAME, ZFM_COMMA)); \
    } \
    /** @brief see #ZFMethod */ \
    template<typename T_ReturnType ZFM_REPEAT(N, ZFMARCO_REPEAT_TEMPLATE, ZFM_COMMA)> \
    inline T_ReturnType executeStatic(ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_EMPTY)) const \
    { \
        return ZFCastReinterpret( \
                T_ReturnType (*)(const ZFMethod *, ZFObject * ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA)), \
                this->_ZFP_ZFMethod_invoker) \
            (this, zfnull ZFM_REPEAT(N, ZFM_REPEAT_NAME, ZFM_COMMA)); \
    }

/**
 * @brief max param supported by ZFMethod
 */
#define ZFMETHOD_MAX_PARAM 4

// ============================================================
/** @brief see #ZFMethod */
#define ZFMethodIsInline isInline
/** @brief see #ZFMethod */
#define ZFMethodNotInline notInline
/** @brief see #ZFMethod */
#define ZFMethodIsStatic isStatic
/** @brief see #ZFMethod */
#define ZFMethodNotStatic notStatic
/** @brief see #ZFMethod */
#define ZFMethodIsVirtual isVirtual
/** @brief see #ZFMethod */
#define ZFMethodNotVirtual notVirtual
/** @brief see #ZFMethod */
#define ZFMethodIsConst isConst
/** @brief see #ZFMethod */
#define ZFMethodNotConst notConst

// ============================================================
/** @brief see #ZFMethod */
#define ZFMethodNoId
/** @brief see #ZFMethod */
// use a "##" to fix this problem:
//   if id is a macro such as "#define zfstringA std::string"
#define ZFMethodId(methodId) methodId
/** @brief see #ZFMethod */
#define ZFMethodNoDefaultParam
/** @brief see #ZFMethod */
#define ZFMethodDefaultParam(DefaultValue) = (DefaultValue)

// ============================================================
/**
 * @brief reflectable method for ZFObject
 *
 * to use ZFMethod, you must first declare it in your class:
 * @code
 *   zfclass YourClass : zfextends ZFObject
 *   {
 *       ZFOBJECT_DECLARE(YourClass, ZFObject)
 *      / **
 *        * you can add doxygen docs here
 *        * /
 *       ZFMETHOD_DECLARE_0(void, yourMethod)
 *       {
 *           // method code here
 *       }
 *   };
 * @endcode
 * or you may want to declare in header and define in cpp file:
 * @code
 *   // YourClass.h
 *   zfclass YourClass : zfextends ZFObject
 *   {
 *       ZFOBJECT_DECLARE(YourClass, ZFObject)
 *       ZFMETHOD_DECLARE_0(void, yourMethod); // declare only
 *   };
 *
 *   // YourClass.cpp
 *   ZFMETHOD_DEFINE_0(YourClass, void, yourMethod)
 *   {
 *       // method code here
 *   }
 * @endcode
 * you can use any types for return value and param, including reference type,
 * while the param's num is limited to #ZFMETHOD_MAX_PARAM\n
 * you can declare overloaded method,
 * but you must specify the methodId for each of them (see bellow)\n
 * \n
 * the ZFMETHOD_DECLARE_XXX macro takes many params,
 * to walk through it, let's take a example:
 * @code
 *   ZFMETHOD_DECLARE_DETAIL_2(
 *       PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot,
 *       ReturnType, MethodName, ZFMethodIdOrNoId
 *       , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0
 *       , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1
 *       )
 * @endcode
 * -  PublicOrProtectedOrPrivate is the access type for the method,
 *   could be: public, protected, private
 * -  ZFMethodStaticOrNot indicates whether the method is static,
 *   could be: ZFMethodIsStatic, ZFMethodNotStatic
 * -  ZFMethodVirtualOrNot indicates whether the method is virtual,
 *   could be: ZFMethodIsVirtual, ZFMethodNotVirtual
 * -  ZFMethodConstOrNot indicates whether the method is const,
 *   could be: ZFMethodIsConst, ZFMethodNotConst
 * -  ReturnType is the method's return value's type,
 *   could be any types
 * -  MethodName is the method's name
 * -  ZFMethodIdOrNoId is an extra string appended to method
 *   to take care of overloaded method (which have the same method name),
 *   could be any zfcharA sequences which is valid for a function name,
 *   but it's recommended to use the param type string,
 *   such as "int_double" if the method is "void func(int, double)"\n
 *   it's only necessary if you have overloaded methods and
 *   want to reflect them from ZFClass
 * -  ParamTypeN is the type of param at index N,
 *   could be any types
 * -  paramN is the name of param at index N,
 *   could be any string valid for a variable name
 * -  ZFMethodNoDefaultParamOrDefaultParamN is the default value for the param,
 *   could be ZFMethodNoDefaultParam or ZFMethodDefaultParam(YourDefaultValue)
 *
 * to override a method declared in parent,
 * subclass should use ZFMETHOD_OVERRIDE_DECLARE_XXX/ZFMETHOD_OVERRIDE_DEFINE_XXX,
 * which is similar to ZFMETHOD_DECLARE_XXX/ZFMETHOD_DEFINE_XXX\n
 * overrided method isn't belong to subclass,
 * they still can only be found by parent's ZFClass\n
 * \n
 * once declared, the method would be automatically registered to
 * it's owner class when at least one instance created\n
 * also, similar to #ZFOBJECT_REGISTER,
 * you can add ZFMETHOD_REGISTER to your header file or cpp file to
 * statically register ZFMethod, so that you may access the method
 * even if none of instance ever created\n
 * \n
 * to access the method, use ZFMethodAccess/ZFMethodAccessDetail is recommended:
 * @code
 *   const ZFMethod *method = ZFMethodAccess(YourClassType, yourMethodName);
 * @endcode
 * also, you may use ZFClass's member function to reflect from string,
 * such as methodForName (see #ZFClass for more info)\n
 * \n
 * once you have successfully get the ZFMethod data,
 * you can execute it without knowing the owner's class type:
 * @code
 *   const ZFClass *cls = ...; // we may not know the actual class type
 *   const ZFMethod *method = cls->methodForName(zfText("someMethod"));
 *   zfautoObject objTmp = cls->newInstance();
 *   ZFObject *obj = objTmp.toObject();
 *
 *   if(!method->methodIsStatic())
 *   { // execute
 *       YourReturnType result = method->execute<YourReturnType, ParamType...>(obj, someParam...);
 *   }
 *   else
 *   { // or executeStatic if the method is static
 *       YourReturnType result = method->executeStatic<YourReturnType, ParamType...>(someParam...);
 *   }
 *
 *   // or, you may use generic version:
 *   method->execute<YourReturnType, ParamType...>(obj, someParam...);
 * @endcode
 * @warning you take the full responsibility to make sure
 *   the ReturnType and ParamType exactly match the method,
 *   if not, no compile error would occurred,
 *   but app may (or may not) crash at runtime
 *
 * \n
 * if you want to reflect overloaded method, use both methodName and methodId,
 * the methodId is defined in ZFMETHOD_DECLARE_XXX (ZFMethodIdOrNoId):
 * @code
 *   cls->methodForName(zfText("methodName"), zfText("methodId"));
 * @endcode
 *
 * @warning in subclass, you may declare a method with the same name of parent's one,
 *   but the behavior may be unexpected in some situations,
 *   so it's highly deprecated to do so
 * @note it's thread-safe to execute ZFMethod,
 *   but whether the actual run code (the code in the method body)
 *   is thread-safe, is depends on you
 */
zffinal zfclassNotPOD ZF_ENV_EXPORT ZFMethod
{
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFMethod)

public:
    zfbool _ZFP_ZFMethodNeedInit;
    void _ZFP_ZFMethod_init(ZF_IN zfbool methodIsUserRegister,
                            ZF_IN ZFFuncAddrType invoker,
                            ZF_IN ZFMethodGenericInvoker methodGenericInvoker,
                            ZF_IN const zfchar *methodName,
                            ZF_IN const zfchar *methodId,
                            ZF_IN const zfchar *returnTypeName,
                            ...);
    void _ZFP_ZFMethod_initClassMemberType(ZF_IN const ZFClass *ownerClass,
                                           ZF_IN ZFMethodPrivilegeType privilegeType,
                                           ZF_IN zfbool isStatic,
                                           ZF_IN zfbool isVirtual,
                                           ZF_IN zfbool isConst);
    void _ZFP_ZFMethod_initFuncType(ZF_IN const zfchar *methodNamespace);
    /** @cond ZFPrivateDoc */
    ZFMethod(void);
    /** @endcond */
    ~ZFMethod(void);

public:
    _ZFP_ZFMETHOD_INVOKER(0)
    _ZFP_ZFMETHOD_INVOKER(1)
    _ZFP_ZFMETHOD_INVOKER(2)
    _ZFP_ZFMETHOD_INVOKER(3)
    _ZFP_ZFMETHOD_INVOKER(4)

public:
    /**
     * @brief get a short info about the method
     *
     * may look like this: \n
     *   OwnerClass::MethodName
     */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #objectInfoT */
    zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

    // ============================================================
    // general
public:
    /**
     * @brief whether the method is registered by #ZFMethodUserRegister_0
     */
    inline zfbool methodIsUserRegister(void) const
    {
        return this->_ZFP_ZFMethod_methodIsUserRegister;
    }
    /**
     * @brief get the method's name
     * @see methodId
     */
    inline const zfchar *methodName(void) const
    {
        return this->_ZFP_ZFMethod_methodName.cString();
    }
    /**
     * @brief get the method id
     *
     * the method id is user-defined id to specify the overloaded method,
     * which was defined in ZFMETHOD_DECLARE_XXX seris,
     * it's ensured to be an empty string if not defined, instead of null
     * @see methodName
     */
    inline const zfchar *methodId(void) const
    {
        return this->_ZFP_ZFMethod_methodId.cString();
    }

    /**
     * @brief get the method's return value's type name
     */
    inline const zfchar *methodReturnTypeName(void) const
    {
        return this->_ZFP_ZFMethod_returnTypeName.cString();
    }

    /**
     * @brief get the method's param num
     */
    inline zfindex methodParamCount(void) const
    {
        return this->_ZFP_ZFMethod_paramNum;
    }
    /**
     * @brief get the method's param type name at index
     */
    inline const zfchar *methodParamTypeNameAtIndex(ZF_IN zfindex index) const
    {
        return this->_ZFP_ZFMethod_paramTypeNameList[index];
    }

    /**
     * @brief invoker function address for both class member type and function type
     */
    inline ZFFuncAddrType methodInvoker(void) const
    {
        return this->_ZFP_ZFMethod_invoker;
    }
    /**
     * @brief override original #methodInvoker
     *
     * set null to remove the overrided value,
     * the original invoker can be accessed by #methodInvokerOrg
     * @note no safe check for the method's proto type,
     *   you must ensure it's valid and safe to be called
     * @note assume the original method's proto type is:
     *   @code
     *     ReturnType method(ParamType0, ParamType1, ...);
     *   @endcode
     *   then the invoker's proto type must be:
     *   @code
     *     ReturnType method(ZFObject *, ParamType0, ParamType1, ...);
     *   @endcode
     * @note overriding the invoker only affect methods when they are reflected,
     *   calling it directly won't be affected
     *   @code
     *     obj->myMethod(); // won't be affected
     *     method->execute<void>(obj); // would be affected
     *   @endcode
     */
    void methodInvokerSet(ZF_IN ZFFuncAddrType methodInvoker) const;
    /**
     * @brief see #methodInvokerSet
     */
    inline ZFFuncAddrType methodInvokerOrg(void) const
    {
        return this->_ZFP_ZFMethod_invokerOrg;
    }

    /**
     * @brief generic invoker for advanced reflection
     *
     * as a static language, C++'s function invoking require explicit type declaration,
     * which is a pain when binding with script languages\n
     * since ZFFramework supply reflection (though limited),
     * we are trying to solve the dynamic script binding, how it works:
     * -  #ZFVarConvertFromZFObject/#ZFVarConvertToZFObject\n
     *   to supply type conversion to ZFObject types without knowing actual type
     * -  #ZFMethodGenericInvoker\n
     *   to invoke the reflectable method without static type binding
     *   (all params and result are #ZFObject types)
     *
     * \n
     * typical steps for users:
     * -# have all methods you need to bind been declared by #ZFMETHOD_DECLARE_0 series
     * -# supply wrapper class to hold the type
     * -# ensure all params can be converted to ZFObject types,
     *   by declaring them by #ZFVAR_CONVERT_DECLARE
     * -# all done, all binding works should be done by impl
     *
     * \n
     * typical steps for impl:
     * -# supply type convert methods to bind #ZFVarConvertFromZFObject/#ZFVarConvertToZFObject types to script languages
     * -# using reflection of #ZFClass and #ZFMethod,
     *   to bind all class and methods to script languages
     *
     * \n
     * the final script that calling ZFFFramework, should look like:
     * @code
     *   var obj = MyZFObjectType::newInstance();
     *   var objParam = SomeType::newInstance();
     *   var result = obj.myFunc(v_zfnumber(1), v_zfstring("2"), objParam)
     * @endcode
     */
    inline ZFMethodGenericInvoker methodGenericInvoker(void) const
    {
        return this->_ZFP_ZFMethod_methodGenericInvoker;
    }
    /**
     * @brief change default impl for #methodGenericInvoker
     *
     * the original invoker can be accessed by #methodGenericInvokerOrg
     */
    void methodGenericInvokerSet(ZF_IN ZFMethodGenericInvoker methodGenericInvoker) const;
    /**
     * @brief see #methodGenericInvokerSet
     */
    inline ZFMethodGenericInvoker methodGenericInvokerOrg(void) const
    {
        return this->_ZFP_ZFMethod_methodGenericInvokerOrg;
    }

    // ============================================================
    // class member type
public:
    /**
     * @brief get the owner's class info
     */
    inline const ZFClass *ownerClass(void) const
    {
        return this->_ZFP_ZFMethod_ownerClass;
    }

    /**
     * @brief get the method's access type
     */
    inline ZFMethodPrivilegeType methodPrivilegeType(void) const
    {
        return this->_ZFP_ZFMethod_privilegeType;
    }
    /**
     * @brief get the string of access type
     */
    const zfchar *methodPrivilegeTypeString(void) const;

    /**
     * @brief true if method is static
     */
    inline zfbool methodIsStatic(void) const
    {
        return this->_ZFP_ZFMethod_methodIsStatic;
    }

    /**
     * @brief true if method is virtual
     */
    inline zfbool methodIsVirtual(void) const
    {
        return this->_ZFP_ZFMethod_methodIsVirtual;
    }

    /**
     * @brief true if method is const
     */
    inline zfbool methodIsConst(void) const
    {
        return this->_ZFP_ZFMethod_methodIsConst;
    }

    // ============================================================
    // func type
public:
    /**
     * @brief true if this method is function type (declared by ZFMETHOD_FUNC_DECLARE_XXX series)
     */
    inline zfbool methodIsFunctionType(void) const
    {
        return (this->_ZFP_ZFMethod_ownerClass == zfnull);
    }
    /**
     * @brief get the method namespace, for func type only
     */
    inline const zfchar *methodNamespace(void) const
    {
        return this->_ZFP_ZFMethod_methodNamespace.cString();
    }

public:
    ZFMethod *_ZFP_ZFMethod_removeConst(void) const
    {
        return const_cast<ZFMethod *>(this);
    }

private:
    // general
    zfbool _ZFP_ZFMethod_methodIsUserRegister;
    ZFFuncAddrType _ZFP_ZFMethod_invoker;
    ZFFuncAddrType _ZFP_ZFMethod_invokerOrg;
    ZFMethodGenericInvoker _ZFP_ZFMethod_methodGenericInvoker;
    ZFMethodGenericInvoker _ZFP_ZFMethod_methodGenericInvokerOrg;
    zfstring _ZFP_ZFMethod_methodName;
    zfstring _ZFP_ZFMethod_methodId;
    zfstring _ZFP_ZFMethod_returnTypeName;
    zfindex _ZFP_ZFMethod_paramNum;
    zfstring _ZFP_ZFMethod_paramTypeNameList[ZFMETHOD_MAX_PARAM];

    // for class member type
    const ZFClass *_ZFP_ZFMethod_ownerClass;
    ZFMethodPrivilegeType _ZFP_ZFMethod_privilegeType;
    zfbool _ZFP_ZFMethod_methodIsStatic;
    zfbool _ZFP_ZFMethod_methodIsVirtual;
    zfbool _ZFP_ZFMethod_methodIsConst;

    // for func type
    zfstring _ZFP_ZFMethod_methodNamespace;
};
extern ZF_ENV_EXPORT ZFMethod *_ZFP_ZFMethodInstanceFind(ZF_IN const zfchar *internalMethodId);
extern ZF_ENV_EXPORT ZFMethod *_ZFP_ZFMethodInstanceAccess(ZF_IN const zfchar *internalMethodId);
extern ZF_ENV_EXPORT zfbool _ZFP_ZFMethodInstanceCleanup(ZF_IN const zfchar *internalMethodId);
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFMethodInstanceHolder
{
public:
    _ZFP_ZFMethodInstanceHolder(ZF_IN const zfchar *internalMethodId)
    : internalMethodId(internalMethodId)
    , method(_ZFP_ZFMethodInstanceAccess(internalMethodId))
    {
    }
    ~_ZFP_ZFMethodInstanceHolder(void)
    {
        _ZFP_ZFMethodInstanceCleanup(this->internalMethodId);
    }
private:
    zfstring internalMethodId;
public:
    ZFMethod *method;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodDef_h_

#include "ZFMethodDeclareDef.h"
#include "ZFMethodDeclareFuncDef.h"
#include "ZFMethodFilterDef.h"

