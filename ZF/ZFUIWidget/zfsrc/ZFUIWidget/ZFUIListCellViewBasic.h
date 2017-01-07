/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIListCellViewBasic.h
 * @brief basic list cell view with icon and label
 */

#ifndef _ZFI_ZFUIListCellViewBasic_h_
#define _ZFI_ZFUIListCellViewBasic_h_

#include "ZFUIListCellView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIListCellViewBasic
zfclassFwd _ZFP_ZFUIListCellViewBasicPrivate;
/**
 * @brief basic list cell view with icon and label
 */
zfclass ZF_ENV_EXPORT ZFUIListCellViewBasic : zfextends ZFUIListCellView
{
    ZFOBJECT_DECLARE(ZFUIListCellViewBasic, ZFUIListCellView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIListCellViewBasic)

public:
    // ============================================================
    // properties
    /**
     * @brief cell's icon
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, cellIcon, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageView *, cellIcon)
    {
        zfblockedAlloc(ZFUIImageView, cellIcon);
        propertyValue = zfautoObjectCreateWithoutLeakTest(cellIcon);
        cellIcon->viewSizeMaxSet(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeIcon()));
    }

    /**
     * @brief cell's main label
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, cellLabelMain, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextView *, cellLabelMain)
    {
        zfblockedAlloc(ZFUITextView, cellLabelMain);
        propertyValue = zfautoObjectCreateWithoutLeakTest(cellLabelMain);
    }

    /**
     * @brief cell's sub label
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, cellLabelSub, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextView *, cellLabelSub)
    {
        zfblockedAlloc(ZFUITextView, cellLabelSub);
        propertyValue = zfautoObjectCreateWithoutLeakTest(cellLabelSub);
        cellLabelSub->textColorSet(ZFUIGlobalStyle::DefaultStyle()->textColorSecondary());
        cellLabelSub->textSizeSet(ZFUIGlobalStyle::DefaultStyle()->textSizeSmall());
    }

public:
    /**
     * @brief container for icon, you may add your own custom view
     */
    zffinal ZFUIView *cellIconContainer(void);
    /**
     * @brief container for center item, you may add your own custom view
     */
    zffinal ZFUIView *cellCenterContainer(void);
    /**
     * @brief container for main label, you may add your own custom view
     */
    zffinal ZFUIView *cellLabelMainContainer(void);
    /**
     * @brief container for sub label, you may add your own custom view
     */
    zffinal ZFUIView *cellLabelSubContainer(void);
    /**
     * @brief container for accessory, you may add your own custom view
     */
    zffinal ZFUIView *cellAccessoryContainer(void);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void objectPropertyValueOnInit(ZF_IN const ZFProperty *property);

private:
    _ZFP_ZFUIListCellViewBasicPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListCellViewBasic_h_

