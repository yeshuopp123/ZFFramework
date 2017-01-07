/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIOnScreenKeyboardAutoFit.h
 * @brief a scroll container that automatically fits content's position
 *   accorrding #ZFUIOnScreenKeyboardState
 */

#ifndef _ZFI_ZFUIOnScreenKeyboardAutoFit_h_
#define _ZFI_ZFUIOnScreenKeyboardAutoFit_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIOnScreenKeyboardAutoFitLayout
zfclassFwd _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate;
/**
 * @brief a scroll container that automatically fits content's position
 *   accorrding #ZFUIOnScreenKeyboardState
 *
 * recommended set as #ZFUIView::viewDelegateClass
 */
zfclass ZF_ENV_EXPORT ZFUIOnScreenKeyboardAutoFitLayout : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIOnScreenKeyboardAutoFitLayout, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIOnScreenKeyboardAutoFitLayout)

public:
    /**
     * @brief whether enable auto fit, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, autoFitEnable,
                                ZFPropertyInitValue(zftrue))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfbool, autoFitEnable);
    /**
     * @brief whether auto move focused view to visible position, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, autoFitFocusedViewToVisible,
                                ZFPropertyInitValue(zftrue))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfbool, autoFitFocusedViewToVisible);

    /**
     * @brief whether allow manually scroll when auto fit enabled, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, autoFitScrollEnable,
                                ZFPropertyInitValue(zffalse))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfbool, autoFitScrollEnable);

public:
    /**
     * @brief current focused child view inside this layout, null if none
     */
    virtual ZFUIView *autoFitFocusedView(void);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnInitFinish(void);

protected:
    zfoverride
    virtual zfbool viewDelegateSupported(void)
    {
        return zffalse;
    }

    zfoverride
    virtual void viewDelegateLayoutOnMeasure(ZF_OUT ZFUISize &ret,
                                             ZF_IN const ZFUISize &sizeHint,
                                             ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void viewDelegateLayoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);

private:
    _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate *d;
    friend zfclassFwd _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate;
};

// ============================================================
/**
 * @brief util method to apply #ZFUIOnScreenKeyboardAutoFitLayout to a window
 */
extern ZF_ENV_EXPORT void ZFUIOnScreenKeyboardAutoFitStart(ZF_IN ZFUIWindow *window);
/** @brief see #ZFUIOnScreenKeyboardAutoFitStart */
extern ZF_ENV_EXPORT void ZFUIOnScreenKeyboardAutoFitStop(ZF_IN ZFUIWindow *window);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIOnScreenKeyboardAutoFit_h_

