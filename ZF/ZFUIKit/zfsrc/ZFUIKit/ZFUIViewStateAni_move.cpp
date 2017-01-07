/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIViewStateAni_move.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIViewStateAniSettingForMove)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFUIViewStateAniSettingForMove, ZFUIViewStateAniSettingForMove, ZFUIViewStateAniSettingForMove, instance, ZFLevelZFFrameworkNormal)

zfclass _ZFP_ZFUIViewStateAniImpl_move : zfextends ZFObject, zfimplements ZFUIViewStateAniImplProtocol
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIViewStateAniImpl_move, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIViewStateAniImplProtocol)

public:
    zfoverride
    virtual void stateAniViewAniPrepare(ZF_IN ZFUIView *view, ZF_IN_OUT ZFArrayEditable *aniList)
    {
        zfsuperI(ZFUIViewStateAniImplProtocol)::stateAniViewAniPrepare(view, aniList);
        if(!aniList->isEmpty()) {return ;}

        ZFUIViewStateAniSettingForMove *setting = ZFUIViewStateAniSettingForMove::instance();
        if(!setting->aniEnable() || !setting->aniFilter.filterCheckActive(view)) {return ;}

        const ZFUIRect &layoutedFramePrev = view->layoutedFramePrev();
        if(layoutedFramePrev.size.width == 0 || layoutedFramePrev.size.height == 0)
        {
            return ;
        }
        const ZFUIRect &layoutedFrame = view->layoutedFrame();
        zfint aniTolerance = setting->aniTolerance();
        if(zfmAbs(layoutedFrame.point.x - layoutedFramePrev.point.x) <= aniTolerance
                && zfmAbs(layoutedFrame.point.y - layoutedFramePrev.point.y) <= aniTolerance
                && zfmAbs(layoutedFrame.size.width - layoutedFramePrev.size.width) <= aniTolerance * 2
                && zfmAbs(layoutedFrame.size.height - layoutedFramePrev.size.height) <= aniTolerance * 2
            )
        {
            return ;
        }

        zfblockedAlloc(ZFAnimationNativeView, ani);
        aniList->add(ani);
        ani->aniTargetSet(view);

        ani->aniDurationSet(setting->aniDuration());
        ani->aniCurveSet(setting->aniCurve());

        if(zfmAbs(layoutedFrame.size.width - layoutedFramePrev.size.width) > aniTolerance * 2 && layoutedFrame.size.width > 0)
        {
            ani->aniScaleXFromSet((zffloat)layoutedFramePrev.size.width / layoutedFrame.size.width);
        }
        if(zfmAbs(layoutedFrame.size.height - layoutedFramePrev.size.height) > aniTolerance * 2 && layoutedFrame.size.height > 0)
        {
            ani->aniScaleYFromSet((zffloat)layoutedFramePrev.size.height / layoutedFrame.size.height);
        }

        ZFUIPoint from = ZFUIRectGetCenter(layoutedFramePrev);
        ZFUIPoint to = ZFUIRectGetCenter(layoutedFrame);
        if(zfmAbs(from.x - to.x) > aniTolerance)
        {
            ani->aniTranslatePixelXFromSet((zfint)((from.x - to.x) / ani->aniScaleXFrom()));
        }
        if(zfmAbs(from.y - to.y) > aniTolerance)
        {
            ani->aniTranslatePixelYFromSet((zfint)((from.y - to.y) / ani->aniScaleYFrom()));
        }
    }
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniImpl_move_register, ZFLevelZFFrameworkNormal)
{
    this->impl = zfAllocWithoutLeakTest(_ZFP_ZFUIViewStateAniImpl_move);
    ZFUIViewStateAniImplRegister(this->impl);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniImpl_move_register)
{
    ZFUIViewStateAniImplUnregister(this->impl);
    zfReleaseWithoutLeakTest(this->impl);
}
public:
    ZFUIViewStateAniImplProtocol *impl;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniImpl_move_register)

ZF_NAMESPACE_GLOBAL_END

