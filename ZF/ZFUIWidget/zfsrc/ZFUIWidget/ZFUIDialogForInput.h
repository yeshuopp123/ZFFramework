/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIDialogForInput.h
 * @brief basic dialog for user to input a short text
 */

#ifndef _ZFI_ZFUIDialogForInput_h_
#define _ZFI_ZFUIDialogForInput_h_

#include "ZFUIDialogBasic.h"
#include "ZFUITextEditWidget.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDialogForInput
/**
 * @brief basic dialog for input
 */
zfclass ZF_ENV_EXPORT ZFUIDialogForInput : zfextends ZFUIDialogBasic
{
    ZFOBJECT_DECLARE(ZFUIDialogForInput, ZFUIDialogBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogForInput)

public:
    /**
     * @brief the input text edit view
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextEditWidget *, inputView, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextEditWidget *, inputView)
    {
        zfblockedAlloc(ZFUITextEditWidget, inputView);
        propertyValue = zfautoObjectCreateWithoutLeakTest(inputView);
        inputView->to<ZFUIView *>()->viewSizeMinSet(ZFUISizeMake(
                ZFUIGlobalStyle::DefaultStyle()->itemSizeDialogWidth(),
                ZFUIGlobalStyle::DefaultStyle()->itemSizeControl()
            ));
    }

    /**
     * @brief util method to set input hint text
     */
    virtual inline void inputHintTextSet(ZF_IN const zfchar *text)
    {
        this->inputView()->textPlaceHolder()->textContentStringSet(text);
    }
    /**
     * @brief util method to get input hint text
     */
    virtual inline const zfchar *inputHintText(void)
    {
        return this->inputView()->textPlaceHolder()->textContentString();
    }

    /**
     * @brief util method to set input text
     */
    virtual inline void inputTextSet(ZF_IN const zfchar *text)
    {
        this->inputView()->textContentStringSet(text);
    }
    /**
     * @brief util method to get input text
     */
    virtual inline const zfchar *inputText(void)
    {
        return this->inputView()->textContentString();
    }

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialogForInput_h_

