/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFAnimationTimeLine.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFAnimationTimeLinePrivate
zfclass _ZFP_ZFAnimationTimeLinePrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFAnimationTimeLinePrivate, ZFObject)

public:
    ZFAnimationTimeLine *pimplOwner;
    ZFTimer *timer;
    zftimet timeStart;

    ZFLISTENER_DECLARE(timerOnEvent)
    {
        zffloat progress = zfmApplyRange(
            (zffloat)(ZFTime::timestamp() - this->timeStart) / this->pimplOwner->aniDuration()
            , (zffloat)0, (zffloat)1);
        if(this->pimplOwner->aniTimeLineCurve() != zfnull)
        {
            progress = this->pimplOwner->aniTimeLineCurve()->progressUpdate(progress);
        }
        this->pimplOwner->aniTimeLineOnUpdate(progress);
        if(ZFTime::timestamp() - this->timeStart >= this->pimplOwner->aniDuration())
        {
            this->timer->timerStop();
            this->pimplOwner->aniImplNotifyStop();
        }
    }

public:
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->timer = zfAllocWithoutLeakTest(ZFTimer);
        this->timer->observerAdd(ZFTimer::EventTimerOnActivate(),
            ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, timerOnEvent)));
        return this;
    }
    virtual void objectOnDealloc(void)
    {
        zfReleaseWithoutLeakTest(this->timer);
        zfsuper::objectOnDealloc();
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAnimationTimeLine)

ZFOBSERVER_EVENT_REGISTER(ZFAnimationTimeLine, AniTimeLineOnUpdate)

// ============================================================
// object
ZFObject *ZFAnimationTimeLine::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAllocWithoutLeakTest(_ZFP_ZFAnimationTimeLinePrivate);
    d->pimplOwner = this;
    return this;
}
void ZFAnimationTimeLine::objectOnDealloc(void)
{
    zfReleaseWithoutLeakTest(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfidentity ZFAnimationTimeLine::objectHash(void)
{
    return zfidentityHash(zfsuper::objectHash()
        , this->aniTimeLineInterval()
        );
}
ZFCompareResult ZFAnimationTimeLine::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(zfsuper::objectCompare(anotherObj) != ZFCompareTheSame)
    {
        return ZFCompareUncomparable;
    }
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->aniTimeLineInterval() == another->aniTimeLineInterval())
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

// ============================================================
// start stop
void ZFAnimationTimeLine::aniImplStart(void)
{
    zfsuper::aniImplStart();
    d->timeStart = ZFTime::timestamp();
    if(this->aniDuration() < this->aniTimeLineInterval())
    {
        d->timer->timerIntervalSet(this->aniDuration());
    }
    else
    {
        d->timer->timerIntervalSet(this->aniTimeLineInterval());
    }

    d->timer->timerActivateInMainThreadSet(this->aniTimeLineNotifyInMainThread());
    d->timer->timerStart();
}
void ZFAnimationTimeLine::aniImplStop(void)
{
    d->timer->timerStop();
    zfsuper::aniImplStop();
}

void ZFAnimationTimeLine::aniTimeLineOnUpdate(ZF_IN zffloat progress)
{
    this->observerNotify(
        ZFAnimationTimeLine::EventAniTimeLineOnUpdate(),
        ZFValue::floatValueCreate(progress).toObject());
}

ZF_NAMESPACE_GLOBAL_END

