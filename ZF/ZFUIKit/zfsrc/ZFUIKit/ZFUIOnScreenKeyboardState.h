/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIOnScreenKeyboardState.h
 * @brief on screen keyboard state helper
 */

#ifndef _ZFI_ZFUIOnScreenKeyboardState_h_
#define _ZFI_ZFUIOnScreenKeyboardState_h_

#include "ZFUISysWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief on screen keyboard state helper
 */
zffinal zfclass ZF_ENV_EXPORT ZFUIOnScreenKeyboardState : zfextends ZFObject
{
    ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(ZFUIOnScreenKeyboardState, ZFObject)

public:
    /**
     * @brief access keyboard state for sys window
     */
    static ZFUIOnScreenKeyboardState *instanceForSysWindow(ZF_IN_OPT ZFUISysWindow *ownerSysWindow = zfnull);
    /**
     * @brief access keyboard state for view
     */
    static ZFUIOnScreenKeyboardState *instanceForView(ZF_IN_OPT ZFUIView *view = zfnull);

public:
    /**
     * @brief owner sys window, null and invalid if not accessed by #instanceForSysWindow
     *   (which is typically not allowed)
     */
    virtual ZFUISysWindow *ownerSysWindow(void);

public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when keyboard visibility changed or keyboard frame changed
     */
    ZFOBSERVER_EVENT(KeyboardStateOnChange)

public:
    /**
     * @brief true if the keyboard currently showing
     */
    virtual zfbool keyboardShowing(void);
    /**
     * @brief current key board frame, #ZFUIRectZero if not show
     */
    virtual const ZFUIRect &keyboardFrame(void);
    /**
     * @brief previous key board frame, #ZFUIRectZero if not show
     */
    virtual const ZFUIRect &keyboardFramePrev(void);

    /**
     * @brief fix client frame accorrding to current keyboard frame
     *
     * output is the available client frame that won't be covered by the keyboard
     */
    virtual void keyboardFixClientFrame(ZF_OUT ZFUIRect &clientFrame);
    /**
     * @brief see #keyboardFixClientFrame
     */
    virtual inline ZFUIRect keyboardFixClientFrame(void)
    {
        ZFUIRect ret = ZFUIRectZero;
        this->keyboardFixClientFrame(ret);
        return ret;
    }

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    ZFUISysWindow *_ZFP_ZFUIOnScreenKeyboardState_ownerSysWindow;
    zfbool _ZFP_ZFUIOnScreenKeyboardState_keyboardShowing;
    ZFUIRect _ZFP_ZFUIOnScreenKeyboardState_keyboardFrame;
    ZFUIRect _ZFP_ZFUIOnScreenKeyboardState_keyboardFramePrev;
protected:
    /** @cond ZFPrivateDoc */
    ZFUIOnScreenKeyboardState(void)
    : _ZFP_ZFUIOnScreenKeyboardState_ownerSysWindow(zfnull)
    , _ZFP_ZFUIOnScreenKeyboardState_keyboardShowing(zffalse)
    , _ZFP_ZFUIOnScreenKeyboardState_keyboardFrame(ZFUIRectZero)
    , _ZFP_ZFUIOnScreenKeyboardState_keyboardFramePrev(ZFUIRectZero)
    {
    }
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIOnScreenKeyboardState_h_

