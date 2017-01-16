/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTimeLineProperty.h
 * @brief update property based on time line
 */

#ifndef _ZFI_ZFTimeLineProperty_h_
#define _ZFI_ZFTimeLineProperty_h_

#include "ZFTimeLineCurve.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief update property based on time line, see #ZFTimeLineCurve
 *
 * this is an abstract property update manager based on time line logic,
 * to achieve, you must:
 * -  have two property named "fromValue" and "toValue"
 *   which describe the property value change's range,
 *   also, it must be serializable
 * -  override abstract methods to achieve value change action
 */
zfabstract ZF_ENV_EXPORT ZFTimeLineProperty : zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFTimeLineProperty, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable, ZFCopyable)

public:
    /**
     * @brief the owner property
     */
    ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE(const ZFProperty *, ownerProperty)

public:
    zfoverride
    virtual zfidentity objectHash(void)
    {
        if(this->ownerProperty() == zfnull)
        {
            return zfidentityCalcString(this->classData()->className());
        }
        else
        {
            return zfidentityHash(zfidentityCalcString(this->classData()->className())
                , zfidentityCalcString(this->ownerProperty()->ownerClass()->className())
                , zfidentityCalcString(this->ownerProperty()->propertyTypeIdName())
                );
        }
    }
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj)
    {
        if(this == anotherObj) {return ZFCompareTheSame;}
        zfself *another = ZFCastZFObject(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}

        const ZFProperty *fromValueProperty = ZFPropertyGet(this->classData(), zfText("fromValue"));
        const ZFProperty *toValueProperty = ZFPropertyGet(this->classData(), zfText("fromValue"));
        if(fromValueProperty == zfnull || toValueProperty == zfnull
            || !another->classData()->classIsTypeOf(fromValueProperty->ownerClass())
            || !another->classData()->classIsTypeOf(toValueProperty->ownerClass())
            || fromValueProperty->callbackCompare(fromValueProperty, this, another) != ZFCompareTheSame
            || toValueProperty->callbackCompare(toValueProperty, this, another) != ZFCompareTheSame
            || this->ownerProperty() != another->ownerProperty())
        {
            return ZFCompareUncomparable;
        }
        return ZFCompareTheSame;
    }

public:
    /**
     * @brief see #ZFTimeLineProperty
     *
     * caller must make sure #ownerProperty is valid and ownerObj containing #ownerProperty
     */
    zffinal inline void progressUpdate(ZF_IN ZFObject *ownerObj,
                                       ZF_IN zffloat progress)
    {
        this->progressOnUpdate(ownerObj, progress);
    }
    /**
     * @brief see #ZFTimeLineProperty
     *
     * caller must make sure #ownerProperty is valid and ownerObj containing #ownerProperty
     */
    zffinal inline void stateSave(ZF_IN ZFObject *ownerObj)
    {
        this->stateOnSave(ownerObj);
    }
    /**
     * @brief see #ZFTimeLineProperty
     *
     * caller must make sure #ownerProperty is valid and ownerObj containing #ownerProperty
     */
    zffinal inline void stateRestore(ZF_IN ZFObject *ownerObj)
    {
        this->stateOnRestore(ownerObj);
    }

protected:
    /**
     * @brief see #ZFTimeLineProperty
     */
    virtual void progressOnUpdate(ZF_IN ZFObject *ownerObj,
                                  ZF_IN zffloat progress) = 0;
    /**
     * @brief see #ZFTimeLineProperty
     */
    virtual void stateOnSave(ZF_IN ZFObject *ownerObj) = 0;
    /**
     * @brief see #ZFTimeLineProperty
     */
    virtual void stateOnRestore(ZF_IN ZFObject *ownerObj) = 0;
};

// ============================================================
/**
 * @brief util macro to declare #ZFTimeLineProperty for short
 *
 * you should supply progressFunc as a function body with this proto type:
 * @code
 *   Type onProgress(ZF_IN Type const &fromValue, ZF_IN Type const &toValue, ZF_IN zffloat progress);
 * @endcode
 * for example:
 * @code
 *   ZFTIMELINEPROPERTY_DECLARE_BY_VALUE_DETAIL(TimeLinePropertyClass, YourType, {
 *       return (fromValue + (YourType)((toValue - fromValue) * progress));
 *   })
 * @endcode
 */
#define ZFTIMELINEPROPERTY_DECLARE_BY_VALUE_DETAIL(ZFTimeLinePropertyClassName, Type, progressFunc) \
    /** @brief see #ZFTimeLineProperty */ \
    zfclass ZF_ENV_EXPORT ZFTimeLinePropertyClassName : zfextends ZFTimeLineProperty \
    { \
        ZFOBJECT_DECLARE(ZFTimeLinePropertyClassName, ZFTimeLineProperty) \
        /** @brief see #ZFTimeLineProperty */ \
        ZFPROPERTY_ASSIGN(Type, fromValue) \
        /** @brief see #ZFTimeLineProperty */ \
        ZFPROPERTY_ASSIGN(Type, toValue) \
    public: \
        /** @brief init with from and to value */ \
        virtual ZFObject *objectOnInit(ZF_IN Type const &from, \
                                       ZF_IN Type const &to) \
        { \
            this->objectOnInit(); \
            this->fromValueSet(from); \
            this->toValueSet(to); \
            return this; \
        } \
        zfoverride \
        virtual ZFObject *objectOnInit(void) \
        { \
            return zfsuper::objectOnInit(); \
        } \
    protected: \
        zfoverride \
        virtual void progressOnUpdate(ZF_IN ZFObject *ownerObj, \
                                      ZF_IN zffloat progress) \
        { \
            zfsuper::progressOnUpdate(ownerObj, progress); \
            this->ownerProperty()->setterMethod()->execute<void, Type const &>(ownerObj, \
                this->progressAction(this->fromValue(), this->toValue(), progress)); \
        } \
        zfoverride \
        virtual void stateOnSave(ZF_IN ZFObject *ownerObj) \
        { \
            zfsuper::stateOnSave(ownerObj); \
            this->_savedState = this->ownerProperty()->getterMethod()->execute<Type const &>(ownerObj); \
        } \
        zfoverride \
        virtual void stateOnRestore(ZF_IN ZFObject *ownerObj) \
        { \
            zfsuper::stateOnRestore(ownerObj); \
            this->ownerProperty()->setterMethod()->execute<void, Type const &>(ownerObj, this->_savedState); \
        } \
    private: \
        Type _savedState; \
        inline Type progressAction(ZF_IN Type const &fromValue, ZF_IN Type const &toValue, ZF_IN zffloat progress) \
        progressFunc \
    };

/**
 * @brief see #ZFTIMELINEPROPERTY_DECLARE_BY_VALUE_DETAIL
 */
#define ZFTIMELINEPROPERTY_DECLARE_BY_VALUE(Type) \
    ZFTIMELINEPROPERTY_DECLARE_BY_VALUE_DETAIL(ZFM_CAT(ZFTimeLineProperty_, Type), Type, { \
        return (fromValue + (Type)((toValue - fromValue) * progress)); \
    })

// ============================================================
/**
 * @brief util macro to declare #ZFTimeLineProperty for short
 *
 * you should supply progressFunc as a function body with this proto type:
 * @code
 *   Type onProgress(ZF_IN zffloat progress);
 * @endcode
 * for example:
 * @code
 *   ZFTIMELINEPROPERTY_DECLARE_CUSTOM(TimeLinePropertyClass, YourType, {
 *       return (YourType)yourFuncWithProgress(progress);
 *   })
 * @endcode
 */
#define ZFTIMELINEPROPERTY_DECLARE_CUSTOM(ZFTimeLinePropertyClassName, Type, progressFunc) \
    /** @brief see #ZFTimeLineProperty */ \
    zfclass ZF_ENV_EXPORT ZFTimeLinePropertyClassName : zfextends ZFTimeLineProperty \
    { \
        ZFOBJECT_DECLARE(ZFTimeLinePropertyClassName, ZFTimeLineProperty) \
    protected: \
        zfoverride \
        virtual void progressOnUpdate(ZF_IN ZFObject *ownerObj, \
                                      ZF_IN zffloat progress) \
        { \
            zfsuper::progressOnUpdate(ownerObj, progress); \
            this->ownerProperty()->setterMethod()->execute<void, Type const &>(ownerObj, \
                this->progressAction(progress)); \
        } \
        zfoverride \
        virtual void stateOnSave(ZF_IN ZFObject *ownerObj) \
        { \
            zfsuper::stateOnSave(ownerObj); \
            this->_savedState = this->ownerProperty()->getterMethod()->execute<Type const &>(ownerObj); \
        } \
        zfoverride \
        virtual void stateOnRestore(ZF_IN ZFObject *ownerObj) \
        { \
            zfsuper::stateOnRestore(ownerObj); \
            this->ownerProperty()->setterMethod()->execute<void, Type const &>(ownerObj, this->_savedState); \
        } \
    private: \
        Type _savedState; \
        inline Type progressAction(ZF_IN zffloat progress) \
        progressFunc \
    };

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLineProperty_h_

