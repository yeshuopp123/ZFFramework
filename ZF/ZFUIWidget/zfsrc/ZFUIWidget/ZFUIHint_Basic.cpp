/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIHint_Basic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// hint with simple text and icon
ZFSTYLE_DEFAULT_DEFINE(ZFUIHintContentBasic)

zfautoObject ZFUIHintMake(ZF_IN const zfchar *text,
                          ZF_IN_OPT ZFUIImage *icon /* = zfnull */)
{
    zfblockedAlloc(ZFUIHint, hint);

    zfblockedAlloc(ZFAnimationNativeView, hintAniShow);
    hint->hintAniShowSet(hintAniShow);
    hintAniShow->aniAlphaFromSet(0);
    zfblockedAlloc(ZFAnimationNativeView, hintAniHide);
    hint->hintAniHideSet(hintAniHide);
    hintAniHide->aniAlphaToSet(0);

    zfblockedAlloc(ZFUIHintContentBasic, hintContent);
    hint->hintContentSet(hintContent);
    hintContent->buttonLabelTextStringSet(text);
    hintContent->buttonIconImageSet(icon);

    return zfautoObjectCreate(hint);
}

ZF_NAMESPACE_GLOBAL_END

