/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIAnimatedImage.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUIAnimatedImageAniFrame)

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIAnimatedImage)

// ============================================================
// _ZFP_ZFUIAnimatedImagePrivate
zfclassNotPOD _ZFP_ZFUIAnimatedImagePrivate
{
public:
    ZFUIAnimatedImage *pimplOwner;
    zfbool aniRunning;
    zfindex aniFrameIndexLast;
    zfindex aniFrameIndexCurrent;
    zfindex aniRepeatCountCurrent;
    zfidentity aniFrameTaskId;
    ZFListener aniFramesOnChangeListener;
    ZFListener aniFrameDurationOnFinishListener;

public:
    _ZFP_ZFUIAnimatedImagePrivate(void)
    : pimplOwner(zfnull)
    , aniRunning(zffalse)
    , aniFrameIndexLast(zfindexMax)
    , aniFrameIndexCurrent(zfindexMax)
    , aniRepeatCountCurrent(0)
    , aniFrameTaskId(zfidentityInvalid)
    , aniFramesOnChangeListener(ZFCallbackForRawFunction(_ZFP_ZFUIAnimatedImagePrivate::aniFramesOnChange))
    , aniFrameDurationOnFinishListener(ZFCallbackForRawFunction(_ZFP_ZFUIAnimatedImagePrivate::aniFrameDurationOnFinish))
    {
    }

public:
    zfindex aniIndexNext(ZF_IN zfindex index)
    {
        return ((index + 1) % this->pimplOwner->aniFrames()->count());
    }
    void aniRunNext(void)
    {
        zfblockedRelease(zfRetain(this->pimplOwner));

        zfbool loopFinish = zffalse;
        if(this->aniFrameIndexLast != zfindexMax
            && this->aniFrameIndexCurrent == this->pimplOwner->aniFrames()->count() - 1)
        {
            loopFinish = zftrue;
        }

        do
        {
            this->aniFrameIndexLast = this->aniFrameIndexCurrent;
            this->aniFrameIndexCurrent = this->aniIndexNext(this->aniFrameIndexCurrent);
            this->pimplOwner->aniFrameOnUpdate();
            if(!this->aniRunning)
            {
                return ;
            }
        } while(this->pimplOwner->aniFrameDurationCurrent() == 0);

        if(loopFinish)
        {
            if(this->aniRepeatCountCurrent < this->pimplOwner->aniRepeatCount())
            {
                ++this->aniRepeatCountCurrent;
                this->pimplOwner->aniOnRepeat();
                if(!this->aniRunning)
                {
                    return ;
                }
            }
            else
            {
                this->pimplOwner->aniStop();
                return ;
            }
        }

        this->pimplOwner->aniOnUpdate();
        if(!this->aniRunning)
        {
            return ;
        }
        this->aniFrameTaskId = ZFThreadExecuteInMainThreadAfterDelay(
            this->pimplOwner->aniFrameDurationFixedCurrent(),
            this->aniFrameDurationOnFinishListener,
            this->pimplOwner->objectHolder());
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(aniFramesOnChange)
    {
        ZFUIAnimatedImage *pimplOwner = userData->to<ZFObjectHolder *>()->holdedObj;
        if(pimplOwner->aniRunning())
        {
            zfCoreCriticalMessageTrim(zfTextA("[ZFUIAnimatedImage] you must not modify aniFrame while animating"));
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(aniFrameDurationOnFinish)
    {
        ZFUIAnimatedImage *pimplOwner = userData->to<ZFObjectHolder *>()->holdedObj;
        pimplOwner->d->aniRunNext();
    }
};

// ============================================================
// ZFUIAnimatedImage
ZFOBJECT_REGISTER(ZFUIAnimatedImage)

ZFOBSERVER_EVENT_REGISTER(ZFUIAnimatedImage, AniOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFUIAnimatedImage, AniOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFUIAnimatedImage, AniOnRepeat)
ZFOBSERVER_EVENT_REGISTER(ZFUIAnimatedImage, AniOnUpdate)
ZFOBSERVER_EVENT_REGISTER(ZFUIAnimatedImage, AniFrameOnUpdate)

void ZFUIAnimatedImage::aniStart(void)
{
    if(d->aniRunning || this->aniFrameCount() == 0)
    {
        return ;
    }
    d->aniRunning = zftrue;
    this->aniFrames()->observerAdd(ZFContainer::EventContentOnChange(), d->aniFramesOnChangeListener, this->objectHolder());
    zfRetain(this);

    d->aniRepeatCountCurrent = 0;
    if(!this->aniKeepPreviousState())
    {
        d->aniFrameIndexCurrent = zfindexMax;
    }

    d->aniRunNext();
}
void ZFUIAnimatedImage::aniStop(void)
{
    if(!d->aniRunning)
    {
        return ;
    }
    d->aniRunning = zffalse;
    this->aniFrames()->observerRemove(ZFContainer::EventContentOnChange(), d->aniFramesOnChangeListener);
    d->aniFrameIndexLast = zfindexMax;

    zfidentity aniFrameTaskIdTmp = d->aniFrameTaskId;
    d->aniFrameTaskId = zfidentityInvalid;
    ZFThreadExecuteCancel(aniFrameTaskIdTmp);

    zfRelease(this);
}
zfbool ZFUIAnimatedImage::aniRunning(void)
{
    return d->aniRunning;
}

void ZFUIAnimatedImage::aniManual(ZF_IN zfindex aniFrame)
{
    this->aniStop();

    d->aniFrameIndexLast = d->aniFrameIndexCurrent;
    if(aniFrame < this->aniFrames()->count())
    {
        d->aniFrameIndexCurrent = aniFrame;
    }
    else
    {
        d->aniFrameIndexCurrent = 0;
    }
    if(d->aniFrameIndexCurrent >= this->aniFrames()->count())
    {
        d->aniFrameIndexCurrent = zfindexMax;
    }
    this->aniFrameOnUpdate();
    this->aniOnUpdate();
}

ZFUIImage *ZFUIAnimatedImage::aniFrameImageCurrent(void)
{
    if(d->aniFrameIndexCurrent < this->aniFrames()->count())
    {
        return this->aniFrameImageAtIndex(d->aniFrameIndexCurrent);
    }
    else
    {
        return zfnull;
    }
}
zftimet ZFUIAnimatedImage::aniFrameDurationCurrent(void)
{
    if(d->aniFrameIndexCurrent < this->aniFrames()->count())
    {
        return this->aniFrameDurationAtIndex(d->aniFrameIndexCurrent);
    }
    else
    {
        return zftimetZero;
    }
}
zftimet ZFUIAnimatedImage::aniFrameDurationFixedCurrent(void)
{
    if(d->aniFrameIndexCurrent < this->aniFrames()->count())
    {
        return this->aniFrameDurationFixedAtIndex(d->aniFrameIndexCurrent);
    }
    else
    {
        return zftimetZero;
    }
}
ZFUIAnimatedImageAniFrame *ZFUIAnimatedImage::aniFrameCurrent(void)
{
    if(d->aniFrameIndexCurrent < this->aniFrames()->count())
    {
        return this->aniFrameAtIndex(d->aniFrameIndexCurrent);
    }
    else
    {
        return zfnull;
    }
}
zfindex ZFUIAnimatedImage::aniFrameIndexCurrent(void)
{
    if(d->aniFrameIndexCurrent < this->aniFrames()->count())
    {
        return d->aniFrameIndexCurrent;
    }
    else
    {
        return zfindexMax;
    }
}

zfindex ZFUIAnimatedImage::aniRepeatCountCurrent(void)
{
    return d->aniRepeatCountCurrent;
}

ZFObject *ZFUIAnimatedImage::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIAnimatedImagePrivate);
    d->pimplOwner = this;
    return this;
}
void ZFUIAnimatedImage::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFUIAnimatedImage::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    zfstringAppend(ret, zfText(" %zi of %zi"), this->aniFrameIndexCurrent(), this->aniFrameCount());
    if(this->aniFrameCount() > 0)
    {
        for(zfindex i = 0; i < this->aniFrameCount(); ++i)
        {
            if(i == 0)
            {
                ret += zfText(" frames:");
            }
            zfstringAppend(ret, zfText(" %s"), zfsFromInt(this->aniFrameDurationFixedAtIndex(i)).cString());
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

