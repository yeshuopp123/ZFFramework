/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZF2048AppSettingDialog.h"
#include "ZF2048AppButton.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZF2048AppDataWidthMin ((zfindex)2)
#define _ZFP_ZF2048AppDataWidthMax ((zfindex)10)

zfclass _ZFP_ZF2048AppSettingDialogPrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZF2048AppSettingDialogPrivate, ZFObject)

public:
    ZF2048AppSettingDialog *owner;

public:
    ZFPROPERTY_RETAIN(ZFUIView *, contentView)

public:
    ZFUITextView *dataWidthHint;
    ZFUIButtonBasic *dataWidthIncrease;
    ZFUITextEdit *dataWidthEdit;
    ZFUIButtonBasic *dataWidthDecrease;

    ZFUITextView *dataHeightHint;
    ZFUIButtonBasic *dataHeightIncrease;
    ZFUITextEdit *dataHeightEdit;
    ZFUIButtonBasic *dataHeightDecrease;

    ZFUIButtonBasic *confirmButton;
    ZFUIButtonBasic *cancelButton;

public:
    static zfindex readData(ZF_IN ZFUITextEdit *edit)
    {
        zfindex ret = 4;
        ZFString *value = edit->textContent();
        if(value != zfnull)
        {
            zfindexFromString(ret, value->stringValue());
        }
        if(ret < _ZFP_ZF2048AppDataWidthMin || ret > _ZFP_ZF2048AppDataWidthMax)
        {
            ret = 4;
        }
        return ret;
    }

public:
    zfoverride
    virtual ZFObject *objectOnInit(ZF_IN ZF2048AppSettingDialog *owner)
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

        this->loadContent();
        this->prepareLogic();

        zfLangApplyProperty_zfstring(this->owner, ZFPropertyAccess(ZF2048AppDialog, dialogTitle), zfText("ZF2048SettingDialog_title"));

        zfLangApplyZFUITextViewText(this->dataWidthHint, zfText("ZF2048SettingDialog_widthHint"));
        zfLangApplyZFUITextViewText(this->dataHeightHint, zfText("ZF2048SettingDialog_heightHint"));

        zfLangApplyZFUIButtonBasicTextNormal(this->dataWidthIncrease, zfText("ZF2048SettingDialog_dataIncrease"));
        zfLangApplyZFUIButtonBasicTextNormal(this->dataWidthDecrease, zfText("ZF2048SettingDialog_dataDecrease"));

        zfLangApplyZFUIButtonBasicTextNormal(this->dataHeightIncrease, zfText("ZF2048SettingDialog_dataIncrease"));
        zfLangApplyZFUIButtonBasicTextNormal(this->dataHeightDecrease, zfText("ZF2048SettingDialog_dataDecrease"));

        zfLangApplyZFUIButtonBasicTextNormal(this->confirmButton, zfText("ZF2048SettingDialog_confirm"));
        zfLangApplyZFUIButtonBasicTextNormal(this->cancelButton, zfText("ZF2048SettingDialog_cancel"));
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        ZFObjectGlobalEventObserverRemoveByOwner(this);
        zfsuper::objectOnDealloc();
    }

private:
    void loadContent(void)
    {
        if(zftrue)
        { // load from xml
            ZFInputCallback input = ZFInputCallbackForResFile(zfText("ZF2048/app/settingdialog.xml"));
            ZFSerializableData data = ZFXmlParse(input);
            zfautoObject obj = ZFObjectFromSerializableData(data);
            this->contentViewSet(obj.toAny());
            zfCoreAssert(this->contentView() != zfnull);
        }
        else
        { // load by code
            zfblockedAlloc(ZFUILinearLayout, mainView);
            this->contentViewSet(mainView);
            mainView->layoutOrientationSet(ZFUIOrientation::e_Top);
            mainView->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

            zfblockedAlloc(ZFUILinearLayout, settingContainer);
            mainView->childAdd(settingContainer);
            settingContainer->layoutOrientationSet(ZFUIOrientation::e_Left);
            settingContainer->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

            {
                zfblockedAlloc(ZFUITextView, dataWidthHintTmp);
                settingContainer->childAdd(dataWidthHintTmp);
                dataWidthHintTmp->viewIdSet(zfText("dataWidthHint"));

                zfblockedAlloc(ZFUILinearLayout, dataWidthContainerTmp);
                settingContainer->childAdd(dataWidthContainerTmp);
                dataWidthContainerTmp->layoutOrientationSet(ZFUIOrientation::e_Top);

                zfblockedAlloc(ZF2048AppButton, dataWidthIncreaseTmp);
                dataWidthContainerTmp->childAdd(dataWidthIncreaseTmp);
                dataWidthIncreaseTmp->viewIdSet(zfText("dataWidthIncrease"));

                zfblockedAlloc(ZFUITextEditWidget, dataWidthEditTmp);
                dataWidthContainerTmp->childAdd(dataWidthEditTmp);
                dataWidthEditTmp->viewIdSet(zfText("dataWidthEdit"));

                zfblockedAlloc(ZF2048AppButton, dataWidthDecreaseTmp);
                dataWidthContainerTmp->childAdd(dataWidthDecreaseTmp);
                dataWidthDecreaseTmp->viewIdSet(zfText("dataWidthDecrease"));
            }
            {
                zfblockedAlloc(ZFUITextView, dataHeightHintTmp);
                settingContainer->childAdd(dataHeightHintTmp);
                dataHeightHintTmp->viewIdSet(zfText("dataHeightHint"));

                zfblockedAlloc(ZFUILinearLayout, dataHeightContainerTmp);
                settingContainer->childAdd(dataHeightContainerTmp);
                dataHeightContainerTmp->layoutOrientationSet(ZFUIOrientation::e_Top);

                zfblockedAlloc(ZF2048AppButton, dataHeightIncreaseTmp);
                dataHeightContainerTmp->childAdd(dataHeightIncreaseTmp);
                dataHeightIncreaseTmp->viewIdSet(zfText("dataHeightIncrease"));

                zfblockedAlloc(ZFUITextEditWidget, dataHeightEditTmp);
                dataHeightContainerTmp->childAdd(dataHeightEditTmp);
                dataHeightEditTmp->viewIdSet(zfText("dataHeightEdit"));

                zfblockedAlloc(ZF2048AppButton, dataHeightDecreaseTmp);
                dataHeightContainerTmp->childAdd(dataHeightDecreaseTmp);
                dataHeightDecreaseTmp->viewIdSet(zfText("dataHeightDecrease"));
            }

            zfblockedAlloc(ZFUILinearLayout, buttonLayout);
            mainView->childAdd(buttonLayout);
            buttonLayout->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

            zfblockedAlloc(ZF2048AppButton, confirmButtonTmp);
            buttonLayout->childAdd(confirmButtonTmp);
            confirmButtonTmp->viewIdSet(zfText("confirmButton"));

            zfblockedAlloc(ZF2048AppButton, cancelButtonTmp);
            buttonLayout->childAdd(cancelButtonTmp);
            cancelButtonTmp->viewIdSet(zfText("cancelButton"));
        }

        this->dataWidthHint = this->contentView()->childFindById(zfText("dataWidthHint"))->toAny();
        this->dataWidthIncrease = this->contentView()->childFindById(zfText("dataWidthIncrease"))->toAny();
        this->dataWidthEdit = this->contentView()->childFindById(zfText("dataWidthEdit"))->toAny();
        this->dataWidthDecrease = this->contentView()->childFindById(zfText("dataWidthDecrease"))->toAny();

        this->dataHeightHint = this->contentView()->childFindById(zfText("dataHeightHint"))->toAny();
        this->dataHeightIncrease = this->contentView()->childFindById(zfText("dataHeightIncrease"))->toAny();
        this->dataHeightEdit = this->contentView()->childFindById(zfText("dataHeightEdit"))->toAny();
        this->dataHeightDecrease = this->contentView()->childFindById(zfText("dataHeightDecrease"))->toAny();

        this->confirmButton = this->contentView()->childFindById(zfText("confirmButton"))->toAny();
        this->cancelButton = this->contentView()->childFindById(zfText("cancelButton"))->toAny();
    }
    void prepareLogic(void)
    {
        ZFLISTENER_LOCAL(increaseOnClick, {
            ZFUIButtonBasic *increase = userData->tagGet<ZFObjectHolder *>(zfText("increase"))->holdedObj;
            ZFUITextEdit *edit = userData->tagGet<ZFObjectHolder *>(zfText("edit"))->holdedObj;
            ZFUIButtonBasic *decrease = userData->tagGet<ZFObjectHolder *>(zfText("decrease"))->holdedObj;
            zfindex value = _ZFP_ZF2048AppSettingDialogPrivate::readData(edit);
            ++value;
            value = zfmApplyRange<zfindex>(value, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
            edit->textContentStringSet(zfindexToString(value));
            increase->buttonEnableSet(value != _ZFP_ZF2048AppDataWidthMax);
            decrease->buttonEnableSet(value != _ZFP_ZF2048AppDataWidthMin);
        })
        ZFLISTENER_LOCAL(decreaseOnClick, {
            ZFUIButtonBasic *increase = userData->tagGet<ZFObjectHolder *>(zfText("increase"))->holdedObj;
            ZFUITextEdit *edit = userData->tagGet<ZFObjectHolder *>(zfText("edit"))->holdedObj;
            ZFUIButtonBasic *decrease = userData->tagGet<ZFObjectHolder *>(zfText("decrease"))->holdedObj;
            zfindex value = _ZFP_ZF2048AppSettingDialogPrivate::readData(edit);
            --value;
            value = zfmApplyRange<zfindex>(value, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
            edit->textContentStringSet(zfindexToString(value));
            increase->buttonEnableSet(value != _ZFP_ZF2048AppDataWidthMax);
            decrease->buttonEnableSet(value != _ZFP_ZF2048AppDataWidthMin);
        })
        ZFLISTENER_LOCAL(editOnChange, {
            ZFUIButtonBasic *increase = userData->tagGet<ZFObjectHolder *>(zfText("increase"))->holdedObj;
            ZFUITextEdit *edit = userData->tagGet<ZFObjectHolder *>(zfText("edit"))->holdedObj;
            ZFUIButtonBasic *decrease = userData->tagGet<ZFObjectHolder *>(zfText("decrease"))->holdedObj;
            zfindex value = _ZFP_ZF2048AppSettingDialogPrivate::readData(edit);
            value = zfmApplyRange<zfindex>(value, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
            increase->buttonEnableSet(value != _ZFP_ZF2048AppDataWidthMax);
            decrease->buttonEnableSet(value != _ZFP_ZF2048AppDataWidthMin);
        })
        ZFLISTENER_LOCAL(editOnClickReturn, {
            ZFUITextEdit *textEdit = listenerData.sender->toAny();
            textEdit->viewFocusRequest(zffalse);
        })

        zfblockedAlloc(ZFObject, userDataForWidth);
        userDataForWidth->tagSet(zfText("increase"), this->dataWidthIncrease->objectHolder());
        userDataForWidth->tagSet(zfText("edit"), this->dataWidthEdit->objectHolder());
        userDataForWidth->tagSet(zfText("decrease"), this->dataWidthDecrease->objectHolder());

        zfblockedAlloc(ZFObject, userDataForHeight);
        userDataForHeight->tagSet(zfText("increase"), this->dataHeightIncrease->objectHolder());
        userDataForHeight->tagSet(zfText("edit"), this->dataHeightEdit->objectHolder());
        userDataForHeight->tagSet(zfText("decrease"), this->dataHeightDecrease->objectHolder());

        zfblockedAlloc(ZFRegExp, textFilter);
        textFilter->regExpCompile(zfstringWithFormat(zfText("\\d{1,2}")));
        this->dataWidthEdit->textEditFilterSet(textFilter);
        this->dataHeightEdit->textEditFilterSet(textFilter);

        this->dataWidthIncrease->observerAdd(ZFUIButton::EventButtonOnClick(), increaseOnClick, userDataForWidth);
        this->dataWidthEdit->observerAdd(ZFUITextEdit::EventTextEditOnChange(), editOnChange, userDataForWidth);
        this->dataWidthEdit->observerAdd(ZFUITextEdit::EventTextEditReturnOnClick(), editOnClickReturn, userDataForWidth);
        this->dataWidthDecrease->observerAdd(ZFUIButton::EventButtonOnClick(), decreaseOnClick, userDataForWidth);

        this->dataHeightIncrease->observerAdd(ZFUIButton::EventButtonOnClick(), increaseOnClick, userDataForHeight);
        this->dataHeightEdit->observerAdd(ZFUITextEdit::EventTextEditOnChange(), editOnChange, userDataForHeight);
        this->dataHeightEdit->observerAdd(ZFUITextEdit::EventTextEditReturnOnClick(), editOnClickReturn, userDataForHeight);
        this->dataHeightDecrease->observerAdd(ZFUIButton::EventButtonOnClick(), decreaseOnClick, userDataForHeight);

        this->confirmButton->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, confirmOnClick)));
        this->cancelButton->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, cancelOnClick)));
    }
    ZFLISTENER_DECLARE(confirmOnClick)
    {
        this->owner->dataWidth = zfself::readData(this->dataWidthEdit);
        this->owner->dataHeight = zfself::readData(this->dataHeightEdit);
        this->owner->settingOnChange();
        this->owner->dialogHide();
    }
    ZFLISTENER_DECLARE(cancelOnClick)
    {
        this->owner->dialogHide();
    }
};

ZFOBJECT_REGISTER(ZF2048AppSettingDialog)

ZFOBSERVER_EVENT_REGISTER(ZF2048AppSettingDialog, SettingOnChange)

ZFObject *ZF2048AppSettingDialog::objectOnInit(ZF_IN zfindex dataWidth,
                                               ZF_IN zfindex dataHeight)
{
    this->objectOnInit();
    this->dataWidth = zfmApplyRange(dataWidth, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
    this->dataHeight = zfmApplyRange(dataHeight, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
    return this;
}
ZFObject *ZF2048AppSettingDialog::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAlloc(_ZFP_ZF2048AppSettingDialogPrivate, this);
    this->dataWidth = 4;
    this->dataHeight = 4;
    this->dialogContentSet(d->contentView());
    return this;
}
void ZF2048AppSettingDialog::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    d->dataWidthEdit->textContentStringSet(zfindexToString(this->dataWidth));
    d->dataHeightEdit->textContentStringSet(zfindexToString(this->dataHeight));
}
void ZF2048AppSettingDialog::objectOnDealloc(void)
{
    ZFPropertyChange(d, zfnull);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

