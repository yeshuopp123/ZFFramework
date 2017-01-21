/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFUISysWindow.h
 * @brief protocol for ZFUISysWindow
 */

#ifndef _ZFI_ZFProtocolZFUISysWindow_h_
#define _ZFI_ZFProtocolZFUISysWindow_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUISysWindow.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUISysWindow
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUISysWindow)
public:
    /**
     * @brief see #ZFUISysWindow::mainWindow
     *
     * for implementations,
     * you should create a new window if not initialized,
     * or return the created one if exist
     */
    virtual ZFUISysWindow *mainWindow(void) zfpurevirtual;
    /**
     * @brief called to cleanup native main window
     *
     * this is ensured to be called during #ZFFrameworkCleanup
     * with level #ZFLevelZFFrameworkLow\n
     * after cleanup, you must ensured it's reentrant to access the main window again
     */
    virtual void mainWindowOnCleanup(void) zfpurevirtual;
    /**
     * @brief called when main window destroyed by #notifyOnDestroy
     */
    virtual void mainWindowOnDestroy(void) zfpurevirtual;

    /**
     * @brief called to add root view to native window container
     *
     * note, you must properly invoke #ZFUIView::nativeViewNotifyBeforeAdd
     */
    virtual void nativeWindowOnRootViewAdd(ZF_IN ZFUISysWindow *window) zfpurevirtual;
    /**
     * @brief called to remove root view to native window container
     *
     * note, you must properly invoke #ZFUIView::nativeViewNotifyAfterRemove
     */
    virtual void nativeWindowOnRootViewRemove(ZF_IN ZFUISysWindow *window) zfpurevirtual;

    /**
     * @brief see #ZFUISysWindow::modalWindowShow
     */
    virtual ZFUISysWindow *modalWindowShow(ZF_IN ZFUISysWindow *ownerWindow) zfpurevirtual;
    /**
     * @brief see #ZFUISysWindow::modalWindowFinish
     */
    virtual void modalWindowFinish(ZF_IN ZFUISysWindow *ownerWindow,
                                   ZF_IN ZFUISysWindow *windowToFinish) zfpurevirtual;

    /**
     * @brief called to update suggested window layout param
     */
    virtual void updateSuggestedWindowLayoutParam(ZF_IN ZFUISysWindow *window) zfpurevirtual;
    /**
     * @brief called when window layout param changed
     */
    virtual void windowLayoutParamOnChange(ZF_IN ZFUISysWindow *window) zfpurevirtual;

    /**
     * @brief see #ZFUISysWindow::windowOrientation
     */
    virtual ZFUIOrientationEnum windowOrientation(ZF_IN ZFUISysWindow *window) zfpurevirtual;
    /**
     * @brief see #ZFUISysWindow::windowOrientationFlagsSet,
     *  impl should have #ZFUIOrientation::e_Top as init value
     */
    virtual void windowOrientationFlagsSet(ZF_IN ZFUISysWindow *window,
                                           ZF_IN const ZFUIOrientationFlags &flags) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementation must call this method to measure window's frame,
     *   and layout window using the result frame
     */
    zffinal ZFUIRect notifyMeasureWindow(ZF_IN ZFUISysWindow *window,
                                         ZF_IN const ZFUIRect &rootRefRect)
    {
        return window->_ZFP_ZFUISysWindow_measureWindow(rootRefRect);
    }
    /**
     * @brief implementation must call this to notify after window create
     */
    zffinal void notifyOnCreate(ZF_IN ZFUISysWindow *window,
                                ZF_IN void *nativeWindow)
    {
        window->_ZFP_ZFUISysWindow_onCreate(nativeWindow);
    }
    /**
     * @brief implementation must call this to notify before window destroy
     */
    zffinal void notifyOnDestroy(ZF_IN ZFUISysWindow *window)
    {
        window->_ZFP_ZFUISysWindow_onDestroy();
    }
    /**
     * @brief implementation must call this to notify after window resume,
     *   note that rotate a window should not notify this
     */
    zffinal void notifyOnResume(ZF_IN ZFUISysWindow *window)
    {
        window->_ZFP_ZFUISysWindow_onResume();
    }
    /**
     * @brief implementation must call this to notify before window pause,
     *   note that rotate a window should not notify this
     */
    zffinal void notifyOnPause(ZF_IN ZFUISysWindow *window)
    {
        window->_ZFP_ZFUISysWindow_onPause();
    }
    /**
     * @brief implementation must call this to notify when window rotate
     */
    zffinal void notifyOnRotate(ZF_IN ZFUISysWindow *window)
    {
        window->_ZFP_ZFUISysWindow_onRotate();
    }
ZFPROTOCOL_INTERFACE_END(ZFUISysWindow)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUISysWindow_h_

