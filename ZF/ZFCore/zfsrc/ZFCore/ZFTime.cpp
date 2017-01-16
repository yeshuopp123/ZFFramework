/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFTime.h"
#include "protocol/ZFProtocolZFTime.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFTimeValue
const ZFTimeValue ZFTimeValueZero = {0, 0};

ZFVAR_CONVERT_WRAPPER_DEFINE(ZFTimeValue)

void ZFTimeValueNormalize(ZF_IN_OUT ZFTimeValue &v)
{
    while(v.usec >= 1000000LL)
    {
        ++v.sec;
        v.usec -= 1000000LL;
    }
    while(v.usec <= -1000000LL)
    {
        --v.sec;
        v.usec += 1000000LL;
    }
    if(v.sec > 0 && v.usec < 0)
    {
        --v.sec;
        v.usec += 1000000LL;
    }
    else if(v.sec < 0 && v.usec > 0)
    {
        ++v.sec;
        v.usec -= 1000000LL;
    }
}

void ZFTimeValueDec(ZF_OUT ZFTimeValue &result,
                    ZF_IN const ZFTimeValue &tv1,
                    ZF_IN const ZFTimeValue &tv2)
{
    result.sec = tv1.sec - tv2.sec;
    result.usec = tv1.usec - tv2.usec;
    ZFTimeValueNormalize(result);
}
ZFTimeValue ZFTimeValueDec(ZF_IN const ZFTimeValue &tv1,
                           ZF_IN const ZFTimeValue &tv2)
{
    ZFTimeValue result;
    ZFTimeValueDec(result, tv1, tv2);
    return result;
}
void ZFTimeValueInc(ZF_OUT ZFTimeValue &result,
                    ZF_IN const ZFTimeValue &tv1,
                    ZF_IN const ZFTimeValue &tv2)
{
    result.sec = tv1.sec + tv2.sec;
    result.usec = tv1.usec + tv2.usec;
    ZFTimeValueNormalize(result);
}
ZFTimeValue ZFTimeValueInc(ZF_IN const ZFTimeValue &tv1,
                           ZF_IN const ZFTimeValue &tv2)
{
    ZFTimeValue result;
    ZFTimeValueInc(result, tv1, tv2);
    return result;
}
void ZFTimeValueMul(ZF_OUT ZFTimeValue &result,
                    ZF_IN const ZFTimeValue &tv,
                    ZF_IN zfindex v)
{
    result.sec = tv.sec * v;
    result.usec = tv.usec * v;
    ZFTimeValueNormalize(result);
}
ZFTimeValue ZFTimeValueMul(ZF_IN const ZFTimeValue &tv,
                           ZF_IN zfindex v)
{
    ZFTimeValue result;
    ZFTimeValueMul(result, tv, v);
    return result;
}
void ZFTimeValueDiv(ZF_OUT ZFTimeValue &result,
                    ZF_IN const ZFTimeValue &tv,
                    ZF_IN zfindex v)
{
    zfCoreAssertWithMessage(v != 0, zfTextA("divided by 0"));
    result.usec = tv.usec / v;
    zflongdouble secTmp = (zflongdouble)tv.sec / v;
    result.sec = tv.sec / v;
    result.usec += (zftimet)((secTmp - result.sec) * 1000000LL);
    ZFTimeValueNormalize(result);
}
ZFTimeValue ZFTimeValueDiv(ZF_IN const ZFTimeValue &tv,
                           ZF_IN zfindex v)
{
    ZFTimeValue result;
    ZFTimeValueDiv(result, tv, v);
    return result;
}

ZFCompareResult ZFTimeValueCompare(ZF_IN const ZFTimeValue &tv1,
                                   ZF_IN const ZFTimeValue &tv2)
{
    if(tv1.sec == tv2.sec)
    {
        if(tv1.usec == tv2.usec)
        {
            return ZFCompareTheSame;
        }
        else if(tv1.usec < tv2.usec)
        {
            return ZFCompareSmaller;
        }
        else
        {
            return ZFCompareGreater;
        }
    }
    else if(tv1.sec < tv2.sec)
    {
        return ZFCompareSmaller;
    }
    else
    {
        return ZFCompareGreater;
    }
}

void ZFTimeValueToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFTimeValue const &value)
{
    ret += zfText("(");
    zfsFromIntT(ret, value.sec);
    ret += zfText(", ");
    zfsFromIntT(ret, value.usec);
    ret += zfText(")");
}
const zfchar *ZFTimeValueFromString(ZF_OUT ZFTimeValue &ret,
                                    ZF_IN const zfchar *src,
                                    ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    ZFCoreArrayPOD<zftimet> tmp;
    const zfchar *errPos = zfCoreDataPairSplitInt(tmp, 2, src, srcLen);
    if(errPos != zfnull)
    {
        return errPos;
    }
    ret.sec = tmp[0];
    ret.usec = tmp[1];
    return zfnull;
}

void ZFTimeValueToStringFriendly(ZF_IN_OUT zfstring &ret, ZF_IN ZFTimeValue const &value)
{
    ZFTimeInfo ti;
    if(ZFTime::timeInfoFromTimeValue(ti, value))
    {
        if(ti.year != 0)
        {
            zfstringAppend(ret, zfText("%02d-"), ti.year);
        }
        if(!ret.isEmpty() || ti.month != 0 || ti.day != 0)
        {
            zfstringAppend(ret, zfText("%02u-%02u "), ti.month + 1, ti.day + 1);
        }

        if(!ret.isEmpty() || ti.hour != 0 || ti.minute != 0)
        {
            zfstringAppend(ret, zfText("%02u:%02u:"), ti.hour, ti.minute);
        }

        if(!ret.isEmpty())
        {
            zfstringAppend(ret, zfText("%02u."), ti.second);
        }
        else
        {
            zfstringAppend(ret, zfText("%u."), ti.second);
        }

        zfstringAppend(ret, zfText("%03u%03u"), ti.miliSecond, ti.microSecond);
    }
    else
    {
        zfstringAppend(ret, zfText("%s.%06s"), zfsFromInt(value.sec).cString(), zfsFromInt(value.usec).cString());
    }
}

zfbool operator <(ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1)
{
    return (ZFTimeValueCompare(v0, v1) == ZFCompareSmaller);
}
zfbool operator <=(ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1)
{
    return (ZFTimeValueCompare(v0, v1) != ZFCompareGreater);
}
zfbool operator >(ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1)
{
    return (ZFTimeValueCompare(v0, v1) == ZFCompareGreater);
}
zfbool operator >=(ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1)
{
    return (ZFTimeValueCompare(v0, v1) != ZFCompareSmaller);
}
ZFTimeValue operator +(ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1)
{
    return ZFTimeValueInc(v0, v1);
}
ZFTimeValue operator -(ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1)
{
    return ZFTimeValueDec(v0, v1);
}
void operator +=(ZF_IN_OUT ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1)
{
    ZFTimeValueInc(v0, v0, v1);
}
void operator -=(ZF_IN_OUT ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1)
{
    ZFTimeValueDec(v0, v0, v1);
}
ZFTimeValue operator *(ZF_IN const ZFTimeValue &v0, ZF_IN const zfindex &v1)
{
    return ZFTimeValueMul(v0, v1);
}
ZFTimeValue operator /(ZF_IN const ZFTimeValue &v0, ZF_IN const zfindex &v1)
{
    return ZFTimeValueDiv(v0, v1);
}
void operator *=(ZF_IN_OUT ZFTimeValue &v0, ZF_IN const zfindex &v1)
{
    ZFTimeValueMul(v0, v0, v1);
}
void operator /=(ZF_IN_OUT ZFTimeValue &v0, ZF_IN const zfindex &v1)
{
    ZFTimeValueDiv(v0, v0, v1);
}

// ============================================================
ZFPROPERTY_TYPE_DEFINE(ZFTimeValue, ZFTimeValue, {
        v = ZFTimeValueZero;

        if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZFTimeValue(), serializableData, outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        const zfchar *element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTimeValue_sec);
        if(element != zfnull && zftimetFromString(v.sec, element) != zfnull)
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTimeValue_sec, element);
            return zffalse;
        }

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTimeValue_usec);
        if(element != zfnull && zftimetFromString(v.usec, element) != zfnull)
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTimeValue_usec, element);
            return zffalse;
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        serializableData.itemClassSet(ZFPropertyTypeId_ZFTimeValue());

        if(v.sec != 0)
        {
            serializableData.attributeSet(ZFSerializableKeyword_ZFTimeValue_sec, zftimetToString(v.sec));
        }
        if(v.usec != 0)
        {
            serializableData.attributeSet(ZFSerializableKeyword_ZFTimeValue_usec, zftimetToString(v.usec));
        }

        return zftrue;
    })

// ============================================================
ZFOUTPUT_TYPE_DEFINE(ZFTimeValue, {output << ZFTimeValueToString(v);})
ZFINPUT_TYPE_DEFINE(ZFTimeValue, ZFTimeValue, {
    v = ZFTimeValueZero;
    ZFCoreArrayPOD<zftimet> elementsTmp;
    if(!ZFInputCallbackReadDataPairInt(elementsTmp, input, 2))
    {
        return zffalse;
    }
    v.sec = elementsTmp[0];
    v.usec = elementsTmp[1];
    return zftrue;
})

// ============================================================
// ZFTimeInfo
const ZFTimeInfo ZFTimeInfoZero = {0, 0, 0, 0, 0, 0, 0, 0};

void ZFTimeInfoToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFTimeInfo const &value)
{
    zfstringAppend(ret, zfText("%d-%02u-%02u %02u:%02u:%02u.%03u %03u"),
        value.year, value.month + 1, value.day + 1,
        value.hour, value.minute, value.second,
        value.miliSecond, value.microSecond);
}

// ============================================================
ZFOUTPUT_TYPE_DEFINE(ZFTimeInfo, {output << ZFTimeInfoToString(v);})

// ============================================================
#define _ZFP_ZFTimeImpl ZFPROTOCOL_ACCESS(ZFTime)

// _ZFP_ZFTimePrivate
zfclassNotPOD _ZFP_ZFTimePrivate
{
public:
    ZFTimeValue tv;
    ZFTimeValue tz;
    ZFTimeValue tvAppliedTimeZone;
    ZFTimeInfo ti;
public:
    _ZFP_ZFTimePrivate(void)
    : tv(ZFTimeValueZero)
    , tz(ZFTimeValueZero)
    , tvAppliedTimeZone(ZFTimeValueZero)
    , ti(ZFTimeInfoZero)
    {
    }
    void clear(void)
    {
        this->tv = ZFTimeValueZero;
        this->tz = ZFTimeValueZero;
        this->tvAppliedTimeZone = ZFTimeValueZero;
        this->ti = ZFTimeInfoZero;
    }
    void copyFrom(_ZFP_ZFTimePrivate *p)
    {
        this->tv = p->tv;
        this->tz = p->tz;
        this->tvAppliedTimeZone = p->tvAppliedTimeZone;
        this->ti = p->ti;
    }
    zfbool timeValueSet(const ZFTimeValue &tv)
    {
        this->tv = tv;
        ZFTimeValueInc(this->tvAppliedTimeZone, this->tv, this->tz);
        if(!_ZFP_ZFTimeImpl->timeInfoFromTimeValue(this->ti, this->tvAppliedTimeZone))
        {
            this->clear();
            return zffalse;
        }

        return zftrue;
    }
    zfbool timeZoneSet(const ZFTimeValue &tz)
    {
        this->tz = tz;
        ZFTimeValueInc(this->tvAppliedTimeZone, this->tv, this->tz);
        if(!_ZFP_ZFTimeImpl->timeInfoFromTimeValue(this->ti, this->tvAppliedTimeZone))
        {
            this->clear();
            return zffalse;
        }

        return zftrue;
    }
    zfbool timeInfoSet(ZF_IN const ZFTimeInfo &ti,
                       ZF_IN const ZFTimeValue &tz)
    {
        if(!_ZFP_ZFTimeImpl->timeInfoToTimeValue(this->tvAppliedTimeZone, ti))
        {
            this->clear();
            return zffalse;
        }

        this->ti = ti;
        this->tz = tz;
        ZFTimeValueDec(this->tv, this->tvAppliedTimeZone, this->tz);
        return zftrue;
    }
};

// ============================================================
// ZFTime
ZFOBJECT_REGISTER(ZFTime)

zfbool ZFTime::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    ZFTimeValue timeValue = ZFTimeValueZero;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFTime_timeValue, ZFTimeValue, timeValue);
    this->timeValueSet(timeValue);

    ZFTimeValue timeZone = ZFTimeValueZero;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFTime_timeZone, ZFTimeValue, timeZone);
    this->timeZoneSet(timeZone);

    return zftrue;
}
zfbool ZFTime::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                             ZF_IN ZFSerializable *referencedOwnerOrNull,
                                             ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHint, ref,
        ZFSerializableKeyword_ZFTime_timeValue, ZFTimeValue, this->timeValue(), ref->timeValue(), ZFTimeValueZero);

    ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHint, ref,
        ZFSerializableKeyword_ZFTime_timeZone, ZFTimeValue, this->timeZone(), ref->timeZone(), ZFTimeValueZero);

    return zftrue;
}

zfbool ZFTime::leapYear(ZF_IN zfint year)
{
    return _ZFP_ZFTimeImpl->leapYear(year);
}
zfint ZFTime::leapYearBetween(ZF_IN zfint year1,
                              ZF_IN zfint year2)
{
    return _ZFP_ZFTimeImpl->leapYearBetween(year1, year2);
}
zftimet ZFTime::timestamp(void)
{
    return _ZFP_ZFTimeImpl->timestamp();
}
ZFTimeValue ZFTime::currentTimeValue(void)
{
    ZFTimeValue tv;
    _ZFP_ZFTimeImpl->currentTimeValue(tv);
    return tv;
}
ZFTimeInfo ZFTime::currentTimeInfo(ZF_IN_OPT const ZFTimeValue &localTimeZone /* = ZFTime::timeZoneLocal() */)
{
    ZFTimeInfo ti;
    ZFTime::timeInfoFromTimeValue(ti,
        ZFTimeValueInc(ZFTime::currentTimeValue(), localTimeZone));
    return ti;
}
zfbool ZFTime::timeInfoFromTimeValue(ZF_OUT ZFTimeInfo &ti,
                   ZF_IN const ZFTimeValue &tv)
{
    return _ZFP_ZFTimeImpl->timeInfoFromTimeValue(ti, tv);
}
zfbool ZFTime::timeInfoToTimeValue(ZF_OUT ZFTimeValue &tv,
                   ZF_IN const ZFTimeInfo &ti)
{
    return _ZFP_ZFTimeImpl->timeInfoToTimeValue(tv, ti);
}
zfbool ZFTime::timeInfoToTimeValue(ZF_OUT ZFTimeValue &tv,
                                   ZF_IN zfint year,
                                   ZF_IN zfuint month,
                                   ZF_IN zfuint day,
                                   ZF_IN zfuint hour,
                                   ZF_IN zfuint minute,
                                   ZF_IN zfuint second,
                                   ZF_IN zfuint miliSecond,
                                   ZF_IN zfuint microSecond)
{
    return _ZFP_ZFTimeImpl->timeInfoToTimeValue(
        tv,
        ZFTimeInfoMake(year, month, day, hour, minute, second, miliSecond, microSecond));
}

const ZFTimeValue &ZFTime::timeZoneLocal(void)
{
    return _ZFP_ZFTimeImpl->timeZoneLocal();
}

ZFObject *ZFTime::objectOnInit(ZF_IN ZFTime *time)
{
    this->objectOnInit();
    if(time != zfnull)
    {
        d->copyFrom(time->d);
    }
    return this;
}
ZFObject *ZFTime::objectOnInit(ZF_IN const ZFTimeValue &tv,
                               ZF_IN_OPT const ZFTimeValue &timeZone /* = zfself::timeZoneLocal() */)
{
    this->objectOnInit();
    zfself::timeZoneSet(timeZone);
    zfself::timeValueSet(tv);
    return this;
}
ZFObject *ZFTime::objectOnInit(ZF_IN const ZFTimeInfo &ti,
                               ZF_IN_OPT const ZFTimeValue &timeZone /* = zfself::timeZoneLocal() */)
{
    this->objectOnInit();
    zfself::timeInfoSet(ti, timeZone);
    return this;
}
ZFObject *ZFTime::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFTimePrivate);
    return this;
}
void ZFTime::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFTime::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfstringAppend(ret, zfText("%04d-%02d-%02d %02d:%02d:%02d.%03d %03d"),
        this->timeInfoYear(),
        this->timeInfoMonth() + 1,
        this->timeInfoDay() + 1,
        this->timeInfoHour(),
        this->timeInfoMinute(),
        this->timeInfoSecond(),
        this->timeInfoMiliSecond(),
        this->timeInfoMicroSecond());
}

zfclassPOD _ZFP_ZFTime_hash
{
public:
    ZFTimeValue tv;
    ZFTimeValue tz;
};
zfidentity ZFTime::objectHash(void)
{
    _ZFP_ZFTime_hash tmp = {d->tv, d->tz};
    return zfidentityCalcPOD(tmp);
}

ZFCompareResult ZFTime::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    return ZFTimeValueCompare(this->timeValueAppliedTimeZone(), another->timeValueAppliedTimeZone());
}
zfbool ZFTime::timeValueSet(ZF_IN const ZFTimeValue &tv)
{
    d->timeValueSet(tv);
    return _ZFP_ZFTimeImpl->timeInfoFromTimeValue(d->ti, d->tvAppliedTimeZone);
}
const ZFTimeValue &ZFTime::timeValue(void)
{
    return d->tv;
}

zfbool ZFTime::timeZoneSet(ZF_IN_OPT const ZFTimeValue &timeZone /* = zfself::timeZoneLocal() */)
{
    return d->timeZoneSet(timeZone);
}
const ZFTimeValue &ZFTime::timeZone(void)
{
    return d->tz;
}
const ZFTimeValue &ZFTime::timeValueAppliedTimeZone(void)
{
    return d->tvAppliedTimeZone;
}

zfbool ZFTime::timeInfoSet(ZF_IN zfint year,
                           ZF_IN zfuint month,
                           ZF_IN zfuint day,
                           ZF_IN zfuint hour,
                           ZF_IN zfuint minute,
                           ZF_IN zfuint second,
                           ZF_IN zfuint miliSecond,
                           ZF_IN zfuint microSecond,
                           ZF_IN_OPT const ZFTimeValue &tz /* = ZFTime::timeZoneLocal() */)
{
    return d->timeInfoSet(ZFTimeInfoMake(year, month, day, hour, minute, second, miliSecond, microSecond), tz);
}
zfbool ZFTime::timeInfoSet(ZF_IN const ZFTimeInfo &ti,
                        ZF_IN_OPT const ZFTimeValue &tz /* = ZFTime::timeZoneLocal() */)
{
    return d->timeInfoSet(ti, tz);
}
const ZFTimeInfo &ZFTime::timeInfo(void)
{
    return d->ti;
}

zfint ZFTime::timeInfoYear(void)
{
    return d->ti.year;
}
zfuint ZFTime::timeInfoMonth(void)
{
    return d->ti.month;
}
zfuint ZFTime::timeInfoDay(void)
{
    return d->ti.day;
}
zfuint ZFTime::timeInfoDayOfWeek(void)
{
    return _ZFP_ZFTimeImpl->timeInfoDayOfWeek(d->tvAppliedTimeZone, d->ti);
}
zfuint ZFTime::timeInfoDayOfYear(void)
{
    return _ZFP_ZFTimeImpl->timeInfoDayOfYear(d->tvAppliedTimeZone, d->ti);
}
zfuint ZFTime::timeInfoHour(void)
{
    return d->ti.hour;
}
zfuint ZFTime::timeInfoMinute(void)
{
    return d->ti.minute;
}
zfuint ZFTime::timeInfoSecond(void)
{
    return d->ti.second;
}
zfuint ZFTime::timeInfoMiliSecond(void)
{
    return d->ti.miliSecond;
}
zfuint ZFTime::timeInfoMicroSecond(void)
{
    return d->ti.microSecond;
}

ZF_NAMESPACE_GLOBAL_END

