/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUITextEditWidget.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUITextEditWidget)

// ============================================================
// ZFUITextEditWidget
ZFOBJECT_REGISTER(ZFUITextEditWidget)

static void _ZFP_ZFUITextEditWidget_updateClearButton(ZF_IN ZFUITextEditWidget *textEditWidget)
{
    if(textEditWidget->textEditClearButtonAutoEnable())
    {
        textEditWidget->textEditClearButton()->to<ZFUIView *>()->viewVisibleSet(
            !textEditWidget->textContentIsEmpty());
    }
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUITextEditWidget, zfbool, textEditClearButtonAutoEnable)
{
    this->textEditClearButtonAutoEnableSetInternal(newValue);
    _ZFP_ZFUITextEditWidget_updateClearButton(this);
}

ZFObject *ZFUITextEditWidget::objectOnInit(void)
{
    zfsuper::objectOnInit();

    ZFUIImageView *textEditBackgroundView = this->textEditBackgroundView()->to<ZFUIImageView *>();
    this->internalImplViewAdd(textEditBackgroundView);
    textEditBackgroundView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
    textEditBackgroundView->serializableRefLayoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);

    ZFUIButtonBasic *textEditClearButton = this->textEditClearButton()->to<ZFUIButtonBasic *>();
    this->internalBackgroundViewAdd(textEditClearButton);
    textEditClearButton->layoutParam()->layoutAlignSet(ZFUIAlign::e_RightInner);
    textEditClearButton->serializableRefLayoutParam()->layoutAlignSet(ZFUIAlign::e_RightInner);

    _ZFP_ZFUITextEditWidget_updateClearButton(this);

    return this;
}
void ZFUITextEditWidget::objectOnDealloc(void)
{
    zfsuper::objectOnDealloc();
}

void ZFUITextEditWidget::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayoutPrepare(bounds);
    this->nativeImplViewMarginUpdate();
}
void ZFUITextEditWidget::nativeImplViewMarginOnUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin)
{
    zfsuper::nativeImplViewMarginOnUpdate(nativeImplViewMargin);

    ZFUIImageView *textEditBackgroundView = this->textEditBackgroundView()->to<ZFUIImageView *>();
    if(textEditBackgroundView->viewVisible())
    {
        ZFUIImage *image = textEditBackgroundView->imageContent();
        if(image != zfnull)
        {
            ZFUIMarginInc(nativeImplViewMargin, nativeImplViewMargin, image->imageNinePatch());
        }
    }

    ZFUIButtonBasic *textEditClearButton = this->textEditClearButton()->to<ZFUIButtonBasic *>();
    if(textEditClearButton->viewVisible())
    {
        textEditClearButton->layoutMeasure(ZFUISizeInvalid, ZFUISizeParamWrapWidthWrapHeight);
        zfint size = ZFUIMarginGetX(textEditClearButton->layoutParam()->layoutMargin()) + textEditClearButton->layoutMeasuredSize().width;
        if(ZFBitTest(textEditClearButton->layoutParam()->layoutAlign(), ZFUIAlign::e_LeftInner))
        {
            nativeImplViewMargin.left += size;
        }
        else
        {
            nativeImplViewMargin.right += size;
        }
    }
}
zfbool ZFUITextEditWidget::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    if(internalView == this->textEditClearButton()->toObject())
    {
        return zffalse;
    }
    return zfsuper::internalViewShouldLayout(internalView);
}
void ZFUITextEditWidget::internalBackgroundViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalBackgroundViewOnLayout(bounds);

    ZFUIButtonBasic *textEditClearButton = this->textEditClearButton()->to<ZFUIButtonBasic *>();
    if(!textEditClearButton->viewVisible())
    {
        return ;
    }

    ZFUIMargin margin = ZFUIMarginZero;
    ZFUIImageView *textEditBackgroundView = this->textEditBackgroundView()->to<ZFUIImageView *>();
    if(textEditBackgroundView->viewVisible())
    {
        ZFUIImage *image = textEditBackgroundView->imageContent();
        if(image != zfnull)
        {
            margin = image->imageNinePatch();
        }
    }
    textEditClearButton->layout(ZFUIAlignApply(
        textEditClearButton->layoutParam()->layoutAlign(),
        bounds,
        textEditClearButton->layoutMeasuredSize(),
        margin));
}
void ZFUITextEditWidget::textEditOnChange(ZF_IN ZFString *oldText)
{
    zfsuper::textEditOnChange(oldText);
    _ZFP_ZFUITextEditWidget_updateClearButton(this);
}

ZF_NAMESPACE_GLOBAL_END

