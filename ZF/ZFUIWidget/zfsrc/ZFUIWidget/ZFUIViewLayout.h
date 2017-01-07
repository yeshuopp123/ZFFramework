/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIViewLayout.h
 * @brief simple container view that measure accorrding children
 */

#ifndef _ZFI_ZFUIViewLayout_h_
#define _ZFI_ZFUIViewLayout_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIViewLayout
/**
 * @brief simple container view that measure accorrding children
 */
zfclass ZF_ENV_EXPORT ZFUIViewLayout : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIViewLayout, ZFUIView)

    // ============================================================
    // override ZFUIView
protected:
    /**
     * @brief #ZFUIViewLayout would measure accorrding all of its children
     */
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewLayout_h_

