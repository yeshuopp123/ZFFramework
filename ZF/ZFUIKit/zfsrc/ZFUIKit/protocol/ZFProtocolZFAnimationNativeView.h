/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFAnimationNativeView.h
 * @brief protocol for ZFAnimationNativeView
 */

#ifndef _ZFI_ZFProtocolZFAnimationNativeView_h_
#define _ZFI_ZFProtocolZFAnimationNativeView_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFAnimationNativeView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFAnimationNativeView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFAnimationNativeView)
public:
    /**
     * @brief create native animation
     */
    virtual void *nativeAniCreate(ZF_IN ZFAnimationNativeView *ani) zfpurevirtual;
    /**
     * @brief destroy native animation
     */
    virtual void nativeAniDestroy(ZF_IN ZFAnimationNativeView *ani,
                                  ZF_IN void *nativeAni) zfpurevirtual;

    /**
     * @brief start native animation
     *
     * @note you should copy all settings from the ani
     * @note this method have no auto scale logic,
     *   size unit are scaled size under ZFFramework,
     *   you should apply scale manually by nativeAniScale
     *
     * here's a list of things you need to implement:
     * -  aniCurve (ZFAnimationNativeViewCurveEnum)
     * -  aniAlpha (zffloat)
     * -  aniScaleX/Y/Z (zffloat)
     * -  aniTranslateX/Y/Z (zffloat)
     * -  aniTranslatePixelX/Y/Z (zfint)
     * -  aniRotateX/Y/Z (zfint)
     */
    virtual void nativeAniStart(ZF_IN ZFAnimationNativeView *ani,
                                ZF_IN zffloat nativeAniScale) zfpurevirtual;
    /**
     * @brief stop native animation
     *
     * animation must be stopped immediately
     */
    virtual void nativeAniStop(ZF_IN ZFAnimationNativeView *ani) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementation must call this to notify animation stopped
     */
    zffinal void notifyAniStop(ZF_IN ZFAnimationNativeView *ani)
    {
        ani->_ZFP_ZFAnimationNativeView_notifyStop();
    }
ZFPROTOCOL_INTERFACE_END(ZFAnimationNativeView)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFAnimationNativeView_h_

