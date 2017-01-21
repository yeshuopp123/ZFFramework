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

zfclass ZFUIKit_ZFAnimationNativeView_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFAnimationNativeView_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIView, view);
        container->childAdd(view);
        view->viewSizePreferedSet(ZFUISizeMake(80, 60));
        view->viewBackgroundColorSet(ZFUIColorRandom());
        view->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);

        this->ani()->aniTargetSet(view);
        this->ani()->aniDurationSet((zftimet)3000);

        this->prepareSettingButton(window, this->ani());

        ZFLISTENER_LOCAL(startOnClick, {
            userData->to<ZFObjectHolder *>()->holdedObj.to<ZFAnimation *>()->aniStart();
        })
        zfblockedAlloc(ZFUIKit_test_Button, startButton);
        startButton->buttonLabelTextStringSet(zfText("start"));
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), startOnClick, this->ani()->objectHolder());
        container->childAdd(startButton);
        startButton->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner | ZFUIAlign::e_RightInner);
    }

    ZFPROPERTY_RETAIN_READONLY(ZFAnimationNativeView *, ani, ZFPropertyInitValue(zflineAlloc(ZFAnimationNativeView)))

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFAnimationNativeView *ani)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingForResetProperty(settings, ani, ZFCoreArrayPODCreate(const ZFProperty *
                , ZFPropertyAccess(ZFAnimationNativeView, aniCurve)
                , ZFPropertyAccess(ZFAnimationNativeView, aniAlphaFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniAlphaTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleXFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleXTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleYFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleYTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleZFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleZTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateXFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateXTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateYFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateYTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateZFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateZTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelXFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelXTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelYFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelYTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelZFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelZTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateXFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateXTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateYFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateYTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateZFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateZTo)
            ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zftimet, ZFPropertyAccess(ZFAnimationNativeView, aniDuration),
            ZFCoreArrayPODCreate(zftimet
                , (zftimet)3000
                , (zftimet)250
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, ZFAnimationNativeViewCurveEnum, ZFPropertyAccess(ZFAnimationNativeView, aniCurve),
            ZFCoreArrayPODCreate(ZFAnimationNativeViewCurveEnum
                , ZFAnimationNativeViewCurve::e_Linear
                , ZFAnimationNativeViewCurve::e_EaseIn
                , ZFAnimationNativeViewCurve::e_EaseOut
                , ZFAnimationNativeViewCurve::e_EaseInOut
                ));

        { // aniAlpha
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniAlphaFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniAlphaTo),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
        } // aniAlpha

        { // aniScale
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleXFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleXTo),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleYFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleYTo),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleZFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleZTo),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
        } // aniScale

        { // aniTranslate
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateXFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateXTo),
                ZFCoreArrayPODCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateYFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateYTo),
                ZFCoreArrayPODCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateZFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateZTo),
                ZFCoreArrayPODCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));
        } // aniTranslate

        { // aniTranslatePixel
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zfint, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelXFrom),
                ZFCoreArrayPODCreate(zfint
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zfint, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelXTo),
                ZFCoreArrayPODCreate(zfint
                    , 0
                    , 100
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zfint, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelYFrom),
                ZFCoreArrayPODCreate(zfint
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zfint, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelYTo),
                ZFCoreArrayPODCreate(zfint
                    , 0
                    , 100
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zfint, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelZFrom),
                ZFCoreArrayPODCreate(zfint
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zfint, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelZTo),
                ZFCoreArrayPODCreate(zfint
                    , 0
                    , 100
                    ));
        } // aniTranslatePixel

        { // aniRotate
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zfint, ZFPropertyAccess(ZFAnimationNativeView, aniRotateXFrom),
                ZFCoreArrayPODCreate(zfint
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zfint, ZFPropertyAccess(ZFAnimationNativeView, aniRotateXTo),
                ZFCoreArrayPODCreate(zfint
                    , 0
                    , 90
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zfint, ZFPropertyAccess(ZFAnimationNativeView, aniRotateYFrom),
                ZFCoreArrayPODCreate(zfint
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zfint, ZFPropertyAccess(ZFAnimationNativeView, aniRotateYTo),
                ZFCoreArrayPODCreate(zfint
                    , 0
                    , 90
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zfint, ZFPropertyAccess(ZFAnimationNativeView, aniRotateZFrom),
                ZFCoreArrayPODCreate(zfint
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zfint, ZFPropertyAccess(ZFAnimationNativeView, aniRotateZTo),
                ZFCoreArrayPODCreate(zfint
                    , 0
                    , 90
                    ));
        } // aniRotate

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFAnimationNativeView_test)

ZF_NAMESPACE_GLOBAL_END

