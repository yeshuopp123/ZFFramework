/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFValue.h
 * @brief store a value as a ZFObject
 */

#ifndef _ZFI_ZFValue_h_
#define _ZFI_ZFValue_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE

/**
 * @brief type of ZFValue
 */
ZFENUM_BEGIN(ZFValueType)
    _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
    ZFENUM_VALUE(bool)               /**< zfbool */
    ZFENUM_VALUE(char)               /**< zfchar */
    ZFENUM_VALUE(int)                /**< zfint */
    ZFENUM_VALUE(unsignedInt)        /**< zfuint */
    ZFENUM_VALUE(index)              /**< zfindex */
    ZFENUM_VALUE(float)              /**< zffloat */
    ZFENUM_VALUE(double)             /**< zfdouble */
    ZFENUM_VALUE(longDouble)         /**< zflongdouble */
    ZFENUM_VALUE(time)               /**< zftimet */
    ZFENUM_VALUE(flags)              /**< zfflags */
    ZFENUM_VALUE(identity)           /**< zfidentity */
    ZFENUM_VALUE(serializableData)   /**< serializable data */
ZFENUM_SEPARATOR(ZFValueType)
    _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
    ZFENUM_VALUE_REGISTER(bool)
    ZFENUM_VALUE_REGISTER(char)
    ZFENUM_VALUE_REGISTER(int)
    ZFENUM_VALUE_REGISTER(unsignedInt)
    ZFENUM_VALUE_REGISTER(index)
    ZFENUM_VALUE_REGISTER(float)
    ZFENUM_VALUE_REGISTER(double)
    ZFENUM_VALUE_REGISTER(longDouble)
    ZFENUM_VALUE_REGISTER(time)
    ZFENUM_VALUE_REGISTER(flags)
    ZFENUM_VALUE_REGISTER(identity)
    ZFENUM_VALUE_REGISTER(serializableData)
ZFENUM_END(ZFValueType)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFValue_valueType zfText("valueType")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFValue_value zfText("value")

zfclassFwd _ZFP_ZFValuePrivate;
/**
 * @brief readonly value container as a ZFObject
 *
 * ZFValue store a value by a union,
 * so try to access the value with the right type\n
 * it's recommended to use createXXX instead of #zfAlloc to make a ZFValue,
 * since createXXX may return a cached ZFValue object for performance and memory usage
 * \n
 * serializable data:
 * @code
 *   <ZFValue
 *       valueType="int" // optional, int type by default
 *       value="" // optional, type's default value by default
 *   />
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFValue : zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFValue, ZFObject)
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
        this->valueSet(ZFCastZFObjectUnchecked(zfself *, anotherObj));
    }

public:
    /**
     * @brief init from another value
     */
    virtual ZFObject *objectOnInit(ZF_IN ZFValue *another);
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
    zfoverride
    virtual zfidentity objectHash(void);
    /**
     * @brief compare with anotherObj
     *
     * two ZFValue is considered as same only if
     * value type and value both the same\n
     * \n
     * if you want to compare two different type with logical value compare,
     * you should use compareType, while type is "Bool, Int", etc\n
     * \n
     * while compare with logical value,
     * we would cast them to appropriate type first\n
     * the cast priority is:\n
     *   low accuracy < high accuracy\n
     *   signed value < unsigned value\n
     * for example:\n
     * -  when comparing zfint and zfuint, would cast to zfuint
     * -  when comparing zfchar and zfint, would cast to zfuint
     * -  when comparing zfint and zffloat, would cast to zfdouble
     *
     * @note serializable type can only compared with serializable type,
     *   and it's compared by #ZFSerializableData::objectCompare
     * @note when compare zfbool type and other type,
     *   the zfbool type's value is regarded as 0 if false and 1 if true
     * @note it's your responsibility to make sure the comparation
     *   between different types is meaningful
     */
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief ture if current value is convertable to toType
     *
     * while converting, zfbool type is regarded as (int)1 if true, and (int)0 if false\n
     * other types would convert by #ZFCastStatic\n
     * a serializable type can only convert to a serializable type
     */
    virtual zfbool valueConvertableTo(ZF_IN ZFValueTypeEnum toType);

public:
    #define _ZFP_ZFValue_method_DECLARE(TypeName, T) \
        public: \
            /** @brief create a new ZFValue */ \
            static zfautoObject TypeName##ValueCreate(ZF_IN T const &v); \
            /** @brief see #objectCompare */ \
            virtual ZFCompareResult TypeName##ValueCompare(ZF_IN T const &v); \
            /** @brief try to access the value, convert if able, assert fail if not convertable */ \
            virtual T TypeName##Value(void); \
            /** @brief directly access value as raw type, no type safe check and you must ensure type is matched */ \
            virtual T const &TypeName##ValueAccess(void); \
        protected: \
            /** @brief set new value */ \
            virtual void TypeName##ValueSet(ZF_IN T const &v);

    _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
    _ZFP_ZFValue_method_DECLARE(bool, zfbool)
    _ZFP_ZFValue_method_DECLARE(char, zfchar)
    _ZFP_ZFValue_method_DECLARE(int, zfint)
    _ZFP_ZFValue_method_DECLARE(unsignedInt, zfuint)
    _ZFP_ZFValue_method_DECLARE(index, zfindex)
    _ZFP_ZFValue_method_DECLARE(float, zffloat)
    _ZFP_ZFValue_method_DECLARE(double, zfdouble)
    _ZFP_ZFValue_method_DECLARE(longDouble, zflongdouble)
    _ZFP_ZFValue_method_DECLARE(time, zftimet)
    _ZFP_ZFValue_method_DECLARE(flags, zfflags)
    _ZFP_ZFValue_method_DECLARE(identity, zfidentity)
    _ZFP_ZFValue_method_DECLARE(serializableData, ZFSerializableData)

public:
    /**
     * @brief get the type of ZFValue
     */
    virtual ZFValueTypeEnum valueType(void);

    /**
     * @brief get the type name of ZFValue
     */
    virtual const zfchar *valueTypeName(void);

    /**
     * @brief return a string describe the value in short
     */
    virtual void valueStringT(ZF_IN_OUT zfstring &ret);
    /** @brief see #valueStringT */
    virtual inline zfstring valueString(void)
    {
        zfstring ret;
        this->valueStringT(ret);
        return ret;
    }

protected:
    /**
     * @brief set new value from another, do nothing if another is null
     */
    virtual void valueSet(ZF_IN ZFValue *another);

private:
    _ZFP_ZFValuePrivate *d;
};

/**
 * @brief editable ZFValue
 */
zfclass ZF_ENV_EXPORT ZFValueEditable : zfextends ZFValue
{
    ZFOBJECT_DECLARE(ZFValueEditable, ZFValue)

public:
    zfoverride
    virtual ZFObject *objectOnInit(ZF_IN ZFValue *another) {return zfsuper::objectOnInit(another);}
public:
    zfoverride
    virtual ZFObject *objectOnInit(void) {return zfsuper::objectOnInit();}
    zfoverride
    virtual void objectOnDealloc(void) {zfsuper::objectOnDealloc();}

public:
    zfoverride
    virtual void valueSet(ZF_IN ZFValue *another)
    {
        zfsuper::valueSet(another);
    }

public:
    #define _ZFP_ZFValueEditable_method_DECLARE(TypeName, T) \
        /** @brief create a new ZFValueEditable */ \
        static zfautoObject TypeName##ValueCreate(ZF_IN T const &v); \
        zfoverride \
        virtual void TypeName##ValueSet(ZF_IN T const &v) {zfsuper::TypeName##ValueSet(v);}

    _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
    _ZFP_ZFValueEditable_method_DECLARE(bool, zfbool)
    _ZFP_ZFValueEditable_method_DECLARE(char, zfchar)
    _ZFP_ZFValueEditable_method_DECLARE(int, zfint)
    _ZFP_ZFValueEditable_method_DECLARE(unsignedInt, zfuint)
    _ZFP_ZFValueEditable_method_DECLARE(index, zfindex)
    _ZFP_ZFValueEditable_method_DECLARE(float, zffloat)
    _ZFP_ZFValueEditable_method_DECLARE(double, zfdouble)
    _ZFP_ZFValueEditable_method_DECLARE(longDouble, zflongdouble)
    _ZFP_ZFValueEditable_method_DECLARE(time, zftimet)
    _ZFP_ZFValueEditable_method_DECLARE(flags, zfflags)
    _ZFP_ZFValueEditable_method_DECLARE(identity, zfidentity)
    _ZFP_ZFValueEditable_method_DECLARE(serializableData, ZFSerializableData)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFValue_h_

