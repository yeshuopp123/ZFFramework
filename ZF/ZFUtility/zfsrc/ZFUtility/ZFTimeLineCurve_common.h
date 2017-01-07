/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTimeLineCurve_common.h
 * @brief see #ZFTimeLineCurve
 */

#ifndef _ZFI_ZFTimeLineCurve_common_h_
#define _ZFI_ZFTimeLineCurve_common_h_

#include "ZFTimeLineCurve.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief linear time line, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveLinear : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveLinear, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

public:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return time;
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLineCurve_common_h_

