/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIWebKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWebKit_ZFUIWebView_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWebKit_ZFUIWebView_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIWebView, testView);
        container->childAdd(testView);
        testView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        testView->layoutParam()->layoutMarginSet(ZFUIMarginMake(40));
        testView->viewBackgroundColorSet(ZFUIColorRed);

        this->prepareSettingButton(window, testView);

        testView->webLoadUrl(zfText("http://www.baidu.com"));
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIWebView *testView)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWebKit_ZFUIWebView_test)

ZF_NAMESPACE_GLOBAL_END

