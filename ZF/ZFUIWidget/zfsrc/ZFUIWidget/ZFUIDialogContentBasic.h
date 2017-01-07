/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIDialogContentBasic.h
 * @brief basic content for #ZFUIDialogBasic
 */

#ifndef _ZFI_ZFUIDialogContentBasic_h_
#define _ZFI_ZFUIDialogContentBasic_h_

#include "ZFUIDialogBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDialogContentBasic
zfclassFwd _ZFP_ZFUIDialogContentBasicPrivate;
/**
 * @brief basic content for #ZFUIDialogBasic
 */
zfclass ZF_ENV_EXPORT ZFUIDialogContentBasic : zfextends ZFUIView, zfimplements ZFUIDialogContent
{
    ZFOBJECT_DECLARE(ZFUIDialogContentBasic, ZFUIView)
    ZFIMPLEMENTS_DECLARE(ZFUIDialogContent)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogContentBasic)

public:
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN_NO_AUTO_INIT(ZFUISize, viewSizeMin, ZFUISizeMake(
        ZFUIGlobalStyle::DefaultStyle()->itemSizeDialogWidth(),
        ZFUIGlobalStyle::DefaultStyle()->itemSizeDialog()))

    /**
     * @brief dialog title view
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, titleTextView, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextView *, titleTextView)
    {
        zfblockedAlloc(ZFUITextView, titleTextView);
        propertyValue = zfautoObjectCreateWithoutLeakTest(titleTextView);
        titleTextView->textSizeSet(ZFUIGlobalStyle::DefaultStyle()->textSizeBig());
        titleTextView->textAppearanceSet(ZFUITextAppearance::e_Bold);
    }

    /**
     * @brief dialog content view
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, contentTextView, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextView *, contentTextView)
    {
        zfblockedAlloc(ZFUITextView, contentTextView);
        propertyValue = zfautoObjectCreateWithoutLeakTest(contentTextView);
        contentTextView->textSingleLineSet(zffalse);
    }

public:
    // ============================================================
    // title
    zfoverride
    virtual ZFUIView *dialogTitleContainer(void);
    zfoverride
    virtual ZFUITextView *dialogTitleView(void);

    // ============================================================
    // content
    zfoverride
    virtual ZFUIView *dialogContentContainer(void);
    zfoverride
    virtual ZFUITextView *dialogContentView(void);

    // ============================================================
    // button
    zfoverride
    virtual ZFUIView *dialogButtonContainer(void);
    zfoverride
    virtual ZFUIButton *dialogButton(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                     ZF_IN_OPT zfbool autoCreateIfNotExist = zftrue);
    zfoverride
    virtual const zfchar *dialogButtonText(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType);
    zfoverride
    virtual void dialogButtonTextSet(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                     ZF_IN const zfchar *text);
    zfoverride
    virtual void dialogButtonRemove(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType);

    // ============================================================
    // button
    zfoverride
    virtual zfindex dialogButtonCount(void);
    zfoverride
    virtual ZFUIButton *dialogButtonAtIndex(ZF_IN zfindex index);
    zfoverride
    virtual zfindex dialogButtonFind(ZF_IN ZFUIButton *dialogButton);
    zfoverride
    virtual void dialogButtonAdd(ZF_IN ZFUIButton *button,
                                 ZF_IN_OPT zfindex atIndex = zfindexMax);
    zfoverride
    virtual void dialogButtonRemove(ZF_IN ZFUIButton *button);
    zfoverride
    virtual void dialogButtonRemoveAtIndex(ZF_IN zfindex index);
    zfoverride
    virtual void dialogButtonRemoveAll(void);

    // ============================================================
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void internalForegroundViewOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView);

private:
    _ZFP_ZFUIDialogContentBasicPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialogContentBasic_h_

