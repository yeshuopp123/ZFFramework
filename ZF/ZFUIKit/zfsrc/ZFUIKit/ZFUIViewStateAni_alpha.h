/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIViewStateAni_alpha.h
 * @brief alpha animation for #ZFUIViewStateAniStart
 */

#ifndef _ZFI_ZFUIViewStateAni_alpha_h_
#define _ZFI_ZFUIViewStateAni_alpha_h_

#include "ZFUIViewStateAni.h"
#include "ZFAnimationNativeView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief alpha animation for #ZFUIViewStateAniStart
 *
 * if a view's #ZFUIView::layoutedFramePrev's size is zero but #ZFUIView::layoutedFrame is not,
 * a alpha animation of #ZFAnimationNativeView would be applied
 */
zfclass ZF_ENV_EXPORT ZFUIViewStateAniSettingForAlpha : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUIViewStateAniSettingForAlpha, ZFStyleableObject)
    ZFOBJECT_SINGLETON_DECLARE_DETAIL(private, public, ZFUIViewStateAniSettingForAlpha, instance)

    /**
     * @brief whether enable this ani
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, aniEnable, ZFPropertyInitValue(zftrue))
    /**
     * @brief filter to ignore views from being auto animated, empty by default
     */
    ZFFilterForZFObject aniFilter;
    /**
     * @brief minimum offset to apply the animation, #ZFUIGlobalStyle::itemSpace by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniToleranceX, ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->itemSpace()))
    /**
     * @brief minimum offset to apply the animation, #ZFUIGlobalStyle::itemSpace by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniToleranceY, ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->itemSpace()))
    /**
     * @brief ani duration, #ZFUIGlobalStyle::aniDurationLong by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniDuration, ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->aniDurationLong()))
    /**
     * @brief ani curve, #ZFAnimationNativeViewCurve::e_EaseOut by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFAnimationNativeViewCurveEnum, aniCurve, ZFPropertyInitValue(ZFAnimationNativeViewCurve::e_EaseOut))
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewStateAni_alpha_h_

