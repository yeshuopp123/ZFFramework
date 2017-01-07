/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZF2048AppAutoMoveSettingDialog.h"
#include "ZF2048AppButton.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZF2048AppAutoMoveActionItem : zfextends ZF2048AppButton
{
    ZFOBJECT_DECLARE(_ZFP_ZF2048AppAutoMoveActionItem, ZF2048AppButton)

    ZFPROPERTY_ASSIGN(ZF2048AppAutoMoveActionEnum, actionValue)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZF2048AppAutoMoveActionEnum, actionValue)
    {
        this->actionValueSetInternal(newValue);
        zfLangApplyZFUIButtonBasicTextNormal(this,
            zfstringWithFormat(zfText("ZF2048AutoMove_%s"), ZF2048AppAutoMoveAction::EnumNameForValue(newValue)));
    }
};

zfclass _ZFP_ZF2048AppAutoMoveSettingDialogPrivate : zfextends ZFUILinearLayout
{
    ZFOBJECT_DECLARE(_ZFP_ZF2048AppAutoMoveSettingDialogPrivate, ZFUILinearLayout)

public:
    ZF2048AppAutoMoveSettingDialog *owner;

    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, hintText, ZFPropertyInitValue(zflineAlloc(ZFUITextView)))
    ZFPROPERTY_RETAIN_READONLY(ZFUIListView *, actionList, ZFPropertyInitValue(zflineAlloc(ZFUIListView)))
    ZFPROPERTY_RETAIN_READONLY(ZFUIListAdapterArray *, actionListAdapter, ZFPropertyInitValue(zflineAlloc(ZFUIListAdapterArray)))
    ZFPROPERTY_RETAIN_READONLY(ZFUILinearLayout *, addActionLayout, ZFPropertyInitValue(zflineAlloc(ZFUILinearLayout)))
    ZFPROPERTY_RETAIN_READONLY(_ZFP_ZF2048AppAutoMoveActionItem *, addAction_left, ZFPropertyInitValue(zflineAlloc(_ZFP_ZF2048AppAutoMoveActionItem)))
    ZFPROPERTY_RETAIN_READONLY(_ZFP_ZF2048AppAutoMoveActionItem *, addAction_top, ZFPropertyInitValue(zflineAlloc(_ZFP_ZF2048AppAutoMoveActionItem)))
    ZFPROPERTY_RETAIN_READONLY(_ZFP_ZF2048AppAutoMoveActionItem *, addAction_right, ZFPropertyInitValue(zflineAlloc(_ZFP_ZF2048AppAutoMoveActionItem)))
    ZFPROPERTY_RETAIN_READONLY(_ZFP_ZF2048AppAutoMoveActionItem *, addAction_bottom, ZFPropertyInitValue(zflineAlloc(_ZFP_ZF2048AppAutoMoveActionItem)))
    ZFPROPERTY_RETAIN_READONLY(_ZFP_ZF2048AppAutoMoveActionItem *, addAction_random, ZFPropertyInitValue(zflineAlloc(_ZFP_ZF2048AppAutoMoveActionItem)))
    ZFPROPERTY_RETAIN_READONLY(ZFUILinearLayout *, buttonLayout, ZFPropertyInitValue(zflineAlloc(ZFUILinearLayout)))
    ZFPROPERTY_RETAIN_READONLY(ZF2048AppButton *, confirmButton, ZFPropertyInitValue(zflineAlloc(ZF2048AppButton)))
    ZFPROPERTY_RETAIN_READONLY(ZF2048AppButton *, cancelButton, ZFPropertyInitValue(zflineAlloc(ZF2048AppButton)))

public:
    zfoverride
    virtual ZFObject *objectOnInit(ZF_IN ZF2048AppAutoMoveSettingDialog *owner)
    {
        this->objectOnInit();
        this->owner = owner;
        return this;
    }
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        return this;
    }
    zfoverride
    virtual void objectOnInitFinish(void)
    {
        zfsuper::objectOnInitFinish();

        this->layoutOrientationSet(ZFUIOrientation::e_Top);
        this->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

        this->childAdd(this->hintText());
        this->hintText()->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner);
        zfLangApplyZFUITextViewText(this->hintText(), zfText("ZF2048AutoMoveDialog_hint"));

        this->childAdd(this->actionList());
        this->actionList()->cellSizeHintSet(-1);
        this->actionList()->viewSizeMinSet(ZFUISizeMake(0, 120));
        this->actionList()->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        this->actionList()->layoutParam()->sizeHintSet(ZFUISizeMake(200, 150));

        this->actionList()->listAdapterSet(this->actionListAdapter());
        this->actionList()->cellUpdater()->add(zflineAlloc(ZFUIListCellUpdaterBasic));

        this->childAdd(this->addActionLayout());
        this->addActionLayout()->layoutOrientationSet(ZFUIOrientation::e_Left);
        this->addActionLayout()->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());
        this->addActionLayout()->childAdd(this->addAction_left());
        this->addActionLayout()->childAdd(this->addAction_top());
        this->addActionLayout()->childAdd(this->addAction_right());
        this->addActionLayout()->childAdd(this->addAction_bottom());
        this->addActionLayout()->childAdd(this->addAction_random());

        this->childAdd(this->buttonLayout());
        this->buttonLayout()->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());
        this->buttonLayout()->layoutOrientationSet(ZFUIOrientation::e_Left);
        this->buttonLayout()->childAdd(this->confirmButton());
        zfLangApplyZFUIButtonBasicTextNormal(this->confirmButton(), zfText("ZF2048AutoMoveDialog_confirm"));
        this->buttonLayout()->childAdd(this->cancelButton());
        zfLangApplyZFUIButtonBasicTextNormal(this->cancelButton(), zfText("ZF2048AutoMoveDialog_cancel"));

        this->addActionButtonSetup();
        this->buttonSetup();
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfsuper::objectOnDealloc();
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(addActionOnClick)
    {
        ZF2048AppAutoMoveSettingDialog *owner = userData->to<ZFObjectHolder *>()->holdedObj;
        _ZFP_ZF2048AppAutoMoveActionItem *clicked = listenerData.sender->toAny();

        owner->autoMoves.add(clicked->actionValue());
        zfblockedAlloc(_ZFP_ZF2048AppAutoMoveActionItem, actionItem);
        zfblockedAlloc(ZFUIListCell, cell);
        cell->cellViewSet(actionItem);
        owner->d->actionListAdapter()->cellAdd(cell);
        actionItem->actionValueSet(clicked->actionValue());
        actionItem->observerAdd(ZFUIButtonBasic::EventButtonOnClick(), ZFCallbackForRawFunction(zfself::actionItemOnClick), owner->objectHolder());
        owner->d->actionList()->listReload();

        owner->d->confirmButton()->buttonEnableSet(owner->d->actionListAdapter()->cellCount() > 0);
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(actionItemOnClick)
    {
        ZF2048AppAutoMoveSettingDialog *owner = userData->to<ZFObjectHolder *>()->holdedObj;
        _ZFP_ZF2048AppAutoMoveActionItem *actionItem = listenerData.sender->toAny();

        zfindex index = owner->d->actionListAdapter()->cellArray()->find(actionItem->viewParent(), ZFComparerCheckEqualOnly);
        zfCoreAssert(index != zfindexMax);
        owner->autoMoves.remove(index);
        owner->d->actionListAdapter()->cellRemove(index);
        owner->d->actionList()->listReload();

        owner->d->confirmButton()->buttonEnableSet(owner->d->actionListAdapter()->cellCount() > 0);
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(confirmButtonOnClick)
    {
        ZF2048AppAutoMoveSettingDialog *owner = userData->to<ZFObjectHolder *>()->holdedObj;
        owner->autoMoveSettingOnChange();
        owner->dialogHide();
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(cancelButtonOnClick)
    {
        ZF2048AppAutoMoveSettingDialog *owner = userData->to<ZFObjectHolder *>()->holdedObj;
        owner->dialogHide();
    }
private:
    void addActionButtonSetup(void)
    {
        ZFListener onClickListener = ZFCallbackForRawFunction(zfself::addActionOnClick);
        ZFObjectHolder *ownerHolder = this->owner->objectHolder();
        this->addAction_left()->actionValueSet(ZF2048AppAutoMoveAction::e_Left);
        this->addAction_left()->observerAdd(ZFUIButton::EventButtonOnClick(), onClickListener, ownerHolder);
        this->addAction_top()->actionValueSet(ZF2048AppAutoMoveAction::e_Top);
        this->addAction_top()->observerAdd(ZFUIButton::EventButtonOnClick(), onClickListener, ownerHolder);
        this->addAction_right()->actionValueSet(ZF2048AppAutoMoveAction::e_Right);
        this->addAction_right()->observerAdd(ZFUIButton::EventButtonOnClick(), onClickListener, ownerHolder);
        this->addAction_bottom()->actionValueSet(ZF2048AppAutoMoveAction::e_Bottom);
        this->addAction_bottom()->observerAdd(ZFUIButton::EventButtonOnClick(), onClickListener, ownerHolder);
        this->addAction_random()->actionValueSet(ZF2048AppAutoMoveAction::e_Random);
        this->addAction_random()->observerAdd(ZFUIButton::EventButtonOnClick(), onClickListener, ownerHolder);
    }
    void buttonSetup(void)
    {
        this->confirmButton()->buttonEnableSet(zffalse);
        this->confirmButton()->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForRawFunction(zfself::confirmButtonOnClick), this->owner->objectHolder());
        this->cancelButton()->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForRawFunction(zfself::cancelButtonOnClick), this->owner->objectHolder());
    }
};

ZFOBJECT_REGISTER(ZF2048AppAutoMoveSettingDialog)

ZFOBSERVER_EVENT_REGISTER(ZF2048AppAutoMoveSettingDialog, AutoMoveSettingOnChange)

ZFObject *ZF2048AppAutoMoveSettingDialog::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAlloc(_ZFP_ZF2048AppAutoMoveSettingDialogPrivate, this);
    this->dialogContentSet(d);
    zfLangApplyProperty_zfstring(this, ZFPropertyAccess(zfself, dialogTitle), zfText("ZF2048_autoMove"));

    d->addAction_left()->buttonSimulateClick();
    d->addAction_top()->buttonSimulateClick();
    d->addAction_left()->buttonSimulateClick();
    d->addAction_bottom()->buttonSimulateClick();

    return this;
}
void ZF2048AppAutoMoveSettingDialog::objectOnDealloc(void)
{
    ZFPropertyChange(d, zfnull);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

