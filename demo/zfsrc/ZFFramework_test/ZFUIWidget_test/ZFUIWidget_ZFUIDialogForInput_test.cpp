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

zfclass ZFUIWidget_ZFUIDialogForInput_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIDialogForInput_test, ZFFramework_test_TestCase)

    ZFPROPERTY_RETAIN_READONLY(ZFUIDialogForInput *, dialog, ZFPropertyInitValue(zflineAlloc(ZFUIDialogForInput)))

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        this->dialog()->dialogApplyAutoHide(this->dialog()->dialogButton_Cancel());
        this->dialog()->dialogButtonTextSet_Cancel(zfText("cancel"));
        this->dialog()->dialogApplyAutoHide(this->dialog()->dialogButton_Yes());
        this->dialog()->dialogButtonTextSet_Yes(zfText("confirm"));
        ZFLISTENER_LOCAL(yesOnClick, {
            ZFUIDialogForInput *dialog = userData->to<ZFObjectHolder *>()->holdedObj;
            zfLogT() << zfText("onConfirm") << listenerData.sender
                << zfText("text:") << dialog->inputText();
        })
        this->dialog()->dialogButton_Yes()->observerAdd(ZFUIButton::EventButtonOnClick(), yesOnClick, this->dialog()->objectHolder());
        this->dialog()->dialogTitleTextSet(zfText("i'm title"));
        this->dialog()->dialogContentTextSet(zfText("i'm hint"));
        this->dialog()->inputHintTextSet(zfText("input something here"));

        zfblockedAlloc(ZFUIKit_test_Button, showButton);
        container->childAdd(showButton);
        showButton->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        ZFLISTENER_LOCAL(showButtonOnClick, {
            userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIDialogForInput *>()->dialogShow();
        })
        showButton->observerAdd(ZFUIButton::EventButtonOnClick(), showButtonOnClick, this->dialog()->objectHolder());

        this->prepareSettingButton(window, this->dialog());
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIDialogForInput *dialog)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIDialogForInput_test)

ZF_NAMESPACE_GLOBAL_END

