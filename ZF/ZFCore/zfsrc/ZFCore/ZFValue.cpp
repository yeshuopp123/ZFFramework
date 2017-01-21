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
#define _ZFP_ZFValue_initValue_bool() ((zft_zfbool)zffalse)
#define _ZFP_ZFValue_initValue_char() ((zft_zfchar)'\0')
#define _ZFP_ZFValue_initValue_int() ((zft_zfint)0)
#define _ZFP_ZFValue_initValue_unsignedInt() ((zft_zfuint)0)
#define _ZFP_ZFValue_initValue_index() ((zft_zfindex)0)
#define _ZFP_ZFValue_initValue_float() ((zft_zffloat)0)
#define _ZFP_ZFValue_initValue_double() ((zft_zfdouble)0)
#define _ZFP_ZFValue_initValue_longDouble() ((zft_zflongdouble)0)
#define _ZFP_ZFValue_initValue_time() ((zft_zftimet)0)
#define _ZFP_ZFValue_initValue_flags() ((zft_zfflags)0)
#define _ZFP_ZFValue_initValue_identity() ((zft_zfidentity)0)
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
    #define _ZFP_ZFValue_internal_value_DEFINE(TypeName, Type, originalType) \
        originalType v_##TypeName;
    union _ZFP_ZFValueUnion
    {
    public:
        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_internal_value_DEFINE(bool, zfbool, zft_zfbool)
        _ZFP_ZFValue_internal_value_DEFINE(char, zfchar, zft_zfchar)
        _ZFP_ZFValue_internal_value_DEFINE(int, zfint, zft_zfint)
        _ZFP_ZFValue_internal_value_DEFINE(unsignedInt, zfuint, zft_zfuint)
        _ZFP_ZFValue_internal_value_DEFINE(index, zfindex, zft_zfindex)
        _ZFP_ZFValue_internal_value_DEFINE(float, zffloat, zft_zffloat)
        _ZFP_ZFValue_internal_value_DEFINE(double, zfdouble, zft_zfdouble)
        _ZFP_ZFValue_internal_value_DEFINE(longDouble, zflongdouble, zft_zflongdouble)
        _ZFP_ZFValue_internal_value_DEFINE(time, zftimet, zft_zftimet)
        _ZFP_ZFValue_internal_value_DEFINE(flags, zfflags, zft_zfflags)
        _ZFP_ZFValue_internal_value_DEFINE(identity, zfidentity, zft_zfidentity)
        _ZFP_ZFValue_internal_value_DEFINE(serializableData, ZFSerializableData *, ZFSerializableData *)
    };
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
        #define _ZFP_ZFValue_serializableOnSerializeFromData(TypeName, Type, originalType) \
            case ZFValueType::ZFM_CAT(e_, TypeName): \
            { \
                Type v = (Type)_ZFP_ZFValue_initValue(TypeName); \
                ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos, \
                    check, ZFSerializableKeyword_ZFValue_value, Type, v); \
                this->TypeName##ValueSet(v); \
                break; \
            }

        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_serializableOnSerializeFromData(bool, zfbool, zft_zfbool)
        _ZFP_ZFValue_serializableOnSerializeFromData(char, zfchar, zft_zfchar)
        _ZFP_ZFValue_serializableOnSerializeFromData(int, zfint, zft_zfint)
        _ZFP_ZFValue_serializableOnSerializeFromData(unsignedInt, zfuint, zft_zfuint)
        _ZFP_ZFValue_serializableOnSerializeFromData(index, zfindex, zft_zfindex)
        _ZFP_ZFValue_serializableOnSerializeFromData(float, zffloat, zft_zffloat)
        _ZFP_ZFValue_serializableOnSerializeFromData(double, zfdouble, zft_zfdouble)
        _ZFP_ZFValue_serializableOnSerializeFromData(longDouble, zflongdouble, zft_zflongdouble)
        _ZFP_ZFValue_serializableOnSerializeFromData(time, zftimet, zft_zftimet)
        _ZFP_ZFValue_serializableOnSerializeFromData(flags, zfflags, zft_zfflags)
        _ZFP_ZFValue_serializableOnSerializeFromData(identity, zfidentity, zft_zfidentity)
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
        #define _ZFP_ZFValue_serializableOnSerializeToData(TypeName, Type, originalType) \
            case ZFValueType::ZFM_CAT(e_, TypeName): \
            { \
                ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHint, ref, \
                    ZFSerializableKeyword_ZFValue_value, Type, this->TypeName##ValueAccess(), ref->TypeName##ValueAccess(), _ZFP_ZFValue_initValue(TypeName)); \
                break; \
            }

        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_serializableOnSerializeToData(bool, zfbool, zft_zfbool)
        _ZFP_ZFValue_serializableOnSerializeToData(char, zfchar, zft_zfchar)
        _ZFP_ZFValue_serializableOnSerializeToData(int, zfint, zft_zfint)
        _ZFP_ZFValue_serializableOnSerializeToData(unsignedInt, zfuint, zft_zfuint)
        _ZFP_ZFValue_serializableOnSerializeToData(index, zfindex, zft_zfindex)
        _ZFP_ZFValue_serializableOnSerializeToData(float, zffloat, zft_zffloat)
        _ZFP_ZFValue_serializableOnSerializeToData(double, zfdouble, zft_zfdouble)
        _ZFP_ZFValue_serializableOnSerializeToData(longDouble, zflongdouble, zft_zflongdouble)
        _ZFP_ZFValue_serializableOnSerializeToData(time, zftimet, zft_zftimet)
        _ZFP_ZFValue_serializableOnSerializeToData(flags, zfflags, zft_zfflags)
        _ZFP_ZFValue_serializableOnSerializeToData(identity, zfidentity, zft_zfidentity)
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
static inline ZFCompareResult _ZFP_ZFValueCompare(ZFSerializableData const &v1, ZFSerializableData const &v2)
{
    return v1.objectCompare(v2);
}

zfidentity ZFValue::objectHash(void)
{
    zfidentity ret = zfidentityHash(this->valueType());
    switch(this->valueType())
    {
        #define _ZFP_ZFValue_hash_switch_case(TypeName, Type, originalType) \
            case ZFValueType::e_##TypeName: \
                ret = zfidentityHash(ret, ((zft_zfuint32)_ZFP_ZFValue_expandValue(TypeName, this->TypeName##ValueAccess()) & 0xFFFFFFFF)); \
                break;

        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_hash_switch_case(bool, zfbool, zft_zfbool)
        _ZFP_ZFValue_hash_switch_case(char, zfchar, zft_zfchar)
        _ZFP_ZFValue_hash_switch_case(int, zfint, zft_zfint)
        _ZFP_ZFValue_hash_switch_case(unsignedInt, zfuint, zft_zfuint)
        _ZFP_ZFValue_hash_switch_case(index, zfindex, zft_zfindex)
        _ZFP_ZFValue_hash_switch_case(float, zffloat, zft_zffloat)
        _ZFP_ZFValue_hash_switch_case(double, zfdouble, zft_zfdouble)
        _ZFP_ZFValue_hash_switch_case(longDouble, zflongdouble, zft_zflongdouble)
        _ZFP_ZFValue_hash_switch_case(time, zftimet, zft_zftimet)
        _ZFP_ZFValue_hash_switch_case(flags, zfflags, zft_zfflags)
        _ZFP_ZFValue_hash_switch_case(identity, zfidentity, zft_zfidentity)
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfidentityZero;
    }
    return (zfidentity)~ret;
}

ZFCompareResult ZFValue::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    switch(this->valueType())
    {
        #define _ZFP_ZFValue_compare_switch_case(TypeName, Type, originalType) \
            case ZFValueType::ZFM_CAT(e_, TypeName): \
                return _ZFP_ZFValueCompare(this->TypeName##ValueAccess(), another->TypeName##ValueAccess());

        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_compare_switch_case(bool, zfbool, zft_zfbool)
        _ZFP_ZFValue_compare_switch_case(char, zfchar, zft_zfchar)
        _ZFP_ZFValue_compare_switch_case(int, zfint, zft_zfint)
        _ZFP_ZFValue_compare_switch_case(unsignedInt, zfuint, zft_zfuint)
        _ZFP_ZFValue_compare_switch_case(index, zfindex, zft_zfindex)
        _ZFP_ZFValue_compare_switch_case(float, zffloat, zft_zffloat)
        _ZFP_ZFValue_compare_switch_case(double, zfdouble, zft_zfdouble)
        _ZFP_ZFValue_compare_switch_case(longDouble, zflongdouble, zft_zflongdouble)
        _ZFP_ZFValue_compare_switch_case(time, zftimet, zft_zftimet)
        _ZFP_ZFValue_compare_switch_case(flags, zfflags, zft_zfflags)
        _ZFP_ZFValue_compare_switch_case(identity, zfidentity, zft_zfidentity)
        _ZFP_ZFValue_compare_switch_case(serializableData, ZFSerializableData, void)
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
_ZFP_ZFValueCompareCast_DEFINE_1(bool, zft_zfbool)
_ZFP_ZFValueCompareCast_DEFINE_1(char, zft_zfchar)
_ZFP_ZFValueCompareCast_DEFINE_1(int, zft_zfint)
_ZFP_ZFValueCompareCast_DEFINE_1(unsignedInt, zft_zfuint)
_ZFP_ZFValueCompareCast_DEFINE_1(index, zft_zfindex)
_ZFP_ZFValueCompareCast_DEFINE_1(float, zft_zffloat)
_ZFP_ZFValueCompareCast_DEFINE_1(double, zft_zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_1(longDouble, zft_zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_1(time, zft_zftimet)
_ZFP_ZFValueCompareCast_DEFINE_1(flags, zft_zfflags)
_ZFP_ZFValueCompareCast_DEFINE_1(identity, zft_zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_1(serializableData, ZFSerializableData)

// ============================================================
_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValueCompareCast_DEFINE_2(bool, char, zft_zfchar)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, int, zft_zfint)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, unsignedInt, zft_zfuint)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, index, zft_zfindex)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, float, zft_zffloat)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, double, zft_zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, longDouble, zft_zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, time, zft_zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, flags, zft_zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, identity, zft_zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(bool, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(char, int, zft_zfint)
_ZFP_ZFValueCompareCast_DEFINE_2(char, unsignedInt, zft_zfuint)
_ZFP_ZFValueCompareCast_DEFINE_2(char, index, zft_zfindex)
_ZFP_ZFValueCompareCast_DEFINE_2(char, float, zft_zffloat)
_ZFP_ZFValueCompareCast_DEFINE_2(char, double, zft_zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(char, longDouble, zft_zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(char, time, zft_zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(char, flags, zft_zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(char, identity, zft_zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(char, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(int, unsignedInt, zft_zfuint)
_ZFP_ZFValueCompareCast_DEFINE_2(int, index, zft_zfindex)
_ZFP_ZFValueCompareCast_DEFINE_2(int, float, zft_zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int, double, zft_zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int, longDouble, zft_zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(int, time, zft_zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(int, flags, zft_zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(int, identity, zft_zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(int, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, index, zft_zfindex)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, float, zft_zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, double, zft_zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, longDouble, zft_zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, time, zft_zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, flags, zft_zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, identity, zft_zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(unsignedInt, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(index, float, zft_zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(index, double, zft_zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(index, longDouble, zft_zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(index, time, zft_zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(index, flags, zft_zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(index, identity, zft_zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(index, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(float, double, zft_zfdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(float, longDouble, zft_zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(float, time, zft_zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(float, flags, zft_zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(float, identity, zft_zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(float, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(double, longDouble, zft_zflongdouble)
_ZFP_ZFValueCompareCast_DEFINE_2(double, time, zft_zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(double, flags, zft_zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(double, identity, zft_zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(double, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(longDouble, time, zft_zftimet)
_ZFP_ZFValueCompareCast_DEFINE_2(longDouble, flags, zft_zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(longDouble, identity, zft_zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(longDouble, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(time, flags, zft_zfflags)
_ZFP_ZFValueCompareCast_DEFINE_2(time, identity, zft_zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(time, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(flags, identity, zft_zfidentity)
_ZFP_ZFValueCompareCast_DEFINE_2(flags, serializableData, void)

_ZFP_ZFValueCompareCast_DEFINE_2(flags, serializableData, void)
// ============================================================

#define _ZFP_ZFValue_compareType_switch_case(SwitchType, TypeName) \
    case ZFValueType::e_##SwitchType: \
        return _ZFP_ZFValueCompare( \
            (_ZFP_ZFValueCompareCast_##SwitchType##_##TypeName)_ZFP_ZFValue_expandValue(SwitchType, this->SwitchType##ValueAccess()), \
            (_ZFP_ZFValueCompareCast_##SwitchType##_##TypeName)_ZFP_ZFValue_expandValue(TypeName, v));

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
#define _ZFP_ZFValue_compareType(TypeName, Type, originalType) \
    ZFCompareResult ZFValue::TypeName##ValueCompare(ZF_IN Type const &v) \
    { \
        switch(this->valueType()) \
        { \
            _ZFP_ZFValue_compareType_switch_case(bool, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(char, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(int, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(unsignedInt, TypeName) \
            _ZFP_ZFValue_compareType_switch_case(index, TypeName) \
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
_ZFP_ZFValue_compareType(bool, zfbool, zft_zfbool)
_ZFP_ZFValue_compareType(char, zfchar, zft_zfchar)
_ZFP_ZFValue_compareType(int, zfint, zft_zfint)
_ZFP_ZFValue_compareType(unsignedInt, zfuint, zft_zfuint)
_ZFP_ZFValue_compareType(index, zfindex, zft_zfindex)
_ZFP_ZFValue_compareType(float, zffloat, zft_zffloat)
_ZFP_ZFValue_compareType(double, zfdouble, zft_zfdouble)
_ZFP_ZFValue_compareType(longDouble, zflongdouble, zft_zflongdouble)
_ZFP_ZFValue_compareType(time, zftimet, zft_zftimet)
_ZFP_ZFValue_compareType(flags, zfflags, zft_zfflags)
_ZFP_ZFValue_compareType(identity, zfidentity, zft_zfidentity)

ZFCompareResult ZFValue::serializableDataValueCompare(ZF_IN const ZFSerializableData &v)
{
    if(this->valueType() != ZFValueType::e_serializableData)
    {
        return ZFCompareUncomparable;
    }
    return this->serializableDataValueAccess().objectCompare(v);
}

// ============================================================
#define _ZFP_ZFValue_create_noCache_DEFINE(TypeName, Type, originalType) \
    zfautoObject ZFValue::TypeName##ValueCreate(ZF_IN Type const &v) \
    { \
        zfblockedAlloc(ZFValue, value); \
        value->TypeName##ValueSet(v); \
        return zfautoObjectCreate(value); \
    }
#define _ZFP_ZFValue_create_hasCache_DEFINE(TypeName, Type, originalType, negativeCount, positiveCount) \
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
_ZFP_ZFValue_create_noCache_DEFINE(char, zfchar, zft_zfchar)
_ZFP_ZFValue_create_hasCache_DEFINE(int, zfint, zft_zfint, 3, 10)
_ZFP_ZFValue_create_hasCache_DEFINE(unsignedInt, zfuint, zft_zfuint, 0, 10)
_ZFP_ZFValue_create_hasCache_DEFINE(index, zfindex, zft_zfindex, 0, 32)
_ZFP_ZFValue_create_noCache_DEFINE(float, zffloat, zft_zffloat)
_ZFP_ZFValue_create_noCache_DEFINE(double, zfdouble, zft_zfdouble)
_ZFP_ZFValue_create_noCache_DEFINE(longDouble, zflongdouble, zft_zflongdouble)
_ZFP_ZFValue_create_noCache_DEFINE(time, zftimet, zft_zftimet)
_ZFP_ZFValue_create_noCache_DEFINE(flags, zfflags, zft_zfflags)
_ZFP_ZFValue_create_noCache_DEFINE(identity, zfidentity, zft_zfidentity)
_ZFP_ZFValue_create_noCache_DEFINE(serializableData, ZFSerializableData, void)

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
#define _ZFP_ZFValue_getValue_swith_case(TypeName, Type, originalType) \
    case ZFValueType::e_##TypeName: \
        return (Type)(originalType)d->v.v_##TypeName;

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
#define _ZFP_ZFValue_getValue_DEFINE(TypeName, Type, originalType) \
    Type ZFValue::TypeName##Value(void) \
    { \
        switch(this->valueType()) \
        { \
            _ZFP_ZFValue_getValue_swith_case(int, Type, originalType) \
            _ZFP_ZFValue_getValue_swith_case(unsignedInt, Type, originalType) \
            _ZFP_ZFValue_getValue_swith_case(index, Type, originalType) \
            _ZFP_ZFValue_getValue_swith_case(float, Type, originalType) \
            _ZFP_ZFValue_getValue_swith_case(double, Type, originalType) \
            _ZFP_ZFValue_getValue_swith_case(longDouble, Type, originalType) \
            _ZFP_ZFValue_getValue_swith_case(time, Type, originalType) \
            _ZFP_ZFValue_getValue_swith_case(flags, Type, originalType) \
            _ZFP_ZFValue_getValue_swith_case(identity, Type, originalType) \
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
        #define _ZFP_ZFValue_boolValue_switch_case(TypeName, Type, originalType) \
            case ZFValueType::e_##TypeName: \
                return (d->v.v_##TypeName != _ZFP_ZFValue_initValue(TypeName));

        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_boolValue_switch_case(bool, zfbool, zft_zfbool)
        _ZFP_ZFValue_boolValue_switch_case(char, zfchar, zft_zfchar)
        _ZFP_ZFValue_boolValue_switch_case(int, zfint, zft_zfint)
        _ZFP_ZFValue_boolValue_switch_case(unsignedInt, zfuint, zft_zfuint)
        _ZFP_ZFValue_boolValue_switch_case(index, zfindex, zft_zfindex)
        _ZFP_ZFValue_boolValue_switch_case(float, zffloat, zft_zffloat)
        _ZFP_ZFValue_boolValue_switch_case(double, zfdouble, zft_zfdouble)
        _ZFP_ZFValue_boolValue_switch_case(longDouble, zflongdouble, zft_zflongdouble)
        _ZFP_ZFValue_boolValue_switch_case(time, zftimet, zft_zftimet)
        _ZFP_ZFValue_boolValue_switch_case(flags, zfflags, zft_zfflags)
        _ZFP_ZFValue_boolValue_switch_case(identity, zfidentity, zft_zfidentity)

        case ZFValueType::e_serializableData:
            _ZFP_ZFValueLogUnConvertable(this->valueType(), ZFValueType::ZFM_CAT(e_, bool));
            return zffalse;
        default:
            zfCoreCriticalShouldNotGoHere();
            return zffalse;
    }
}

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValue_getValue_DEFINE(char, zfchar, zft_zfchar)
_ZFP_ZFValue_getValue_DEFINE(int, zfint, zft_zfint)
_ZFP_ZFValue_getValue_DEFINE(unsignedInt, zfuint, zft_zfuint)
_ZFP_ZFValue_getValue_DEFINE(index, zfindex, zft_zfindex)
_ZFP_ZFValue_getValue_DEFINE(float, zffloat, zft_zffloat)
_ZFP_ZFValue_getValue_DEFINE(double, zfdouble, zft_zfdouble)
_ZFP_ZFValue_getValue_DEFINE(longDouble, zflongdouble, zft_zflongdouble)
_ZFP_ZFValue_getValue_DEFINE(time, zftimet, zft_zftimet)
_ZFP_ZFValue_getValue_DEFINE(flags, zfflags, zft_zfflags)
_ZFP_ZFValue_getValue_DEFINE(identity, zfidentity, zft_zfidentity)

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
/*
 * tricks to convert primitive types to wrapper object reference,
 * valid only if sizeof(primitive type) equals to wrapper object
 */
#define _ZFP_ZFValue_accessValue_DEFINE(TypeName, Type, originalType) \
    Type const &ZFValue::TypeName##ValueAccess(void) \
    { \
        return (Type const &)d->v.v_##TypeName; \
    }

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValue_accessValue_DEFINE(bool, zfbool, zft_zfbool)
_ZFP_ZFValue_accessValue_DEFINE(char, zfchar, zft_zfchar)
_ZFP_ZFValue_accessValue_DEFINE(int, zfint, zft_zfint)
_ZFP_ZFValue_accessValue_DEFINE(unsignedInt, zfuint, zft_zfuint)
_ZFP_ZFValue_accessValue_DEFINE(index, zfindex, zft_zfindex)
_ZFP_ZFValue_accessValue_DEFINE(float, zffloat, zft_zffloat)
_ZFP_ZFValue_accessValue_DEFINE(double, zfdouble, zft_zfdouble)
_ZFP_ZFValue_accessValue_DEFINE(longDouble, zflongdouble, zft_zflongdouble)
_ZFP_ZFValue_accessValue_DEFINE(time, zftimet, zft_zftimet)
_ZFP_ZFValue_accessValue_DEFINE(flags, zfflags, zft_zfflags)
_ZFP_ZFValue_accessValue_DEFINE(identity, zfidentity, zft_zfidentity)

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
        #define _ZFP_ZFValue_valueSet_switch_case(TypeName, Type, originalType) \
            case ZFValueType::ZFM_CAT(e_, TypeName): \
                this->TypeName##ValueSet(another->TypeName##ValueAccess()); \
                break;

        _ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
        _ZFP_ZFValue_valueSet_switch_case(bool, zfbool, zft_zfbool)
        _ZFP_ZFValue_valueSet_switch_case(char, zfchar, zft_zfchar)
        _ZFP_ZFValue_valueSet_switch_case(int, zfint, zft_zfint)
        _ZFP_ZFValue_valueSet_switch_case(unsignedInt, zfuint, zft_zfuint)
        _ZFP_ZFValue_valueSet_switch_case(index, zfindex, zft_zfindex)
        _ZFP_ZFValue_valueSet_switch_case(float, zffloat, zft_zffloat)
        _ZFP_ZFValue_valueSet_switch_case(double, zfdouble, zft_zfdouble)
        _ZFP_ZFValue_valueSet_switch_case(longDouble, zflongdouble, zft_zflongdouble)
        _ZFP_ZFValue_valueSet_switch_case(time, zftimet, zft_zftimet)
        _ZFP_ZFValue_valueSet_switch_case(flags, zfflags, zft_zfflags)
        _ZFP_ZFValue_valueSet_switch_case(identity, zfidentity, zft_zfidentity)

        case ZFValueType::e_serializableData:
            this->serializableDataValueSet(another->serializableDataValueAccess());
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

// ============================================================
#define _ZFP_ZFValue_setValue_DEFINE(TypeName, Type, originalType) \
    void ZFValue::TypeName##ValueSet(ZF_IN Type const &v) \
    { \
        d->clearValue(); \
        d->type = ZFValueType::ZFM_CAT(e_, TypeName); \
        d->v.v_##TypeName = v; \
    }

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValue_setValue_DEFINE(bool, zfbool, zft_zfbool)
_ZFP_ZFValue_setValue_DEFINE(char, zfchar, zft_zfchar)
_ZFP_ZFValue_setValue_DEFINE(int, zfint, zft_zfint)
_ZFP_ZFValue_setValue_DEFINE(unsignedInt, zfuint, zft_zfuint)
_ZFP_ZFValue_setValue_DEFINE(index, zfindex, zft_zfindex)
_ZFP_ZFValue_setValue_DEFINE(float, zffloat, zft_zffloat)
_ZFP_ZFValue_setValue_DEFINE(double, zfdouble, zft_zfdouble)
_ZFP_ZFValue_setValue_DEFINE(longDouble, zflongdouble, zft_zflongdouble)
_ZFP_ZFValue_setValue_DEFINE(time, zftimet, zft_zftimet)
_ZFP_ZFValue_setValue_DEFINE(flags, zfflags, zft_zfflags)
_ZFP_ZFValue_setValue_DEFINE(identity, zfidentity, zft_zfidentity)

void ZFValue::serializableDataValueSet(ZF_IN const ZFSerializableData &v)
{
    d->clearValue();
    d->type = ZFValueType::e_serializableData;
    d->v.v_serializableData = zfnew(ZFSerializableData);
    d->v.v_serializableData->copyFrom(v);
}

// ============================================================
#define _ZFP_ZFValueEditable_create_DEFINE(TypeName, Type, originalType) \
    zfautoObject ZFValueEditable::TypeName##ValueCreate(ZF_IN Type const &v) \
    { \
        zfblockedAlloc(ZFValueEditable, ret); \
        ret->TypeName##ValueSet(v); \
        return zfautoObjectCreate(ret); \
    }

_ZFP_ZFVALUE_DUMMY_FLAG_TO_ADD_TYPE
_ZFP_ZFValueEditable_create_DEFINE(bool, zfbool, zft_zfbool)
_ZFP_ZFValueEditable_create_DEFINE(char, zfchar, zft_zfchar)
_ZFP_ZFValueEditable_create_DEFINE(int, zfint, zft_zfint)
_ZFP_ZFValueEditable_create_DEFINE(unsignedInt, zfuint, zft_zfuint)
_ZFP_ZFValueEditable_create_DEFINE(index, zfindex, zft_zfindex)
_ZFP_ZFValueEditable_create_DEFINE(float, zffloat, zft_zffloat)
_ZFP_ZFValueEditable_create_DEFINE(double, zfdouble, zft_zfdouble)
_ZFP_ZFValueEditable_create_DEFINE(longDouble, zflongdouble, zft_zflongdouble)
_ZFP_ZFValueEditable_create_DEFINE(time, zftimet, zft_zftimet)
_ZFP_ZFValueEditable_create_DEFINE(flags, zfflags, zft_zfflags)
_ZFP_ZFValueEditable_create_DEFINE(identity, zfidentity, zft_zfidentity)
_ZFP_ZFValueEditable_create_DEFINE(serializableData, ZFSerializableData, void)

ZF_NAMESPACE_GLOBAL_END

