/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUITextView.h"
#include "protocol/ZFProtocolZFUITextView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUITextView)

// ============================================================
// _ZFP_ZFUITextViewPrivate
zfclassNotPOD _ZFP_ZFUITextViewPrivate
{
public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFUITextView) *impl;
    ZFUITextView *pimplOwner;

public:
    void updateSizeRelatedProperty(void)
    {
        this->impl->textShadowOffsetSet(this->pimplOwner, ZFUISizeApplyScale(this->pimplOwner->textShadowOffset(), this->pimplOwner->scaleGetFixed()));
        this->impl->textSizeSet(this->pimplOwner, ZFUISizeApplyScale(this->pimplOwner->textSize(), this->pimplOwner->scaleGetFixed()));
        this->impl->textSizeAutoChangeMinSizeSet(this->pimplOwner, ZFUISizeApplyScale(this->pimplOwner->textSizeAutoChangeMinSize(), this->pimplOwner->scaleGetFixed()));
        this->impl->textSizeAutoChangeMaxSizeSet(this->pimplOwner, ZFUISizeApplyScale(this->pimplOwner->textSizeAutoChangeMaxSize(), this->pimplOwner->scaleGetFixed()));
    }

public:
    _ZFP_ZFUITextViewPrivate(void)
    : impl(ZFPROTOCOL_ACCESS(ZFUITextView))
    , pimplOwner(zfnull)
    {
    }
};

// ============================================================
// ZFUITextView
ZFOBJECT_REGISTER(ZFUITextView)

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextView, ZFString *, textContent)
{
    zfbool changed = (ZFObjectCompare(this->textContent(), newValue) != ZFCompareTheSame);
    this->textContentSetInternal(newValue);
    d->impl->textContentSet(this, this->textContent());
    if(changed)
    {
        this->layoutRequest();
    }
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextView, ZFUITextAppearanceEnum, textAppearance)
{
    zfbool changed = (this->textAppearance() != newValue);
    this->textAppearanceSetInternal(newValue);
    d->impl->textAppearanceSet(this, this->textAppearance());
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextView, ZFUIAlignFlags, textAlign)
{
    this->textAlignSetInternal(newValue);
    d->impl->textAlignSet(this, this->textAlign());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextView, ZFUIColor, textColor)
{
    this->textColorSetInternal(newValue);
    d->impl->textColorSet(this, this->textColor());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextView, ZFUIColor, textShadowColor)
{
    this->textShadowColorSetInternal(newValue);
    d->impl->textShadowColorSet(this, this->textShadowColor());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextView, ZFUISize, textShadowOffset)
{
    this->textShadowOffsetSetInternal(newValue);
    d->impl->textShadowOffsetSet(this, ZFUISizeApplyScale(this->textShadowOffset(), this->scaleGetFixed()));
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextView, zfint, textSize)
{
    zfbool changed = (this->textSize() != newValue);
    this->textSizeSetInternal(newValue);
    d->impl->textSizeSet(this, ZFUISizeApplyScale(this->textSize(), this->scaleGetFixed()));
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextView, zfint, textSizeAutoChangeMinSize)
{
    zfbool changed = (this->textSizeAutoChangeMinSize() != newValue);
    this->textSizeAutoChangeMinSizeSetInternal(newValue);
    d->impl->textSizeAutoChangeMinSizeSet(this, ZFUISizeApplyScale(this->textSizeAutoChangeMinSize(), this->scaleGetFixed()));
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextView, zfint, textSizeAutoChangeMaxSize)
{
    zfbool changed = (this->textSizeAutoChangeMaxSize() != newValue);
    this->textSizeAutoChangeMaxSizeSetInternal(newValue);
    d->impl->textSizeAutoChangeMaxSizeSet(this, ZFUISizeApplyScale(this->textSizeAutoChangeMaxSize(), this->scaleGetFixed()));
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextView, zfbool, textSingleLine)
{
    zfbool changed = (this->textSingleLine() != newValue);
    this->textSingleLineSetInternal(newValue);
    d->impl->textSingleLineSet(this, this->textSingleLine());
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextView, ZFUITextTruncateModeEnum, textTruncateMode)
{
    zfbool changed = (this->textTruncateMode() != newValue);
    this->textTruncateModeSetInternal(newValue);
    d->impl->textTruncateModeSet(this, this->textTruncateMode());
    if(changed)
    {
        this->layoutRequest();
    }
}

ZFObject *ZFUITextView::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUITextViewPrivate);
    d->pimplOwner = this;

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD _ZFP_ZFUITextView_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUITextView)->nativeTextViewDestroy(view->to<ZFUITextView *>(), nativeImplView);
        }
    };
    this->nativeImplViewSet(d->impl->nativeTextViewCreate(this),
                            _ZFP_ZFUITextView_nativeImplViewDestroy::action);

    return this;
}
void ZFUITextView::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUITextView::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    d->updateSizeRelatedProperty();
}

void ZFUITextView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(this->textContent() != zfnull && this->textContent()->length() > 0)
    {
        ret += zfText(" \"");
        ret += this->textContent()->stringValue();
        ret += zfText("\"");
    }
}

ZFUISize ZFUITextView::measureTextView(ZF_IN_OPT const ZFUISize &sizeHint /* = ZFUISizeZero */)
{
    return ZFUISizeApplyScaleReversely(d->impl->measureNativeTextView(this,
        ZFUISizeApplyScale(sizeHint, this->scaleGetFixed()),
        ZFUISizeApplyScale(this->textSize(), this->scaleGetFixed())),
        this->scaleGetFixed());
}

zfint ZFUITextView::textSizeCurrent(void)
{
    return ZFUISizeApplyScaleReversely(d->impl->textSizeCurrent(this), this->scaleGetFixed());
}

void ZFUITextView::scaleOnChange(void)
{
    zfsuper::scaleOnChange();
    d->updateSizeRelatedProperty();
}
void ZFUITextView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = this->measureTextView(sizeHint);
}
void ZFUITextView::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayout(bounds);
    d->impl->layoutNativeTextView(this,
        ZFUISizeApplyScale(ZFUISizeApplyMargin(bounds.size, this->nativeImplViewMargin()), this->scaleGetFixed()));
}

ZF_NAMESPACE_GLOBAL_END

