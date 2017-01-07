/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFStyleableDef.h
 * @brief styleable
 */

#ifndef _ZFI_ZFStyleableDef_h_
#define _ZFI_ZFStyleableDef_h_

#include "ZFObjectDef.h"
#include "ZFObjectGlobalInstanceDef.h"
#include "ZFSerializableDef.h"
#include "ZFCopyableDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFStyleable
zfclassFwd _ZFP_I_ZFStyleable_PropertyTypeHolder;
/**
 * @brief styleable element that can apply style from another object
 *
 * a styleable typically is a serializable,
 * while serializable process data from and to #ZFSerializableData,
 * a styleable process data from and to other styleable object\n
 * \n
 * to use styleable, simply implements from #ZFStyleable,
 * if all of your properties are declared as #ZFProperty,
 * then every thing's done,
 * and style can be copied by #ZFStyleable::styleableCopyFrom\n
 * for a list of copy method, please refer to #ZFStyleable::styleableOnCheckPropertyType
 */
zfinterface ZF_ENV_EXPORT ZFStyleable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(ZFStyleable, ZFSerializable, ZFCopyable)

protected:
    /** @cond ZFPrivateDoc */
    ZFStyleable(void)
    : _ZFP_ZFStyleable_defaultStyleCache(zfnull)
    {
    }
    /** @endcond */

private:
    ZFStyleable *_ZFP_ZFStyleable_defaultStyleCache;
public:
    /**
     * @brief return default style of this instance
     */
    virtual ZFStyleable *defaultStyle(void);

public:
    /**
     * @brief copy style from another styleable, see #ZFStyleable
     *
     * usually you should not override this method,
     * override #styleableOnCopyFrom instead
     */
    zffinal void styleableCopyFrom(ZF_IN ZFStyleable *anotherStyleable);

public:
    /**
     * @brief true if this object is #defaultStyle
     */
    virtual zfbool styleableIsDefaultStyle(void)
    {
        return zffalse;
    }

protected:
    /** @brief see #ZFStyleable::styleableOnCheckPropertyType */
    typedef enum {
        PropertyTypeNotStyleable, /**< @brief see #ZFStyleable::styleableOnCheckPropertyType */
        PropertyTypeNormal, /**< @brief see #ZFStyleable::styleableOnCheckPropertyType */
        PropertyTypeStyleable, /**< @brief see #ZFStyleable::styleableOnCheckPropertyType */
        PropertyTypeCopyable, /**< @brief see #ZFStyleable::styleableOnCheckPropertyType */
    } PropertyType;
    /**
     * @brief check property type
     *
     * property type shows how styleable would act while copying from another styleable:
     * -  PropertyTypeNotStyleable:
     *   shows the property should not be copied
     * -  PropertyTypeNormal:
     *   shows the property should be copied normally,
     *   i.e. use getter and setter to copy value
     * -  PropertyTypeStyleable:
     *   shows the property itself is styleable,
     *   we would access its value and copy style by #styleableCopyFrom\n
     *   if the property is both #ZFStyleable and #ZFCopyable,
     *   it would be treated as #ZFStyleable
     * -  PropertyTypeCopyable:
     *   shows the property itself is copyable,
     *   we would access its value and copy style by #ZFCopyable::copyFrom
     *
     * \n
     * by default, a property is treated as not copyable if matches any of conditions below:
     * -  getter or setter method is private
     * -  #ZFPROPERTY_NO_REFLECT has been declared
     *
     * a property is treated as styleable/copyable property if matches all of conditions below:
     * -  is retain property and the type is ZFStyleable/ZFCopyable
     * -  getter method is not private and setter method is private
     *
     * all other property are treated as normal property
     */
    virtual ZFStyleable::PropertyType styleableOnCheckPropertyType(ZF_IN const ZFProperty *property);
    /**
     * @brief copy property with styleable logic
     *
     * @note anotherStyleable not ensured to be same type as self,
     *   while the property is ensured to be same and safe to copy
     */
    virtual void styleableOnCopyPropertyFrom(ZF_IN ZFStyleable *anotherStyleable,
                                             ZF_IN const ZFProperty *property,
                                             ZF_IN ZFStyleable::PropertyType propertyType);
    /**
     * @brief for subclass to achieve custom style copy step,
     *   called by #styleableCopyFrom, see #ZFStyleable
     */
    virtual inline void styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
    {
    }

private:
    zffinal _ZFP_I_ZFStyleable_PropertyTypeHolder *_ZFP_ZFStyleable_getPropertyTypeHolder(void);
};

// ============================================================
// ZFStyleableObject
/**
 * @brief common styleable object
 *
 * implements #ZFStyleable, #ZFSerializable, #ZFCopyable\n
 * every style, serialize and copy logic has been done by reflect,
 * if all of your properties are declared as #ZFProperty
 */
zfclass ZF_ENV_EXPORT ZFStyleableObject : zfextends ZFObject, zfimplements ZFStyleable, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFStyleableObject, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFStyleable, ZFSerializable, ZFCopyable)

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->styleableCopyFrom(ZFCastZFObjectUnchecked(zfself *, anotherObj));
    }
};

// ============================================================
// DefaultStyle
#define _ZFP_ZFStyleableDefault_level ZFLevelZFFrameworkNormal
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFStyleableDefaultPointerHolder
{
public:
    void *d;
    _ZFP_ZFStyleableDefaultPointerHolder(void) : d(zfnull) {}
};

extern ZF_ENV_EXPORT _ZFP_ZFStyleableDefaultPointerHolder *_ZFP_ZFStyleableDefaultRefAccess(ZF_IN const zfchar *name);
typedef void (*_ZFP_ZFStyleableDefaultDeleteCallback)(ZF_IN void *instance);
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFStyleableDefaultDeleteCallbackHolder
{
public:
    _ZFP_ZFStyleableDefaultDeleteCallbackHolder(ZF_IN _ZFP_ZFStyleableDefaultDeleteCallback deleteCallback,
                                                ZF_IN void *instance)
    : deleteCallback(deleteCallback)
    , instance(instance)
    {
    }
    ~_ZFP_ZFStyleableDefaultDeleteCallbackHolder(void)
    {
        if(this->deleteCallback && this->instance)
        {
            this->deleteCallback(this->instance);
        }
    }
private:
    _ZFP_ZFStyleableDefaultDeleteCallback deleteCallback;
    void *instance;
};

#define _ZFP_ZFSTYLE_DEFAULT_DECLARE(YourStyle) \
    public: \
        zfclass _ZFP_ZFStyleableDefault_##YourStyle; \
    public: \
        /** \n default style for @ref YourStyle */ \
        static YourStyle *DefaultStyle(void); \
        /** @brief default style for @ref YourStyle (reflectable) */ \
        ZFMETHOD_DECLARE_STATIC_0(ZFStyleable *, DefaultStyleReflect); \
    private: \
        static void _ZFP_ZFStyleablEnumDefaultStyleSet(ZF_IN YourStyle *newInstance); \
        static ZFCorePointerBase *&_ZFP_ZFStyleableDefaultCleaner(void); \
        static void _ZFP_ZFStyleableDefaultOnDelete(ZF_IN void *instance); \
    public:
#define _ZFP_ZFSTYLE_DEFAULT_DEFINE(YourStyle) \
    zfclass YourStyle::_ZFP_ZFStyleableDefault_##YourStyle : zfextends YourStyle \
    { \
        ZFOBJECT_DECLARE(_ZFP_ZFStyleableDefault_##YourStyle, YourStyle) \
    public: \
        zfoverride \
        virtual zfbool styleableIsDefaultStyle(void) \
        { \
            return zftrue; \
        } \
    }; \
    YourStyle *YourStyle::DefaultStyle(void) \
    { \
        ZFCoreMutexLocker(); \
        static _ZFP_ZFStyleableDefaultPointerHolder *holder = _ZFP_ZFStyleableDefaultRefAccess(ZFM_TOSTRING(YourStyle)); \
        if(holder->d == zfnull) \
        { \
            if(ZFFrameworkStateCheck(_ZFP_ZFStyleableDefault_level) == ZFFrameworkStateAvailable) \
            { \
                zfautoObject obj = _ZFP_ZFStyleableDefault_##YourStyle::ClassData()->newInstance( \
                    ZF_CALLER_FILE, zfTextA("DefaultStyle"), ZF_CALLER_LINE); \
                if(obj != zfautoObjectNull) \
                { \
                    zfself::_ZFP_ZFStyleablEnumDefaultStyleSet(obj.to<YourStyle *>()); \
                } \
            } \
        } \
        return ZFCastStatic(YourStyle *, holder->d); \
    } \
    ZFMETHOD_DEFINE_STATIC_0(YourStyle, ZFStyleable *, DefaultStyleReflect) \
    { \
        return ZFCastZFObjectUnchecked(ZFStyleable *, zfself::DefaultStyle()); \
    } \
    void YourStyle::_ZFP_ZFStyleablEnumDefaultStyleSet(ZF_IN YourStyle *newInstance) \
    { \
        if(ZFFrameworkStateCheck(_ZFP_ZFStyleableDefault_level) != ZFFrameworkStateAvailable) \
        { \
            return ; \
        } \
        _ZFP_ZFStyleableDefaultPointerHolder *holder = _ZFP_ZFStyleableDefaultRefAccess(ZFM_TOSTRING(YourStyle)); \
        if(holder->d == newInstance) \
        { \
            return ; \
        } \
        ZFCorePointerBase *&cleanerRef = _ZFP_ZFStyleableDefaultCleaner(); \
        ZFCorePointerBase *cleanerOld = cleanerRef; \
        cleanerRef = zfnull; \
        if(newInstance != zfnull) \
        { \
            holder->d = zfRetain(newInstance); \
            newInstance->toObject()->objectCachedSet(zftrue); \
            cleanerRef = ZFObjectGlobalInstanceAdd(ZFCorePointerForObject<_ZFP_ZFStyleableDefaultDeleteCallbackHolder *>( \
                zfnew(_ZFP_ZFStyleableDefaultDeleteCallbackHolder, YourStyle::_ZFP_ZFStyleableDefaultOnDelete, holder->d)), \
                _ZFP_ZFStyleableDefault_level); \
        } \
        if(cleanerOld != zfnull) \
        { \
            ZFObjectGlobalInstanceRemove(cleanerOld, _ZFP_ZFStyleableDefault_level); \
        } \
    } \
    ZFCorePointerBase *&YourStyle::_ZFP_ZFStyleableDefaultCleaner(void) \
    { \
        static ZFCorePointerBase *_cleaner = zfnull; \
        return _cleaner; \
    } \
    void YourStyle::_ZFP_ZFStyleableDefaultOnDelete(ZF_IN void *instance) \
    { \
        _ZFP_ZFStyleableDefaultPointerHolder *holder = _ZFP_ZFStyleableDefaultRefAccess(ZFM_TOSTRING(YourStyle)); \
        if(holder->d == instance) \
        { \
            holder->d = zfnull; \
        } \
        YourStyle *tmp = ZFCastStatic(YourStyle *, instance); \
        tmp->toObject()->objectCachedSet(zffalse); \
        zfRelease(tmp); \
    }

/**
 * @brief used to declare a default style, see #ZFStyleable
 *
 * usage:
 * @code
 *   // in h file
 *   zfclass YourStyleableObject : zfextends ParentStyleable
 *   {
 *       ZFOBJECT_DECLARE(YourStyleableObject, ParentStyleable)
 *       ZFSTYLE_DEFAULT_DECLARE(YourStyleableObject)
 *   };
 *
 *   // in cpp file
 *   ZFSTYLE_DEFAULT_DEFINE(YourStyleableObject)
 * @endcode
 * the macros above declare these types for you:
 * -  default style access method:
 *   static YourObjectStyle *DefaultStyle(void);
 *   virtual ZFStyleable *defaultStyle(void);
 * -  default style access method as ZFMethod (reflectable):
 *   static ZFStyleable *DefaultStyleReflect(void);
 *
 * \n
 * @note a style holder object would be created automatically when access the default style,
 *   and would be deleted automatically during #ZFFrameworkCleanup with level #ZFLevelZFFrameworkNormal,
 *   so it would result null if you access DefaultStyle before initialized or after it's deallocated
 * @note the default style holder object should only be used to hold the styles,
 *   typically you should not use it directly
 * @note it is useful to combine styleable logic and serializable logic, typical usage:
 *   @code
 *     <YourObject styleableType="ZFMethod" styleableData="YourStyle::DefaultStyleReflect" />
 *   @endcode
 *   serializing this code would result to create an instance of YourObject,
 *   then copy style from YourStyle::DefaultStyleReflect by #ZFStyleable::styleableCopyFrom\n
 *   see #ZFObjectCreatorType_ZFMethod for more info
 * @note for performance, the default style won't be copied to its instance by default,
 *   you may use #ZFClass::instanceObserverAdd to observe styleable object's
 *   instance alloc event, and apply your default styles\n
 *   or use #ZFStyleDefaultApplyAutoCopy or #ZFSTYLE_DEFAULT_AUTO_COPY to make the auto copy
 */
#define ZFSTYLE_DEFAULT_DECLARE(YourStyle) \
    _ZFP_ZFSTYLE_DEFAULT_DECLARE(YourStyle)
/** @brief see #ZFSTYLE_DEFAULT_DECLARE */
#define ZFSTYLE_DEFAULT_DEFINE(YourStyle) \
    _ZFP_ZFSTYLE_DEFAULT_DEFINE(YourStyle) \
    ZFMETHOD_REGISTER(YourStyle, DefaultStyleReflect)

/**
 * @brief util method to setup automatically copy style from
 *   #ZFStyleable::defaultStyle if its property value changed
 *
 * for performance, the default style won't be copied to each instance by default,
 * by calling this method, when #ZFStyleable::defaultStyle's property value changed,
 * it would be copied to the registered instance
 */
extern ZF_ENV_EXPORT void ZFStyleDefaultApplyAutoCopy(ZF_IN ZFStyleable *style);

/**
 * @brief util macro to setup automatically copy style from
 *   #ZFStyleable::defaultStyle
 *
 * use #ZFINTERFACE_ON_INIT_DECLARE to copy styles during init,
 * and use #ZFStyleDefaultApplyAutoCopy to ensure default style value changes would be applied
 */
#define ZFSTYLE_DEFAULT_AUTO_COPY() \
    ZFINTERFACE_ON_INIT_DECLARE() \
    { \
        if(!this->styleableIsDefaultStyle()) \
        { \
            ZFStyleable *defaultStyle = this->defaultStyle(); \
            if(defaultStyle != zfnull) \
            { \
                this->styleableCopyFrom(defaultStyle); \
                ZFStyleDefaultApplyAutoCopy(this); \
            } \
        } \
    }

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFStyleableDef_h_

