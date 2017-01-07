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

zfclass ZFUIKit_ZFUIScrollView_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIScrollView_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIScrollView, scrollView);
        container->childAdd(scrollView);
        scrollView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        scrollView->layoutParam()->layoutMarginSet(ZFUIMarginMake(40));
        scrollView->viewBackgroundColorSet(ZFUIColorRed);

        this->setupScrollListener(scrollView);
        this->setupScrollContent(scrollView, ZFUISizeMake(120), ZFUISizeMake(60), ZFUISizeMake(10));

#if 1 // test embeded scroll view
        zfblockedAlloc(ZFUIScrollView, embededScrollView);
        scrollView->childAdd(embededScrollView);
        embededScrollView->layoutParam()->layoutMarginSet(ZFUIMarginMake(80));
        embededScrollView->layoutParam()->sizeHintSet(ZFUISizeMake(200, 100));
        embededScrollView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        embededScrollView->viewBackgroundColorSet(ZFUIColorMake(192, 0, 0, 255));

        this->setupScrollListener(embededScrollView);
        this->setupScrollContent(embededScrollView, ZFUISizeMake(60), ZFUISizeMake(30), ZFUISizeMake(10));
#endif

        this->prepareSettingButton(window, scrollView);
    }

private:
    void setupScrollListener(ZF_IN ZFUIScrollView *scrollView)
    {
#if 1 // output logs during scroll event may cause performance issue
        ZFLISTENER_LOCAL(onDragBegin, {
            zfLogTrimT() << zfText("onDragBegin  ") << listenerData.sender->objectInfoOfInstance() << listenerData.sender->to<ZFUIScrollView *>()->scrollContentFrame();
        })
        scrollView->observerAdd(ZFUIScrollView::EventScrollOnDragBegin(), onDragBegin);

        ZFLISTENER_LOCAL(onDrag, {
            zfLogTrimT() << zfText("onDrag       ") << listenerData.sender->objectInfoOfInstance() << listenerData.sender->to<ZFUIScrollView *>()->scrollContentFrame();
        })
        scrollView->observerAdd(ZFUIScrollView::EventScrollOnDrag(), onDrag);

        ZFLISTENER_LOCAL(onDragEnd, {
            zfLogTrimT() << zfText("onDragEnd    ") << listenerData.sender->objectInfoOfInstance() << listenerData.sender->to<ZFUIScrollView *>()->scrollContentFrame();
        })
        scrollView->observerAdd(ZFUIScrollView::EventScrollOnDragEnd(), onDragEnd);

        ZFLISTENER_LOCAL(onScrollBegin, {
            zfLogTrimT() << zfText("onScrollBegin") << listenerData.sender->objectInfoOfInstance() << listenerData.sender->to<ZFUIScrollView *>()->scrollContentFrame();
        })
        scrollView->observerAdd(ZFUIScrollView::EventScrollOnScrollBegin(), onScrollBegin);

        ZFLISTENER_LOCAL(onScroll, {
            zfLogTrimT() << zfText("onScroll     ") << listenerData.sender->objectInfoOfInstance() << listenerData.sender->to<ZFUIScrollView *>()->scrollContentFrame();
        })
        scrollView->observerAdd(ZFUIScrollView::EventScrollOnScroll(), onScroll);

        ZFLISTENER_LOCAL(onScrollEnd, {
            zfLogTrimT() << zfText("onScrollEnd  ") << listenerData.sender->objectInfoOfInstance() << listenerData.sender->to<ZFUIScrollView *>()->scrollContentFrame();
        })
        scrollView->observerAdd(ZFUIScrollView::EventScrollOnScrollEnd(), onScrollEnd);
#endif
    }
    void setupScrollContent(ZF_IN ZFUIScrollView *scrollView,
                            ZF_IN const ZFUISize &itemSize,
                            ZF_IN const ZFUISize &itemSpace,
                            ZF_IN const ZFUISize &itemCount,
                            ZF_IN_OPT const ZFUIMargin &contentMargin = ZFUIMarginZero)
    {
        scrollView->scrollContentFrameSet(ZFUIRectMake(
            0,
            0,
            ZFUIMarginGetX(contentMargin) + itemSize.width + (itemSize.width + itemSpace.width) * itemCount.width,
            ZFUIMarginGetY(contentMargin) + itemSize.height + (itemSize.height + itemSpace.height) * itemCount.height));

        for(zfint y = contentMargin.top;
            y <= scrollView->scrollContentFrame().size.height - itemSize.height - contentMargin.bottom;
            y += itemSize.height + itemSpace.height)
        {
            for(zfint x = contentMargin.left;
                x <= scrollView->scrollContentFrame().size.height - itemSize.width - contentMargin.right;
                x += itemSize.width + itemSpace.width)
            {
                zfblockedAlloc(ZFUIButton, btn);
                scrollView->childAdd(btn);
                btn->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
                btn->layoutParam()->sizeHintSet(itemSize);
                btn->layoutParam()->layoutMarginSet(ZFUIMarginMake(x, y, 0, 0));
                btn->viewBackgroundColorSet(ZFUIColorRandom(192));

                ZFLISTENER_LOCAL(onClick, {
                    zfLogTrimT() << zfText("clicked") << listenerData.sender;
                })
                btn->observerAdd(ZFUIButton::EventButtonOnClick(), onClick);
            }
        }
    }
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIScrollView *scrollView)
    {
        zfblockedAlloc(ZFArrayEditable, settings);
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollAlignToAxis));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollBounceHorizontal));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollBounceVertical));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollBounceHorizontalAlways));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollBounceVerticalAlways));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollAlignToPageHorizontal));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollAlignToPageVertical));

#define _ZFP_ZFUIKit_ZFUIScrollView_test_autoScrollSpeed 100
        { // auto scroll x
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userDataSet(zflineAlloc(ZFObject));
            setting->userData()->tagSet(zfText("scrollView"), scrollView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFUIScrollView *scrollView = userData->tagGet<ZFObjectHolder *>(zfText("scrollView"))->holdedObj;
                ZFStringEditable *text = listenerData.param0->to<ZFStringEditable *>();
                text->stringValueSet(zfstringWithFormat(zfText("autoScrollSpeedX: %d"), scrollView->autoScrollSpeedX()));
            })
            setting->buttonTextGetterSet(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIScrollView *scrollView = userData->tagGet<ZFObjectHolder *>(zfText("scrollView"))->holdedObj;
                if(scrollView->autoScrollSpeedX() == 0)
                {
                    scrollView->autoScrollStartX(_ZFP_ZFUIKit_ZFUIScrollView_test_autoScrollSpeed * ((zfmRand(2) == 0) ? 1 : -1));
                }
                else
                {
                    scrollView->autoScrollStopX();
                }
            })
            setting->buttonClickListenerSet(buttonClickListener);
        }
        { // auto scroll y
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userDataSet(zflineAlloc(ZFObject));
            setting->userData()->tagSet(zfText("scrollView"), scrollView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFUIScrollView *scrollView = userData->tagGet<ZFObjectHolder *>(zfText("scrollView"))->holdedObj;
                ZFStringEditable *text = listenerData.param0->to<ZFStringEditable *>();
                text->stringValueSet(zfstringWithFormat(zfText("autoScrollSpeedY: %d"), scrollView->autoScrollSpeedY()));
            })
            setting->buttonTextGetterSet(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIScrollView *scrollView = userData->tagGet<ZFObjectHolder *>(zfText("scrollView"))->holdedObj;
                if(scrollView->autoScrollSpeedY() == 0)
                {
                    scrollView->autoScrollStartY(_ZFP_ZFUIKit_ZFUIScrollView_test_autoScrollSpeed * ((zfmRand(2) == 0) ? 1 : -1));
                }
                else
                {
                    scrollView->autoScrollStopY();
                }
            })
            setting->buttonClickListenerSet(buttonClickListener);
        }

        { // scroll child to visible
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userDataSet(zflineAlloc(ZFObject));
            setting->userData()->tagSet(zfText("scrollView"), scrollView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFStringEditable *text = listenerData.param0->to<ZFStringEditable *>();
                text->stringValueSet(zfText("scrollChildToVisible"));
            })
            setting->buttonTextGetterSet(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIScrollView *scrollView = userData->tagGet<ZFObjectHolder *>(zfText("scrollView"))->holdedObj;
                zfindex index = scrollView->childCount() / 2;
                scrollView->scrollChildToVisible(scrollView->childAtIndex(index));
            })
            setting->buttonClickListenerSet(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIScrollView_test)

ZF_NAMESPACE_GLOBAL_END

