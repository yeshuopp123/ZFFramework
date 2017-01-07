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

zfclass ZFUIWidget_ZFUILinearLayout_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUILinearLayout_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUILinearLayout, layout);
        container->childAdd(layout);
        layout->layoutParam()->layoutMarginSet(ZFUIMarginMake(40));
        layout->viewBackgroundColorSet(ZFUIColorRed);

        this->prepareChildren(layout);

        this->prepareSettingButton(window, layout);
    }

private:
    void prepareChildren(ZF_IN ZFUILinearLayout *layout)
    {
        for(zfindex i = 0; i < 3; ++i)
        {
            zfblockedAlloc(ZFUITextView, child);
            layout->childAdd(child);
            child->textContentStringSet(zfstringWithFormat(zfText("text %zi"), i));
            child->textAlignSet(ZFUIAlign::e_Center);
            child->viewBackgroundColorSet(ZFUIColorRandom(255));
        }

        layout->childAtIndex(1)->layoutParam<ZFUILinearLayoutParam *>()->layoutWeightSet(1);
        layout->childAtIndex(2)->layoutParam<ZFUILinearLayoutParam *>()->layoutWeightSet(2);

        zfblockedAlloc(ZFUILinearLayout, l);
        layout->childAdd(l);
        l->layoutOrientationSet(ZFUIOrientation::e_Bottom);
        for(zfindex i = 0; i < 3; ++i)
        {
            zfblockedAlloc(ZFUITextView, child);
            l->childAdd(child);
            child->textContentStringSet(zfstringWithFormat(zfText("text %zi"), 3 + i));
            child->textAlignSet(ZFUIAlign::e_Center);
            child->viewBackgroundColorSet(ZFUIColorRandom(255));
        }
    }
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUILinearLayout *layout)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingForLayoutRequest(settings, layout);

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIOrientationEnum, ZFPropertyAccess(ZFUILinearLayout, layoutOrientation),
            ZFCoreArrayPODCreate(ZFUIOrientationEnum
                , ZFUIOrientation::e_Right
                , ZFUIOrientation::e_Bottom
                , ZFUIOrientation::e_Left
                , ZFUIOrientation::e_Top
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIMargin, ZFPropertyAccess(ZFUILinearLayout, layoutChildMargin),
            ZFCoreArrayPODCreate(ZFUIMargin
                , ZFUIMarginZero
                , ZFUIMarginMake(8)
                , ZFUIMarginMake(12)
                , ZFUIMarginMake(24)
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, zfint, ZFPropertyAccess(ZFUILinearLayout, layoutChildSpace),
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
ZFOBJECT_REGISTER(ZFUIWidget_ZFUILinearLayout_test)

ZF_NAMESPACE_GLOBAL_END

