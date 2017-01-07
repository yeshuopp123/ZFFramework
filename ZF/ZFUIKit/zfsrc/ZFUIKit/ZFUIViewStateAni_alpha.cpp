/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIViewStateAni_alpha.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIViewStateAniSettingForAlpha)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFUIViewStateAniSettingForAlpha, ZFUIViewStateAniSettingForAlpha, ZFUIViewStateAniSettingForAlpha, instance, ZFLevelZFFrameworkNormal)

zfclass _ZFP_ZFUIViewStateAniImpl_alpha : zfextends ZFObject, zfimplements ZFUIViewStateAniImplProtocol
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIViewStateAniImpl_alpha, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIViewStateAniImplProtocol)

public:
    zfoverride
    virtual void stateAniViewAniPrepare(ZF_IN ZFUIView *view, ZF_IN_OUT ZFArrayEditable *aniList)
    {
        zfsuperI(ZFUIViewStateAniImplProtocol)::stateAniViewAniPrepare(view, aniList);
        if(!aniList->isEmpty()) {return ;}

        ZFUIViewStateAniSettingForAlpha *setting = ZFUIViewStateAniSettingForAlpha::instance();
        if(!setting->aniEnable() || !setting->aniFilter.filterCheckActive(view)) {return ;}

        const ZFUIRect &layoutedFrame = view->layoutedFrame();
        const ZFUIRect &layoutedFramePrev = view->layoutedFramePrev();
        if((layoutedFramePrev.size.width != 0 && layoutedFramePrev.size.height != 0)
            || layoutedFrame.size.width <= setting->aniToleranceX()
            || layoutedFrame.size.height <= setting->aniToleranceY())
        {
            return ;
        }

        zfblockedAlloc(ZFAnimationNativeView, ani);
        aniList->add(ani);
        ani->aniTargetSet(view);

        ani->aniDurationSet(setting->aniDuration());
        ani->aniCurveSet(setting->aniCurve());
        ani->aniAlphaFromSet(0);
    }
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniImpl_alpha_register, ZFLevelZFFrameworkNormal)
{
    this->impl = zfAllocWithoutLeakTest(_ZFP_ZFUIViewStateAniImpl_alpha);
    ZFUIViewStateAniImplRegister(this->impl);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniImpl_alpha_register)
{
    ZFUIViewStateAniImplUnregister(this->impl);
    zfReleaseWithoutLeakTest(this->impl);
}
public:
    ZFUIViewStateAniImplProtocol *impl;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniImpl_alpha_register)

ZF_NAMESPACE_GLOBAL_END

