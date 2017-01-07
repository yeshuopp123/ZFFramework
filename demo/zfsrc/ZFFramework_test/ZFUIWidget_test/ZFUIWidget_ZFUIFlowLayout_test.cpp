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

zfclass ZFUIWidget_ZFUIFlowLayout_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIFlowLayout_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIFlowLayout, layout);
        container->childAdd(layout);
        layout->layoutParam()->layoutMarginSet(ZFUIMarginMake(40, 40 + ZFUIGlobalStyle::DefaultStyle()->itemSizeButton(), 40, 40));
        layout->viewBackgroundColorSet(ZFUIColorRed);

        this->prepareAddRemoveButton(container, layout);

        this->prepareSettingButton(window, layout);
    }

private:
    void prepareAddRemoveButton(ZFUIView *container, ZFUIView *layout)
    {
        zfblockedAlloc(ZFUIKit_test_Button, addButton);
        container->childAdd(addButton);
        ZFLISTENER_LOCAL(addButtonOnClick, {
            ZFUIView *layout = userData->to<ZFObjectHolder *>()->holdedObj;
            zfblockedAlloc(ZFUITextView, view);
            layout->childAdd(view);
            view->viewBackgroundColorSet(ZFUIColorRandom());
            zfindex textLength = zfmRand(1, 10);
            #if 1
                textLength = layout->childCount() + 1;
            #endif
            zfstring text;
            text.capacitySet(textLength);
            zfchar c = '0' + ((layout->childCount() - 1) % 10);
            for(zfindex i = 0; i < textLength; ++i)
            {
                text += c;
            }
            view->textContentStringSet(text);
        })
        addButton->observerAdd(ZFUIButton::EventButtonOnClick(), addButtonOnClick, layout->objectHolder());
        addButton->buttonLabelTextStringSet(zfText("add"));

        zfblockedAlloc(ZFUIKit_test_Button, removeButton);
        container->childAdd(removeButton);
        removeButton->layoutParam()->layoutMarginSet(ZFUIMarginMake(70, 0, 0, 0));
        ZFLISTENER_LOCAL(removeButtonOnClick, {
            ZFUIView *layout = userData->to<ZFObjectHolder *>()->holdedObj;
            if(layout->childCount() > 0)
            {
                layout->childRemoveAtIndex(layout->childCount() - 1);
            }
        })
        removeButton->observerAdd(ZFUIButton::EventButtonOnClick(), removeButtonOnClick, layout->objectHolder());
        removeButton->buttonLabelTextStringSet(zfText("remove"));
    }
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIFlowLayout *layout)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingForLayoutRequest(settings, layout);

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIOrientationEnum, ZFPropertyAccess(ZFUIFlowLayout, layoutOrientationMain),
            ZFCoreArrayPODCreate(ZFUIOrientationEnum
                , ZFUIOrientation::e_Right
                , ZFUIOrientation::e_Bottom
                , ZFUIOrientation::e_Left
                , ZFUIOrientation::e_Top
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIOrientationEnum, ZFPropertyAccess(ZFUIFlowLayout, layoutOrientationSecondary),
            ZFCoreArrayPODCreate(ZFUIOrientationEnum
                , ZFUIOrientation::e_Right
                , ZFUIOrientation::e_Bottom
                , ZFUIOrientation::e_Left
                , ZFUIOrientation::e_Top
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIMargin, ZFPropertyAccess(ZFUIFlowLayout, layoutChildMargin),
            ZFCoreArrayPODCreate(ZFUIMargin
                , ZFUIMarginZero
                , ZFUIMarginMake(8)
                , ZFUIMarginMake(12)
                , ZFUIMarginMake(24)
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, zfint, ZFPropertyAccess(ZFUIFlowLayout, layoutChildSpaceX),
            ZFCoreArrayPODCreate(zfint
                , 0
                , 8
                , 12
                , 24
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, zfint, ZFPropertyAccess(ZFUIFlowLayout, layoutChildSpaceY),
            ZFCoreArrayPODCreate(zfint
                , 0
                , 8
                , 12
                , 24
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout->layoutParam(), ZFUISizeParam, ZFPropertyAccess(ZFUIViewLayoutParam, sizeParam),
            ZFCoreArrayPODCreate(ZFUISizeParam
                , ZFUISizeParamWrapWidthWrapHeight
                , ZFUISizeParamFillWidthWrapHeight
                , ZFUISizeParamWrapWidthFillHeight
                , ZFUISizeParamFillWidthFillHeight
                ));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIFlowLayout_test)

ZF_NAMESPACE_GLOBAL_END

