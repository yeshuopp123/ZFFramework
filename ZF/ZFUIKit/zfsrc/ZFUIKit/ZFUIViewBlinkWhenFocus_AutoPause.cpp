/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIViewBlinkWhenFocus_AutoPause.h"

#include "ZFUIWindow.h"
#include "ZFUISysWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocus_AutoPause_DataHolder, ZFLevelZFFrameworkLow)
{
    this->doActionListener = ZFCallbackForRawFunction(zfself::doAction);
    ZFObjectGlobalEventObserverAdd(ZFUIWindow::EventWindowOnShow(), this->doActionListener);
    ZFObjectGlobalEventObserverAdd(ZFUIWindow::EventWindowOnHide(), this->doActionListener);
    ZFObjectGlobalEventObserverAdd(ZFUISysWindow::EventSysWindowOnResume(), this->doActionListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocus_AutoPause_DataHolder)
{
    ZFObjectGlobalEventObserverRemove(ZFUIWindow::EventWindowOnShow(), this->doActionListener);
    ZFObjectGlobalEventObserverRemove(ZFUIWindow::EventWindowOnHide(), this->doActionListener);
    ZFObjectGlobalEventObserverRemove(ZFUISysWindow::EventSysWindowOnResume(), this->doActionListener);
}
public:
    ZFListener doActionListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(doAction)
    {
        ZFUIViewBlinkWhenFocusAutoApplyPauseForTime();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocus_AutoPause_DataHolder)

ZF_NAMESPACE_GLOBAL_END

