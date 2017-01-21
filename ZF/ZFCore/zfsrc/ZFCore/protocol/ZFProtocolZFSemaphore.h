/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFSemaphore.h
 * @brief protocol for ZFThread (used in ZFSemaphore)
 */

#ifndef _ZFI_ZFProtocolZFSemaphore_h_
#define _ZFI_ZFProtocolZFSemaphore_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFSemaphore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFThread (used in ZFSemaphore)
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFSemaphore)
public:
    /**
     * @brief create native semaphore
     */
    virtual void *nativeSemaphoreCreate(ZF_IN ZFSemaphore *semaphore) zfpurevirtual;
    /**
     * @brief create native semaphore
     */
    virtual void nativeSemaphoreDestroy(ZF_IN ZFSemaphore *semaphore,
                                        ZF_IN void *nativeSemaphore) zfpurevirtual;

    /**
     * @brief see #ZFSemaphore::semaphoreLock
     */
    virtual void semaphoreLock(ZF_IN ZFSemaphore *semaphore) zfpurevirtual;
    /**
     * @brief see #ZFSemaphore::semaphoreUnlock
     */
    virtual void semaphoreUnlock(ZF_IN ZFSemaphore *semaphore) zfpurevirtual;

    /**
     * @brief see #ZFSemaphore::semaphoreSignal
     */
    virtual void semaphoreSignal(ZF_IN ZFSemaphore *semaphore) zfpurevirtual;
    /**
     * @brief see #ZFSemaphore::semaphoreBroadcast
     */
    virtual void semaphoreBroadcast(ZF_IN ZFSemaphore *semaphore) zfpurevirtual;
    /**
     * @brief see #ZFSemaphore::semaphoreWait
     */
    virtual void semaphoreWait(ZF_IN ZFSemaphore *semaphore) zfpurevirtual;
    /**
     * @brief see #ZFSemaphore::semaphoreWait
     */
    virtual zfbool semaphoreWait(ZF_IN ZFSemaphore *semaphore,
                                 ZF_IN const zftimet &miliSecsTimeout) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFSemaphore)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFSemaphore_h_

