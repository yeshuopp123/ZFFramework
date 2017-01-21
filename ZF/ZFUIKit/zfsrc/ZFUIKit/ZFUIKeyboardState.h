/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIKeyboardState.h
 * @brief hardware keyboard state helper
 */

#ifndef _ZFI_ZFUIKeyboardState_h_
#define _ZFI_ZFUIKeyboardState_h_

#include "ZFUIKitDef.h"
#include "ZFUIKeyCode.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief hardware keyboard state helper
 */
zffinal zfclass ZF_ENV_EXPORT ZFUIKeyboardState : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIKeyboardState, ZFObject)

    ZFOBJECT_SINGLETON_DECLARE(ZFUIKeyboardState, instance)

public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when pressed key changed
     */
    ZFOBSERVER_EVENT(KeyPressedOnChange)

public:
    /**
     * @brief true is key is pressed
     */
    virtual zfbool keyPressed(ZF_IN ZFUIKeyCodeEnum keyCode);
    /**
     * @brief true is key is pressed
     */
    virtual zfbool keyPressedRaw(ZF_IN zfflags keyCodeRaw);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIKeyboardState_h_

