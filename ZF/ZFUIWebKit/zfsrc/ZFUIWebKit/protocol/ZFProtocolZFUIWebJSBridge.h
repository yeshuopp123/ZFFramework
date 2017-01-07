/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFUIWebJSBridge.h
 * @brief protocol for ZFUIWebJSBridge
 */

#ifndef _ZFI_ZFProtocolZFUIWebJSBridge_h_
#define _ZFI_ZFProtocolZFUIWebJSBridge_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIWebKit/ZFUIWebJSBridge.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFUIWebJSBridge
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIWebJSBridge)
public:
    /**
     * @brief create native web JS bridge
     */
    virtual void *nativeWebJSBridgeCreate(ZF_IN ZFUIWebJSBridge *webJSBridge) = 0;
    /**
     * @brief destroy native web JS bridge
     */
    virtual void nativeWebJSBridgeDestroy(ZF_IN ZFUIWebJSBridge *webJSBridge,
                                          ZF_IN void *nativeWebJSBridge) = 0;

public:
    /** @brief see #ZFUIWebJSBridge::webMessageSend */
    virtual ZFSerializableData webMessageSend(ZF_IN ZFUIWebJSBridge *webJSBridge,
                                              ZF_IN_OUT ZFSerializableData &messageSend) = 0;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementations must notify when message sent from web
     */
    zffinal ZFSerializableData notifyWebMessageRecv(ZF_IN ZFUIWebJSBridge *webJSBridge,
                                                    ZF_IN_OUT ZFSerializableData &messageRecv)
    {
        return webJSBridge->_ZFP_ZFUIWebJSBridge_notifyWebMessageRecv(messageRecv);
    }
ZFPROTOCOL_INTERFACE_END(ZFUIWebJSBridge)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIWebJSBridge_h_

