/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIRootView.h"

#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIRootView)

ZFOBSERVER_EVENT_REGISTER(ZFUIRootView, RootViewOnRequestLayout)

void ZFUIRootView::scaleSet(ZF_IN zffloat scale)
{
    if(scale > 0)
    {
        this->_ZFP_ZFUIView_scaleSetRecursively(scale, this->scaleGetForImpl());
    }
}

void ZFUIRootView::layoutRequest(void)
{
    if(!this->layoutRequested())
    {
        zfsuper::layoutRequest();
        this->rootViewOnRequestLayout();
    }
    else
    {
        zfsuper::layoutRequest();
    }
}

void ZFUIRootView::viewOnAddToParent(ZF_IN ZFUIView *parent)
{
    zfCoreCriticalMessage(zfTextA("you must not add ZFUIRootView to another parent, parent: %s"), zfsCoreZ2A(ZFObjectInfo(parent).cString()));
}
void ZFUIRootView::viewOnRemoveFromParent(ZF_IN ZFUIView *parent)
{
    zfCoreCriticalMessage(zfTextA("you must not add ZFUIRootView to another parent, parent: %s"), zfsCoreZ2A(ZFObjectInfo(parent).cString()));
}

void ZFUIRootView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = sizeHint;
}

ZF_NAMESPACE_GLOBAL_END

