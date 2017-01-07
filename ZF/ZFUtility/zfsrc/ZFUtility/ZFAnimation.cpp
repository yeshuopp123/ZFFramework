/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFAnimation.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFAnimationPrivate
zfclassNotPOD _ZFP_ZFAnimationPrivate
{
public:
    ZFObject *aniTarget;
    zfbool aniRunning;
    zfbool aniDelaying;
    zfidentity aniDelayTaskId;
    zfidentity aniDelayThreadId;
    zfbool aniStopCalled;
    zfbool aniStoppedByUser;
    ZFIdentityGenerator aniIdGenerator;

public:
    _ZFP_ZFAnimationPrivate(void)
    : aniTarget(zfnull)
    , aniRunning(zffalse)
    , aniDelaying(zffalse)
    , aniDelayTaskId(zfidentityInvalid)
    , aniDelayThreadId(zfidentityInvalid)
    , aniStopCalled(zffalse)
    , aniStoppedByUser(zffalse)
    , aniIdGenerator()
    {
    }
};

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAnimationDelayDataHolder, ZFLevelZFFrameworkNormal)
{
    this->delayOnFinishListener = ZFCallbackForRawFunction(zfself::delayOnFinish);
}
public:
    ZFListener delayOnFinishListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(delayOnFinish)
    {
        ZFAnimation *ani = listenerData.param0->to<ZFObjectHolder *>()->holdedObj;
        ZFValue *taskId = userData->to<ZFValue *>();
        ani->_ZFP_ZFAnimation_aniImplDelayNotifyFinish(taskId->identityValue());
    }
ZF_GLOBAL_INITIALIZER_END(ZFAnimationDelayDataHolder)

// ============================================================
ZFOBJECT_REGISTER(ZFAnimation)

ZFOBSERVER_EVENT_REGISTER(ZFAnimation, AniOnInvalid)
ZFOBSERVER_EVENT_REGISTER(ZFAnimation, AniOnDelayFinish)
ZFOBSERVER_EVENT_REGISTER(ZFAnimation, AniOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFAnimation, AniOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFAnimation, AniOnStopOrOnInvalid)

ZFObject *ZFAnimation::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAnimationPrivate);
    return this;
}
void ZFAnimation::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFAnimation::objectOnDeallocPrepare(void)
{
    this->aniStop();
    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFAnimation::objectHash(void)
{
    return (zfidentity)this->aniDuration();
}
ZFCompareResult ZFAnimation::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->aniDuration() == another->aniDuration())
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

void ZFAnimation::aniTargetSet(ZF_IN ZFObject *aniTarget)
{
    zfCoreAssertWithMessage(!d->aniRunning, zfTextA("change animation's target while animation is running"));
    d->aniTarget = aniTarget;
}
ZFObject *ZFAnimation::aniTarget(void)
{
    return d->aniTarget;
}

void ZFAnimation::aniStart(void)
{
    zfCoreAssertWithMessage(!d->aniRunning, zfTextA("start an animation which already started"));
    d->aniStoppedByUser = zffalse;
    if(!this->aniValid())
    {
        this->aniOnInvalid();
        this->aniOnStopOrOnInvalid(zffalse);
        return ;
    }

    zfRetain(this);
    zfRetain(this->aniTarget());

    d->aniRunning = zftrue;
    d->aniStopCalled = zffalse;
    d->aniIdGenerator.next();
    this->aniOnStart();

    if(this->aniDelay() <= 0)
    {
        d->aniDelaying = zffalse;
        this->aniImplStart();
        return ;
    }

    d->aniDelaying = zftrue;
    this->aniImplDelay();
}
zfbool ZFAnimation::aniRunning(void)
{
    return d->aniRunning;
}
void ZFAnimation::aniStop(void)
{
    if(!(d->aniRunning) || d->aniStopCalled)
    {
        return ;
    }
    d->aniStopCalled = zftrue;
    d->aniStoppedByUser = zftrue;
    d->aniIdGenerator.next();
    if(d->aniDelaying)
    {
        d->aniDelaying = zffalse;
        this->aniImplDelayCancel();
    }
    else
    {
        this->aniImplStop();
    }
    this->aniImplNotifyStop();
}
zfbool ZFAnimation::aniStoppedByUser(void)
{
    return d->aniStoppedByUser;
}

zfidentity ZFAnimation::aniId(void)
{
    return d->aniIdGenerator.current();
}

zfbool ZFAnimation::aniValid(void)
{
    return this->aniImplCheckValid();
}
zfbool ZFAnimation::aniImplCheckValid(void)
{
    return (this->aniDuration() > 0);
}

void ZFAnimation::_ZFP_ZFAnimation_aniImplDelayNotifyFinish(ZF_IN zfidentity taskId)
{
    if(taskId == d->aniDelayTaskId)
    {
        this->aniImplDelayNotifyFinish();
    }
}
void ZFAnimation::aniImplDelay(void)
{
    ++(d->aniDelayTaskId);
    d->aniDelayThreadId = ZFThreadExecuteInMainThreadAfterDelay(
        this->aniDelay(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAnimationDelayDataHolder)->delayOnFinishListener,
        ZFValue::identityValueCreate(d->aniDelayTaskId).toObject(),
        this->objectHolder());
}
void ZFAnimation::aniImplDelayCancel(void)
{
    ++(d->aniDelayTaskId);
    ZFThreadExecuteCancel(d->aniDelayThreadId);
}
void ZFAnimation::aniImplDelayNotifyFinish(void)
{
    zfCoreAssertWithMessage(d->aniDelaying, zfTextA("notify delay finish an animation which not delaying"));
    d->aniDelaying = zffalse;
    this->aniImplStart();
}

void ZFAnimation::aniImplStart(void)
{
    // subclass should override this
}
void ZFAnimation::aniImplStop(void)
{
    // subclass should override this
}

void ZFAnimation::aniImplNotifyStop(void)
{
    zfCoreAssertWithMessage(d->aniRunning, zfTextA("notify stop an animation which not started"));
    ZFObject *aniTargetToRelease = this->aniTarget();

    d->aniRunning = zffalse;
    d->aniStopCalled = zffalse;
    this->aniOnStop();
    this->aniOnStopOrOnInvalid(zftrue);

    zfRelease(aniTargetToRelease);
    zfRelease(this);
}

ZF_NAMESPACE_GLOBAL_END

