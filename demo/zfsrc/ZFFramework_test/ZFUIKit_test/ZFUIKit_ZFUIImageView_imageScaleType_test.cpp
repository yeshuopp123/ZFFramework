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

zfclass ZFUIKit_ZFUIImageView_imageScaleType_test_Container : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImageView_imageScaleType_test_Container, ZFUIView)

    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIImageView *, imageViewSmall, ZFPropertyInitValue(zflineAlloc(ZFUIImageView)))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIImageView *, imageViewLarge, ZFPropertyInitValue(zflineAlloc(ZFUIImageView)))

public:
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->viewBackgroundColorSet(ZFUIColorYellow);

        zfautoObject imageSmall = ZFUIImageRes(zfText("test_normal.png"));
        zfautoObject imageLarge = ZFUIImageScale(imageSmall.toAny(), ZFUISizeMake(600));

        this->childAdd(this->imageViewSmall());
        this->imageViewSmall()->imageContentSet(imageSmall.toAny());
        this->imageViewSmall()->viewBackgroundColorSet(ZFUIColorRed);

        this->childAdd(this->imageViewLarge());
        this->imageViewLarge()->imageContentSet(imageLarge.toAny());
        this->imageViewLarge()->viewBackgroundColorSet(ZFUIColorBlue);

        return this;
    }
protected:
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds)
    {
        zfsuper::layoutOnLayout(bounds);
        if(bounds.size.height >= bounds.size.width)
        {
            this->imageViewSmall()->layout(ZFUIRectApplyMargin(
                ZFUIRectMake(0, 0, bounds.size.width, bounds.size.height / 2),
                ZFUIMarginMake(5)));
            this->imageViewLarge()->layout(ZFUIRectApplyMargin(
                ZFUIRectMake(0, bounds.size.height / 2, bounds.size.width, bounds.size.height / 2),
                ZFUIMarginMake(5)));
        }
        else
        {
            this->imageViewSmall()->layout(ZFUIRectMake(0, 0, bounds.size.width, bounds.size.height / 2));
            this->imageViewLarge()->layout(ZFUIRectMake(bounds.size.width / 2, 0, bounds.size.width, bounds.size.height / 2));
        }
    }
};

zfclass ZFUIKit_ZFUIImageView_imageScaleType_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImageView_imageScaleType_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIKit_ZFUIImageView_imageScaleType_test_Container, layout);
        container->childAdd(layout);
        layout->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);

        zfblockedAlloc(ZFArrayEditable, imageViews);
        imageViews->add(layout->imageViewSmall());
        imageViews->add(layout->imageViewLarge());
        this->prepareSettingButton(window, imageViews);
    }

private:
    static void _nextValue(ZF_IN_OUT ZFUIContentScaleTypeEnum &value)
    {
        zfindex index = ZFUIContentScaleType::EnumIndexForValue(value);
        zfCoreAssert(index != zfindexMax);
        ++index;
        if(index >= ZFUIContentScaleType::EnumCount())
        {
            index = 0;
        }
        value = (ZFUIContentScaleTypeEnum)ZFUIContentScaleType::EnumValueAtIndex(index);
    }
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFArray *imageViews)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        { // imageScaleType
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userDataSet(imageViews);
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFArray *imageViews = userData->to<ZFArray *>();
                ZFStringEditable *text = listenerData.param0->to<ZFStringEditable *>();

                ZFUIImageView *imageView = imageViews->getFirst<ZFUIImageView *>();
                text->stringValueSet(ZFUIContentScaleType::EnumNameForValue(imageView->imageScaleType()));
            })
            setting->buttonTextGetterSet(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFArray *imageViews = userData->to<ZFArray *>();
                ZFUIContentScaleTypeEnum value = imageViews->getFirst<ZFUIImageView *>()->imageScaleType();
                _nextValue(value);
                for(zfindex i = 0; i < imageViews->count(); ++i)
                {
                    imageViews->get<ZFUIImageView *>(i)->imageScaleTypeSet(value);
                }
            })
            setting->buttonClickListenerSet(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIImageView_imageScaleType_test)

ZF_NAMESPACE_GLOBAL_END

