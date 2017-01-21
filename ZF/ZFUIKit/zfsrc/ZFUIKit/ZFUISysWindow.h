/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUISysWindow.h
 * @brief root native window
 */

#ifndef _ZFI_ZFUISysWindow_h_
#define _ZFI_ZFUISysWindow_h_

#include "ZFUIView.h"
#include "ZFUIRootView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFUISysWindowPrivate;
/**
 * @brief root native window
 *
 * use #mainWindow to access app's main window,
 * use #modalWindowShow to show a new window,
 * never create #ZFUISysWindow's instance manually
 */
zffinal zfclass ZF_ENV_EXPORT ZFUISysWindow : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUISysWindow, ZFObject)

    // ============================================================
    // observer events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window created,
     * see #nativeWindowIsCreated for more info
     */
    ZFOBSERVER_EVENT(SysWindowOnCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window about to destroy
     */
    ZFOBSERVER_EVENT(SysWindowOnDestroy)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window become active
     */
    ZFOBSERVER_EVENT(SysWindowOnResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window enter background
     */
    ZFOBSERVER_EVENT(SysWindowOnPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window rotated,
     * new orientation can be accessed by #windowOrientation
     */
    ZFOBSERVER_EVENT(SysWindowOnRotate)

public:
    /**
     * @brief impl for #nativeWindowEmbed
     */
    zfclassNotPOD ZF_ENV_EXPORT NativeWindowEmbedImpl
    {
    public:
        ZFUISysWindow *_ZFP_ownerZFUISysWindow;
        /**
         * @brief the attached #ZFUISysWindow
         */
        inline ZFUISysWindow *ownerZFUISysWindow(void)
        {
            return this->_ZFP_ownerZFUISysWindow;
        }

    public:
        /** @cond ZFPrivateDoc */
        NativeWindowEmbedImpl(void) : _ZFP_ownerZFUISysWindow(zfnull) {}
        /** @endcond */
        virtual ~NativeWindowEmbedImpl(void) {}

    public:
        /**
         * @brief called when owner #ZFUISysWindow is about to destroy
         */
        virtual void nativeWindowOnCleanup(void) zfpurevirtual;

    public:
        /**
         * @brief called to add root view to native window container
         *
         * note, you must properly invoke #ZFUIView::nativeViewNotifyBeforeAdd
         */
        virtual void nativeWindowOnRootViewAdd(void) zfpurevirtual;
        /**
         * @brief called to remove root view to native window container
         *
         * note, you must properly invoke #ZFUIView::nativeViewNotifyAfterRemove
         */
        virtual void nativeWindowOnRootViewRemove(void) zfpurevirtual;

        /**
         * @brief called when window layout param changed
         */
        virtual void windowLayoutParamOnChange(void) zfpurevirtual;

        /**
         * @brief see #ZFUISysWindow::windowOrientation
         */
        virtual ZFUIOrientationEnum windowOrientation(void) zfpurevirtual;
        /**
         * @brief see #ZFUISysWindow::windowOrientationFlagsSet,
         *  impl should have #ZFUIOrientation::e_Top as init value
         */
        virtual void windowOrientationFlagsSet(ZF_IN const ZFUIOrientationFlags &flags) zfpurevirtual;

        // ============================================================
        // callbacks that implementations must notify
    public:
        /**
         * @brief implementation must call this method to measure window's frame,
         *   and layout window using the result frame
         */
        zffinal ZFUIRect notifyMeasureWindow(ZF_IN const ZFUIRect &rootRefRect)
        {
            return this->ownerZFUISysWindow()->_ZFP_ZFUISysWindow_measureWindow(rootRefRect);
        }
        /**
         * @brief implementation must call this to notify after window create
         */
        zffinal void notifyOnCreate(ZF_IN void *nativeWindow)
        {
            this->ownerZFUISysWindow()->_ZFP_ZFUISysWindow_onCreate(nativeWindow);
        }
        /**
         * @brief implementation must call this to notify before window destroy
         */
        zffinal void notifyOnDestroy(void)
        {
            this->ownerZFUISysWindow()->_ZFP_ZFUISysWindow_onDestroy();
        }
        /**
         * @brief implementation must call this to notify after window resume,
         *   note that rotate a window should not notify this
         */
        zffinal void notifyOnResume(void)
        {
            this->ownerZFUISysWindow()->_ZFP_ZFUISysWindow_onResume();
        }
        /**
         * @brief implementation must call this to notify before window pause,
         *   note that rotate a window should not notify this
         */
        zffinal void notifyOnPause(void)
        {
            this->ownerZFUISysWindow()->_ZFP_ZFUISysWindow_onPause();
        }
        /**
         * @brief implementation must call this to notify when window rotate
         */
        zffinal void notifyOnRotate(void)
        {
            this->ownerZFUISysWindow()->_ZFP_ZFUISysWindow_onRotate();
        }
    };
    /**
     * @brief used to embed #ZFUISysWindow to existing UI framework
     *
     * #ZFUISysWindow would create internal native window by default,
     * however, you may create your own native window and attach #ZFUISysWindow to it\n
     * to achieve this feature,
     * you must supply custom embed impl by #ZFUISysWindow::NativeWindowEmbedImpl,
     * call this method to get an attached #ZFUISysWindow,
     * and manually manage the life cycle of the returned #ZFUISysWindow\n
     * see #ZFUISysWindow::NativeWindowEmbedImpl for all the impl that you need to implement
     */
    static zfautoObject nativeWindowEmbed(ZF_IN ZFUISysWindow::NativeWindowEmbedImpl *impl);
    /**
     * @brief see #nativeWindowEmbed
     */
    virtual ZFUISysWindow::NativeWindowEmbedImpl *nativeWindowEmbedImpl(void);

public:
    /**
     * @brief manually register main window, must be called before accessing #mainWindow
     *
     * this method is useful if you want to embed #ZFUISysWindow to existing UI framework,
     * to create a custom #ZFUISysWindow, see #nativeWindowEmbed\n
     * @note you must manually manage the life cycle of the registered #ZFUISysWindow
     * @note once registered, it can not be changed back to original internal window,
     *   until #ZFFrameworkCleanup
     */
    static void mainWindowRegister(ZF_IN ZFUISysWindow *window);
    /**
     * @brief get application's main window
     *
     * usually, use only one window is recommended
     */
    static ZFUISysWindow *mainWindow(void);

protected:
    ZFCLASS_PRIVATE_ALLOC("can only be created by reflect or ZFUISysWindow::modalWindowShow")
    zfoverride
    virtual ZFObject *objectOnInit(void);
public:
    zfoverride
    virtual void objectOnDealloc(void);

public:
    /**
     * @brief access the internal native window
     *
     * note, this value would be valid only after
     * #EventSysWindowOnCreate
     */
    virtual void *nativeWindow(void);

public:
    /**
     * @brief true if sys window created
     *
     * @note for some implementation,
     *   creating a ZFUISysWindow may or may not create native window immediately,
     *   you may use this method as well as #EventSysWindowOnCreate to see when did window created,
     *   typical usage:
     *   @code
     *       ZFUISysWindow *newWindow = ZFUISysWindow::mainWindow()->modalWindowShow();
     *       if(!newWindow->nativeWindowIsCreated())
     *       {
     *           ZFListener observer = ... {
     *               initWindow();
     *           };
     *           newWindow->observerAdd(ZFUISysWindow::EventSysWindowOnCreate(), observer);
     *       }
     *       else
     *       {
     *           initWindow();
     *       }
     *   @endcode
     */
    virtual zfbool nativeWindowIsCreated(void);
    /**
     * @brief true if sys window resumed
     */
    virtual zfbool nativeWindowIsResumed(void);

public:
    /**
     * @brief get current orientation
     */
    virtual ZFUIOrientationEnum windowOrientation(void);
    /**
     * @brief set supported orientation, orientation may or may not change immediately
     */
    virtual void windowOrientationFlagsSet(ZF_IN const ZFUIOrientationFlags &windowOrientationFlags);
    /**
     * @brief get supported orientation
     */
    virtual const ZFUIOrientationFlags &windowOrientationFlags(void);

public:
    /**
     * @brief show a new modal window,
     *   may or may not show immediately,
     *   may have different behavior on different implementations
     *
     * use only if necessary\n
     * it's recommended that app have only one ZFUISysWindow,
     * use #ZFUIWindow if necessary
     */
    virtual ZFUISysWindow *modalWindowShow(void);
    /**
     * @brief finish and destroy self as a modal window
     */
    virtual void modalWindowFinish(void);
    /**
     * @brief get self's current showing modal window
     */
    virtual ZFUISysWindow *modalWindowGetShowing(void);
    /**
     * @brief get self's owner if self is a showing modal window
     */
    virtual ZFUISysWindow *modalWindowGetOwner(void);

public:
    /**
     * @brief get window's layout param, fill parent by default
     */
    virtual ZFUIViewLayoutParam *windowLayoutParam(void);

    /**
     * @brief access the root view of the window
     *
     * the root view is ensured #ZFUIRootView type,
     * and you must not add it to another view
     */
    virtual ZFUIRootView *rootView(void);

public:
    zffinal ZFUIRect _ZFP_ZFUISysWindow_measureWindow(ZF_IN const ZFUIRect &rootRefRect);
    zffinal void _ZFP_ZFUISysWindow_onCreate(ZF_IN void *nativeWindow);
    zffinal void _ZFP_ZFUISysWindow_onDestroy(void);
    zffinal void _ZFP_ZFUISysWindow_onResume(void);
    zffinal void _ZFP_ZFUISysWindow_onPause(void);
    zffinal void _ZFP_ZFUISysWindow_onRotate(void);

private:
    _ZFP_ZFUISysWindowPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUISysWindow_h_

