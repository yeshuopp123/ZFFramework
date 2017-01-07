/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static zfindex _ZFP_ZFCore_ZFTime_test_leapYearNumByEnum(zfint year1, zfint year2)
{
    zfint t1 = year1;
    zfint t2 = year2;
    if(year1 > year2)
    {
        t1 = year2;
        t2 = year1;
    }
    zfuint ret = 0;
    for( ; t1 <= t2; ++t1)
    {
        if((t1 % 100 != 0 && t1 % 4 == 0)
           || t1 % 400 == 0)
        {
            ++ret;
        }
    }
    return ret;
}
static zfstringA _ZFP_ZFCore_ZFTime_test_printZFTimeValue(const ZFTimeValue &tv)
{
    zfstringA s;
    zfstringAppend(s, zfTextA("\t%s\t%s"),
        zfsFromInt<zfstringA>(tv.sec).cString(),
        zfsFromInt<zfstringA>(tv.usec).cString());
    return s;
}

static zfstring _ZFP_ZFCore_ZFTime_test_printZFTime(ZFTime *t)
{
    zfstring ret;
    zfstringAppend(ret, zfText("  %s"), _ZFP_ZFCore_ZFTime_test_printZFTimeValue(t->timeValue()).cString());
    zfstringAppend(ret, zfText("  %6d-%02d-%02d %02d:%02d:%02d.%03d %03d, dayOfWeek: %d, dayOfYear: %d"),
        t->timeInfoYear(),
        t->timeInfoMonth() + 1,
        t->timeInfoDay() + 1,
        t->timeInfoHour(),
        t->timeInfoMinute(),
        t->timeInfoSecond(),
        t->timeInfoMiliSecond(),
        t->timeInfoMicroSecond(),
        t->timeInfoDayOfWeek(),
        t->timeInfoDayOfYear());
    return ret;
}

zfclass ZFCore_ZFTime_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFTime_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFTime"));

        srand((zfuint)ZFTime::currentTimeValue().sec);
        this->testCaseOutput(zfText("calc leap year between two years, if nothing wrong, none would be printed:"));
        for(zfint i = 0; i < 200; ++i)
        {
            zfint y1 = rand() % 30000;
            zfint y2 = rand() % 30000;
            zfint n1 = (zfint)_ZFP_ZFCore_ZFTime_test_leapYearNumByEnum(y1, y2);
            zfint n2 = (zfint)ZFTime::leapYearBetween(y1, y2);
            if(n1 != n2 && n1 != -n2)
            {
                this->testCaseOutput(zfText("calc LeapYear num error at loop: %d (y1: %6d, y2: %6d) n1: %6d, n2: %6d"),
                    i,
                    y1,
                    y2,
                    n1,
                    n2);
            }
        }
        this->testCaseOutputSeparator();

        zfblockedAlloc(ZFTime, t, ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current time:"));
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t));

        zfblockedAlloc(ZFTime, t2, ZFTimeValueZero);
        this->testCaseOutput(zfText("zero time:"));
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t2));

        zfblockedAlloc(ZFTime, t3, ZFTimeInfoMake(
            t->timeInfoYear(),
            t->timeInfoMonth(),
            t->timeInfoDay(),
            t->timeInfoHour(),
            t->timeInfoMinute(),
            t->timeInfoSecond(),
            t->timeInfoMiliSecond(),
            t->timeInfoMicroSecond()));
        this->testCaseOutput(zfText("create a ZFTime:"));
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t3));

        zfblockedAlloc(ZFTime, t4, ZFTimeInfoMake(
            t->timeInfoYear() - 300,
            t->timeInfoMonth(),
            t->timeInfoDay(),
            t->timeInfoHour(),
            t->timeInfoMinute(),
            t->timeInfoSecond(),
            t->timeInfoMiliSecond(),
            t->timeInfoMicroSecond()));
        this->testCaseOutput(zfText("time before 1970:"));
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t4));

        zfblockedAlloc(ZFTime, t5, ZFTimeInfoMake(
            t->timeInfoYear() - 3000,
            t->timeInfoMonth(),
            t->timeInfoDay(),
            t->timeInfoHour(),
            t->timeInfoMinute(),
            t->timeInfoSecond(),
            t->timeInfoMiliSecond(),
            t->timeInfoMicroSecond()));
        this->testCaseOutput(zfText("time before 0000:"));
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t5));

        zfblockedAlloc(ZFTime, t6, ZFTimeInfoMake(
            t->timeInfoYear() + 3000,
            t->timeInfoMonth(),
            t->timeInfoDay(),
            t->timeInfoHour(),
            t->timeInfoMinute(),
            t->timeInfoSecond(),
            t->timeInfoMiliSecond(),
            t->timeInfoMicroSecond()));
        this->testCaseOutput(zfText("time after 3000:"));
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t6));

        zfblockedAlloc(ZFTime, t7, t->timeValue());
        this->testCaseOutput(zfText("copy a ZFTime:"));
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t7));

        // ============================================================
        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFTime performance test"));
        zfblockedAlloc(ZFTime, tCur1);
        zfblockedAlloc(ZFTime, tCur2);

        this->testCaseOutput(zfText("\naccess none"));
        tCur1->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s"), _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        tCur2->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s, used: %s"),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput(zfText("\naccess Year"));
        tCur1->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s"), _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoYear();
        tCur2->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s, used: %s"),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput(zfText("\naccess Month"));
        tCur1->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s"), _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoMonth();
        tCur2->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s, used: %s"),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput(zfText("\naccess Day"));
        tCur1->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s"), _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoDay();
        tCur2->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s, used: %s"),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput(zfText("\naccess Hour"));
        tCur1->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s"), _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoHour();
        tCur2->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s, used: %s"),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput(zfText("\naccess Minute"));
        tCur1->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s"), _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoMinute();
        tCur2->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s, used: %s"),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput(zfText("\naccess Second"));
        tCur1->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s"), _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoSecond();
        tCur2->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s, used: %s"),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput(zfText("\naccess MiliSecond"));
        tCur1->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s"), _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoMiliSecond();
        tCur2->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s, used: %s"),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput(zfText("\naccess MicroSecond"));
        tCur1->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s"), _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoMicroSecond();
        tCur2->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s, used: %s"),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput(zfText("\naccess DayOfWeek"));
        tCur1->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s"), _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoDayOfWeek();
        tCur2->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s, used: %s"),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput(zfText("\naccess DayOfYear"));
        tCur1->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s"), _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoDayOfYear();
        tCur2->timeValueSet(ZFTime::currentTimeValue());
        this->testCaseOutput(zfText("current timeValue: %s, used: %s"),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFTime_test)

ZF_NAMESPACE_GLOBAL_END

