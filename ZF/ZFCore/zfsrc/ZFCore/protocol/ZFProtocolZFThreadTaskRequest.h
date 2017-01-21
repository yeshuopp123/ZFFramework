/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFThreadTaskRequest.h
 * @brief protocol for #ZFThreadTaskRequest
 */

#ifndef _ZFI_ZFProtocolZFThreadTaskRequest_h_
#define _ZFI_ZFProtocolZFThreadTaskRequest_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFThreadTaskRequest
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFThreadTaskRequest)
public:
    /**
     * @brief see #ZFThreadTaskRequest
     */
    virtual void taskRequest(ZF_IN const ZFListener &task,
                             ZF_IN ZFObject *param0,
                             ZF_IN ZFObject *param1) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFThreadTaskRequest)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFThreadTaskRequest_h_

