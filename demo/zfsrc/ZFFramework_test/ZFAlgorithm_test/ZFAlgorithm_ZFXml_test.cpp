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

zfclass ZFAlgorithm_ZFXml_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFXml_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFXml"));

        const zfchar *src = zfText(
            "<?xml type=\"declaration\"?>"
            "<!DOCTYPE TYPE \"DOCTYPE\">"
            "<?pi ProcessingInstruction?>"
            "<element0 attribute0=\"attribute value0\">"
            "<element1 attribute0=\"attribute value0\" />"
            "<element2 attribute0=\"attribute value0\" attribute1=\"attribute value1\""
            " attribute2=\"attribute value2\" attribute3=\"attribute value3\" />"
            "<element3>text0</element3>"
            "<element4><![CDATA[cdata text]]></element4>"
            "<element5>text with escaped chars: &lt; &gt; &amp; &apos; &quot;</element5>"
            "</element0>"
            );

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFXml parse from string:"));
        ZFXmlItem doc = ZFXmlFromString(src);

        this->testCaseOutput(zfText("ZFXml parse from string, result:"));
        zfLogTrimT() << ZFXmlToString(doc);

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFXml add element"));
        ZFXmlItem element(ZFXmlType::e_XmlElement);
        element.xmlNameSet(zfText("testElement"));
        doc.xmlChildAdd(element);
        this->testCaseOutput(zfText("ZFXml add element, result:"));
        zfLogTrimT() << ZFXmlToString(doc);

        this->performanceTest(doc);

        this->testCaseStop();
    }
private:
    void performanceTest(ZF_IN const ZFXmlItem &xmlItem)
    {
        zfindex toDataTimes = 10000;
        ZFCoreStatistic::invokeTimeAccurateLogBegin(zfText("ZFXmlPerformance_test_toData"));
        for(zfindex i = 0; i < toDataTimes; ++i)
        {
            zfstring tmp;
            ZFXmlToOutput(ZFOutputCallbackForString(tmp), xmlItem);
        }
        ZFCoreStatistic::invokeTimeAccurateLogEnd(zfText("ZFXmlPerformance_test_toData"));

        zfindex fromDataTimes = toDataTimes;
        zfstring xmlString;
        ZFXmlToOutput(ZFOutputCallbackForString(xmlString), xmlItem);
        ZFCoreStatistic::invokeTimeAccurateLogBegin(zfText("ZFXmlPerformance_test_fromData"));
        for(zfindex i = 0; i < fromDataTimes; ++i)
        {
            ZFXmlFromInput(ZFInputCallbackForBuffer(xmlString.cString()));
        }
        ZFCoreStatistic::invokeTimeAccurateLogEnd(zfText("ZFXmlPerformance_test_fromData"));
        ZFXmlItem xmlItemNew = ZFXmlFromInput(ZFInputCallbackForBuffer(xmlString.cString()));

        this->testCaseOutputSeparator();

        ZFTimeValue toDataUsedTime = ZFCoreStatistic::invokeTimeAccurateGetTotalTime(zfText("ZFXmlPerformance_test_toData"));
        this->testCaseOutput(zfText("serialize to data %zi times cost %s seconds"),
            toDataTimes,
            ZFTimeValueToStringFriendly(toDataUsedTime).cString());

        ZFTimeValue fromDataUsedTime = ZFCoreStatistic::invokeTimeAccurateGetTotalTime(zfText("ZFXmlPerformance_test_fromData"));
        this->testCaseOutput(zfText("serialize from data %zi times cost %s seconds"),
            fromDataTimes,
            ZFTimeValueToStringFriendly(fromDataUsedTime).cString());

        #if 0
            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("content: %s"), xmlString.cString());
            this->testCaseOutput(zfText("newly: %s"), ZFXmlToString(xmlItemNew).cString());
        #endif

        ZFCoreStatistic::invokeTimeAccurateRemove(zfText("ZFXmlPerformance_test_toData"));
        ZFCoreStatistic::invokeTimeAccurateRemove(zfText("ZFXmlPerformance_test_fromData"));
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFXml_test)

ZF_NAMESPACE_GLOBAL_END

