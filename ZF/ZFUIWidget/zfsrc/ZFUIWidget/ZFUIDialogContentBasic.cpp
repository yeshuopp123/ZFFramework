/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIDialogContentBasic.h"
#include "ZFUIDialogContentBasic_Buttons.h"
#include "ZFUILinearLayout.h"
#include "ZFUIViewLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIDialogContentBasic)

// ============================================================
ZFOBJECT_REGISTER(ZFUIDialogContentBasic)

// ============================================================
zfclassNotPOD _ZFP_ZFUIDialogContentBasicPrivate
{
public:
    ZFUILinearLayout *mainContainer;

    ZFUIViewLayout *dialogTitleContainer;
    ZFUIViewLayout *dialogContentContainer;
    ZFUIViewLayout *dialogButtonContainer;

    // button order (left to right):
    // Normal(s), Destructive, Cancel, No, Yes
    zfindex dialogButtonInternalCount;
    ZFUILinearLayout *dialogButtonLayout;
    ZFUIDialogContentBasicButtonYes *dialogButton_Yes;
    ZFUIDialogContentBasicButtonNo *dialogButton_No;
    ZFUIDialogContentBasicButtonCancel *dialogButton_Cancel;
    ZFUIDialogContentBasicButtonDestructive *dialogButton_Destructive;
public:
    _ZFP_ZFUIDialogContentBasicPrivate(void)
    : mainContainer(zfnull)
    , dialogTitleContainer(zfnull)
    , dialogContentContainer(zfnull)
    , dialogButtonContainer(zfnull)
    , dialogButtonInternalCount(0)
    , dialogButtonLayout(zfnull)
    , dialogButton_Yes(zfnull)
    , dialogButton_No(zfnull)
    , dialogButton_Cancel(zfnull)
    , dialogButton_Destructive(zfnull)
    {
    }
};

// ============================================================
// title
ZFUIView *ZFUIDialogContentBasic::dialogTitleContainer(void)
{
    return d->dialogTitleContainer;
}
ZFUITextView *ZFUIDialogContentBasic::dialogTitleView(void)
{
    return this->titleTextView();
}

// ============================================================
// content
ZFUIView *ZFUIDialogContentBasic::dialogContentContainer(void)
{
    return d->dialogContentContainer;
}
ZFUITextView *ZFUIDialogContentBasic::dialogContentView(void)
{
    return this->contentTextView();
}

// ============================================================
// button
ZFUIView *ZFUIDialogContentBasic::dialogButtonContainer(void)
{
    return d->dialogButtonContainer;
}
ZFUIButton *ZFUIDialogContentBasic::dialogButton(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                                 ZF_IN_OPT zfbool autoCreateIfNotExist /* = zftrue */)
{
    switch(dialogButtonType)
    {
        case ZFUIDialogButtonType::e_Normal:
            return zfnull;
        case ZFUIDialogButtonType::e_Yes:
            if(autoCreateIfNotExist && d->dialogButton_Yes == zfnull)
            {
                ++(d->dialogButtonInternalCount);
                d->dialogButton_Yes = zfAllocWithoutLeakTest(ZFUIDialogContentBasicButtonYes);
                zfindex index = 0;
                d->dialogButtonLayout->childAdd(d->dialogButton_Yes, zfnull, index);
                this->dialogButtonOnAdd(d->dialogButton_Yes);
            }
            return d->dialogButton_Yes;
        case ZFUIDialogButtonType::e_No:
            if(autoCreateIfNotExist && d->dialogButton_No == zfnull)
            {
                ++(d->dialogButtonInternalCount);
                d->dialogButton_No = zfAllocWithoutLeakTest(ZFUIDialogContentBasicButtonNo);
                zfindex index = 0;
                if(d->dialogButton_Yes != zfnull) {++index;}
                d->dialogButtonLayout->childAdd(d->dialogButton_No, zfnull, index);
                this->dialogButtonOnAdd(d->dialogButton_No);
            }
            return d->dialogButton_No;
        case ZFUIDialogButtonType::e_Cancel:
            if(autoCreateIfNotExist && d->dialogButton_Cancel == zfnull)
            {
                ++(d->dialogButtonInternalCount);
                d->dialogButton_Cancel = zfAllocWithoutLeakTest(ZFUIDialogContentBasicButtonCancel);
                zfindex index = 0;
                if(d->dialogButton_Yes != zfnull) {++index;}
                if(d->dialogButton_No != zfnull) {++index;}
                d->dialogButtonLayout->childAdd(d->dialogButton_Cancel, zfnull, index);
                this->dialogButtonOnAdd(d->dialogButton_Cancel);
            }
            return d->dialogButton_Cancel;
        case ZFUIDialogButtonType::e_Destructive:
            if(autoCreateIfNotExist && d->dialogButton_Destructive == zfnull)
            {
                ++(d->dialogButtonInternalCount);
                d->dialogButton_Destructive = zfAllocWithoutLeakTest(ZFUIDialogContentBasicButtonDestructive);
                zfindex index = 0;
                if(d->dialogButton_Yes != zfnull) {++index;}
                if(d->dialogButton_No != zfnull) {++index;}
                if(d->dialogButton_Cancel != zfnull) {++index;}
                d->dialogButtonLayout->childAdd(d->dialogButton_Destructive, zfnull, index);
                this->dialogButtonOnAdd(d->dialogButton_Destructive);
            }
            return d->dialogButton_Destructive;
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}
const zfchar *ZFUIDialogContentBasic::dialogButtonText(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType)
{
    ZFUIButtonBasic *button = ZFCastZFObjectUnchecked(ZFUIButtonBasic *, this->dialogButton(dialogButtonType, zffalse));
    return ((button != zfnull) ? button->buttonLabelTextString() : zfnull);
}
void ZFUIDialogContentBasic::dialogButtonTextSet(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                                 ZF_IN const zfchar *text)
{
    this->dialogButton(dialogButtonType)->to<ZFUIButtonBasic *>()->buttonLabelTextStringSet(text);
}
void ZFUIDialogContentBasic::dialogButtonRemove(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType)
{
    switch(dialogButtonType)
    {
        case ZFUIDialogButtonType::e_Normal:
            break;
        case ZFUIDialogButtonType::e_Yes:
            if(d->dialogButton_Yes != zfnull)
            {
                this->dialogButtonOnRemove(d->dialogButton_Yes);
                --(d->dialogButtonInternalCount);
                zfindex index = 0;
                d->dialogButtonLayout->childRemoveAtIndex(index);
                zfReleaseWithoutLeakTest(d->dialogButton_Yes);
                d->dialogButton_Yes = zfnull;
            }
            break;
        case ZFUIDialogButtonType::e_No:
            if(d->dialogButton_No != zfnull)
            {
                this->dialogButtonOnRemove(d->dialogButton_No);
                --(d->dialogButtonInternalCount);
                zfindex index = 0;
                if(d->dialogButton_Yes != zfnull) {++index;}
                d->dialogButtonLayout->childRemoveAtIndex(index);
                zfReleaseWithoutLeakTest(d->dialogButton_No);
                d->dialogButton_No = zfnull;
            }
            break;
        case ZFUIDialogButtonType::e_Cancel:
            if(d->dialogButton_Cancel != zfnull)
            {
                this->dialogButtonOnRemove(d->dialogButton_Cancel);
                --(d->dialogButtonInternalCount);
                zfindex index = 0;
                if(d->dialogButton_Yes != zfnull) {++index;}
                if(d->dialogButton_No != zfnull) {++index;}
                d->dialogButtonLayout->childRemoveAtIndex(index);
                zfReleaseWithoutLeakTest(d->dialogButton_Cancel);
                d->dialogButton_Cancel = zfnull;
            }
            break;
        case ZFUIDialogButtonType::e_Destructive:
            if(d->dialogButton_Destructive != zfnull)
            {
                this->dialogButtonOnRemove(d->dialogButton_Destructive);
                --(d->dialogButtonInternalCount);
                zfindex index = 0;
                if(d->dialogButton_Yes != zfnull) {++index;}
                if(d->dialogButton_No != zfnull) {++index;}
                if(d->dialogButton_Cancel != zfnull) {++index;}
                d->dialogButtonLayout->childRemoveAtIndex(index);
                zfReleaseWithoutLeakTest(d->dialogButton_Destructive);
                d->dialogButton_Destructive = zfnull;
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

// ============================================================
// button
zfindex ZFUIDialogContentBasic::dialogButtonCount(void)
{
    return (d->dialogButtonLayout->childCount() - d->dialogButtonInternalCount);
}
ZFUIButton *ZFUIDialogContentBasic::dialogButtonAtIndex(ZF_IN zfindex index)
{
    if(index >= this->dialogButtonCount())
    {
        zfCoreCriticalIndexOutOfRange(index, this->dialogButtonCount());
        return zfnull;
    }
    return d->dialogButtonLayout->childAtIndex(d->dialogButtonInternalCount + index)->to<ZFUIButton *>();
}
zfindex ZFUIDialogContentBasic::dialogButtonFind(ZF_IN ZFUIButton *dialogButton)
{
    zfindex ret = d->dialogButtonLayout->childFind(dialogButton);
    if(ret == zfindexMax || ret < d->dialogButtonInternalCount)
    {
        return zfindexMax;
    }
    return (ret - d->dialogButtonInternalCount);
}
void ZFUIDialogContentBasic::dialogButtonAdd(ZF_IN ZFUIButton *button,
                                             ZF_IN_OPT zfindex atIndex /* = zfindexMax */)
{
    if(atIndex == zfindexMax)
    {
        atIndex = this->dialogButtonCount();
    }
    else if(atIndex > this->dialogButtonCount())
    {
        zfCoreCriticalIndexOutOfRange(atIndex, this->dialogButtonCount() + 1);
        return ;
    }
    d->dialogButtonLayout->childAdd(button, zfnull, atIndex + d->dialogButtonInternalCount);
    this->dialogButtonOnAdd(button);
}
void ZFUIDialogContentBasic::dialogButtonRemove(ZF_IN ZFUIButton *button)
{
    this->dialogButtonRemoveAtIndex(this->dialogButtonFind(button));
}
void ZFUIDialogContentBasic::dialogButtonRemoveAtIndex(ZF_IN zfindex index)
{
    if(index == zfindexMax)
    {
        return ;
    }
    else if(index >= this->dialogButtonCount())
    {
        zfCoreCriticalIndexOutOfRange(index, this->dialogButtonCount());
        return ;
    }
    this->dialogButtonOnRemove(d->dialogButtonLayout->childAtIndex(d->dialogButtonInternalCount + index)->to<ZFUIButton *>());
    d->dialogButtonLayout->childRemoveAtIndex(d->dialogButtonInternalCount + index);
}
void ZFUIDialogContentBasic::dialogButtonRemoveAll(void)
{
    while(d->dialogButtonLayout->childCount() > d->dialogButtonInternalCount)
    {
        this->dialogButtonOnRemove(d->dialogButtonLayout->childAtIndex(d->dialogButtonLayout->childCount() - 1)->to<ZFUIButton *>());
        d->dialogButtonLayout->childRemoveAtIndex(d->dialogButtonLayout->childCount() - 1);
    }
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogContentBasicDataHolder, ZFLevelZFFrameworkNormal)
{
    this->textChangeListener = ZFCallbackForRawFunction(zfself::textChange);
    this->containerChildChangeListener = ZFCallbackForRawFunction(zfself::containerChildChange);
}
public:
    ZFListener textChangeListener;
    ZFListener containerChildChangeListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(textChange)
    {
        const ZFProperty *property = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFProperty *>();
        if(property != ZFPropertyAccess(ZFUITextView, textContent))
        {
            return ;
        }
        ZFUITextView *textView = listenerData.sender->to<ZFUITextView *>();
        textView->viewVisibleSet(!textView->textContentIsEmpty());
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(containerChildChange)
    {
        ZFUIView *containerView = listenerData.sender->to<ZFUIView *>();
        containerView->viewVisibleSet(containerView->childCount() > 0);
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogContentBasicDataHolder)
ZFObject *ZFUIDialogContentBasic::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIDialogContentBasicPrivate);

    d->mainContainer = zfAllocWithoutLeakTest(ZFUILinearLayout);
    this->internalForegroundViewAdd(d->mainContainer);
    d->mainContainer->layoutOrientationSet(ZFUIOrientation::e_Top);
    d->mainContainer->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

    { // title
        ZFUITextView *titleTextView = this->titleTextView()->to<ZFUITextView *>();
        d->mainContainer->childAdd(titleTextView);
        titleTextView->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
        titleTextView->observerAdd(ZFObject::EventObjectPropertyValueAfterChange(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogContentBasicDataHolder)->textChangeListener);
        titleTextView->viewVisibleSet(!titleTextView->textContentIsEmpty());
        titleTextView->textColorSet(ZFUIGlobalStyle::DefaultStyle()->textColorSecondary());
        titleTextView->textSingleLineSet(zffalse);

        d->dialogTitleContainer = zfAllocWithoutLeakTest(ZFUIViewLayout);
        d->mainContainer->childAdd(d->dialogTitleContainer);
        d->dialogTitleContainer->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
        d->dialogTitleContainer->observerAdd(ZFUIView::EventViewChildOnChange(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogContentBasicDataHolder)->containerChildChangeListener);
    }

    { // content
        ZFUITextView *contentTextView = this->contentTextView()->to<ZFUITextView *>();
        d->mainContainer->childAdd(contentTextView);
        contentTextView->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner);
        contentTextView->observerAdd(ZFObject::EventObjectPropertyValueAfterChange(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogContentBasicDataHolder)->textChangeListener);
        contentTextView->viewVisibleSet(!contentTextView->textContentIsEmpty());

        d->dialogContentContainer = zfAllocWithoutLeakTest(ZFUIViewLayout);
        d->mainContainer->childAdd(d->dialogContentContainer);
        d->dialogContentContainer->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner);
        d->dialogContentContainer->layoutParam<ZFUILinearLayoutParam *>()->layoutWeightSet(1);
        d->dialogContentContainer->observerAdd(ZFUIView::EventViewChildOnChange(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogContentBasicDataHolder)->containerChildChangeListener);
    }

    { // button
        d->dialogButtonContainer = zfAllocWithoutLeakTest(ZFUIViewLayout);
        d->mainContainer->childAdd(d->dialogButtonContainer);
        d->dialogButtonContainer->layoutParam()->layoutAlignSet(ZFUIAlign::e_RightInner);

        d->dialogButtonLayout = zfAllocWithoutLeakTest(ZFUILinearLayout);
        d->dialogButtonContainer->childAdd(d->dialogButtonLayout);
        d->dialogButtonLayout->layoutParam()->layoutAlignSet(ZFUIAlign::e_RightInner);
        d->dialogButtonLayout->layoutOrientationSet(ZFUIOrientation::e_Right);
        d->dialogButtonLayout->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());
    }

    return this;
}
void ZFUIDialogContentBasic::objectOnDealloc(void)
{
    zfCoreAssert(d->dialogButton_Yes == zfnull);
    zfCoreAssert(d->dialogButton_No == zfnull);
    zfCoreAssert(d->dialogButton_Cancel == zfnull);
    zfCoreAssert(d->dialogButton_Destructive == zfnull);

    zfReleaseWithoutLeakTest(d->dialogButtonLayout);
    zfReleaseWithoutLeakTest(d->dialogButtonContainer);
    zfReleaseWithoutLeakTest(d->dialogContentContainer);
    zfReleaseWithoutLeakTest(d->dialogTitleContainer);
    zfReleaseWithoutLeakTest(d->mainContainer);

    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}
void ZFUIDialogContentBasic::objectOnDeallocPrepare(void)
{
    this->dialogButtonRemove(ZFUIDialogButtonType::e_Yes);
    this->dialogButtonRemove(ZFUIDialogButtonType::e_No);
    this->dialogButtonRemove(ZFUIDialogButtonType::e_Cancel);
    this->dialogButtonRemove(ZFUIDialogButtonType::e_Destructive);
    zfsuper::objectOnDeallocPrepare();
}

void ZFUIDialogContentBasic::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                             ZF_IN const ZFUISize &sizeHint,
                                             ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = d->mainContainer->layoutMeasure(sizeHint, sizeParam);
}
void ZFUIDialogContentBasic::internalForegroundViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalForegroundViewOnLayout(bounds);
    d->mainContainer->layout(bounds);
}
zfbool ZFUIDialogContentBasic::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    if(!zfsuper::internalViewShouldLayout(internalView))
    {
        return zffalse;
    }
    if(internalView == d->mainContainer)
    {
        return zffalse;
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

