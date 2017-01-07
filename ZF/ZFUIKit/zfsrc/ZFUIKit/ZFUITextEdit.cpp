/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUITextEdit.h"
#include "protocol/ZFProtocolZFUITextEdit.h"

#include "ZFUIViewFocus.h"
#include "ZFUIOnScreenKeyboardState.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUITextEditKeyboardType)

ZFENUM_DEFINE(ZFUITextEditKeyboardReturnType)
ZFENUM_DEFINE(ZFUITextEditKeyboardReturnAction)

ZFSTYLE_DEFAULT_DEFINE(ZFUITextEdit)

// ============================================================
// _ZFP_ZFUITextEditPrivate
zfclassNotPOD _ZFP_ZFUITextEditPrivate
{
public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFUITextEdit) *impl;
    ZFUITextEdit *pimplOwner;
    zfbool textEditing;
    zfbool textChangedByImplFlag;

public:
    void updateSizeRelatedProperty(void)
    {
        this->impl->textShadowOffsetSet(this->pimplOwner, ZFUISizeApplyScale(this->pimplOwner->textShadowOffset(), this->pimplOwner->scaleGetFixed()));
        this->impl->textSizeSet(this->pimplOwner, ZFUISizeApplyScale(this->pimplOwner->textSize(), this->pimplOwner->scaleGetFixed()));
    }

public:
    _ZFP_ZFUITextEditPrivate(void)
    : impl(ZFPROTOCOL_ACCESS(ZFUITextEdit))
    , pimplOwner(zfnull)
    , textEditing(zffalse)
    , textChangedByImplFlag(zffalse)
    {
    }
};

// ============================================================
// ZFUITextEdit
ZFOBJECT_REGISTER(ZFUITextEdit)

ZFOBSERVER_EVENT_REGISTER(ZFUITextEdit, TextEditOnEditBegin)
ZFOBSERVER_EVENT_REGISTER(ZFUITextEdit, TextEditOnEditEnd)
ZFOBSERVER_EVENT_REGISTER(ZFUITextEdit, TextEditOnCheckChange)
ZFOBSERVER_EVENT_REGISTER(ZFUITextEdit, TextEditOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUITextEdit, TextEditReturnOnClick)
ZFOBSERVER_EVENT_REGISTER(ZFUITextEdit, TextEditOnConfirm)

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, zfbool, textEditEnable)
{
    this->textEditEnableSetInternal(newValue);
    d->impl->textEditEnableSet(this, this->textEditEnable());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, zfbool, textEditSecured)
{
    this->textEditSecuredSetInternal(newValue);
    d->impl->textEditSecuredSet(this, this->textEditSecured());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, ZFUITextEditKeyboardTypeEnum, textEditKeyboardType)
{
    this->textEditKeyboardTypeSetInternal(newValue);
    d->impl->textEditKeyboardTypeSet(this, this->textEditKeyboardType());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, ZFUITextEditKeyboardReturnTypeEnum, textEditKeyboardReturnType)
{
    this->textEditKeyboardReturnTypeSetInternal(newValue);
    d->impl->textEditKeyboardReturnTypeSet(this, this->textEditKeyboardReturnType());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, ZFUITextEditKeyboardReturnActionEnum, textEditKeyboardReturnAction)
{
    this->textEditKeyboardReturnActionSetInternal(newValue);
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, ZFRegExp *, textEditFilter)
{
    this->textEditFilterSetInternal(newValue);
    if(this->textContent() != zfnull && !this->textShouldChange(this->textContent()))
    {
        this->textContentSet(zfnull);
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, zfindexRange, textSelectRange)
{
    d->impl->textSelectRangeSet(this, newValue);
    zfindexRange tmp = zfindexRangeZero;
    d->impl->textSelectRange(this, tmp);
    this->textSelectRangeSetInternal(tmp);
}
ZFPROPERTY_CUSTOM_GETTER_DEFINE(ZFUITextEdit, zfindexRange, textSelectRange)
{
    zfindexRange tmp = zfindexRangeZero;
    d->impl->textSelectRange(this, tmp);
    this->textSelectRangeSetInternal(tmp);
    return this->textSelectRangeInternal();
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, ZFString *, textContent)
{
    ZFString *newValueTmp = newValue;
    if(newValue != zfnull && !newValue->isEmpty() && !this->textShouldChange(newValue))
    {
        newValueTmp = zfnull;
    }

    this->textContentSetInternal(newValueTmp);
    if(!d->textChangedByImplFlag)
    {
        d->impl->textContentSet(this, this->textContent());
    }
    ZFCastZFObjectUnchecked(ZFUIView *, this->textPlaceHolder())->viewVisibleSet(
        this->textContent() == zfnull || this->textContent()->isEmpty());
    this->layoutRequest();
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, ZFUITextAppearanceEnum, textAppearance)
{
    zfbool changed = (this->textAppearance() != newValue);
    this->textAppearanceSetInternal(newValue);
    d->impl->textAppearanceSet(this, this->textAppearance());
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, ZFUIAlignFlags, textAlign)
{
    this->textAlignSetInternal(newValue);
    d->impl->textAlignSet(this, this->textAlign());
    this->textPlaceHolder()->textAlignSet(this->textAlign());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, ZFUIColor, textColor)
{
    this->textColorSetInternal(newValue);
    d->impl->textColorSet(this, this->textColor());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, ZFUIColor, textShadowColor)
{
    this->textShadowColorSetInternal(newValue);
    d->impl->textShadowColorSet(this, this->textShadowColor());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, ZFUISize, textShadowOffset)
{
    this->textShadowOffsetSetInternal(newValue);
    d->impl->textShadowOffsetSet(this, ZFUISizeApplyScale(this->textShadowOffset(), this->scaleGetFixed()));
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEdit, zfint, textSize)
{
    zfbool changed = (this->textSize() != newValue);
    this->textSizeSetInternal(newValue);
    d->impl->textSizeSet(this, ZFUISizeApplyScale(this->textSize(), this->scaleGetFixed()));
    if(changed)
    {
        this->layoutRequest();
    }
}

ZFObject *ZFUITextEdit::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUITextEditPrivate);
    d->pimplOwner = this;

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD _ZFP_ZFUITextEdit_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUITextEdit)->nativeTextEditDestroy(view->to<ZFUITextEdit *>(), nativeImplView);
        }
    };
    this->nativeImplViewSet(d->impl->nativeTextEditCreate(this),
        _ZFP_ZFUITextEdit_nativeImplViewDestroy::action);

    ZFUIView *textPlaceHolderTmp = ZFCastZFObject(ZFUIView *, this->textPlaceHolder());
    if(textPlaceHolderTmp == zfnull)
    {
        if(this->textPlaceHolder() != zfnull)
        {
            zfCoreCriticalClassNotTypeOf(this->textPlaceHolder()->classData(), ZFUIView::ClassData());
        }
        else
        {
            zfCoreCriticalMessage(zfTextA("textPlaceHolder must not be null"));
        }
        return zfnull;
    }
    this->internalImplViewAdd(textPlaceHolderTmp);
    textPlaceHolderTmp->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
    textPlaceHolderTmp->serializableRefLayoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);

    return this;
}
void ZFUITextEdit::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUITextEdit::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    d->updateSizeRelatedProperty();
}

void ZFUITextEdit::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(this->textPlaceHolder()->textContent() != zfnull)
    {
        zfstringAppend(ret, zfText(" (%s)"), this->textPlaceHolder()->textContent()->stringValue());
    }

    if(this->textEditSecured())
    {
        ret += zfText(" EditSecured");
    }
    else if(this->textContent() != zfnull && this->textContent()->length() > 0)
    {
        zfstringAppend(ret, zfText(" \"%s\""), this->textContent()->stringValue());
    }

    if(!this->textEditEnable())
    {
        ret += zfText(" EditDisabled");
    }
}

ZFUISize ZFUITextEdit::measureTextEdit(ZF_IN_OPT const ZFUISize &sizeHint /* = ZFUISizeZero */)
{
    ZFUISize ret = ZFUISizeZero;
    ZFUISizeApplyScaleReversely(ret, d->impl->measureNativeTextEdit(this,
        ZFUISizeApplyScale(ZFUIViewLayoutParam::sizeHintOffset(sizeHint, ZFUISizeMake(
                    0 - ZFUIMarginGetX(this->nativeImplViewMargin()),
                    0 - ZFUIMarginGetY(this->nativeImplViewMargin())
                )),
            this->scaleGetFixed()),
        ZFUISizeApplyScale(this->textSize(), this->scaleGetFixed())),
        this->scaleGetFixed());
    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUITextEdit, textPlaceHolder), this)
        && this->textPlaceHolder()->textContentString() != zfnull)
    {
        ZFUISize hintSize = this->textPlaceHolder()->to<ZFUITextView *>()->measureTextView();
        if(ret.width < hintSize.width)
        {
            ret.width = hintSize.width;
        }
        if(ret.height < hintSize.height)
        {
            ret.height = hintSize.height;
        }
    }
    ZFUISizeApplyMarginReversely(ret, ret, this->nativeImplViewMargin());
    return ret;
}

void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyBeginEdit(void)
{
    d->textEditing = zftrue;
    this->textEditOnEditBegin();
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyEndEdit(void)
{
    d->textEditing = zffalse;
    this->textEditOnEditEnd();
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyChange(ZF_IN ZFString *newText)
{
    ZFString *oldText = this->textContent();
    zfRetainWithoutLeakTest(oldText);

    d->textChangedByImplFlag = zftrue;
    this->textContentSet(newText);
    d->textChangedByImplFlag = zffalse;

    this->textEditOnChange(oldText);

    zfReleaseWithoutLeakTest(oldText);
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyReturnClicked(void)
{
    this->textEditReturnOnClick();
    this->textEditNotifyConfirm();

    switch(this->textEditKeyboardReturnAction())
    {
        case ZFUITextEditKeyboardReturnAction::e_None:
            break;
        case ZFUITextEditKeyboardReturnAction::e_FocusNext:
            if(this->viewFocused())
            {
                ZFUIView *next = ZFUIViewFocusNextFind(this);
                if(next == zfnull)
                {
                    this->textEditEnd();
                }
                else
                {
                    ZFUITextEdit *nextTmp = ZFCastZFObject(ZFUITextEdit *, next);
                    if(nextTmp != zfnull)
                    {
                        nextTmp->textEditBegin();
                    }
                    else
                    {
                        next->viewFocusRequest(zftrue);
                    }
                }
            }
            break;
        case ZFUITextEditKeyboardReturnAction::e_HideKeyboard:
        {
            ZFUIOnScreenKeyboardState *keyboardState = ZFUIOnScreenKeyboardState::instanceForView(this);
            if(keyboardState != zfnull && keyboardState->keyboardShowing() && this->textEditing())
            {
                this->textEditEnd();
            }
        }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
zfbool ZFUITextEdit::textShouldChange(ZF_IN ZFString *newText)
{
    zfbool shouldChange = zftrue;
    this->textEditOnCheckChange(newText, shouldChange);
    if(!shouldChange && newText != zfnull && !newText->isEmpty())
    {
        return zffalse;
    }
    else
    {
        return zftrue;
    }
}
void ZFUITextEdit::textEditBegin(void)
{
    d->impl->textEditBegin(this);
}
void ZFUITextEdit::textEditEnd(void)
{
    d->impl->textEditEnd(this);
}
zfbool ZFUITextEdit::textEditing(void)
{
    return d->textEditing;
}

void ZFUITextEdit::textEditOnEditBegin(void)
{
    this->observerNotify(ZFUITextEdit::EventTextEditOnEditBegin());
}
void ZFUITextEdit::textEditOnEditEnd(void)
{
    this->observerNotify(ZFUITextEdit::EventTextEditOnEditEnd());
}
void ZFUITextEdit::textEditOnCheckChange(ZF_IN ZFString *newText, ZF_IN_OUT zfbool &shouldChange)
{
    shouldChange = zftrue;
    if(newText != zfnull && !newText->isEmpty() && this->textEditFilter() != zfnull)
    {
        ZFRegExpResult regexpResult;
        this->textEditFilter()->regExpMatchExact(regexpResult, newText->stringValue());
        if(!regexpResult.matched)
        {
            shouldChange = zffalse;
        }
    }

    zfautoObject t = ZFValueEditable::boolValueCreate(shouldChange);
    this->observerNotify(ZFUITextEdit::EventTextEditOnCheckChange(), newText, t.toObject());
    shouldChange = t.to<ZFValue *>()->boolValue();
}
void ZFUITextEdit::textEditOnChange(ZF_IN ZFString *oldText)
{
    this->observerNotify(ZFUITextEdit::EventTextEditOnChange(), oldText);
}
void ZFUITextEdit::textEditReturnOnClick(void)
{
    this->observerNotify(ZFUITextEdit::EventTextEditReturnOnClick());
}
void ZFUITextEdit::textEditOnConfirm(void)
{
    this->observerNotify(ZFUITextEdit::EventTextEditOnConfirm());
}

void ZFUITextEdit::scaleOnChange(void)
{
    zfsuper::scaleOnChange();
    d->updateSizeRelatedProperty();
}
void ZFUITextEdit::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = this->viewSizePrefered();
    if(ret.width < 0)
    {
        ret.width = ZFUIGlobalStyle::DefaultStyle()->itemSizeControl();
    }
    if(ret.height < 0)
    {
        ret.height = ZFUIGlobalStyle::DefaultStyle()->itemSizeControl();
    }
}
void ZFUITextEdit::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayout(bounds);
}
void ZFUITextEdit::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
{
    switch(keyEvent->keyCode)
    {
        case ZFUIKeyCode::e_kShift:
        case ZFUIKeyCode::e_kTab:
            zfsuper::viewEventOnKeyEvent(keyEvent);
            break;
        default:
            keyEvent->eventResolvedSet(zftrue);
            break;
    }
}
void ZFUITextEdit::viewFocusOnChange(void)
{
    zfsuper::viewFocusOnChange();
    this->textEditNotifyConfirm();
}

zfbool ZFUITextEdit::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    if(internalView == this->textPlaceHolder()->toObject())
    {
        return zffalse;
    }
    return zfsuper::internalViewShouldLayout(internalView);
}
void ZFUITextEdit::internalBackgroundViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalBackgroundViewOnLayout(bounds);

    ZFUITextView *textPlaceHolder = this->textPlaceHolder()->to<ZFUITextView *>();
    textPlaceHolder->layout(ZFUIRectApplyMargin(
            bounds,
            ZFUIMarginInc(this->nativeImplViewMargin(), textPlaceHolder->layoutParam()->layoutMargin())
        ));
}

ZF_NAMESPACE_GLOBAL_END

