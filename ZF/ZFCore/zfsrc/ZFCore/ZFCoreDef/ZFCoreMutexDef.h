/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreMutexDef.h
 * @brief core mutex
 */

#ifndef _ZFI_ZFCoreMutexDef_h_
#define _ZFI_ZFCoreMutexDef_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreUtilDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief impl object for #ZFCoreMutexLock
 */
zfclassNotPOD ZF_ENV_EXPORT ZFCoreMutexImpl
{
public:
    /** @cond ZFPrivateDoc */
    ZFCoreMutexImpl(void) {}
    virtual ~ZFCoreMutexImpl(void) {}
    /** @endcond */
public:
    /**
     * @brief lock (must be reentrant)
     */
    virtual void mutexImplLock(void) = 0;
    /**
     * @brief unlock
     */
    virtual void mutexImplUnlock(void) = 0;
};
extern ZF_ENV_EXPORT ZFCoreMutexImpl *_ZFP_ZFCoreMutexImplObject;
/**
 * @brief #ZFCoreMutexLock's implementation, change with caution
 */
extern ZF_ENV_EXPORT void ZFCoreMutexImplSet(ZF_IN ZFCoreMutexImpl *impl);
/**
 * @brief see #ZFCoreMutexImplSet
 */
extern ZF_ENV_EXPORT ZFCoreMutexImpl *ZFCoreMutexImplGet(void);

extern ZF_ENV_EXPORT ZFFuncAddrType _ZFP_ZFCoreMutexImpl_lock;
extern ZF_ENV_EXPORT ZFFuncAddrType _ZFP_ZFCoreMutexImpl_unlock;

/**
 * @brief internal use only
 *
 * you may change it at run time by changing #ZFCoreMutexImplSet
 */
#define ZFCoreMutexLock() _ZFP_ZFCoreMutexImpl_lock()
/** @brief see #ZFCoreMutexLock */
#define ZFCoreMutexUnlock() _ZFP_ZFCoreMutexImpl_unlock()

/**
 * @brief see #ZFCoreMutexLocker
 */
zfclassLikePOD ZF_ENV_EXPORT ZFCoreMutexLockerHolder
{
public:
    /** @cond ZFPrivateDoc */
    ZFCoreMutexLockerHolder(void)
    {
        ZFCoreMutexLock();
    }
    ~ZFCoreMutexLockerHolder(void)
    {
        ZFCoreMutexUnlock();
    }
    /** @endcond */
};
/**
 * @brief util method to lock current block
 *
 * usage:
 * @code
 *   {
 *       ZFCoreMutexLocker(); // lock
 *       if(...) return ; // safe to return
 *   } // unlock after block
 * @endcode
 *
 * you may also use the helper #ZFCoreMutexLockerHolder
 * @code
 *   Type var = (ZFCoreMutexLockerHolder(), yourFuncSynced());
 * @endcode
 */
#define ZFCoreMutexLocker() ZFCoreMutexLockerHolder ZFUniqueName(ZFCoreMutexLocker)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreMutexDef_h_

