/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIAnimatedImage.h
 * @brief animated image container using multiple #ZFUIImage
 */

#ifndef _ZFI_ZFUIAnimatedImage_h_
#define _ZFI_ZFUIAnimatedImage_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIAnimatedImage
/**
 * @brief image frame data for #ZFUIAnimatedImage
 */
zfclass ZF_ENV_EXPORT ZFUIAnimatedImageAniFrame : zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFUIAnimatedImageAniFrame, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable, ZFCopyable)

public:
    /**
     * @brief image of the frame
     */
    ZFPROPERTY_RETAIN(ZFUIImage *, aniFrameImage)
    /**
     * @brief duration of this frame, -1 by default
     *
     * negative value means not specified, use #ZFUIAnimatedImage::aniFrameDurationDefault\n
     * 0 means disable this frame
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniFrameDuration, ZFPropertyInitValue(-1))
};

// ============================================================
// ZFUIAnimatedImage
zfclassFwd _ZFP_ZFUIAnimatedImagePrivate;
/**
 * @brief animated image container using multiple #ZFUIImage
 *
 * usage:
 * -  use #ZFUIAnimatedImage::aniFrameAdd to add each frame
 * -  add observer to #ZFUIAnimatedImage::EventAniOnUpdate
 * -  use #ZFUIAnimatedImage::aniFrameImageCurrent to access current image
 */
zfclass ZF_ENV_EXPORT ZFUIAnimatedImage : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUIAnimatedImage, ZFStyleableObject)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIAnimatedImage)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when ani started
     */
    ZFOBSERVER_EVENT(AniOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when ani stopped
     */
    ZFOBSERVER_EVENT(AniOnStop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when last frame finished
     */
    ZFOBSERVER_EVENT(AniOnRepeat)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called each time the frame changed
     */
    ZFOBSERVER_EVENT(AniOnUpdate)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to update the frame
     */
    ZFOBSERVER_EVENT(AniFrameOnUpdate)

public:
    /**
     * @brief default duration between each frame, 10 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniFrameDurationDefault,
                                ZFPropertyInitValue(10))

    /**
     * @brief whether keep previous state when start animating, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, aniKeepPreviousState,
                                ZFPropertyInitValue(zftrue))

    /**
     * @brief number of times to repeat, zfindexMax by default
     *
     * -  0 means no repeat, run once and stop
     * -  zfindexMax means repeat until stop manually
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, aniRepeatCount,
                                ZFPropertyInitValue(zfindexMax))

    /**
     * @brief ani frames, holds #ZFUIAnimatedImageAniFrame
     */
    ZFPROPERTY_RETAIN_DETAIL(ZFArrayEditable *, aniFrames,
                             ZFPropertyInitValue(zflineAlloc(ZFArrayEditable)),
                             protected, protected)

public:
    /** @brief see #ZFUIAnimatedImageAniFrame */
    virtual inline zfindex aniFrameCount(void)
    {
        return this->aniFrames()->count();
    }
    /** @brief see #ZFUIAnimatedImageAniFrame */
    virtual inline void aniFrameAdd(ZF_IN ZFUIImage *aniFrameImage,
                                    ZF_IN_OPT zftimet aniFrameDuration = (zftimet)-1)
    {
        zfCoreAssert(aniFrameImage != zfnull);
        zfblockedAlloc(ZFUIAnimatedImageAniFrame, aniFrame);
        aniFrame->aniFrameImageSet(aniFrameImage);
        aniFrame->aniFrameDurationSet(aniFrameDuration);
        this->aniFrames()->add(aniFrame);
    }
    /** @brief see #ZFUIAnimatedImageAniFrame */
    virtual inline void aniFrameRemoveAtIndex(ZF_IN zfindex index)
    {
        this->aniFrames()->remove(index);
    }
    /** @brief see #ZFUIAnimatedImageAniFrame */
    virtual inline ZFUIAnimatedImageAniFrame *aniFrameAtIndex(ZF_IN zfindex index)
    {
        return this->aniFrames()->get<ZFUIAnimatedImageAniFrame *>(index);
    }
    /** @brief see #ZFUIAnimatedImageAniFrame */
    virtual inline ZFUIImage *aniFrameImageAtIndex(ZF_IN zfindex index)
    {
        return this->aniFrames()->get<ZFUIAnimatedImageAniFrame *>(index)->aniFrameImage();
    }
    /** @brief see #ZFUIAnimatedImageAniFrame */
    virtual inline zftimet aniFrameDurationAtIndex(ZF_IN zfindex index)
    {
        return this->aniFrames()->get<ZFUIAnimatedImageAniFrame *>(index)->aniFrameDuration();
    }
    /** @brief see #ZFUIAnimatedImageAniFrame */
    virtual zftimet aniFrameDurationFixedAtIndex(ZF_IN zfindex index)
    {
        zftimet ret = this->aniFrames()->get<ZFUIAnimatedImageAniFrame *>(index)->aniFrameDuration();
        if(ret < 0)
        {
            ret = this->aniFrameDurationDefault();
        }
        return ret;
    }

public:
    /**
     * @brief start animation
     */
    zffinal void aniStart(void);
    /**
     * @brief stop animation
     */
    zffinal void aniStop(void);
    /**
     * @brief true if running
     */
    zffinal zfbool aniRunning(void);

public:
    /**
     * @brief manually update frame to specified index,
     *   reset to 0 frame if index out of range
     *
     * note calling this method would cause #aniStop being called first,
     * #EventAniFrameOnUpdate and #EventAniOnUpdate would be notified,
     * other event won't be notified
     */
    zffinal void aniManual(ZF_IN zfindex aniFrame);
    /**
     * @brief see #aniManual
     */
    zffinal inline void aniManualPrev(void)
    {
        this->aniManual((this->aniFrameIndexCurrent() > 0)
            ? (this->aniFrameIndexCurrent() - 1)
            : (this->aniFrameCount() - 1));
    }
    /**
     * @brief see #aniManual
     */
    zffinal inline void aniManualNext(void)
    {
        this->aniManual(this->aniFrameIndexCurrent() + 1);
    }

public:
    /**
     * @brief see #aniFrameIndexCurrent
     */
    virtual ZFUIImage *aniFrameImageCurrent(void);
    /**
     * @brief see #aniFrameIndexCurrent
     */
    virtual zftimet aniFrameDurationCurrent(void);
    /**
     * @brief see #aniFrameIndexCurrent
     */
    virtual zftimet aniFrameDurationFixedCurrent(void);
    /**
     * @brief see #aniFrameIndexCurrent
     */
    virtual ZFUIAnimatedImageAniFrame *aniFrameCurrent(void);
    /**
     * @brief current ani frame index
     *
     * may be zfindexMax if not started
     */
    virtual zfindex aniFrameIndexCurrent(void);

public:
    /**
     * @brief repeat count since start, 0 for first time
     */
    virtual zfindex aniRepeatCountCurrent(void);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

protected:
    /** @brief see #EventAniOnStart */
    virtual inline void aniOnStart(void)
    {
        this->observerNotify(ZFUIAnimatedImage::EventAniOnStart());
    }
    /** @brief see #EventAniOnStop */
    virtual inline void aniOnStop(void)
    {
        this->observerNotify(ZFUIAnimatedImage::EventAniOnStop());
    }
    /** @brief see #EventAniOnRepeat */
    virtual inline void aniOnRepeat(void)
    {
        this->observerNotify(ZFUIAnimatedImage::EventAniOnRepeat());
    }
    /** @brief see #EventAniOnUpdate */
    virtual inline void aniOnUpdate(void)
    {
        this->observerNotify(ZFUIAnimatedImage::EventAniOnUpdate());
    }
    /** @brief see #EventAniFrameOnUpdate */
    virtual inline void aniFrameOnUpdate(void)
    {
        this->observerNotify(ZFUIAnimatedImage::EventAniFrameOnUpdate());
    }

private:
    _ZFP_ZFUIAnimatedImagePrivate *d;
    friend zfclassFwd _ZFP_ZFUIAnimatedImagePrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIAnimatedImage_h_

