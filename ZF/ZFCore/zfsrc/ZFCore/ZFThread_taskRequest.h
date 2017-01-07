/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFThread_taskRequest.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_taskRequest_h_
#define _ZFI_ZFThread_taskRequest_h_

#include "ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// task request
/** @brief see #ZFThreadTaskRequest */
zfclass ZF_ENV_EXPORT ZFThreadTaskRequestData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFThreadTaskRequestData, ZFObject)

public:
    /** @brief see #ZFThreadTaskRequest */
    ZFPROPERTY_ASSIGN_READONLY(zfidentity, taskId, ZFPropertyInitValue(zfidentityInvalid))
    /** @brief see #ZFThreadTaskRequest */
    ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE(ZFListener, taskCallback)
    /** @brief see #ZFThreadTaskRequest */
    ZFPROPERTY_RETAIN(ZFObject *, taskParam0)
    /** @brief see #ZFThreadTaskRequest */
    ZFPROPERTY_RETAIN(ZFObject *, taskParam1)
    /** @brief see #ZFThreadTaskRequest */
    ZFPROPERTY_RETAIN(ZFObject *, taskUserData)
    /** @brief see #ZFThreadTaskRequest */
    ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE(ZFObject *, taskOwner)
};
/** @brief see #ZFThreadTaskRequest */
zfclass ZF_ENV_EXPORT ZFThreadTaskRequestMergeCallbackData : zfextends ZFObject
{
    ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(ZFThreadTaskRequestMergeCallbackData, ZFObject)

protected:
    ZFThreadTaskRequestMergeCallbackData(void)
    : taskRequestDataOld(zfnull)
    , taskRequestDataNew(zfnull)
    , taskRequestDataMerged(zfnull)
    {
    }

public:
    /** @brief see #ZFThreadTaskRequest */
    ZFThreadTaskRequestData *taskRequestDataOld;
    /** @brief see #ZFThreadTaskRequest */
    ZFThreadTaskRequestData *taskRequestDataNew;
    /** @brief see #ZFThreadTaskRequest */
    ZFThreadTaskRequestData *taskRequestDataMerged;
};
extern ZF_ENV_EXPORT const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask(void);
extern ZF_ENV_EXPORT const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask(void);
extern ZF_ENV_EXPORT const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge(void);
/**
 * @brief see #ZFThreadTaskRequest, simply ignore old task and replace with new task
 */
#define ZFThreadTaskRequestMergeCallbackIgnoreOldTask _ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask()
/**
 * @brief see #ZFThreadTaskRequest, simply ignore new task and use the old one
 */
#define ZFThreadTaskRequestMergeCallbackIgnoreNewTask _ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask()
/**
 * @brief see #ZFThreadTaskRequest, don't merge and add as new task
 */
#define ZFThreadTaskRequestMergeCallbackDoNotMerge _ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge()
/**
 * @brief see #ZFThreadTaskRequest, #ZFThreadTaskRequestMergeCallbackIgnoreOldTask by default
 */
#define ZFThreadTaskRequestMergeCallbackDefault ZFThreadTaskRequestMergeCallbackIgnoreOldTask

// ============================================================
// task request
/**
 * @brief request a task that would be run on main thread after a proper time,
 *   safe to be called in background thread
 *
 * task's param is the params passed to this method,
 * and return value is ignored\n
 * \n
 * tasks are ensured scheduled in different CPU time\n
 * \n
 * if duplicated task is found, the mergeCallback would be called\n
 * two tasks are treated as duplicated task if task callback and owner are both the same,
 * while param0/param1/userData won't be compared
 *
 * \n
 * the mergeCallback's param0 is a #ZFThreadTaskRequestMergeCallbackData
 * contains old and new task data (#ZFThreadTaskRequestData),
 * you should decide merged task and set it to #ZFThreadTaskRequestMergeCallbackData::taskRequestDataMerged,
 * whether retain existing one or allocating new one,
 * or set it to null to show the task can not be merged\n
 * for example:\n
 * to ignore old task, you may retain new one and set it to taskRequestDataMerged\n
 * to ignore new task, you may retain the old one instead\n
 * to merge, allocate a new task data and setup necessary info\n
 * to disable merge, set taskRequestDataMerged to null, and old/new task would be scheduled separately\n
 * or, you may use the pre-defined callbacks such as #ZFThreadTaskRequestMergeCallbackDoNotMerge
 */
extern ZF_ENV_EXPORT zfidentity ZFThreadTaskRequest(ZF_IN ZFThreadTaskRequestData *taskRequestData,
                                                    ZF_IN_OPT const ZFListener &mergeCallback = ZFThreadTaskRequestMergeCallbackDefault);
/** @brief see #ZFThreadTaskRequest */
inline zfidentity ZFThreadTaskRequest(ZF_IN const ZFListener &taskCallback,
                                      ZF_IN_OPT ZFObject *taskUserData = zfnull,
                                      ZF_IN_OPT ZFObject *taskParam0 = zfnull,
                                      ZF_IN_OPT ZFObject *taskParam1 = zfnull,
                                      ZF_IN_OPT ZFObject *taskOwner = zfnull,
                                      ZF_IN_OPT const ZFListener &taskMergeCallback = ZFThreadTaskRequestMergeCallbackDefault)
{
    zfblockedAllocWithoutLeakTest(ZFThreadTaskRequestData, taskRequestData);
    taskRequestData->taskCallbackSet(taskCallback);
    taskRequestData->taskUserDataSet(taskUserData);
    taskRequestData->taskParam0Set(taskParam0);
    taskRequestData->taskParam1Set(taskParam1);
    taskRequestData->taskOwnerSet(taskOwner);
    return ZFThreadTaskRequest(taskRequestData, taskMergeCallback);
}
/**
 * @brief see #ZFThreadTaskRequest
 */
extern ZF_ENV_EXPORT void ZFThreadTaskCancel(ZF_IN zfidentity taskId);
/**
 * @brief see #ZFThreadTaskRequest
 */
extern ZF_ENV_EXPORT void ZFThreadTaskCancelExactly(ZF_IN const ZFListener &task,
                                                    ZF_IN_OPT ZFObject *userData = zfnull,
                                                    ZF_IN_OPT ZFObject *param0 = zfnull,
                                                    ZF_IN_OPT ZFObject *param1 = zfnull);
/**
 * @brief see #ZFThreadTaskRequest
 */
extern ZF_ENV_EXPORT void ZFThreadTaskCancel(ZF_IN const ZFListener &task);
/**
 * @brief see #ZFThreadTaskRequest
 *
 * owner would be compared by pointer value
 */
extern ZF_ENV_EXPORT void ZFThreadTaskCancelWithOwner(ZF_IN ZFObject *owner);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_taskRequest_h_

