/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIWebJSBridge.h"
#include "protocol/ZFProtocolZFUIWebJSBridge.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUIWebJSBridgeSendData)
ZFOBJECT_REGISTER(ZFUIWebJSBridgeRecvData)

// ============================================================
zfclassNotPOD _ZFP_ZFUIWebJSBridgePrivate
{
public:
    void *nativeWebJSBridge;
    ZFUIWebView *webView;

public:
    _ZFP_ZFUIWebJSBridgePrivate(void)
    : nativeWebJSBridge(zfnull)
    , webView(zfnull)
    {
    }
};

// ============================================================
// ZFUIWebJSBridge
ZFOBJECT_REGISTER(ZFUIWebJSBridge)

ZFOBSERVER_EVENT_REGISTER(ZFUIWebJSBridge, WebMessageBeforeSend)
ZFOBSERVER_EVENT_REGISTER(ZFUIWebJSBridge, WebMessageAfterSend)
ZFOBSERVER_EVENT_REGISTER(ZFUIWebJSBridge, WebMessageBeforeRecv)
ZFOBSERVER_EVENT_REGISTER(ZFUIWebJSBridge, WebMessageAfterRecv)

#define _ZFP_ZFUIWebJSBridge_tagKey zfText("_ZFP_ZFUIWebJSBridge_tagKey")
ZFUIWebJSBridge *ZFUIWebJSBridge::instanceForWebView(ZF_IN ZFUIWebView *webView)
{
    ZFUIWebJSBridge *ret = webView->tagGet<ZFUIWebJSBridge *>(_ZFP_ZFUIWebJSBridge_tagKey);
    if(ret == zfnull)
    {
        zfblockedAlloc(ZFUIWebJSBridge, tmp);
        webView->tagSet(_ZFP_ZFUIWebJSBridge_tagKey, tmp);
        ret->d->webView = webView;
        ret = tmp;
    }
    return ret;
}

ZFUIWebView *ZFUIWebJSBridge::webView(void)
{
    return d->webView;
}

ZFSerializableData ZFUIWebJSBridge::webMessageSend(ZF_IN_OUT ZFSerializableData &messageSend)
{
    zfblockedAllocWithoutLeakTest(ZFUIWebJSBridgeSendData, dataSend);
    dataSend->messageSend = messageSend;
    this->webMessageBeforeSend(dataSend);
    dataSend->messageResponse = ZFPROTOCOL_ACCESS(ZFUIWebJSBridge)->webMessageSend(this, messageSend);
    this->webMessageAfterSend(dataSend);
    return dataSend->messageResponse;
}
ZFSerializableData ZFUIWebJSBridge::_ZFP_ZFUIWebJSBridge_notifyWebMessageRecv(ZF_IN_OUT ZFSerializableData &messageRecv)
{
    zfblockedAllocWithoutLeakTest(ZFUIWebJSBridgeRecvData, dataRecv);
    dataRecv->messageRecv = messageRecv;
    this->webMessageBeforeRecv(dataRecv);
    this->webMessageAfterRecv(dataRecv);
    return dataRecv->messageResponse;
}

ZFObject *ZFUIWebJSBridge::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfpoolNew(_ZFP_ZFUIWebJSBridgePrivate);
    d->nativeWebJSBridge = ZFPROTOCOL_ACCESS(ZFUIWebJSBridge)->nativeWebJSBridgeCreate(this);

    return this;
}
void ZFUIWebJSBridge::objectOnDealloc(void)
{
    ZFPROTOCOL_ACCESS(ZFUIWebJSBridge)->nativeWebJSBridgeDestroy(this, d->nativeWebJSBridge);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

