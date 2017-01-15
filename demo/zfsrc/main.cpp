/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFFramework_test_global.h"
#include "ZFFramework_test/ZFUIKit_test/ZFUIKit_test.h"
#include "ZFCore.h"
#include "ZFUIKit.h"
#include "ZFUIWidget.h"
#include "ZFUIWebKit.h"

#include "ZFCore/protocol/ZFProtocolZFThreadTaskRequest.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"

static zfbool _autoLeakTestOn = zftrue;
static zfbool _ZFP_ZFFramework_test_protocolCheck(void);
static zfautoObject _ZFP_ZFFramework_test_containerViewPrepare(void);
static void _ZFP_ZFFramework_test_prepareTestCase(ZF_IN ZFUIView *containerView);

ZFMAIN_ENTRY(params)
{
    if(_ZFP_ZFFramework_test_protocolCheck())
    {
        zfautoObject containerView = _ZFP_ZFFramework_test_containerViewPrepare();
        _ZFP_ZFFramework_test_prepareTestCase(containerView);
    }
    return 0;
}

static zfbool _ZFP_ZFFramework_test_protocolCheck(void)
{
    {
        ZFCoreArray<ZFProtocolImplInfoData> implDatas = ZFProtocolImplInfoDataGetNotImplemented();
        if(!implDatas.isEmpty())
        {
            zfclassNotPOD _ZFP_main_ZFProtocolImplInfoData_sort
            {
            public:
                static ZFCompareResult action(ZF_IN ZFProtocolImplInfoData const &v0, ZF_IN ZFProtocolImplInfoData const &v1)
                {
                    return ZFComparerDefault(v0.protocolName, v1.protocolName);
                }
            };
            implDatas.sort(_ZFP_main_ZFProtocolImplInfoData_sort::action);

            zfLogTrimT() << zfText("note, these protocol has not been implemented:");
            for(zfindex i = 0; i < implDatas.count(); ++i)
            {
                zfLogTrimT().autoSpaceSet(zffalse).autoEndlSet(zffalse) << zfText("    ");
                ZFProtocolImplInfoDataPrint(implDatas[i],
                    zfLogTrimT().autoSpaceSet(zffalse).autoEndlSet(zffalse));
                zfLogTrimT();
            }
        }
    }
    if(!ZFPROTOCOL_IS_AVAILABLE(ZFUIView)
       || !ZFPROTOCOL_IS_AVAILABLE(ZFUISysWindow)
       || !ZFPROTOCOL_IS_AVAILABLE(ZFThreadTaskRequest))
    {
        ZFTestCaseRunAllStart();
        return zffalse;
    }
    return zftrue;
}
static zfautoObject _ZFP_ZFFramework_test_containerViewPrepare(void)
{
    zfblockedAlloc(ZFUIWindow, window);
    window->windowShow();

    zfblockedAlloc(ZFUIKit_test_ListView, containerView);
    window->childAdd(containerView);
    containerView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);

    {
        zfblockedAlloc(ZFUIKit_test_Button, button);
        containerView->childAdd(button);

        _autoLeakTestOn = zffalse;
        ZFLISTENER_LOCAL(onClickButton, {
            _autoLeakTestOn = !_autoLeakTestOn;
            listenerData.sender->to<ZFUIButtonBasic *>()->buttonLabelTextStringSet(
                zfstringWithFormat(zfText("autoLeakTest %s"), _autoLeakTestOn ? zfText("on") : zfText("off")));
        })
        button->observerAdd(ZFUIButton::EventButtonOnClick(), onClickButton);
        button->buttonBackgroundStyle()->viewBackgroundColorSet(ZFUIColorRed);
        button->buttonSimulateClick();
    }

    {
        zfblockedAlloc(ZFUIKit_test_Button, button);
        containerView->childAdd(button);

        ZFLISTENER_LOCAL(onClickButton, {
            if(ZFUIViewStateAniAutoApplyStarted())
            {
                ZFUIViewStateAniAutoApplyStop();
            }
            else
            {
                ZFUIViewStateAniAutoApplyStart();
            }
            listenerData.sender->to<ZFUIButtonBasic *>()->buttonLabelTextStringSet(
                zfstringWithFormat(zfText("stateAni %s"), ZFUIViewStateAniAutoApplyStarted() ? zfText("on") : zfText("off")));
        })
        button->observerAdd(ZFUIButton::EventButtonOnClick(), onClickButton);
        button->buttonBackgroundStyle()->viewBackgroundColorSet(ZFUIColorRed);
        ZFUIViewStateAniAutoApplyStart();
        button->buttonSimulateClick();
    }

    {
        zfblockedAlloc(ZFUIView, separator);
        containerView->childAdd(separator);
        separator->viewSizeMinSet(ZFUISizeMake(0, 5));
        separator->viewSizeMaxSet(ZFUISizeMake(-1, 5));
        separator->viewBackgroundColorSet(ZFUIColorGray);
    }

    return zfautoObjectCreate(containerView);
}

static void leakTestBegin(void)
{
    if(_autoLeakTestOn)
    {
        ZFLeakTestBegin();
    }
}
static void leakTestEnd(void)
{
    if(_autoLeakTestOn)
    {
        ZFLeakTestPrintStatus(ZFLeakTestObjectFilter().filterAddName(ZFThreadTaskRequestData::ClassData()->className()));
        ZFLeakTestEnd();
    }
}
static void _ZFP_ZFFramework_test_prepareTestCaseSubModule(ZF_IN ZFUIView *containerView,
                                                           ZF_IN const zfchar *subModuleName,
                                                           ZF_IN ZFCoreArrayPOD<const ZFClass *> const &testCases);
static void _ZFP_ZFFramework_test_prepareTestCaseSubModuleTest(ZF_IN ZFUIView *containerView,
                                                               ZF_IN const zfchar *subModuleName,
                                                               ZF_IN const ZFClass *testCase);
static void _ZFP_ZFFramework_test_prepareTestCase(ZF_IN ZFUIView *containerView)
{
    ZFCoreArrayPOD<const ZFClass *> allTestCase = ZFTestCaseGetAll();
    zfclassNotPOD _ZFP_main_class_sort
    {
    public:
        static ZFCompareResult action(ZF_IN const ZFClass * const &cls0, ZF_IN const ZFClass * const &cls1)
        {
            return ZFComparerDefault(cls0->className(), cls1->className());
        }
    };
    allTestCase.sort(_ZFP_main_class_sort::action);

    while(!allTestCase.isEmpty())
    {
        ZFCoreArrayPOD<const ZFClass *> subModule;
        zfstring subModuleName = allTestCase[0]->className();
        {
            zfindex t = zfstringFind(subModuleName, '_');
            if(t == zfindexMax)
            {
                subModuleName.removeAll();
            }
            else
            {
                subModuleName = zfstring(subModuleName.cString(), t);
            }
        }
        for(zfindex i = 0; i < allTestCase.count(); ++i)
        {
            if(zfsncmp(allTestCase[i]->className(), subModuleName.cString(), subModuleName.length()) == 0)
            {
                subModule.add(allTestCase[i]);
                allTestCase.remove(i);
                --i;
            }
        }
        _ZFP_ZFFramework_test_prepareTestCaseSubModule(containerView, subModuleName, subModule);
    }
}
zfclass _ZFP_ZFFramework_test_TestCaseSubModuleData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFFramework_test_TestCaseSubModuleData, ZFObject)

    zfstring subModuleName;
    ZFCoreArrayPOD<const ZFClass *> testCases;
};
static void _ZFP_ZFFramework_test_prepareTestCaseSubModule(ZF_IN ZFUIView *containerView,
                                                           ZF_IN const zfchar *subModuleName,
                                                           ZF_IN ZFCoreArrayPOD<const ZFClass *> const &testCases)
{
    zfblockedAlloc(ZFUIKit_test_Button, button);
    containerView->childAdd(button);

    ZFLISTENER_LOCAL(onClickButton, {
        _ZFP_ZFFramework_test_TestCaseSubModuleData *subModuleData = userData->toAny();

        zfblockedAlloc(ZFUIWindow, subModuleWindow);
        subModuleWindow->viewBackgroundColorSet(ZFUIColorWhite);
        subModuleWindow->windowShow();
        zfblockedAlloc(ZFUIKit_test_ListView, containerView);
        subModuleWindow->childAdd(containerView);
        containerView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);

        {
            zfblockedAlloc(ZFUIKit_test_Button, closeButton);
            containerView->childAdd(closeButton);
            closeButton->buttonLabelTextStringSet(zfText("back"));
            ZFLISTENER_LOCAL(closeButtonOnClick, {
                userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIWindow *>()->windowHide();
            })
            closeButton->observerAdd(ZFUIButton::EventButtonOnClick(), closeButtonOnClick, subModuleWindow->objectHolder());
            closeButton->buttonBackgroundStyle()->viewBackgroundColorSet(ZFUIColorRed);

            zfblockedAlloc(ZFUIView, separator);
            containerView->childAdd(separator);
            separator->viewSizeMinSet(ZFUISizeMake(0, 5));
            separator->viewSizeMaxSet(ZFUISizeMake(-1, 5));
            separator->viewBackgroundColorSet(ZFUIColorGray);
        }

        for(zfindex i = 0; i < subModuleData->testCases.count(); ++i)
        {
            _ZFP_ZFFramework_test_prepareTestCaseSubModuleTest(containerView, subModuleData->subModuleName, subModuleData->testCases[i]);
        }
    })
    zfblockedAlloc(_ZFP_ZFFramework_test_TestCaseSubModuleData, subModuleData);
    subModuleData->subModuleName = subModuleName;
    subModuleData->testCases = testCases;
    button->observerAdd(ZFUIButton::EventButtonOnClick(), onClickButton, subModuleData);
    button->buttonLabelTextStringSet(subModuleName);
}
static void _ZFP_ZFFramework_test_prepareTestCaseSubModuleTest(ZF_IN ZFUIView *containerView,
                                                               ZF_IN const zfchar *subModuleName,
                                                               ZF_IN const ZFClass *testCase)
{
    zfblockedAlloc(ZFUIKit_test_Button, button);
    containerView->childAdd(button);

    ZFLISTENER_LOCAL(onClickButton, {
        leakTestBegin();

        const ZFClass *testCase = userData->tagGet<ZFPointerHolder *>(zfText("testCase"))->holdedDataPointer<const ZFClass *>();
        ZFUIView *containerView = userData->tagGet<ZFObjectHolder *>(zfText("containerView"))->holdedObj;
        ZFTestCase *running = zfnull;
        containerView->viewUIEnableTreeSet(zffalse);
        ZFTestCaseRun(testCase, &running);
        if(running != zfnull)
        {
            ZFLISTENER_LOCAL(testCaseOnStop, {
                ZFUIView *containerView = userData->tagGet<ZFObjectHolder *>(zfText("containerView"))->holdedObj;
                containerView->viewUIEnableTreeSet(zftrue);
                if(_autoLeakTestOn)
                {
                    ZFLISTENER_LOCAL(leakTestEndDelay, {
                        leakTestEnd();
                    })
                    ZFThreadTaskRequest(leakTestEndDelay);
                }
            })
            running->observerAdd(ZFTestCase::EventTestCaseOnStop(), testCaseOnStop, userData);
        }
        else
        {
            leakTestEnd();
            containerView->viewUIEnableTreeSet(zftrue);
        }
    })
    zfblockedAlloc(ZFObject, userData);
    userData->tagSet(zfText("testCase"), zflineAlloc(ZFPointerHolder, testCase));
    userData->tagSet(zfText("containerView"), containerView->objectHolder());
    button->observerAdd(ZFUIButton::EventButtonOnClick(), onClickButton, userData);
    button->buttonLabelTextStringSet(zfstring(testCase->className() + zfslen(subModuleName) + 1));
}

ZF_GLOBAL_INITIALIZER_INIT(iOS7AutoTitleSpace)
{
    zfstring frameworkName = ZFEnvInfo::frameworkName();
    if(zfstringFindCaseInsensitive(frameworkName, zfText("iOS")) != zfindexMax
       || zfstringFindCaseInsensitive(frameworkName, zfText("iPhone")) != zfindexMax
       || zfstringFindCaseInsensitive(frameworkName, zfText("iPad")) != zfindexMax)
    {
        if(ZFVersionGetInt(ZFEnvInfo::frameworkVersion(), ZFVersion_main) >= 7)
        {
            ZFLISTENER_LOCAL(windowLayoutParamOnUpdate, {
                ZFUIViewLayoutParam *lp = listenerData.sender->to<ZFUIWindow *>()->windowLayoutParam();
                ZFUIMargin margin = lp->layoutMargin();
                margin.top = 20;
                lp->layoutMarginSet(margin);
            })
            ZFObjectGlobalEventObserverAdd(ZFUIWindow::EventWindowLayoutParamOnInit(), windowLayoutParamOnUpdate);
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(iOS7AutoTitleSpace)

