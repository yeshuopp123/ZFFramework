/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZF2048App.h"
#include "ZFUIWidget.h"

#include "ZF2048App/ZF2048AppButton.h"
#include "ZF2048App/ZF2048AppAutoMoveSettingDialog.h"
#include "ZF2048App/ZF2048AppAutoMoveRunner.h"
#include "ZF2048App/ZF2048AppSettingDialog.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZF2048AppTextView : zfextends ZFUITextView
{
    ZFOBJECT_DECLARE(_ZFP_ZF2048AppTextView, ZFUITextView)

public:
    zfoverride
    virtual void objectOnInitFinish(void)
    {
        zfsuper::objectOnInitFinish();
        this->textColorSet(ZFUIColorBlue);
    }
};

zfclass _ZFP_ZF2048AppPrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZF2048AppPrivate, ZFObject)

public:
    ZF2048App *owner;

    ZFPROPERTY_RETAIN_READONLY(ZFUILinearLayout *, mainLayout, ZFPropertyInitValue(zflineAlloc(ZFUILinearLayout)))
    ZFPROPERTY_RETAIN_READONLY(ZFUILinearLayout *, headerLayout, ZFPropertyInitValue(zflineAlloc(ZFUILinearLayout)))

    ZFPROPERTY_RETAIN_READONLY(ZFUIButtonBasic *, autoMoveButton, ZFPropertyInitValue(zflineAlloc(ZF2048AppButton)))
    ZFPROPERTY_RETAIN_READONLY(ZFUIButtonBasic *, undoButton, ZFPropertyInitValue(zflineAlloc(ZF2048AppButton)))
    ZFPROPERTY_RETAIN_READONLY(ZFUIViewLayout *, scoreLayoutHolder, ZFPropertyInitValue(zflineAlloc(ZFUIViewLayout)))
    ZFPROPERTY_RETAIN_READONLY(ZFUILinearLayout *, scoreLayout, ZFPropertyInitValue(zflineAlloc(ZFUILinearLayout)))
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, scoreHint, ZFPropertyInitValue(zflineAlloc(_ZFP_ZF2048AppTextView)))
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, scoreLabel, ZFPropertyInitValue(zflineAlloc(_ZFP_ZF2048AppTextView)))
    ZFPROPERTY_RETAIN_READONLY(ZFUIButtonBasic *, settingButton, ZFPropertyInitValue(zflineAlloc(ZF2048AppButton)))
    ZFPROPERTY_RETAIN_READONLY(ZFUIButtonBasic *, aboutButton, ZFPropertyInitValue(zflineAlloc(ZF2048AppButton)))

    ZFPROPERTY_RETAIN_READONLY(ZF2048AppAutoMoveSettingDialog *, autoMoveSettingDialog, ZFPropertyInitValue(zflineAlloc(ZF2048AppAutoMoveSettingDialog)))
    ZFPROPERTY_RETAIN_READONLY(ZF2048AppAutoMoveRunner *, autoMoveRunner, ZFPropertyInitValue(zflineAlloc(ZF2048AppAutoMoveRunner)))

public:
    void orientationUpdate(void)
    {
        ZFUIWindow *window = ZFUIWindow::windowForView(this->mainLayout());
        ZFUIOrientationEnum orientation = ZFUIOrientation::e_Top;
        if(window != zfnull)
        {
            orientation = window->ownerSysWindow()->windowOrientation();
        }
        else
        {
            orientation = ZFUISysWindow::mainWindow()->windowOrientation();
        }
        switch(orientation)
        {
            case ZFUIOrientation::e_Top:
            case ZFUIOrientation::e_Bottom:
                this->mainLayout()->layoutOrientationSet(ZFUIOrientation::e_Top);
                this->headerLayout()->layoutOrientationSet(ZFUIOrientation::e_Left);
                this->headerLayout()->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthWrapHeight);
                this->scoreLayout()->layoutOrientationSet(ZFUIOrientation::e_Left);
                break;
            case ZFUIOrientation::e_Left:
            case ZFUIOrientation::e_Right:
                this->mainLayout()->layoutOrientationSet(ZFUIOrientation::e_Left);
                this->headerLayout()->layoutOrientationSet(ZFUIOrientation::e_Top);
                this->headerLayout()->layoutParam()->sizeParamSet(ZFUISizeParamWrapWidthFillHeight);
                this->scoreLayout()->layoutOrientationSet(ZFUIOrientation::e_Top);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
    ZFLISTENER_DECLARE(orientationOnChange)
    {
        if(ZFUIWindow::windowForView(this->mainLayout())->ownerSysWindow() != listenerData.sender)
        {
            return ;
        }
        this->orientationUpdate();
    }

public:
    ZFLISTENER_DECLARE(appPaused)
    {
        this->autoMoveRunner()->runnerStop();
        this->autoMoveButton()->buttonCheckedSet(zffalse);
    }

public:
    void gameOver(void)
    {
        zfblockedAlloc(ZF2048AppDialog, hintDialog);
        zfLangApplyProperty_zfstring(hintDialog, ZFPropertyAccess(ZF2048AppDialog, dialogTitle), zfText("ZF2048_gameOver"));

        zfblockedAlloc(ZFUILinearLayout, container);
        hintDialog->dialogContentSet(container);
        container->layoutOrientationSet(ZFUIOrientation::e_Left);
        container->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

        zfblockedAlloc(ZFObject, userData);
        userData->tagSet(zfText("dialog"), hintDialog->objectHolder());
        userData->tagSet(zfText("game"), this->owner->game()->objectHolder());

        zfblockedAlloc(ZF2048AppButton, retryButton);
        container->childAdd(retryButton);
        zfLangApplyZFUIButtonBasicTextNormal(retryButton, zfText("ZF2048_gameOver_retry"));
        ZFLISTENER_LOCAL(retryButtonOnClick, {
            ZF2048AppDialog *dialog = userData->tagGet<ZFObjectHolder *>(zfText("dialog"))->holdedObj;
            ZF2048Game *game = userData->tagGet<ZFObjectHolder *>(zfText("game"))->holdedObj;
            dialog->dialogHide();
            game->gameReset(game->gameDataWidth(), game->gameDataHeight());
            game->gameFocus();
        })
        retryButton->observerAdd(ZFUIButton::EventButtonOnClick(), retryButtonOnClick, userData);

        zfblockedAlloc(ZF2048AppButton, cancelButton);
        container->childAdd(cancelButton);
        zfLangApplyZFUIButtonBasicTextNormal(cancelButton, zfText("ZF2048_gameOver_cancel"));
        ZFLISTENER_LOCAL(cancelButtonOnClick, {
            ZF2048AppDialog *dialog = userData->tagGet<ZFObjectHolder *>(zfText("dialog"))->holdedObj;
            dialog->dialogHide();

            ZF2048Game *game = userData->tagGet<ZFObjectHolder *>(zfText("game"))->holdedObj;
            game->gameFocus();
        })
        cancelButton->observerAdd(ZFUIButton::EventButtonOnClick(), cancelButtonOnClick, userData);

        hintDialog->dialogShow();
    }

public:
    void dataUpdate(void)
    {
        this->scoreLabel()->textContentStringSet(zfintToString(this->owner->game()->gameScore()));
        if(!this->owner->game()->gameCanMove())
        {
            this->gameOver();
        }
    }
    ZFLISTENER_DECLARE(dataOnChange)
    {
        this->dataUpdate();
    }

public:
    ZFLISTENER_DECLARE(dialogAfterHide)
    {
        this->owner->game()->gameFocus();
    }

    ZFLISTENER_DECLARE(autoMoveOnClick)
    {
        if(this->autoMoveButton()->buttonChecked())
        {
            this->autoMoveButton()->buttonCheckedSet(zffalse);
            this->autoMoveSettingDialog()->dialogShow();
        }
        else
        {
            this->autoMoveRunner()->runnerStop();
            this->owner->game()->gameFocus();
        }
    }
    ZFLISTENER_DECLARE(autoMoveOnStart)
    {
        this->autoMoveButton()->buttonCheckedSet(zftrue);
        this->autoMoveRunner()->actionList.removeAll();
        this->autoMoveRunner()->actionList.copyFrom(this->autoMoveSettingDialog()->autoMoves);
        this->autoMoveRunner()->runnerStart();
    }
    ZFLISTENER_DECLARE(autoMoveOnStop)
    {
        this->autoMoveButton()->buttonCheckedSet(zffalse);
    }
    ZFLISTENER_DECLARE(autoMoveStop)
    {
        this->autoMoveRunner()->runnerStop();
    }
    ZFLISTENER_DECLARE(settingOnChange)
    {
        ZF2048AppSettingDialog *settingDialog = listenerData.sender->toAny();
        this->owner->game()->gameReset(settingDialog->dataWidth, settingDialog->dataHeight);
    }
    ZFLISTENER_DECLARE(settingOnClick)
    {
        zfblockedAlloc(ZF2048AppSettingDialog, settingDialog, this->owner->game()->gameDataWidth(), this->owner->game()->gameDataHeight());
        settingDialog->observerAdd(ZF2048AppSettingDialog::EventSettingOnChange(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, settingOnChange)));
        settingDialog->observerAdd(ZF2048AppSettingDialog::EventDialogAfterHide(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, dialogAfterHide)));
        settingDialog->dialogShow();
    }
public:
    void setupChildren(void)
    {
        this->owner->childAdd(this->mainLayout());
        this->mainLayout()->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        this->mainLayout()->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

        this->mainLayout()->childAdd(this->headerLayout());
        this->headerLayout()->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

        this->headerLayout()->childAdd(this->autoMoveButton());
        this->autoMoveButton()->buttonCheckableSet(zftrue);
        this->autoMoveButton()->buttonBackgroundStyleChecked()->imageContentSet(this->autoMoveButton()->buttonBackgroundStyleNormal()->imageContent());

        this->headerLayout()->childAdd(this->undoButton());

        this->headerLayout()->childAdd(this->scoreLayoutHolder());
        this->scoreLayoutHolder()->layoutParam<ZFUILinearLayoutParam *>()->layoutWeightSet(1);

        this->scoreLayoutHolder()->childAdd(this->scoreLayout());
        this->scoreLayout()->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
        this->scoreLayout()->childAdd(this->scoreHint());

        this->scoreHint()->viewSizeMinSet(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeText()));
        this->scoreHint()->textAlignSet(ZFUIAlign::e_Center);

        this->scoreLayout()->childAdd(this->scoreLabel());
        this->scoreLabel()->viewSizeMinSet(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeText()));
        this->scoreLabel()->textAlignSet(ZFUIAlign::e_Center);

        this->headerLayout()->childAdd(this->settingButton());
        this->headerLayout()->childAdd(this->aboutButton());

        this->mainLayout()->childAdd(this->owner->game());
        this->owner->game()->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
    }
    void setupSetting_autoMove(void)
    {
        this->autoMoveButton()->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, autoMoveOnClick)));

        this->autoMoveSettingDialog()->observerAdd(ZF2048AppAutoMoveSettingDialog::EventAutoMoveSettingOnChange(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, autoMoveOnStart)));
        this->autoMoveSettingDialog()->observerAdd(ZF2048AppAutoMoveSettingDialog::EventDialogAfterHide(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, dialogAfterHide)));

        this->autoMoveRunner()->game = this->owner->game();
        this->autoMoveRunner()->observerAdd(ZF2048AppAutoMoveRunner::EventRunnerOnStop(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, autoMoveOnStop)));

        zfLangApplyZFUIButtonBasicTextNormal(this->autoMoveButton(), zfText("ZF2048_autoMove"));
        zfLangApplyZFUIButtonBasicTextChecked(this->autoMoveButton(), zfText("ZF2048_autoMoveStop"));
    }
    void setupSetting_undo(void)
    {
        ZFLISTENER_LOCAL(undoOnClick, {
            ZF2048App *owner = userData->to<ZFObjectHolder *>()->holdedObj;
            owner->game()->gameUndo();
            owner->game()->gameFocus();
        })
        this->undoButton()->observerAdd(ZFUIButton::EventButtonOnClick(), undoOnClick, this->owner->objectHolder());
        this->undoButton()->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, autoMoveStop)));

        zfLangApplyZFUIButtonBasicTextNormal(this->undoButton(), zfText("ZF2048_undo"));
    }
    void setupSetting_setting(void)
    {
        this->settingButton()->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, settingOnClick)));
        this->settingButton()->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, autoMoveStop)));

        zfLangApplyZFUIButtonBasicTextNormal(this->settingButton(), zfText("ZF2048_setting"));
    }
    void setupSetting_about(void)
    {
        ZFLISTENER_LOCAL(aboutOnClick, {
            _ZFP_ZF2048AppPrivate::showAbout();

            ZF2048App *owner = userData->to<ZFObjectHolder *>()->holdedObj;
            owner->game()->gameFocus();
        })
        this->aboutButton()->observerAdd(ZFUIButton::EventButtonOnClick(), aboutOnClick, this->owner->objectHolder());
        this->aboutButton()->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, autoMoveStop)));

        zfLangApplyZFUIButtonBasicTextNormal(this->aboutButton(), zfText("ZF2048_about"));
    }
public:
    static void showAbout(void)
    {
        ZFUIHintShow(zfLang(zfText("ZF2048_aboutContent")));
    }

public:
    zfoverride
    virtual ZFObject *objectOnInit(ZF_IN ZF2048App *owner)
    {
        this->objectOnInit();
        this->owner = owner;
        return this;
    }
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        return zfsuper::objectOnInit();
    }
    zfoverride
    virtual void objectOnInitFinish(void)
    {
        zfsuper::objectOnInitFinish();

        this->setupChildren();

        this->setupSetting_autoMove();
        this->setupSetting_undo();
        this->setupSetting_setting();
        this->setupSetting_about();
        zfLangApplyZFUITextViewText(this->scoreHint(), zfText("ZF2048_score"));

        this->owner->game()->observerAdd(ZF2048Game::EventGameDataOnChange(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, dataOnChange)));

        ZFObjectGlobalEventObserverAdd(ZFObject::ObserverAddParam()
                .eventIdSet(ZFUISysWindow::EventSysWindowOnRotate())
                .observerSet(ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, orientationOnChange)))
                .ownerSet(this)
            );

        ZFObjectGlobalEventObserverAdd(ZFUISysWindow::EventSysWindowOnPause(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, appPaused)));

        this->dataUpdate();
        this->orientationUpdate();
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        ZFObjectGlobalEventObserverRemoveByOwner(this);
        zfsuper::objectOnDealloc();
    }
};

ZFObject *ZF2048App::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAlloc(_ZFP_ZF2048AppPrivate, this);

    return this;
}
void ZF2048App::objectOnDealloc(void)
{
    ZFPropertyChange(d, zfnull);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

