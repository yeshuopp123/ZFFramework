/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFAnimationNativeView.h
 * @brief native animation
 */

#ifndef _ZFI_ZFAnimationNativeView_h_
#define _ZFI_ZFAnimationNativeView_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief native animation's curve
 */
ZFENUM_BEGIN(ZFAnimationNativeViewCurve)
    ZFENUM_VALUE(Linear) /**< @brief linear */
    ZFENUM_VALUE(EaseIn) /**< @brief slow to fast */
    ZFENUM_VALUE(EaseOut) /**< @brief fast to slow */
    ZFENUM_VALUE(EaseInOut) /**< @brief slow to fast to slow */
ZFENUM_SEPARATOR(ZFAnimationNativeViewCurve)
    ZFENUM_VALUE_REGISTER(Linear)
    ZFENUM_VALUE_REGISTER(EaseIn)
    ZFENUM_VALUE_REGISTER(EaseOut)
    ZFENUM_VALUE_REGISTER(EaseInOut)
ZFENUM_END(ZFAnimationNativeViewCurve)

// ============================================================
zfclassFwd _ZFP_ZFAnimationNativeViewPrivate;
/**
 * @brief animation using native implementation
 */
zfclass ZF_ENV_EXPORT ZFAnimationNativeView : zfextends ZFAnimation
{
    ZFOBJECT_DECLARE(ZFAnimationNativeView, ZFAnimation)

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);
public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

    // ============================================================
public:
    /**
     * @brief native animation
     */
    virtual void *nativeAnimation(void);

    // ============================================================
    // property
public:
    /**
     * @brief whether automatically disable #ZFAnimation::aniTarget while animating, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, aniTargetAutoDisable,
                                ZFPropertyInitValue(zftrue))

    /**
     * @brief animation's curve, #ZFAnimationNativeViewCurve::e_Linear by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFAnimationNativeViewCurveEnum, aniCurve,
                                ZFPropertyInitValue(ZFAnimationNativeViewCurve::e_Linear))

    /**
     * @brief alpha from, 1 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniAlphaFrom,
                                ZFPropertyInitValue(1.0f))
    /**
     * @brief alpha to, 1 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniAlphaTo,
                                ZFPropertyInitValue(1.0f))

    /**
     * @brief scale x from, 1 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniScaleXFrom,
                                ZFPropertyInitValue(1.0f))
    /**
     * @brief scale x to, 1 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniScaleXTo,
                                ZFPropertyInitValue(1.0f))
    /**
     * @brief scale y from, 1 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniScaleYFrom,
                                ZFPropertyInitValue(1.0f))
    /**
     * @brief scale y to, 1 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniScaleYTo,
                                ZFPropertyInitValue(1.0f))
    /**
     * @brief scale y from, 1 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniScaleZFrom,
                                ZFPropertyInitValue(1.0f))
    /**
     * @brief scale y to, 1 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniScaleZTo,
                                ZFPropertyInitValue(1.0f))

    /**
     * @brief translate x from, 0 by default
     *
     * this value is relative to target view's size
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniTranslateXFrom,
                                ZFPropertyInitValue(0))
    /**
     * @brief translate x to, 0 by default
     *
     * this value is relative to target view's size
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniTranslateXTo,
                                ZFPropertyInitValue(0))
    /**
     * @brief translate y from, 0 by default
     *
     * this value is relative to target view's size
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniTranslateYFrom,
                                ZFPropertyInitValue(0))
    /**
     * @brief translate y to, 0 by default
     *
     * this value is relative to target view's size
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniTranslateYTo,
                                ZFPropertyInitValue(0))
    /**
     * @brief translate y from, 0 by default
     *
     * this value is relative to target view's size
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniTranslateZFrom,
                                ZFPropertyInitValue(0))
    /**
     * @brief translate y to, 0 by default
     *
     * this value is relative to target view's size
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, aniTranslateZTo,
                                ZFPropertyInitValue(0))

    /**
     * @brief translate x from, 0 by default
     *
     * this value is absolute pixel size
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniTranslatePixelXFrom,
                                ZFPropertyInitValue(0))
    /**
     * @brief translate x to, 0 by default
     *
     * this value is absolute pixel size
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniTranslatePixelXTo,
                                ZFPropertyInitValue(0))
    /**
     * @brief translate y from, 0 by default
     *
     * this value is absolute pixel size
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniTranslatePixelYFrom,
                                ZFPropertyInitValue(0))
    /**
     * @brief translate y to, 0 by default
     *
     * this value is absolute pixel size
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniTranslatePixelYTo,
                                ZFPropertyInitValue(0))
    /**
     * @brief translate y from, 0 by default
     *
     * this value is absolute pixel size
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniTranslatePixelZFrom,
                                ZFPropertyInitValue(0))
    /**
     * @brief translate y to, 0 by default
     *
     * this value is absolute pixel size
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniTranslatePixelZTo,
                                ZFPropertyInitValue(0))

    /**
     * @brief rotate x from, 0 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniRotateXFrom,
                                ZFPropertyInitValue(0))
    /**
     * @brief rotate x to, 0 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniRotateXTo,
                                ZFPropertyInitValue(0))
    /**
     * @brief rotate y from, 0 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniRotateYFrom,
                                ZFPropertyInitValue(0))
    /**
     * @brief rotate y to, 0 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniRotateYTo,
                                ZFPropertyInitValue(0))
    /**
     * @brief rotate z from, 0 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniRotateZFrom,
                                ZFPropertyInitValue(0))
    /**
     * @brief rotate z to, 0 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniRotateZTo,
                                ZFPropertyInitValue(0))

    // ============================================================
    // start stop
protected:
    zfoverride
    virtual zfbool aniImplCheckValid(void);
    zfoverride
    virtual void aniOnStart(void);
    zfoverride
    virtual void aniOnStop(void);

protected:
    zfoverride
    virtual void aniImplStart(void);
    zfoverride
    virtual void aniImplStop(void);
public:
    zffinal void _ZFP_ZFAnimationNativeView_notifyStop(void)
    {
        this->aniImplNotifyStop();
    }

private:
    _ZFP_ZFAnimationNativeViewPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAnimationNativeView_h_

