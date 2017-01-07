/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZF2048AppDialog.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZF2048AppDialogPrivate
{
public:
    ZFUILinearLayout *dialogContainer;
    ZFUITextView *dialogTitleView;

public:
    _ZFP_ZF2048AppDialogPrivate(void)
    : dialogContainer(zfnull)
    , dialogTitleView(zfnull)
    {
    }
};

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZF2048AppDialog, zfstring, dialogTitle)
{
    this->dialogTitleSetInternal(newValue);
    d->dialogTitleView->textContentStringSet(newValue);
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZF2048AppDialog, ZFUIView *, dialogContent)
{
    if(this->dialogContent() != zfnull)
    {
        this->dialogContent()->viewRemoveFromParent();
    }
    this->dialogContentSetInternal(newValue);
    if(this->dialogContent() != zfnull)
    {
        d->dialogContainer->childAdd(this->dialogContent());
    }
}

ZFObject *ZF2048AppDialog::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfnew(_ZFP_ZF2048AppDialogPrivate);

    d->dialogContainer = zfAlloc(ZFUILinearLayout);
    this->dialogInternalContainer()->childAdd(d->dialogContainer);
    d->dialogContainer->layoutOrientationSet(ZFUIOrientation::e_Top);
    d->dialogContainer->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());
    d->dialogContainer->viewSizeMinSet(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeDialog()));

    d->dialogTitleView = zfAlloc(ZFUITextView);
    d->dialogContainer->childAdd(d->dialogTitleView);
    d->dialogTitleView->viewSizeMinSet(ZFUISizeMake(0, ZFUIGlobalStyle::DefaultStyle()->itemSizeDialog()));
    d->dialogTitleView->textAlignSet(ZFUIAlign::e_Center);

    return this;
}
void ZF2048AppDialog::objectOnDealloc(void)
{
    ZFPropertyChange(d->dialogContainer, zfnull);
    ZFPropertyChange(d->dialogTitleView, zfnull);

    zfdelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

