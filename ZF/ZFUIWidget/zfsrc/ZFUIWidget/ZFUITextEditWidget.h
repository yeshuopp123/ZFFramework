/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUITextEditWidget.h
 * @brief basic text edit widget
 */

#ifndef _ZFI_ZFUITextEditWidget_h_
#define _ZFI_ZFUITextEditWidget_h_

#include "ZFUIButtonBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUITextEditWidget
/**
 * @brief basic text edit widget
 */
zfclass ZF_ENV_EXPORT ZFUITextEditWidget : zfextends ZFUITextEdit
{
    ZFOBJECT_DECLARE(ZFUITextEditWidget, ZFUITextEdit)
    ZFSTYLE_DEFAULT_DECLARE(ZFUITextEditWidget)

public:
    // ============================================================
    // properties
    /**
     * @brief the background view
     *
     * note, image's nine patch would be added to #ZFUIView::nativeImplViewMargin
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, textEditBackgroundView, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageView *, textEditBackgroundView)
    {
        zfblockedAlloc(ZFUIImageView, textEditBackgroundView);
        propertyValue = zfautoObjectCreateWithoutLeakTest(textEditBackgroundView);
        textEditBackgroundView->imageContentSet(ZFUIImageResXml(zfText("ZFUIWidget/ZFUITextEditWidget_background.xml")));
    }

    /**
     * @brief the clear button
     *
     * note, the clear button's size would be added to #ZFUIView::nativeImplViewMargin
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUIButtonBasic *, textEditClearButton, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIButtonBasic *, textEditClearButton)
    {
        zfblockedAlloc(ZFUIButtonBasic, textEditClearButton);
        propertyValue = zfautoObjectCreateWithoutLeakTest(textEditClearButton);
        textEditClearButton->buttonIconImageSet(ZFUIImageResXml(zfText("ZFUIWidget/ZFUITextEditWidget_clearButton.xml")));
        textEditClearButton->viewSizeMinSet(ZFUISizeZero);
        textEditClearButton->viewVisibleSet(zffalse);
    }

    /**
     * @brief whether automatically show or hide clear button, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, textEditClearButtonAutoEnable, ZFPropertyInitValue(zffalse))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfbool, textEditClearButtonAutoEnable);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void nativeImplViewMarginOnUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin);
    zfoverride
    virtual zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView);
    zfoverride
    virtual void internalBackgroundViewOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void textEditOnChange(ZF_IN ZFString *oldText);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUITextEditWidget_h_

