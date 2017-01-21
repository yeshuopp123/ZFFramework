/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFPropertyTypeFwdDef.h
 * @brief type define for ZFProperty
 */

#ifndef _ZFI_ZFPropertyTypeFwdDef_h_
#define _ZFI_ZFPropertyTypeFwdDef_h_

#include "ZFPropertyFwdDef.h"
#include "zfautoObjectFwd.h"
#include "ZFAnyDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyTypeIdData_zfautoObjectCreate(ZF_OUT zfautoObject &ret, ZF_IN ZFObject *obj);
/**
 * @brief type data traits for #ZFPROPERTY_TYPE_DECLARE
 *
 * type must be registered by #ZFPROPERTY_TYPE_DECLARE before use this template,
 * for advanced reflection use only
 */
template<typename T_Type, typename T_ZFObject = void>
zfclassNotPOD ZFPropertyTypeIdData
{
public:
    /**
     * @brief whether this type already registered
     */
    static zfbool propertyReflectable(void)
    {
        return zffalse;
    }
    /**
     * @brief get property type id, or return #ZFPropertyTypeId_none if not registered
     */
    static const zfchar *propertyTypeId(void)
    {
        return ZFPropertyTypeId_none;
    }
    /**
     * @brief convert type from ZFObject, for advanced reflection use only
     *
     * this method used to convert types from/to ZFObject without knowing its actual type,
     * currently used by #ZFMethod::methodGenericInvoker\n
     * \n
     * types must be registered by #ZFPROPERTY_TYPE_DECLARE,
     * which would automatically declare a wrapper object type for you\n
     * assume your type's name is "YourType",
     * then there would be a wrapper class named "v_YourType",
     * with only one plain member variable named "zfv":
     * @code
     *   zfclass v_YourType : zfextends ZFObject
     *   {
     *       ZFOBJECT_DECLARE(v_YourType, ZFObject)
     *   public:
     *       YourType zfv;
     *   };
     * @endcode
     */
    static zfbool propertyConvertFromZFObject(ZF_OUT T_Type &v, ZF_IN ZFObject *obj)
    {
        return zffalse;
    }
    /**
     * @brief see #propertyConvertFromZFObject
     */
    static zfbool propertyConvertToZFObject(ZF_OUT zfautoObject &obj, ZF_IN T_Type const &v)
    {
        return zffalse;
    }
};
/** @cond ZFPrivateDoc */
ZFM_CLASS_HAS_MEMBER_DECLARE(_ZFP_ZFPropertyTypeIdData, ClassData, const ZFClass * (*F)(void))
template<typename T_Type>
zfclassNotPOD ZFPropertyTypeIdData<T_Type,
        typename zftEnableIf<ZFM_CLASS_HAS_MEMBER(_ZFP_ZFPropertyTypeIdData, ClassData, typename zftTraitsType<T_Type>::TraitsType)>::EnableIf
    >
{
public:
    static zfbool propertyReflectable(void)
    {
        return zftrue;
    }
    static const zfchar *propertyTypeId(void)
    {
        return ZFPropertyTypeId_none;
    }
    static zfbool propertyConvertFromZFObject(ZF_OUT T_Type &v, ZF_IN ZFObject *obj)
    {
        v = ZFCastZFObject(T_Type, obj);
        return zftrue;
    }
    static zfbool propertyConvertToZFObject(ZF_OUT zfautoObject &obj, ZF_IN T_Type const &v)
    {
        _ZFP_ZFPropertyTypeIdData_zfautoObjectCreate(obj, ZFCastZFObject(ZFObject *, v));
        return zftrue;
    }
};
template<typename T_Type>
zfclassNotPOD ZFPropertyTypeIdData<ZFAnyT<T_Type> >
{
public:
    static zfbool propertyReflectable(void)
    {
        return zftrue;
    }
    static const zfchar *propertyTypeId(void)
    {
        return ZFPropertyTypeId_none;
    }
    static zfbool propertyConvertFromZFObject(ZF_OUT ZFAnyT<T_Type> &v, ZF_IN ZFObject *obj)
    {
        v = ZFCastZFObject(T_Type, obj);
        return zftrue;
    }
    static zfbool propertyConvertToZFObject(ZF_OUT zfautoObject &obj, ZF_IN ZFAnyT<T_Type> const &v)
    {
        _ZFP_ZFPropertyTypeIdData_zfautoObjectCreate(obj, v.toObject());
        return zftrue;
    }
};
/** @endcond */

// ============================================================
#define _ZFP_ZFPROPERTY_TYPE_ID_DATA_DECLARE(TypeName, Type) \
    /** @brief type wrapper for #ZFPropertyTypeIdData::propertyConvertFromZFObject */ \
    zfclass ZF_ENV_EXPORT v_##TypeName : zfextends ZFObject \
    { \
        ZFOBJECT_DECLARE(v_##TypeName, ZFObject) \
    public: \
        /** @brief the value, see #ZFPropertyTypeIdData::propertyConvertFromZFObject */ \
        Type zfv; \
    public: \
        zfoverride \
        virtual ZFObject *objectOnInit(void) {return zfsuper::objectOnInit();} \
        /** @brief init with value */ \
        virtual ZFObject *objectOnInit(ZF_IN Type const &value) \
        { \
            this->objectOnInit(); \
            this->zfv = value; \
            return this; \
        } \
        zfoverride \
        virtual inline void objectInfoOnAppendTokenLeft(ZF_IN_OUT zfstring &ret) {} \
        zfoverride \
        virtual inline void objectInfoOnAppendTokenRight(ZF_IN_OUT zfstring &ret) {} \
        zfoverride \
        virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret); \
    }; \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFPropertyTypeIdData<Type> \
    { \
    public: \
        static zfbool propertyReflectable(void) \
        { \
            return zftrue; \
        } \
        static const zfchar *propertyTypeId(void) \
        { \
            return ZFPropertyTypeId_##TypeName(); \
        } \
        static zfbool propertyConvertFromZFObject(ZF_OUT Type &v, ZF_IN ZFObject *obj) \
        { \
            v_##TypeName *t = ZFCastZFObject(v_##TypeName *, obj); \
            if(t == zfnull) \
            { \
                return zffalse; \
            } \
            else \
            { \
                v = t->zfv; \
                return zftrue; \
            } \
        } \
        static zfbool propertyConvertToZFObject(ZF_OUT zfautoObject &obj, ZF_IN Type const &v) \
        { \
            ZFCoreMutexLock(); \
            v_##TypeName *t = zflockfree_zfAllocWithoutLeakTest(v_##TypeName); \
            t->zfv = v; \
            _ZFP_ZFPropertyTypeIdData_zfautoObjectCreate(obj, t); \
            zflockfree_zfReleaseWithoutLeakTest(t); \
            ZFCoreMutexUnlock(); \
            return zffalse; \
        } \
    }; \
    /** @endcond */

#define _ZFP_ZFPROPERTY_TYPE_ID_DATA_DEFINE(TypeName, Type) \
    ZFOBJECT_REGISTER(v_##TypeName) \
    void v_##TypeName::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) \
    { \
        ZFCoreElementInfoGetter<Type>::elementInfoGetter(ret, this->zfv); \
    }

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyTypeFwdDef_h_

