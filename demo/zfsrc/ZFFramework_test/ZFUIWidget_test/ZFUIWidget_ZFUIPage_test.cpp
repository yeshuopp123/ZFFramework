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

zfclass ZFUIWidget_ZFUIPage_test_Page : zfextends ZFUIPageBasic
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIPage_test_Page, ZFUIPageBasic)

    ZFPROPERTY_RETAIN_WITH_INIT(ZFUILinearLayout *, _mainView, ZFPropertyInitValue(zflineAlloc(ZFUILinearLayout)))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUILinearLayout *, _titleView, ZFPropertyInitValue(zflineAlloc(ZFUILinearLayout)))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIButtonBasic *, _titleLeftView, ZFPropertyInitValue(zflineAlloc(ZFUIButtonBasic)))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUITextView *, _titleCenterView, ZFPropertyInitValue(zflineAlloc(ZFUITextView)))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIButtonBasic *, _contentView, ZFPropertyInitValue(zflineAlloc(ZFUIButtonBasic)))

protected:
    zfoverride
    virtual void pageOnCreate(void)
    {
        zfsuper::pageOnCreate();

        this->pageGroupIdSet(this->pageCreateParam()->to<ZFString *>()->stringValue());

        this->_mainView()->layoutOrientationSet(ZFUIOrientation::e_Top);

        this->_mainView()->childAdd(this->_titleView());
        this->_titleView()->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthWrapHeight);
        this->_titleView()->viewBackgroundColorSet(ZFUIColorRandom());
        this->_titleView()->layoutOrientationSet(ZFUIOrientation::e_Left);
        this->_titleView()->viewSizeMinSet(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeButton()));

        this->_titleView()->childAdd(this->_titleLeftView());
        this->_titleLeftView()->layoutParam<ZFUILinearLayoutParam *>()->layoutWeightSet(1);
        this->_titleLeftView()->buttonLabelTextStringSet(zfText("back"));
        this->_titleLeftView()->viewBackgroundColorSet(ZFUIColorRandom());
        {
            ZFLISTENER_LOCAL(_titleLeftViewOnClick, {
                ZFUIPage *page = userData->to<ZFObjectHolder *>()->holdedObj;
                page->pageDestroy();
            })
            this->_titleLeftView()->observerAdd(ZFUIButton::EventButtonOnClick(), _titleLeftViewOnClick, this->objectHolder());
        }

        this->_titleView()->childAdd(this->_titleCenterView());
        this->_titleCenterView()->layoutParam<ZFUILinearLayoutParam *>()->layoutWeightSet(3);
        this->_titleCenterView()->textContentStringSet(zfstringWithFormat(zfText("page %p"), this));
        this->_titleCenterView()->textAlignSet(ZFUIAlign::e_Center);

        this->_mainView()->childAdd(this->_contentView());
        this->_contentView()->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        this->_contentView()->viewBackgroundColorSet(ZFUIColorRandom());
        this->_contentView()->buttonLabelTextStringSet(zfstringWithFormat(zfText("belong to %s"), this->pageGroupId().cString()));
        {
            ZFLISTENER_LOCAL(_contentViewOnClick, {
                ZFUIPage *page = userData->to<ZFObjectHolder *>()->holdedObj;
                page->pageManager()->requestPageCreate(ZFUIPageRequestPageCreateParam()
                    .pageClassSet(zfself::ClassData())
                    .pageCreateParamSet(page->pageCreateParam()));
            })
            this->_contentView()->observerAdd(ZFUIButton::EventButtonOnClick(), _contentViewOnClick, this->objectHolder());
        }

        ZFAnimationNativeView *pageAni = zfnull;

        this->pageAniResumeByRequestSet(pageAni = zflineAlloc(ZFAnimationNativeView));
        pageAni->aniTranslateXFromSet(1);

        this->pageAniResumeFromBackgroundSet(pageAni = zflineAlloc(ZFAnimationNativeView));
        pageAni->aniTranslateXFromSet(-1);

        this->pageAniPauseToBackgroundSet(pageAni = zflineAlloc(ZFAnimationNativeView));
        pageAni->aniTranslateXToSet(-1);

        this->pageAniPauseBeforeDestroySet(pageAni = zflineAlloc(ZFAnimationNativeView));
        pageAni->aniTranslateXToSet(1);
    }
    static ZFCompareResult pageFindFirstByPageGroupId(ZF_IN ZFUIPage * const &e0,
                                                      ZF_IN const zfchar * const &e1)
    {
        if(zfscmpTheSame(e0->pageGroupId().cString(), e1))
        {
            return ZFCompareTheSame;
        }
        return ZFCompareUncomparable;
    }
    zfoverride
    virtual void pageOnResume(ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        zfsuper::pageOnResume(reason);
        zfindex index = this->pageManager()->pageList().find(this->pageGroupId().cString(), zfself::pageFindFirstByPageGroupId);
        if(index != zfindexMax && this->pageManager()->pageAt(index) != this)
        {
            this->_titleLeftView()->viewVisibleSet(zftrue);
        }
        else
        {
            this->_titleLeftView()->viewVisibleSet(zffalse);
        }
    }
    zfoverride
    virtual void pageAniOnUpdateAniTarget(ZF_IN ZFAnimation *pageAni)
    {
        zfsuper::pageAniOnUpdateAniTarget(pageAni);
        pageAni->aniTargetSet(this->_mainView());
    }
};
zfclass ZFUIWidget_ZFUIPage_test_PageManager : zfextends ZFUIPageManagerBasic
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIPage_test_PageManager, ZFUIPageManagerBasic)

    ZFPROPERTY_RETAIN(ZFUIView *, _container)
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUILinearLayout *, _mainView, ZFPropertyInitValue(zflineAlloc(ZFUILinearLayout)))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIView *, _pageContainer, ZFPropertyInitValue(zflineAlloc(ZFUIView)))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUILinearLayout *, _buttonLayout, ZFPropertyInitValue(zflineAlloc(ZFUILinearLayout)))

public:
    virtual ZFObject *objectOnInit(ZF_IN ZFUIView *container)
    {
        this->objectOnInit();
        this->_containerSet(container);
        return this;
    }
    virtual ZFObject *objectOnInit(void)
    {
        return zfsuper::objectOnInit();
    }
    virtual void objectOnInitFinish(void)
    {
        zfsuper::objectOnInitFinish();
        this->embededCreate();
        this->embededResume();
    }
    virtual void objectOnDeallocPrepare(void)
    {
        this->embededPause();
        this->embededDestroy();
        zfsuper::objectOnDeallocPrepare();
    }

protected:
    zfoverride
    virtual void managerOnCreate(void)
    {
        zfsuper::managerOnCreate();

        this->_container()->childAdd(this->_mainView());
        this->_mainView()->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        this->_mainView()->layoutOrientationSet(ZFUIOrientation::e_Top);
        this->_mainView()->layoutParam()->layoutMarginSet(ZFUIMarginMake(20));

        this->_mainView()->childAdd(this->_pageContainer());
        this->_pageContainer()->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        this->_pageContainer()->layoutParam<ZFUILinearLayoutParam *>()->layoutWeightSet(1);

        this->_mainView()->childAdd(this->_buttonLayout());
        this->_buttonLayout()->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthWrapHeight);
        this->_buttonLayout()->layoutOrientationSet(ZFUIOrientation::e_Left);

        for(zfindex i = 0; i < 4; ++i)
        {
            zfblockedAlloc(ZFString, pageGroupId, zfstringWithFormat(zfText("pageGroup %zi"), i));

            this->requestPageCreate(ZFUIPageRequestPageCreateParam()
                .pageClassSet(ZFUIWidget_ZFUIPage_test_Page::ClassData())
                .pageCreateParamSet(pageGroupId)
                .pageAutoResumeSet(zffalse));

            zfblockedAlloc(ZFUIButtonBasic, button);
            this->_buttonLayout()->childAdd(button);
            button->layoutParam<ZFUILinearLayoutParam *>()->layoutWeightSet(1);
            button->buttonLabelTextStringSet(zfstringWithFormat(zfText("tab %zi"), i));
            button->buttonCheckableSet(zftrue);
            ZFLISTENER_LOCAL(buttonOnClick, {
                ZFUIWidget_ZFUIPage_test_PageManager *pageManager = userData->tagGet<ZFObjectHolder *>(zfText("pageManager"))->holdedObj;
                ZFString *pageGroupId = userData->tagGet<ZFString *>(zfText("pageGroupId"));

                if(!listenerData.sender->to<ZFUIButton *>()->buttonChecked())
                {
                    listenerData.sender->to<ZFUIButton *>()->buttonCheckedSet(zftrue);

                    zfbool first = zftrue;
                    for(zfindex i = 0; i < pageManager->pageCount(); ++i)
                    {
                        if(pageManager->pageAt(i)->pageGroupId().compare(pageGroupId->stringValue()) == 0)
                        {
                            if(first)
                            {
                                first = zffalse;
                            }
                            else
                            {
                                pageManager->pageAt(i)->pageDestroy();
                            }
                        }
                    }
                }
                else
                {
                    for(zfindex i = 0; i < pageManager->_buttonLayout()->childCount(); ++i)
                    {
                        ZFUIButton *button = pageManager->_buttonLayout()->childAtIndex(i)->toAny();
                        button->buttonCheckedSet(zffalse);
                    }
                    listenerData.sender->to<ZFUIButton *>()->buttonCheckedSet(zftrue);

                    zfblockedAlloc(ZFAnimationNativeView, resumeAni);
                    resumeAni->aniScaleXFromSet(0.8f);
                    resumeAni->aniScaleYFromSet(0.8f);
                    resumeAni->aniAlphaFromSet(0.5f);
                    zfblockedAlloc(ZFAnimationNativeView, pauseAni);
                    pauseAni->aniAlphaToSet(0);
                    pageManager->pageAniOverrideForOnce(resumeAni, pauseAni);

                    pageManager->requestPageGroupResume(pageGroupId->stringValue());
                }
            })
            zfblockedAlloc(ZFObject, userData);
            userData->tagSet(zfText("pageManager"), this->objectHolder());
            userData->tagSet(zfText("pageGroupId"), pageGroupId);
            button->observerAdd(ZFUIButton::EventButtonOnClick(), buttonOnClick, userData);
            zfint r = zfmRand(255);
            zfint g = zfmRand(255);
            zfint b = zfmRand(255);
            button->buttonBackgroundStyleNormal()->imageContentSet(ZFUIImageLoadFromColor(ZFUIColorMake(64, r, g, b)).toAny());
            button->buttonBackgroundStyleHighlighted()->imageContentSet(ZFUIImageLoadFromColor(ZFUIColorMake(255, r, g, b)).toAny());
        }
        this->pageAniOverrideForOnce(zfnull, zfnull);
        this->_buttonLayout()->childAtIndex(0)->to<ZFUIButton *>()->buttonCheckedSet(zftrue);
    }
    zfoverride
    virtual void managerUIBlockedOnChange(void)
    {
        zfsuper::managerUIBlockedOnChange();
        this->_container()->viewUIEnableTreeSet(!this->managerUIBlocked());
    }
    zfoverride
    virtual void pageOnAttach(ZF_IN ZFUIPage *page,
                              ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        zfsuper::pageOnAttach(page, reason);

        ZFUIWidget_ZFUIPage_test_Page *pageTmp = page->toAny();
        this->_pageContainer()->childAdd(pageTmp->_mainView());
        pageTmp->_mainView()->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
    }
    zfoverride
    virtual void pageOnDetach(ZF_IN ZFUIPage *page,
                             ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        zfsuper::pageOnDetach(page, reason);

        ZFUIWidget_ZFUIPage_test_Page *pageTmp = page->toAny();
        this->_pageContainer()->childRemove(pageTmp->_mainView());
    }
};

zfclass ZFUIWidget_ZFUIPage_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIPage_test, ZFFramework_test_TestCase)

private:
    ZFUIPageManagerBasic *_pageManager;

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFFramework_test_protocolCheck(ZFAnimationNativeView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        this->_pageManager = zfAlloc(ZFUIWidget_ZFUIPage_test_PageManager, container);

        this->prepareSettingButton(window, this->_pageManager);
    }
    zfoverride
    virtual void testCaseOnStop(ZF_IN ZFResultTypeEnum testCaseResult)
    {
        zfRelease(this->_pageManager);
        this->_pageManager = zfnull;
        zfsuper::testCaseOnStop(testCaseResult);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIPageManagerBasic *pageManager)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIPage_test)

ZF_NAMESPACE_GLOBAL_END

