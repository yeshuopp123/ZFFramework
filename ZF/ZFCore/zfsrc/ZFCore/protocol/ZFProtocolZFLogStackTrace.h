/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFLogStackTrace.h
 * @brief protocol for ZFLog
 */

#ifndef _ZFI_ZFProtocolZFLogStackTrace_h_
#define _ZFI_ZFProtocolZFLogStackTrace_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFLog.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFLog
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLogStackTrace)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @see #zfLogStackTrace
     */
    virtual void stackTrace(ZF_IN_OUT zfstring &ret,
                            ZF_IN_OPT const zfchar *prefix = zfnull,
                            ZF_IN_OPT zfindex ignoreLevel = 0,
                            ZF_IN_OPT zfindex maxLevel = 20) zfpurevirtual;

    /**
     * @see #zfLogCallerInfo
     */
    virtual void callerInfo(ZF_IN_OUT zfstring &ret,
                            ZF_IN_OPT zfindex ignoreLevel = 0) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFLogStackTrace)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFLogStackTrace_h_

