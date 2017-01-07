/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIViewBlink.h
 * @brief blink view so that it's easy to be noticed
 */

#ifndef _ZFI_ZFUIViewBlink_h_
#define _ZFI_ZFUIViewBlink_h_

#include "ZFUIView.h"
#include "ZFUIImageIO.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief default mask image for #ZFUIViewBlink, an alpha white image by default
 *
 * this property would be initialized and destroyed automatically during #ZFFrameworkCleanup
 * as level #ZFLevelZFFrameworkLow
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIViewBlinkImageDefault;

/**
 * @brief param for #ZFUIViewBlink
 */
zfclassLikePOD ZF_ENV_EXPORT ZFUIViewBlinkParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFUIViewBlinkParam)

public:
    /** @brief image to blink, null to use default */
    ZFCORE_PARAM(ZFUIImage *, blinkImage)

    /** @brief duration to blink, #ZFUIGlobalStyle::aniDurationNormal by default */
    ZFCORE_PARAM_WITH_INIT(zftimet, blinkDuration, ZFUIGlobalStyle::DefaultStyle()->aniDurationNormal())
    /** @brief blink count, 1 by default */
    ZFCORE_PARAM_WITH_INIT(zfindex, blinkCount, 1)
};
/**
 * @brief blink view so that it's easy to be noticed
 */
extern ZF_ENV_EXPORT void ZFUIViewBlink(ZF_IN ZFUIView *view,
                                        ZF_IN const ZFUIViewBlinkParam &blinkParam = ZFUIViewBlinkParam());
/**
 * @brief blink view so that it's easy to be noticed
 */
inline void ZFUIViewBlink(ZF_IN ZFUIView *view,
                          ZF_IN ZFUIImage *blinkImage,
                          ZF_IN_OPT zftimet blinkDuration = ZFUIGlobalStyle::DefaultStyle()->aniDurationNormal(),
                          ZF_IN_OPT zfindex blinkCount = 1)
{
    ZFUIViewBlink(view, ZFUIViewBlinkParam()
            .blinkImageSet(blinkImage)
            .blinkDurationSet(blinkDuration)
            .blinkCountSet(blinkCount)
        );
}

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when a view start to blink
 */
ZFOBSERVER_EVENT_GLOBAL(ViewBlinkOn)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when a view stop blink
 */
ZFOBSERVER_EVENT_GLOBAL(ViewBlinkOff)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewBlink_h_

