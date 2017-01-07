/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIViewBlinkWhenFocus.h
 * @brief auto blink focused view
 */

#ifndef _ZFI_ZFUIViewBlinkWhenFocus_h_
#define _ZFI_ZFUIViewBlinkWhenFocus_h_

#include "ZFUIView.h"
#include "ZFUIImageIO.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief filter to exclude certain view to be auto blinked, empty by default
 */
extern ZF_ENV_EXPORT ZFFilterForZFObject ZFUIViewBlinkWhenFocusFilter;

// ============================================================
/**
 * @brief whether auto start #ZFUIViewBlinkWhenFocusAutoApplyStart, true by default
 *
 * this value would be initialized as true during #ZFFrameworkInit as level #ZFLevelZFFrameworkLow,
 * and would be used when #ZFLevelAppLow
 */
extern ZF_ENV_EXPORT zfbool ZFUIViewBlinkWhenFocusAutoApply;
/**
 * @brief default mask image for #ZFUIViewBlinkWhenFocusAutoApplyStart, an alpha white image by default
 *
 * this property would be initialized and destroyed automatically during #ZFFrameworkCleanup
 * as level #ZFLevelZFFrameworkLow
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIViewBlinkWhenFocusMaskImage;

/**
 * @brief util method to automatically blink focused view
 *
 * by default:
 * -  #ZFUIViewBlinkWhenFocusAutoApplyStart would be called automatically during #ZFFrameworkInit
 *   as level #ZFLevelAppLow if #ZFUIViewBlinkWhenFocusAutoApply
 * -  #ZFUIViewBlinkWhenFocusAutoApplyStop would be called automatically during #ZFFrameworkCleanup
 *   as level #ZFLevelZFFrameworkLow
 */
extern ZF_ENV_EXPORT void ZFUIViewBlinkWhenFocusAutoApplyStart(ZF_IN_OPT ZFUIImage *img = zfnull);
/**
 * @brief see #ZFUIViewBlinkWhenFocusAutoApplyStart
 */
extern ZF_ENV_EXPORT void ZFUIViewBlinkWhenFocusAutoApplyStop(void);
/**
 * @brief see #ZFUIViewBlinkWhenFocusAutoApplyStart
 */
extern ZF_ENV_EXPORT zfbool ZFUIViewBlinkWhenFocusAutoApplyStarted(void);

/**
 * @brief temporary pause the auto blink
 *
 * can be called more than one time, but must be paired with #ZFUIViewBlinkWhenFocusAutoApplyResume
 */
extern ZF_ENV_EXPORT void ZFUIViewBlinkWhenFocusAutoApplyPause(void);
/** @brief see #ZFUIViewBlinkWhenFocusAutoApplyPause */
extern ZF_ENV_EXPORT void ZFUIViewBlinkWhenFocusAutoApplyResume(void);
/** @brief see #ZFUIViewBlinkWhenFocusAutoApplyPause */
extern ZF_ENV_EXPORT zfindex ZFUIViewBlinkWhenFocusAutoApplyPaused(void);

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when #ZFUIViewBlinkWhenFocusAutoApplyStart
 */
ZFOBSERVER_EVENT_GLOBAL(ViewBlinkWhenFocusAutoApplyStart)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when #ZFUIViewBlinkWhenFocusAutoApplyStop
 */
ZFOBSERVER_EVENT_GLOBAL(ViewBlinkWhenFocusAutoApplyStop)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when #ZFUIViewBlinkWhenFocusAutoApplyPause
 */
ZFOBSERVER_EVENT_GLOBAL(ViewBlinkWhenFocusAutoApplyPause)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when #ZFUIViewBlinkWhenFocusAutoApplyResume
 */
ZFOBSERVER_EVENT_GLOBAL(ViewBlinkWhenFocusAutoApplyResume)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when a view is blinked
 */
ZFOBSERVER_EVENT_GLOBAL(ViewBlinkWhenFocusViewBlinkOn)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when a view is blinked
 */
ZFOBSERVER_EVENT_GLOBAL(ViewBlinkWhenFocusViewBlinkOff)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
// ZFUIViewBlinkWhenFocusAutoApplyPauseForTime
/**
 * @brief pause #ZFUIViewBlinkWhenFocusAutoApplyPause for a specified time,
 *   and resume it automatically after time out
 *
 * the task would be scheduled and run in future even if time is 0
 */
extern ZF_ENV_EXPORT void ZFUIViewBlinkWhenFocusAutoApplyPauseForTime(ZF_IN zftimet time = 0);
/**
 * @brief cancel #ZFUIViewBlinkWhenFocusAutoApplyPauseForTime
 *
 * typically you should not call this method manually,
 * which would cancel all the task that started by #ZFUIViewBlinkWhenFocusAutoApplyPauseForTime
 */
extern ZF_ENV_EXPORT void ZFUIViewBlinkWhenFocusAutoApplyPauseForTimeCancel(void);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewBlinkWhenFocus_h_

