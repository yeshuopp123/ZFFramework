/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFOperation_Types.h
 * @brief necessary types for #ZFOperation
 */

#ifndef _ZFI_ZFOperation_Types_h_
#define _ZFI_ZFOperation_Types_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFOperationParam;
zfclassFwd ZFOperationResult;
zfclassFwd ZFOperationObserver;
zfclassFwd ZFOperationCache;
zfclassFwd ZFOperationProgress;
zfclassFwd ZFOperationTaskData;
zfclassFwd ZFOperationStartParam;
zfclassFwd ZFOperation;

// ============================================================
/** @brief see #ZFOperation::cacheExpireTimeDefault */
#define ZFOperationCacheExpireTimeUnspecified -1
/** @brief see #ZFOperation::cacheExpireTimeDefault */
#define ZFOperationCacheExpireTimeDisable 0
/** @brief see #ZFOperation::cacheExpireTimeDefault */
#define ZFOperationCacheExpireTimeVeryShort zftimetOneMinute
/** @brief see #ZFOperation::cacheExpireTimeDefault */
#define ZFOperationCacheExpireTimeShort (zftimetOneMinute*5)
/** @brief see #ZFOperation::cacheExpireTimeDefault */
#define ZFOperationCacheExpireTimeNormal (zftimetOneMinute*30)
/** @brief see #ZFOperation::cacheExpireTimeDefault */
#define ZFOperationCacheExpireTimeLong zftimetOneDay
/** @brief see #ZFOperation::cacheExpireTimeDefault */
#define ZFOperationCacheExpireTimeVeryLong zftimetOneWeek
/** @brief see #ZFOperation::cacheExpireTimeDefault */
#define ZFOperationCacheExpireTimeMax zftimetOneYear

// ============================================================
/**
 * @brief base operation param type for ZFOperation,
 *   must be comparable by #ZFOperationParam::paramIsEqual
 *
 * subclass of this class must follow these rules:
 * -  class name must be "YourOperation+Param"
 *   while "YourOperation" is its owner operation
 * -  have class registered by #ZFOBJECT_REGISTER
 * -  all params must be declared as ZFProperty and must be comparable,
 *   see #ZFPROPERTY_RETAIN for how to declare
 */
zfclass ZF_ENV_EXPORT ZFOperationParam : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFOperationParam, ZFObject)

protected:
    ZFCLASS_PRIVATE_ALLOC("can only be created by ZFOperation::createParam")
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        return zfsuper::objectOnInit();
    }

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief compare whether two param is equal,
     *   see #paramOnCheckPropertyIsEqual, #paramOnCheckIsEqual
     */
    inline zfbool paramIsEqual(ZF_IN ZFOperationParam *anotherOperationParam)
    {
        return (this->paramOnCheckPropertyIsEqual(anotherOperationParam)
            && this->paramOnCheckIsEqual(anotherOperationParam));
    }

protected:
    /**
     * @brief called by #paramIsEqual
     *
     * by default, this method would check all property by reflection
     * (ZFClassUtil::#ZFClassUtil::allPropertyIsEqual)\n
     * you may override for your custom check logic
     * @see paramIsEqual, paramOnCheckIsEqual
     */
    virtual zfbool paramOnCheckPropertyIsEqual(ZF_IN ZFOperationParam *anotherOperationParam);
    /**
     * @brief called by #paramIsEqual
     *
     * by default, this method always return true\n
     * you may override for your custom check logic
     * @see paramIsEqual, paramOnCheckPropertyIsEqual
     */
    virtual zfbool paramOnCheckIsEqual(ZF_IN ZFOperationParam *anotherOperationParam);
};

// ============================================================
/**
 * @brief base operation result type for ZFOperation
 *
 * subclass of this class must follow these rules:
 * -  class name must be "YourOperation+Result"
 *   while "YourOperation" is its owner operation
 * -  have class registered by #ZFOBJECT_REGISTER
 */
zfclass ZF_ENV_EXPORT ZFOperationResult : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFOperationResult, ZFObject)

protected:
    ZFCLASS_PRIVATE_ALLOC("can only be created by ZFOperation::createResult")
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        return zfsuper::objectOnInit();
    }

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

public:
    /**
     * @brief result type
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFResultTypeEnum, resultType,
                                ZFPropertyInitValue(ZFResultType::e_Fail))
};

// ============================================================
/**
 * @brief base operation observer type for ZFOperation
 *
 * subclass of this class must follow these rules:
 * -  class name must be "YourOperation+Observer"
 *   while "YourOperation" is its owner operation
 * -  have class registered by #ZFOBJECT_REGISTER
 *
 * \n
 * you may use this object to observer task event,
 * by #ZFObject::observerAdd,
 * event is declared in #ZFOperation,
 * such as #ZFOperation::EventOperationTaskOnStop
 */
zfclass ZF_ENV_EXPORT ZFOperationObserver : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFOperationObserver, ZFObject)

protected:
    ZFCLASS_PRIVATE_ALLOC("can only be created by ZFOperation::createObserver")
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        return zfsuper::objectOnInit();
    }
};

// ============================================================
/**
 * @brief base operation cache type for ZFOperation
 *
 * subclass of this class must follow these rules:
 * -  class name must be "YourOperation+Cache"
 *   while "YourOperation" is its owner operation
 * -  have class registered by #ZFOBJECT_REGISTER
 */
zfclass ZF_ENV_EXPORT ZFOperationCache : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFOperationCache, ZFObject)

protected:
    ZFCLASS_PRIVATE_ALLOC("can only be created by ZFOperation::createCache")
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        return zfsuper::objectOnInit();
    }

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief cached param
     */
    ZFPROPERTY_RETAIN(ZFOperationParam *, operationParam)
    /**
     * @brief cached result
     */
    ZFPROPERTY_RETAIN(ZFOperationResult *, operationResult)

    /**
     * @brief cache expire time, see #cacheTime, #ZFOperation::cacheExpireTimeDefault
     */
    ZFPROPERTY_ASSIGN(zftimet, cacheExpireTime)
    /**
     * @brief cache's create time in seconds, since ZFTime::currentTimeValue().sec
     */
    ZFPROPERTY_ASSIGN(zftimet, cacheTime)

public:
    /**
     * @brief whether the cache is expired
     *
     * check by cache time only, whether the cache is valid should be checked by #ZFOperation::cacheIsValid
     */
    virtual zfbool cacheIsExpired(void);
};

// ============================================================
/**
 * @brief base class to indicate the progress of a #ZFOperation
 */
zfclass ZF_ENV_EXPORT ZFOperationProgress : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFOperationProgress, ZFObject)

protected:
    ZFCLASS_PRIVATE_ALLOC("can only be created by ZFOperation::createProgress")
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        return zfsuper::objectOnInit();
    }

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

public:
    /**
     * @brief progress ranged in [0.0 ~ 1.0]
     */
    ZFPROPERTY_ASSIGN(zffloat, taskProgress)
};

// ============================================================
/**
 * @brief how ZFOperation would do when cache matched
 */
ZFENUM_BEGIN(ZFOperationCacheMatchAction)
    /**
     * @brief not specified, use operation's default action (#ZFOperation::cacheMatchAction)
     */
    ZFENUM_VALUE(Unspecified)
    /**
     * @brief simply notify finish with success state (i.e. match from cache and finish)
     */
    ZFENUM_VALUE(NotifyFinish)
    /**
     * @brief notify finish with success state then remove the cache (i.e. one time cache)
     */
    ZFENUM_VALUE(NotifyFinishThenRemove)
    /**
     * @brief ignore and start new task
     *
     * old cache won't be removed until the new task finished successfully and being added to cache
     */
    ZFENUM_VALUE(StartNew)
    /**
     * @brief ignore and start new task, then remove the cache (i.e. force start)
     */
    ZFENUM_VALUE(StartNewThenRemove)
ZFENUM_SEPARATOR(ZFOperationCacheMatchAction)
    ZFENUM_VALUE_REGISTER(Unspecified)
    ZFENUM_VALUE_REGISTER(NotifyFinish)
    ZFENUM_VALUE_REGISTER(NotifyFinishThenRemove)
    ZFENUM_VALUE_REGISTER(StartNew)
    ZFENUM_VALUE_REGISTER(StartNewThenRemove)
ZFENUM_END(ZFOperationCacheMatchAction)

// ============================================================
/**
 * @brief how ZFOperation act when start duplicate operation (i.e. start by same param)
 */
ZFENUM_BEGIN(ZFOperationTaskDuplicateAction)
    /**
     * @brief not specified, use operation's default action (#ZFOperation::taskDuplicateAction)
     */
    ZFENUM_VALUE(Unspecified)
    /**
     * @brief merge the duplicate operation, observer would be activated normally
     *
     * subclass's taskOnStart and taskOnStop would be called normally for first task,
     * while taskOnStartDuplicated/taskOnStopDuplicated would be called for others\n
     * #ZFOperation::taskNotifyFinish would notify and finish all the duplicated task\n
     * typically, subclass should simply ignore taskOnStartDuplicated
     */
    ZFENUM_VALUE(Merge)
    /**
     * @brief treat as new task and start normally
     */
    ZFENUM_VALUE(NewTask)
    /**
     * @brief completely ignore the operation, only notify the observer with cancel status
     */
    ZFENUM_VALUE(Ignore)
ZFENUM_SEPARATOR(ZFOperationTaskDuplicateAction)
    ZFENUM_VALUE_REGISTER(Unspecified)
    ZFENUM_VALUE_REGISTER(Merge)
    ZFENUM_VALUE_REGISTER(NewTask)
    ZFENUM_VALUE_REGISTER(Ignore)
ZFENUM_END(ZFOperationTaskDuplicateAction)

// ============================================================
/**
 * @brief start param for #ZFOperation::taskStart
 */
zffinal zfclass ZF_ENV_EXPORT ZFOperationStartParam : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFOperationStartParam, ZFObject)

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

private:
    ZFOperationTaskData *_operationTaskData;
public:
    /**
     * @brief used to store the operation's task data
     *
     * must be created by #ZFOperation::createTaskData,
     * this object would be stored and used during
     * the entire operation task's life cycle,
     * you may use this object to store or pass data
     * by #ZFObject::tagSet
     */
    virtual void operationTaskDataSet(ZF_IN ZFOperationTaskData *operationTaskData);
    /**
     * @brief see #operationTaskData
     */
    virtual ZFOperationTaskData *operationTaskData(void)
    {
        return this->_operationTaskData;
    }
    /**
     * @brief override default cache expire time,
     *   #ZFOperationCacheExpireTimeUnspecified by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, cacheExpireTime, ZFPropertyInitValue(ZFOperationCacheExpireTimeUnspecified))
    /**
     * @brief do what if cache matched,
     *   #ZFOperationCacheMatchAction::e_Unspecified by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFOperationCacheMatchActionEnum, cacheMatchAction, ZFPropertyInitValue(ZFOperationCacheMatchAction::e_Unspecified))
    /**
     * @brief do what if duplicated task started,
     *   #ZFOperationTaskDuplicateAction::e_Unspecified by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFOperationTaskDuplicateActionEnum, taskDuplicateAction, ZFPropertyInitValue(ZFOperationTaskDuplicateAction::e_Unspecified))

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOperation_Types_h_

