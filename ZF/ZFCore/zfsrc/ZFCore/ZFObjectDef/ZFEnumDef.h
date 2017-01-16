/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFEnumDef.h
 * @brief enum utility
 */

#ifndef _ZFI_ZFEnumDef_h_
#define _ZFI_ZFEnumDef_h_

#include "ZFObjectDef.h"
#include "ZFSerializableDef.h"
#include "ZFCopyableDef.h"
#include "ZFCoreType_IODef.h"
#include "ZFVarConvertDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief value type for ZFEnum, equal to zfint
 */
typedef zfflags ZFEnumValue;
/**
 * @brief invalid value for ZFEnum
 */
#define ZFEnumValueInvalid ((ZFEnumValue)-1)
/**
 * @brief invalid value's name for ZFEnum
 */
#define ZFEnumValueNameInvalid zfText("ZFEnumValueInvalid")

// ============================================================
/**
 * @brief abstract base class of all enum type of ZFFramework
 *
 * please refer to #ZFENUM_BEGIN to see how to use enum type\n
 * \n
 * serializable data:
 * @code
 *   <EnumClass
 *       value="EmunValueName" // optional, #ZFEnumValueNameInvalid by default
 *   />
 * @endcode
 */
zfabstract ZF_ENV_EXPORT ZFEnum : zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFEnum, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable, ZFCopyable)

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->enumValueSet(ZFCastZFObjectUnchecked(zfself *, anotherObj)->enumValue());
    }

public:
    /**
     * @brief init with value or ZFEnumValueInvalid if invalid
     */
    virtual ZFObject *objectOnInit(ZF_IN ZFEnumValue value);
    /**
     * @brief init with ZFEnum
     */
    virtual ZFObject *objectOnInit(ZF_IN ZFEnum *another);
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual inline void objectInfoOnAppendTokenLeft(ZF_IN_OUT zfstring &ret)
    {
    }
    zfoverride
    virtual inline void objectInfoOnAppendTokenRight(ZF_IN_OUT zfstring &ret)
    {
    }
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief compare result of two enum values as int,
     *   or return ZFCompareUncomparable if any of them are ZFEnumValueInvalid
     */
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief get value num
     */
    virtual zfindex enumCount(void) = 0;
    /**
     * @brief get the index of enum value, or zfindexMax if no such enum value
     */
    virtual zfindex enumIndexForValue(ZF_IN ZFEnumValue value) = 0;
    /**
     * @brief get the value at index, or ZFEnumValueInvalid if not exist
     */
    virtual ZFEnumValue enumValueAtIndex(ZF_IN zfindex index) = 0;
    /**
     * @brief get the name at index, or ZFEnumValueNameInvalid if not exist
     */
    virtual const zfchar *enumNameAtIndex(ZF_IN zfindex index) = 0;
    /**
    * @brief get the full name (EnumType::EnumValueName) at index, or ZFEnumValueNameInvalid if not exist
     */
    virtual const zfchar *enumFullNameAtIndex(ZF_IN zfindex index) = 0;
    /**
     * @brief return true if contain the specified value
     */
    virtual zfbool enumContainValue(ZF_IN ZFEnumValue value) = 0;

    /**
     * @brief get the value with specified name, or ZFEnumValueInvalid if not exist
     */
    virtual ZFEnumValue enumValueForName(ZF_IN const zfchar *name) = 0;
    /**
     * @brief get the name with specified value, or ZFEnumValueNameInvalid if not exist
     */
    virtual const zfchar *enumNameForValue(ZF_IN ZFEnumValue value) = 0;

    /**
     * @brief get the enum value, or #ZFEnumValueInvalid if invalid
     */
    virtual ZFEnumValue enumValue(void)
    {
        return _ZFP_ZFEnum_value;
    }
    /**
     * @brief get the enum value and cast to desired type,
     *   you must ensure it's not #ZFEnumValueInvalid
     */
    template<typename T_EnumType>
    inline T_EnumType enumValue(void)
    {
        return ZFCastStatic(T_EnumType, _ZFP_ZFEnum_value);
    }
    /**
     * @brief get current value's name, or ZFEnumValueNameInvalid if invalid
     */
    virtual const zfchar *enumName(void)
    {
        return this->enumNameForValue(_ZFP_ZFEnum_value);
    }

protected:
    /**
     * @brief set the enum value
     *
     * must be the enum's value or ZFEnumValueInvalid
     */
    virtual void enumValueSet(ZF_IN ZFEnumValue value)
    {
        if(!this->enumContainValue(value))
        {
            _ZFP_ZFEnum_value = ZFEnumValueInvalid;
            return ;
        }
        _ZFP_ZFEnum_value = value;
    }

private:
    ZFEnumValue _ZFP_ZFEnum_value;
};

// ============================================================
zfclassFwd _ZFP_ZFEnumDataPrivate;
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFEnumData
{
public:
    _ZFP_ZFEnumData(void);
    ~_ZFP_ZFEnumData(void);
public:
    zfbool needInitFlag;
    void add(ZF_IN zfbool isEnableDuplicateValue,
             ZF_IN ZFEnumValue value,
             ZF_IN const zfchar *name,
             ZF_IN const zfchar *fullName,
             ZF_IN const ZFClass *ownerClass,
             ZF_IN const zfcharA *callerFile,
             ZF_IN zfindex callerLine);
    zfindex enumCount(void) const;
    zfindex enumIndexForValue(ZF_IN ZFEnumValue value) const;
    ZFEnumValue enumValueAtIndex(ZF_IN zfindex index) const;
    const zfchar *enumNameAtIndex(ZF_IN zfindex index) const;
    const zfchar *enumFullNameAtIndex(ZF_IN zfindex index) const;
    zfbool enumContainValue(ZF_IN ZFEnumValue value) const;
    ZFEnumValue enumValueForName(ZF_IN const zfchar *name) const;
    const zfchar *enumNameForValue(ZF_IN ZFEnumValue value) const;
private:
    _ZFP_ZFEnumDataPrivate *d;
};
extern ZF_ENV_EXPORT _ZFP_ZFEnumData *_ZFP_ZFEnumDataAccess(const zfchar *name);

// ============================================================
/**
 * @brief macros to define a key-value map for enum type
 *
 * usage:
 * @code
 *   // ============================================================
 *   // in h file
 *  / **
 *    * you can add Doxygen docs for EnumName (as a ZFObject) here
 *    * /
 *   ZFENUM_BEGIN(EnumName)
 *      / **
 *        * you can add Doxygen docs for Value1 here
 *        * /
 *       ZFENUM_VALUE(Value1)
 *       ZFENUM_VALUE(Value2) // /< you can add Doxygen docs for Value2 here
 *       ZFENUM_VALUE_WITH_INIT(Value3, 33) // you can assign the value for enum item
 *       ZFENUM_VALUE_WITH_INIT(Value4, e_Value2) // you can assign a same value with old value
 *   ZFENUM_SEPARATOR(EnumName)
 *       // you must use ZFENUM_VALUE_REGISTER to map the value and name
 *       ZFENUM_VALUE_REGISTER(Value1)
 *       ZFENUM_VALUE_REGISTER(Value2)
 *       // you can set a custom name by ZFENUM_VALUE_REGISTER_WITH_NAME
 *       ZFENUM_VALUE_REGISTER_WITH_NAME(Value3, zfText("CustomNameValue3"))
 *       // (here Value4 is equal to Value2)
 *       // when register a new name for a existing value,
 *       // old name would be overrided,
 *       // as well as the value-name map
 *       ZFENUM_VALUE_REGISTER_WITH_NAME(Value4, zfText("Value4 override Value2"))
 *   ZFENUM_END(EnumName)
 *
 *   // ============================================================
 *   // in cpp file
 *   // add this macro for necessary code expansion
 *   ZFENUM_DEFINE(EnumName)
 * @endcode
 * @note if there are more than one enum with same value,
 *   only the last registered one would be mapped
 * once defined, you can:
 * -  use EnumName to access the enum value and name
 *   @code
 *     ZFEnumValue value;
 *     const zfchar *name;
 *     value = EnumName::e_Value1;
 *     value = EnumName::EnumValueForName(zfText("Value1"));
 *     name = EnumName::EnumNameForValue(value);
 *   @endcode
 * -  use EnumName to store the enum value as a ZFObject
 *   @code
 *     EnumName *e = zfAlloc(EnumName());
 *     e->enumValueSet(EnumName::e_Value1);
 *     ZFEnumValue value = e->enumValue();
 *     const zfchar *name = e->enumName();
 *     zfRelease(e);
 *   @endcode
 * -  use base class ZFEnum to achieve dynamic binding
 *   @code
 *     ZFEnumValue value;
 *     const zfchar *name;
 *
 *     ZFEnum *e = zfAlloc(EnumName, EnumName::e_Value1);
 *     value = e->enumValue(); // return the value stored as EnumName
 *     zfRelease(e);
 *
 *     zfautoObject tmp = ZFClass::newInstanceForName(zfText("EnumName")); // see #ZFOBJECT_REGISTER for more info
 *     e = tmp.to<ZFEnum *>();
 *     for(zfindex i = 0; i < e->enumCount(); ++i)
 *     { // OK, list all the value and name for e, which is EnumName type
 *         value = e->enumValueAtIndex(i);
 *         name = e->enumNameAtIndex(i);
 *     }
 *     value = e->enumValueForName(zfText("Value1")); // OK, value from EnumName
 *     name = e->enumNameForValue(value); // OK, name from EnumName
 *     zfRelease(e);
 *   @endcode
 * -  you can access the internal enum type by EnumNameEnum
 * -  usually, it's highly recommended to use the internal enum type for performance:
 *   @code
 *     // pass by EnumNameEnum value, which is actually a int value
 *     void func1(MyEnumEnum e) {...}
 *   @endcode
 */
#define ZFENUM_BEGIN(ChildEnum) \
    zfclass ZF_ENV_EXPORT ChildEnum : zfextends ZFEnum \
    { \
        ZFOBJECT_DECLARE(ChildEnum, ZFEnum) \
    public: \
        /** @brief see @ref ChildEnum */ \
        typedef enum \
        {
/**
 * @brief see #ZFENUM_BEGIN
 */
#define ZFENUM_VALUE(Value) /** \n */ e_##Value,
/**
 * @brief see #ZFENUM_BEGIN
 */
#define ZFENUM_VALUE_WITH_INIT(Value, initValue) /** @brief \n Value(initValue)\n */ e_##Value = initValue,
#define _ZFP_ZFENUM_SEPARATOR(ChildEnum, isEnableDuplicateValue_) \
        } ZFEnumType; \
    public: \
        /** @brief calculate hash for value */ \
        static zfidentity hashForValue(ZF_IN ZFEnumValue value) \
        { \
            return (zfidentityCalcString(zfself::ClassData()->className()) ^ ((zfidentity)value)); \
        } \
        /** @brief see #ZFObject::objectHash */ \
        virtual zfidentity objectHash(void) \
        { \
            return zfself::hashForValue(this->enumValue()); \
        } \
    public: \
        /** @brief get the count of enum value */ \
        ZFMETHOD_DECLARE_DETAIL_0(public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
                                  zfindex, EnumCount, ZFMethodNoId) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumCount(); \
        } \
        /** @brief get the index of enum value, or zfindexMax if no such enum value */ \
        ZFMETHOD_DECLARE_DETAIL_1(public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
                                  zfindex, EnumIndexForValue, ZFMethodNoId, \
                                  ZFEnumValue, value, ZFMethodNoDefaultParam) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumIndexForValue(value); \
        } \
        /** @brief get the value at index, or ZFEnumValueInvalid if not exist */ \
        ZFMETHOD_DECLARE_DETAIL_1(public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
                                  ZFEnumValue, EnumValueAtIndex, ZFMethodNoId, \
                                  zfindex, index, ZFMethodNoDefaultParam) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumValueAtIndex(index); \
        } \
        /** @brief get the name at index, or ZFEnumValueNameInvalid if not exist */ \
        ZFMETHOD_DECLARE_DETAIL_1(public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
                                  const zfchar *, EnumNameAtIndex, ZFMethodNoId, \
                                  zfindex, index, ZFMethodNoDefaultParam) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumNameAtIndex(index); \
        } \
        /** @brief get the name at index, or ZFEnumValueNameInvalid if not exist */ \
        ZFMETHOD_DECLARE_DETAIL_1(public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
                                  const zfchar *, EnumFullNameAtIndex, ZFMethodNoId, \
                                  zfindex, index, ZFMethodNoDefaultParam) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumFullNameAtIndex(index); \
        } \
        /** @brief return true if contain the specified value */ \
        ZFMETHOD_DECLARE_DETAIL_1(public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
                                  zfbool, EnumContainValue, ZFMethodNoId, \
                                  ZFEnumValue, value, ZFMethodNoDefaultParam) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumContainValue(value); \
        } \
        /** @brief get value with name, or ZFEnumValueInvalid if not exist */ \
        ZFMETHOD_DECLARE_DETAIL_1(public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
                                  ZFEnumValue, EnumValueForName, ZFMethodNoId, \
                                  const zfchar *, name, ZFMethodNoDefaultParam) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumValueForName(name); \
        } \
        /** @brief get name with value, or ZFEnumValueNameInvalid if not exist */ \
        ZFMETHOD_DECLARE_DETAIL_1(public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
                                  const zfchar *, EnumNameForValue, ZFMethodNoId, \
                                  ZFEnumValue, value, ZFMethodNoDefaultParam) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumNameForValue(value); \
        } \
    public: \
        zfoverride \
        virtual zfindex enumCount(void) \
        { \
            return zfself::EnumCount(); \
        } \
        zfoverride \
        virtual zfindex enumIndexForValue(ZF_IN ZFEnumValue value) \
        { \
            return zfself::EnumIndexForValue(value); \
        } \
        zfoverride \
        virtual ZFEnumValue enumValueAtIndex(ZF_IN zfindex index) \
        { \
            return zfself::EnumValueAtIndex(index); \
        } \
        zfoverride \
        virtual const zfchar *enumNameAtIndex(ZF_IN zfindex index) \
        { \
            return zfself::EnumNameAtIndex(index); \
        } \
        zfoverride \
        virtual const zfchar *enumFullNameAtIndex(ZF_IN zfindex index) \
        { \
            return zfself::EnumFullNameAtIndex(index); \
        } \
        zfoverride \
        virtual zfbool enumContainValue(ZF_IN ZFEnumValue value) \
        { \
            return zfself::EnumContainValue(value); \
        } \
        zfoverride \
        virtual ZFEnumValue enumValueForName(ZF_IN const zfchar *name) \
        { \
            return zfself::EnumValueForName(name); \
        } \
        zfoverride \
        virtual const zfchar *enumNameForValue(ZF_IN ZFEnumValue value) \
        { \
            return zfself::EnumNameForValue(value); \
        } \
    private: \
        static const _ZFP_ZFEnumData *_ZFP_ZFEnumDataRef(void) \
        { \
            static _ZFP_ZFEnumData *d = _ZFP_ZFEnumDataAccess(ZFM_TOSTRING(ChildEnum)); \
            if(d->needInitFlag) \
            { \
                d->needInitFlag = zffalse; \
                zfbool isEnableDuplicateValue = isEnableDuplicateValue_; \
                const zfchar *prefix = zfText(#ChildEnum) zfText("::");
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_SEPARATOR(ChildEnum) _ZFP_ZFENUM_SEPARATOR(ChildEnum, zffalse)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_SEPARATOR_ALLOW_DUPLICATE_VALUE(ChildEnum) _ZFP_ZFENUM_SEPARATOR(ChildEnum, zftrue)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_VALUE_REGISTER_WITH_NAME(Value, Name) \
                d->add(isEnableDuplicateValue, zfself::e_##Value, Name, zfsConnectLineFree(prefix, Name), zfself::ClassData(), ZF_CALLER_FILE, ZF_CALLER_LINE);
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_VALUE_REGISTER(Value) \
                ZFENUM_VALUE_REGISTER_WITH_NAME(Value, zfText(#Value))
#define _ZFP_ZFENUM_END_EXTRA(ChildEnum) \
    /** @brief editable version of @ref ChildEnum */ \
    zfclass ZF_ENV_EXPORT ChildEnum##Editable : zfextends ChildEnum \
    { \
        ZFOBJECT_DECLARE(ChildEnum##Editable, ChildEnum) \
    public: \
        zfoverride \
        virtual void enumValueSet(ZF_IN ZFEnumValue value) \
        { \
            zfsuper::enumValueSet(value); \
        } \
    }; \
    /** @brief same as @ref ChildEnum##Enum, see @ref ChildEnum */ \
    typedef ChildEnum::ZFEnumType ChildEnum##Enum; \
    _ZFP_ZFENUM_CONVERTER_DECLARE(ChildEnum) \
    ZFPROPERTY_TYPE_DECLARE(ChildEnum##Enum, ChildEnum##Enum) \
    ZFVAR_CONVERT_DECLARE(ChildEnum##Enum, { \
            ChildEnum *wrap = ZFCastZFObject(ChildEnum *, obj); \
            if(wrap == zfnull) \
            { \
                return (obj == zfnull); \
            } \
            v = (ChildEnum##Enum)wrap->enumValue(); \
        }, { \
            ChildEnum *wrap = zfAlloc(ChildEnum, (ZFEnumValue)v); \
            obj = zfautoObjectCreate(wrap); \
            zfRelease(wrap); \
        })
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_END(ChildEnum) \
            } \
            return d; \
        } \
    public: \
        /** @brief default value for @ref ChildEnum */ \
        static inline ChildEnum::ZFEnumType EnumDefault(void) \
        { \
            return (ChildEnum::ZFEnumType)ChildEnum::EnumValueAtIndex(0); \
        } \
    }; \
    _ZFP_ZFENUM_END_EXTRA(ChildEnum)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_END_WITH_DEFAULT(ChildEnum, defaultEnum) \
            } \
            return d; \
        } \
    public: \
        /** @brief default value for @ref ChildEnum (defaultEnum) */ \
        static inline ChildEnum::ZFEnumType EnumDefault(void) \
        { \
            return ChildEnum::e_##defaultEnum; \
        } \
    }; \
    _ZFP_ZFENUM_END_EXTRA(ChildEnum)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_DEFINE(ChildEnum) \
    ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(ChildEnum##Enum, ChildEnum##Enum) \
    _ZFP_ZFENUM_CONVERTER_DEFINE(ChildEnum) \
    ZFOBJECT_REGISTER(ChildEnum) \
    ZFOBJECT_REGISTER(ChildEnum##Editable)

// ============================================================
// zfflags conversion
/**
 * @brief zfflags conversion using ZFEnum as source, see #zfflagsToString
 */
extern ZF_ENV_EXPORT void zfflagsToString(ZF_IN_OUT zfstring &ret,
                                          ZF_IN const ZFClass *enumClass,
                                          ZF_IN zfflags const &value,
                                          ZF_IN_OPT zfbool includeNotConverted = zftrue,
                                          ZF_IN_OPT zfbool exclusiveMode = zffalse,
                                          ZF_OUT_OPT zfflags *notConverted = zfnull,
                                          ZF_IN_OPT zfchar separatorToken = '|');
/** @brief see #zfflagsToString */
inline zfstring zfflagsToString(ZF_IN const ZFClass *enumClass,
                                ZF_IN zfflags const &value,
                                ZF_IN_OPT zfbool includeNotConverted = zftrue,
                                ZF_IN_OPT zfbool exclusiveMode = zffalse,
                                ZF_OUT_OPT zfflags *notConverted = zfnull,
                                ZF_IN_OPT zfchar separatorToken = '|')
{
    zfstring ret;
    zfflagsToString(ret, enumClass, value, includeNotConverted, exclusiveMode, notConverted, separatorToken);
    return ret;
}
/**
 * @brief zfflags conversion using ZFEnum as source, see #zfflagsFromString
 */
extern ZF_ENV_EXPORT const zfchar *zfflagsFromString(ZF_OUT zfflags &ret,
                                                     ZF_IN const ZFClass *enumClass,
                                                     ZF_IN const zfchar *src,
                                                     ZF_IN_OPT zfindex srcLen = zfindexMax,
                                                     ZF_IN_OPT zfchar separatorToken = '|');

// ============================================================
#define _ZFP_ZFENUM_CONVERTER_DECLARE(EnumName) \
    /** @brief see @ref EnumName, return empty string if error */ \
    extern ZF_ENV_EXPORT void EnumName##ToString(ZF_IN_OUT zfstring &ret, ZF_IN EnumName *const &value); \
    /** @brief see @ref EnumName, return empty string if error */ \
    inline zfstring EnumName##ToString(ZF_IN EnumName *const &value) \
    { \
        zfstring ret; \
        EnumName##ToString(ret, value); \
        return ret; \
    } \
    /** @brief see @ref EnumName, return enum object if success */ \
    extern ZF_ENV_EXPORT const zfchar *EnumName##FromString(ZF_OUT zfautoObject &ret, \
                                                            ZF_IN const zfchar *src, \
                                                            ZF_IN_OPT zfindex srcLen = zfindexMax); \
    /** @brief see @ref EnumName, return empty string if error */ \
    extern ZF_ENV_EXPORT void EnumName##EnumToString(ZF_IN_OUT zfstring &ret, ZF_IN EnumName##Enum const &value); \
    /** @brief see @ref EnumName, return empty string if error */ \
    inline zfstring EnumName##EnumToString(ZF_IN EnumName##Enum const &value) \
    { \
        zfstring ret; \
        EnumName##EnumToString(ret, value); \
        return ret; \
    } \
    /** @brief see @ref EnumName */ \
    extern ZF_ENV_EXPORT const zfchar *EnumName##EnumFromString(ZF_OUT EnumName##Enum &ret, \
                                                                ZF_IN const zfchar *src, \
                                                                ZF_IN_OPT zfindex srcLen = zfindexMax); \
    ZFOUTPUT_TYPE_DECLARE(EnumName##Enum) \
    ZFOUTPUT_TYPE(const EnumName##Enum *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}}) \
    ZFOUTPUT_TYPE(EnumName##Enum *, {output << (const EnumName##Enum *)v;}) \
    ZFINPUT_TYPE_DECLARE(EnumName##Enum, EnumName##Enum) \
    ZFCOMPARER_DEFAULT_DECLARE(EnumName##Enum, EnumName##Enum, { \
            return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable); \
        })
#define _ZFP_ZFENUM_CONVERTER_DEFINE(EnumName) \
    void EnumName##ToString(ZF_IN_OUT zfstring &ret, ZF_IN EnumName *const &value) \
    { \
        ret += ((value == zfnull) ? zfText("") : value->enumName()); \
    } \
    const zfchar *EnumName##FromString(ZF_OUT zfautoObject &ret, \
                                       ZF_IN const zfchar *src, \
                                       ZF_IN_OPT zfindex srcLen /* = zfindexMax */) \
    { \
        if(zfsncmp(src, ZFEnumValueNameInvalid, srcLen) == 0) \
        { \
            ret = zfautoObjectCreate(zflineAlloc(EnumName, ZFEnumValueInvalid)); \
            return zfnull; \
        } \
        ZFEnumValue tmpValue = EnumName::EnumValueForName( \
            (srcLen == zfindexMax) ? src : zfstring(src, srcLen).cString()); \
        if(tmpValue == ZFEnumValueInvalid) \
        { \
            return src; \
        } \
        else \
        { \
            ret = zfautoObjectCreate(zflineAlloc(EnumName, tmpValue)); \
            return zfnull; \
        } \
    } \
    void EnumName##EnumToString(ZF_IN_OUT zfstring &ret, ZF_IN EnumName##Enum const &value) \
    { \
        ret += EnumName::EnumNameForValue(value); \
    } \
    const zfchar *EnumName##EnumFromString(ZF_OUT EnumName##Enum &ret, \
                                           ZF_IN const zfchar *src, \
                                           ZF_IN_OPT zfindex srcLen /* = zfindexMax */) \
    { \
        if(zfsncmp(src, ZFEnumValueNameInvalid, srcLen) == 0) \
        { \
            ret = (EnumName##Enum)ZFEnumValueInvalid; \
            return zfnull; \
        } \
        ret = (EnumName##Enum)EnumName::EnumValueForName( \
            (srcLen == zfindexMax) ? src : zfstring(src, srcLen).cString()); \
        return (((ZFEnumValue)ret == ZFEnumValueInvalid) ? src : zfnull); \
    } \
    ZFOUTPUT_TYPE_DEFINE(EnumName##Enum, {output << EnumName::EnumNameForValue(v);}) \
    ZFINPUT_TYPE_DEFINE(EnumName##Enum, EnumName##Enum, \
    { \
        zfstring tmp; \
        ZFInputCallbackReadUntil(tmp, input); \
        return (EnumName##EnumFromString(v, tmp.cString()) == zfnull); \
    })

// ============================================================
#define _ZFP_ZFENUM_FLAGS_DECLARE(EnumName, EnumFlagsName, defaultFix) \
    /** @brief EnumName as #zfflags, see @ref EnumName, @ref EnumFlagsName##ToString, @ref EnumFlagsName##FromString */ \
    zffinal zfclassLikePOD ZF_ENV_EXPORT EnumFlagsName : zfextendsLikePOD ZFEnumFlagsBase \
    { \
        defaultFix \
    public: \
        /** @cond ZFPrivateDoc */ \
        EnumFlagsName(void) : flags(0) {} \
        EnumFlagsName(ZF_IN const ZFEnumValue &flags) : flags(flags) {} \
        EnumFlagsName(ZF_IN const EnumName##Enum &flags) : flags((ZFEnumValue)flags) {} \
        EnumFlagsName(ZF_IN const EnumFlagsName &ref) : flags(ref.flags) {} \
        virtual ~EnumFlagsName(void) {} \
    public: \
        const ZFEnumValue &enumValue(void) const {return this->flags;} \
        void enumValueSet(ZF_IN const ZFEnumValue &flags) {this->flags = flags;} \
        void enumValueSet(ZF_IN const EnumName##Enum &flags) {this->flags = (ZFEnumValue)flags;} \
    public: \
        operator const ZFEnumValue &(void) const {return this->flags;} \
        EnumFlagsName &operator = (ZF_IN const ZFEnumValue &flags) {this->flags = flags; return *this;} \
        EnumFlagsName &operator = (ZF_IN const EnumName##Enum &flags) {this->flags = (ZFEnumValue)flags; return *this;} \
        EnumFlagsName &operator = (ZF_IN const EnumFlagsName &ref) {this->flags = ref.flags; return *this;} \
        zfbool operator == (ZF_IN const ZFEnumValue &flags) const {return (this->flags == flags);} \
        zfbool operator == (ZF_IN const EnumName##Enum &flags) const {return (this->flags == (ZFEnumValue)flags);} \
        zfbool operator == (ZF_IN const EnumFlagsName &ref) const {return (this->flags == ref.flags);} \
        zfbool operator != (ZF_IN const ZFEnumValue &flags) const {return (this->flags != flags);} \
        zfbool operator != (ZF_IN const EnumName##Enum &flags) const {return (this->flags != (ZFEnumValue)flags);} \
        zfbool operator != (ZF_IN const EnumFlagsName &ref) const {return (this->flags != ref.flags);} \
        EnumFlagsName &operator |= (ZF_IN const ZFEnumValue &flags) {this->flags |= flags; return *this;} \
        EnumFlagsName &operator |= (ZF_IN const EnumName##Enum &flags) {this->flags |= (ZFEnumValue)flags; return *this;} \
        EnumFlagsName &operator |= (ZF_IN const EnumFlagsName &ref) {this->flags |= ref.flags; return *this;} \
        EnumFlagsName &operator &= (ZF_IN const ZFEnumValue &flags) {this->flags &= flags; return *this;} \
        EnumFlagsName &operator &= (ZF_IN const EnumName##Enum &flags) {this->flags &= (ZFEnumValue)flags; return *this;} \
        EnumFlagsName &operator &= (ZF_IN const EnumFlagsName &ref) {this->flags &= ref.flags; return *this;} \
    public: \
        virtual void objectInfoT(ZF_IN_OUT zfstring &ret) const; \
        virtual inline zfstring objectInfo(void) const \
        { \
            zfstring ret; \
            this->objectInfoT(ret); \
            return ret; \
        } \
        /** @endcond */ \
    private: \
        ZFEnumValue flags; \
    };
#define _ZFP_ZFENUM_FLAGS_CONVERTER_DECLARE(EnumName, EnumFlagsName) \
    /** @brief see @ref EnumName, #zfflagsToString */ \
    extern ZF_ENV_EXPORT void EnumFlagsName##ToString(ZF_IN_OUT zfstring &ret, \
                                                      ZF_IN EnumFlagsName const &value, \
                                                      ZF_IN_OPT zfbool includeNotConverted = zftrue, \
                                                      ZF_IN_OPT zfbool exclusiveMode = zffalse, \
                                                      ZF_OUT_OPT zfflags *notConverted = zfnull, \
                                                      ZF_IN_OPT zfchar separatorToken = '|'); \
    /** @brief see @ref EnumName, #zfflagsToString */ \
    inline zfstring EnumFlagsName##ToString(ZF_IN EnumFlagsName const &value, \
                                            ZF_IN_OPT zfbool includeNotConverted = zftrue, \
                                            ZF_IN_OPT zfbool exclusiveMode = zffalse, \
                                            ZF_OUT_OPT zfflags *notConverted = zfnull, \
                                            ZF_IN_OPT zfchar separatorToken = '|') \
    { \
        zfstring ret; \
        EnumFlagsName##ToString(ret, value, includeNotConverted, exclusiveMode, notConverted, separatorToken); \
        return ret; \
    } \
    /** @brief see @ref EnumName, #zfflagsFromString */ \
    extern ZF_ENV_EXPORT const zfchar *EnumFlagsName##FromString(ZF_OUT EnumFlagsName &ret, \
                                                                 ZF_IN const zfchar *src, \
                                                                 ZF_IN_OPT zfindex srcLen = zfindexMax, \
                                                                 ZF_IN_OPT zfchar separatorToken = '|'); \
    ZFOUTPUT_TYPE_DECLARE(EnumFlagsName) \
    ZFOUTPUT_TYPE(const EnumFlagsName *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}}) \
    ZFOUTPUT_TYPE(EnumFlagsName *, {output << (const EnumFlagsName *)v;}) \
    ZFINPUT_TYPE_DECLARE(EnumFlagsName, EnumFlagsName)
#define _ZFP_ZFENUM_FLAGS_CONVERTER_DEFINE(EnumName, EnumFlagsName) \
    void EnumFlagsName##ToString(ZF_IN_OUT zfstring &ret, \
                                 ZF_IN EnumFlagsName const &value, \
                                 ZF_IN_OPT zfbool includeNotConverted /* = zftrue */, \
                                 ZF_IN_OPT zfbool exclusiveMode /* = zffalse */, \
                                 ZF_OUT_OPT zfflags *notConverted /* = zfnull */, \
                                 ZF_IN_OPT zfchar separatorToken /* = '|' */) \
    { \
        zfflagsToString(ret, EnumName::ClassData(), \
            (zfflags)value.enumValue(), includeNotConverted, exclusiveMode, notConverted, separatorToken); \
    } \
    const zfchar *EnumFlagsName##FromString(ZF_OUT EnumFlagsName &ret, \
                                            ZF_IN const zfchar *src, \
                                            ZF_IN_OPT zfindex srcLen /* = zfindexMax */, \
                                            ZF_IN_OPT zfchar separatorToken /* = '|' */) \
    { \
        zfflags flags = 0; \
        const zfchar *result = zfflagsFromString(flags, \
            EnumName::ClassData(), \
            src, srcLen, separatorToken); \
        ret.enumValueSet((ZFEnumValue)flags); \
        return result; \
    } \
    ZFOUTPUT_TYPE_DEFINE(EnumFlagsName, {output << v.objectInfo();}) \
    ZFINPUT_TYPE_DEFINE(EnumFlagsName, EnumFlagsName, \
    { \
        zfstring tmp; \
        ZFInputCallbackReadUntil(tmp, input); \
        return (EnumFlagsName##FromString(v, tmp.cString()) == zfnull); \
    })
/**
 * @brief util macro to declare a flag set type for a ZFEnum type
 *
 * usage:
 * @code
 *   // in .h file
 *   ZFENUM_FLAGS_DECLARE(YourEnumType, YourEnumFlagsType)
 *
 *   // in cpp file
 *   ZFENUM_FLAGS_DEFINE(YourEnumType, YourEnumFlagsType)
 * @endcode
 * declare those types/methods:
 * -  YourEnumFlagsType:\n
 *   flag set type, you may use it like #zfflags
 * -  toString converter:\n
 *   void YourEnumFlagsTypeToString(ZF_IN_OUT zfstring &ret,
 *                                  ZF_IN YourEnumFlagsType const &value,
 *                                  ZF_IN_OPT zfbool includeNotConverted = zftrue,
 *                                  ZF_IN_OPT zfbool exclusiveMode = zffalse,
 *                                  ZF_OUT_OPT zfflags *notConverted = zfnull,
 *                                  ZF_IN_OPT zfchar separatorToken = '|');
 *   zfstring YourEnumFlagsTypeToString(ZF_IN YourEnumFlagsType const &value,
 *                                      ZF_IN_OPT zfbool includeNotConverted = zftrue,
 *                                      ZF_IN_OPT zfbool exclusiveMode = zffalse,
 *                                      ZF_OUT_OPT zfflags *notConverted = zfnull,
 *                                      ZF_IN_OPT zfchar separatorToken = '|');
 * -  fromString converter:\n
 *   const zfchar *YourEnumFlagsTypeFromString(ZF_OUT YourEnumFlagsType &ret,
 *                                             ZF_IN const zfchar *src,
 *                                             ZF_IN_OPT zfindex srcLen = zfindexMax,
 *                                             ZF_IN_OPT zfchar separatorToken = '|');
 */
#define ZFENUM_FLAGS_DECLARE(EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_FLAGS_DECLARE(EnumName, EnumFlagsName, ZFM_EMPTY()) \
    ZFPROPERTY_TYPE_DECLARE(EnumFlagsName, EnumFlagsName) \
    _ZFP_ZFENUM_FLAGS_CONVERTER_DECLARE(EnumName, EnumFlagsName) \
    ZFVAR_CONVERT_WRAPPER_DECLARE(EnumFlagsName, EnumFlagsName) \
    ZFVAR_CONVERT_DECLARE_BY_WRAPPER(EnumFlagsName, EnumFlagsName)
#define _ZFP_ZFENUM_FLAGS_DEFAULT_EXPAND(EnumName, EnumFlagsName, defaultValue) \
    public: \
        /** @brief default value for EnumFlagsName (defaultValue) */ \
        static inline ZFEnumValue EnumDefault(void) \
        { \
            return defaultValue; \
        }
/**
 * @brief see #ZFENUM_FLAGS_DECLARE, declare with default value,
 *   which can be accessed by YourEnumFlagsType::EnumDefault()
 */
#define ZFENUM_FLAGS_DECLARE_WITH_DEFAULT(EnumName, EnumFlagsName, defaultValue) \
    _ZFP_ZFENUM_FLAGS_DECLARE(EnumName, EnumFlagsName, _ZFP_ZFENUM_FLAGS_DEFAULT_EXPAND(EnumName, EnumFlagsName, defaultValue)) \
    ZFPROPERTY_TYPE_DECLARE(EnumFlagsName, EnumFlagsName) \
    _ZFP_ZFENUM_FLAGS_CONVERTER_DECLARE(EnumName, EnumFlagsName) \
    ZFVAR_CONVERT_WRAPPER_DECLARE(EnumFlagsName, EnumFlagsName) \
    ZFVAR_CONVERT_DECLARE_BY_WRAPPER(EnumFlagsName, EnumFlagsName)

/** @brief see #ZFENUM_FLAGS_DECLARE */
#define ZFENUM_FLAGS_DEFINE(EnumName, EnumFlagsName) \
    ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(EnumFlagsName, EnumFlagsName) \
    _ZFP_ZFENUM_FLAGS_CONVERTER_DEFINE(EnumName, EnumFlagsName) \
    void EnumFlagsName::objectInfoT(ZF_IN_OUT zfstring &ret) const \
    { \
        zfflagsToString(ret, EnumName::ClassData(), (zfflags)this->enumValue()); \
    } \
    ZFVAR_CONVERT_WRAPPER_DEFINE(EnumFlagsName)

// ============================================================
// ZFEnumFlagsBase
/**
 * @brief base class of enum flags
 */
zfclassLikePOD ZF_ENV_EXPORT ZFEnumFlagsBase
{
public:
    /** @cond ZFPrivateDoc */
    ZFEnumFlagsBase(void)
    {
    }
    virtual ~ZFEnumFlagsBase(void)
    {
    }
    /** @endcond */

public:
    /**
     * @brief get a short info
     */
    virtual zfstring objectInfo(void) const = 0;
};

ZFOUTPUT_TYPE_DECLARE(ZFEnumFlagsBase)
ZFOUTPUT_TYPE(const ZFEnumFlagsBase *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFEnumFlagsBase *, {output << (const ZFEnumFlagsBase *)v;})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnumDef_h_

