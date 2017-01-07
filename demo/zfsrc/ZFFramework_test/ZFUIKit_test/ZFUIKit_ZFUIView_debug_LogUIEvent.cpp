/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 0
ZF_GLOBAL_INITIALIZER_INIT(ZFUIKit_ZFUIView_debug_LogUIEvent)
{
    this->onUIEventListener = ZFCallbackForRawFunction(zfself::onUIEvent);
    ZFObjectGlobalEventObserverAdd(
            ZFUIView::EventViewOnEvent(),
            this->onUIEventListener
        );
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIKit_ZFUIView_debug_LogUIEvent)
{
    ZFObjectGlobalEventObserverRemove(
            ZFUIView::EventViewOnEvent(),
            this->onUIEventListener
        );
}
private:
    ZFListener onUIEventListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(onUIEvent)
    {
        zfLogTrimT() << listenerData.sender->objectInfoOfInstance() << listenerData.param0;
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIKit_ZFUIView_debug_LogUIEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

