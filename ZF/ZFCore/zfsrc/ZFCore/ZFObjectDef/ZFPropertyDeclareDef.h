/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFPropertyDeclareDef.h
 * @brief macros to declare ZFProperty
 */

#ifndef _ZFI_ZFPropertyDeclareDef_h_
#define _ZFI_ZFPropertyDeclareDef_h_

#include "ZFPropertyDef.h"
#include "zfautoObjectFwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFClass;
// ============================================================
/**
 * @brief access the property directly, compile error if not declared
 */
#define ZFPropertyAccess(OwnerClass, Name) ((const ZFProperty *)OwnerClass::_ZFP_ZFProperty_##Name())

/**
 * @brief get property by name, or null if not registered
 */
extern ZF_ENV_EXPORT const ZFProperty *ZFPropertyGet(ZF_IN const zfchar *clsName,
                                                     ZF_IN const zfchar *propertyName);
/**
 * @brief get property by name, or null if not registered
 */
extern ZF_ENV_EXPORT const ZFProperty *ZFPropertyGet(ZF_IN const ZFClass *cls,
                                                     ZF_IN const zfchar *propertyName);

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyCallbackGetInfoRetainDefault(ZF_IN const ZFProperty *property,
                                                                      ZF_IN ZFObject *ownerObj,
                                                                      ZF_IN_OUT zfstring &ret);

#define _ZFP_ZFPROPERTY_DECLARE(Type, ZFPropertyTypeId_noneOrType, Name, \
                                propertyClassOfRetainProperty) \
    public: \
        static ZFProperty *_ZFP_ZFPropertyAccess_##Name(void) \
        { \
            static _ZFP_ZFPropertyInstanceHolder _propertyInfoHolder( \
                zfsConnectLineFree(zfself::ClassData()->className(), zfText("::"), ZFM_TOSTRING(Name))); \
            ZFProperty *_propertyInfo = _propertyInfoHolder.propertyInfo; \
            if(_propertyInfo->_ZFP_ZFPropertyNeedInit) \
            { \
                _propertyInfo->_ZFP_ZFPropertyNeedInit = zffalse; \
                _propertyInfo->_ZFP_ZFPropertyInit( \
                    zffalse, \
                    zfself::ClassData(), \
                    ZFM_TOSTRING(Name), \
                    ZFM_TOSTRING(Type), \
                    ZFPropertyTypeId_noneOrType, \
                    ZFMethodAccess(zfself, _ZFP_ZFPROPERTY_SETTER_NAME(Type, Name)), \
                    ZFMethodAccess(zfself, _ZFP_ZFPROPERTY_GETTER_NAME(Type, Name)), \
                    propertyClassOfRetainProperty); \
                _propertyInfo->callbackIsValueAccessed = zfself::_ZFP_ZFPropertyCallbackIsValueAccessed_##Name; \
                _propertyInfo->callbackIsInitValue = zfself::_ZFP_ZFPropertyCallbackIsInitValue_##Name; \
                _propertyInfo->callbackResetInitValue = zfself::_ZFP_ZFPropertyCallbackResetInitValue_##Name; \
                _propertyInfo->callbackCompare = zfself::_ZFP_ZFPropertyCallbackCompare_##Name; \
                _propertyInfo->callbackCopy = zfself::_ZFP_ZFPropertyCallbackCopy_##Name; \
                _propertyInfo->propertyReflectable = !ZFM_CLASS_HAS_MEMBER(_, _ZFP_ZFPropertyNoReflect_##Name, zfself); \
                zfself::_ZFP_ZFPropertyCallbackSetup_##Name(_propertyInfo); \
                zfself::_ZFP_ZFObjectGetClass()->_ZFP_ZFClass_propertyRegister(_propertyInfo); \
            } \
            return _propertyInfo; \
        } \
        static ZFProperty *_ZFP_ZFProperty_##Name(void) \
        { \
            static ZFProperty *_propertyInfo = zfself::_ZFP_ZFPropertyAccess_##Name(); \
            return _propertyInfo; \
        } \
        ZFM_CLASS_HAS_MEMBER_DECLARE(_, _ZFP_ZFPropertyNoReflect_##Name, void (*F)(void)) \
    private: \
        _ZFP_ZFPROPERTY_DECLARE_CALLBACK_IS_INIT_VALUE(Type, Name) \
        _ZFP_ZFPROPERTY_DECLARE_CALLBACK_RESET_INIT_VALUE(Type, Name) \
        _ZFP_ZFPROPERTY_DECLARE_CALLBACK_COMPARE(Type, Name) \
        _ZFP_ZFPROPERTY_DECLARE_CALLBACK_COPY(Type, Name) \
    public:
#define _ZFP_ZFPROPERTY_VALUE_DECLARE_RETAIN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                             ZFPropertyInitValueOrNoInitValue) \
    private: \
        template<typename T_Type> \
        zffinal zfclassNotPOD _ZFP_ZFPropertyValue_##Name \
        { \
        public: \
            _ZFP_ZFPropertyValue_##Name(void) \
            : value(zfnull), valueHolder() \
            { \
                zfself::_ZFP_ZFProperty_##Name(); \
            } \
            ~_ZFP_ZFPropertyValue_##Name(void) \
            { \
                zfpoolDelete(this->value); \
            } \
        public: \
            inline T_Type &propertyInit(ZF_IN zfself *owner, \
                                        ZF_IN_OPT zfbool needNotifySetter = zftrue) \
            { \
                if(!(this->value)) \
                { \
                    ZFCoreMutexLock(); \
                    T_Type *valueTmp = zfnull; \
                    zflockfree_zfRetain(*(valueTmp = zfpoolNew(T_Type, ZFPropertyInitValueOrNoInitValue))); \
                    zfautoObject valueHolderTmp = zflockfree_zfautoObjectCreate(ZFObjectToObject(*valueTmp)); \
                    zflockfree_zfRelease(*valueTmp); \
                    _ZFP_ZFPropertyRawInitStepInvoker_##Name<zfself, \
                        ZFM_CLASS_HAS_MEMBER(_, _ZFP_ZFPropertyRawInitStep_##Name, zfself)>::action(owner, valueHolderTmp); \
                    if(owner->classData()->_ZFP_ZFClass_propertyInitStepAction( \
                        owner->toObject(), \
                        zfself::_ZFP_ZFProperty_##Name(), \
                        ZFCastReinterpret(void *, &valueHolderTmp)) && needNotifySetter) \
                    { \
                        this->value = valueTmp; \
                        *valueTmp = zfnull; \
                        /* call setter to update owner */ \
                        owner->_ZFP_ZFPROPERTY_SETTER_NAME(Type, Name)(valueHolderTmp.template to<T_Type>()); \
                    } \
                    else \
                    { \
                        this->value = valueTmp; \
                        this->valueHolder = valueHolderTmp; \
                        *valueTmp = this->valueHolder.template to<T_Type>(); \
                        owner->toObject()->_ZFP_ZFObject_objectPropertyValueCheckMarkCache(zfself::_ZFP_ZFProperty_##Name(), zftrue); \
                    } \
                    owner->toObject()->_ZFP_ZFObject_objectPropertyValueOnInit(zfself::_ZFP_ZFProperty_##Name()); \
                    ZFCoreMutexUnlock(); \
                } \
                return (*(this->value)); \
            } \
            inline zfbool propertyAccessed(void) const \
            { \
                return (this->value != zfnull); \
            } \
            inline void propertyReset(ZF_IN zfself *owner) \
            { \
                if(this->value) \
                { \
                    owner->toObject()->_ZFP_ZFObject_objectPropertyValueCheckMarkCache(zfself::_ZFP_ZFProperty_##Name(), zffalse); \
                    owner->toObject()->_ZFP_ZFObject_objectPropertyValueOnReset(zfself::_ZFP_ZFProperty_##Name()); \
                    zfpoolDelete(this->value); \
                    this->value = zfnull; \
                    this->valueHolder = zfautoObjectNull; \
                } \
            } \
        private: \
            T_Type *value; \
        public: \
            zfautoObject valueHolder; \
        }; \
        _ZFP_ZFPropertyValue_##Name<Type> Name##_ZFPropertyValue; \
    public: \
        ZFM_CLASS_HAS_MEMBER_DECLARE(_, _ZFP_ZFPropertyRawInitStep_##Name, void (*F)(ZF_IN_OUT zfautoObject &)) \
        /* ZFDoxygenBugFix */ \
        /* can't be filtered by "_ZFP_*", reason unknown */ \
        /** @cond ZFPrivateDoc */ \
        template<typename T_Owner, int hasCustom> \
        zfclassNotPOD _ZFP_ZFPropertyRawInitStepInvoker_##Name \
        { \
        public: \
            static inline void action(ZF_IN T_Owner *owner, ZF_IN_OUT zfautoObject &propertyValue) \
            { \
            } \
        }; \
        template<typename T_Owner> \
        zfclassNotPOD _ZFP_ZFPropertyRawInitStepInvoker_##Name<T_Owner, 1> \
        { \
        public: \
            static inline void action(ZF_IN T_Owner *owner, ZF_IN_OUT zfautoObject &propertyValue) \
            { \
                owner->_ZFP_ZFPropertyRawInitStep_##Name(propertyValue); \
            } \
        }; \
        /** @endcond */ \
    public: \
        zfbool _ZFP_ZFPropertyCallbackIsInitValueActionInternal_##Name(ZF_OUT_OPT void *outInitValue) \
        { \
            if(Name##_ZFPropertyValue.propertyAccessed()) \
            { \
                zfself::_ZFP_ZFPropertyValue_##Name<Type> _holder; \
                if(outInitValue != zfnull) \
                { \
                    *(zfautoObject *)outInitValue = zfautoObjectCreateWithoutLeakTest( \
                        ZFCastZFObjectUnchecked(ZFObject *, _holder.propertyInit(this, zffalse))); \
                } \
                return (ZFComparerDefault( \
                        this->_ZFP_ZFPROPERTY_GETTER_NAME_INTERNAL(Type, Name)(), _holder.propertyInit(this, zffalse)) \
                    == ZFCompareTheSame); \
            } \
            else \
            { \
                return zftrue; \
            } \
        } \
    public:
#define _ZFP_ZFPROPERTY_VALUE_DECLARE_ASSIGN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                             ZFPropertyInitValueOrNoInitValue) \
    private: \
        template<typename T_Type> \
        zffinal zfclassNotPOD _ZFP_ZFPropertyValue_##Name \
        { \
        public: \
            _ZFP_ZFPropertyValue_##Name(void) \
            : value(zfnull) \
            { \
                zfself::_ZFP_ZFProperty_##Name(); \
            } \
            ~_ZFP_ZFPropertyValue_##Name(void) \
            { \
                zfpoolDelete(this->value); \
            } \
        public: \
            inline T_Type &propertyInit(ZF_IN zfself *owner, \
                                        ZF_IN_OPT zfbool needNotifySetter = zftrue) \
            { \
                if(!(this->value)) \
                { \
                    ZFCoreMutexLock(); \
                    T_Type *valueTmp = zfpoolNew(T_Type, ZFPropertyInitValueOrNoInitValue); \
                    _ZFP_ZFPropertyRawInitStepInvoker_##Name<zfself, \
                        ZFM_CLASS_HAS_MEMBER(_, _ZFP_ZFPropertyRawInitStep_##Name, zfself)>::action(owner, *valueTmp); \
                    if(owner->classData()->_ZFP_ZFClass_propertyInitStepAction( \
                        owner->toObject(), \
                        zfself::_ZFP_ZFProperty_##Name(), \
                        ZFCastReinterpret(void *, valueTmp)) && needNotifySetter) \
                    { \
                        this->value = valueTmp; \
                        /* call setter to update owner */ \
                        owner->_ZFP_ZFPROPERTY_SETTER_NAME(Type, Name)(*valueTmp); \
                    } \
                    else \
                    { \
                        this->value = valueTmp; \
                    } \
                    owner->toObject()->_ZFP_ZFObject_objectPropertyValueOnInit(zfself::_ZFP_ZFProperty_##Name()); \
                    ZFCoreMutexUnlock(); \
                } \
                return (*(this->value)); \
            } \
            inline zfbool propertyAccessed(void) const \
            { \
                return (this->value != zfnull); \
            } \
            inline void propertyReset(ZF_IN zfself *owner) \
            { \
                if(this->value) \
                { \
                    owner->toObject()->_ZFP_ZFObject_objectPropertyValueOnReset(zfself::_ZFP_ZFProperty_##Name()); \
                    zfpoolDelete(this->value); \
                    this->value = zfnull; \
                } \
            } \
        private: \
            T_Type *value; \
        }; \
        _ZFP_ZFPropertyValue_##Name<Type> Name##_ZFPropertyValue; \
    public: \
        ZFM_CLASS_HAS_MEMBER_DECLARE(_, _ZFP_ZFPropertyRawInitStep_##Name, void (*F)(ZF_IN_OUT Type &)) \
        /* ZFDoxygenBugFix */ \
        /* can't be filtered by "_ZFP_*", reason unknown */ \
        /** @cond ZFPrivateDoc */ \
        template<typename T_Owner, int hasCustom> \
        zfclassNotPOD _ZFP_ZFPropertyRawInitStepInvoker_##Name \
        { \
        public: \
            static inline void action(ZF_IN T_Owner *owner, ZF_IN_OUT Type &propertyValue) \
            { \
            } \
        }; \
        template<typename T_Owner> \
        zfclassNotPOD _ZFP_ZFPropertyRawInitStepInvoker_##Name<T_Owner, 1> \
        { \
        public: \
            static inline void action(ZF_IN T_Owner *owner, ZF_IN_OUT Type &propertyValue) \
            { \
                owner->_ZFP_ZFPropertyRawInitStep_##Name(propertyValue); \
            } \
        }; \
        /** @endcond */ \
    public: \
        zfbool _ZFP_ZFPropertyCallbackIsInitValueActionInternal_##Name(ZF_OUT_OPT void *outInitValue) \
        { \
            if(Name##_ZFPropertyValue.propertyAccessed()) \
            { \
                zfself::_ZFP_ZFPropertyValue_##Name<Type> _holder; \
                if(outInitValue != zfnull) \
                { \
                    *(Type *)outInitValue = _holder.propertyInit(this, zffalse); \
                } \
                return (ZFComparerDefault( \
                        this->_ZFP_ZFPROPERTY_GETTER_NAME_INTERNAL(Type, Name)(), _holder.propertyInit(this, zffalse)) \
                    == ZFCompareTheSame); \
            } \
            else \
            { \
                return zftrue; \
            } \
        } \
    public:

/**
 * @brief declare raw init step for property
 *
 * usage:
 * @code
 *   zfclass Owner : zfextends ZFObject
 *   {
 *       ZFOBJECT_DECLARE(Owner, ZFObject)
 *
 *       ZFPROPERTY_RETAIN_READONLY(MyType *, myValue, ZFPropertyInitValue(myInitValue))
 *       ZFPROPERTY_RAW_INIT_STEP_RETAIN_DECLARE(MyType *, myValue)
 *       {
 *           propertyValue = myInitValue2;
 *       }
 *   }
 * @endcode
 *
 * -  only the direct owner of the property can declare raw init value,
 *   and it can not be overrided
 * -  the property's init order:
 *   -# #ZFPropertyInitValue, can not be overrided
 *   -# #ZFPROPERTY_RAW_INIT_STEP_RETAIN_DECLARE, can not be overrided
 *   -# #ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN, can be overrided
 * -  comparing to other init method,
 *   this method is designed for static and complex init step
 */
#define ZFPROPERTY_RAW_INIT_STEP_RETAIN_DECLARE(Type, Name) \
    public: \
        static void _ZFP_ZFPropertyRawInitStep_##Name(ZF_IN_OUT zfautoObject &propertyValue)
/** @brief see #ZFPROPERTY_RAW_INIT_STEP_RETAIN_DECLARE */
#define ZFPROPERTY_RAW_INIT_STEP_RETAIN_DEFINE(OwnerClass, Type, Name) \
    void OwnerClass::_ZFP_ZFPropertyRawInitStep_##Name(ZF_IN_OUT zfautoObject &propertyValue)

/** @brief see #ZFPROPERTY_RAW_INIT_STEP_RETAIN_DECLARE */
#define ZFPROPERTY_RAW_INIT_STEP_ASSIGN_DECLARE(Type, Name) \
    public: \
        static void _ZFP_ZFPropertyRawInitStep_##Name(ZF_IN_OUT Type &propertyValue)
/** @brief see #ZFPROPERTY_RAW_INIT_STEP_RETAIN_DECLARE */
#define ZFPROPERTY_RAW_INIT_STEP_ASSIGN_DEFINE(OwnerClass, Type, Name) \
    void OwnerClass::_ZFP_ZFPropertyRawInitStep_##Name(ZF_IN_OUT Type &propertyValue)

#define _ZFP_ZFPROPERTY_DECLARE_CALLBACK_IS_INIT_VALUE(Type, Name) \
    private: \
        static zfbool _ZFP_ZFPropertyCallbackIsValueAccessed_##Name(ZF_IN const ZFProperty *property, \
                                                                    ZF_IN ZFObject *ownerObj) \
        { \
            return ZFCastZFObjectUnchecked(zfself *, ownerObj)->_ZFP_ZFPropertyCallbackIsValueAccessedAction_##Name(); \
        } \
        zfbool _ZFP_ZFPropertyCallbackIsValueAccessedAction_##Name(void) \
        { \
            return Name##_ZFPropertyValue.propertyAccessed(); \
        } \
        static zfbool _ZFP_ZFPropertyCallbackIsInitValue_##Name(ZF_IN const ZFProperty *property, \
                                                                ZF_IN ZFObject *ownerObj, \
                                                                ZF_OUT_OPT void *outInitValue) \
        { \
            return ZFCastZFObjectUnchecked(zfself *, ownerObj)->_ZFP_ZFPropertyCallbackIsInitValueAction_##Name(outInitValue); \
        } \
        virtual zfbool _ZFP_ZFPropertyCallbackIsInitValueAction_##Name(ZF_OUT_OPT void *outInitValue) \
        { \
            return _ZFP_ZFPropertyCallbackIsInitValueActionWrapper_##Name<zfself, \
                    ZFM_CLASS_HAS_MEMBER(_, _ZFP_ZFPropertyCallbackIsInitValueActionCustom_##Name, zfself) \
                >::action(this, outInitValue); \
        } \
        ZFM_CLASS_HAS_MEMBER_DECLARE(_, _ZFP_ZFPropertyCallbackIsInitValueActionCustom_##Name, zfbool (T::*F)(void *)) \
        template<typename T_Owner, int hasCustom> \
        zfclassNotPOD _ZFP_ZFPropertyCallbackIsInitValueActionWrapper_##Name \
        { \
        public: \
            static inline zfbool action(ZF_IN T_Owner *owner, ZF_OUT_OPT void *outInitValue) \
            { \
                return owner->_ZFP_ZFPropertyCallbackIsInitValueActionInternal_##Name(outInitValue); \
            } \
        }; \
        template<typename T_Owner> \
        zfclassNotPOD _ZFP_ZFPropertyCallbackIsInitValueActionWrapper_##Name<T_Owner, 1> \
        { \
        public: \
            static inline zfbool action(ZF_IN T_Owner *owner, ZF_OUT_OPT void *outInitValue) \
            { \
                return owner->_ZFP_ZFPropertyCallbackIsInitValueActionCustom_##Name(outInitValue); \
            } \
        };
#define _ZFP_ZFPROPERTY_DECLARE_CALLBACK_RESET_INIT_VALUE(Type, Name) \
    private: \
        static void _ZFP_ZFPropertyCallbackResetInitValue_##Name(ZF_IN const ZFProperty *property, \
                                                                 ZF_IN ZFObject *ownerObj) \
        { \
            zfself *owner = ownerObj->to<zfself *>(); \
            owner->Name##_ZFPropertyValue.propertyReset(owner); \
        } \
    public:

#define _ZFP_ZFPROPERTY_DECLARE_CALLBACK_COMPARE(Type, Name) \
    private: \
        static ZFCompareResult _ZFP_ZFPropertyCallbackCompare_##Name(ZF_IN const ZFProperty *property, \
                                                                     ZF_IN ZFObject *obj0, \
                                                                     ZF_IN ZFObject *obj1) \
        { \
            return ZFCastZFObjectUnchecked(zfself *, obj0)->_ZFP_ZFPropertyCallbackCompareAction_##Name(obj1); \
        } \
        virtual ZFCompareResult _ZFP_ZFPropertyCallbackCompareAction_##Name(ZF_IN ZFObject *anotherObj) \
        { \
            return _ZFP_ZFPropertyCallbackCompareActionWrapper_##Name<zfself, \
                    ZFM_CLASS_HAS_MEMBER(_, _ZFP_ZFPropertyCallbackCompareActionCustom_##Name, zfself) \
                >::action(this, anotherObj); \
        } \
        ZFM_CLASS_HAS_MEMBER_DECLARE(_, _ZFP_ZFPropertyCallbackCompareActionCustom_##Name, ZFCompareResult (T::*F)(ZF_IN ZFObject *)) \
        template<typename T_Owner, int hasCustom> \
        zfclassNotPOD _ZFP_ZFPropertyCallbackCompareActionWrapper_##Name \
        { \
        public: \
            static inline ZFCompareResult action(ZF_IN T_Owner *owner, ZF_IN ZFObject *anotherObj) \
            { \
                return owner->_ZFP_ZFPropertyCallbackCompareActionInternal_##Name(anotherObj); \
            } \
        }; \
        template<typename T_Owner> \
        zfclassNotPOD _ZFP_ZFPropertyCallbackCompareActionWrapper_##Name<T_Owner, 1> \
        { \
        public: \
            static inline ZFCompareResult action(ZF_IN T_Owner *owner, ZF_IN ZFObject *anotherObj) \
            { \
                return owner->_ZFP_ZFPropertyCallbackCompareActionCustom_##Name(anotherObj); \
            } \
        }; \
    public: \
        virtual ZFCompareResult _ZFP_ZFPropertyCallbackCompareActionInternal_##Name(ZF_IN ZFObject *anotherObj) \
        { \
            return ZFComparerDefault( \
                this->_ZFP_ZFPROPERTY_GETTER_NAME(Type, Name)(), \
                ZFCastZFObjectUnchecked(zfself *, anotherObj)->_ZFP_ZFPROPERTY_GETTER_NAME(Type, Name)()); \
        }

#define _ZFP_ZFPROPERTY_DECLARE_CALLBACK_COPY(Type, Name) \
    private: \
        static void _ZFP_ZFPropertyCallbackCopy_##Name(ZF_IN const ZFProperty *property, \
                                                       ZF_IN ZFObject *dstObj, \
                                                       ZF_IN ZFObject *srcObj) \
        { \
            property->setterMethod()->execute<void, Type const &>(dstObj, \
                property->getterMethod()->execute<Type const &>(srcObj)); \
        }
#define _ZFP_ZFPROPERTY_DECLARE_CALLBACK_RETAIN(Type, Name) \
    private: \
        static void _ZFP_ZFPropertyCallbackRetainSet_##Name(ZF_IN const ZFProperty *property, \
                                                            ZF_IN ZFObject *dstObj, \
                                                            ZF_IN ZFObject *src) \
        { \
            property->setterMethod()->execute<void, Type const &>(dstObj, ZFCastZFObjectUnchecked(Type, src)); \
        } \
        static ZFObject *_ZFP_ZFPropertyCallbackRetainGet_##Name(ZF_IN const ZFProperty *property, \
                                                                 ZF_IN ZFObject *ownerObj) \
        { \
            return ZFCastZFObjectUnchecked(ZFObject *, property->getterMethod()->execute<Type const &>(ownerObj)); \
        } \
        static void _ZFP_ZFPropertyCallbackSetup_##Name(ZFProperty *propertyInfo) \
        { \
            propertyInfo->callbackRetainSet = zfself::_ZFP_ZFPropertyCallbackRetainSet_##Name; \
            propertyInfo->callbackRetainGet = zfself::_ZFP_ZFPropertyCallbackRetainGet_##Name; \
            propertyInfo->callbackGetInfo = _ZFP_ZFPropertyCallbackGetInfoRetainDefault; \
        }
#define _ZFP_ZFPROPERTY_DECLARE_CALLBACK_ASSIGN(Type, Name) \
    private: \
        static void _ZFP_ZFPropertyCallbackAssignSet_##Name(ZF_IN const ZFProperty *property, \
                                                            ZF_IN ZFObject *dstObj, \
                                                            ZF_IN void *src) \
        { \
            property->setterMethod()->execute<void, Type const &>(dstObj, *(ZFCastStatic(Type *, src))); \
        } \
        static const void *_ZFP_ZFPropertyCallbackAssignGet_##Name(ZF_IN const ZFProperty *property, \
                                                                   ZF_IN ZFObject *ownerObj) \
        { \
            return &(property->getterMethod()->execute<Type const &>(ownerObj)); \
        } \
        static void _ZFP_ZFPropertyCallbackGetInfoAssign_##Name(ZF_IN const ZFProperty *property, \
                                                                ZF_IN ZFObject *ownerObj, \
                                                                ZF_IN_OUT zfstring &ret) \
        { \
            ZFCoreElementInfoGetter<Type>::elementInfoGetter(ret, \
                property->getterMethod()->execute<Type const &>(ownerObj)); \
        } \
        static void _ZFP_ZFPropertyCallbackSetup_##Name(ZFProperty *propertyInfo) \
        { \
            propertyInfo->callbackAssignSet = zfself::_ZFP_ZFPropertyCallbackAssignSet_##Name; \
            propertyInfo->callbackAssignGet = zfself::_ZFP_ZFPropertyCallbackAssignGet_##Name; \
            propertyInfo->callbackGetInfo = zfself::_ZFP_ZFPropertyCallbackGetInfoAssign_##Name; \
        }

#define _ZFP_ZFPROPERTY_DECLARE_RETAIN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                       ZFPropertyInitValueOrNoInitValue) \
    _ZFP_ZFPROPERTY_DECLARE(Type, ZFPropertyTypeId_noneOrType, Name, \
                            zftTraitsType<Type>::TraitsType::ClassData()) \
    _ZFP_ZFPROPERTY_VALUE_DECLARE_RETAIN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                         ZFPropertyInitValueOrNoInitValue) \
    _ZFP_ZFPROPERTY_DECLARE_CALLBACK_RETAIN(Type, Name)
#define _ZFP_ZFPROPERTY_DECLARE_ASSIGN_CAST_EXPAND(v) \
    v
#define _ZFP_ZFPROPERTY_DECLARE_ASSIGN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                       ZFPropertyInitValueOrNoInitValue) \
    _ZFP_ZFPROPERTY_DECLARE(Type, ZFPropertyTypeId_noneOrType, Name, \
                            zfnull) \
    _ZFP_ZFPROPERTY_VALUE_DECLARE_ASSIGN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                         ZFPropertyInitValueOrNoInitValue) \
    _ZFP_ZFPROPERTY_DECLARE_CALLBACK_ASSIGN(Type, Name)

// ============================================================
#define _ZFP_ZFPROPERTY_VALUE_REF(Name) \
    (Name##_ZFPropertyValue.propertyInit(this))

#define _ZFP_ZFPROPERTY_SETTER_NAME(Type, Name) \
    Name##Set
#define _ZFP_ZFPROPERTY_SETTER_NAME_INTERNAL(Type, Name) \
    Name##SetInternal

#define _ZFP_ZFPROPERTY_GETTER_NAME(Type, Name) \
    Name
#define _ZFP_ZFPROPERTY_GETTER_NAME_INTERNAL(Type, Name) \
    Name##Internal

// ============================================================
#define _ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE_public protected
#define _ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE_protected protected
#define _ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE_private private
#define _ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE_(ForAccessType) \
    _ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE_##ForAccessType
#define _ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE(ForAccessType) \
    _ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE_(ForAccessType)

// ============================================================
#define _ZFP_ZFPROPERTY_SETTER_INTERNAL_RETAIN(AccessType, Type, Name) \
    AccessType: \
        inline void _ZFP_ZFPROPERTY_SETTER_NAME_INTERNAL(Type, Name)(Type const &newValue) \
        { \
            toObject()->_ZFP_ZFObject_objectPropertyValueBeforeChange(zfself::_ZFP_ZFProperty_##Name()); \
            _ZFP_ZFPROPERTY_VALUE_REF(Name) = newValue; \
            ZFCoreMutexLock(); \
            this->Name##_ZFPropertyValue.valueHolder = \
                zflockfree_zfautoObjectCreate(ZFObjectToObject(newValue)); \
            ZFCoreMutexUnlock(); \
            toObject()->_ZFP_ZFObject_objectPropertyValueAfterChange(zfself::_ZFP_ZFProperty_##Name()); \
        }
#define _ZFP_ZFPROPERTY_SETTER_INTERNAL_ASSIGN(AccessType, Type, Name) \
    AccessType: \
        inline void _ZFP_ZFPROPERTY_SETTER_NAME_INTERNAL(Type, Name)(Type const &newValue) \
        { \
            toObject()->_ZFP_ZFObject_objectPropertyValueBeforeChange(zfself::_ZFP_ZFProperty_##Name()); \
            _ZFP_ZFPROPERTY_VALUE_REF(Name) = newValue; \
            toObject()->_ZFP_ZFObject_objectPropertyValueAfterChange(zfself::_ZFP_ZFProperty_##Name()); \
        }
#define _ZFP_ZFPROPERTY_GETTER_INTERNAL(AccessType, Type, Name) \
    AccessType: \
        inline Type const &_ZFP_ZFPROPERTY_GETTER_NAME_INTERNAL(Type, Name)(void) \
        { \
            return _ZFP_ZFPROPERTY_VALUE_REF(Name); \
        }

// ============================================================
#define _ZFP_ZFPROPERTY_SETTER(AccessType, Type, Name) \
    AccessType: \
        ZFMETHOD_DECLARE_NO_AUTOREGISTER_1( \
            AccessType, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
            void, _ZFP_ZFPROPERTY_SETTER_NAME(Type, Name), ZFMethodNoId, \
            Type const &, newValue, ZFMethodNoDefaultParam) \
        { \
            _ZFP_ZFPropertySetterWrapper_##Name<zfself, \
                    ZFM_CLASS_HAS_MEMBER(_, _ZFP_ZFPropertySetterCustom_##Name, zfself) \
                >::action(this, newValue); \
        } \
    private: \
        ZFM_CLASS_HAS_MEMBER_DECLARE(_, _ZFP_ZFPropertySetterCustom_##Name, void (T::*F)(ZF_IN Type const &)) \
        template<typename T_Owner, int hasCustom> \
        zfclassNotPOD _ZFP_ZFPropertySetterWrapper_##Name \
        { \
        public: \
            static inline void action(ZF_IN T_Owner *owner, ZF_IN Type const &newValue) \
            { \
                owner->_ZFP_ZFPropertySetterInternal_##Name(newValue); \
            } \
        }; \
        template<typename T_Owner> \
        zfclassNotPOD _ZFP_ZFPropertySetterWrapper_##Name<T_Owner, 1> \
        { \
        public: \
            static inline void action(ZF_IN T_Owner *owner, ZF_IN Type const &newValue) \
            { \
                owner->_ZFP_ZFPropertySetterCustom_##Name(newValue); \
            } \
        }; \
    public: \
        inline void _ZFP_ZFPropertySetterInternal_##Name(ZF_IN Type const &newValue) \
        { \
            this->_ZFP_ZFPROPERTY_SETTER_NAME_INTERNAL(Type, Name)(newValue); \
        }
#define _ZFP_ZFPROPERTY_GETTER(AccessType, Type, Name) \
    AccessType: \
        ZFMETHOD_DECLARE_NO_AUTOREGISTER_0( \
            AccessType, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
            Type const &, _ZFP_ZFPROPERTY_GETTER_NAME(Type, Name), ZFMethodNoId) \
        { \
            return _ZFP_ZFPropertyGetterWrapper_##Name<zfself, \
                    ZFM_CLASS_HAS_MEMBER(_, _ZFP_ZFPropertyGetterCustom_##Name, zfself) \
                >::action(this); \
        } \
    private: \
        ZFM_CLASS_HAS_MEMBER_DECLARE(_, _ZFP_ZFPropertyGetterCustom_##Name, Type const & (T::*F)(void)) \
        template<typename T_Owner, int hasCustom> \
        zfclassNotPOD _ZFP_ZFPropertyGetterWrapper_##Name \
        { \
        public: \
            static inline Type const &action(ZF_IN T_Owner *owner) \
            { \
                return owner->_ZFP_ZFPropertyGetterInternal_##Name(); \
            } \
        }; \
        template<typename T_Owner> \
        zfclassNotPOD _ZFP_ZFPropertyGetterWrapper_##Name<T_Owner, 1> \
        { \
        public: \
            static inline Type const &action(ZF_IN T_Owner *owner) \
            { \
                return owner->_ZFP_ZFPropertyGetterCustom_##Name(); \
            } \
        }; \
    public: \
        inline Type const &_ZFP_ZFPropertyGetterInternal_##Name(void) \
        { \
            return this->_ZFP_ZFPROPERTY_GETTER_NAME_INTERNAL(Type, Name)(); \
        }

// ============================================================
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPropertyNoInitValue
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPropertyInitValue(v) v

// ============================================================
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_RETAIN_NOT_SERIALIZABLE( \
    Type, Name, \
    ZFPropertyInitValueOrNoInitValue, \
    SetterAccessType, \
    GetterAccessType) \
        _ZFP_ZFPROPERTY_GETTER(GetterAccessType, Type, Name) \
        /** @brief internal getter, see @ref Name */ \
        _ZFP_ZFPROPERTY_GETTER_INTERNAL(_ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE(GetterAccessType), Type, Name) \
        /** @brief see @ref Name */ \
        _ZFP_ZFPROPERTY_SETTER(SetterAccessType, Type, Name) \
        /** @brief internal getter, see @ref Name */ \
        _ZFP_ZFPROPERTY_SETTER_INTERNAL_RETAIN(_ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE(SetterAccessType), Type, Name) \
        _ZFP_ZFPROPERTY_DECLARE_RETAIN(Type, ZFPropertyTypeId_none, Name, \
                                       ZFPropertyInitValueOrNoInitValue) \
        public:
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_RETAIN_DETAIL( \
    Type, Name, \
    ZFPropertyInitValueOrNoInitValue, \
    SetterAccessType, \
    GetterAccessType) \
        _ZFP_ZFPROPERTY_GETTER(GetterAccessType, Type, Name) \
        /** @brief internal getter, see @ref Name */ \
        _ZFP_ZFPROPERTY_GETTER_INTERNAL(_ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE(GetterAccessType), Type, Name) \
        /** @brief see @ref Name */ \
        _ZFP_ZFPROPERTY_SETTER(SetterAccessType, Type, Name) \
        /** @brief internal getter, see @ref Name */ \
        _ZFP_ZFPROPERTY_SETTER_INTERNAL_RETAIN(_ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE(SetterAccessType), Type, Name) \
        _ZFP_ZFPROPERTY_DECLARE_RETAIN(Type, _ZFP_ZFPropertyTypeId_ZFObject, Name, \
                                       ZFPropertyInitValueOrNoInitValue) \
        public:
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_RETAIN_WITH_INIT( \
    Type, Name, \
    ZFPropertyInitValueOrNoInitValue) \
    ZFPROPERTY_RETAIN_DETAIL( \
        Type, Name, \
        ZFPropertyInitValueOrNoInitValue, \
        public, \
        public)
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_RETAIN_READONLY(Type, Name, ZFPropertyInitValueOrNoInitValue) \
    ZFPROPERTY_RETAIN_DETAIL( \
        Type, Name, \
        ZFPropertyInitValueOrNoInitValue, \
        private, \
        public)
/**
 * @brief declare a retain property
 *
 * -  ZFPROPERTY_XXX is similar to \@property in Object-C,
 *   which would automatically generate setter and getter for you\n
 *   here's a list of what they'll generate
 *   (assume property's type is Type and property's name is Name):
 *   -  ZFPROPERTY_RETAIN / ZFPROPERTY_ASSIGN:
 *     -  public:\n
 *       virtual void NameSet(Type const &newValue); // (reflectable)\n
 *       virtual Type const &Name(void); // (reflectable)
 *     -  protected:\n
 *       inline void NameSetInternal(Type const &newValue);\n
 *       inline Type const &NameInternal(void);
 *   -  for a retain property, Type must be ZFObject or ZFObject's subclass,
 *     and value is set by #ZFPropertyChange
 *   -  for a assign property, Type could be any type except for const type
 * -  ZFProperty support those type only:
 *   -  base type, such as zfcharA, int, const zfcharA *
 *   -  ZFObject *
 *   -  classes that have constructor with no argument and have operator= support
 * -  ZFProperty is also reflectable,
 *   you may reflect the ZFProperty itself by #ZFClass::propertyForName,
 *   or reflect the setter and getter as ZFMethod,
 *   for how to reflect, see #ZFMethod\n
 *   also, similar to ZFMethod, if you want the static binding for ZFProperty,
 *   use ZFPROPERTY_REGISTER,
 *   however, since a property is valid only if a instance has created,
 *   it's not quite necessary to register it
 * -  you may customize the accessibility for getter and setter,
 *   or add default value for setter,
 *   or set your custom setter,
 *   by using ZFPROPERTY_XXX_DETAIL:\n
 *   ZFPROPERTY_RETAIN_DETAIL(
 *       Type, Name, ZFPropertyInitValueOrNoInitValue,
 *       SetterAccessType,
 *       GetterAccessType)\n
 *   the ZFPropertyInitValueOrNoInitValue is the init value for the property,
 *     could be ZFPropertyInitValue(your_value) or ZFPropertyNoInitValue,
 *     if not set, the built-in value would be set,
 *     e.g. zffalse for zfbool, empty string for zfstring
 *     (for retain property, the init value must be auto released object,
 *     use #zfautoObject or #zflineAlloc or #zflineRelease is recommended)\n
 *   the SetterAccessType/GetterAccessType could be
 *     public/protected/private\n
 *   for example:
 *   @code
 *     zfclass OwnerClass : zfextends Base
 *     {
 *         ZFOBJECT_DECLARE(OwnerClass, Base)
 *
 *         ZFPROPERTY_RETAIN_DETAIL(
 *             zfstring, ZFPropertyTypeId_zfstring, StringProperty, \
 *             ZFPropertyNoInitValue,
 *             public,
 *             public)
 *
 *         ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfstring, StringProperty)
 *         {
 *             ...; // you may change newValue here or do your custom task
 *             this->StringPropertySetInternal(newValue); // call internal setter to setup new value
 *         }
 *     };
 *   @endcode
 *   @note for a retain property, it'll be released automatically when it's owner is dealloced
 * -  to override a property in subclass, you should use
 *   #ZFPROPERTY_OVERRIDE_SETTER_DECLARE and/or #ZFPROPERTY_OVERRIDE_GETTER_DECLARE
 *   @code
 *     // OwnerClass.h
 *     zfclass OwnerClass : zfextends Base
 *     {
 *         ...
 *         ZFPROPERTY_OVERRIDE_SETTER_DECLARE(zfcharA, OverridePropertyInHeader)
 *         { // actual setter here, defined in header file
 *             // use zfsuper::XXXSet to set
 *             zfsuper::OverridePropertyInHeaderSet(newValue);
 *         }
 *         ZFPROPERTY_OVERRIDE_SETTER_DECLARE(zfcharA, OverridePropertyInCppFile);
 *     };
 *
 *     // OwnerClass.cpp
 *     ZFPROPERTY_OVERRIDE_SETTER_DEFINE(OwnerClass, zfcharA, OverridePropertyInCppFile)
 *     {
 *         // setter that defined in cpp file
 *         OverridePropertyInCppFileSet(newValue);
 *     }
 *   @endcode
 *   @warning you must not declare two property with same name in child and base class
 *   @note overrided property won't be included in it's ZFClass,
 *     it's only a method override, no new ZFMethod or ZFProperty would be declared in child class
 *   @warning AccessType and Type for the overrided property must be the same
 *     declared in base class
 *   @warning you should never override a property's getter and setter except by
 *     ZFPROPERTY_OVERRIDE_XXX
 * -  to add Doxygen docs, you should:
 *   @code
 *    / **
 *      * @brief without fn defines, doc for getters only (ie. Name)
 *      *
 *      * setters' docs would be auto generated,
 *      * which is referencing to getters
 *      * /
 *     ZFPROPERTY_RETAIN(Type, Name)
 *   @endcode
 *   or:
 *   @code
 *    / **
 *      * @fn NameSet
 *      * @brief doc for NameSet
 *      * @fn Name
 *      * @brief doc for Name
 *      * /
 *     ZFPROPERTY_RETAIN(Type, Name)
 *   @endcode
 *
 * \n
 * ADVANCED:\n
 * there's a param named ZFPropertyTypeId_noneOrType for assign type property (by #ZFPROPERTY_ASSIGN_DETAIL_WITH_CUSTOM_TYPEID),
 * it's used for advanced property copy operation,
 * such as #ZFPropertySerializeFrom,
 * which can set property from and to string without knowing the actual type,
 * if your property doesn't need or can't do auto setup,
 * you may set it as #ZFPropertyTypeId_none or your custom type id,
 * otherwise, you must make sure the type is correct
 */
#define ZFPROPERTY_RETAIN(Type, Name) \
    ZFPROPERTY_RETAIN_DETAIL( \
        Type, Name, \
        ZFPropertyNoInitValue, \
        public, \
        public)

// ============================================================
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN_DETAIL_WITH_CUSTOM_TYPEID( \
    Type, Name, \
    ZFPropertyInitValueOrNoInitValue, \
    SetterAccessType, \
    GetterAccessType, \
    ZFPropertyTypeId_noneOrType) \
        _ZFP_ZFPROPERTY_GETTER(GetterAccessType, Type, Name) \
        /** @brief internal getter, see @ref Name */ \
        _ZFP_ZFPROPERTY_GETTER_INTERNAL(_ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE(GetterAccessType), Type, Name) \
        /** @brief see @ref Name */ \
        _ZFP_ZFPROPERTY_SETTER(SetterAccessType, Type, Name) \
        /** @brief internal setter, see @ref Name */ \
        _ZFP_ZFPROPERTY_SETTER_INTERNAL_ASSIGN(_ZFP_ZFPROPERTY_INTERNAL_ACCESSTYPE(SetterAccessType), Type, Name) \
        _ZFP_ZFPROPERTY_DECLARE_ASSIGN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                       ZFPropertyInitValueOrNoInitValue) \
        public:
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE(Type, Name) \
    ZFPROPERTY_ASSIGN_DETAIL_WITH_CUSTOM_TYPEID( \
        Type, Name, \
        ZFPropertyNoInitValue, \
        public, \
        public, \
        ZFPropertyTypeId_none)
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN_DETAIL( \
    Type, Name, \
    ZFPropertyInitValueOrNoInitValue, \
    SetterAccessType, \
    GetterAccessType) \
    ZFPROPERTY_ASSIGN_DETAIL_WITH_CUSTOM_TYPEID( \
        Type, Name, \
        ZFPropertyInitValueOrNoInitValue, \
        SetterAccessType, \
        GetterAccessType, \
        ZFPropertyTypeId_##Type)
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN_WITH_INIT( \
    Type, Name, \
    ZFPropertyInitValueOrNoInitValue) \
    ZFPROPERTY_ASSIGN_DETAIL( \
        Type, Name, \
        ZFPropertyInitValueOrNoInitValue, \
        public, \
        public)
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN_READONLY(Type, Name, ZFPropertyInitValueOrNoInitValue) \
    ZFPROPERTY_ASSIGN_DETAIL( \
        Type, Name, \
        ZFPropertyInitValueOrNoInitValue, \
        private, \
        public)
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN(Type, Name) \
    ZFPROPERTY_ASSIGN_DETAIL( \
        Type, Name, \
        ZFPropertyNoInitValue, \
        public, \
        public)

// ============================================================
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_CUSTOM_INIT_VALUE_CHECKER_DECLARE(Type, Name) \
    public: \
        zfbool _ZFP_ZFPropertyCallbackIsInitValueActionCustom_##Name(ZF_OUT_OPT void *outInitValue)
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_CUSTOM_INIT_VALUE_CHECKER_DEFINE(OwnerClass, Type, Name) \
    zfbool OwnerClass::_ZFP_ZFPropertyCallbackIsInitValueActionCustom_##Name(ZF_OUT_OPT void *outInitValue)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_CUSTOM_VALUE_COMPARER_DECLARE(Type, Name) \
    public: \
        virtual ZFCompareResult _ZFP_ZFPropertyCallbackCompareActionCustom_##Name(ZF_IN ZFObject *anotherObj)
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_CUSTOM_VALUE_COMPARER_DEFINE(OwnerClass, Type, Name) \
    ZFCompareResult OwnerClass::_ZFP_ZFPropertyCallbackCompareActionCustom_##Name(ZF_IN ZFObject *anotherObj)
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_CUSTOM_VALUE_COMPARER_ACCESS_ANOTHER_VALUE(Type, Name) \
    (ZFCastZFObjectUnchecked(zfself *, anotherObj)->_ZFP_ZFPROPERTY_GETTER_NAME(Type, Name)())

// ============================================================
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_CUSTOM_SETTER_DECLARE(Type, Name) \
    public: \
        void _ZFP_ZFPropertySetterCustom_##Name(ZF_IN Type const &newValue)
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_CUSTOM_SETTER_DEFINE(OwnerClass, Type, Name) \
    void OwnerClass::_ZFP_ZFPropertySetterCustom_##Name(ZF_IN Type const &newValue)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_CUSTOM_GETTER_DECLARE(Type, Name) \
    public: \
        Type const &_ZFP_ZFPropertyGetterCustom_##Name(void)
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_CUSTOM_GETTER_DEFINE(OwnerClass, Type, Name) \
    Type const &OwnerClass::_ZFP_ZFPropertyGetterCustom_##Name(void)

// ============================================================
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_OVERRIDE_SETTER_DECLARE(AccessType, Type, Name) \
    ZFMETHOD_OVERRIDE_DECLARE_DETAIL_1( \
        AccessType, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
        void, _ZFP_ZFPROPERTY_SETTER_NAME(Type, Name), ZFMethodNoId, \
        Type const &, newValue, ZFMethodNoDefaultParam)
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_OVERRIDE_SETTER_DEFINE(OwnerClass, Type, Name) \
    ZFMETHOD_OVERRIDE_DEFINE_1( \
        OwnerClass, void, _ZFP_ZFPROPERTY_SETTER_NAME(Type, Name), \
        Type const &, newValue, ZFMethodNoDefaultParam)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_OVERRIDE_GETTER_DECLARE(AccessType, Type, Name) \
    ZFMETHOD_OVERRIDE_DECLARE_DETAIL_0( \
        AccessType, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
        Type const &, _ZFP_ZFPROPERTY_GETTER_NAME(Type, Name), ZFMethodNoId)
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_OVERRIDE_GETTER_DEFINE(OwnerClass, Type, Name) \
    ZFMETHOD_OVERRIDE_DEFINE_0( \
        OwnerClass, Type const &, _ZFP_ZFPROPERTY_GETTER_NAME(Type, Name))

// ============================================================
#define _ZFP_ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE(TypeFix, Type, Name, autoInitOrNot) \
    private: \
        template<typename T_OwnerZFObject> \
        zfclassLikePOD _ZFP_ZFPropertyInitStepAutoRegister_##Name \
        { \
        public: \
            _ZFP_ZFPropertyInitStepAutoRegister_##Name(void) \
            { \
                static zfbool needInit = zftrue; \
                if(needInit) \
                { \
                    needInit = zffalse; \
                    T_OwnerZFObject::ClassData()->_ZFP_ZFClass_propertyInitStepRegister( \
                        ZFPropertyAccess(T_OwnerZFObject, Name), \
                        T_OwnerZFObject::_ZFP_ZFPropertyInitStepInvoker_##Name, \
                        autoInitOrNot); \
                } \
            } \
        }; \
        _ZFP_ZFPropertyInitStepAutoRegister_##Name<zfself> _ZFP_ZFPropertyInitStepAutoRegisterInstance_##Name; \
        static void _ZFP_ZFPropertyInitStepInvoker_##Name(ZF_IN const ZFProperty *property, \
                                                          ZF_IN ZFObject *obj, \
                                                          ZF_IN_OUT void *valueAddr) \
        { \
            TypeFix &v = *ZFCastReinterpret(TypeFix *, valueAddr); \
            ZFCastZFObjectUnchecked(zfself *, obj)->zfself::_ZFP_ZFPropertyInitStep_##Name(v); \
        } \
    public: \
        zffinal void _ZFP_ZFPropertyInitStep_##Name(ZF_IN_OUT TypeFix &propertyValue)

/**
 * @brief override property's custom init step, use with caution
 *
 * proto type:\n
 *   void customInitStep(ZF_IN_OUT Type &value)\n
 * \n
 * @code
 *   ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN(Type, Name)
 *   {
 *       // change your own init value
 *       propertyValue = yourNewValue;
 *   }
 * @endcode
 * @note once overrided by this method,
 *   the property is ensured to be initialized during object's init step (before #ZFObject::objectOnInit),
 *   and setter is ensured called during init
 * @note if more than one class override the init step in the inherit tree,
 *   it's ensured only the last overrided child's would be called,
 *   and you may use #ZFPROPERTY_OVERRIDE_INIT_STEP_CALLSUPER to call super's init action
 */
#define ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN(Type, Name) \
    _ZFP_ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE(Type, Type, Name, zftrue)
/** @brief see #ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN */
#define ZFPROPERTY_OVERRIDE_INIT_STEP_DEFINE_ASSIGN(OwnerClass, Type, Name) \
    void OwnerClass::_ZFP_ZFPropertyInitStep_##Name(ZF_IN_OUT Type &propertyValue)

/** @brief see #ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN */
#define ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN(Type, Name) \
    _ZFP_ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE(zfautoObject, Type, Name, zftrue)
/** @brief see #ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN */
#define ZFPROPERTY_OVERRIDE_INIT_STEP_DEFINE_RETAIN(OwnerClass, Type, Name) \
    void OwnerClass::_ZFP_ZFPropertyInitStep_##Name(ZF_IN_OUT zfautoObject &propertyValue)

/** @brief see #ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN */
#define ZFPROPERTY_OVERRIDE_INIT_STEP_CALLSUPER(SuperClass, Type, Name) \
    this->to<SuperClass *>()->SuperClass::_ZFP_ZFPropertyInitStep_##Name(propertyValue)

/**
 * @brief util macro for #ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN
 */
#define ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(Type, Name, customInitValue) \
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN(Type, Name) \
    { \
        propertyValue = customInitValue; \
    }
/**
 * @brief util macro for #ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN
 */
#define ZFPROPERTY_OVERRIDE_INIT_VALUE_RETAIN(Type, Name, customInitValue) \
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN(Type, Name) \
    { \
        propertyValue = zflockfree_zfautoObjectCreate(customInitValue); \
    }

// ============================================================
/** @brief see #ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN */
#define ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN_NO_AUTO_INIT(Type, Name) \
    _ZFP_ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE(Type, Type, Name, zffalse)

/** @brief see #ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN */
#define ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(Type, Name) \
    _ZFP_ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE(zfautoObject, Type, Name, zffalse)

/**
 * @brief util macro for #ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN_NO_AUTO_INIT
 */
#define ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN_NO_AUTO_INIT(Type, Name, customInitValue) \
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN_NO_AUTO_INIT(Type, Name) \
    { \
        propertyValue = customInitValue; \
    }
/**
 * @brief util macro for #ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT
 */
#define ZFPROPERTY_OVERRIDE_INIT_VALUE_RETAIN_NO_AUTO_INIT(Type, Name, customInitValue) \
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(Type, Name) \
    { \
        propertyValue = zflockfree_zfautoObjectCreate(customInitValue); \
    }

// ============================================================
/**
 * @brief override property's init value checker
 *
 * proto type:\n
 *   zfbool isInitValue(void)
 */
#define ZFPROPERTY_OVERRIDE_INIT_VALUE_CHECKER_DECLARE(Type, Name) \
    private: \
        virtual zfbool _ZFP_ZFPropertyCallbackIsInitValueAction_##Name(ZF_OUT_OPT void *outInitValue)
/**
 * @brief see #ZFPROPERTY_OVERRIDE_INIT_VALUE_CHECKER_DECLARE
 */
#define ZFPROPERTY_OVERRIDE_INIT_VALUE_CHECKER_DEFINE(OwnerClass, Type, Name) \
    zfbool OwnerClass::_ZFP_ZFPropertyCallbackIsInitValueAction_##Name(ZF_OUT_OPT void *outInitValue)
/**
 * @brief see #ZFPROPERTY_OVERRIDE_INIT_VALUE_CHECKER_DECLARE
 */
#define ZFPROPERTY_OVERRIDE_INIT_VALUE_CHECKER_CALLSUPER(SuperOwnerClass, Type, Name) \
    SuperOwnerClass::_ZFP_ZFPropertyCallbackIsInitValueAction_##Name(outInitValue)

/**
 * @brief override property's value comparer
 */
#define ZFPROPERTY_OVERRIDE_VALUE_COMPARER_DECLARE(Type, Name) \
    private: \
        virtual ZFCompareResult _ZFP_ZFPropertyCallbackCompareAction_##Name(ZF_IN ZFObject *anotherObj)
/**
 * @brief see #ZFPROPERTY_OVERRIDE_VALUE_COMPARER_DECLARE
 */
#define ZFPROPERTY_OVERRIDE_VALUE_COMPARER_DEFINE(OwnerClass, Type, Name) \
    ZFCompareResult OwnerClass::_ZFP_ZFPropertyCallbackCompareAction_##Name(ZF_IN ZFObject *anotherObj)
/**
 * @brief see #ZFPROPERTY_OVERRIDE_VALUE_COMPARER_DECLARE
 */
#define ZFPROPERTY_OVERRIDE_VALUE_COMPARER_CALLSUPER(SuperOwnerClass, Type, Name) \
    SuperOwnerClass::_ZFP_ZFPropertyCallbackCompareAction_##Name(anotherObj)
/**
 * @brief see #ZFPROPERTY_OVERRIDE_VALUE_COMPARER_DECLARE
 */
#define ZFPROPERTY_OVERRIDE_VALUE_COMPARER_ACCESS_ANOTHER_VALUE(Type, Name) \
    (ZFCastZFObjectUnchecked(zfself *, anotherObj)->_ZFP_ZFPROPERTY_GETTER_NAME(Type, Name)())

// ============================================================
/**
 * @brief dummy macro to mark the property should not be reflected,
 *   see #ZFProperty::propertyReflectable
 */
#define ZFPROPERTY_NO_REFLECT(Type, Name) \
    private: \
        static void _ZFP_ZFPropertyNoReflect_##Name(void) {} \
    public:

// ============================================================
/**
 * @brief see #ZFPROPERTY_RETAIN
 *
 * statically register a property\n
 * you can find the property in its owner class only if it's registered,
 * which can be achieved by any of these:
 * -  any instance of owner class has ever been created,
 *   by default, #ZFClass would do the registration work for you automatically
 *   if the owner class is not an abstract class
 * -  ZFPROPERTY_REGISTER is declared
 */
#define ZFPROPERTY_REGISTER(OwnerClass, Type, Name) \
    ZF_STATIC_INITIALIZER_INIT(ZFPropertyRegister_##OwnerClass##_##Name) \
    { \
        (void)OwnerClass::_ZFP_ZFProperty_##Name(); \
    } \
    ZF_STATIC_INITIALIZER_END(ZFPropertyRegister_##OwnerClass##_##Name) \
    ZFMETHOD_REGISTER(OwnerClass, _ZFP_ZFPROPERTY_SETTER_NAME(Type, Name)) \
    ZFMETHOD_REGISTER(OwnerClass, _ZFP_ZFPROPERTY_GETTER_NAME(Type, Name))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyDeclareDef_h_

