/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFJsonEscapeChar.h
 * @brief protocol for ZFJson
 */

#ifndef _ZFI_ZFProtocolZFJsonEscapeChar_h_
#define _ZFI_ZFProtocolZFJsonEscapeChar_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFAlgorithm/ZFJson.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFJson
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFJsonEscapeChar)
    public:
    /**
     * @brief see #ZFJsonEscapeCharEncode
     */
    virtual void jsonEscapeCharEncode(ZF_OUT const ZFOutputCallback &dst,
                                      ZF_IN const zfchar *src,
                                      ZF_IN_OPT zfindex count = zfindexMax) zfpurevirtual;
    /**
     * @brief see #ZFJsonEscapeCharEncode
     */
    virtual void jsonEscapeCharDecode(ZF_OUT const ZFOutputCallback &dst,
                                      ZF_IN const zfchar *src,
                                      ZF_IN_OPT zfindex count = zfindexMax) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFJsonEscapeChar)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFJsonEscapeChar_h_

