/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIHint_Basic.h
 * @brief common types of #ZFUIHint
 */

#ifndef _ZFI_ZFUIHint_Basic_h_
#define _ZFI_ZFUIHint_Basic_h_

#include "ZFUIHint.h"
#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// hint with simple text and icon
/**
 * @brief #ZFUIHint's content with simple text and background
 */
zfclass ZF_ENV_EXPORT ZFUIHintContentBasic : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZFUIHintContentBasic, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIHintContentBasic)

public:
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(zfbool, viewUIEnableTree, zffalse)
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(ZFUISize, viewSizeMin, ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(), ZFUIGlobalStyle::DefaultStyle()->itemSizeButton()))

    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN(ZFUITextView *, buttonLabelStyleNormal)
    {
        ZFUITextView *value = propertyValue.to<ZFUITextView *>();
        value->textColorSet(ZFUIColorWhite);
        value->textSingleLineSet(zffalse);
        value->textSizeAutoChangeMinSizeSet(0);
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN(ZFUIImageView *, buttonBackgroundStyleNormal)
    {
        ZFUIImageView *value = propertyValue;
        value->imageContentSet(ZFUIImageResXml(zfText("ZFUIWidget/ZFUIHintContentBasic_background.xml")));
    }

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        if(this->buttonLabelTextString() != zfnull)
        {
            ret += zfText(" \"");
            ret += this->buttonLabelTextString();
            ret += zfText("\"");
        }
    }
};

/**
 * @brief make a hint with text
 *
 * the hint would:
 * -  #ZFUIHintContentBasic as the #ZFUIHint::hintContent
 * -  have an alpha change animation as the hint's show and hide animation
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIHintMake(ZF_IN const zfchar *text,
                                               ZF_IN_OPT ZFUIImage *icon = zfnull);
/**
 * @brief #ZFUIHintMake and show the hint
 */
inline zfautoObject ZFUIHintShow(ZF_IN const zfchar *text,
                                 ZF_IN_OPT ZFUIImage *icon = zfnull)
{
    zfautoObject hint = ZFUIHintMake(text, icon);
    hint.to<ZFUIHint *>()->hintShow();
    return hint;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIHint_Basic_h_

