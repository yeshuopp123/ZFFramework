/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIViewStateAniAutoApply.h
 * @brief automatically invoke #ZFUIViewStateAniStart when #ZFUIView::EventViewLayoutOnLayoutRequest
 *   or #ZFUIView::EventViewOnAddToParent
 */

#ifndef _ZFI_ZFUIViewStateAniAutoApply_h_
#define _ZFI_ZFUIViewStateAniAutoApply_h_

#include "ZFUIViewStateAni.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief whether auto start #ZFUIViewStateAniAutoApplyStart, false by default
 *
 * this value would be initialized as true during #ZFFrameworkInit as level #ZFLevelZFFrameworkLow,
 * and would be used when #ZFLevelAppLow
 */
extern ZF_ENV_EXPORT zfbool ZFUIViewStateAniAutoApply;

/**
 * @brief automatically invoke #ZFUIViewStateAniStart when #ZFUIView::EventViewLayoutOnLayoutRequest
 *
 * @note activating auto start of state animation would affect all views,
 *   which may cause strange behavior as well as performance issue,
 *   use only when fully tested
 */
extern ZF_ENV_EXPORT void ZFUIViewStateAniAutoApplyStart(void);
/**
 * @brief see #ZFUIViewStateAniAutoApplyStart
 */
extern ZF_ENV_EXPORT void ZFUIViewStateAniAutoApplyStop(void);
/**
 * @brief see #ZFUIViewStateAniAutoApplyStart
 */
extern ZF_ENV_EXPORT zfbool ZFUIViewStateAniAutoApplyStarted(void);

/**
 * @brief temporary pause the #ZFUIViewStateAniAutoApplyStart
 *
 * can be called more than one time, but must be paired with #ZFUIViewStateAniAutoApplyResume
 */
extern ZF_ENV_EXPORT void ZFUIViewStateAniAutoApplyPause(void);
/** @brief see #ZFUIViewStateAniAutoApplyPause */
extern ZF_ENV_EXPORT void ZFUIViewStateAniAutoApplyResume(void);
/** @brief see #ZFUIViewStateAniAutoApplyPause */
extern ZF_ENV_EXPORT zfindex ZFUIViewStateAniAutoApplyPaused(void);

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when #ZFUIViewStateAniAutoApplyStart
 */
ZFOBSERVER_EVENT_GLOBAL(ViewStateAniAutoApplyStart)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when #ZFUIViewStateAniAutoApplyStop
 */
ZFOBSERVER_EVENT_GLOBAL(ViewStateAniAutoApplyStop)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when #ZFUIViewStateAniAutoApplyPause
 */
ZFOBSERVER_EVENT_GLOBAL(ViewStateAniAutoApplyPause)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when #ZFUIViewStateAniAutoApplyResume
 */
ZFOBSERVER_EVENT_GLOBAL(ViewStateAniAutoApplyResume)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
// ZFUIViewStateAniAutoApplyPauseForTime
/**
 * @brief pause #ZFUIViewStateAniAutoApplyPause for a specified time,
 *   and resume it automatically after time out
 *
 * the task would be scheduled and run in future even if time is 0
 */
extern ZF_ENV_EXPORT void ZFUIViewStateAniAutoApplyPauseForTime(ZF_IN zftimet time = 0);
/**
 * @brief cancel #ZFUIViewStateAniAutoApplyPauseForTime
 *
 * typically you should not call this method manually,
 * which would cancel all the task that started by #ZFUIViewStateAniAutoApplyPauseForTime
 */
extern ZF_ENV_EXPORT void ZFUIViewStateAniAutoApplyPauseForTimeCancel(void);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewStateAniAutoApply_h_

