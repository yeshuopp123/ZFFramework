/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTimeLineProperty_ZFUIKit.h
 * @brief see #ZFTimeLineProperty
 */

#ifndef _ZFI_ZFTimeLineProperty_ZFUIKit_h_
#define _ZFI_ZFTimeLineProperty_ZFUIKit_h_

#include "ZFAdditionDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// types

ZFTIMELINEPROPERTY_DECLARE_BY_VALUE_DETAIL(ZFTimeLineProperty_ZFUIPoint, ZFUIPoint, {
    return ZFUIPointMake(
        fromValue.x + (zfint)((toValue.x - fromValue.x) * progress),
        fromValue.y + (zfint)((toValue.y - fromValue.y) * progress)
        );
})

ZFTIMELINEPROPERTY_DECLARE_BY_VALUE_DETAIL(ZFTimeLineProperty_ZFUISize, ZFUISize, {
    return ZFUISizeMake(
        fromValue.width + (zfint)((toValue.width - fromValue.width) * progress),
        fromValue.height + (zfint)((toValue.height - fromValue.height) * progress)
        );
})

ZFTIMELINEPROPERTY_DECLARE_BY_VALUE_DETAIL(ZFTimeLineProperty_ZFUIMargin, ZFUIMargin, {
    return ZFUIMarginMake(
        fromValue.left + (zfint)((toValue.left - fromValue.left) * progress),
        fromValue.top + (zfint)((toValue.top - fromValue.top) * progress),
        fromValue.right + (zfint)((toValue.right - fromValue.right) * progress),
        fromValue.bottom + (zfint)((toValue.bottom - fromValue.bottom) * progress)
        );
})

ZFTIMELINEPROPERTY_DECLARE_BY_VALUE_DETAIL(ZFTimeLineProperty_ZFUIRect, ZFUIRect, {
    return ZFUIRectMake(
        fromValue.point.x + (zfint)((toValue.point.x - fromValue.point.x) * progress),
        fromValue.point.y + (zfint)((toValue.point.y - fromValue.point.y) * progress),
        fromValue.size.width + (zfint)((toValue.size.width - fromValue.size.width) * progress),
        fromValue.size.height + (zfint)((toValue.size.height - fromValue.size.height) * progress)
        );
})

ZFTIMELINEPROPERTY_DECLARE_BY_VALUE_DETAIL(ZFTimeLineProperty_ZFUIColor, ZFUIColor, {
    return ZFUIColorMake(
        ZFUIColorGetA(fromValue) + (zfint)((ZFUIColorGetA(toValue) - ZFUIColorGetA(fromValue)) * progress),
        ZFUIColorGetR(fromValue) + (zfint)((ZFUIColorGetR(toValue) - ZFUIColorGetR(fromValue)) * progress),
        ZFUIColorGetG(fromValue) + (zfint)((ZFUIColorGetG(toValue) - ZFUIColorGetG(fromValue)) * progress),
        ZFUIColorGetB(fromValue) + (zfint)((ZFUIColorGetB(toValue) - ZFUIColorGetB(fromValue)) * progress)
        );
})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLineProperty_ZFUIKit_h_

