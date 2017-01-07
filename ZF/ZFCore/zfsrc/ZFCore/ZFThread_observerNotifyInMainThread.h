/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFThread_observerNotifyInMainThread.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_observerNotifyInMainThread_h_
#define _ZFI_ZFThread_observerNotifyInMainThread_h_

#include "ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief notify observer in main thread, see #ZFObject::observerNotify
 *
 * the observer would be scheduled to run in future
 * even if current thread is main thread\n
 * the task can be canceled by #ZFObserverNotifyInMainThreadCancel if not actually started
 * @note obj, customSender, params would be retained during running
 */
extern ZF_ENV_EXPORT zfidentity ZFObserverNotifyInMainThreadWithCustomSender(ZF_IN ZFObject *customSender,
                                                                             ZF_IN ZFObject *obj,
                                                                             ZF_IN const zfidentity &eventId,
                                                                             ZF_IN_OPT ZFObject *param0 = zfnull,
                                                                             ZF_IN_OPT ZFObject *param1 = zfnull);
/** @brief see #ZFObserverNotifyInMainThreadWithCustomSender */
inline zfidentity ZFObserverNotifyInMainThread(ZF_IN ZFObject *obj,
                                               ZF_IN const zfidentity &eventId,
                                               ZF_IN_OPT ZFObject *param0 = zfnull,
                                               ZF_IN_OPT ZFObject *param1 = zfnull)
{
    return ZFObserverNotifyInMainThreadWithCustomSender(obj, obj, eventId, param0, param1);
}
/** @brief see #ZFObserverNotifyInMainThreadWithCustomSender */
extern ZF_ENV_EXPORT void ZFObserverNotifyInMainThreadCancel(ZF_IN zfidentity taskId);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_observerNotifyInMainThread_h_

