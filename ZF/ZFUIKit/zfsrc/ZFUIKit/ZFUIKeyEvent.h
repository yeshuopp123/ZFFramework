/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIKeyEvent.h
 * @brief key event for ui elements
 */

#ifndef _ZFI_ZFUIKeyEvent_h_
#define _ZFI_ZFUIKeyEvent_h_

#include "ZFUIEvent.h"
#include "ZFUIKeyCode.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief key event for ui elements
 */
zffinal zfclass ZF_ENV_EXPORT ZFUIKeyEvent : zfextends ZFUIEvent
{
    ZFOBJECT_DECLARE(ZFUIKeyEvent, ZFUIEvent)

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief a unique value to differ key event from each other
     */
    zfidentity keyId;
    /**
     * @brief action
     */
    ZFUIKeyActionEnum keyAction;
    /**
     * @brief key code
     */
    ZFUIKeyCodeEnum keyCode;
    /**
     * @brief raw key code depends on implementation, try not to abuse it
     */
    zfflags keyCodeRaw;

public:
    ZFCACHEABLE_DECLARE(ZFUIKeyEvent)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIKeyEvent_h_

