/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIAnimatedImageView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIAnimatedImageView)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIAnimatedImageViewDataHolder, ZFLevelZFFrameworkNormal)
{
    this->animatedImageOnUpdateListener = ZFCallbackForRawFunction(zfself::animatedImageOnUpdate);
}
public:
    ZFListener animatedImageOnUpdateListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(animatedImageOnUpdate)
    {
        ZFUIAnimatedImageView *view = userData->to<ZFObjectHolder *>()->holdedObj;
        if(view->animatedImageWrapToImage())
        {
            view->imageContentSet(view->animatedImage()->aniFrameImageCurrent());
        }
        else
        {
            view->layoutRequestOverrideSet(zftrue);
            view->imageContentSet(view->animatedImage()->aniFrameImageCurrent());
            view->layoutRequestOverrideSet(zffalse);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIAnimatedImageViewDataHolder)

// ============================================================
// ZFUIAnimatedImageView
ZFOBJECT_REGISTER(ZFUIAnimatedImageView)

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIAnimatedImageView, zfbool, animatedImageWrapToImage)
{
    this->animatedImageWrapToImageSetInternal(newValue);
    this->layoutRequest();
}

void ZFUIAnimatedImageView::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    this->animatedImage()->observerAdd(ZFUIAnimatedImage::EventAniFrameOnUpdate(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIAnimatedImageViewDataHolder)->animatedImageOnUpdateListener,
        this->objectHolder());
}
void ZFUIAnimatedImageView::objectOnDeallocPrepare(void)
{
    this->animatedImage()->observerRemove(ZFUIAnimatedImage::EventAniFrameOnUpdate(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIAnimatedImageViewDataHolder)->animatedImageOnUpdateListener);
    this->aniStop();
    zfsuper::objectOnDeallocPrepare();
}

void ZFUIAnimatedImageView::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayoutPrepare(bounds);
    if(this->imageContent() == zfnull && this->animatedImage()->aniFrameCount() > 0)
    {
        this->animatedImage()->aniManualNext();
    }
}
void ZFUIAnimatedImageView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                            ZF_IN const ZFUISize &sizeHint,
                                            ZF_IN const ZFUISizeParam &sizeParam)
{
    if(this->animatedImageWrapToImage())
    {
        zfsuper::layoutOnMeasure(ret, sizeHint, sizeParam);
    }
    else
    {
        ret = ZFUISizeZero;
        ZFUIAnimatedImage *animatedImage = this->animatedImage();
        for(zfindex i = animatedImage->aniFrameCount() - 1; i != zfindexMax; --i)
        {
            ZFUIImage *aniFrame = animatedImage->aniFrameImageAtIndex(i);
            ret.width = zfmMax(ret.width, aniFrame->imageSize().width);
            ret.height = zfmMax(ret.height, aniFrame->imageSize().height);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

