/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFPropertyDef.h
 * @brief property for ZFObject type
 */

#ifndef _ZFI_ZFPropertyDef_h_
#define _ZFI_ZFPropertyDef_h_

#include "ZFPropertyFwdDef.h"
#include "ZFMethodDef.h"
#include "ZFObjectClassTypeFwd.h"
#include "ZFObjectCastDef.h"
#include "ZFAnyDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief shows that the property has no type and can not be used in advanced property copy operation,
 *   ensured an empty string
 */
#define ZFPropertyTypeId_none zfText("")

#define _ZFP_ZFPropertyTypeId_ZFObject zfText("_ZFP_ZFPropertyTypeId_ZFObject")

// ============================================================
/**
 * @brief info for a property for ZFObject, see #ZFPROPERTY_RETAIN for more info
 */
zffinal zfclassNotPOD ZF_ENV_EXPORT ZFProperty
{
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFProperty)

public:
    /**
     * @brief whether two property's type id is matched,
     *   true if two type string is equal or both of them is #ZFPropertyTypeId_none
     */
    static zfbool propertyTypeIdIsMatch(ZF_IN const zfchar *type0, ZF_IN const zfchar *type1);

public:
    /**
     * @brief get info about this property
     */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #objectInfoT */
    zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /**
     * @brief true if this property is registered by #ZFPropertyUserRegisterRetain
     */
    inline zfbool propertyIsUserRegister(void) const
    {
        return this->_ZFP_ZFProperty_propertyIsUserRegister;
    }
    /**
     * @brief get the property's owner class
     */
    inline const ZFClass *ownerClass(void) const
    {
        return this->_ZFP_ZFProperty_ownerClass;
    }
    /**
     * @brief name for the property
     *
     * assume property's name is "myProperty",
     * then the setter would be "myPropertySet",
     * and the getter would be "myProperty",
     * and getter name would return "myProperty"
     */
    inline const zfchar *propertyName(void) const
    {
        return this->_ZFP_ZFProperty_name.cString();
    }
    /**
     * @brief type string for the property
     *
     * note the type string is the macro expand from the type,
     * it may or may not be same for same type,
     * usually for debug use only
     */
    inline const zfchar *propertyTypeName(void) const
    {
        return this->_ZFP_ZFProperty_typeName.cString();
    }
    /**
     * @brief typeid string declared in ZFPROPERTY_XXX
     *
     * this value should be ensured the type id for the type or #ZFPropertyTypeId_none if no known type,
     * this value is used for property's advanced copy function,
     * see #ZFPropertySerializeFrom
     * @note for retain property, this value is always #ZFPropertyTypeId_none
     */
    inline const zfchar *propertyTypeIdName(void) const
    {
        return this->_ZFP_ZFProperty_typeIdName.cString();
    }
    /**
     * @brief whether this property is declared as serializable
     *
     * by default, all property can be serialized,
     * you may explicitly declare a property not serializable by
     * -  for assign property, declare property with #ZFPropertyTypeId_none as type id
     * -  for retain property, using #ZFPROPERTY_RETAIN_NOT_SERIALIZABLE
     *
     * a non-serializable property would be ignored while serializing its owner object
     */
    inline zfbool propertyIsSerializable(void) const
    {
        return this->_ZFP_ZFProperty_serializable;
    }
    /**
     * @brief get the getter method
     */
    inline const ZFMethod *setterMethod(void) const
    {
        return this->_ZFP_ZFProperty_setterMethod;
    }
    /**
     * @brief get the getter method
     */
    inline const ZFMethod *getterMethod(void) const
    {
        return this->_ZFP_ZFProperty_getterMethod;
    }

    /**
     * @brief true if the property is retain property
     *
     * note that bool property is also a assign property,
     * it also have a getter method named "propertyName"
     */
    inline zfbool propertyIsRetainProperty(void) const
    {
        return (this->callbackRetainSet != zfnull);
    }
    /**
     * @brief for retain property only, get the retain property's declared class
     *
     * note it's the declared class, property's value may be subclass of it
     */
    inline const ZFClass *propertyClassOfRetainProperty(void) const
    {
        return this->_ZFP_ZFProperty_propertyClassOfRetainProperty;
    }

public:
    /**
     * @brief used to check whether the property has been accessed
     */
    ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed;
    /**
     * @brief used to check whether the property is in init value state
     */
    ZFPropertyCallbackIsInitValue callbackIsInitValue;
    /**
     * @brief used to reset the property to its init value state
     */
    ZFPropertyCallbackResetInitValue callbackResetInitValue;
    /**
     * @brief used to compare two object's property value
     */
    ZFPropertyCallbackCompare callbackCompare;
    /**
     * @brief used to copy property from another object
     */
    ZFPropertyCallbackCopy callbackCopy;
    /**
     * @brief see #ZFPropertyRetainSet
     */
    ZFPropertyCallbackRetainSet callbackRetainSet;
    /**
     * @brief see #ZFPropertyRetainGet
     */
    ZFPropertyCallbackRetainGet callbackRetainGet;
    /**
     * @brief see #ZFPropertyAssignSet
     */
    ZFPropertyCallbackAssignSet callbackAssignSet;
    /**
     * @brief see #ZFPropertyAssignGet
     */
    ZFPropertyCallbackAssignGet callbackAssignGet;
    /**
     * @brief see #ZFPropertyGetInfo
     */
    ZFPropertyCallbackGetInfo callbackGetInfo;

public:
    /**
     * @brief whether this property should be reflectable
     *
     * true by default, set #ZFPROPERTY_NO_REFLECT to disable it\n
     * a non-reflectable property would be excluded
     * while serializing or copying styleable objects,
     * see #ZFSerializable, #ZFCopyable, #ZFStyleable
     */
    zfbool propertyReflectable;

public:
    /** @cond ZFPrivateDoc */
    ZFProperty(void);
    ~ZFProperty(void);
    zfbool _ZFP_ZFPropertyNeedInit;
    void _ZFP_ZFPropertyInit(ZF_IN zfbool propertyIsUserRegister,
                             ZF_IN const ZFClass *ownerClass,
                             ZF_IN const zfchar *name,
                             ZF_IN const zfchar *typeName,
                             ZF_IN const zfcharA *typeIdName,
                             ZF_IN const ZFMethod *setterMethod,
                             ZF_IN const ZFMethod *getterMethod,
                             ZF_IN const ZFClass *propertyClassOfRetainProperty);
    ZFProperty *_ZFP_ZFProperty_removeConst(void) const
    {
        return const_cast<ZFProperty *>(this);
    }
    /** @endcond */
private:
    zfbool _ZFP_ZFProperty_propertyIsUserRegister;
    const ZFClass *_ZFP_ZFProperty_ownerClass;
    zfstring _ZFP_ZFProperty_name;
    zfstring _ZFP_ZFProperty_typeName;
    zfstring _ZFP_ZFProperty_typeIdName;
    zfbool _ZFP_ZFProperty_serializable;
    const ZFMethod *_ZFP_ZFProperty_setterMethod;
    const ZFMethod *_ZFP_ZFProperty_getterMethod;

    const ZFClass *_ZFP_ZFProperty_propertyClassOfRetainProperty;
};
extern ZF_ENV_EXPORT ZFProperty *_ZFP_ZFPropertyInstanceFind(const zfchar *internalPropertyId);
extern ZF_ENV_EXPORT ZFProperty *_ZFP_ZFPropertyInstanceAccess(const zfchar *internalPropertyId);
extern ZF_ENV_EXPORT zfbool _ZFP_ZFPropertyInstanceCleanup(const zfchar *internalPropertyId);
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFPropertyInstanceHolder
{
public:
    _ZFP_ZFPropertyInstanceHolder(ZF_IN const zfchar *internalPropertyId)
    : internalPropertyId(internalPropertyId)
    , propertyInfo(_ZFP_ZFPropertyInstanceAccess(internalPropertyId))
    {
    }
    ~_ZFP_ZFPropertyInstanceHolder(void)
    {
        _ZFP_ZFPropertyInstanceCleanup(this->internalPropertyId);
    }
private:
    zfstring internalPropertyId;
public:
    ZFProperty *propertyInfo;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyDef_h_

#include "ZFPropertyDeclareDef.h"
#include "ZFPropertyFilterDef.h"
#include "ZFPropertyUtilDef.h"

