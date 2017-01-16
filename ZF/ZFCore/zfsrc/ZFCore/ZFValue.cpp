/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFValue.h"

ZF_NAMESPACE_GLOBAL_BEGIN

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
#define _ZFP_ZFValue_expandValue_bool(value) ((value) ? 1 : 0)
#define _ZFP_ZFValue_expandValue_char(value) (value)
#define _ZFP_ZFValue_expandValue_int(value) (value)
#define _ZFP_ZFValue_expandValue_unsignedInt(value) (value)
#define _ZFP_ZFValue_expandValue_index(value) (value)
#define _ZFP_ZFValue_expandValue_int8(value) (value)
#define _ZFP_ZFValue_expandValue_unsignedInt8(value) (value)
#define _ZFP_ZFValue_expandValue_int16(value) (value)
#define _ZFP_ZFValue_expandValue_unsignedInt16(value) (value)
#define _ZFP_ZFValue_expandValue_int32(value) (value)
#define _ZFP_ZFValue_expandValue_unsignedInt32(value) (value)
#define _ZFP_ZFValue_expandValue_int64(value) (value)
#define _ZFP_ZFValue_expandValue_unsignedInt64(value) (value)
#define _ZFP_ZFValue_expandValue_float(value) (value)
#define _ZFP_ZFValue_expandValue_double(value) (value)
#define _ZFP_ZFValue_expandValue_longDouble(value) (value)
#define _ZFP_ZFValue_expandValue_time(value) (value)
#define _ZFP_ZFValue_expandValue_flags(value) (value)
#define _ZFP_ZFValue_expandValue_identity(value) (value)
#define _ZFP_ZFValue_expandValue_serializableData(value) (value)

#define _ZFP_ZFValue_expandValue(TypeName, value) \
    _ZFP_ZFValue_expandValue_##TypeName(value)

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
#define _ZFP_ZFValue_initValue_bool() ((zfbool)zffalse)
#define _ZFP_ZFValue_initValue_char() ((zfchar)'\0')
#define _ZFP_ZFValue_initValue_int() ((zfint)0)
#define _ZFP_ZFValue_initValue_unsignedInt() ((zfuint)0)
#define _ZFP_ZFValue_initValue_index() ((zfindex)0)
#define _ZFP_ZFValue_initValue_int8() ((zfint8)0)
#define _ZFP_ZFValue_initValue_unsignedInt8() ((zfuint8)0)
#define _ZFP_ZFValue_initValue_int16() ((zfint16)0)
#define _ZFP_ZFValue_initValue_unsignedInt16() ((zfuint16)0)
#define _ZFP_ZFValue_initValue_int32() ((zfint32)0)
#define _ZFP_ZFValue_initValue_unsignedInt32() ((zfuint32)0)
#define _ZFP_ZFValue_initValue_int64() ((zfint64)0)
#define _ZFP_ZFValue_initValue_unsignedInt64() ((zfuint64)0)
#define _ZFP_ZFValue_initValue_float() ((zffloat)0)
#define _ZFP_ZFValue_initValue_double() ((zfdouble)0)
#define _ZFP_ZFValue_initValue_longDouble() ((zflongdouble)0)
#define _ZFP_ZFValue_initValue_time() ((zftimet)0)
#define _ZFP_ZFValue_initValue_flags() ((zfflags)0)
#define _ZFP_ZFValue_initValue_identity() ((zfidentity)0)
#define _ZFP_ZFValue_initValue_serializableData() void

#define _ZFP_ZFValue_initValue(TypeName) \
    _ZFP_ZFValue_initValue_##TypeName()

ZFENUM_DEFINE(ZFValueType)

// ============================================================
// ZFValue cleaner
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFValueDataHolder, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFValueDataHolder)
{
    if(!valueToDestroy.isEmpty())
    {
        ZFCoreArrayPOD<ZFValue **> tmp;
        tmp.copyFrom(valueToDestroy);
        for(zfindex i = 0; i < tmp.count(); ++i)
        {
            zfReleaseWithoutLeakTest(*(tmp[i]));
            *(tmp[i]) = zfnull;
        }
    }
}
public:
    ZFCoreArrayPOD<ZFValue **> valueToDestroy;
ZF_GLOBAL_INITIALIZER_END(ZFValueDataHolder)
#define _ZFP_ZFValueAddToDestroyArray(zfValueInstance) \
    (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFValueDataHolder)->valueToDestroy.add(&zfValueInstance))

// ============================================================
// _ZFP_ZFValuePrivate
zffinal zfclassNotPOD _ZFP_ZFValuePrivate
{
public:
    ZFValueTypeEnum type;
    #define _ZFP_ZFValue_internal_value_DEFINE(TypeName, Type) \
        Type v_##TypeName;
    typedef union
    {
        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_internal_value_DEFINE(bool, zfbool)
        _ZFP_ZFValue_internal_value_DEFINE(char, zfchar)
        _ZFP_ZFValue_internal_value_DEFINE(int, zfint)
        _ZFP_ZFValue_internal_value_DEFINE(unsignedInt, zfuint)
        _ZFP_ZFValue_internal_value_DEFINE(index, zfindex)
        _ZFP_ZFValue_internal_value_DEFINE(int8, zfint8)
        _ZFP_ZFValue_internal_value_DEFINE(unsignedInt8, zfuint8)
        _ZFP_ZFValue_internal_value_DEFINE(int16, zfint16)
        _ZFP_ZFValue_internal_value_DEFINE(unsignedInt16, zfuint16)
        _ZFP_ZFValue_internal_value_DEFINE(int32, zfint32)
        _ZFP_ZFValue_internal_value_DEFINE(unsignedInt32, zfuint32)
        _ZFP_ZFValue_internal_value_DEFINE(int64, zfint64)
        _ZFP_ZFValue_internal_value_DEFINE(unsignedInt64, zfuint64)
        _ZFP_ZFValue_internal_value_DEFINE(float, zffloat)
        _ZFP_ZFValue_internal_value_DEFINE(double, zfdouble)
        _ZFP_ZFValue_internal_value_DEFINE(longDouble, zflongdouble)
        _ZFP_ZFValue_internal_value_DEFINE(time, zftimet)
        _ZFP_ZFValue_internal_value_DEFINE(flags, zfflags)
        _ZFP_ZFValue_internal_value_DEFINE(identity, zfidentity)
        _ZFP_ZFValue_internal_value_DEFINE(serializableData, ZFSerializableData *)
    } _ZFP_ZFValueUnion;
    _ZFP_ZFValueUnion v;
public:
    _ZFP_ZFValuePrivate(void)
    : type(ZFValueType::e_int)
    , v()
    {
    }
    ~_ZFP_ZFValuePrivate(void)
    {
        this->clearValue();
    }
    void clearValue(void)
    {
        if(this->type == ZFValueType::e_serializableData)
        {
            zfdelete(this->v.v_serializableData);
        }
        zfmemset(&v, 0, sizeof(_ZFP_ZFValueUnion));
        this->type = ZFValueType::e_int;
    }
    void valueStringT(ZF_IN_OUT zfstring &ret)
    {
        switch(this->type)
        {
            _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
            case ZFValueType::e_bool:
                ret += (this->v.v_bool ? ZFTOKEN_zfbool_zftrue : ZFTOKEN_zfbool_zffalse);
                return ;
            case ZFValueType::e_char:
            {
                ret += this->v.v_char;
                return ;
            }
            case ZFValueType::e_int:
                zfsFromIntT(ret, this->v.v_int);
                return ;
            case ZFValueType::e_unsignedInt:
                zfsFromIntT(ret, this->v.v_unsignedInt);
                return ;
            case ZFValueType::e_index:
                zfsFromIntT(ret, this->v.v_index);
                return ;
            case ZFValueType::e_int8:
                zfsFromIntT(ret, this->v.v_int8);
                return ;
            case ZFValueType::e_unsignedInt8:
                zfsFromIntT(ret, this->v.v_unsignedInt8);
                return ;
            case ZFValueType::e_int16:
                zfsFromIntT(ret, this->v.v_int16);
                return ;
            case ZFValueType::e_unsignedInt16:
                zfsFromIntT(ret, this->v.v_unsignedInt16);
                return ;
            case ZFValueType::e_int32:
                zfsFromIntT(ret, this->v.v_int32);
                return ;
            case ZFValueType::e_unsignedInt32:
                zfsFromIntT(ret, this->v.v_unsignedInt32);
                return ;
            case ZFValueType::e_int64:
                zfsFromIntT(ret, this->v.v_int64);
                return ;
            case ZFValueType::e_unsignedInt64:
                zfsFromIntT(ret, this->v.v_unsignedInt64);
                return ;
            case ZFValueType::e_float:
                zfsFromFloatT(ret, this->v.v_float);
                return ;
            case ZFValueType::e_double:
                zfsFromFloatT(ret, this->v.v_double);
                return ;
            case ZFValueType::e_longDouble:
                zfsFromFloatT(ret, this->v.v_longDouble);
                return ;
            case ZFValueType::e_time:
                zfsFromIntT(ret, this->v.v_time);
                return ;
            case ZFValueType::e_flags:
                zfsFromIntT(ret, this->v.v_flags, 16);
                return ;
            case ZFValueType::e_identity:
                zfsFromIntT(ret, this->v.v_identity);
                return ;
            case ZFValueType::e_serializableData:
            {
                ret += ZFTOKEN_ZFObjectInfoLeft;
                if(this->v.v_serializableData->itemClass() != zfnull)
                {
                    ret += this->v.v_serializableData->itemClass();
                }
                else
                {
                    ret += ZFTOKEN_zfnull;
                }
                ret += ZFTOKEN_ZFObjectInfoRight;
                return ;
            }
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
};

// ============================================================
// ZFValue
ZFOBJECT_REGISTER(ZFValue)

zfbool ZFValue::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    // valueType
    ZFValueTypeEnum valueType = ZFValueType::EnumDefault();
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFValue_valueType, ZFValueTypeEnum, valueType);

    // value
    switch(valueType)
    {
        #define _ZFP_ZFValue_serializableOnSerializeFromData(TypeName, Type) \
            case ZFValueType::ZFM_CAT(e_, TypeName): \
            { \
                Type v = _ZFP_ZFValue_initValue(TypeName); \
                ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos, \
                    check, ZFSerializableKeyword_ZFValue_value, Type, v); \
                this->TypeName##ValueSet(v); \
                break; \
            }

        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_serializableOnSerializeFromData(bool, zfbool)
        _ZFP_ZFValue_serializableOnSerializeFromData(char, zfchar)
        _ZFP_ZFValue_serializableOnSerializeFromData(int, zfint)
        _ZFP_ZFValue_serializableOnSerializeFromData(unsignedInt, zfuint)
        _ZFP_ZFValue_serializableOnSerializeFromData(index, zfindex)
        _ZFP_ZFValue_serializableOnSerializeFromData(int8, zfint8)
        _ZFP_ZFValue_serializableOnSerializeFromData(unsignedInt8, zfuint8)
        _ZFP_ZFValue_serializableOnSerializeFromData(int16, zfint16)
        _ZFP_ZFValue_serializableOnSerializeFromData(unsignedInt16, zfuint16)
        _ZFP_ZFValue_serializableOnSerializeFromData(int32, zfint32)
        _ZFP_ZFValue_serializableOnSerializeFromData(unsignedInt32, zfuint32)
        _ZFP_ZFValue_serializableOnSerializeFromData(int64, zfint64)
        _ZFP_ZFValue_serializableOnSerializeFromData(unsignedInt64, zfuint64)
        _ZFP_ZFValue_serializableOnSerializeFromData(float, zffloat)
        _ZFP_ZFValue_serializableOnSerializeFromData(double, zfdouble)
        _ZFP_ZFValue_serializableOnSerializeFromData(longDouble, zflongdouble)
        _ZFP_ZFValue_serializableOnSerializeFromData(time, zftimet)
        _ZFP_ZFValue_serializableOnSerializeFromData(flags, zfflags)
        _ZFP_ZFValue_serializableOnSerializeFromData(identity, zfidentity)
        case ZFValueType::e_serializableData:
        {
            ZFSerializableData v;
            const zfchar *valueString = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFValue_value);
            if(valueString != zfnull)
            {
                if(!ZFSerializableDataFromString(v, valueString, zfindexMax, outErrorHint))
                {
                    if(outErrorPos != zfnull)
                    {
                        *outErrorPos = serializableData;
                    }
                    return zffalse;
                }
            }
            this->serializableDataValueSet(v);
            break;
        }
        default:
            zfCoreCriticalShouldNotGoHere();
            return zffalse;
    }

    return zftrue;
}
zfbool ZFValue::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                              ZF_IN ZFSerializable *referencedOwnerOrNull,
                                              ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHint, ref,
        ZFSerializableKeyword_ZFValue_valueType, ZFValueTypeEnum, this->valueType(), ref->valueType(), ZFValueType::EnumDefault());

    // value
    switch(this->valueType())
    {
        #define _ZFP_ZFValue_serializableOnSerializeToData(TypeName, Type) \
            case ZFValueType::ZFM_CAT(e_, TypeName): \
            { \
                ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHint, ref, \
                    ZFSerializableKeyword_ZFValue_value, Type, this->TypeName##ValueAccess(), ref->TypeName##ValueAccess(), _ZFP_ZFValue_initValue(TypeName)); \
                break; \
            }

        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_serializableOnSerializeToData(bool, zfbool)
        _ZFP_ZFValue_serializableOnSerializeToData(char, zfchar)
        _ZFP_ZFValue_serializableOnSerializeToData(int, zfint)
        _ZFP_ZFValue_serializableOnSerializeToData(unsignedInt, zfuint)
        _ZFP_ZFValue_serializableOnSerializeToData(index, zfindex)
        _ZFP_ZFValue_serializableOnSerializeToData(int8, zfint8)
        _ZFP_ZFValue_serializableOnSerializeToData(unsignedInt8, zfuint8)
        _ZFP_ZFValue_serializableOnSerializeToData(int16, zfint16)
        _ZFP_ZFValue_serializableOnSerializeToData(unsignedInt16, zfuint16)
        _ZFP_ZFValue_serializableOnSerializeToData(int32, zfint32)
        _ZFP_ZFValue_serializableOnSerializeToData(unsignedInt32, zfuint32)
        _ZFP_ZFValue_serializableOnSerializeToData(int64, zfint64)
        _ZFP_ZFValue_serializableOnSerializeToData(unsignedInt64, zfuint64)
        _ZFP_ZFValue_serializableOnSerializeToData(float, zffloat)
        _ZFP_ZFValue_serializableOnSerializeToData(double, zfdouble)
        _ZFP_ZFValue_serializableOnSerializeToData(longDouble, zflongdouble)
        _ZFP_ZFValue_serializableOnSerializeToData(time, zftimet)
        _ZFP_ZFValue_serializableOnSerializeToData(flags, zfflags)
        _ZFP_ZFValue_serializableOnSerializeToData(identity, zfidentity)
        case ZFValueType::e_serializableData:
        {
            if((ref == zfnull && !this->serializableDataValueAccess().isEmpty())
                || (ref != zfnull && this->serializableDataValueAccess().objectCompare(ref->serializableDataValueAccess()) != ZFCompareTheSame))
            {
                zfstring valueString;
                if(!ZFSerializableDataToString(valueString, this->serializableDataValueAccess(), outErrorHint))
                {
                    return zffalse;
                }
                serializableData.attributeSet(ZFSerializableKeyword_ZFValue_value, valueString);
            }
            break;
        }
        default:
            zfCoreCriticalShouldNotGoHere();
            return zffalse;
    }

    return zftrue;
}

ZFObject *ZFValue::objectOnInit(ZF_IN ZFValue *another)
{
    this->objectOnInit();
    zfself::valueSet(another);
    return this;
}
ZFObject *ZFValue::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFValuePrivate);
    return this;
}
void ZFValue::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFValue::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    d->valueStringT(ret);
}

// ============================================================
template<typename T_Value>
static inline ZFCompareResult _ZFP_ZFValueCompare(T_Value const &v1, T_Value const &v2)
{
    if(v1 == v2)
    {
        return ZFCompareTheSame;
    }
    else if(v1 < v2)
    {
        return ZFCompareSmaller;
    }
    else
    {
        return ZFCompareGreater;
    }
}
static inline ZFCompareResult _ZFP_ZFValueCompare(float const &v1, float const &v2)
{
    if(zffloatIsSmaller(v1, v2))
    {
        return ZFCompareSmaller;
    }
    else if(zffloatIsGreater(v1, v2))
    {
        return ZFCompareGreater;
    }
    else
    {
        return ZFCompareTheSame;
    }
}
static inline ZFCompareResult _ZFP_ZFValueCompare(double const &v1, double const &v2)
{
    if(zffloatIsSmaller(v1, v2))
    {
        return ZFCompareSmaller;
    }
    else if(zffloatIsGreater(v1, v2))
    {
        return ZFCompareGreater;
    }
    else
    {
        return ZFCompareTheSame;
    }
}
static inline ZFCompareResult _ZFP_ZFValueCompare(long double const &v1, long double const &v2)
{
    if(zffloatIsSmaller(v1, v2))
    {
        return ZFCompareSmaller;
    }
    else if(zffloatIsGreater(v1, v2))
    {
        return ZFCompareGreater;
    }
    else
    {
        return ZFCompareTheSame;
    }
}
static inline ZFCompareResult _ZFP_ZFValueCompare(ZFSerializableData const &v1, ZFSerializableData const &v2)
{
    return v1.objectCompare(v2);
}

zfidentity ZFValue::objectHash(void)
{
    zfidentity ret = zfidentityHash(this->valueType());
    switch(this->valueType())
    {
        #define _ZFP_ZFValue_hash_switch_case(TypeName, Type) \
            case ZFValueType::e_##TypeName: \
                ret = zfidentityHash(ret, ((zfidentity)_ZFP_ZFValue_expandValue(TypeName, this->TypeName##ValueAccess()) & 0xFFFFFFFF)); \
                break;

        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_hash_switch_case(bool, zfbool)
        _ZFP_ZFValue_hash_switch_case(char, zfchar)
        _ZFP_ZFValue_hash_switch_case(int, zfint)
        _ZFP_ZFValue_hash_switch_case(unsignedInt, zfuint)
        _ZFP_ZFValue_hash_switch_case(index, zfindex)
        _ZFP_ZFValue_hash_switch_case(int8, zfint8)
        _ZFP_ZFValue_hash_switch_case(unsignedInt8, zfuint8)
        _ZFP_ZFValue_hash_switch_case(int16, zfint16)
        _ZFP_ZFValue_hash_switch_case(unsignedInt16, zfuint16)
        _ZFP_ZFValue_hash_switch_case(int32, zfint32)
        _ZFP_ZFValue_hash_switch_case(unsignedInt32, zfuint32)
        _ZFP_ZFValue_hash_switch_case(int64, zfint64)
        _ZFP_ZFValue_hash_switch_case(unsignedInt64, zfuint64)
        _ZFP_ZFValue_hash_switch_case(float, zffloat)
        _ZFP_ZFValue_hash_switch_case(double, zfdouble)
        _ZFP_ZFValue_hash_switch_case(longDouble, zflongdouble)
        _ZFP_ZFValue_hash_switch_case(time, zftimet)
        _ZFP_ZFValue_hash_switch_case(flags, zfflags)
        _ZFP_ZFValue_hash_switch_case(identity, zfidentity)
        default:
            zfCoreCriticalShouldNotGoHere();
            return 0;
    }
    return ~ret;
}

ZFCompareResult ZFValue::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    switch(this->valueType())
    {
        #define _ZFP_ZFValue_compare_switch_case(TypeName, Type) \
            case ZFValueType::ZFM_CAT(e_, TypeName): \
                return _ZFP_ZFValueCompare(this->TypeName##ValueAccess(), another->TypeName##ValueAccess());

        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_compare_switch_case(bool, zfbool)
        _ZFP_ZFValue_compare_switch_case(char, zfchar)
        _ZFP_ZFValue_compare_switch_case(int, zfint)
        _ZFP_ZFValue_compare_switch_case(unsignedInt, zfuint)
        _ZFP_ZFValue_compare_switch_case(index, zfindex)
        _ZFP_ZFValue_compare_switch_case(int8, zfint8)
        _ZFP_ZFValue_compare_switch_case(unsignedInt8, zfuint8)
        _ZFP_ZFValue_compare_switch_case(int16, zfint16)
        _ZFP_ZFValue_compare_switch_case(unsignedInt16, zfuint16)
        _ZFP_ZFValue_compare_switch_case(int32, zfint32)
        _ZFP_ZFValue_compare_switch_case(unsignedInt32, zfuint32)
        _ZFP_ZFValue_compare_switch_case(int64, zfint64)
        _ZFP_ZFValue_compare_switch_case(unsignedInt64, zfuint64)
        _ZFP_ZFValue_compare_switch_case(float, zffloat)
        _ZFP_ZFValue_compare_switch_case(double, zfdouble)
        _ZFP_ZFValue_compare_switch_case(longDouble, zflongdouble)
        _ZFP_ZFValue_compare_switch_case(time, zftimet)
        _ZFP_ZFValue_compare_switch_case(flags, zfflags)
        _ZFP_ZFValue_compare_switch_case(identity, zfidentity)
        _ZFP_ZFValue_compare_switch_case(serializableData, ZFSerializableData)
        default:
            zfCoreCriticalShouldNotGoHere();
            return ZFCompareUncomparable;
    }
}

#define _ZFP_ZFValueCompareCast_DEFINE_1(TypeName, CastType) \
    typedef CastType _ZFP_ZFValueCompareCast_##TypeName##_##TypeName;
#define _ZFP_ZFValueCompareCast_DEFINE_2(TypeName0, TypeName1, CastType) \
    typedef CastType _ZFP_ZFValueCompareCast_##TypeName0##_##TypeName1; \
    typedef CastType _ZFP_ZFValueCompareCast_##TypeName1##_##TypeName0;

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValueCompareCast_DEFINE_1(bool, zfbool)
_ZFP_ZFValueCompareCast_DEFINE_1(char, zfchar)
_ZFP_ZFValueCompareCast_DEFINE_1(int, zfint)
_ZFP_ZFValueCompareCast_DEFINE_1(unsignedInt, zfuint)
_ZFP_ZFValueCompareCast_DEFINE_1(index, zfindex)
_ZFP_ZFValueCompareCast_DEFINE_1(int8, zfint8)
_ZFP_ZFValueCompareCast_DEFINE_1(unsignedInt8, zfuint8)
_ZFP_ZFValueCompareCast_DEFINE_1(int16, zfint16)
_ZFP_ZFValueCompareCast_DEFINE_1(unsignedInt16, zfuint16)
_ZFP_ZFValueCompareCast_DEFINE_1(int32, zfint32)
_ZFP_ZFValueCompareCast_DEFINE_1(unsignedInt32, zfuint32)
_ZFP_ZFValueCompareCast_DEFINE_1(int64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_1(unsignedInt64, zfuint64)
_ZFP_ZFValueCompareCast_DEFINE_1(float, zffloat)
_ZFP_ZFValueCompareCast_DEFINE_1(double, zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_1(longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_1(time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_1(flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_1(identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_1(serializableData, ZFSerializableData)

// ============================================================
_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValueCompareCast_DEFINE_2(bool, char, zfchar)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, int, zfint)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, unsignedInt, zfuint)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, index, zfindex)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, int8, zfint8)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, unsignedInt8, zfuint8)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, int16, zfint16)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, unsignedInt16, zfuint16)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, int32, zfint32)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, unsignedInt32, zfuint32)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, int64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, unsignedInt64, zfuint64)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, float, zffloat)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, double, zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(char, int, zfint)
_ZFP_ZFValueCompareCast_DEFINE_2(char, unsignedInt, zfuint)
_ZFP_ZFValueCompareCast_DEFINE_2(char, index, zfindex)
_ZFP_ZFValueCompareCast_DEFINE_2(char, int8, zfint8)
_ZFP_ZFValueCompareCast_DEFINE_2(char, unsignedInt8, zfuint8)
_ZFP_ZFValueCompareCast_DEFINE_2(char, int16, zfint16)
_ZFP_ZFValueCompareCast_DEFINE_2(char, unsignedInt16, zfuint16)
_ZFP_ZFValueCompareCast_DEFINE_2(char, int32, zfint32)
_ZFP_ZFValueCompareCast_DEFINE_2(char, unsignedInt32, zfuint32)
_ZFP_ZFValueCompareCast_DEFINE_2(char, int64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(char, unsignedInt64, zfuint64)
_ZFP_ZFValueCompareCast_DEFINE_2(char, float, zffloat)
_ZFP_ZFValueCompareCast_DEFINE_2(char, double, zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(char, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(char, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(char, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(char, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(char, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(int, unsignedInt, zfuint)
_ZFP_ZFValueCompareCast_DEFINE_2(int, index, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int, int8, zfint)
_ZFP_ZFValueCompareCast_DEFINE_2(int, unsignedInt8, zfint)
_ZFP_ZFValueCompareCast_DEFINE_2(int, int16, zfint)
_ZFP_ZFValueCompareCast_DEFINE_2(int, unsignedInt16, zfint)
_ZFP_ZFValueCompareCast_DEFINE_2(int, int32, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int, unsignedInt32, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int, int64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int, unsignedInt64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int, float, zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int, double, zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(int, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(int, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(int, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, index, zfindex)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, int8, zfuint)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, unsignedInt8, zfuint)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, int16, zfuint)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, unsignedInt16, zfuint)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, int32, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, unsignedInt32, zfuint64)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, int64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, unsignedInt64, zfuint64)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, float, zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, double, zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(index, int8, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(index, unsignedInt8, zfindex)
_ZFP_ZFValueCompareCast_DEFINE_2(index, int16, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(index, unsignedInt16, zfindex)
_ZFP_ZFValueCompareCast_DEFINE_2(index, int32, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(index, unsignedInt32, zfuint64)
_ZFP_ZFValueCompareCast_DEFINE_2(index, int64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(index, unsignedInt64, zfuint64)
_ZFP_ZFValueCompareCast_DEFINE_2(index, float, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(index, double, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(index, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(index, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(index, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(index, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(index, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(int8, unsignedInt8, zfint16)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, int16, zfint16)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, unsignedInt16, zfint32)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, int32, zfint32)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, unsignedInt32, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, int64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, unsignedInt64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, float, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, double, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(int8, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, int16, zfint32)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, unsignedInt16, zfuint16)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, int32, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, unsignedInt32, zfuint32)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, int64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, unsignedInt64, zfuint64)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, float, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, double, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt8, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(int16, unsignedInt16, zfint32)
_ZFP_ZFValueCompareCast_DEFINE_2(int16, int32, zfint32)
_ZFP_ZFValueCompareCast_DEFINE_2(int16, unsignedInt32, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int16, int64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int16, unsignedInt64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int16, float, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int16, double, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int16, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int16, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(int16, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(int16, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(int16, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt16, int32, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt16, unsignedInt32, zfuint32)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt16, int64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt16, unsignedInt64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt16, float, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt16, double, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt16, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt16, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt16, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt16, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt16, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(int32, unsignedInt32, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int32, int64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int32, unsignedInt64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int32, float, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int32, double, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int32, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int32, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(int32, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(int32, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(int32, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt32, int64, zfint64)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt32, unsignedInt64, zfuint64)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt32, float, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt32, double, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt32, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt32, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt32, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt32, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt32, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(int64, unsignedInt64, zfuint64)
_ZFP_ZFValueCompareCast_DEFINE_2(int64, float, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int64, double, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int64, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int64, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(int64, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(int64, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(int64, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt64, float, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt64, double, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt64, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt64, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt64, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt64, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt64, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(float, double, zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(float, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(float, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(float, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(float, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(float, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(double, longDouble, zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(double, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(double, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(double, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(double, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(longDouble, time, zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(longDouble, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(longDouble, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(longDouble, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(time, flags, zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(time, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(time, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(flags, identity, zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(flags, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(flags, serializableData, void)
// ============================================================

#define _ZFP_ZFValue_compareType_switch_case(SwitchType, TypeName) \
    case ZFValueType::e_##SwitchType: \
        return _ZFP_ZFValueCompare( \
            (_ZFP_ZFValueCompareCast_##SwitchType##_##TypeName)_ZFP_ZFValue_expandValue(SwitchType, this->SwitchType##ValueAccess()), \
            (_ZFP_ZFValueCompareCast_##SwitchType##_##TypeName)_ZFP_ZFValue_expandValue(TypeName, v));

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
#define _ZFP_ZFValue_compareType(TypeName, Type) \
    ZFCompareResult ZFValue::TypeName##ValueCompare(ZF_IN Type const &v) \
    { \
        switch(this->valueType()) \
        { \
            _ZFP_ZFValue_compareType_switch_case(bool, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(char, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(int, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(unsignedInt, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(index, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(int8, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(unsignedInt8, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(int16, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(unsignedInt16, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(int32, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(unsignedInt32, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(int64, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(unsignedInt64, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(float, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(double, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(longDouble, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(time, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(flags, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(identity, TypeName) \
            case ZFValueType::e_serializableData: \
                return ZFCompareUncomparable; \
            default: \
                zfCoreCriticalShouldNotGoHere(); \
                return ZFCompareUncomparable; \
        } \
    }

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValue_compareType(bool, zfbool)
_ZFP_ZFValue_compareType(char, zfchar)
_ZFP_ZFValue_compareType(int, zfint)
_ZFP_ZFValue_compareType(unsignedInt, zfuint)
_ZFP_ZFValue_compareType(index, zfindex)
_ZFP_ZFValue_compareType(int8, zfint8)
_ZFP_ZFValue_compareType(unsignedInt8, zfuint8)
_ZFP_ZFValue_compareType(int16, zfint16)
_ZFP_ZFValue_compareType(unsignedInt16, zfuint16)
_ZFP_ZFValue_compareType(int32, zfint32)
_ZFP_ZFValue_compareType(unsignedInt32, zfuint32)
_ZFP_ZFValue_compareType(int64, zfint64)
_ZFP_ZFValue_compareType(unsignedInt64, zfuint64)
_ZFP_ZFValue_compareType(float, zffloat)
_ZFP_ZFValue_compareType(double, zfdouble)
_ZFP_ZFValue_compareType(longDouble, zflongdouble)
_ZFP_ZFValue_compareType(time, zftimet)
_ZFP_ZFValue_compareType(flags, zfflags)
_ZFP_ZFValue_compareType(identity, zfidentity)

ZFCompareResult ZFValue::serializableDataValueCompare(ZF_IN const ZFSerializableData &v)
{
    if(this->valueType() != ZFValueType::e_serializableData)
    {
        return ZFCompareUncomparable;
    }
    return this->serializableDataValueAccess().objectCompare(v);
}

// ============================================================
#define _ZFP_ZFValue_create_noCache_DEFINE(TypeName, Type) \
    zfautoObject ZFValue::TypeName##ValueCreate(ZF_IN Type const &v) \
    { \
        zfblockedAlloc(ZFValue, value); \
        value->TypeName##ValueSet(v); \
        return zfautoObjectCreate(value); \
    }
#define _ZFP_ZFValue_create_hasCache_DEFINE(TypeName, Type, negativeCount, positiveCount) \
    zfautoObject ZFValue::TypeName##ValueCreate(ZF_IN Type const &v) \
    { \
        ZFCoreMutexLocker(); \
        static ZFValue *s_value[negativeCount + 1 + positiveCount] = {0}; \
        zfindex i = (zfindex)(v + negativeCount); \
        if(i < (negativeCount + 1 + positiveCount)) \
        { \
            if(s_value[i] == zfnull) \
            { \
                s_value[i] = zfAllocWithoutLeakTest(ZFValue); \
                s_value[i]->TypeName##ValueSet(v); \
                _ZFP_ZFValueAddToDestroyArray(s_value[i]); \
            } \
            return zfautoObjectCreate(s_value[i]); \
        } \
        zfblockedAlloc(ZFValue, value); \
        value->TypeName##ValueSet(v); \
        return zfautoObjectCreate(value); \
    }
zfautoObject ZFValue::boolValueCreate(ZF_IN zfbool const &v)
{
    ZFCoreMutexLocker();
    static ZFValue *s_value[2] = {0};
    zfindex i = (v ? 1 : 0);

    if(s_value[i] == zfnull)
    {
        s_value[i] = zfAllocWithoutLeakTest(ZFValue);
        s_value[i]->boolValueSet(v);
        _ZFP_ZFValueAddToDestroyArray(s_value[i]);
    }
    return zfautoObjectCreate(s_value[i]);
}
_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValue_create_noCache_DEFINE(char, zfchar)
_ZFP_ZFValue_create_hasCache_DEFINE(int, zfint, 3, 10)
_ZFP_ZFValue_create_hasCache_DEFINE(unsignedInt, zfuint, 0, 10)
_ZFP_ZFValue_create_hasCache_DEFINE(index, zfindex, 0, 32)
_ZFP_ZFValue_create_noCache_DEFINE(int8, zfint8)
_ZFP_ZFValue_create_noCache_DEFINE(unsignedInt8, zfuint8)
_ZFP_ZFValue_create_noCache_DEFINE(int16, zfint16)
_ZFP_ZFValue_create_noCache_DEFINE(unsignedInt16, zfuint16)
_ZFP_ZFValue_create_noCache_DEFINE(int32, zfint32)
_ZFP_ZFValue_create_noCache_DEFINE(unsignedInt32, zfuint32)
_ZFP_ZFValue_create_noCache_DEFINE(int64, zfint64)
_ZFP_ZFValue_create_noCache_DEFINE(unsignedInt64, zfuint64)
_ZFP_ZFValue_create_noCache_DEFINE(float, zffloat)
_ZFP_ZFValue_create_noCache_DEFINE(double, zfdouble)
_ZFP_ZFValue_create_noCache_DEFINE(longDouble, zflongdouble)
_ZFP_ZFValue_create_noCache_DEFINE(time, zftimet)
_ZFP_ZFValue_create_noCache_DEFINE(flags, zfflags)
_ZFP_ZFValue_create_noCache_DEFINE(identity, zfidentity)
_ZFP_ZFValue_create_noCache_DEFINE(serializableData, ZFSerializableData)

// ============================================================
zfbool ZFValue::valueConvertableTo(ZF_IN ZFValueTypeEnum toType)
{
    switch(toType)
    {
        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        case ZFValueType::e_bool:
        case ZFValueType::e_char:
        case ZFValueType::e_int:
        case ZFValueType::e_unsignedInt:
        case ZFValueType::e_index:
        case ZFValueType::e_int8:
        case ZFValueType::e_unsignedInt8:
        case ZFValueType::e_int16:
        case ZFValueType::e_unsignedInt16:
        case ZFValueType::e_int32:
        case ZFValueType::e_unsignedInt32:
        case ZFValueType::e_int64:
        case ZFValueType::e_unsignedInt64:
        case ZFValueType::e_float:
        case ZFValueType::e_double:
        case ZFValueType::e_longDouble:
        case ZFValueType::e_time:
        case ZFValueType::e_flags:
        case ZFValueType::e_identity:
            return (this->valueType() != ZFValueType::e_serializableData);
        case ZFValueType::e_serializableData:
            return (this->valueType() == ZFValueType::e_serializableData);
        default:
            zfCoreCriticalShouldNotGoHere();
            return zffalse;
    }
}

// ============================================================
#define _ZFP_ZFValueLogUnConvertable(fromType, toType) \
    zfCoreCriticalMessage(zfTextA("could not convert value type from %s to %s"), \
        zfsCoreZ2A(ZFValueType::EnumNameForValue(fromType)), \
        zfsCoreZ2A(ZFValueType::EnumNameForValue(toType)))
#define _ZFP_ZFValue_getValue_swith_case(TypeName, Type) \
    case ZFValueType::e_##TypeName: \
        return (Type)d->v.v_##TypeName;

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
#define _ZFP_ZFValue_getValue_DEFINE(TypeName, Type) \
    Type ZFValue::TypeName##Value(void) \
    { \
        switch(this->valueType()) \
        { \
            _ZFP_ZFValue_getValue_swith_case(int, Type) \
            _ZFP_ZFValue_getValue_swith_case(unsignedInt, Type) \
            _ZFP_ZFValue_getValue_swith_case(index, Type) \
            _ZFP_ZFValue_getValue_swith_case(int8, Type) \
            _ZFP_ZFValue_getValue_swith_case(unsignedInt8, Type) \
            _ZFP_ZFValue_getValue_swith_case(int16, Type) \
            _ZFP_ZFValue_getValue_swith_case(unsignedInt16, Type) \
            _ZFP_ZFValue_getValue_swith_case(int32, Type) \
            _ZFP_ZFValue_getValue_swith_case(unsignedInt32, Type) \
            _ZFP_ZFValue_getValue_swith_case(int64, Type) \
            _ZFP_ZFValue_getValue_swith_case(unsignedInt64, Type) \
            _ZFP_ZFValue_getValue_swith_case(float, Type) \
            _ZFP_ZFValue_getValue_swith_case(double, Type) \
            _ZFP_ZFValue_getValue_swith_case(longDouble, Type) \
            _ZFP_ZFValue_getValue_swith_case(time, Type) \
            _ZFP_ZFValue_getValue_swith_case(flags, Type) \
            _ZFP_ZFValue_getValue_swith_case(identity, Type) \
            case ZFValueType::e_serializableData: \
                _ZFP_ZFValueLogUnConvertable(this->valueType(), ZFValueType::ZFM_CAT(e_, TypeName)); \
                return (Type)_ZFP_ZFValue_initValue(TypeName); \
            default: \
                zfCoreCriticalShouldNotGoHere(); \
                return (Type)_ZFP_ZFValue_initValue(TypeName); \
        } \
    }

zfbool ZFValue::boolValue(void)
{
    switch(this->valueType())
    {
        #define _ZFP_ZFValue_boolValue_switch_case(TypeName, Type) \
            case ZFValueType::e_##TypeName: \
                return (d->v.v_##TypeName != _ZFP_ZFValue_initValue(TypeName));

        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_boolValue_switch_case(bool, zfbool)
        _ZFP_ZFValue_boolValue_switch_case(char, zfchar)
        _ZFP_ZFValue_boolValue_switch_case(int, zfint)
        _ZFP_ZFValue_boolValue_switch_case(unsignedInt, zfuint)
        _ZFP_ZFValue_boolValue_switch_case(index, zfindex)
        _ZFP_ZFValue_boolValue_switch_case(int8, zfint8)
        _ZFP_ZFValue_boolValue_switch_case(unsignedInt8, zfuint8)
        _ZFP_ZFValue_boolValue_switch_case(int16, zfint16)
        _ZFP_ZFValue_boolValue_switch_case(unsignedInt16, zfuint16)
        _ZFP_ZFValue_boolValue_switch_case(int32, zfint32)
        _ZFP_ZFValue_boolValue_switch_case(unsignedInt32, zfuint32)
        _ZFP_ZFValue_boolValue_switch_case(int64, zfint64)
        _ZFP_ZFValue_boolValue_switch_case(unsignedInt64, zfuint64)
        _ZFP_ZFValue_boolValue_switch_case(float, zffloat)
        _ZFP_ZFValue_boolValue_switch_case(double, zfdouble)
        _ZFP_ZFValue_boolValue_switch_case(longDouble, zflongdouble)
        _ZFP_ZFValue_boolValue_switch_case(time, zftimet)
        _ZFP_ZFValue_boolValue_switch_case(flags, zfflags)
        _ZFP_ZFValue_boolValue_switch_case(identity, zfidentity)

        case ZFValueType::e_serializableData:
            _ZFP_ZFValueLogUnConvertable(this->valueType(), ZFValueType::ZFM_CAT(e_, bool));
            return zffalse;
        default:
            zfCoreCriticalShouldNotGoHere();
            return zffalse;
    }
}

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValue_getValue_DEFINE(char, zfchar)
_ZFP_ZFValue_getValue_DEFINE(int, zfint)
_ZFP_ZFValue_getValue_DEFINE(unsignedInt, zfuint)
_ZFP_ZFValue_getValue_DEFINE(index, zfindex)
_ZFP_ZFValue_getValue_DEFINE(int8, zfint8)
_ZFP_ZFValue_getValue_DEFINE(unsignedInt8, zfuint8)
_ZFP_ZFValue_getValue_DEFINE(int16, zfint16)
_ZFP_ZFValue_getValue_DEFINE(unsignedInt16, zfuint16)
_ZFP_ZFValue_getValue_DEFINE(int32, zfint32)
_ZFP_ZFValue_getValue_DEFINE(unsignedInt32, zfuint32)
_ZFP_ZFValue_getValue_DEFINE(int64, zfint64)
_ZFP_ZFValue_getValue_DEFINE(unsignedInt64, zfuint64)
_ZFP_ZFValue_getValue_DEFINE(float, zffloat)
_ZFP_ZFValue_getValue_DEFINE(double, zfdouble)
_ZFP_ZFValue_getValue_DEFINE(longDouble, zflongdouble)
_ZFP_ZFValue_getValue_DEFINE(time, zftimet)
_ZFP_ZFValue_getValue_DEFINE(flags, zfflags)
_ZFP_ZFValue_getValue_DEFINE(identity, zfidentity)

ZFSerializableData ZFValue::serializableDataValue(void)
{
    if(this->valueType() != ZFValueType::e_serializableData)
    {
        _ZFP_ZFValueLogUnConvertable(this->valueType(), ZFValueType::e_serializableData);
        return *(d->v.v_serializableData);
    }
    return *(d->v.v_serializableData);
}

// ============================================================
#define _ZFP_ZFValue_accessValue_DEFINE(TypeName, Type) \
    Type const &ZFValue::TypeName##ValueAccess(void) \
    { \
        return d->v.v_##TypeName; \
    }

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValue_accessValue_DEFINE(bool, zfbool)
_ZFP_ZFValue_accessValue_DEFINE(char, zfchar)
_ZFP_ZFValue_accessValue_DEFINE(int, zfint)
_ZFP_ZFValue_accessValue_DEFINE(unsignedInt, zfuint)
_ZFP_ZFValue_accessValue_DEFINE(index, zfindex)
_ZFP_ZFValue_accessValue_DEFINE(int8, zfint8)
_ZFP_ZFValue_accessValue_DEFINE(unsignedInt8, zfuint8)
_ZFP_ZFValue_accessValue_DEFINE(int16, zfint16)
_ZFP_ZFValue_accessValue_DEFINE(unsignedInt16, zfuint16)
_ZFP_ZFValue_accessValue_DEFINE(int32, zfint32)
_ZFP_ZFValue_accessValue_DEFINE(unsignedInt32, zfuint32)
_ZFP_ZFValue_accessValue_DEFINE(int64, zfint64)
_ZFP_ZFValue_accessValue_DEFINE(unsignedInt64, zfuint64)
_ZFP_ZFValue_accessValue_DEFINE(float, zffloat)
_ZFP_ZFValue_accessValue_DEFINE(double, zfdouble)
_ZFP_ZFValue_accessValue_DEFINE(longDouble, zflongdouble)
_ZFP_ZFValue_accessValue_DEFINE(time, zftimet)
_ZFP_ZFValue_accessValue_DEFINE(flags, zfflags)
_ZFP_ZFValue_accessValue_DEFINE(identity, zfidentity)

ZFSerializableData const &ZFValue::serializableDataValueAccess(void)
{
    return *(d->v.v_serializableData);
}

// ============================================================
ZFValueTypeEnum ZFValue::valueType(void)
{
    return d->type;
}

const zfchar *ZFValue::valueTypeName(void)
{
    return ZFValueType::EnumNameForValue(this->valueType());
}

void ZFValue::valueStringT(ZF_IN_OUT zfstring &ret)
{
    d->valueStringT(ret);
}

void ZFValue::valueSet(ZF_IN ZFValue *another)
{
    if(another == zfnull)
    {
        return ;
    }
    switch(another->valueType())
    {
        #define _ZFP_ZFValue_valueSet_switch_case(TypeName, Type) \
            case ZFValueType::ZFM_CAT(e_, TypeName): \
                this->TypeName##ValueSet(another->TypeName##ValueAccess()); \
                break;

        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_valueSet_switch_case(bool, zfbool)
        _ZFP_ZFValue_valueSet_switch_case(char, zfchar)
        _ZFP_ZFValue_valueSet_switch_case(int, zfint)
        _ZFP_ZFValue_valueSet_switch_case(unsignedInt, zfuint)
        _ZFP_ZFValue_valueSet_switch_case(index, zfindex)
        _ZFP_ZFValue_valueSet_switch_case(int8, zfint8)
        _ZFP_ZFValue_valueSet_switch_case(unsignedInt8, zfuint8)
        _ZFP_ZFValue_valueSet_switch_case(int16, zfint16)
        _ZFP_ZFValue_valueSet_switch_case(unsignedInt16, zfuint16)
        _ZFP_ZFValue_valueSet_switch_case(int32, zfint32)
        _ZFP_ZFValue_valueSet_switch_case(unsignedInt32, zfuint32)
        _ZFP_ZFValue_valueSet_switch_case(int64, zfint64)
        _ZFP_ZFValue_valueSet_switch_case(unsignedInt64, zfuint64)
        _ZFP_ZFValue_valueSet_switch_case(float, zffloat)
        _ZFP_ZFValue_valueSet_switch_case(double, zfdouble)
        _ZFP_ZFValue_valueSet_switch_case(longDouble, zflongdouble)
        _ZFP_ZFValue_valueSet_switch_case(time, zftimet)
        _ZFP_ZFValue_valueSet_switch_case(flags, zfflags)
        _ZFP_ZFValue_valueSet_switch_case(identity, zfidentity)

        case ZFValueType::e_serializableData:
            this->serializableDataValueSet(another->serializableDataValueAccess());
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

// ============================================================
#define _ZFP_ZFValue_setValue_DEFINE(TypeName, Type) \
    void ZFValue::TypeName##ValueSet(ZF_IN Type const &v) \
    { \
        d->clearValue(); \
        d->type = ZFValueType::ZFM_CAT(e_, TypeName); \
        d->v.v_##TypeName = v; \
    }

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValue_setValue_DEFINE(bool, zfbool)
_ZFP_ZFValue_setValue_DEFINE(char, zfchar)
_ZFP_ZFValue_setValue_DEFINE(int, zfint)
_ZFP_ZFValue_setValue_DEFINE(unsignedInt, zfuint)
_ZFP_ZFValue_setValue_DEFINE(index, zfindex)
_ZFP_ZFValue_setValue_DEFINE(int8, zfint8)
_ZFP_ZFValue_setValue_DEFINE(unsignedInt8, zfuint8)
_ZFP_ZFValue_setValue_DEFINE(int16, zfint16)
_ZFP_ZFValue_setValue_DEFINE(unsignedInt16, zfuint16)
_ZFP_ZFValue_setValue_DEFINE(int32, zfint32)
_ZFP_ZFValue_setValue_DEFINE(unsignedInt32, zfuint32)
_ZFP_ZFValue_setValue_DEFINE(int64, zfint64)
_ZFP_ZFValue_setValue_DEFINE(unsignedInt64, zfuint64)
_ZFP_ZFValue_setValue_DEFINE(float, zffloat)
_ZFP_ZFValue_setValue_DEFINE(double, zfdouble)
_ZFP_ZFValue_setValue_DEFINE(longDouble, zflongdouble)
_ZFP_ZFValue_setValue_DEFINE(time, zftimet)
_ZFP_ZFValue_setValue_DEFINE(flags, zfflags)
_ZFP_ZFValue_setValue_DEFINE(identity, zfidentity)

void ZFValue::serializableDataValueSet(ZF_IN const ZFSerializableData &v)
{
    d->clearValue();
    d->type = ZFValueType::e_serializableData;
    d->v.v_serializableData = zfnew(ZFSerializableData);
    d->v.v_serializableData->copyFrom(v);
}

// ============================================================
#define _ZFP_ZFValueEditable_create_DEFINE(TypeName, Type) \
    zfautoObject ZFValueEditable::TypeName##ValueCreate(ZF_IN Type const &v) \
    { \
        zfblockedAlloc(ZFValueEditable, ret); \
        ret->TypeName##ValueSet(v); \
        return zfautoObjectCreate(ret); \
    }

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValueEditable_create_DEFINE(bool, zfbool)
_ZFP_ZFValueEditable_create_DEFINE(char, zfchar)
_ZFP_ZFValueEditable_create_DEFINE(int, zfint)
_ZFP_ZFValueEditable_create_DEFINE(unsignedInt, zfuint)
_ZFP_ZFValueEditable_create_DEFINE(index, zfindex)
_ZFP_ZFValueEditable_create_DEFINE(int8, zfint8)
_ZFP_ZFValueEditable_create_DEFINE(unsignedInt8, zfuint8)
_ZFP_ZFValueEditable_create_DEFINE(int16, zfint16)
_ZFP_ZFValueEditable_create_DEFINE(unsignedInt16, zfuint16)
_ZFP_ZFValueEditable_create_DEFINE(int32, zfint32)
_ZFP_ZFValueEditable_create_DEFINE(unsignedInt32, zfuint32)
_ZFP_ZFValueEditable_create_DEFINE(int64, zfint64)
_ZFP_ZFValueEditable_create_DEFINE(unsignedInt64, zfuint64)
_ZFP_ZFValueEditable_create_DEFINE(float, zffloat)
_ZFP_ZFValueEditable_create_DEFINE(double, zfdouble)
_ZFP_ZFValueEditable_create_DEFINE(longDouble, zflongdouble)
_ZFP_ZFValueEditable_create_DEFINE(time, zftimet)
_ZFP_ZFValueEditable_create_DEFINE(flags, zfflags)
_ZFP_ZFValueEditable_create_DEFINE(identity, zfidentity)
_ZFP_ZFValueEditable_create_DEFINE(serializableData, ZFSerializableData)

ZF_NAMESPACE_GLOBAL_END

