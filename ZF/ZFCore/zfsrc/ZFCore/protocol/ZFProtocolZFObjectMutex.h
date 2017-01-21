/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFObjectMutex.h
 * @brief protocol for ZFThread (used in ZFObject)
 */

#ifndef _ZFI_ZFProtocolZFObjectMutex_h_
#define _ZFI_ZFProtocolZFObjectMutex_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for mutex (used in ZFObject)
 * @warning this protocol's implementations must not access any ZFObject types
 * @warning this is an essential module for ZFObject,
 *   must be registered statically by #ZFPROTOCOL_IMPLEMENTATION_REGISTER
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFObjectMutex)
public:
    /**
     * @brief create native mutex
     */
    virtual ZFObjectMutexImpl *nativeMutexCreate(void) zfpurevirtual;
    /**
     * @brief create native mutex
     */
    virtual void nativeMutexDestroy(ZF_IN ZFObjectMutexImpl *nativeMutex) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFObjectMutex)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFObjectMutex_h_

