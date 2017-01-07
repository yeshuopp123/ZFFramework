/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFOperationQueue.h
 * @brief abstract operation queue
 */

#ifndef _ZFI_ZFOperationQueue_h_
#define _ZFI_ZFOperationQueue_h_

#include "ZFOperation.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief hold the child task's info
 */
zfclass ZF_ENV_EXPORT ZFOperationQueueChildTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFOperationQueueChildTaskData, ZFObject)

public:
    /** @brief see #ZFObject::objectOnInit */
    virtual ZFObject *objectOnInit(ZF_IN ZFOperation *childOperation,
                                   ZF_IN ZFOperationStartParam *childTaskData);
    /** @brief see #ZFObject::objectOnInit */
    virtual ZFObject *objectOnInit(void);

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

public:
    /**
     * @brief hold the child operation
     */
    ZFPROPERTY_RETAIN(ZFOperation *, childOperation)
    /**
     * @brief hold the info of child task
     */
    ZFPROPERTY_RETAIN(ZFOperationStartParam *, childTaskData)

    /**
     * @brief whether regard queue as cancel if this child task is canceled, true by default
     *
     * see #stopQueueIfFail
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, stopQueueIfCancel,
                                ZFPropertyInitValue(zftrue))
    /**
     * @brief whether regard queue as fail if this child task failed, true by default
     *
     * if true, all other child tasks that hasn't been started would be canceled\n
     * if false, queue is regarded as success while the failed child's result stored in
     * #ZFOperationQueueResult::childResults
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, stopQueueIfFail,
                                ZFPropertyInitValue(zftrue))

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);
};

// ============================================================
/** @brief see #ZFOperationParam */
zfclass ZF_ENV_EXPORT ZFOperationQueueParam : zfextends ZFOperationParam
{
    ZFOBJECT_DECLARE(ZFOperationQueueParam, ZFOperationParam)

public:
    zfoverride
    virtual zfidentity objectHash(void);

public:
    /**
     * @brief child task datas to start, hold type of #ZFOperationQueueChildTaskData
     */
    ZFPROPERTY_RETAIN_READONLY(ZFArrayEditable *, childTaskDatas,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFArrayEditable)))

    /**
     * @brief how many task to start at a time, 1 by default, must greater than 0
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, childToStartMax, ZFPropertyInitValue(1))
};

// ============================================================
/** @brief see #ZFOperationResult */
zfclass ZF_ENV_EXPORT ZFOperationQueueResult : zfextends ZFOperationResult
{
    ZFOBJECT_DECLARE(ZFOperationQueueResult, ZFOperationResult)

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief true if any of child task canceled
     */
    ZFPROPERTY_ASSIGN(zfbool, hasCanceledChildTask)
    /**
     * @brief true if any of child task failed
     */
    ZFPROPERTY_ASSIGN(zfbool, hasFailedChildTask)
    /**
     * @brief store the child tasks' results, hold type of #ZFOperationTaskData
     *
     * @note the order of child result is not the same with #ZFOperationQueueParam,
     *   use #childResultsInOrder instead
     */
    ZFPROPERTY_RETAIN_READONLY(ZFArrayEditable *, childResults,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFArrayEditable)))
    /**
     * @brief child results that have same order with #ZFOperationQueueParam, hold type of #ZFOperationTaskData
     */
    ZFPROPERTY_RETAIN_READONLY(ZFArrayEditable *, childResultsInOrder,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFArrayEditable)))
};

// ============================================================
/** @brief see #ZFOperationObserver */
#define ZFOperationQueueObserver ZFOperationObserver
/** @brief see #ZFOperationCache */
#define ZFOperationQueueCache ZFOperationCache

// ============================================================
/**
 * @brief progress for #ZFOperationQueue
 */
zfclass ZF_ENV_EXPORT ZFOperationQueueProgress : zfextends ZFOperationProgress
{
    ZFOBJECT_DECLARE(ZFOperationQueueProgress, ZFOperationProgress)

public:
    /**
     * @brief hold the child's data that notified the progress,
     *   total progress's data would be stored in #ZFOperationProgress::taskProgress
     */
    ZFPROPERTY_RETAIN(ZFOperationTaskData *, childProgressData)
    /**
     * @brief hold all of the children's progress (ZFOperationTaskData *)
     *
     * order of the array is ensured same as the order of queue's param's child param datas\n
     * if a child hasn't ever notified progress,
     * the child's position of this array may store a #zfnullObject,
     * otherwise, it would store the latest progress data of the child has notified
     */
    ZFPROPERTY_RETAIN_READONLY(ZFArrayEditable *, childLastProgressDatas,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFArrayEditable)))
};

// ============================================================
/** @brief see #ZFOperationTaskData */
#define ZFOperationQueueTaskData ZFOperationTaskData

// ============================================================
/**
 * @brief abstract operation queue, see #ZFOperation
 */
zffinal zfclass ZF_ENV_EXPORT ZFOperationQueue : zfextends ZFOperation
{
    ZFOBJECT_DECLARE(ZFOperationQueue, ZFOperation)

    ZFOBJECT_SINGLETON_DECLARE(ZFOperationQueue, instance)

public:
    zfoverride
    virtual zfbool paramIsValid(ZF_IN ZFOperationParam *operationParam);

protected:
    /**
     * @brief for an operation queue, you should not override this method anymore
     */
    zfoverride
    virtual void taskOnStart(ZF_IN ZFOperationTaskData *operationTaskData);
    /**
     * @brief for an operation queue, you should not override this method anymore
     */
    zfoverride
    virtual void taskOnStop(ZF_IN ZFOperationTaskData *operationTaskData);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOperationQueue_h_

