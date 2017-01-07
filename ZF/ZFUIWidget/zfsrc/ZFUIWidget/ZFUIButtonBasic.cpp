/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIButtonBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFUIButtonBasicPrivate
zfclassNotPOD _ZFP_ZFUIButtonBasicPrivate
{
public:
    ZFUIButtonBasic *pimplOwner;
    ZFUITextView *buttonLabel;
    ZFUIImageView *buttonIcon;
    ZFUIImageView *buttonBackground;

public:
    _ZFP_ZFUIButtonBasicPrivate(void)
    : pimplOwner(zfnull)
    , buttonLabel(zfnull)
    , buttonIcon(zfnull)
    , buttonBackground(zfnull)
    {
    }
    ~_ZFP_ZFUIButtonBasicPrivate(void)
    {
    }

public:
    void updateButtonLabel(void)
    {
        ZFString *value = this->pimplOwner->buttonLabelStyle(this->pimplOwner->buttonState())->textContent();
        if(value == zfnull)
        {
            if(this->pimplOwner->buttonState() == ZFUIButtonState::e_Checked)
            {
                value = this->pimplOwner->buttonLabelStyleHighlighted()->textContent();
            }
            else if(this->pimplOwner->buttonState() == ZFUIButtonState::e_CheckedHighlighted)
            {
                value = this->pimplOwner->buttonLabelStyleChecked()->textContent();
                if(value == zfnull)
                {
                    value = this->pimplOwner->buttonLabelStyleHighlighted()->textContent();
                }
            }
            if(value == zfnull)
            {
                value = this->pimplOwner->buttonLabelStyleNormal()->textContent();
            }
        }
        this->buttonLabel->textContentSet(value);
    }
    void updateButtonIcon(void)
    {
        ZFUIImage *value = this->pimplOwner->buttonIconStyle(this->pimplOwner->buttonState())->imageContent();
        if(value == zfnull)
        {
            if(this->pimplOwner->buttonState() == ZFUIButtonState::e_Checked)
            {
                value = this->pimplOwner->buttonIconStyleHighlighted()->imageContent();
            }
            else if(this->pimplOwner->buttonState() == ZFUIButtonState::e_CheckedHighlighted)
            {
                value = this->pimplOwner->buttonIconStyleChecked()->imageContent();
                if(value == zfnull)
                {
                    value = this->pimplOwner->buttonIconStyleHighlighted()->imageContent();
                }
            }
            if(value == zfnull)
            {
                value = this->pimplOwner->buttonIconStyleNormal()->imageContent();
            }
        }
        this->buttonIcon->imageContentSet(value);
    }
    void updateButtonBackground(void)
    {
        ZFUIImage *value = this->pimplOwner->buttonBackgroundStyle(this->pimplOwner->buttonState())->imageContent();
        if(value == zfnull)
        {
            if(this->pimplOwner->buttonState() == ZFUIButtonState::e_Checked)
            {
                value = this->pimplOwner->buttonBackgroundStyleHighlighted()->imageContent();
            }
            else if(this->pimplOwner->buttonState() == ZFUIButtonState::e_CheckedHighlighted)
            {
                value = this->pimplOwner->buttonBackgroundStyleChecked()->imageContent();
                if(value == zfnull)
                {
                    value = this->pimplOwner->buttonBackgroundStyleHighlighted()->imageContent();
                }
            }
            if(value == zfnull)
            {
                value = this->pimplOwner->buttonBackgroundStyleNormal()->imageContent();
            }
        }
        this->buttonBackground->imageContentSet(value);
    }

public:
    #define _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(T_Component, T_State) \
        static ZFLISTENER_PROTOTYPE_EXPAND(button##T_Component##Style##T_State##Changed) \
        { \
            const ZFProperty *property = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFProperty *>(); \
            ZFUIButtonBasic *button = userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIButtonBasic *>(); \
            button->prepareButton##T_Component(); \
            if(button->buttonState() == ZFUIButtonState::e_##T_State) \
            { \
                property->callbackCopy(property, button->button##T_Component()->toObject(), listenerData.sender); \
            } \
            else if(property == ZFPropertyAccess(ZFUITextView, textContent)) \
            { \
                button->d->updateButton##T_Component(); \
            } \
        }

    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Label, Normal)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Label, Highlighted)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Label, Checked)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Label, CheckedHighlighted)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Label, Disabled)

    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Icon, Normal)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Icon, Highlighted)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Icon, Checked)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Icon, CheckedHighlighted)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Icon, Disabled)

    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Background, Normal)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Background, Highlighted)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Background, Checked)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Background, CheckedHighlighted)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Background, Disabled)
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIButtonBasicListenerHolder, ZFLevelZFFrameworkNormal)
{
    this->buttonLabelStyleNormalChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonLabelStyleNormalChanged);
    this->buttonLabelStyleHighlightedChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonLabelStyleHighlightedChanged);
    this->buttonLabelStyleCheckedChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonLabelStyleCheckedChanged);
    this->buttonLabelStyleCheckedHighlightedChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonLabelStyleCheckedHighlightedChanged);
    this->buttonLabelStyleDisabledChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonLabelStyleDisabledChanged);
    this->buttonIconStyleNormalChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonIconStyleNormalChanged);
    this->buttonIconStyleHighlightedChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonIconStyleHighlightedChanged);
    this->buttonIconStyleCheckedChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonIconStyleCheckedChanged);
    this->buttonIconStyleCheckedHighlightedChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonIconStyleCheckedHighlightedChanged);
    this->buttonIconStyleDisabledChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonIconStyleDisabledChanged);
    this->buttonBackgroundStyleNormalChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonBackgroundStyleNormalChanged);
    this->buttonBackgroundStyleHighlightedChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonBackgroundStyleHighlightedChanged);
    this->buttonBackgroundStyleCheckedChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonBackgroundStyleCheckedChanged);
    this->buttonBackgroundStyleCheckedHighlightedChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonBackgroundStyleCheckedHighlightedChanged);
    this->buttonBackgroundStyleDisabledChangedListener = ZFCallbackForRawFunction(_ZFP_ZFUIButtonBasicPrivate::buttonBackgroundStyleDisabledChanged);
}
public:
    ZFListener buttonLabelStyleNormalChangedListener;
    ZFListener buttonLabelStyleHighlightedChangedListener;
    ZFListener buttonLabelStyleCheckedChangedListener;
    ZFListener buttonLabelStyleCheckedHighlightedChangedListener;
    ZFListener buttonLabelStyleDisabledChangedListener;
    ZFListener buttonIconStyleNormalChangedListener;
    ZFListener buttonIconStyleHighlightedChangedListener;
    ZFListener buttonIconStyleCheckedChangedListener;
    ZFListener buttonIconStyleCheckedHighlightedChangedListener;
    ZFListener buttonIconStyleDisabledChangedListener;
    ZFListener buttonBackgroundStyleNormalChangedListener;
    ZFListener buttonBackgroundStyleHighlightedChangedListener;
    ZFListener buttonBackgroundStyleCheckedChangedListener;
    ZFListener buttonBackgroundStyleCheckedHighlightedChangedListener;
    ZFListener buttonBackgroundStyleDisabledChangedListener;
ZF_GLOBAL_INITIALIZER_END(ZFUIButtonBasicListenerHolder)

// ============================================================
// ZFUIButtonBasic
ZFOBJECT_REGISTER(ZFUIButtonBasic)
ZFSTYLE_DEFAULT_DEFINE(ZFUIButtonBasic)

ZFObject *ZFUIButtonBasic::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIButtonBasicPrivate);
    d->pimplOwner = this;
    return this;
}

void ZFUIButtonBasic::objectOnDealloc(void)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIButtonBasicListenerHolder) *listenerHolder = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIButtonBasicListenerHolder);
    const ZFProperty *property = zfnull;

    #define _ZFP_ZFUIButtonBasic_componentObserverRemove(Component, State) \
        property = ZFPropertyAccess(ZFUIButtonBasic, button##Component##Style##State); \
        if(property->callbackIsValueAccessed(property, this)) \
        { \
            this->button##Component##Style##State()->toObject()->observerRemove(ZFObject::EventObjectPropertyValueAfterChange(), listenerHolder->button##Component##Style##State##ChangedListener); \
        }

    _ZFP_ZFUIButtonBasic_componentObserverRemove(Label, Normal)
    _ZFP_ZFUIButtonBasic_componentObserverRemove(Label, Highlighted)
    _ZFP_ZFUIButtonBasic_componentObserverRemove(Label, Checked)
    _ZFP_ZFUIButtonBasic_componentObserverRemove(Label, CheckedHighlighted)
    _ZFP_ZFUIButtonBasic_componentObserverRemove(Label, Disabled)

    _ZFP_ZFUIButtonBasic_componentObserverRemove(Icon, Normal)
    _ZFP_ZFUIButtonBasic_componentObserverRemove(Icon, Highlighted)
    _ZFP_ZFUIButtonBasic_componentObserverRemove(Icon, Checked)
    _ZFP_ZFUIButtonBasic_componentObserverRemove(Icon, CheckedHighlighted)
    _ZFP_ZFUIButtonBasic_componentObserverRemove(Icon, Disabled)

    _ZFP_ZFUIButtonBasic_componentObserverRemove(Background, Normal)
    _ZFP_ZFUIButtonBasic_componentObserverRemove(Background, Highlighted)
    _ZFP_ZFUIButtonBasic_componentObserverRemove(Background, Checked)
    _ZFP_ZFUIButtonBasic_componentObserverRemove(Background, CheckedHighlighted)
    _ZFP_ZFUIButtonBasic_componentObserverRemove(Background, Disabled)

    zfReleaseWithoutLeakTest(d->buttonLabel);
    zfReleaseWithoutLeakTest(d->buttonIcon);
    zfReleaseWithoutLeakTest(d->buttonBackground);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIButtonBasic::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
}

void ZFUIButtonBasic::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(this->buttonLabel() != zfnull && this->buttonLabel()->textContent() != zfnull && this->buttonLabel()->textContent()->length() > 0)
    {
        zfstringAppend(ret, zfText(" \"%s\""), this->buttonLabel()->textContent()->stringValue());
    }
}

void ZFUIButtonBasic::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                      ZF_IN const ZFUISize &sizeHint,
                                      ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUIView *labelView = ZFCastZFObjectUnchecked(ZFUIView *, d->buttonLabel);
    ZFUIView *iconView = ZFCastZFObjectUnchecked(ZFUIView *, d->buttonIcon);
    ZFUIView *backgroundView = ZFCastZFObjectUnchecked(ZFUIView *, d->buttonBackground);
    ZFUISize sizeHintTmp = sizeHint;
    if(sizeHintTmp.width >= 0)
    {
        sizeHintTmp.width -= this->buttonContentMargin().left + this->buttonContentMargin().right;
        if(sizeHintTmp.width < 0)
        {
            sizeHintTmp.width = 0;
        }
    }
    if(sizeHintTmp.height >= 0)
    {
        sizeHintTmp.height -= this->buttonContentMargin().top + this->buttonContentMargin().bottom;
        if(sizeHintTmp.height < 0)
        {
            sizeHintTmp.height = 0;
        }
    }

    ZFUISize labelSize = ZFUISizeZero;
    if(labelView != zfnull && labelView->viewVisible() && !d->buttonLabel->textContentIsEmpty())
    {
        labelSize = labelView->layoutMeasure(sizeHintTmp, ZFUISizeParamWrapWidthWrapHeight);
    }

    ZFUISize iconSize = ZFUISizeZero;
    if(iconView != zfnull && iconView->viewVisible() && d->buttonIcon->imageContent() != zfnull)
    {
        iconSize = iconView->layoutMeasure(sizeHintTmp, ZFUISizeParamWrapWidthWrapHeight);
    }

    ZFUISize backgroundSize = ZFUISizeZero;
    if(backgroundView != zfnull && backgroundView->viewVisible() && d->buttonBackground->imageContent() != zfnull)
    {
        ZFUISize sizeHintBg = sizeHint;
        if(sizeHintBg.width >= 0)
        {
            sizeHintBg.width -= this->buttonContentMargin().left + this->buttonContentMargin().right;
            if(sizeHintBg.width < 0)
            {
                sizeHintBg.width = 0;
            }
        }
        if(sizeHintBg.height >= 0)
        {
            sizeHintBg.height -= this->buttonContentMargin().top + this->buttonContentMargin().bottom;
            if(sizeHintBg.height < 0)
            {
                sizeHintBg.height = 0;
            }
        }
        backgroundSize = backgroundView->layoutMeasure(sizeHintBg, ZFUISizeParamWrapWidthWrapHeight);
    }

    ZFUISize contentSize = ZFUISizeZero;
    zfint contentSpace = 0;
    switch(this->buttonContentIconPosition())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            if(iconSize.width > 0 && labelSize.width > 0)
            {
                contentSpace = this->buttonContentSpace();
            }
            contentSize.width = iconSize.width + contentSpace + labelSize.width;
            contentSize.height = zfmMax(iconSize.height, labelSize.height);
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            if(iconSize.height > 0 && labelSize.height > 0)
            {
                contentSpace = this->buttonContentSpace();
            }
            contentSize.width = zfmMax(iconSize.width, labelSize.width);
            contentSize.height = iconSize.height + contentSpace + labelSize.height;
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }

    if(backgroundSize.width > 0)
    {
        backgroundSize.width += ZFUIMarginGetX(this->buttonBackgroundMargin());
    }
    if(backgroundSize.height > 0)
    {
        backgroundSize.height += ZFUIMarginGetY(this->buttonBackgroundMargin());
    }
    if(contentSize.width > 0)
    {
        contentSize.width += ZFUIMarginGetX(this->buttonContentMargin());
    }
    if(contentSize.height > 0)
    {
        contentSize.height += ZFUIMarginGetY(this->buttonContentMargin());
    }
    ret.width = zfmMax(backgroundSize.width, contentSize.width);
    ret.height = zfmMax(backgroundSize.height, contentSize.height);
}
void ZFUIButtonBasic::internalBackgroundViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalBackgroundViewOnLayout(bounds);

    ZFUIView *backgroundView = ZFCastZFObjectUnchecked(ZFUIView *, d->buttonBackground);
    if(backgroundView != zfnull)
    {
        backgroundView->layout(ZFUIRectApplyMargin(bounds, this->buttonBackgroundMargin()));
    }

    ZFUISize sizeHint = ZFUISizeApplyMargin(bounds.size, this->buttonContentMargin());

    ZFUIView *labelView = ZFCastZFObjectUnchecked(ZFUIView *, d->buttonLabel);
    ZFUIView *iconView = ZFCastZFObjectUnchecked(ZFUIView *, d->buttonIcon);

    ZFUISize labelSize = ZFUISizeZero;
    if(labelView != zfnull && labelView->viewVisible() && !d->buttonLabel->textContentIsEmpty())
    {
        labelSize = labelView->layoutMeasure(sizeHint, ZFUISizeParamWrapWidthWrapHeight);
    }

    ZFUISize iconSize = ZFUISizeZero;
    if(iconView != zfnull && iconView->viewVisible() && d->buttonIcon->imageContent() != zfnull)
    {
        iconSize = iconView->layoutMeasure(sizeHint, ZFUISizeParamWrapWidthWrapHeight);
    }

    ZFUIRect contentFrame = ZFUIRectZero;
    zfint contentSpace = 0;
    switch(this->buttonContentIconPosition())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            if(iconSize.width > 0 && labelSize.width > 0)
            {
                contentSpace = this->buttonContentSpace();
            }
            contentFrame = ZFUIAlignApply(
                this->buttonContentAlign(),
                bounds,
                ZFUISizeMake(iconSize.width + contentSpace + labelSize.width, zfmMax(iconSize.height, labelSize.height)),
                this->buttonContentMargin());
            if(this->buttonContentIconPosition() == ZFUIOrientation::e_Left)
            {
                if(iconView != zfnull)
                {
                    iconView->layout(ZFUIAlignApply(ZFUIAlign::e_LeftInner, contentFrame, iconSize));
                }
                if(labelView != zfnull)
                {
                    labelView->layout(ZFUIAlignApply(ZFUIAlign::e_RightInner, contentFrame, labelSize));
                }
            }
            else
            {
                if(iconView != zfnull)
                {
                    iconView->layout(ZFUIAlignApply(ZFUIAlign::e_RightInner, contentFrame, iconSize));
                }
                if(labelView != zfnull)
                {
                    labelView->layout(ZFUIAlignApply(ZFUIAlign::e_LeftInner, contentFrame, labelSize));
                }
            }
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            if(iconSize.height > 0 && labelSize.height > 0)
            {
                contentSpace = this->buttonContentSpace();
            }
            contentFrame = ZFUIAlignApply(
                this->buttonContentAlign(),
                bounds,
                ZFUISizeMake(zfmMax(iconSize.width, labelSize.width), iconSize.height + contentSpace + labelSize.height),
                this->buttonContentMargin());
            if(this->buttonContentIconPosition() == ZFUIOrientation::e_Top)
            {
                if(iconView != zfnull)
                {
                    iconView->layout(ZFUIAlignApply(ZFUIAlign::e_TopInner, contentFrame, iconSize));
                }
                if(labelView != zfnull)
                {
                    labelView->layout(ZFUIAlignApply(ZFUIAlign::e_BottomInner, contentFrame, labelSize));
                }
            }
            else
            {
                if(iconView != zfnull)
                {
                    iconView->layout(ZFUIAlignApply(ZFUIAlign::e_BottomInner, contentFrame, iconSize));
                }
                if(labelView != zfnull)
                {
                    labelView->layout(ZFUIAlignApply(ZFUIAlign::e_TopInner, contentFrame, labelSize));
                }
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

zfbool ZFUIButtonBasic::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    if((d->buttonLabel != zfnull && d->buttonLabel->toObject() == internalView)
        || (d->buttonIcon != zfnull && d->buttonIcon->toObject() == internalView)
        || (d->buttonBackground != zfnull && d->buttonBackground->toObject() == internalView))
    {
        return zffalse;
    }
    return zfsuper::internalViewShouldLayout(internalView);
}

void ZFUIButtonBasic::buttonStateOnChange(void)
{
    zfsuper::buttonStateOnChange();

    #define _ZFP_ZFUIButtonBasic_buttonStateOn(StateName) \
        if(d->buttonLabel != zfnull) \
        { \
            ZFCastZFObjectUnchecked(ZFStyleable *, d->buttonLabel)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, this->buttonLabelStyle##StateName())); \
            d->updateButtonLabel(); \
        } \
        if(d->buttonIcon != zfnull) \
        { \
            ZFCastZFObjectUnchecked(ZFStyleable *, d->buttonIcon)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, this->buttonIconStyle##StateName())); \
            d->updateButtonIcon(); \
        } \
        if(d->buttonBackground != zfnull) \
        { \
            ZFCastZFObjectUnchecked(ZFStyleable *, d->buttonBackground)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, this->buttonBackgroundStyle##StateName())); \
            d->updateButtonBackground(); \
        }
    switch(this->buttonState())
    {
        case ZFUIButtonState::e_Normal:
            if(d->buttonLabel != zfnull)
            {
                ZFCastZFObjectUnchecked(ZFStyleable *, d->buttonLabel)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, this->buttonLabelStyleNormal()));
            }
            if(d->buttonIcon != zfnull)
            {
                ZFCastZFObjectUnchecked(ZFStyleable *, d->buttonIcon)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, this->buttonIconStyleNormal()));
            }
            if(d->buttonBackground != zfnull)
            {
                ZFCastZFObjectUnchecked(ZFStyleable *, d->buttonBackground)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, this->buttonBackgroundStyleNormal()));
            }
            break;
        case ZFUIButtonState::e_Highlighted:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Highlighted)
            break;
        case ZFUIButtonState::e_Checked:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Checked)
            break;
        case ZFUIButtonState::e_CheckedHighlighted:
            _ZFP_ZFUIButtonBasic_buttonStateOn(CheckedHighlighted)
            break;
        case ZFUIButtonState::e_Disabled:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Disabled)
            break;
        default:
            break;
    }
}

const ZFClass *ZFUIButtonBasic::buttonLabelClass(void)
{
    return ZFUITextView::ClassData();
}
const ZFClass *ZFUIButtonBasic::buttonIconClass(void)
{
    return ZFUIImageView::ClassData();
}
const ZFClass *ZFUIButtonBasic::buttonBackgroundClass(void)
{
    return ZFUIImageView::ClassData();
}

ZFUITextView *ZFUIButtonBasic::buttonLabel(void)
{
    this->prepareButtonLabel();
    return d->buttonLabel;
}
ZFUIImageView *ZFUIButtonBasic::buttonIcon(void)
{
    this->prepareButtonIcon();
    return d->buttonIcon;
}
ZFUIImageView *ZFUIButtonBasic::buttonBackground(void)
{
    this->prepareButtonBackground();
    return d->buttonBackground;
}

void ZFUIButtonBasic::prepareButtonLabel(void)
{
    if(d->buttonLabel == zfnull)
    {
        ZFObject *obj = zfRetainWithoutLeakTest(this->buttonLabelClass()->newInstanceWithoutLeakTest().toObject());
        d->buttonLabel = ZFCastZFObject(ZFUITextView *, obj);
        zfCoreAssert(d->buttonLabel != zfnull);
        this->internalBackgroundViewAdd(ZFCastZFObjectUnchecked(ZFUIView *, d->buttonLabel));
    }
}
void ZFUIButtonBasic::prepareButtonIcon(void)
{
    if(d->buttonIcon == zfnull)
    {
        ZFObject *obj = zfRetainWithoutLeakTest(this->buttonIconClass()->newInstanceWithoutLeakTest().toObject());
        d->buttonIcon = ZFCastZFObject(ZFUIImageView *, obj);
        zfCoreAssert(d->buttonIcon != zfnull);
        this->internalBackgroundViewAdd(ZFCastZFObjectUnchecked(ZFUIView *, d->buttonIcon));

        if(d->buttonLabel != zfnull)
        {
            ZFUIView *t = d->buttonLabel->to<ZFUIView *>();
            this->internalBackgroundViewRemove(t);
            this->internalBackgroundViewAdd(t);
        }
    }
}
void ZFUIButtonBasic::prepareButtonBackground(void)
{
    if(d->buttonBackground == zfnull)
    {
        ZFObject *obj = zfRetainWithoutLeakTest(this->buttonBackgroundClass()->newInstanceWithoutLeakTest().toObject());
        d->buttonBackground = ZFCastZFObject(ZFUIImageView *, obj);
        zfCoreAssert(d->buttonBackground != zfnull);
        this->internalBackgroundViewAdd(ZFCastZFObjectUnchecked(ZFUIView *, d->buttonBackground));

        if(d->buttonIcon != zfnull)
        {
            ZFUIView *t = d->buttonIcon->to<ZFUIView *>();
            this->internalBackgroundViewRemove(t);
            this->internalBackgroundViewAdd(t);
        }
        if(d->buttonLabel != zfnull)
        {
            ZFUIView *t = d->buttonLabel->to<ZFUIView *>();
            this->internalBackgroundViewRemove(t);
            this->internalBackgroundViewAdd(t);
        }
    }
}

void ZFUIButtonBasic::objectPropertyValueOnInit(ZF_IN const ZFProperty *property)
{
    zfsuper::objectPropertyValueOnInit(property);

    #define _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(T_Component, T_State) \
        else if(property == ZFPropertyAccess(ZFUIButtonBasic, button##T_Component##Style##T_State)) \
        { \
            this->prepareButton##T_Component(); \
            this->button##T_Component##Style##T_State()->toObject()->observerAdd( \
                ZFObject::EventObjectPropertyValueAfterChange(), \
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIButtonBasicListenerHolder)->button##T_Component##Style##T_State##ChangedListener, \
                this->objectHolder()); \
        }

    if(zffalse)
    {
    }

    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Label, Normal)
    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Label, Highlighted)
    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Label, Checked)
    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Label, CheckedHighlighted)
    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Label, Disabled)

    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Icon, Normal)
    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Icon, Highlighted)
    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Icon, Checked)
    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Icon, CheckedHighlighted)
    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Icon, Disabled)

    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Background, Normal)
    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Background, Highlighted)
    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Background, Checked)
    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Background, CheckedHighlighted)
    _ZFP_ZFUIButtonBasic_objectPropertyValueOnInit(Background, Disabled)
}

ZF_NAMESPACE_GLOBAL_END

