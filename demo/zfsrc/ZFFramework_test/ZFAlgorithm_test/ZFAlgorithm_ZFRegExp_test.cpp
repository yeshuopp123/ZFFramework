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

zfclass ZFAlgorithm_ZFRegExp_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFRegExp_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfblockedAlloc(ZFRegExp, regexp);
        const zfchar *patternFrom = zfText("(ab*c)(?<n0>ab*c)");
        const zfchar *patternTo = zfText("{$1}");
        const zfchar *stringFrom = zfText("zacabcabbcabbbcz");

        regexp->regExpCompile(patternFrom);

        {
            ZFRegExpResult result;
            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("find:"));
            regexp->regExpMatch(result, stringFrom);
            this->testCaseOutput(zfText("  pattern    : %s"), patternFrom);
            this->testCaseOutput(zfText("  string     : %s"), stringFrom);
            this->testCaseOutput(zfText("  result     : %s"), result.objectInfo().cString());
            this->testCaseOutput(zfText("  named group: %zi"), regexp->regExpNamedGroupIndexForName(zfText("n0")));
        }

        {
            ZFRegExpResult result;
            zfstring stringTo;
            zfindex maxCount = zfindexMax;
            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("replace:"));
            regexp->regExpReplace(stringTo, result, stringFrom, patternTo, maxCount);
            this->testCaseOutput(zfText("  pattern from: %s"), patternFrom);
            this->testCaseOutput(zfText("  pattern to  : %s"), patternTo);
            this->testCaseOutput(zfText("  string  from: %s"), stringFrom);
            this->testCaseOutput(zfText("  string  to  : %s"), stringTo.cString());
            this->testCaseOutput(zfText("  match result: %s"), result.objectInfo().cString());
            this->testCaseOutput(zfText("  named group: %zi"), regexp->regExpNamedGroupIndexForName(zfText("n0")));
        }
        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFRegExp_test)

ZF_NAMESPACE_GLOBAL_END

