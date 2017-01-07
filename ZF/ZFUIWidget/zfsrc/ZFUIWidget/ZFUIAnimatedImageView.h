/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIAnimatedImageView.h
 * @brief image view to hold #ZFUIAnimatedImage
 */

#ifndef _ZFI_ZFUIAnimatedImageView_h_
#define _ZFI_ZFUIAnimatedImageView_h_

#include "ZFUIAnimatedImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIAnimatedImageView
zfclassFwd _ZFP_ZFUIAnimatedImageViewPrivate;
/**
 * @brief image view to hold #ZFUIAnimatedImage
 */
zfclass ZF_ENV_EXPORT ZFUIAnimatedImageView : zfextends ZFUIImageView
{
    ZFOBJECT_DECLARE(ZFUIAnimatedImageView, ZFUIImageView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIAnimatedImageView)

public:
    /**
     * @brief the image
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUIAnimatedImage *, animatedImage, ZFPropertyInitValue(zflineAlloc(ZFUIAnimatedImage)))
    /**
     * @brief whether wrap size to each image frame's size, false by default
     *
     * -  true: this view would be measured each time the image frame updated,
     *   and this view's size would be measured to the current image frame's size
     * -  false: this view would not be measured when image frame updated,
     *   and this view's size would be measured to the max size of all image frame
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, animatedImageWrap, ZFPropertyInitValue(zffalse))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfbool, animatedImageWrap);

public:
    /** @brief see #ZFUIAnimatedImage::aniStart */
    virtual inline void aniStart(void)
    {
        this->animatedImage()->aniStart();
    }
    /** @brief see #ZFUIAnimatedImage::aniStop */
    virtual inline void aniStop(void)
    {
        this->animatedImage()->aniStop();
    }
    /** @brief see #ZFUIAnimatedImage::aniRunning */
    virtual inline zfbool aniRunning(void)
    {
        return this->animatedImage()->aniRunning();
    }

public:
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIAnimatedImageView_h_

