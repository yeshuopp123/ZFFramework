/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUtility_test.h"

#include "ZFAlgorithm.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUtility_ZFLang_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUtility_ZFLang_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutput(zfText("load default language"));
        zfLangLoad(ZFXmlParse(ZFInputCallbackForResFile(zfText("test_ZFLang_default.xml"))));
        this->test();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("load custom language"));
        zfLangLoad(ZFXmlParse(ZFInputCallbackForResFile(zfText("test_ZFLang_custom.xml"))));
        this->test();

        this->testCaseStop();
    }
private:
    void test(void)
    {
        this->testCaseOutput(zfLang(zfText("hello")));
        this->testCaseOutput(zfLang(zfText("hello with no localized string")));
        this->testCaseOutput(zfLang(zfText("hello in default only")));
        this->testCaseOutput(zfLang(zfText("hello in custom only")));
        this->testCaseOutput(zfstringWithFormat(zfLang(zfText("hello with format: %s")).cString(), zfText("i'm passed string")));
    }
};
ZFOBJECT_REGISTER(ZFUtility_ZFLang_test)

ZF_NAMESPACE_GLOBAL_END

