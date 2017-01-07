/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIHint_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIHint_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFFramework_test_protocolCheck(ZFUISysWindow)

        zfautoObject hint = ZFUIHintShow(zfText("this is a normal hint"));
#if 0
        ZFLISTENER_LOCAL(hintOnHide, {
            zfLogT();
            ZFTestCase *testCase = userData->to<ZFObjectHolder *>()->holdedObj;
            testCase->testCaseStop();
        })
#else
        ZFLISTENER_LOCAL(hintOnHide, {
            for(zfindex i = 0; i < 3; ++i)
            {
                ZFUIHintShow(zfstringWithFormat(zfText("this is a stacked hint %zi"), i));
            }
            zfautoObject last = ZFUIHintShow(zfText("this is a very")
                zfText(" long long long long long long long long long long")
                zfText(" long long long long long long long long long long")
                zfText(" long long long long long long long long long hint"));
            ZFLISTENER_LOCAL(lastHintOnHide, {
                ZFTestCase *testCase = userData->to<ZFObjectHolder *>()->holdedObj;
                testCase->testCaseStop();
            })
            last.toObject()->observerAdd(ZFUIHint::EventHintOnHide(), lastHintOnHide, userData);
        })
#endif
        hint.toObject()->observerAdd(ZFUIHint::EventHintOnHide(), hintOnHide, this->objectHolder());
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIHint_test)

ZF_NAMESPACE_GLOBAL_END

