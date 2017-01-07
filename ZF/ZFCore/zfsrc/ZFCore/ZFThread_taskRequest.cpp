/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFThread_taskRequest.h"
#include "protocol/ZFProtocolZFThreadTaskRequest.h"
#include "ZFArray.h"
#include "ZFMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// task processing
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestCallback_action);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask_action);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask_action);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge_action);

static ZFListener *_ZFP_ZFThread_wrappedTaskCallback = zfnull;
static ZFListener *_ZFP_ZFThread_mergeCallbackIgnoreOldTask = zfnull;
static ZFListener *_ZFP_ZFThread_mergeCallbackIgnoreNewTask = zfnull;
static ZFListener *_ZFP_ZFThread_mergeCallbackDoNotMerge = zfnull;
static zfbool _ZFP_ZFThread_taskRunning = zffalse;
static ZFArrayEditable *_ZFP_ZFThread_taskDatas = zfnull;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadTaskRequestDataHolder, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFThread_wrappedTaskCallback = zfnew(ZFListener);
    *_ZFP_ZFThread_wrappedTaskCallback = ZFCallbackForRawFunction(_ZFP_ZFThreadTaskRequestCallback_action);
    _ZFP_ZFThread_mergeCallbackIgnoreOldTask = zfnew(ZFListener);
    *_ZFP_ZFThread_mergeCallbackIgnoreOldTask = ZFCallbackForRawFunction(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask_action);
    _ZFP_ZFThread_mergeCallbackIgnoreNewTask = zfnew(ZFListener);
    *_ZFP_ZFThread_mergeCallbackIgnoreNewTask = ZFCallbackForRawFunction(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask_action);
    _ZFP_ZFThread_mergeCallbackDoNotMerge = zfnew(ZFListener);
    *_ZFP_ZFThread_mergeCallbackDoNotMerge = ZFCallbackForRawFunction(_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge_action);
    _ZFP_ZFThread_taskDatas = zfAllocWithoutLeakTest(ZFArrayEditable);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadTaskRequestDataHolder)
{
    zfdelete(_ZFP_ZFThread_wrappedTaskCallback);
    _ZFP_ZFThread_wrappedTaskCallback = zfnull;
    zfdelete(_ZFP_ZFThread_mergeCallbackIgnoreOldTask);
    _ZFP_ZFThread_mergeCallbackIgnoreOldTask = zfnull;
    zfdelete(_ZFP_ZFThread_mergeCallbackIgnoreNewTask);
    _ZFP_ZFThread_mergeCallbackIgnoreNewTask = zfnull;
    zfdelete(_ZFP_ZFThread_mergeCallbackDoNotMerge);
    _ZFP_ZFThread_mergeCallbackDoNotMerge = zfnull;
    _ZFP_ZFThread_taskRunning = zffalse;
    zfReleaseWithoutLeakTest(_ZFP_ZFThread_taskDatas);
    _ZFP_ZFThread_taskDatas = zfnull;
}
public:
    ZFIdentityGenerator taskIdHolder;
ZF_GLOBAL_INITIALIZER_END(ZFThreadTaskRequestDataHolder)
#define _ZFP_ZFThreadTaskRequestTaskIdHolder (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadTaskRequestDataHolder)->taskIdHolder)

static void _ZFP_ZFThreadTaskRequest_taskStop(ZF_IN ZFThreadTaskRequestData *taskData)
{
    _ZFP_ZFThreadTaskRequestTaskIdHolder.markUnused(taskData->taskId());
}

static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestCallback_action)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }
    if(_ZFP_ZFThread_taskDatas != zfnull && !_ZFP_ZFThread_taskDatas->isEmpty())
    {
        // take and remove a task
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->getFirst<ZFThreadTaskRequestData *>();
        zfRetain(taskData);
        _ZFP_ZFThread_taskDatas->removeFirst();

        // run
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
        taskData->taskCallback().execute(
            ZFListenerData(zfidentityInvalid, zfnull, taskData->taskParam0(), taskData->taskParam1()),
            taskData->taskUserData());
        if(lockAvailable)
        {
            zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
        }

        // cleanup
        _ZFP_ZFThreadTaskRequest_taskStop(taskData);
        zfRelease(taskData);

        // schedule next task or quit
        if(!_ZFP_ZFThread_taskDatas->isEmpty())
        {
            ZFPROTOCOL_ACCESS(ZFThreadTaskRequest)->taskRequest(*_ZFP_ZFThread_wrappedTaskCallback, zfnull, zfnull);
        }
        else
        {
            _ZFP_ZFThread_taskRunning = zffalse;
        }
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
    }
    else
    {
        _ZFP_ZFThread_taskRunning = zffalse;
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask_action)
{
    ZFThreadTaskRequestMergeCallbackData *mergeCallbackData = listenerData.param0->to<ZFThreadTaskRequestMergeCallbackData *>();
    mergeCallbackData->taskRequestDataMerged = zfRetain(mergeCallbackData->taskRequestDataNew);
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask_action)
{
    ZFThreadTaskRequestMergeCallbackData *mergeCallbackData = listenerData.param0->to<ZFThreadTaskRequestMergeCallbackData *>();
    mergeCallbackData->taskRequestDataMerged = zfRetain(mergeCallbackData->taskRequestDataOld);
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge_action)
{
    // leave empty merged data, nothing to do
}

const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask(void)
{
    return *_ZFP_ZFThread_mergeCallbackIgnoreOldTask;
}
const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask(void)
{
    return *_ZFP_ZFThread_mergeCallbackIgnoreNewTask;
}
const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge(void)
{
    return *_ZFP_ZFThread_mergeCallbackDoNotMerge;
}

zfidentity ZFThreadTaskRequest(ZF_IN ZFThreadTaskRequestData *taskRequestData,
                               ZF_IN_OPT const ZFListener &mergeCallback /* = ZFThreadTaskRequestMergeCallbackDefault */)
{
    if(taskRequestData == zfnull || !taskRequestData->taskCallback().callbackIsValid())
    {
        return zfidentityInvalid;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }

    zfindex oldTaskIndex = zfindexMax;
    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *existing = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(existing->taskCallback().objectCompare(taskRequestData->taskCallback()) == ZFCompareTheSame
            && existing->taskOwner() == taskRequestData->taskOwner())
        {
            oldTaskIndex = i;
            break;
        }
    }
    zfidentity taskId = zfidentityInvalid;
    if(oldTaskIndex != zfindexMax && mergeCallback != ZFThreadTaskRequestMergeCallbackDoNotMerge)
    {
        zfblockedAllocWithoutLeakTest(ZFThreadTaskRequestMergeCallbackData, mergeCallbackData);
        mergeCallbackData->taskRequestDataOld = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(oldTaskIndex);
        mergeCallbackData->taskRequestDataNew = taskRequestData;
        mergeCallback.execute(ZFListenerData(zfidentityInvalid, zfnull, mergeCallbackData));
        if(mergeCallbackData->taskRequestDataMerged != zfnull)
        {
            taskRequestData = mergeCallbackData->taskRequestDataMerged;
            taskId = mergeCallbackData->taskRequestDataMerged->taskId();

            _ZFP_ZFThread_taskDatas->remove(oldTaskIndex);
            _ZFP_ZFThread_taskDatas->add(mergeCallbackData->taskRequestDataMerged);
            zfRelease(mergeCallbackData->taskRequestDataMerged);
            mergeCallbackData->taskRequestDataMerged = zfnull;
        }
        else
        {
            taskId = _ZFP_ZFThreadTaskRequestTaskIdHolder.nextMarkUsed();
            _ZFP_ZFThread_taskDatas->add(taskRequestData);
        }
    }
    else
    {
        taskId = _ZFP_ZFThreadTaskRequestTaskIdHolder.nextMarkUsed();
        _ZFP_ZFThread_taskDatas->add(taskRequestData);
    }
    ZFPropertyAccess(ZFThreadTaskRequestData, taskId)->setterMethod()->execute<void, zfidentity const &>(taskRequestData, taskId);

    if(!_ZFP_ZFThread_taskRunning)
    {
        _ZFP_ZFThread_taskRunning = zftrue;
        ZFPROTOCOL_ACCESS(ZFThreadTaskRequest)->taskRequest(*_ZFP_ZFThread_wrappedTaskCallback, zfnull, zfnull);
    }
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }
    return taskId;
}
void ZFThreadTaskCancel(ZF_IN zfidentity taskId)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(taskData->taskId() == taskId)
        {
            _ZFP_ZFThreadTaskRequest_taskStop(taskData);
            _ZFP_ZFThread_taskDatas->remove(i);
            break;
        }
    }
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }
}
void ZFThreadTaskCancelExactly(ZF_IN const ZFListener &task,
                               ZF_IN_OPT ZFObject *userData /* = zfnull */,
                               ZF_IN_OPT ZFObject *param0 /* = zfnull */,
                               ZF_IN_OPT ZFObject *param1 /* = zfnull */)
{
    if(!task.callbackIsValid())
    {
        return ;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(taskData->taskCallback().objectCompare(task) == ZFCompareTheSame
            && ZFObjectCompare(taskData->taskUserData(), userData) == ZFCompareTheSame
            && ZFObjectCompare(taskData->taskParam0(), param0) == ZFCompareTheSame
            && ZFObjectCompare(taskData->taskParam1(), param1) == ZFCompareTheSame)
        {
            _ZFP_ZFThreadTaskRequest_taskStop(taskData);
            _ZFP_ZFThread_taskDatas->remove(i);
            break;
        }
    }
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }
}
void ZFThreadTaskCancel(ZF_IN const ZFListener &task)
{
    if(!task.callbackIsValid())
    {
        return ;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(taskData->taskCallback().objectCompare(task) == ZFCompareTheSame)
        {
            _ZFP_ZFThreadTaskRequest_taskStop(taskData);
            _ZFP_ZFThread_taskDatas->remove(i);
            --i;
        }
    }
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }
}
void ZFThreadTaskCancelWithOwner(ZF_IN ZFObject *owner)
{
    if(owner == zfnull)
    {
        return ;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(taskData->taskOwner() == owner)
        {
            _ZFP_ZFThreadTaskRequest_taskStop(taskData);
            _ZFP_ZFThread_taskDatas->remove(i);
            --i;
        }
    }
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }
}

ZF_NAMESPACE_GLOBAL_END

