/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIWebJSBridge.h
 * @brief web JS bridge
 */

#ifndef _ZFI_ZFUIWebJSBridge_h_
#define _ZFI_ZFUIWebJSBridge_h_

#include "ZFUIWebView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief store necessary data for #ZFUIWebJSBridge::webMessageBeforeSend
 */
zfclass ZF_ENV_EXPORT ZFUIWebJSBridgeSendData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIWebJSBridgeSendData, ZFObject)

public:
    /**
     * @brief message sent to web
     */
    ZFSerializableData messageSend;
    /**
     * @brief message response from web
     */
    ZFSerializableData messageResponse;
};

// ============================================================
/**
 * @brief store necessary data for #ZFUIWebJSBridge::webMessageBeforeRecv
 */
zfclass ZF_ENV_EXPORT ZFUIWebJSBridgeRecvData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIWebJSBridgeRecvData, ZFObject)

public:
    /**
     * @brief message received from web
     */
    ZFSerializableData messageRecv;
    /**
     * @brief message response to web
     */
    ZFSerializableData messageResponse;
};

// ============================================================
zfclassFwd _ZFP_ZFUIWebJSBridgePrivate;
/**
 * @brief web JS bridge
 */
zfclass ZF_ENV_EXPORT ZFUIWebJSBridge : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIWebJSBridge, ZFObject)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when receive message from web\n
     * param0 is a #ZFUIWebJSBridgeSendData
     */
    ZFOBSERVER_EVENT(WebMessageBeforeSend)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when receive message from web\n
     * param0 is a #ZFUIWebJSBridgeSendData
     */
    ZFOBSERVER_EVENT(WebMessageAfterSend)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when receive message from web\n
     * param0 is a #ZFUIWebJSBridgeRecvData
     */
    ZFOBSERVER_EVENT(WebMessageBeforeRecv)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when receive message from web\n
     * param0 is a #ZFUIWebJSBridgeRecvData
     */
    ZFOBSERVER_EVENT(WebMessageAfterRecv)

public:
    /**
     * @brief access the bridge via the webView
     *
     * return old one if exist or create new one if not exist\n
     * the bridge object would be stored as an object tag to it's owner webView
     */
    static ZFUIWebJSBridge *instanceForWebView(ZF_IN ZFUIWebView *webView);

public:
    /**
     * @brief owner of this JS bridge
     */
    virtual ZFUIWebView *webView(void);

public:
    /**
     * @brief send message to web
     *
     * note: the message to send can be modified by observing #EventWebMessageBeforeSend
     */
    virtual ZFSerializableData webMessageSend(ZF_IN_OUT ZFSerializableData &messageSend);
    zffinal ZFSerializableData _ZFP_ZFUIWebJSBridge_notifyWebMessageRecv(ZF_IN_OUT ZFSerializableData &messageRecv);

protected:
    ZFCLASS_PRIVATE_ALLOC("can only be created by ZFUIWebJSBridge::instanceForWebView")
    zfoverride
    virtual ZFObject *objectOnInit(void);
public:
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    /** @brief see #EventWebMessageBeforeSend */
    virtual inline void webMessageBeforeSend(ZF_IN ZFUIWebJSBridgeSendData *dataSend)
    {
        this->observerNotify(ZFUIWebJSBridge::EventWebMessageBeforeSend(), dataSend);
    }
    /** @brief see #EventWebMessageAfterSend */
    virtual inline void webMessageAfterSend(ZF_IN ZFUIWebJSBridgeSendData *dataSend)
    {
        this->observerNotify(ZFUIWebJSBridge::EventWebMessageAfterSend(), dataSend);
    }
    /** @brief see #EventWebMessageBeforeRecv */
    virtual inline void webMessageBeforeRecv(ZF_IN ZFUIWebJSBridgeRecvData *dataRecv)
    {
        this->observerNotify(ZFUIWebJSBridge::EventWebMessageBeforeRecv(), dataRecv);
    }
    /** @brief see #EventWebMessageAfterRecv */
    virtual inline void webMessageAfterRecv(ZF_IN ZFUIWebJSBridgeRecvData *dataRecv)
    {
        this->observerNotify(ZFUIWebJSBridge::EventWebMessageAfterRecv(), dataRecv);
    }

private:
    _ZFP_ZFUIWebJSBridgePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIWebJSBridge_h_

