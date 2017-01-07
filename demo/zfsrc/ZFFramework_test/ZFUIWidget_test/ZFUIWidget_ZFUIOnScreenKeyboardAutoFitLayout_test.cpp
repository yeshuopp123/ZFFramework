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

zfclass ZFUIWidget_ZFUIOnScreenKeyboardAutoFitLayout_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIOnScreenKeyboardAutoFitLayout_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        window->viewDelegateClassSet(ZFUIOnScreenKeyboardAutoFitLayout::ClassData()->className());
        ZFUIOnScreenKeyboardAutoFitLayout *layout = window->viewDelegate<ZFUIOnScreenKeyboardAutoFitLayout *>();
        this->prepareSettingButton(window, layout);

        container->viewBackgroundColorSet(ZFUIColorGreen);
        for(zfindex i = 0; i < 3; ++i)
        {
            zfblockedAlloc(ZFUITextEdit, view);
            container->childAdd(view);
            view->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthWrapHeight);
            view->viewBackgroundColorSet(ZFUIColorRandom());
            view->layoutParam()->layoutMarginSet(ZFUIMarginMake(10));
        }
        container->childAtIndex(0)->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner);
        container->childAtIndex(1)->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
        container->childAtIndex(2)->layoutParam()->layoutAlignSet(ZFUIAlign::e_BottomInner);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIOnScreenKeyboardAutoFitLayout *layout)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingForBoolProperty(settings, layout, ZFPropertyAccess(ZFUIOnScreenKeyboardAutoFitLayout, autoFitEnable));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, layout, ZFPropertyAccess(ZFUIOnScreenKeyboardAutoFitLayout, autoFitFocusedViewToVisible));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIOnScreenKeyboardAutoFitLayout_test)

ZF_NAMESPACE_GLOBAL_END

