/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFTimer.h
 * @brief protocol for ZFTimer
 */

#ifndef _ZFI_ZFProtocolZFTimer_h_
#define _ZFI_ZFProtocolZFTimer_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFTimer.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFTimer
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFTimer)
public:
    /**
     * @brief create and return a native timer
     */
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer) = 0;
    /**
     * @brief destroy native timer
     */
    virtual void nativeTimerDestroy(ZF_IN ZFTimer *timer,
                                    ZF_IN void *nativeTimer) = 0;
    /**
     * @brief start timer
     */
    virtual void timerStart(ZF_IN ZFTimer *timer) = 0;
    /**
     * @brief stop the timer
     */
    virtual void timerStop(ZF_IN ZFTimer *timer) = 0;

    // ============================================================
    // callbacks that implementations must notify
    /**
     * @brief must be called in timer thread when timer thread started
     *   (can be called only once for one timer, before first time fire the timer)
     *
     * return a thread token for #notifyTimerStop
     */
    zffinal void notifyTimerStart(ZF_IN ZFTimer *timer)
    {
        timer->_ZFP_ZFTimer_timerOnStart();
    }
    /**
     * @brief must be called to notify timer event
     */
    zffinal void notifyTimerActivate(ZF_IN ZFTimer *timer)
    {
        timer->_ZFP_ZFTimer_timerOnActivate();
    }
    /**
     * @brief must be called when timer thread is about to stop
     */
    zffinal void notifyTimerStop(ZF_IN ZFTimer *timer)
    {
        timer->_ZFP_ZFTimer_timerOnStop();
    }
ZFPROTOCOL_INTERFACE_END(ZFTimer)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFTimer_h_

