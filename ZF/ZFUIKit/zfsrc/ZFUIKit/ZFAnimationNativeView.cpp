/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFAnimationNativeView.h"
#include "protocol/ZFProtocolZFAnimationNativeView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFAnimationNativeViewCurve)

// ============================================================
zfclassNotPOD _ZFP_ZFAnimationNativeViewPrivate
{
public:
    void *nativeAni;
    zfbool aniTargetAutoDisableCached;
    zfbool aniTargetEnableSaved;

public:
    _ZFP_ZFAnimationNativeViewPrivate(void)
    : nativeAni(zfnull)
    , aniTargetAutoDisableCached(zffalse)
    , aniTargetEnableSaved(zffalse)
    {
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAnimationNativeView)

ZFObject *ZFAnimationNativeView::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAnimationNativeViewPrivate);
    d->nativeAni = ZFPROTOCOL_ACCESS(ZFAnimationNativeView)->nativeAniCreate(this);
    return this;
}
void ZFAnimationNativeView::objectOnDealloc(void)
{
    ZFPROTOCOL_ACCESS(ZFAnimationNativeView)->nativeAniDestroy(this, d->nativeAni);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFAnimationNativeView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ret += zfText("-");
    zfsFromPointerT(ret, this->nativeAnimation());
    ZFClassUtil::objectPropertyInfo(ret, this);
}
zfidentity ZFAnimationNativeView::objectHash(void)
{
    // no need to hash everything, for performance
    return zfidentityHash(zfsuper::objectHash()
            , (zfidentity)this->aniCurve()
            , (zfidentity)this->aniAlphaTo()
            , (zfidentity)this->aniScaleXTo()
            , (zfidentity)this->aniScaleYTo()
            , (zfidentity)this->aniTranslateXTo()
            , (zfidentity)this->aniTranslateYTo()
            , (zfidentity)this->aniTranslatePixelXTo()
            , (zfidentity)this->aniTranslatePixelYTo()
            , (zfidentity)this->aniRotateXTo()
            , (zfidentity)this->aniRotateYTo()
            , (zfidentity)this->aniRotateZTo()
        );
}
ZFCompareResult ZFAnimationNativeView::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(zfsuper::ZFObject::objectCompare(anotherObj) != ZFCompareTheSame)
    {
        return ZFCompareUncomparable;
    }
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(zftrue
        && this->aniCurve() == another->aniCurve()
        && this->aniAlphaFrom() == another->aniAlphaFrom()
        && this->aniAlphaTo() == another->aniAlphaTo()
        && this->aniScaleXFrom() == another->aniScaleXFrom()
        && this->aniScaleXTo() == another->aniScaleXTo()
        && this->aniScaleYFrom() == another->aniScaleYFrom()
        && this->aniScaleYTo() == another->aniScaleYTo()
        && this->aniTranslateXFrom() == another->aniTranslateXFrom()
        && this->aniTranslateXTo() == another->aniTranslateXTo()
        && this->aniTranslateYFrom() == another->aniTranslateYFrom()
        && this->aniTranslateYTo() == another->aniTranslateYTo()
        && this->aniTranslatePixelXFrom() == another->aniTranslatePixelXFrom()
        && this->aniTranslatePixelXTo() == another->aniTranslatePixelXTo()
        && this->aniTranslatePixelYFrom() == another->aniTranslatePixelYFrom()
        && this->aniTranslatePixelYTo() == another->aniTranslatePixelYTo()
        && this->aniRotateXFrom() == another->aniRotateXFrom()
        && this->aniRotateXTo() == another->aniRotateXTo()
        && this->aniRotateYFrom() == another->aniRotateYFrom()
        && this->aniRotateYTo() == another->aniRotateYTo()
        && this->aniRotateZFrom() == another->aniRotateZFrom()
        && this->aniRotateZTo() == another->aniRotateZTo()
        )
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

void *ZFAnimationNativeView::nativeAnimation(void)
{
    return d->nativeAni;
}

// ============================================================
// start stop
zfbool ZFAnimationNativeView::aniImplCheckValid(void)
{
    return (zfsuper::aniImplCheckValid()
        && this->aniTarget() != zfnull && this->aniTarget()->classData()->classIsTypeOf(ZFUIView::ClassData())
    );
}

void ZFAnimationNativeView::aniOnStart(void)
{
    zfsuper::aniOnStart();
    ZFUIView *aniTarget = ZFAny(this->aniTarget());
    if(aniTarget == zfnull)
    {
        d->aniTargetAutoDisableCached = zffalse;
    }
    else
    {
        d->aniTargetAutoDisableCached = this->aniTargetAutoDisable();
        if(d->aniTargetAutoDisableCached)
        {
            d->aniTargetEnableSaved = aniTarget->viewUIEnableTree();
            aniTarget->viewUIEnableTreeSet(zffalse);
        }
    }
}
void ZFAnimationNativeView::aniOnStop(void)
{
    ZFUIView *aniTarget = ZFAny(this->aniTarget());
    if(aniTarget != zfnull && d->aniTargetAutoDisableCached)
    {
        aniTarget->viewUIEnableTreeSet(d->aniTargetAutoDisableCached);
    }
    zfsuper::aniOnStop();
}

void ZFAnimationNativeView::aniImplStart(void)
{
    zfsuper::aniImplStart();
    ZFUIView *aniTarget = ZFAny(this->aniTarget());
    {
        ZFUIView *parent = aniTarget;
        while(parent->viewParent() != zfnull)
        {
            parent = parent->viewParent();
        }
        parent->layoutIfNeed();
    }
    ZFPROTOCOL_ACCESS(ZFAnimationNativeView)->nativeAniStart(this,
        aniTarget != zfnull ? aniTarget->scaleGetFixed() : 1.0f);
}
void ZFAnimationNativeView::aniImplStop(void)
{
    ZFPROTOCOL_ACCESS(ZFAnimationNativeView)->nativeAniStop(this);
    zfsuper::aniImplStop();
}

ZF_NAMESPACE_GLOBAL_END

