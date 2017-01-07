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

#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFUIViewFocusState_test)
{
    this->listener = ZFCallbackForRawFunction(zfself::viewFocusOnChange);
    ZFObjectGlobalEventObserverAdd(ZFUIView::EventViewFocusOnChange(), this->listener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewFocusState_test)
{
    ZFObjectGlobalEventObserverRemove(ZFUIView::EventViewFocusOnChange(), this->listener);
}
private:
    ZFListener listener;
    static ZFLISTENER_PROTOTYPE_EXPAND(viewFocusOnChange)
    {
        zfLogTrimT() << zfText("[ZFUIViewFocus]")
            << listenerData.sender
            << zfText("changed to") << listenerData.sender->to<ZFUIView *>()->viewFocused();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusState_test)
#endif

ZF_NAMESPACE_GLOBAL_END

