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

zfclass ZFUIKit_ZFUIViewStateAni_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIViewStateAni_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIView, testContainer);
        container->childAdd(testContainer);
        testContainer->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        testContainer->layoutParam()->layoutMarginSet(ZFUIMarginMake(0, 50, 0, 0));

        // use a view that not in view tree to test whether it would cause wrong ani state
        zfblockedAlloc(ZFUIView, testDummy);
        testContainer->tagSet(zfText("testDummy"), testDummy);

        for(zfindex i = 0; i < 4; ++i)
        {
            zfblockedAlloc(ZFUIView, child);
            testContainer->childAdd(child);
            child->viewSizePreferedSet(ZFUISizeMake(zfmRand(20, 80), zfmRand(20, 80)));
            child->viewBackgroundColorSet(ZFUIColorRandom(128));
        }

        ZFLISTENER_LOCAL(startOnClick, {
            ZFUIViewStateAniStart();

            ZFUIView *testContainer = userData->to<ZFObjectHolder *>()->holdedObj;

            {
                ZFUIView *testDummy = testContainer->tagGet<ZFUIView *>(zfText("testDummy"));
                testDummy->childRemoveAll();
                zfblockedAlloc(ZFUIView, testDummyChild);
                testDummy->childAdd(testDummyChild);
            }

            ZFCoreArrayPOD<ZFUIAlignEnum> xAlign = ZFCoreArrayPODCreate(ZFUIAlignEnum,
                ZFUIAlign::e_LeftInner, ZFUIAlign::e_Center, ZFUIAlign::e_RightInner);
            ZFCoreArrayPOD<ZFUIAlignEnum> yAlign = ZFCoreArrayPODCreate(ZFUIAlignEnum,
                ZFUIAlign::e_TopInner, ZFUIAlign::e_Center, ZFUIAlign::e_BottomInner);

            for(zfindex i = 0; i < testContainer->childCount(); ++i)
            {
                ZFUIView *child = testContainer->childAtIndex(i);

                child->layoutParam()->layoutAlignSet(
                        xAlign[zfmRand(xAlign.count())] | yAlign[zfmRand(yAlign.count())]
                    );
            }
        })
        zfblockedAlloc(ZFUIKit_test_Button, startButton);
        container->childAdd(startButton);
        startButton->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner | ZFUIAlign::e_RightInner);
        startButton->buttonLabelTextStringSet(zfText("start"));
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), startOnClick, testContainer->objectHolder());

        this->prepareSettingButton(window);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingForNormalProperty(settings, ZFUIViewStateAniSettingForMove::instance(), zftimet, ZFPropertyAccess(ZFUIViewStateAniSettingForMove, aniDuration),
            ZFCoreArrayPODCreate(zftimet
                , (zftimet)250
                , (zftimet)3000
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, ZFUIViewStateAniSettingForMove::instance(), ZFAnimationNativeViewCurveEnum, ZFPropertyAccess(ZFUIViewStateAniSettingForMove, aniCurve),
            ZFCoreArrayPODCreate(ZFAnimationNativeViewCurveEnum
                , ZFAnimationNativeViewCurve::e_Linear
                , ZFAnimationNativeViewCurve::e_EaseIn
                , ZFAnimationNativeViewCurve::e_EaseOut
                , ZFAnimationNativeViewCurve::e_EaseInOut
                ));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIViewStateAni_test)

// ============================================================
// debug
#if 0
ZF_GLOBAL_INITIALIZER_INIT(ZFUIViewStateAniDebug)
{
    this->debugEventStateAniOnInitListener = ZFCallbackForRawFunction(zfself::debugEventStateAniOnInit);
    this->debugEventStateAniOnDeallocListener = ZFCallbackForRawFunction(zfself::debugEventStateAniOnDealloc);
    this->debugEventStateAniViewAttachListener = ZFCallbackForRawFunction(zfself::debugEventStateAniViewAttach);
    this->debugEventStateAniViewDetachListener = ZFCallbackForRawFunction(zfself::debugEventStateAniViewDetach);
    this->debugEventStateAniViewAniPrepareListener = ZFCallbackForRawFunction(zfself::debugEventStateAniViewAniPrepare);
    this->debugEventStateAniViewAniStartListener = ZFCallbackForRawFunction(zfself::debugEventStateAniViewAniStart);
    this->debugEventStateAniViewAniStopListener = ZFCallbackForRawFunction(zfself::debugEventStateAniViewAniStop);

    ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniOnInit(), this->debugEventStateAniOnInitListener);
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniOnDealloc(), this->debugEventStateAniOnDeallocListener);
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniViewAttach(), this->debugEventStateAniViewAttachListener);
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniViewDetach(), this->debugEventStateAniViewDetachListener);
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniViewAniPrepare(), this->debugEventStateAniViewAniPrepareListener);
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniViewAniStart(), this->debugEventStateAniViewAniStartListener);
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniViewAniStop(), this->debugEventStateAniViewAniStopListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniDebug)
{
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniOnInit(), this->debugEventStateAniOnInitListener);
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniOnDealloc(), this->debugEventStateAniOnDeallocListener);
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniViewAttach(), this->debugEventStateAniViewAttachListener);
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniViewDetach(), this->debugEventStateAniViewDetachListener);
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniViewAniPrepare(), this->debugEventStateAniViewAniPrepareListener);
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniViewAniStart(), this->debugEventStateAniViewAniStartListener);
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniViewAniStop(), this->debugEventStateAniViewAniStopListener);
}
public:
    ZFListener debugEventStateAniOnInitListener;
    ZFListener debugEventStateAniOnDeallocListener;
    ZFListener debugEventStateAniViewAttachListener;
    ZFListener debugEventStateAniViewDetachListener;
    ZFListener debugEventStateAniViewAniPrepareListener;
    ZFListener debugEventStateAniViewAniStartListener;
    ZFListener debugEventStateAniViewAniStopListener;
public:
    static ZFLISTENER_PROTOTYPE_EXPAND(debugEventStateAniOnInit)
    {
        zfLogTrimT() << zfText("[ZFUIViewStateAni]") << zfText("onInit");
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(debugEventStateAniOnDealloc)
    {
        zfLogTrimT() << zfText("[ZFUIViewStateAni]") << zfText("onDealloc");
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(debugEventStateAniViewAttach)
    {
        zfLogTrimT() << zfText("[ZFUIViewStateAni]") << zfText("viewAttach") << listenerData.sender;
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(debugEventStateAniViewDetach)
    {
        zfLogTrimT() << zfText("[ZFUIViewStateAni]") << zfText("viewDetach") << listenerData.sender;
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(debugEventStateAniViewAniPrepare)
    {
        zfLogTrimT() << zfText("[ZFUIViewStateAni]") << zfText("viewAniPrepare") << listenerData.sender
            << listenerData.sender->to<ZFUIView *>()->layoutedFramePrev();
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(debugEventStateAniViewAniStart)
    {
        zfLogTrimT() << zfText("[ZFUIViewStateAni]") << zfText("viewAniStart") << listenerData.sender << listenerData.param0;
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(debugEventStateAniViewAniStop)
    {
        zfLogTrimT() << zfText("[ZFUIViewStateAni]") << zfText("viewAniStop") << listenerData.sender << listenerData.param0;
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniDebug)
#endif

ZF_NAMESPACE_GLOBAL_END

