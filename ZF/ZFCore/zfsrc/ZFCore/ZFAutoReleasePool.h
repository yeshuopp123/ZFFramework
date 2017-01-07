/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFAutoReleasePool.h
 * @brief automatically release all object in pool upon pool's drain or delete
 */

#ifndef _ZFI_ZFAutoReleasePool_h_
#define _ZFI_ZFAutoReleasePool_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFAutoReleasePoolPrivate;
/**
 * @brief auto release pool similar to Object-C's NSAutoreleasePool
 *
 * automatically release all object in pool upon pool's drain or delete
 * @see zfRelease
 */
zfclass ZF_ENV_EXPORT ZFAutoReleasePool : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFAutoReleasePool, ZFObject)

    /**
     * @brief global memory pool,
     *   you should drain it manually and
     *   you take full responsibility to keep it safe to drain
     *
     * note, this singleton instance is ensured with level #ZFLevelAppLow
     */
    ZFOBJECT_SINGLETON_DECLARE(ZFAutoReleasePool, instance)

public:
    /**
     * @brief init with max capacity
     */
    virtual ZFObject *objectOnInit(ZF_IN zfindex maxSize);
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief add an object which would be auto released upon pool's drain or delete
     */
    virtual void poolAdd(ZF_IN ZFObject *obj,
                         ZF_IN_OPT zfbool enableLeakTest = zftrue);
    /**
     * @brief add with owner info, used by leak test
     */
    virtual void poolAdd(ZF_IN ZFObject *obj,
                         ZF_IN const zfcharA *callerFile,
                         ZF_IN const zfcharA *callerFunction,
                         ZF_IN zfindex callerLine,
                         ZF_IN_OPT zfbool enableLeakTest = zftrue);
    /**
     * @brief manually drain a pool, all object in the pool would be released immediately
     * @note you must make sure all object in pool are safe to release
     *   when draining the pool
     */
    virtual void poolDrain(void);

private:
    _ZFP_ZFAutoReleasePoolPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAutoReleasePool_h_

