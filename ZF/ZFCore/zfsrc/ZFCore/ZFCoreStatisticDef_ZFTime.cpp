/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreStatisticDef_ZFTime.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

template<typename T_Time>
zfclassPOD _ZFP_ZFCoreStatisticInvokeTimeDataHolder
{
public:
    zfindex invokeCount;
    T_Time invokeTotalTime;
    T_Time invokeStartTime;
    zfindex reentrantCount;
};

// ============================================================
static ZFCoreMap &_ZFP_ZFCoreStatisticInvokeTimeDataMap(void)
{
    static ZFCoreMap m; // _ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet>
    return m;
}

void invokeTimeLogBegin(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *>(key);
    if(data == zfnull)
    {
        data = (_ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *)zfmalloc(sizeof(_ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet>));
        zfmemset(data, 0, sizeof(_ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet>));
        m.set(key, ZFCorePointerForPOD<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *>(data));

        ++(data->invokeCount);
        data->invokeStartTime = ZFTime::timestamp();
    }
    else
    {
        ++(data->invokeCount);

        if(data->invokeStartTime != 0)
        {
            ++(data->reentrantCount);
        }
        else
        {
            data->invokeStartTime = ZFTime::timestamp();
        }
    }
}
void invokeTimeLogEnd(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *>(key);
    if(data != zfnull)
    {
        if(data->reentrantCount > 0)
        {
            --(data->reentrantCount);
        }
        else
        {
            data->invokeTotalTime += ZFTime::timestamp() - data->invokeStartTime;
            data->invokeStartTime = 0;
        }
    }
}
void invokeTimeRemove(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    m.remove(key);
}
void invokeTimeRemoveAll(void)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    m.removeAll();
}
zfindex invokeTimeGetInvokeCount(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *>(key);
    if(data != zfnull)
    {
        return data->invokeCount;
    }
    return 0;
}
zftimet invokeTimeGetAverageTime(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *>(key);
    if(data != zfnull && data->invokeCount > 0)
    {
        return data->invokeTotalTime / data->invokeCount;
    }
    return 0;
}
zftimet invokeTimeGetTotalTime(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *>(key);
    if(data != zfnull)
    {
        return data->invokeTotalTime;
    }
    return 0;
}
void invokeTimeGetSummary(ZF_OUT zfstring &ret, ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<zftimet> *>(key);
    zfindex invokeCount = ((data == zfnull) ? 0 : data->invokeCount);
    zftimet invokeTotalTime = ((data == zfnull) ? 0 : data->invokeTotalTime);
    zftimet invokeAverageTime = ((invokeCount == 0) ? 0 : invokeTotalTime / invokeCount);
    zfstringAppend(ret, zfText("[%s] invoke count: %s, total: %s, average: %s"),
        (key == zfnull) ? ZFTOKEN_zfnull : key,
        zfsFromInt(invokeCount).cString(),
        zfsFromInt(invokeTotalTime).cString(),
        zfsFromInt(invokeAverageTime).cString());
}

// ============================================================
static ZFCoreMap &_ZFP_ZFCoreStatisticInvokeTimeAccurateDataMap(void)
{
    static ZFCoreMap m; // _ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue>
    return m;
}
void invokeTimeAccurateLogBegin(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeAccurateDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *>(key);
    if(data == zfnull)
    {
        data = (_ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *)zfmalloc(sizeof(_ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue>));
        zfmemset(data, 0, sizeof(_ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue>));
        m.set(key, ZFCorePointerForPOD<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *>(data));

        ++(data->invokeCount);
        data->invokeStartTime = ZFTime::currentTimeValue();
    }
    else
    {
        ++(data->invokeCount);

        if(data->invokeStartTime != ZFTimeValueZero)
        {
            ++(data->reentrantCount);
        }
        else
        {
            data->invokeStartTime = ZFTime::currentTimeValue();
        }
    }
}
void invokeTimeAccurateLogEnd(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeAccurateDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *>(key);
    if(data != zfnull)
    {
        if(data->reentrantCount > 0)
        {
            --(data->reentrantCount);
        }
        else
        {
            data->invokeTotalTime += ZFTime::currentTimeValue() - data->invokeStartTime;
            data->invokeStartTime = ZFTimeValueZero;
        }
    }
}
void invokeTimeAccurateRemove(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeAccurateDataMap();
    m.remove(key);
}
void invokeTimeAccurateRemoveAll(void)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeAccurateDataMap();
    m.removeAll();
}
zfindex invokeTimeAccurateGetInvokeCount(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeAccurateDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *>(key);
    if(data != zfnull)
    {
        return data->invokeCount;
    }
    return 0;
}
ZFTimeValue invokeTimeAccurateGetAverageTime(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeAccurateDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *>(key);
    if(data != zfnull && data->invokeCount > 0)
    {
        return data->invokeTotalTime / data->invokeCount;
    }
    return ZFTimeValueZero;
}
ZFTimeValue invokeTimeAccurateGetTotalTime(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeAccurateDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *>(key);
    if(data != zfnull)
    {
        return data->invokeTotalTime;
    }
    return ZFTimeValueZero;
}
void invokeTimeAccurateGetSummary(ZF_OUT zfstring &ret, ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeAccurateDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeDataHolder<ZFTimeValue> *>(key);
    zfindex invokeCount = ((data == zfnull) ? 0 : data->invokeCount);
    ZFTimeValue invokeTotalTime = ((data == zfnull) ? ZFTimeValueZero : data->invokeTotalTime);
    ZFTimeValue invokeAverageTime = ((invokeCount == 0) ? ZFTimeValueZero : invokeTotalTime / invokeCount);
    zfstringAppend(ret, zfText("[%s] invoke count: %s, total: %s, average: %s"),
        (key == zfnull) ? ZFTOKEN_zfnull : key,
        zfsFromInt(invokeCount).cString(),
        ZFTimeValueToStringFriendly(invokeTotalTime).cString(),
        ZFTimeValueToStringFriendly(invokeAverageTime).cString());
}

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END

