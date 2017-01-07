/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFUIKeyboardState.h
 * @brief protocol for #ZFUIKeyboardState
 */

#ifndef _ZFI_ZFProtocolZFUIKeyboardState_h_
#define _ZFI_ZFProtocolZFUIKeyboardState_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIKeyboardState.h"
#include "ZFUIKit/ZFUIView.h"
#include "ZFUIKit/ZFUIKeyEvent.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIKeyboardState
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIKeyboardState)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief see #ZFUIKeyboardState::keyPressed
     */
    virtual zfbool keyPressed(ZF_IN ZFUIKeyCodeEnum keyCode) = 0;
    /**
     * @brief see #ZFUIKeyboardState::keyPressedRaw
     */
    virtual zfbool keyPressedRaw(ZF_IN zfuint32 keyCodeRaw) = 0;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementations must notify when keyboard's state changed
     */
    zffinal void notifyKeyPressedChange(void)
    {
        ZFUIKeyboardState::instance()->observerNotify(ZFUIKeyboardState::EventKeyPressedOnChange());
    }
ZFPROTOCOL_INTERFACE_END(ZFUIKeyboardState)

// ============================================================
// builtin impl
/**
 * @brief register the builtin impl for #ZFUIKeyboardState
 *
 * register an observer on #ZFUIView::EventViewOnEvent (as level #ZFLevelZFFrameworkEssential)
 * to achieve default impl for #ZFUIKeyboardState\n
 * if your impl require additional keyboard state,
 * use #ZFUIKeyboardStateBuiltinImplNotifyKeyEvent to manually
 * update the keyboard state
 */
extern ZF_ENV_EXPORT void ZFUIKeyboardStateBuiltinImplRegister(void);
/**
 * @brief see #ZFUIKeyboardStateBuiltinImplRegister
 */
extern ZF_ENV_EXPORT void ZFUIKeyboardStateBuiltinImplUnregister(void);
/**
 * @brief see #ZFUIKeyboardStateBuiltinImplRegister
 *
 * if your keyboard event won't be logged by #ZFUIView::EventViewOnEvent,
 * you may manually use this method to update the key state
 */
extern ZF_ENV_EXPORT void ZFUIKeyboardStateBuiltinImplNotifyKeyEvent(ZF_IN ZFUIKeyEvent *event);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIKeyboardState_h_

