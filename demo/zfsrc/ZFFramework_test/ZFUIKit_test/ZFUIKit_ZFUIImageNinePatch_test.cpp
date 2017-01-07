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

zfclass ZFUIKit_ZFUIImageNinePatch_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImageNinePatch_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIImageView, imageView);
        container->childAdd(imageView);
        imageView->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
        imageView->imageContentSet(ZFUIImageRes(zfText("test_normal.png")).to<ZFCopyable *>()->copy());
        imageView->imageContent()->imageNinePatchSet(ZFUIMarginMake(
            imageView->imageContent()->imageSize().width / 2,
            imageView->imageContent()->imageSize().height / 2));

        this->prepareSettingButton(window, imageView);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIImageView *imageView)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        { // wrap or fill
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userDataSet(zflineAlloc(ZFObject));
            setting->userData()->tagSet(zfText("imageView"), imageView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFUIImageView *imageView = userData->tagGet<ZFObjectHolder *>(zfText("imageView"))->holdedObj;
                ZFStringEditable *text = listenerData.param0->to<ZFStringEditable *>();
                zfbool fill = (imageView->layoutParam()->sizeParam().width == ZFUISizeType::e_Fill);
                text->stringValueSet(fill ? zfText("fill") : zfText("wrap"));
            })
            setting->buttonTextGetterSet(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIImageView *imageView = userData->tagGet<ZFObjectHolder *>(zfText("imageView"))->holdedObj;
                zfbool fill = (imageView->layoutParam()->sizeParam().width == ZFUISizeType::e_Fill);
                imageView->layoutParam()->sizeParamSet(fill ? ZFUISizeParamWrapWidthWrapHeight : ZFUISizeParamFillWidthFillHeight);
            })
            setting->buttonClickListenerSet(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIImageNinePatch_test)

ZF_NAMESPACE_GLOBAL_END

