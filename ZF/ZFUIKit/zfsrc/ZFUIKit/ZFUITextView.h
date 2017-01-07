/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUITextView.h
 * @brief view to display plain text
 */

#ifndef _ZFI_ZFUITextView_h_
#define _ZFI_ZFUITextView_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFUITextViewPrivate;
// ============================================================
// ZFUITextView
/**
 * @brief view to display plain text
 *
 * for editable or selectable text view, you should use #ZFUITextView,
 * for rich text, you should supply you own native view,
 * or use web view with html strings
 */
zfclass ZF_ENV_EXPORT ZFUITextView : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUITextView, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUITextView)

public:
    // ============================================================
    // properties
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(zfbool, viewUIEnable, zffalse)
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(zfbool, viewUIEnableTree, zffalse)

public:
    /**
     * @brief text, may be null if not set
     */
    ZFPROPERTY_RETAIN(ZFString *, textContent)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFString *, textContent);
    /**
     * @brief see #textContent, always empty string if not set
     */
    virtual const zfchar *textContentString(void)
    {
        return (this->textContent() ? this->textContent()->stringValue() : zfText(""));
    }
    /**
     * @brief see #textContent
     */
    virtual void textContentStringSet(ZF_IN const zfchar *s)
    {
        this->textContentSet((s && *s) ? zflineAlloc(ZFString, s) : zfnull);
    }

    /**
     * @brief util method to check whether the text is null or empty string
     */
    virtual inline zfbool textContentIsEmpty(void)
    {
        return zfscmpTheSame(this->textContentString(), zfText(""));
    }

    /**
     * @brief text appearance, #ZFUIGlobalStyle::textAppearance by default
     *
     * note, for some implementations and font settings, italic or bold may or may not be supported
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUITextAppearanceEnum, textAppearance,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->textAppearance()))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUITextAppearanceEnum, textAppearance);

    /**
     * @brief text alignment, #ZFUIGlobalStyle::textAlign by default
     *
     * @note only LeftInner, Center, RightInner would be supported for sure
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIAlignFlags, textAlign,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->textAlign()))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUIAlignFlags, textAlign);

    /**
     * @brief text color, #ZFUIGlobalStyle::textColorDefault by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColor,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->textColorDefault()))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUIColor, textColor);

    /**
     * @brief text shadow color, #ZFUIColorTransparent by default, use transparent to disable text shadow
     *
     * note that implementation may have no text shadow support
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textShadowColor,
                                ZFPropertyInitValue(ZFUIColorTransparent))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUIColor, textShadowColor);

    /**
     * @brief text shadow offset, (1, 1) by default
     *
     * note that implementation may have no text shadow support
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISize, textShadowOffset,
                                ZFPropertyInitValue((ZFUISizeMake(1, 1))))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUISize, textShadowOffset);

    /**
     * @brief text size in pixel, #ZFUIGlobalStyle::textSizeNormal by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSize,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->textSizeNormal()))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfint, textSize);

    /**
     * @brief if not 0, auto decrease text size
     *   when the view isn't big enough to hold current text,
     *   #ZFUIGlobalStyle::textSizeTiny by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeAutoChangeMinSize,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->textSizeTiny()))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfint, textSizeAutoChangeMinSize);
    /**
     * @brief if not 0 and larger than #textSizeAutoChangeMinSize,
     *   auto increase text size when the view is bigger than current text need,
     *   0 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeAutoChangeMaxSize,
                                ZFPropertyInitValue(0))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfint, textSizeAutoChangeMaxSize);

    /**
     * @brief single line or not, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, textSingleLine,
                                ZFPropertyInitValue(zftrue))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfbool, textSingleLine);

    /**
     * @brief text truncate mode, #ZFUITextTruncateMode::e_Disable by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUITextTruncateModeEnum, textTruncateMode,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->textTruncateMode()))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUITextTruncateModeEnum, textTruncateMode);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnInitFinish(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief measure text view
     *
     * this is separate from UIView's measure step,
     * usually called by app to measure text's size,
     * and usually have no need to call\n
     * sizeHint shows max size the text view may have, or 0 if no limit
     */
    virtual ZFUISize measureTextView(ZF_IN_OPT const ZFUISize &sizeHint = ZFUISizeZero);

    /**
     * @brief get current text size
     *
     * if the view hasn't been layouted, the size may be invalid
     */
    virtual zfint textSizeCurrent(void);

    // ============================================================
    // override
protected:
    /**
     * @brief we would update text settings when scale changed
     */
    zfoverride
    virtual void scaleOnChange(void);
    /**
     * @brief we would measure accorrding text size
     */
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    /**
     * @brief we would layout text after parent's layout step
     */
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);

private:
    _ZFP_ZFUITextViewPrivate *d;
};

// ============================================================
ZFLANGAPPLY_IMPL(ZFUITextView, ZFUITextViewText, {obj->textContentStringSet(langValue);})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUITextView_h_

