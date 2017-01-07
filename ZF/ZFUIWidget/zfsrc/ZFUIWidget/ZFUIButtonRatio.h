/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIButtonRatio.h
 * @brief check box button
 */

#ifndef _ZFI_ZFUIButtonRatio_h_
#define _ZFI_ZFUIButtonRatio_h_

#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonRatio
/**
 * @brief check box button, see #ZFUIButtonBasic
 */
zfclass ZF_ENV_EXPORT ZFUIButtonRatio : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZFUIButtonRatio, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonRatio)

public:
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN(zfbool, buttonCheckable)
    {
        propertyValue = zftrue;
    }

    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN_NO_AUTO_INIT(ZFUIAlignFlags, buttonContentAlign)
    {
        propertyValue = ZFUIAlign::e_LeftInner;
    }

    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN(ZFUIImageView *, buttonIconStyleNormal)
    {
        propertyValue.to<ZFUIImageView *>()->imageContentSet(ZFUIImageRes(zfText("ZFUIWidget/ZFUIButtonRatio_IconNormal.png")));
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageView *, buttonIconStyleHighlighted)
    {
        propertyValue.to<ZFUIImageView *>()->imageContentSet(ZFUIImageRes(zfText("ZFUIWidget/ZFUIButtonRatio_IconHighlighted.png")));
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageView *, buttonIconStyleChecked)
    {
        propertyValue.to<ZFUIImageView *>()->imageContentSet(ZFUIImageRes(zfText("ZFUIWidget/ZFUIButtonRatio_IconChecked.png")));
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageView *, buttonIconStyleCheckedHighlighted)
    {
        propertyValue.to<ZFUIImageView *>()->imageContentSet(ZFUIImageRes(zfText("ZFUIWidget/ZFUIButtonRatio_IconCheckedHighlighted.png")));
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonRatio_h_

