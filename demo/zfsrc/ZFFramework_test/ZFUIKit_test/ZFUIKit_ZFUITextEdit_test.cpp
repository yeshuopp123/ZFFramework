/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUITextEdit_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUITextEdit_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        ZFLISTENER_LOCAL(onClickReturn, {
            ZFUITextEdit *textEdit = listenerData.sender->toAny();
            textEdit->viewFocusRequest(zffalse);
        })

        zfblockedAlloc(ZFUITextEdit, view);
        container->childAdd(view);
        view->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthWrapHeight);
        view->layoutParam()->layoutMarginSet(ZFUIMarginMake(40));
        view->viewBackgroundColorSet(ZFUIColorRed);
        view->observerAdd(ZFUITextEdit::EventTextEditReturnOnClick(), onClickReturn);
        view->textPlaceHolder()->textContentStringSet(zfText("first text edit"));

        zfblockedAlloc(ZFUITextEdit, view2);
        container->childAdd(view2);
        view2->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        view2->layoutParam()->layoutMarginSet(ZFUIMarginMake(40, 100, 40, 40));
        view2->viewBackgroundColorSet(ZFUIColorBlue);
        view2->observerAdd(ZFUITextEdit::EventTextEditReturnOnClick(), onClickReturn);
        view2->textPlaceHolder()->textContentStringSet(zfText("second text edit"));

        this->prepareSettingButton(window, view);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUITextEdit *view)
    {
        zfblockedAlloc(ZFArrayEditable, settings);
        ZFUIKit_test_prepareSettingForBoolProperty(settings, view, ZFPropertyAccess(ZFUITextEdit, textEditEnable));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUITextEdit_test)

ZF_NAMESPACE_GLOBAL_END

