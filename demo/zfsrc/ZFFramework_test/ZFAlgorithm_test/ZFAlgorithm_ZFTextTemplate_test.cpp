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

zfclass ZFAlgorithm_ZFTextTemplate_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFTextTemplate_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFTextTemplate"));

        const zfchar *src = zfText(
            "========== test replace begin ==========\n"
            "<exist>{ZFTT_R_replace_exist}<exist>\n"
            "<not_exist>{ZFTT_R_replace_not_exist}<not_exist>\n"
            "---------- test replace  end  ----------\n"
            "\n"
            "========== test enableif begin ==========\n"
            "<enableif_true>{ZFTT_C_enableif_true}enableif_true{ZFTT_CE}<enableif_true>\n"
            "<enableif_false>{ZFTT_C_enableif_false}enableif_false{ZFTT_CE}<enableif_false>\n"
            "\n"
            "<enableif_embeded_0>{ZFTT_C_enableif_true}enableif{ZFTT_C_enableif_false}enableif_false{ZFTT_CE}_true{ZFTT_CE}<enableif_embeded_0>\n"
            "<enableif_embeded_0>{ZFTT_C_enableif_true}{ZFTT_R_replace_exist}{ZFTT_CE}<enableif_embeded_0>\n"
            "<enableif_embeded_0>{ZFTT_C_enableif_true}{ZFTT_R_replace_not_exist}{ZFTT_CE}<enableif_embeded_0>\n"
            "\n"
            "<enableif_embeded_1>{ZFTT_C_enableif_false}enableif{ZFTT_C_enableif_true}enableif_true{ZFTT_CE}_false{ZFTT_CE}<enableif_embeded_1>\n"
            "<enableif_embeded_1>{ZFTT_C_enableif_false}{ZFTT_R_replace_exist}{ZFTT_CE}<enableif_embeded_1>\n"
            "<enableif_embeded_1>{ZFTT_C_enableif_false}{ZFTT_R_replace_not_exist}{ZFTT_CE}<enableif_embeded_1>\n"
            "---------- test enableif  end  ----------\n"
            "\n"
            "========== test index begin ==========\n"
            "this is {ZFTT_I_test_index} item\n"
            "this is {ZFTT_I_test_index} item\n"
            "this is {ZFTT_I_test_index} item\n"
            "reset{ZFTT_IR_test_index}\n"
            "this is {ZFTT_I_test_index} item\n"
            "this is {ZFTT_I_test_index} item\n"
            "this is {ZFTT_I_test_index} item\n"
            "---------- test index  end  ----------\n"
            );

        zfstring buf;
        ZFTextTemplateParam param;
        param.replaceDataAdd(zfText("replace_exist"), zfText("_replace_exist_"));
        param.enableDataAdd(zfText("enableif_true"), zftrue);
        ZFTextTemplateApply(param, ZFOutputCallbackForString(buf), src);

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("src:\n%s"), src);

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("applied:\n%s"), buf.cString());

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFTextTemplate_test)

ZF_NAMESPACE_GLOBAL_END

