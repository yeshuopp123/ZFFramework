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

zfclass ZFUIWidget_ZFUIDialogForChoice_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIDialogForChoice_test, ZFFramework_test_TestCase)

    ZFPROPERTY_RETAIN_READONLY(ZFUIDialogForChoice *, dialog, ZFPropertyInitValue(zflineAlloc(ZFUIDialogForChoice)))

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
        ZFLISTENER_LOCAL(choiceOnConfirm, {
            ZFUIDialogForChoice *dialog = userData->to<ZFObjectHolder *>()->holdedObj;
            zfLogT() << zfText("onConfirm") << dialog->choiceSelectedNameList();
        })
        ZFLISTENER_LOCAL(choiceOnChange, {
            ZFUIDialogForChoice *dialog = userData->to<ZFObjectHolder *>()->holdedObj;
            zfLogT() << zfText("onChange") << dialog->choiceSelectedNameList();
        })
        this->dialog()->observerAdd(ZFUIDialogForChoice::EventChoiceOnConfirm(), choiceOnConfirm, this->dialog()->objectHolder());
        this->dialog()->observerAdd(ZFUIDialogForChoice::EventChoiceOnChange(), choiceOnChange, this->dialog()->objectHolder());
        this->dialog()->dialogTitleTextSet(zfText("i'm title"));

        this->dialog()->choiceAdd(zfText("id 0"), zfText("choice 0"));
        this->dialog()->choiceAdd(zfText("id 1"), zfText("choice 1"));
        this->dialog()->choiceAdd(zfText("id 2"), zfText("choice 2"));

        zfblockedAlloc(ZFUIKit_test_Button, showButton);
        container->childAdd(showButton);
        showButton->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        ZFLISTENER_LOCAL(showButtonOnClick, {
            userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIDialogForChoice *>()->dialogShow();
        })
        showButton->observerAdd(ZFUIButton::EventButtonOnClick(), showButtonOnClick, this->dialog()->objectHolder());

        this->prepareSettingButton(window, this->dialog());
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIDialogForChoice *dialog)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIDialogForChoice_test)

ZF_NAMESPACE_GLOBAL_END

