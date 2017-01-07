/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIOnScreenKeyboardState.h"
#include "protocol/ZFProtocolZFUIOnScreenKeyboardState.h"
#include "ZFUIWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIOnScreenKeyboardState)

ZFOBSERVER_EVENT_REGISTER(ZFUIOnScreenKeyboardState, KeyboardStateOnChange)

ZFUIOnScreenKeyboardState *ZFUIOnScreenKeyboardState::instanceForSysWindow(ZF_IN_OPT ZFUISysWindow *ownerSysWindow /* = zfnull */)
{
    if(ownerSysWindow == zfnull)
    {
        ownerSysWindow = ZFUISysWindow::mainWindow();
    }
    ZFUIOnScreenKeyboardState *ret = ownerSysWindow->tagGet<ZFUIOnScreenKeyboardState *>(zfText("_ZFP_ZFUIOnScreenKeyboardState"));
    if(ret == zfnull)
    {
        zfblockedAllocWithoutLeakTest(ZFUIOnScreenKeyboardState, tmp);
        ownerSysWindow->tagSetMarkCached(zfText("_ZFP_ZFUIOnScreenKeyboardState"), tmp);
        tmp->_ZFP_ZFUIOnScreenKeyboardState_ownerSysWindow = ownerSysWindow;
        ret = tmp;
    }
    return ret;
}
ZFUIOnScreenKeyboardState *ZFUIOnScreenKeyboardState::instanceForView(ZF_IN_OPT ZFUIView *view /* = zfnull */)
{
    return ZFUIOnScreenKeyboardState::instanceForSysWindow(ZFUIWindow::sysWindowForView(view));
}

ZFUISysWindow *ZFUIOnScreenKeyboardState::ownerSysWindow(void)
{
    return _ZFP_ZFUIOnScreenKeyboardState_ownerSysWindow;
}

zfbool ZFUIOnScreenKeyboardState::keyboardShowing(void)
{
    return this->_ZFP_ZFUIOnScreenKeyboardState_keyboardShowing;
}
const ZFUIRect &ZFUIOnScreenKeyboardState::keyboardFrame(void)
{
    return this->_ZFP_ZFUIOnScreenKeyboardState_keyboardFrame;
}
const ZFUIRect &ZFUIOnScreenKeyboardState::keyboardFramePrev(void)
{
    return this->_ZFP_ZFUIOnScreenKeyboardState_keyboardFramePrev;
}

void ZFUIOnScreenKeyboardState::keyboardFixClientFrame(ZF_OUT ZFUIRect &clientFrame)
{
    zffloat scale = this->ownerSysWindow()->rootView()->scaleGetFixed();
    ZFPROTOCOL_ACCESS(ZFUIOnScreenKeyboardState)->keyboardFixClientFrame(
        this,
        ZFUIRectApplyScale(this->keyboardFrame(), scale),
        clientFrame);
    ZFUIRectApplyScaleReversely(clientFrame, clientFrame, scale);
}

void ZFUIOnScreenKeyboardState::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsFromPointerT(ret, this->ownerSysWindow());
    if(this->keyboardShowing())
    {
        ret += zfText(" keyboardFrame: ");
        ZFUIRectToString(ret, this->keyboardFrame());
    }
    else
    {
        ret += zfText(" keyboardNotShow");
    }
}

ZF_NAMESPACE_GLOBAL_END

