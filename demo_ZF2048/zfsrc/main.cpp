/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCore.h"
#include "ZFUIKit.h"
#include "ZFUIWidget.h"

#include "ZF2048Core/ZF2048Core.h"
#include "ZF2048UI/ZF2048UIFrame.h"
#include "ZF2048App.h"

ZFMAIN_ENTRY(params)
{
    if(zftrue)
    {
        zfLangLoad(ZFXmlParse(ZFInputCallbackForResFile(zfText("ZF2048/lang/lang_default.xml"))));

        zfblockedAlloc(ZFUIWindow, window);
        window->windowShow();

        zfblockedAlloc(ZF2048App, game);
        window->childAdd(game);
        game->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        game->layoutParam()->layoutMarginSet(ZFUIMarginMake(0, 20, 0, 0));
    }
    else
    {
        zfblockedAlloc(ZF2048Core, game);
        game->reset();
        game->debugStatus();

        zfblockedAlloc(ZFUIWindow, window);
        window->tagSet(zfText("game"), game);
        window->windowShow();
        zfblockedAlloc(ZFUILinearLayout, ll);
        window->childAdd(ll);
        ll->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
        ll->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthWrapHeight);

        zfblockedAlloc(ZFUIButtonBasic, left);
        ll->childAdd(left);
        left->buttonLabelTextStringSet(zfText("left"));
        ZFLISTENER_LOCAL(leftOnClick, {
            ZF2048Core *game = userData->toAny();
            game->moveLeft();
            game->debugStatus();
        })
        left->observerAdd(ZFUIButton::EventButtonOnClick(), leftOnClick, game);

        zfblockedAlloc(ZFUIButtonBasic, top);
        ll->childAdd(top);
        top->buttonLabelTextStringSet(zfText("top"));
        ZFLISTENER_LOCAL(topOnClick, {
            ZF2048Core *game = userData->toAny();
            game->moveTop();
            game->debugStatus();
        })
        top->observerAdd(ZFUIButton::EventButtonOnClick(), topOnClick, game);

        zfblockedAlloc(ZFUIButtonBasic, right);
        ll->childAdd(right);
        right->buttonLabelTextStringSet(zfText("right"));
        ZFLISTENER_LOCAL(rightOnClick, {
            ZF2048Core *game = userData->toAny();
            game->moveRight();
            game->debugStatus();
        })
        right->observerAdd(ZFUIButton::EventButtonOnClick(), rightOnClick, game);

        zfblockedAlloc(ZFUIButtonBasic, bottom);
        ll->childAdd(bottom);
        bottom->buttonLabelTextStringSet(zfText("bottom"));
        ZFLISTENER_LOCAL(bottomOnClick, {
            ZF2048Core *game = userData->toAny();
            game->moveBottom();
            game->debugStatus();
        })
        bottom->observerAdd(ZFUIButton::EventButtonOnClick(), bottomOnClick, game);

        zfblockedAlloc(ZFUIButtonBasic, reset);
        ll->childAdd(reset);
        reset->buttonLabelTextStringSet(zfText("reset"));
        ZFLISTENER_LOCAL(resetOnClick, {
            ZF2048Core *game = userData->toAny();
            game->reset();
            game->debugStatus();
        })
        reset->observerAdd(ZFUIButton::EventButtonOnClick(), resetOnClick, game);

        zfblockedAlloc(ZFUIButtonBasic, undo);
        ll->childAdd(undo);
        undo->buttonLabelTextStringSet(zfText("undo"));
        ZFLISTENER_LOCAL(undoOnClick, {
            ZF2048Core *game = userData->toAny();
            game->undo();
            game->debugStatus();
        })
        undo->observerAdd(ZFUIButton::EventButtonOnClick(), undoOnClick, game);

        for(zfindex i = 0; i < ll->childCount(); ++i)
        {
            ZFUIButtonBasic *button = ll->childAtIndex(i)->toAny();
            button->viewBackgroundColorSet(ZFUIColorRandom());
            button->layoutParam<ZFUILinearLayoutParam *>()->layoutWeightSet(1);
        }
    }

    return 0;
}

#if 0
ZF_GLOBAL_INITIALIZER_INIT(AutoPrintUIViewTree)
{
    ZFLISTENER_LOCAL(action, {
        ZFUIViewPrintViewTree(ZFUISysWindow::mainWindow()->rootView());
    })
    ZFUISysWindow::mainWindow()->observerAdd(ZFUISysWindow::EventSysWindowOnPause(), action);
}
ZF_GLOBAL_INITIALIZER_END(AutoPrintUIViewTree)
#endif

#if 1 // just for fun, click one view 3 times at same point, would change language
ZF_GLOBAL_INITIALIZER_INIT(testLangLoad)
{
    ZFObjectGlobalEventObserverAdd(ZFUIView::EventViewOnEvent(), ZFCallbackForRawFunction(zfself::action));
}
ZF_GLOBAL_INITIALIZER_DESTROY(testLangLoad)
{
    ZFObjectGlobalEventObserverRemove(ZFUIView::EventViewOnEvent(), ZFCallbackForRawFunction(zfself::action));
}
private:
    static ZFLISTENER_PROTOTYPE_EXPAND(action)
    {
        ZFUIMouseEvent *event = ZFCastZFObject(ZFUIMouseEvent *, listenerData.param0);
        if(event == zfnull || event->mouseAction != ZFUIMouseAction::e_MouseUp)
        {
            return ;
        }
        static ZFUIPoint lastPoint = ZFUIPointZero;
        static zfindex lastCount = 0;
        static zfbool langFlag = zffalse;
        if(zfmAbs(event->mousePoint.x - lastPoint.x) <= 5
           && zfmAbs(event->mousePoint.y - lastPoint.y) <= 5)
        {
            ++lastCount;
            if(lastCount == 3)
            {
                lastCount = 0;
                langFlag = !langFlag;
                if(langFlag)
                {
                    zfLangLoad(ZFXmlParse(ZFInputCallbackForResFile(zfText("ZF2048/lang/lang_default.xml"))));
                }
                else
                {
                    zfLangLoad(ZFXmlParse(ZFInputCallbackForResFile(zfText("ZF2048/lang/lang_zh-CN.xml"))));
                }
            }
        }
        else
        {
            lastCount = 0;
        }
        lastPoint = event->mousePoint;
    }
ZF_GLOBAL_INITIALIZER_END(testLangLoad)
#endif

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

