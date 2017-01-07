/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFUITextView.h
 * @brief protocol for #ZFUITextView
 */

#ifndef _ZFI_ZFProtocolZFUITextView_h_
#define _ZFI_ZFProtocolZFUITextView_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUITextView.h"
#include "ZFCore/protocol/ZFProtocolZFString.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFUITextView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUITextView)
public:
    /**
     * @brief create native text view
     */
    virtual void *nativeTextViewCreate(ZF_IN ZFUITextView *textView) = 0;
    /**
     * @brief destroy native text view
     */
    virtual void nativeTextViewDestroy(ZF_IN ZFUITextView *textView,
                                       ZF_IN void *nativeTextView) = 0;

    // ============================================================
    // properties
public:
    /** @brief see #ZFUITextView */
    virtual void textContentSet(ZF_IN ZFUITextView *textView,
                                ZF_IN ZFString *text) = 0;
    /** @brief see #ZFUITextView */
    virtual void textAppearanceSet(ZF_IN ZFUITextView *textView,
                                   ZF_IN ZFUITextAppearanceEnum const &textAppearance) = 0;
    /** @brief see #ZFUITextView */
    virtual void textAlignSet(ZF_IN ZFUITextView *textView,
                              ZF_IN ZFUIAlignFlags const &textAlign) = 0;
    /** @brief see #ZFUITextView */
    virtual void textColorSet(ZF_IN ZFUITextView *textView,
                              ZF_IN ZFUIColor const &textColor) = 0;
    /** @brief see #ZFUITextView */
    virtual void textShadowColorSet(ZF_IN ZFUITextView *textView,
                                    ZF_IN ZFUIColor const &textShadowColor) = 0;
    /** @brief see #ZFUITextView */
    virtual void textShadowOffsetSet(ZF_IN ZFUITextView *textView,
                                     ZF_IN ZFUISize const &textShadowOffset) = 0;
    /** @brief see #ZFUITextView */
    virtual void textSizeSet(ZF_IN ZFUITextView *textView,
                             ZF_IN zfint textSize) = 0;
    /** @brief see #ZFUITextView */
    virtual void textSizeAutoChangeMinSizeSet(ZF_IN ZFUITextView *textView,
                                              ZF_IN zfint textSizeAutoChangeMinSize) = 0;
    /** @brief see #ZFUITextView */
    virtual void textSizeAutoChangeMaxSizeSet(ZF_IN ZFUITextView *textView,
                                              ZF_IN zfint textSizeAutoChangeMaxSize) = 0;
    /** @brief see #ZFUITextView */
    virtual void textSingleLineSet(ZF_IN ZFUITextView *textView,
                                   ZF_IN zfbool textSingleLine) = 0;
    /** @brief see #ZFUITextView */
    virtual void textTruncateModeSet(ZF_IN ZFUITextView *textView,
                                     ZF_IN ZFUITextTruncateModeEnum const &textTruncateMode) = 0;

    // ============================================================
    // layout
public:
    /**
     * @brief measure text view using size hint
     *
     * implementation should truncate or wrap new line according to text style previously set by setTextStyle\n
     * sizeHint shows max size the text view may have, or 0 if no limit
     */
    virtual ZFUISize measureNativeTextView(ZF_IN ZFUITextView *textView,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN zfint textSize) = 0;

    /**
     * @brief see #ZFUITextView::textSizeCurrent
     */
    virtual zfint textSizeCurrent(ZF_IN ZFUITextView *textView) = 0;

    /**
     * @brief layout text view after ZFUIView's layout step,
     *   so that the view may shrink text size if necessary
     */
    virtual void layoutNativeTextView(ZF_IN ZFUITextView *textView,
                                      ZF_IN const ZFUISize &viewSize) = 0;

    // ============================================================
    // util method
public:
    /**
     * @brief util method to calculate text size accorrding auto change setting for this text view
     *
     * usually used by implementation that doesn't support text auto resizing,
     * loop to measure and calculate proper text size,
     * may have performance issues
     */
    zfint calcTextSizeAutoChange(ZF_IN ZFUITextView *textView,
                                 ZF_IN const ZFUISize &sizeHint)
    {
        zfint curTextSize = textView->textSize();
        ZFUISize curSize = this->measureNativeTextView(textView, sizeHint, ZFUISizeApplyScale(curTextSize, textView->scaleGetFixed()));

        if(textView->textSizeAutoChangeMaxSize() > 0)
        {
            while(curTextSize < textView->textSizeAutoChangeMaxSize()
                && ((sizeHint.width <= 0 || curSize.width < sizeHint.width)
                    && (sizeHint.height <= 0 || curSize.height < sizeHint.height)))
            {
                ++curTextSize;
                curSize = this->measureNativeTextView(textView, sizeHint, ZFUISizeApplyScale(curTextSize, textView->scaleGetFixed()));
            }
        }

        if(textView->textSizeAutoChangeMinSize() > 0)
        {
            while(curTextSize > textView->textSizeAutoChangeMinSize()
                && ((sizeHint.width > 0 && curSize.width > sizeHint.width)
                    || (sizeHint.height > 0 && curSize.height > sizeHint.height)))
            {
                --curTextSize;
                curSize = this->measureNativeTextView(textView, sizeHint, ZFUISizeApplyScale(curTextSize, textView->scaleGetFixed()));
            }
        }

        return ZFUISizeApplyScale(curTextSize, textView->scaleGetFixed());
    }
ZFPROTOCOL_INTERFACE_END(ZFUITextView)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUITextView_h_

