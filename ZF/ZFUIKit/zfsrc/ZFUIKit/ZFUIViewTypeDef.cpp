/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIViewTypeDef.h"
#include "ZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIViewChildLayer)

ZFOBJECT_REGISTER(ZFUIViewMeasureResult)

// ============================================================
// ZFUIViewLayoutParam
ZFOBJECT_REGISTER(ZFUIViewLayoutParam)
void ZFUIViewLayoutParam::layoutParamApply(ZF_OUT ZFUIRect &ret,
                                           ZF_IN const ZFUIRect &rect,
                                           ZF_IN ZFUIView *child,
                                           ZF_IN ZFUIViewLayoutParam *lp)
{
    ZFUISize refSizeTmp = ZFUIRectApplyMargin(rect, lp->layoutMargin()).size;
    if(refSizeTmp.width < 0)
    {
        refSizeTmp.width = 0;
    }
    if(refSizeTmp.height < 0)
    {
        refSizeTmp.height = 0;
    }

    if(lp->sizeHint().width >= 0)
    {
        refSizeTmp.width = zfmMin(lp->sizeHint().width, refSizeTmp.width);
    }
    if(lp->sizeHint().height >= 0)
    {
        refSizeTmp.height = zfmMin(lp->sizeHint().height, refSizeTmp.height);
    }
    child->layoutMeasure(refSizeTmp, lp->sizeParam());
    ZFUIAlignApply(ret, lp->layoutAlign(), rect, child->layoutMeasuredSize(), lp->layoutMargin());
}

void ZFUIViewLayoutParam::sizeHintApply(ZF_OUT zfint &ret,
                                        ZF_IN zfint size,
                                        ZF_IN zfint sizeHint,
                                        ZF_IN ZFUISizeTypeEnum sizeParam)
{
    ret = size;
    switch(sizeParam)
    {
        case ZFUISizeType::e_Wrap:
            if(sizeHint >= 0 && size > sizeHint)
            {
                ret = sizeHint;
            }
            break;
        case ZFUISizeType::e_Fill:
            if(sizeHint >= 0)
            {
                ret = sizeHint;
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}
void ZFUIViewLayoutParam::sizeHintApply(ZF_OUT ZFUISize &ret,
                                        ZF_IN const ZFUISize &size,
                                        ZF_IN const ZFUISize &sizeHint,
                                        ZF_IN const ZFUISizeParam &sizeParam)
{
    ret.width = ZFUIViewLayoutParam::sizeHintApply(size.width, sizeHint.width, sizeParam.width);
    ret.height = ZFUIViewLayoutParam::sizeHintApply(size.height, sizeHint.height, sizeParam.height);
}
void ZFUIViewLayoutParam::sizeHintMerge(ZF_OUT zfint &ret,
                                        ZF_IN zfint sizeHint0,
                                        ZF_IN zfint sizeHint1)
{
    if(sizeHint0 < 0 && sizeHint1 < 0)
    {
        ret = -1;
    }
    else if(sizeHint0 >= 0 && sizeHint1 >= 0)
    {
        ret = zfmMin(sizeHint0, sizeHint1);
    }
    else
    {
        ret = zfmMax(sizeHint0, sizeHint1);
    }
}
void ZFUIViewLayoutParam::sizeHintMerge(ZF_OUT ZFUISize &ret,
                                        ZF_IN const ZFUISize &sizeHint0,
                                        ZF_IN const ZFUISize &sizeHint1)
{
    ret.width = ZFUIViewLayoutParam::sizeHintMerge(sizeHint0.width, sizeHint1.width);
    ret.height = ZFUIViewLayoutParam::sizeHintMerge(sizeHint0.height, sizeHint1.height);
}
void ZFUIViewLayoutParam::sizeHintOffset(ZF_OUT zfint &ret,
                                         ZF_IN zfint sizeHint,
                                         ZF_IN zfint offset)
{
    if(offset >= 0)
    {
        ret = ((sizeHint >= 0) ? sizeHint + offset : -1);
    }
    else
    {
        ret = ((sizeHint >= 0) ? zfmMax(0, sizeHint + offset) : -1);
    }
}
void ZFUIViewLayoutParam::sizeHintOffset(ZF_OUT ZFUISize &ret,
                                         ZF_IN const ZFUISize &sizeHint,
                                         ZF_IN const ZFUISize &offset)
{
    ret.width = ZFUIViewLayoutParam::sizeHintOffset(sizeHint.width, offset.width);
    ret.height = ZFUIViewLayoutParam::sizeHintOffset(sizeHint.height, offset.height);
}
void ZFUIViewLayoutParam::sizeHintOffset(ZF_OUT ZFUISize &ret,
                                         ZF_IN const ZFUISize &sizeHint,
                                         ZF_IN zfint offset)
{
    ret.width = ZFUIViewLayoutParam::sizeHintOffset(sizeHint.width, offset);
    ret.height = ZFUIViewLayoutParam::sizeHintOffset(sizeHint.height, offset);
}

ZF_NAMESPACE_GLOBAL_END

