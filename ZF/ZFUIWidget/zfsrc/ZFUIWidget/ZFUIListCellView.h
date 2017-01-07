/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIListCellView.h
 * @brief abstract list content view
 */

#ifndef _ZFI_ZFUIListCellView_h_
#define _ZFI_ZFUIListCellView_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIListCellView
/**
 * @brief abstract list content view
 */
zfabstract ZF_ENV_EXPORT ZFUIListCellView : zfextends ZFUIView
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFUIListCellView, ZFUIView)

public:
    /**
     * @brief container for icon, you may add your own custom view
     */
    virtual ZFUIView *cellIconContainer(void) = 0;
    /**
     * @brief container for center item, you may add your own custom view
     */
    virtual ZFUIView *cellCenterContainer(void) = 0;
    /**
     * @brief container for accessory, you may add your own custom view
     */
    virtual ZFUIView *cellAccessoryContainer(void) = 0;

public:
    zfoverride
    virtual void objectOnInitFinish(void);

protected:
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void internalBackgroundViewOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListCellView_h_

