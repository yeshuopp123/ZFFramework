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

zfclass ZFUIWidget_ZFUIAnimatedImageView_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIAnimatedImageView_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIAnimatedImageView, animatedImageView);
        container->childAdd(animatedImageView);

        animatedImageView->animatedImage()->aniFrameAdd(ZFUIImageLoadFromColor(ZFUIColorRandom(), ZFUISizeMake(40)), 1000);
        animatedImageView->animatedImage()->aniFrameAdd(ZFUIImageLoadFromColor(ZFUIColorRandom(), ZFUISizeMake(60)), 1000);
        animatedImageView->animatedImage()->aniFrameAdd(ZFUIImageLoadFromColor(ZFUIColorRandom(), ZFUISizeMake(80)), 1000);
        animatedImageView->animatedImage()->aniFrameAdd(ZFUIImageLoadFromColor(ZFUIColorRandom(), ZFUISizeMake(40, 40)), 1000);
        animatedImageView->animatedImage()->aniFrameAdd(ZFUIImageLoadFromColor(ZFUIColorRandom(), ZFUISizeMake(60, 40)), 1000);
        animatedImageView->animatedImage()->aniFrameAdd(ZFUIImageLoadFromColor(ZFUIColorRandom(), ZFUISizeMake(80, 40)), 1000);

        this->prepareActionButton(container, animatedImageView);

        this->prepareSettingButton(window, animatedImageView);
    }

private:
    void prepareActionButton(ZF_IN ZFUIView *container,
                             ZF_IN ZFUIAnimatedImageView *animatedImageView)
    {
        ZFLISTENER_LOCAL(manualOnClick, {
            ZFUIAnimatedImageView *animatedImageView = userData->to<ZFObjectHolder *>()->holdedObj;
            animatedImageView->animatedImage()->aniManualNext();
        })
        zfblockedAlloc(ZFUIKit_test_Button, manualButton);
        container->childAdd(manualButton);
        manualButton->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner | ZFUIAlign::e_BottomInner);
        manualButton->buttonLabelTextStringSet(zfText("manual"));
        manualButton->observerAdd(ZFUIButton::EventButtonOnClick(), manualOnClick, animatedImageView->objectHolder());

        ZFLISTENER_LOCAL(startOnClick, {
            ZFUIAnimatedImageView *animatedImageView = userData->to<ZFObjectHolder *>()->holdedObj;
            animatedImageView->aniStart();
        })
        zfblockedAlloc(ZFUIKit_test_Button, startButton);
        container->childAdd(startButton);
        startButton->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center | ZFUIAlign::e_BottomInner);
        startButton->buttonLabelTextStringSet(zfText("start"));
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), startOnClick, animatedImageView->objectHolder());

        ZFLISTENER_LOCAL(stopOnClick, {
            ZFUIAnimatedImageView *animatedImageView = userData->to<ZFObjectHolder *>()->holdedObj;
            animatedImageView->aniStop();
        })
        zfblockedAlloc(ZFUIKit_test_Button, stopButton);
        container->childAdd(stopButton);
        stopButton->layoutParam()->layoutAlignSet(ZFUIAlign::e_RightInner | ZFUIAlign::e_BottomInner);
        stopButton->buttonLabelTextStringSet(zfText("stop"));
        stopButton->observerAdd(ZFUIButton::EventButtonOnClick(), stopOnClick, animatedImageView->objectHolder());
    }
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIAnimatedImageView *animatedImageView)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingForBoolProperty(settings, animatedImageView, ZFPropertyAccess(ZFUIAnimatedImageView, animatedImageWrapToImage));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIAnimatedImageView_test)

ZF_NAMESPACE_GLOBAL_END

