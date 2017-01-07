/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFUITextEdit.h
 * @brief protocol for #ZFUITextEdit
 */

#ifndef _ZFI_ZFProtocolZFUITextEdit_h_
#define _ZFI_ZFProtocolZFUITextEdit_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUITextEdit.h"
#include "ZFCore/protocol/ZFProtocolZFString.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFUITextEdit
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUITextEdit)
public:
    /**
     * @brief create native text edit view
     */
    virtual void *nativeTextEditCreate(ZF_IN ZFUITextEdit *textEdit) = 0;
    /**
     * @brief destroy native text edit view
     */
    virtual void nativeTextEditDestroy(ZF_IN ZFUITextEdit *textEdit,
                                       ZF_IN void *nativeTextEdit) = 0;

    // ============================================================
    // properties
public:
    /** @brief see #ZFUITextEdit */
    virtual void textEditEnableSet(ZF_IN ZFUITextEdit *textEdit,
                                   ZF_IN zfbool textEditEnable) = 0;
    /** @brief see #ZFUITextEdit */
    virtual void textEditSecuredSet(ZF_IN ZFUITextEdit *textEdit,
                                    ZF_IN zfbool textEditSecured) = 0;
    /** @brief see #ZFUITextEdit */
    virtual void textEditKeyboardTypeSet(ZF_IN ZFUITextEdit *textEdit,
                                         ZF_IN ZFUITextEditKeyboardTypeEnum textEditKeyboardType) = 0;
    /** @brief see #ZFUITextEdit */
    virtual void textEditKeyboardReturnTypeSet(ZF_IN ZFUITextEdit *textEdit,
                                               ZF_IN ZFUITextEditKeyboardReturnTypeEnum textEditKeyboardReturnType) = 0;

    /** @brief see #ZFUITextEdit */
    virtual void textSelectRange(ZF_IN ZFUITextEdit *textEdit, ZF_OUT zfindexRange &textSelectRange) = 0;
    /** @brief see #ZFUITextEdit */
    virtual void textSelectRangeSet(ZF_IN ZFUITextEdit *textEdit, ZF_IN const zfindexRange &textSelectRange) = 0;

public:
    /** @brief see #ZFUITextEdit */
    virtual void textContentSet(ZF_IN ZFUITextEdit *textEdit,
                                ZF_IN ZFString *text) = 0;
    /** @brief see #ZFUITextEdit */
    virtual void textAppearanceSet(ZF_IN ZFUITextEdit *textEdit,
                                   ZF_IN ZFUITextAppearanceEnum const &textAppearance) = 0;
    /** @brief see #ZFUITextEdit */
    virtual void textAlignSet(ZF_IN ZFUITextEdit *textEdit,
                              ZF_IN ZFUIAlignFlags const &textAlign) = 0;
    /** @brief see #ZFUITextEdit */
    virtual void textColorSet(ZF_IN ZFUITextEdit *textEdit,
                              ZF_IN ZFUIColor const &textColor) = 0;
    /** @brief see #ZFUITextEdit */
    virtual void textShadowColorSet(ZF_IN ZFUITextEdit *textEdit,
                                    ZF_IN ZFUIColor const &textShadowColor) = 0;
    /** @brief see #ZFUITextEdit */
    virtual void textShadowOffsetSet(ZF_IN ZFUITextEdit *textEdit,
                                     ZF_IN ZFUISize const &textShadowOffset) = 0;
    /** @brief see #ZFUITextEdit */
    virtual void textSizeSet(ZF_IN ZFUITextEdit *textEdit,
                             ZF_IN zfint textSize) = 0;

    // ============================================================
    // layout
public:
    /**
     * @brief measure text edit view using size hint
     *
     * implementation should truncate or wrap new line according to text style previously set by setTextStyle\n
     * sizeHint shows max size the text edit view may have, or 0 if no limit
     */
    virtual ZFUISize measureNativeTextEdit(ZF_IN ZFUITextEdit *textEdit,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN zfint textSize) = 0;

    // ============================================================
    // edit
public:
    /** @brief see #ZFUITextEdit::textEditBegin */
    virtual void textEditBegin(ZF_IN ZFUITextEdit *textEdit) = 0;
    /** @brief see #ZFUITextEdit::textEditEnd */
    virtual void textEditEnd(ZF_IN ZFUITextEdit *textEdit) = 0;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementation must notify to check whether text should change
     */
    zffinal zfbool notifyCheckTextShouldChange(ZF_IN ZFUITextEdit *textEdit,
                                               ZF_IN ZFString *newValue)
    {
        return textEdit->textShouldChange(newValue);
    }
    /**
     * @brief implementation must notify when text changed
     */
    zffinal void notifyTextChange(ZF_IN ZFUITextEdit *textEdit,
                                  ZF_IN ZFString *newValue)
    {
        textEdit->_ZFP_ZFUITextEdit_textNotifyChange(newValue);
    }
    /**
     * @brief implementation must notify when text changed
     */
    zffinal void notifyTextReturnClicked(ZF_IN ZFUITextEdit *textEdit)
    {
        textEdit->_ZFP_ZFUITextEdit_textNotifyReturnClicked();
    }
    /**
     * @brief implementation must notify when text begin edit
     */
    zffinal void notifyTextEditBegin(ZF_IN ZFUITextEdit *textEdit)
    {
        textEdit->_ZFP_ZFUITextEdit_textNotifyBeginEdit();
    }
    /**
     * @brief implementation must notify when text begin edit
     */
    zffinal void notifyTextEditEnd(ZF_IN ZFUITextEdit *textEdit)
    {
        textEdit->_ZFP_ZFUITextEdit_textNotifyEndEdit();
    }
ZFPROTOCOL_INTERFACE_END(ZFUITextEdit)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUITextEdit_h_

