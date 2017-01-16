/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTime.h
 * @brief time utility
 */

#ifndef _ZFI_ZFTime_h_
#define _ZFI_ZFTime_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFTimeValue
/**
 * @brief store a time value
 */
zfclassPOD ZF_ENV_EXPORT ZFTimeValue
{
public:
    zftimet sec;     /**< second */
    zftimet usec;    /**< micro second */
};
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTimeValue>
 *       sec="0" // optional, 0 by default
 *       usec="0" // optional, 0 by default
 *   />
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFTimeValue, ZFTimeValue)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTimeValue_sec zfText("sec")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTimeValue_usec zfText("usec")

/**
 * @brief a zero time value
 */
extern ZF_ENV_EXPORT const ZFTimeValue ZFTimeValueZero;
/**
 * @brief make a time value
 */
inline ZFTimeValue ZFTimeValueMake(ZF_IN const zftimet &sec,
                                   ZF_IN const zftimet &usec)
{
    ZFTimeValue ret = {sec, usec};
    return ret;
}

/**
 * @brief convert between #ZFTimeValue and #zftimet (in miliseconds)
 *
 * note, convert from #ZFTimeValue to #zftimet would lose accuracy
 */
inline ZFTimeValue ZFTimeValueFromMiliSeconds(ZF_IN zftimet t)
{
    ZFTimeValue ret = {(zftimet)(t / 1000), (zftimet)((t % 1000) * 1000)};
    return ret;
}
/**
 * @brief convert between #ZFTimeValue and #zftimet (in miliseconds)
 *
 * note, convert from #ZFTimeValue to #zftimet would lose accuracy
 */
inline zftimet ZFTimeValueToMiliSeconds(ZF_IN const ZFTimeValue &v)
{
    return (zftimet)(v.sec * 1000 + v.usec / 1000);
}

/**
 * @brief normalize time value
 */
extern ZF_ENV_EXPORT void ZFTimeValueNormalize(ZF_IN_OUT ZFTimeValue &v);

/**
 * @brief return tv1 - tv2
 */
extern ZF_ENV_EXPORT void ZFTimeValueDec(ZF_OUT ZFTimeValue &result,
                                         ZF_IN const ZFTimeValue &tv1,
                                         ZF_IN const ZFTimeValue &tv2);
/**
 * @brief return tv1 - tv2
 */
extern ZF_ENV_EXPORT ZFTimeValue ZFTimeValueDec(ZF_IN const ZFTimeValue &tv1,
                                                ZF_IN const ZFTimeValue &tv2);
/**
 * @brief return tv1 + tv2
 */
extern ZF_ENV_EXPORT void ZFTimeValueInc(ZF_OUT ZFTimeValue &result,
                                         ZF_IN const ZFTimeValue &tv1,
                                         ZF_IN const ZFTimeValue &tv2);
/**
 * @brief return tv1 + tv2
 */
extern ZF_ENV_EXPORT ZFTimeValue ZFTimeValueInc(ZF_IN const ZFTimeValue &tv1,
                                                ZF_IN const ZFTimeValue &tv2);
/**
 * @brief return tv * v
 */
extern ZF_ENV_EXPORT void ZFTimeValueMul(ZF_OUT ZFTimeValue &result,
                                         ZF_IN const ZFTimeValue &tv,
                                         ZF_IN zfindex v);
/**
 * @brief return tv * v
 */
extern ZF_ENV_EXPORT ZFTimeValue ZFTimeValueMul(ZF_IN const ZFTimeValue &tv,
                                                ZF_IN zfindex v);
/**
 * @brief return tv / v, assert fail if v is 0
 */
extern ZF_ENV_EXPORT void ZFTimeValueDiv(ZF_OUT ZFTimeValue &result,
                                         ZF_IN const ZFTimeValue &tv,
                                         ZF_IN zfindex v);
/**
 * @brief return tv / v, assert fail if v is 0
 */
extern ZF_ENV_EXPORT ZFTimeValue ZFTimeValueDiv(ZF_IN const ZFTimeValue &tv,
                                                ZF_IN zfindex v);

/**
 * @brief compare two ZFTimeValue
 * @see ZFCompareResult
 */
extern ZF_ENV_EXPORT ZFCompareResult ZFTimeValueCompare(ZF_IN const ZFTimeValue &tv1,
                                                        ZF_IN const ZFTimeValue &tv2);

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFTimeValue, ZFTimeValue)
ZFVAR_CONVERT_WRAPPER_DECLARE(ZFTimeValue, ZFTimeValue)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFTimeValue, ZFTimeValue)

/**
 * @brief convert ZFTimeValue to more readable string, see #ZFTimeValueToString
 *
 * can't be converted back from string, usually for debug use only
 */
extern ZF_ENV_EXPORT void ZFTimeValueToStringFriendly(ZF_IN_OUT zfstring &ret, ZF_IN ZFTimeValue const &value);
/** @brief see #ZFTimeValueToStringFriendly */
inline zfstring ZFTimeValueToStringFriendly(ZF_IN ZFTimeValue const &value)
{
    zfstring ret;
    ZFTimeValueToStringFriendly(ret, value);
    return ret;
}

ZFCORE_POD_COMPARER_DECLARE(ZFTimeValue)
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT zfbool operator <(ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT zfbool operator <=(ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT zfbool operator >(ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT zfbool operator >=(ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT ZFTimeValue operator +(ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT ZFTimeValue operator -(ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT void operator +=(ZF_IN_OUT ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT void operator -=(ZF_IN_OUT ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT ZFTimeValue operator *(ZF_IN const ZFTimeValue &v0, ZF_IN const zfindex &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT ZFTimeValue operator /(ZF_IN const ZFTimeValue &v0, ZF_IN const zfindex &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT void operator *=(ZF_IN_OUT ZFTimeValue &v0, ZF_IN const zfindex &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT void operator /=(ZF_IN_OUT ZFTimeValue &v0, ZF_IN const zfindex &v1);

ZFCOMPARER_DEFAULT_DECLARE(ZFTimeValue, ZFTimeValue, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

ZFOUTPUT_TYPE_DECLARE(ZFTimeValue)
ZFOUTPUT_TYPE(const ZFTimeValue *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFTimeValue *, {output << (const ZFTimeValue *)v;})
ZFINPUT_TYPE_DECLARE(ZFTimeValue, ZFTimeValue)

// ============================================================
// ZFTimeInfo
/**
 * @brief store a time info, internal use only
 */
zfclassPOD ZF_ENV_EXPORT ZFTimeInfo
{
public:
    zfint year;          /**< e.g. 1999 */
    zfuint month;        /**< [0, 11] */
    zfuint day;          /**< [0, 27...30] */
    zfuint hour;         /**< [0, 23] */
    zfuint minute;       /**< [0, 59] */
    zfuint second;       /**< [0, 59] */
    zfuint miliSecond;   /**< [0, 999] */
    zfuint microSecond;  /**< [0, 999] */
};
/**
 * @brief a zero time info (0000-00-00 00:00:00.000 000)
 */
extern ZF_ENV_EXPORT const ZFTimeInfo ZFTimeInfoZero;
/**
 * @brief make a time info, you should make sure value is valid
 */
inline ZFTimeInfo ZFTimeInfoMake(ZF_IN zfint const &year,
                                 ZF_IN zfuint const &month,
                                 ZF_IN zfuint const &day,
                                 ZF_IN zfuint const &hour,
                                 ZF_IN zfuint const &minute,
                                 ZF_IN zfuint const &second,
                                 ZF_IN zfuint const &miliSecond,
                                 ZF_IN zfuint const &microSecond)
{
    ZFTimeInfo ret = {year, month, day, hour, minute, second, miliSecond, microSecond};
    return ret;
}

ZFCORE_POD_COMPARER_DECLARE(ZFTimeInfo)
ZFCOMPARER_DEFAULT_DECLARE(ZFTimeInfo, ZFTimeInfo, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

ZFOUTPUT_TYPE_DECLARE(ZFTimeInfo)
ZFOUTPUT_TYPE(const ZFTimeInfo *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFTimeInfo *, {output << (const ZFTimeInfo *)v;})

/**
 * @brief convert ZFTimeInfo to string
 */
extern ZF_ENV_EXPORT void ZFTimeInfoToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFTimeInfo const &value);
/** @brief see #ZFTimeInfoToString */
inline zfstring ZFTimeInfoToString(ZF_IN ZFTimeInfo const &value)
{
    zfstring ret;
    ZFTimeInfoToString(ret, value);
    return ret;
}

// ============================================================
// ZFTime
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTime_timeValue zfText("timeValue")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTime_timeZone zfText("timeZone")

zfclassFwd _ZFP_ZFTimePrivate;
/**
 * @brief time utility
 *
 * use a ZFTimeValue to store time,
 * which means how many seconds had passed since #ZFTimeInfoZero\n
 * a positive value means time that is after #ZFTimeInfoZero,
 * and a negative value means time that is before #ZFTimeInfoZero\n
 * ZFTimeValue as well as ZFTime contains no actual time logic
 * such as time zone, UTC, GMT, etc,
 * it is simply a time unit, what it means is depends on you\n
 * \n
 * for performance, you may want to use ZFTimeValue or zftimet to process times,
 * is enough for most case and is recommended\n
 * \n
 * serializable data:
 * @code
 *   <ZFTime
 *       timeValue="timeValueString" // ZFTime::timeValueSet, optional, ZFTimeValueZero by default
 *       timeZone="timeZoneString" // ZFTime::timeZoneSet, optional, ZFTimeValueZero by default
 *   />
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFTime : zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFTime, ZFObject)
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
        this->timeValueSet(ZFCastZFObjectUnchecked(zfself *, anotherObj)->timeValue());
        this->timeZoneSet(ZFCastZFObjectUnchecked(zfself *, anotherObj)->timeZone());
    }

public:
    /**
     * @brief return leap year num in range [year1, year2],
     *   may return negative value if (year1 > year2)
     */
    static zfint leapYearBetween(ZF_IN zfint year1,
                                ZF_IN zfint year2);
    /**
     * @brief return true if year is leap year
     */
    static zfbool leapYear(ZF_IN zfint year);

    /**
     * @brief get timestamp in mili seconds
     *
     * use this instead of #currentTimeValue if you only care about time interval,
     * this value is a relative value, what 0 means is not ensured\n
     * \n
     * typically, this method would have better performance and accuracy than #currentTimeValue
     */
    static zftimet timestamp(void);

    /**
     * @brief return time since #ZFTimeInfoZero, negative if before #ZFTimeInfoZero
     */
    static ZFTimeValue currentTimeValue(void);
    /**
     * @brief equal to ZFTimeValueToMiliSeconds(currentTimeValue())
     */
    static inline zftimet currentTimeMiliSeconds(void)
    {
        return ZFTimeValueToMiliSeconds(ZFTime::currentTimeValue());
    }
    /**
     * @brief return current time info, util method to #currentTimeValue
     */
    static ZFTimeInfo currentTimeInfo(ZF_IN_OPT const ZFTimeValue &localTimeZone = ZFTime::timeZoneLocal());
    /**
     * @brief convert time value to time info
     */
    static zfbool timeInfoFromTimeValue(ZF_OUT ZFTimeInfo &ti,
                       ZF_IN const ZFTimeValue &tv);
    /**
     * @brief convert time info to time value
     */
    static zfbool timeInfoToTimeValue(ZF_OUT ZFTimeValue &tv,
                       ZF_IN const ZFTimeInfo &ti);
    /**
     * @brief convert time info to time value
     */
    static zfbool timeInfoToTimeValue(ZF_OUT ZFTimeValue &tv,
                                      ZF_IN zfint year,
                                      ZF_IN zfuint month,
                                      ZF_IN zfuint day,
                                      ZF_IN zfuint hour,
                                      ZF_IN zfuint minute,
                                      ZF_IN zfuint second,
                                      ZF_IN zfuint miliSecond,
                                      ZF_IN zfuint microSecond);

    /**
     * @brief get current time zone
     *
     * while getting time info from a ZFTime object,
     * time zone would be applied,
     * simply by #ZFTimeValueInc to the time value of ZFTime object
     */
    static const ZFTimeValue &timeZoneLocal(void);

public:
    /**
     * @brief init from another ZFTime
     */
    virtual ZFObject *objectOnInit(ZF_IN ZFTime *time);
    /**
     * @brief init from time value
     */
    virtual ZFObject *objectOnInit(ZF_IN const ZFTimeValue &tv,
                                   ZF_IN_OPT const ZFTimeValue &timeZone = zfself::timeZoneLocal());
    /**
     * @brief init from time info, or zero time if invalid
     */
    virtual ZFObject *objectOnInit(ZF_IN const ZFTimeInfo &ti,
                                   ZF_IN_OPT const ZFTimeValue &timeZone = zfself::timeZoneLocal());
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    zfoverride
    virtual zfidentity objectHash(void);

    /**
     * @brief compared by #timeValueAppliedTimeZone
     */
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief set time according to time value
     */
    virtual zfbool timeValueSet(ZF_IN const ZFTimeValue &tv);
    /**
     * @brief access time value
     */
    virtual const ZFTimeValue &timeValue(void);

    /**
     * @brief set time zone attched to this ZFTime object
     * @see timeZoneLocal
     */
    virtual zfbool timeZoneSet(ZF_IN_OPT const ZFTimeValue &timeZone = zfself::timeZoneLocal());
    /**
     * @brief get time zone attched to this ZFTime object
     */
    virtual const ZFTimeValue &timeZone(void);

    /**
     * @brief get time value which has been applied by time zone offset
     */
    virtual const ZFTimeValue &timeValueAppliedTimeZone(void);

    /**
     * @brief set time according to time info
     *
     * set time to #ZFTimeInfoZero if input is invalid
     */
    virtual zfbool timeInfoSet(ZF_IN zfint year,
                               ZF_IN zfuint month,
                               ZF_IN zfuint day,
                               ZF_IN zfuint hour,
                               ZF_IN zfuint minute,
                               ZF_IN zfuint second,
                               ZF_IN zfuint miliSecond,
                               ZF_IN zfuint microSecond,
                               ZF_IN_OPT const ZFTimeValue &tz = ZFTime::timeZoneLocal());
    /**
     * @brief set time according to time info
     *
     * set time to #ZFTimeInfoZero if input is invalid
     */
    virtual zfbool timeInfoSet(ZF_IN const ZFTimeInfo &ti,
                               ZF_IN_OPT const ZFTimeValue &tz = ZFTime::timeZoneLocal());
    /**
     * @brief get time info of current time value (by #timeValueAppliedTimeZone)
     */
    virtual const ZFTimeInfo &timeInfo(void);

    /**
     * @brief year since 0000, e.g. 2012
     */
    virtual zfint timeInfoYear(void);

    /**
     * @brief month range in [0, 11]
     */
    virtual zfuint timeInfoMonth(void);

    /**
     * @brief day of month range in [0, 27...30]
     */
    virtual zfuint timeInfoDay(void);

    /**
     * @brief day of week range in [0, 6], while 0 stands for Sunday
     */
    virtual zfuint timeInfoDayOfWeek(void);
    /**
     * @brief day of year range in [0, 364/365]
     */
    virtual zfuint timeInfoDayOfYear(void);

    /**
     * @brief hour range in [0, 23]
     */
    virtual zfuint timeInfoHour(void);

    /**
     * @brief minute range in [0, 59]
     */
    virtual zfuint timeInfoMinute(void);

    /**
     * @brief second range in [0, 59]
     */
    virtual zfuint timeInfoSecond(void);

    /**
     * @brief miliSecond range in [0, 999]
     */
    virtual zfuint timeInfoMiliSecond(void);

    /**
     * @brief microSecond range in [0, 999]
     */
    virtual zfuint timeInfoMicroSecond(void);

private:
    _ZFP_ZFTimePrivate *d;
};

// ============================================================
/** @brief in seconds */
#define zftimetOneMinute 60LL
/** @brief in seconds */
#define zftimetOneHour 3600LL // 60 * 60
/** @brief in seconds */
#define zftimetOneDay 86400LL // 60 * 60 * 24
/** @brief in seconds */
#define zftimetOneWeek 604800LL // 60 * 60 * 24 * 7
/** @brief 30 days in seconds */
#define zftimetOneMonth 2592000LL // 60 * 60 * 24 * 30
/** @brief 365 days in seconds */
#define zftimetOneYear 31536000LL // 60 * 60 * 24 * 365

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTime_h_

