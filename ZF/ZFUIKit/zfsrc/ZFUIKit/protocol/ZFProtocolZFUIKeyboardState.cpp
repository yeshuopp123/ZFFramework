/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFProtocolZFUIKeyboardState.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFUIKeyboardState)

// ============================================================
static zfbool _ZFP_ZFUIKeyboardStateBuiltinImpl_available = zffalse;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIKeyboardStateBuiltinImpl_DataHolder, ZFLevelZFFrameworkNormal)
{
    _ZFP_ZFUIKeyboardStateBuiltinImpl_available = zftrue;
    this->implRegisterFlag = zffalse;
    this->viewOnEventListener = ZFCallbackForRawFunction(zfself::viewOnEvent);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIKeyboardStateBuiltinImpl_DataHolder)
{
    this->implUnregister();
    _ZFP_ZFUIKeyboardStateBuiltinImpl_available = zffalse;
}
public:
    ZFCoreArrayPOD<ZFUIKeyCodeEnum> keyPressed;
    ZFCoreArrayPOD<zfflags> keyPressedRaw;
    ZFListener viewOnEventListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(viewOnEvent)
    {
        ZFUIKeyEvent *event = ZFCastZFObject(ZFUIKeyEvent *, listenerData.param0);
        if(event != zfnull)
        {
            ZFUIKeyboardStateBuiltinImplNotifyKeyEvent(event);
        }
    }

    zfbool implRegisterFlag;
    void implRegister(void)
    {
        if(this->implRegisterFlag)
        {
            return ;
        }
        this->implRegisterFlag = zftrue;

        ZFObjectGlobalEventObserverAdd(ZFObject::ObserverAddParam()
                .eventIdSet(ZFUIView::EventViewOnEvent())
                .observerSet(this->viewOnEventListener)
                .observerLevelSet(ZFLevelZFFrameworkEssential)
            );
    }
    void implUnregister(void)
    {
        if(!this->implRegisterFlag)
        {
            return ;
        }
        this->implRegisterFlag = zffalse;

        ZFObjectGlobalEventObserverRemove(
            ZFUIView::EventViewOnEvent(),
            this->viewOnEventListener);
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIKeyboardStateBuiltinImpl_DataHolder)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIKeyboardStateBuiltinImpl, ZFUIKeyboardState, ZFProtocolLevel::e_Default)
public:
    virtual zfbool keyPressed(ZF_IN ZFUIKeyCodeEnum keyCode)
    {
        return (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder)->keyPressed.find(keyCode) != zfindexMax);
    }
    virtual zfbool keyPressedRaw(ZF_IN zfflags keyCodeRaw)
    {
        return (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder)->keyPressedRaw.find(keyCodeRaw) != zfindexMax);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIKeyboardStateBuiltinImpl)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIKeyboardStateBuiltinImpl)

// ============================================================
void ZFUIKeyboardStateBuiltinImplRegister(void)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder)->implRegister();
}
void ZFUIKeyboardStateBuiltinImplUnregister(void)
{
    if(_ZFP_ZFUIKeyboardStateBuiltinImpl_available)
    {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder)->implUnregister();
    }
}

void ZFUIKeyboardStateBuiltinImplNotifyKeyEvent(ZF_IN ZFUIKeyEvent *event)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIKeyboardStateBuiltinImpl_DataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder);
    switch(event->keyAction)
    {
        case ZFUIKeyAction::e_KeyDown:
        case ZFUIKeyAction::e_KeyRepeat:
        {
            d->keyPressed.add(event->keyCode);
            d->keyPressedRaw.add(event->keyCodeRaw);
        }
            break;
        case ZFUIKeyAction::e_KeyUp:
        case ZFUIKeyAction::e_KeyCancel:
        {
            zfindex index = 0;
            for( ; index < d->keyPressed.count(); ++index)
            {
                if(d->keyPressed[index] == event->keyCode
                    && d->keyPressedRaw[index] == event->keyCodeRaw)
                {
                    break;
                }
            }
            if(index < d->keyPressed.count())
            {
                d->keyPressed.remove(index);
                d->keyPressedRaw.remove(index);
            }
        }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

ZF_NAMESPACE_GLOBAL_END

