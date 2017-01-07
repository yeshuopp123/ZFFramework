/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIListCell.h
 * @brief abstract list cell to hold list content views
 */

#ifndef _ZFI_ZFUIListCell_h_
#define _ZFI_ZFUIListCell_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIListCell
/**
 * @brief utility class to hold list cell
 *
 * a #ZFUIListView won't supply separators or margins or border radius directly,
 * it's achieved by a wrapper view, which is this #ZFUIListCell\n
 * typically, the extra features are implemented by utility methods described in #ZFUIListCellUpdater
 */
zfclass ZF_ENV_EXPORT ZFUIListCell : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIListCell, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIListCell)

    /**
     * @brief the content view
     */
    ZFPROPERTY_RETAIN(ZFUIView *, cellView)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUIView *, cellView);

    /**
     * @brief content view's layout param,
     *   usually would be updated by #ZFUIListCellUpdater
     *   to achieve additional features,
     *   fill parent and align center by default
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUIViewLayoutParam *, cellViewLayoutParam,
                               ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIViewLayoutParam *, cellViewLayoutParam)
    {
        propertyValue = this->layoutParamCreate();
        ZFUIViewLayoutParam *value = propertyValue.to<ZFUIViewLayoutParam *>();
        value->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        value->layoutAlignSet(ZFUIAlign::e_Center);
    }

protected:
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListCell_h_

