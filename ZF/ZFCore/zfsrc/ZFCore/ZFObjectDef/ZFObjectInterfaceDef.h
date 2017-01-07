/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectInterfaceDef.h
 * @brief interface declare
 */

#ifndef _ZFI_ZFObjectInterfaceDef_h_
#define _ZFI_ZFObjectInterfaceDef_h_

#include "ZFObjectClassTypeFwd.h"
#include "ZFObjectCastDef.h"
#include "ZFAnyDef.h"
#include "ZFClassDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFObject;
zfclassFwd ZFClass;

/**
 * @brief shows the type is an interface, see #ZFInterface
 */
#define zfinterface class
/**
 * @brief extends for interface, see #ZFInterface
 */
#define zfimplements public

// ============================================================
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFInterfaceDummyParent
{
protected:
    virtual ~_ZFP_ZFInterfaceDummyParent(void) {}

public:
    static const ZFClass *ClassData(void)
    {
        return zfnull;
    }
public:
    static void _ZFP_ZFObjectInitImplementationList(ZFClass *cls) {}
public:
    static void _ZFP_ZFInterface_INHERIT_CHECKER(void)
    {
    }
};
#define _ZFP_ZFINTERFACE_DECLARE(InterfaceName, DummyParent) \
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(InterfaceName) \
    protected: \
        /** @brief typedef for super (always #ZFInterface for an interface type) */ \
        typedef DummyParent zfsuper; \
        /** @brief typedef for self */ \
        typedef InterfaceName zfself; \
    private: \
        static ZFClass *_ZFP_ZFObjectCheckInitImplementationList(ZF_IN ZFClass *cls) \
        { \
            ZFCoreMutexLocker(); \
            if(cls->_ZFP_ZFClassNeedInitImplementationList) \
            { \
                cls->_ZFP_ZFClassNeedInitImplementationList = zffalse; \
                if(zfself::_ZFP_ZFObjectInitImplementationList != zfsuper::_ZFP_ZFObjectInitImplementationList) \
                { \
                    zfself::_ZFP_ZFObjectInitImplementationList(cls); \
                } \
            } \
            return cls; \
        } \
    public: \
        static ZFClass *_ZFP_ZFObjectClassRegister(void) \
        { \
            static zfbool ZFCoreLibDestroyFlag = zffalse; \
            static _ZFP_ZFClassRegisterHolder _holder( \
                &ZFCoreLibDestroyFlag, \
                ZFClass::_ZFP_ZFClassInitFinish(zfself::_ZFP_ZFObjectCheckInitImplementationList(ZFClass::_ZFP_ZFClassRegister( \
                    &ZFCoreLibDestroyFlag, \
                    zfText(#InterfaceName), \
                    DummyParent::ClassData(), \
                    zfnull, \
                    zfnull, \
                    zftrue)))); \
            return _holder.cls; \
        } \
        static ZFClass *_ZFP_ZFObjectGetClass(void) \
        { \
            static ZFClass *cls = zfself::_ZFP_ZFObjectClassRegister(); \
            return cls; \
        } \
        /** @brief get class info */ \
        static const ZFClass *ClassData(void) \
        { \
            static const ZFClass *cls = zfself::_ZFP_ZFObjectClassRegister(); \
            return cls; \
        } \
    protected: \
        virtual void _ZFP_ZFInterface_INHERIT_CHECKER(void) \
        { \
            /* used to ensure inherit from ZFInterface */ \
            _ZFP_ZFInterfaceDummyParent::_ZFP_ZFInterface_INHERIT_CHECKER(); \
        } \
    public: \
        /** @cond ZFPrivateDoc */ \
        template<typename T_ZFObject> \
        inline T_ZFObject to(void) \
        { \
            return ZFCastZFObjectUnchecked(T_ZFObject, this); \
        } \
        inline ZFAny toAny(void) \
        { \
            return ZFAny(this); \
        } \
        /** @endcond */ \
    public:
/**
 * @brief see #ZFINTERFACE_DECLARE
 *
 * you may have custom constructor with this macro,
 * but must be declared as protected with no params,
 * and must also declare an virtual protected destructor
 */
#define ZFINTERFACE_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(InterfaceName, ParentInterfaceHint, ...) \
        _ZFP_ZFINTERFACE_DECLARE(InterfaceName, ZFInterface) \
        _ZFP_ZFIMPLEMENTS_DECLARE(ParentInterfaceHint, ##__VA_ARGS__) \
    public:
/**
 * @brief see #ZFInterface
 */
#define ZFINTERFACE_DECLARE(InterfaceName, ParentInterfaceHint, ...) \
        ZFINTERFACE_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(InterfaceName, ParentInterfaceHint, ##__VA_ARGS__) \
    protected: \
        /** @cond ZFPrivateDoc */ \
        InterfaceName(void) {} \
        virtual ~InterfaceName(void) {} \
        /** @endcond */ \
    public:

template<typename T_FromZFObjectOrZFInterface, typename T_ToZFInterface, int isInterface>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFInterfaceCastWrapper
{
};
template<typename T_FromZFObjectOrZFInterface, typename T_ToZFInterface>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFInterfaceCastWrapper<T_FromZFObjectOrZFInterface, T_ToZFInterface, 0>
{
public:
    static ZFInterface *cast(ZF_IN ZFObject * const &obj)
    {
        return (T_ToZFInterface *)static_cast<T_FromZFObjectOrZFInterface *>(obj);
    }
};
template<typename T_FromZFObjectOrZFInterface, typename T_ToZFInterface>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFInterfaceCastWrapper<T_FromZFObjectOrZFInterface, T_ToZFInterface, 1>
{
public:
    template<typename T_ZFObject>
    static inline ZFInterface *cast(ZF_IN T_ZFObject * const &obj)
    {
        return ZFCastZFObjectUnchecked(T_ToZFInterface *, obj->toObject());
    }
};
#define _ZFP_ZFIMPLEMENTS_DECLARE_EXPAND_CLASS(Interface, index, total) \
    Interface::ClassData()
#define _ZFP_ZFIMPLEMENTS_DECLARE_EXPAND_CAST_CALLBACK_NAME(Interface, index, total) \
    &zfself::_ZFP_ZFInterfaceCastCallback_##Interface
#define _ZFP_ZFIMPLEMENTS_DECLARE_EXPAND_CAST_CALLBACK(Interface, index, total) \
    static ZFInterface *_ZFP_ZFInterfaceCastCallback_##Interface(ZF_IN ZFObject * const &obj) \
    { \
        return _ZFP_ZFInterfaceCastWrapper<zfself, Interface, (zftTypeIsTypeOf<zfself, ZFObject>::TypeIsTypeOf ? 0 : 1)>::cast(obj); \
    }
#define _ZFP_ZFIMPLEMENTS_DECLARE_EXPAND_INTERFACE_ON_INIT(Interface, index, total) \
    Interface::_ZFP_ZFInterface_interfaceOnInit();
#define _ZFP_ZFIMPLEMENTS_DECLARE_EXPAND_INTERFACE_ON_DEALLOC(Interface, index, total) \
    Interface::_ZFP_ZFInterface_interfaceOnDealloc();
#define _ZFP_ZFIMPLEMENTS_DECLARE(ImplementedInterfaces, ...) \
    public: \
        static void _ZFP_ZFObjectInitImplementationList(ZFClass *cls) \
        { \
            if(cls->_ZFP_ZFClass_interfaceNeedRegister()) \
            { \
                cls->_ZFP_ZFClass_interfaceRegister( \
                    ZFM_FIX_PARAM(_ZFP_ZFIMPLEMENTS_DECLARE_EXPAND_CLASS, ZFM_COMMA, ImplementedInterfaces, ##__VA_ARGS__) \
                    , 0 \
                    ); \
                cls->_ZFP_ZFClass_interfaceRegisterCastCallback( \
                    ZFM_FIX_PARAM(_ZFP_ZFIMPLEMENTS_DECLARE_EXPAND_CAST_CALLBACK_NAME, ZFM_COMMA, ImplementedInterfaces, ##__VA_ARGS__) \
                    , 0 \
                    ); \
            } \
        } \
    public: \
        ZFM_FIX_PARAM(_ZFP_ZFIMPLEMENTS_DECLARE_EXPAND_CAST_CALLBACK, ZFM_EMPTY, ImplementedInterfaces, ##__VA_ARGS__) \
    public:
/**
 * @brief see #ZFINTERFACE_DECLARE
 */
#define ZFIMPLEMENTS_DECLARE(ImplementedInterfaces, ...) \
        _ZFP_ZFIMPLEMENTS_DECLARE(ImplementedInterfaces, ##__VA_ARGS__) \
    public: \
        virtual inline void _ZFP_ZFObject_interfaceOnInit(void) \
        { \
            zfsuper::_ZFP_ZFObject_interfaceOnInit(); \
            ZFM_FIX_PARAM(_ZFP_ZFIMPLEMENTS_DECLARE_EXPAND_INTERFACE_ON_INIT, ZFM_EMPTY, ImplementedInterfaces, ##__VA_ARGS__) \
        } \
        virtual inline void _ZFP_ZFObject_interfaceOnDealloc(void) \
        { \
            ZFM_FIX_PARAM(_ZFP_ZFIMPLEMENTS_DECLARE_EXPAND_INTERFACE_ON_DEALLOC, ZFM_EMPTY, ImplementedInterfaces, ##__VA_ARGS__) \
            zfsuper::_ZFP_ZFObject_interfaceOnDealloc(); \
        } \
    public:

/**
 * @brief dummy base of interface types
 *
 * usage:
 * @code
 *   // declare an interface
 *   zfinterface YourInterface : zfextends ZFInterface
 *   {
 *       ZFINTERFACE_DECLARE(YourInterface, ParentInterface1, ParentInterface2, ...)
 *   };
 *
 *   // implement an interface
 *   zfclass YourType : zfextends ZFObject, zfimplements YourInterface
 *   {
 *       ZFOBJECT_DECLARE(YourType, ZFObject)
 *       ZFIMPLEMENTS_DECLARE(YourInterface)
 *   };
 * @endcode
 *
 * due to some limitations of C++ (basically the limitations of diamond inheritance),
 * you should remember these while using interface:
 * -  an interface must inherit from #ZFInterface,
 *   and must not be inherited by another interface,
 *   it can only be "implemented" by a "ZFObject type",
 *   for how to achieve the inheritance of an interface,
 *   see bellow
 * -  you must have #ZFINTERFACE_DECLARE declared in your interface,
 *   and must have ZFIMPLEMENTS_DECLARE declared in your child type
 *   that implements the interface
 * -  you must not override anything within an interface,
 *   such as:
 *   -  ZFProperty (ZFPROPERTY_OVERRIDE_XXX)
 *   -  ZFMethod (ZFMETHOD_OVERRIDE_XXX)
 *   -  normal virtual method
 *
 * \n
 * ADVANCED:\n
 * to simulate a interface that inherit from another interface,
 * you should do it like this:\n
 * Java code:
 * @code
 *   public interface ParentInterface {}
 *   public interface ChildInterface extends ParentInterface {}
 *   public class ImplementParent implements ParentInterface {}
 *   public class ImplementChild extends ImplementParent implements ChildInterface {}
 *   public class ImplementChildDirectly implements ChildInterface {}
 * @endcode
 * equivalent code for ZFInterface:
 * @code
 *   zfinterface ParentInterface : zfextends ZFInterface
 *   {
 *       ZFINTERFACE_DECLARE(ParentInterface, ZFInterface)
 *   };
 *   zfinterface ChildInterface : zfextends ZFInterface
 *   {
 *       ZFINTERFACE_DECLARE(ChildInterface, ParentInterface)
 *   }
 *   zfclass ImplementParent : zfextends ZFObject, zfimplements ParentInterface
 *   {
 *       ZFOBJECT_DECLARE(ImplementParent, ZFObject)
 *       ZFIMPLEMENTS_DECLARE(ParentInterface)
 *   }
 *   zfclass ImplementChild : zfextends ImplementParent, zfimplements ChildInterface
 *   {
 *       ZFOBJECT_DECLARE(ImplementChild, ImplementParent)
 *       ZFIMPLEMENTS_DECLARE(ChildInterface)
 *   }
 *   zfclass ImplementChildDirectly : zfextends ZFObject, zfimplements ParentInterface, zfimplements ChildInterface
 *   {
 *       ZFOBJECT_DECLARE(ImplementChild, ZFObject)
 *       ZFIMPLEMENTS_DECLARE(ParentInterface, ChildInterface)
 *   }
 * @endcode
 * note that ChildInterface not directly inherit from ParentInterface,
 * they are bounded by some class that implements them both
 * (ImplementChild and ImplementChildDirectly in this example),
 * ImplementChild inexplicitly implements ParentInterface (inherit from ImplementParent),
 * and ImplementChildDirectly have no parent bounded with ParentInterface,
 * so it must implements ParentInterface manually
 */
zfinterface ZF_ENV_EXPORT ZFInterface
{
    _ZFP_ZFINTERFACE_DECLARE(ZFInterface, _ZFP_ZFInterfaceDummyParent)

protected:
    /** @cond ZFPrivateDoc */
    ZFInterface(void) {}
    virtual ~ZFInterface(void) {}
    /** @endcond */

public:
    static void _ZFP_ZFObjectInitImplementationList(ZFClass *cls) {}
    zffinal inline void _ZFP_ZFInterface_interfaceOnInit(void) {}
    zffinal inline void _ZFP_ZFInterface_interfaceOnDealloc(void) {}

public:
    /** @brief get instance's class info */
    virtual const ZFClass *classData(void) = 0;

    /**
     * @brief convert to ZFObject type
     */
    virtual ZFObject *toObject(void) = 0;
};

/**
 * @brief declare custom init step for the interface
 *
 * although #ZFInterface is designed as interface type,
 * it is normal class in C++ in fact,
 * you can supply your own custom init step\n
 * once declared, the init step would be called
 * after #ZFObject::objectOnInit and before #ZFObject::objectOnInitFinish,
 * ordered from parent interface to child interface,
 * and the paired #ZFINTERFACE_ON_DEALLOC_DECLARE would be called
 * after #ZFObject::objectOnDeallocPrepare and before #ZFObject::objectOnDealloc\n
 * \n
 * note: use only if necessary, and at your own risk\n
 * overrided init step would be called for each instance of this class and all of subclasses,
 * take care of performance
 */
#define ZFINTERFACE_ON_INIT_DECLARE() \
    public: \
        zffinal void _ZFP_ZFInterface_interfaceOnInit(void)
/** @brief see #ZFINTERFACE_ON_INIT_DECLARE */
#define ZFINTERFACE_ON_INIT_DEFINE(YourInterface) \
    void YourInterface::_ZFP_ZFInterface_interfaceOnInit(void)

/** @brief see #ZFINTERFACE_ON_INIT_DECLARE */
#define ZFINTERFACE_ON_DEALLOC_DECLARE() \
    public: \
        zffinal void _ZFP_ZFInterface_interfaceOnDealloc(void)
/** @brief see #ZFINTERFACE_ON_INIT_DECLARE */
#define ZFINTERFACE_ON_DEALLOC_DEFINE(YourInterface) \
    void YourInterface::_ZFP_ZFInterface_interfaceOnDealloc(void)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectInterfaceDef_h_

