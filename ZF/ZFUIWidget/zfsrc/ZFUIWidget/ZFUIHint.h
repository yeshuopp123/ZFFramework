/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIHint.h
 * @brief a quick UI hint to show short messages
 */

#ifndef _ZFI_ZFUIHint_h_
#define _ZFI_ZFUIHint_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief default hint duration, 1500 by default
 */
extern ZF_ENV_EXPORT zftimet ZFUIHintDurationDefault;

// ============================================================
zfclassFwd _ZFP_ZFUIHintPrivate;
/**
 * @brief a quick UI hint to show short messages
 *
 * @note can only be created or show or hide in main thread
 */
zfclass ZF_ENV_EXPORT ZFUIHint : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIHint, ZFObject)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to init the hint object,
     * typically used for one-time init for each hint object,
     * to update app-wide default setting
     */
    ZFOBSERVER_EVENT(HintOnInit)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to update the hint object before show (each time)
     */
    ZFOBSERVER_EVENT(HintOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when the hint showed
     */
    ZFOBSERVER_EVENT(HintOnShow)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when the hint hide
     */
    ZFOBSERVER_EVENT(HintOnHide)

public:
    /**
     * @brief get hint list in specified #ZFUISysWindow
     */
    static ZFCoreArrayPOD<ZFUIHint *> hintList(ZF_IN_OPT ZFUISysWindow *inSysWindow = zfnull);

public:
    /**
     * @brief the window to hold the #hintContent
     *
     * by default, the window would:
     * -  have #ZFUIWindowLevel::e_ZFFrameworkFgHighest as #ZFUIWindow::windowLevel
     * -  have #ZFUIView::viewUIEnableTree been disabled
     * -  have #ZFUIGlobalStyle::itemSizeText as #ZFUIView::viewSizeMin
     * -  have #ZFUIWindow::windowLayoutParam as:
     *   -  #ZFUISizeParamWrapWidthWrapHeight as #ZFUIViewLayoutParam::sizeParam
     *   -  #ZFUIAlign::e_Center as #ZFUIViewLayoutParam::layoutAlign
     *   -  #ZFUIGlobalStyle::itemMargin as #ZFUIViewLayoutParam::layoutMargin
     *
     * you should update the layout param of the hint window instead of the hint view,
     * and the hint view would be layouted accorrding the hintWindow's layout param
     */
    zffinal ZFUIWindow *hintWindow(void);

public:
    /**
     * @brief the actual hint view
     */
    ZFPROPERTY_RETAIN(ZFUIView *, hintContent)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUIView *, hintContent);
    /**
     * @brief the animation to show the hint, null by default
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, hintAniShow)
    /**
     * @brief hint duration in miliseconds, #ZFUIHintDurationDefault by default
     *
     * @note show and hide animation won't be included in #hintDuration
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, hintDuration, ZFPropertyInitValue(ZFUIHintDurationDefault))
    /**
     * @brief the animation to hide the hint, null by default
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, hintAniHide)

    /**
     * @brief whether automatically fix frame accorrding to #ZFUIOnScreenKeyboardAutoResizeStart,
     *   true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, hintWindowAutoResize, ZFPropertyInitValue(zftrue))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfbool, hintWindowAutoResize);

public:
    /**
     * @brief show the hint with animation, do nothing if showing
     *
     * if there's another hint showing,
     * this one would be wait until all previous hint hide\n
     * the hint object would be retained when show,
     * and would be released automatically after hide
     */
    zffinal void hintShow(void);
    /**
     * @brief hide the hint with animation,
     *   do nothing if not showing,
     *   cancel show task if delaying
     */
    zffinal void hintHide(void);
    /**
     * @brief whether the hint is showing or delaying, see #hintDelaying
     */
    zffinal zfbool hintShowing(void);
    /**
     * @brief whether the hint is delaying, see #hintShowing
     */
    zffinal zfbool hintDelaying(void);
    /**
     * @brief return the animation if animating, or null if not
     */
    zffinal ZFAnimation *hintAnimating(void);

protected:
    /** @brief see #ZFUIHint::EventHintOnInit */
    virtual inline void hintOnInit(void)
    {
        this->observerNotify(ZFUIHint::EventHintOnInit());
    }
    /** @brief see #ZFUIHint::EventHintOnUpdate */
    virtual inline void hintOnUpdate(void)
    {
        this->observerNotify(ZFUIHint::EventHintOnUpdate());
    }
    /** @brief see #ZFUIHint::EventHintOnShow */
    virtual inline void hintOnShow(void)
    {
        this->observerNotify(ZFUIHint::EventHintOnShow());
    }
    /** @brief see #ZFUIHint::EventHintOnHide */
    virtual inline void hintOnHide(void)
    {
        this->observerNotify(ZFUIHint::EventHintOnHide());
    }

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        if(this->hintContent() != zfnull)
        {
            ret += zfText(", content: ");
            this->hintContent()->objectInfoT(ret);
        }
    }

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFUIHintPrivate *d;
    friend zfclassFwd _ZFP_ZFUIHintPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIHint_h_

#include "ZFUIHint_Basic.h"

