/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFTimer.h"
#include "ZFCore/ZFThread.h"
#include "ZFCore/ZFValue.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFTimerImpl_default_Timer : zfextends ZFObject
{
    ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(_ZFP_ZFTimerImpl_default_Timer, ZFObject)

public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFTimer) *impl;
    ZFTimer *timer;
private:
    zfbool timerThreadStarted;
    ZFIdentityGenerator threadCallbackIdGenerator;
    zfidentity threadCallbackId;
    ZFIdentityGenerator mainThreadCallbackIdGenerator;
    zfidentity mainThreadCallbackId;
protected:
    _ZFP_ZFTimerImpl_default_Timer(void)
    : impl(zfnull)
    , timer(zfnull)
    , timerThreadStarted(zffalse)
    , threadCallbackIdGenerator()
    , threadCallbackId(zfidentityInvalid)
    , mainThreadCallbackIdGenerator()
    , mainThreadCallbackId(zfidentityInvalid)
    {
    }

public:
    zffinal void timerStart(void)
    {
        this->threadCallbackId = ZFThreadExecuteInNewThread(
            ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, threadCallback)),
            ZFValue::identityValueCreate(this->threadCallbackIdGenerator.next()).toObject(),
            this /* pass this as dummy param to keep retain count */);
    }
    zffinal void timerStop(void)
    {
        this->threadCallbackIdGenerator.next();
        this->mainThreadCallbackIdGenerator.next();
        ZFThreadExecuteCancel(this->threadCallbackId);
        ZFThreadExecuteCancel(this->mainThreadCallbackId);
        if(this->timerThreadStarted)
        {
            this->timerThreadStarted = zffalse;
            this->impl->notifyTimerStop(this->timer);
        }
    }

public:
    ZFLISTENER_DECLARE(threadCallback)
    {
        zfidentity curId = ZFCastZFObjectUnchecked(ZFValue *, userData)->identityValue();

        // delay
        if(curId != this->threadCallbackIdGenerator.current()) {return ;}
        if(this->timer->timerDelay() > 0)
        {
            ZFThread::sleep(this->timer->timerDelay());
        }

        // start
        if(curId != this->threadCallbackIdGenerator.current()) {return ;}
        this->timerThreadStarted = zftrue;
        this->impl->notifyTimerStart(this->timer);
        if(curId != this->threadCallbackIdGenerator.current()) {return ;}

        // timer
        while(curId == this->threadCallbackIdGenerator.current())
        {
            if(this->timer->timerActivateInMainThread())
            {
                this->mainThreadCallbackId = ZFThreadExecuteInNewThread(
                    ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, mainThreadCallback)),
                    ZFValue::identityValueCreate(this->mainThreadCallbackIdGenerator.next()).toObject());
            }
            else
            {
                this->impl->notifyTimerActivate(this->timer);
            }
            ZFThread::sleep(this->timer->timerInterval());
        }
    }
    ZFLISTENER_DECLARE(mainThreadCallback)
    {
        zfidentity curId = ZFCastZFObjectUnchecked(ZFValue *, userData)->identityValue();
        if(curId != this->mainThreadCallbackIdGenerator.current()) {return ;}
        this->impl->notifyTimerActivate(this->timer);
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_default, ZFTimer, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("ZFFramework:ZFThread"))
public:
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer)
    {
        _ZFP_ZFTimerImpl_default_Timer *token = zfAllocWithoutLeakTest(_ZFP_ZFTimerImpl_default_Timer);
        token->impl = this;
        token->timer = timer;
        return token;
    }
    virtual void nativeTimerDestroy(ZF_IN ZFTimer *timer,
                                    ZF_IN void *nativeTimer)
    {
        _ZFP_ZFTimerImpl_default_Timer *token = ZFCastStatic(_ZFP_ZFTimerImpl_default_Timer *, nativeTimer);
        zfReleaseWithoutLeakTest(token);
    }
    virtual void timerStart(ZF_IN ZFTimer *timer)
    {
        _ZFP_ZFTimerImpl_default_Timer *token = ZFCastStatic(_ZFP_ZFTimerImpl_default_Timer *, timer->nativeTimer());
        token->impl = this;
        token->timer = timer;
        token->timerStart();
    }
    virtual void timerStop(ZF_IN ZFTimer *timer)
    {
        _ZFP_ZFTimerImpl_default_Timer *token = ZFCastStatic(_ZFP_ZFTimerImpl_default_Timer *, timer->nativeTimer());
        token->timerStop();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFTimerImpl_default)

ZF_NAMESPACE_GLOBAL_END

