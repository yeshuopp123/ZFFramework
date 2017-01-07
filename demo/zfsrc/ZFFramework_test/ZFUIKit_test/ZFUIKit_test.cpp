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

ZFOBJECT_REGISTER(ZFUIKit_test_Window)
ZFOBJECT_REGISTER(ZFUIKit_test_Button)
ZFOBJECT_REGISTER(ZFUIKit_test_ListView)

void ZFUIKit_test_prepareTestWindow(ZF_OUT ZFUIWindow *&window,
                                    ZF_OUT ZFUIView *&container,
                                    ZF_IN ZFTestCase *testCaseToStop)
{
    window = zfAlloc(ZFUIKit_test_Window);
    zfblockedRelease(window);
    window->windowShow();

    // close button
    zfblockedAlloc(ZFUIKit_test_Button, closeButton);
    window->childAdd(closeButton);
    closeButton->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner | ZFUIAlign::e_RightInner);
    closeButton->buttonLabelTextStringSet(zfText("close"));
    ZFLISTENER_LOCAL(onClickCloseButton, {
        ZFUIWindow *window = userData->tagGet<ZFObjectHolder *>(zfText("window"))->holdedObj;
        ZFTestCase *testCase = userData->tagGet<ZFObjectHolder *>(zfText("testCaseToStop"))->holdedObj;
        window->windowHide();
        testCase->testCaseStop();
    })
    zfblockedAlloc(ZFObject, closeButtonUserData);
    closeButtonUserData->tagSet(zfText("window"), window->objectHolder());
    closeButtonUserData->tagSet(zfText("testCaseToStop"), testCaseToStop->objectHolder());
    closeButton->observerAdd(ZFUIButton::EventButtonOnClick(), onClickCloseButton, closeButtonUserData);

    // container
    container = zfAlloc(ZFUIView);
    zfblockedRelease(container);
    window->childAdd(container);
    container->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
    container->layoutParam()->layoutMarginSet(ZFUIMarginMake(0, 50, 0, 0));
}

zfautoObject ZFUIKit_test_prepareSettingButton(ZF_IN ZFArray *settings)
{
    zfblockedAlloc(ZFUIKit_test_Button, settingsButton);
    settingsButton->buttonLabelTextStringSet(zfText("settings"));

    zfblockedAlloc(ZFUIKit_test_Window, window);
    settingsButton->tagSet(zfText("setting window"), window);
    window->viewAlphaSet(0.8f);
    ZFLISTENER_LOCAL(onClickSetting, {
        ZFUIWindow *window = userData->to<ZFObjectHolder *>()->holdedObj;
        window->windowShow();
    })
    settingsButton->observerAdd(ZFUIButton::EventButtonOnClick(), onClickSetting, window->objectHolder());

    zfblockedAlloc(ZFUIKit_test_Button, closeButton);
    window->childAdd(closeButton);
    closeButton->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner);
    closeButton->buttonLabelStyle()->textContentSet(zflineAlloc(ZFString, zfText("done")));
    ZFLISTENER_LOCAL(onClickCloseButton, {
        ZFUIWindow *window = userData->to<ZFObjectHolder *>()->holdedObj;
        window->windowHide();
    })
    closeButton->observerAdd(ZFUIButton::EventButtonOnClick(), onClickCloseButton, window->objectHolder());

    zfblockedAlloc(ZFUIKit_test_ListView, listView);
    window->childAdd(listView);
    listView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
    listView->layoutParam()->layoutMarginSet(ZFUIMarginMake(0, 50, 0, 0));
    for(zfindex i = 0; i < settings->count(); ++i)
    {
        ZFUIKit_test_SettingData *setting = settings->get<ZFUIKit_test_SettingData *>(i);
        zfCoreAssert(setting->buttonTextGetter().callbackIsValid());
        zfCoreAssert(setting->buttonClickListener().callbackIsValid());

        zfblockedAlloc(ZFUIKit_test_Button, button);
        listView->childAdd(button);
        ZFLISTENER_LOCAL(onButtonClick, {
            ZFUIKit_test_SettingData *setting = ZFAny(userData);
            ZFUIButtonBasic *button = ZFAny(listenerData.sender);

            setting->buttonClickListener().execute(ZFListenerData(zfidentityInvalid, button), setting->userData());

            zfblockedAlloc(ZFStringEditable, buttonText);
            setting->buttonTextGetter().execute(ZFListenerData(zfidentityInvalid, button, buttonText), setting->userData());
            button->buttonLabelTextStringSet(buttonText->stringValue());
        })
        button->observerAdd(ZFUIButton::EventButtonOnClick(), onButtonClick, setting);

        zfblockedAlloc(ZFStringEditable, buttonText);
        setting->buttonTextGetter().execute(ZFListenerData(zfidentityInvalid, button, buttonText), setting->userData());
        button->buttonLabelTextStringSet(buttonText->stringValue());
    }

    return zfautoObjectCreate(settingsButton);
}

void ZFUIKit_test_prepareSettingButtonWithTestWindow(ZF_IN ZFUIWindow *window,
                                                     ZF_IN ZFArray *settings)
{
    zfautoObject buttonHolder = ZFUIKit_test_prepareSettingButton(settings);
    ZFUIButton *button = buttonHolder.toAny();
    window->childAdd(button);
    button->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner);
}

void ZFUIKit_test_prepareSettingForProperty(ZF_IN_OUT ZFArrayEditable *settings,
                                            ZF_IN ZFObject *obj,
                                            ZF_IN ZFTimeLineProperty *modifier,
                                            ZF_IN zffloat step)
{
    zfCoreAssert(settings != zfnull);
    zfCoreAssert(obj != zfnull && modifier != zfnull && modifier->ownerProperty() != zfnull);
    zfCoreAssert(obj->classData()->classIsTypeOf(modifier->ownerProperty()->ownerClass()));

    zfblockedAlloc(ZFObject, userData);
    userData->tagSet(zfText("obj"), obj->objectHolder());
    userData->tagSet(zfText("modifier"), modifier);
    userData->tagSet(zfText("step"), ZFValue::floatValueCreate(step).toObject());
    userData->tagSet(zfText("progress"), ZFValueEditable::floatValueCreate(0).toObject());

    ZFLISTENER_LOCAL(buttonTextGetter, {
        ZFObject *obj = userData->tagGet<ZFObjectHolder *>(zfText("obj"))->holdedObj;
        ZFTimeLineProperty *modifier = userData->tagGet<ZFTimeLineProperty *>(zfText("modifier"));

        ZFStringEditable *text = listenerData.param0->to<ZFStringEditable *>();
        const ZFProperty *property = modifier->ownerProperty();
        text->stringValueSet(zfstringWithFormat(zfText("%s : %s"), property->propertyName(), ZFPropertyGetInfo(property, obj).cString()));
    })
    ZFLISTENER_LOCAL(buttonClickListener, {
        ZFObject *obj = userData->tagGet<ZFObjectHolder *>(zfText("obj"))->holdedObj;
        ZFTimeLineProperty *modifier = userData->tagGet<ZFTimeLineProperty *>(zfText("modifier"));
        zffloat step = userData->tagGet<ZFValue *>(zfText("step"))->floatValue();
        ZFValueEditable *progress = userData->tagGet<ZFValueEditable *>(zfText("progress"));

        zffloat progressNew = (progress->floatValue() + step);
        while(zffloatIsSmaller(progressNew, 0.0f))
        {
            progressNew += 1.0f;
        }
        while(!zffloatIsSmaller(progressNew, 1.0f))
        {
            progressNew -= 1.0f;
        }
        progress->floatValueSet(progressNew);

        modifier->progressUpdate(obj, progressNew);
    })

    settings->add(zflineAlloc(ZFUIKit_test_SettingData, buttonTextGetter, buttonClickListener, userData));
}

void ZFUIKit_test_prepareSettingForBoolProperty(ZF_IN_OUT ZFArrayEditable *settings,
                                                ZF_IN ZFObject *obj,
                                                ZF_IN const ZFProperty *property)
{
    zfCoreAssert(settings != zfnull);
    zfCoreAssert(obj != zfnull && property != zfnull);
    zfCoreAssert(obj->classData()->classIsTypeOf(property->ownerClass()));
    zfCoreAssert(ZFProperty::propertyTypeIdIsMatch(property->propertyTypeIdName(), ZFPropertyTypeId_zfbool));

    zfbool value = property->getterMethod()->execute<zfbool const &>(obj);
    ZFUIKit_test_prepareSettingForNormalProperty(settings, obj, zfbool, property, ZFCoreArrayPODCreate(zfbool, value, !value));
}

void ZFUIKit_test_prepareSettingForLayoutRequest(ZF_IN_OUT ZFArrayEditable *settings,
                                                 ZF_IN ZFUIView *view)
{
    zfCoreAssert(settings != zfnull);
    zfCoreAssert(view != zfnull);

    ZFLISTENER_LOCAL(buttonTextGetter, {
        ZFStringEditable *text = listenerData.param0->to<ZFStringEditable *>();
        text->stringValueSet(zfText("layoutRequest"));
    })
    ZFLISTENER_LOCAL(buttonClickListener, {
        ZFUIView *view = userData->to<ZFObjectHolder *>()->holdedObj;
        view->layoutRequest();
    })

    settings->add(zflineAlloc(ZFUIKit_test_SettingData, buttonTextGetter, buttonClickListener, view->objectHolder()));
}

ZF_NAMESPACE_GLOBAL_END

