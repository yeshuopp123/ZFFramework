/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectDeclareDef.h
 * @brief ZFObject declaration
 */

#ifndef _ZFI_ZFObjectDeclareDef_h_
#define _ZFI_ZFObjectDeclareDef_h_

#include "ZFObjectClassTypeFwd.h"
#include "ZFObjectCastDef.h"
#include "ZFAnyDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
template<typename T_zfsuper, typename T_superInterface, int superImplementsInterface>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfsuperI
{
public:
    typedef T_superInterface SuperType;
};
template<typename T_zfsuper, typename T_superInterface>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfsuperI<T_zfsuper, T_superInterface, 1>
{
public:
    typedef T_zfsuper SuperType;
};
/**
 * @brief class ref to proper super type, see #ZFObject for more info
 */
#define zfsuperI(T_SuperType) \
    _ZFP_zfsuperI<zfsuper, T_SuperType, zftTypeIsTypeOf<zfsuper, T_SuperType>::TypeIsTypeOf>::SuperType

// ============================================================
#define _ZFP_ZFOBJECT_DECLARE_PROTECTED_CONSTRUCTOR(ChildClass, SuperClass) \
    protected: \
        /** @cond ZFPrivateDoc */ \
        ChildClass(void) : SuperClass() {} \
        virtual ~ChildClass(void) {} \
        /** @endcond */ \
    public:
#define _ZFP_ZFOBJECT_DECLARE(ChildClass, SuperClass) \
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ChildClass) \
    protected: \
        /** @brief class ref to super */ \
        typedef SuperClass zfsuper; \
        /** @brief class ref to self */ \
        typedef ChildClass zfself; \
    public: \
        /** @cond ZFPrivateDoc */ \
        template<typename T_ZFObject> \
        inline T_ZFObject to(void) \
        { \
            return ZFCastZFObjectUnchecked(T_ZFObject, this); \
        } \
        virtual inline ZFObject *toObject(void) \
        { \
            return this; \
        } \
        inline ZFAny toAny(void) \
        { \
            return ZFAny(this); \
        } \
        /** @endcond */ \
    public: \
        static ZFClass *_ZFP_ZFObjectGetClass(void) \
        { \
            static ZFClass *cls = zfself::_ZFP_ZFObjectClassRegister(); \
            return cls; \
        } \
        /** @brief get class info */ \
        static const ZFClass *ClassData(void) \
        { \
            return zfself::_ZFP_ZFObjectGetClass(); \
        } \
    public: \
        /** @brief get instance's class info */ \
        virtual const ZFClass *classData(void) \
        { \
            return zfself::_ZFP_ZFObjectGetClass(); \
        } \
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
    public:
#define _ZFP_ZFOBJECT_DECLARE_OBJECT(ChildClass, SuperClass) \
    public: \
        typedef enum {_ZFP_ZFObjectCanAlloc = 1} _ZFP_ZFObjectCanAllocChecker; \
    private: \
        static void _ZFP_ZFObjectDeleteCallback(void *obj) \
        { \
            ZFCastStatic(ChildClass *, obj)->~ChildClass(); \
        } \
    public: \
        zfpoolDeclareFriend() \
        static ZFObject *_ZFP_ZFObject_constructor(void) \
        { \
            return zfpoolNew(zfself); \
        } \
        static void _ZFP_ZFObject_destructor(ZF_IN ZFObject *obj) \
        { \
            zfpoolDelete(_ZFP_ZFCastZFObjectInternal(zfself *, obj)); \
        } \
        static ZFClass *_ZFP_ZFObjectClassRegister(void) \
        { \
            static zfbool ZFCoreLibDestroyFlag = zffalse; \
            static _ZFP_ZFClassRegisterHolder _holder( \
                &ZFCoreLibDestroyFlag, \
                ZFClass::_ZFP_ZFClassInitFinish(zfself::_ZFP_ZFObjectCheckInitImplementationList(ZFClass::_ZFP_ZFClassRegister( \
                    &ZFCoreLibDestroyFlag, \
                    zfText(#ChildClass), \
                    zfsuper::ClassData(), \
                    &zfself::_ZFP_ZFObject_constructor, \
                    &zfself::_ZFP_ZFObject_destructor)))); \
            return _holder.cls; \
        }
#define _ZFP_ZFOBJECT_DECLARE_ABSTRACT(ChildClass, SuperClass) \
    public: \
        typedef enum {_ZFP_ZFObjectCanAlloc = 0} _ZFP_ZFObjectCanAllocChecker; \
    public: \
        static ZFClass *_ZFP_ZFObjectClassRegister(void) \
        { \
            static zfbool ZFCoreLibDestroyFlag = zffalse; \
            static _ZFP_ZFClassRegisterHolder _holder( \
                &ZFCoreLibDestroyFlag, \
                ZFClass::_ZFP_ZFClassInitFinish(zfself::_ZFP_ZFObjectCheckInitImplementationList(ZFClass::_ZFP_ZFClassRegister( \
                    &ZFCoreLibDestroyFlag, \
                    zfText(#ChildClass), \
                    zfsuper::ClassData(), \
                    zfnull, \
                    zfnull)))); \
            return _holder.cls; \
        }
/**
 * @brief necessary for every class inherit from ZFObject
 *
 * for more information, please refer to #ZFObject
 * @see ZFObject, ZFOBJECT_REGISTER, ZFOBJECT_DECLARE_ABSTRACT
 */
#define ZFOBJECT_DECLARE(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE_OBJECT(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE_PROTECTED_CONSTRUCTOR(ChildClass, SuperClass) \
    public:
/**
 * @brief declare object which allow custom constructor
 *
 * by default, ZFObject declare constructor as protected
 * so that app can't construct ZFObject on stack,
 * you may use this macro instead of #ZFOBJECT_DECLARE
 * to allow user defined constructor\n
 * \n
 * NOTE: never abuse this macro,
 * and strongly recommended not to allow ZFObject create on stack (i.e. public constructors)
 */
#define ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE_OBJECT(ChildClass, SuperClass) \
    public:
/**
 * @brief necessary for every abstract class inherit from ZFObject
 *
 * for more information, please refer to #ZFObject
 * @see ZFObject, ZFOBJECT_REGISTER, ZFOBJECT_DECLARE
 */
#define ZFOBJECT_DECLARE_ABSTRACT(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE_ABSTRACT(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE_PROTECTED_CONSTRUCTOR(ChildClass, SuperClass) \
    public:
/**
 * @brief see #ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR
 */
#define ZFOBJECT_DECLARE_ABSTRACT_ALLOW_CUSTOM_CONSTRUCTOR(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE_ABSTRACT(ChildClass, SuperClass) \
    public:

/**
 * @brief register ZFObject to ZFClass map
 *
 * usage:
 * @code
 *   // in something.h:
 *   zfclass YourClass : zfextends BaseClass
 *   {
 *       ZFOBJECT_DECLARE(YourClass, BaseClass)
 *   };
 *
 *   // in something.cpp:
 *   #include "something.h"
 *   ZFOBJECT_REGISTER(YourClass)
 * @endcode
 * you only need this if you want to use the ZFClass map function,
 * such as ZFClass::classForName and ZFClass::newInstanceForName\n
 * \n
 * detailed:\n
 * ZFClass map is set up when YourClass::ClassData is called, which may occurred when:
 * -  YourClass::ClassData or subclass's ClassData, is explicitly called
 * -  ever created a instance of YourClass or subclass
 * -  ZFOBJECT_REGISTER(YourClass) is added to a certain h or cpp file
 *
 * this macro actually do only one thing:
 * declare a static global variable to make sure YourClass::ClassData is called\n
 * so if you add it to cpp file, only one dummy global variable is defined\n
 * on the other hand, if you add it to h file, multiple dummy variables are defined,
 * as more as the cpp files which include the h file
 * @see ZFOBJECT_DECLARE, ZFClass
 */
#define ZFOBJECT_REGISTER(T_ZFObject) \
    ZF_STATIC_INITIALIZER_INIT(ZFObjectRegister_##T_ZFObject) \
    { \
        (void)T_ZFObject::ClassData(); \
    } \
    ZF_STATIC_INITIALIZER_END(ZFObjectRegister_##T_ZFObject)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectDeclareDef_h_

