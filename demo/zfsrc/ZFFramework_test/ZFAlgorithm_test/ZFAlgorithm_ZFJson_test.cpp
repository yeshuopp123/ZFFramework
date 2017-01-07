/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFJson_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFJson_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFJson"));

        const zfchar *src = zfText(
            "{"
            "    \"k0\" : \"v0\","
            ""
            "    \"object0\" : {},"
            "    \"object1\" : {"
            "        \"k0\" : \"v0\","
            "        \"k1\" : \"v1\""
            "    },"
            "    \"object2\" : {"
            "        \"k0\" : {\"k0\" : \"v0\"}"
            "    },"
            ""
            "    \"array0\" : [],"
            "    \"array1\" : ["
            "        {\"k0\" : \"v0\"},"
            "        {\"k0\" : \"v0\"}"
            "    ],"
            ""
            "    \"k1\" : \"special chars: \\\\ \\\"\""
            "}"
            );

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFJson parse from string:"));
        ZFJsonItem json = ZFJsonFromString(src);

        this->testCaseOutput(zfText("ZFJson parse from string, result:"));
        zfLogTrimT() << ZFJsonToString(json);

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFJson add element"));
        json.jsonItemValueSet(zfText("testJsonKey"), zfText("testJsonValue"));
        this->testCaseOutput(zfText("ZFJson add element, result:"));
        zfLogTrimT() << ZFJsonToString(json);

        this->performanceTest(json);

        this->testCaseStop();
    }
private:
    void performanceTest(ZF_IN const ZFJsonItem &jsonItem)
    {
        zfindex toDataTimes = 10000;
        ZFCoreStatistic::invokeTimeAccurateLogBegin(zfText("ZFJsonPerformance_test_toData"));
        for(zfindex i = 0; i < toDataTimes; ++i)
        {
            zfstring tmp;
            ZFJsonToOutput(ZFOutputCallbackForString(tmp), jsonItem);
        }
        ZFCoreStatistic::invokeTimeAccurateLogEnd(zfText("ZFJsonPerformance_test_toData"));

        zfindex fromDataTimes = toDataTimes;
        zfstring jsonString;
        ZFJsonToOutput(ZFOutputCallbackForString(jsonString), jsonItem);
        ZFCoreStatistic::invokeTimeAccurateLogBegin(zfText("ZFJsonPerformance_test_fromData"));
        for(zfindex i = 0; i < fromDataTimes; ++i)
        {
            ZFJsonFromInput(ZFInputCallbackForBuffer(jsonString.cString()));
        }
        ZFCoreStatistic::invokeTimeAccurateLogEnd(zfText("ZFJsonPerformance_test_fromData"));
        ZFJsonItem jsonItemNew = ZFJsonFromInput(ZFInputCallbackForBuffer(jsonString.cString()));

        this->testCaseOutputSeparator();

        ZFTimeValue toDataUsedTime = ZFCoreStatistic::invokeTimeAccurateGetTotalTime(zfText("ZFJsonPerformance_test_toData"));
        this->testCaseOutput(zfText("serialize to data %zi times cost %s seconds"),
            toDataTimes,
            ZFTimeValueToStringFriendly(toDataUsedTime).cString());

        ZFTimeValue fromDataUsedTime = ZFCoreStatistic::invokeTimeAccurateGetTotalTime(zfText("ZFJsonPerformance_test_fromData"));
        this->testCaseOutput(zfText("serialize from data %zi times cost %s seconds"),
            fromDataTimes,
            ZFTimeValueToStringFriendly(fromDataUsedTime).cString());

        #if 0
            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("content: %s"), jsonString.cString());
            this->testCaseOutput(zfText("newly: %s"), ZFJsonToString(jsonItemNew).cString());
        #endif

        ZFCoreStatistic::invokeTimeAccurateRemove(zfText("ZFJsonPerformance_test_toData"));
        ZFCoreStatistic::invokeTimeAccurateRemove(zfText("ZFJsonPerformance_test_fromData"));
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFJson_test)

ZF_NAMESPACE_GLOBAL_END

