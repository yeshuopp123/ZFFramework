/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIKit_test.h"

#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 0
ZF_GLOBAL_INITIALIZER_INIT(ZFUIKit_ZFUIView_debug_AutoPrintViewTree)
{
    if(!ZFPROTOCOL_IS_AVAILABLE(ZFUIView))
    {
        return ;
    }
    ZFLISTENER_LOCAL(windowOnPause, {
        ZFUISysWindow *sysWindow = listenerData.sender->to<ZFUISysWindow *>();
        ZFUIViewPrintViewTree(sysWindow->rootView());
    })
    this->windowOnPauseListener = windowOnPause;
    ZFObjectGlobalEventObserverAdd(
        ZFUISysWindow::EventSysWindowOnPause(), this->windowOnPauseListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIKit_ZFUIView_debug_AutoPrintViewTree)
{
    ZFObjectGlobalEventObserverRemove(
        ZFUISysWindow::EventSysWindowOnPause(), this->windowOnPauseListener);
}
private:
    ZFListener windowOnPauseListener;
ZF_GLOBAL_INITIALIZER_END(ZFUIKit_ZFUIView_debug_AutoPrintViewTree)
#endif

ZF_NAMESPACE_GLOBAL_END

