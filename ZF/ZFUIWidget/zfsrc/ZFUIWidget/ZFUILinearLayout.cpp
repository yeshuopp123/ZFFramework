/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUILinearLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUILinearLayout)

// ============================================================
// ZFUILinearLayoutParam
ZFOBJECT_REGISTER(ZFUILinearLayoutParam)

// ============================================================
// ZFUILinearLayout
ZFOBJECT_REGISTER(ZFUILinearLayout)

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUILinearLayout, ZFUIOrientationEnum, layoutOrientation)
{
    zfbool changed = (this->layoutOrientation() != newValue);
    this->layoutOrientationSetInternal(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUILinearLayout, ZFUIMargin, layoutChildMargin)
{
    zfbool changed = (this->layoutChildMargin() != newValue);
    this->layoutChildMarginSetInternal(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUILinearLayout, zfint, layoutChildSpace)
{
    zfbool changed = (this->layoutChildSpace() != newValue);
    this->layoutChildSpaceSetInternal(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}

// ============================================================
// override ZFUIView
static ZFUISize _ZFP_ZFUILinearLayout_measureHorizontal(ZF_IN ZFUILinearLayout *parent,
                                                        ZF_IN const ZFUISize &sizeHint,
                                                        ZF_IN const ZFUISizeParam &sizeParam);
static ZFUISize _ZFP_ZFUILinearLayout_measureVertical(ZF_IN ZFUILinearLayout *parent,
                                                      ZF_IN const ZFUISize &sizeHint,
                                                      ZF_IN const ZFUISizeParam &sizeParam);
void ZFUILinearLayout::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                       ZF_IN const ZFUISize &sizeHint,
                                       ZF_IN const ZFUISizeParam &sizeParam)
{
    switch(this->layoutOrientation())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            ret = _ZFP_ZFUILinearLayout_measureHorizontal(this, sizeHint, sizeParam);
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            ret = _ZFP_ZFUILinearLayout_measureVertical(this, sizeHint, sizeParam);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

static void _ZFP_ZFUILinearLayout_layoutHorizontal(ZF_IN ZFUILinearLayout *parent,
                                                   ZF_IN const ZFUISize &size,
                                                   ZF_IN zfbool positiveDirection);
static void _ZFP_ZFUILinearLayout_layoutVertical(ZF_IN ZFUILinearLayout *parent,
                                                 ZF_IN const ZFUISize &size,
                                                 ZF_IN zfbool positiveDirection);
void ZFUILinearLayout::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    switch(this->layoutOrientation())
    {
        case ZFUIOrientation::e_Left:
            _ZFP_ZFUILinearLayout_layoutHorizontal(this, bounds.size, zftrue);
            break;
        case ZFUIOrientation::e_Right:
            _ZFP_ZFUILinearLayout_layoutHorizontal(this, bounds.size, zffalse);
            break;
        case ZFUIOrientation::e_Top:
            _ZFP_ZFUILinearLayout_layoutVertical(this, bounds.size, zftrue);
            break;
        case ZFUIOrientation::e_Bottom:
            _ZFP_ZFUILinearLayout_layoutVertical(this, bounds.size, zffalse);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

// ============================================================
// measure
static ZFUISize _ZFP_ZFUILinearLayout_measureHorizontal(ZF_IN ZFUILinearLayout *parent,
                                                        ZF_IN const ZFUISize &sizeHint,
                                                        ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUISize ret = ZFUISizeZero;
    zfbool hasLayoutedChild = zffalse;
    zfint parentMarginX = ZFUIMarginGetX(parent->layoutChildMargin());
    zfint parentMarginY = ZFUIMarginGetY(parent->layoutChildMargin());
    zfint sizeHintLast = -1;
    do
    {
        ret.width = 0;
        sizeHintLast = -1;
        for(zfindex i = 0; i < parent->childCount(); ++i)
        {
            ZFUIView *child = parent->childAtIndex(i);
            ZFUILinearLayoutParam *layoutParam = child->layoutParamT();
            if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
            {
                continue ;
            }
            zfint prevSpace = (hasLayoutedChild ? parent->layoutChildSpace() : 0);
            hasLayoutedChild = zftrue;

            zfint marginX = parentMarginX + ZFUIMarginGetX(layoutParam->layoutMargin());
            zfint marginY = parentMarginY + ZFUIMarginGetY(layoutParam->layoutMargin());
            zfint sizeHintTmp = ZFUIViewLayoutParam::sizeHintMerge(
                layoutParam->sizeHint().height,
                ZFUIViewLayoutParam::sizeHintOffset(sizeHint.height, -marginY));
            if(sizeParam.height == ZFUISizeType::e_Wrap && layoutParam->sizeParam().height == ZFUISizeType::e_Fill)
            {
                child->layoutMeasure(
                    ZFUISizeMake(layoutParam->sizeHint().width, sizeHintTmp),
                    ZFUISizeParamWrapWidthWrapHeight);
                sizeHintTmp = child->layoutMeasuredSize().height;
                if(sizeHintTmp < ret.height)
                {
                    sizeHintTmp = ret.height;
                }
                if(sizeHintLast == -1)
                {
                    sizeHintLast = sizeHintTmp;
                }
            }
            child->layoutMeasure(
                ZFUISizeMake(layoutParam->sizeHint().width, sizeHintTmp),
                layoutParam->sizeParam());
            ret.width += prevSpace + child->layoutMeasuredSize().width + marginX;
            ret.height = zfmMax(ret.height, child->layoutMeasuredSize().height + marginY);
        }
    } while(sizeHintLast != -1 && sizeHintLast != ret.height);
    return ret;
}
static ZFUISize _ZFP_ZFUILinearLayout_measureVertical(ZF_IN ZFUILinearLayout *parent,
                                                      ZF_IN const ZFUISize &sizeHint,
                                                      ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUISize ret = ZFUISizeZero;
    zfbool hasLayoutedChild = zffalse;
    zfint parentMarginX = ZFUIMarginGetX(parent->layoutChildMargin());
    zfint parentMarginY = ZFUIMarginGetY(parent->layoutChildMargin());
    zfint sizeHintLast = -1;
    do
    {
        ret.height = 0;
        sizeHintLast = -1;
        for(zfindex i = 0; i < parent->childCount(); ++i)
        {
            ZFUIView *child = parent->childAtIndex(i);
            ZFUILinearLayoutParam *layoutParam = child->layoutParamT();
            if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
            {
                continue ;
            }
            zfint prevSpace = (hasLayoutedChild ? parent->layoutChildSpace() : 0);
            hasLayoutedChild = zftrue;

            zfint marginX = parentMarginX + ZFUIMarginGetX(layoutParam->layoutMargin());
            zfint marginY = parentMarginY + ZFUIMarginGetY(layoutParam->layoutMargin());
            zfint sizeHintTmp = ZFUIViewLayoutParam::sizeHintMerge(
                layoutParam->sizeHint().width,
                ZFUIViewLayoutParam::sizeHintOffset(sizeHint.width, -marginX));
            if(sizeParam.width == ZFUISizeType::e_Wrap && layoutParam->sizeParam().width == ZFUISizeType::e_Fill)
            {
                child->layoutMeasure(
                    ZFUISizeMake(sizeHintTmp, layoutParam->sizeHint().height),
                    ZFUISizeParamWrapWidthWrapHeight);
                sizeHintTmp = child->layoutMeasuredSize().width;
                if(sizeHintTmp < ret.width)
                {
                    sizeHintTmp = ret.width;
                }
                if(sizeHintLast == -1)
                {
                    sizeHintLast = sizeHintTmp;
                }
            }
            child->layoutMeasure(
                ZFUISizeMake(sizeHintTmp, layoutParam->sizeHint().height),
                layoutParam->sizeParam());
            ret.width = zfmMax(ret.width, child->layoutMeasuredSize().width + marginX);
            ret.height += prevSpace + child->layoutMeasuredSize().height + marginY;
        }
    } while(sizeHintLast != -1 && sizeHintLast != ret.width);
    return ret;
}

// ============================================================
// layout
static void _ZFP_ZFUILinearLayout_layoutHorizontal(ZF_IN ZFUILinearLayout *parent,
                                                   ZF_IN const ZFUISize &size,
                                                   ZF_IN zfbool positiveDirection)
{
    zfint requiredSize = 0;
    zfint totalWeight = 0;
    zfbool hasLayoutedChild = zffalse;
    zfint parentMarginX = ZFUIMarginGetX(parent->layoutChildMargin());
    zfint parentMarginY = ZFUIMarginGetY(parent->layoutChildMargin());
    for(zfindex i = 0; i < parent->childCount(); ++i)
    {
        ZFUIView *child = parent->childAtIndex(i);
        ZFUILinearLayoutParam *layoutParam = child->layoutParamT();
        if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
        {
            continue ;
        }
        zfint prevSpace = (hasLayoutedChild ? parent->layoutChildSpace() : 0);
        hasLayoutedChild = zftrue;

        zfint marginX = parentMarginX + ZFUIMarginGetX(layoutParam->layoutMargin());
        zfint marginY = parentMarginY + ZFUIMarginGetY(layoutParam->layoutMargin());
        if(layoutParam->layoutWeight() > 0)
        {
            totalWeight += layoutParam->layoutWeight();
        }
        else
        {
            switch(layoutParam->sizeParam().width)
            {
                case ZFUISizeType::e_Wrap:
                {
                    child->layoutMeasure(
                        ZFUISizeMake(
                            layoutParam->sizeHint().width,
                            ZFUIViewLayoutParam::sizeHintMerge(
                                layoutParam->sizeHint().height,
                                ZFUIViewLayoutParam::sizeHintOffset(size.height, -marginY))),
                        layoutParam->sizeParam());
                    requiredSize += child->layoutMeasuredSize().width;
                }
                    break;
                case ZFUISizeType::e_Fill:
                    requiredSize = size.width;
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
        }

        requiredSize += prevSpace + marginX;
    }
    hasLayoutedChild = zffalse;
    zfint flexibleSize = zfmMax(0, size.width - requiredSize);
    zfint offset = (positiveDirection ? 0 : size.width);
    for(zfindex i = 0; i < parent->childCount(); ++i)
    {
        ZFUIView *child = parent->childAtIndex(i);
        ZFUILinearLayoutParam *layoutParam = child->layoutParamT();
        if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
        {
            continue ;
        }
        zfint prevSpace = (hasLayoutedChild ? parent->layoutChildSpace() : 0);
        hasLayoutedChild = zftrue;

        zfint marginX = parentMarginX + ZFUIMarginGetX(layoutParam->layoutMargin());
        zfint marginY = parentMarginY + ZFUIMarginGetY(layoutParam->layoutMargin());
        zfint totalUsedSpace = prevSpace + marginX;
        if(layoutParam->layoutWeight() > 0)
        {
            child->layoutMeasure(
                ZFUISizeMake(
                    ZFUIViewLayoutParam::sizeHintMerge(flexibleSize * layoutParam->layoutWeight() / totalWeight, layoutParam->sizeHint().width),
                    ZFUIViewLayoutParam::sizeHintMerge(
                        layoutParam->sizeHint().height,
                        ZFUIViewLayoutParam::sizeHintOffset(size.height, -marginY))
                ),
                ZFUISizeParamMake(
                    ZFUISizeType::e_Fill,
                    layoutParam->sizeParam().height
                ));
            flexibleSize -= child->layoutMeasuredSize().width;
            totalWeight -= layoutParam->layoutWeight();
        }
        else
        {
            if(layoutParam->sizeParam().width == ZFUISizeType::e_Fill)
            {
                child->layoutMeasure(
                    ZFUISizeMake(
                        ZFUIViewLayoutParam::sizeHintMerge(
                            layoutParam->sizeHint().width,
                            positiveDirection ? zfmMax(size.width - offset - totalUsedSpace, 0) : zfmMax(offset - totalUsedSpace, 0)),
                        ZFUIViewLayoutParam::sizeHintMerge(
                            layoutParam->sizeHint().height,
                            ZFUIViewLayoutParam::sizeHintOffset(size.height, -marginY))
                    ),
                    layoutParam->sizeParam());
            }
        }
        if(positiveDirection)
        {
            child->layout(ZFUIAlignApply(
                layoutParam->layoutAlign(),
                ZFUIRectMake(offset + prevSpace, 0, child->layoutMeasuredSize().width + marginX, size.height),
                child->layoutMeasuredSize(),
                layoutParam->layoutMargin() + parent->layoutChildMargin()));
            offset += prevSpace + child->layoutMeasuredSize().width + marginX;
        }
        else
        {
            offset -= child->layoutMeasuredSize().width + marginX + prevSpace;
            child->layout(ZFUIAlignApply(
                layoutParam->layoutAlign(),
                ZFUIRectMake(offset, 0, child->layoutMeasuredSize().width + marginX, size.height),
                child->layoutMeasuredSize(),
                layoutParam->layoutMargin() + parent->layoutChildMargin()));
        }
    }
}
static void _ZFP_ZFUILinearLayout_layoutVertical(ZF_IN ZFUILinearLayout *parent,
                                                 ZF_IN const ZFUISize &size,
                                                 ZF_IN zfbool positiveDirection)
{
    zfint requiredSize = 0;
    zfint totalWeight = 0;
    zfbool hasLayoutedChild = zffalse;
    zfint parentMarginX = ZFUIMarginGetX(parent->layoutChildMargin());
    zfint parentMarginY = ZFUIMarginGetY(parent->layoutChildMargin());
    for(zfindex i = 0; i < parent->childCount(); ++i)
    {
        ZFUIView *child = parent->childAtIndex(i);
        ZFUILinearLayoutParam *layoutParam = child->layoutParamT();
        if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
        {
            continue ;
        }
        zfint prevSpace = (hasLayoutedChild ? parent->layoutChildSpace() : 0);
        hasLayoutedChild = zftrue;

        zfint marginX = parentMarginX + ZFUIMarginGetX(layoutParam->layoutMargin());
        zfint marginY = parentMarginY + ZFUIMarginGetY(layoutParam->layoutMargin());
        if(layoutParam->layoutWeight() > 0)
        {
            totalWeight += layoutParam->layoutWeight();
        }
        else
        {
            switch(layoutParam->sizeParam().height)
            {
                case ZFUISizeType::e_Wrap:
                {
                    child->layoutMeasure(
                        ZFUISizeMake(
                            ZFUIViewLayoutParam::sizeHintMerge(
                                layoutParam->sizeHint().width,
                                ZFUIViewLayoutParam::sizeHintOffset(size.width, -marginX)),
                            layoutParam->sizeHint().height),
                        layoutParam->sizeParam());
                    requiredSize += child->layoutMeasuredSize().height;
                }
                    break;
                case ZFUISizeType::e_Fill:
                    requiredSize = size.height;
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
        }

        requiredSize += prevSpace + marginY;
    }
    hasLayoutedChild = zffalse;
    zfint flexibleSize = zfmMax(0, size.height - requiredSize);
    zfint offset = (positiveDirection ? 0 : size.height);
    for(zfindex i = 0; i < parent->childCount(); ++i)
    {
        ZFUIView *child = parent->childAtIndex(i);
        ZFUILinearLayoutParam *layoutParam = child->layoutParamT();
        if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
        {
            continue ;
        }
        zfint prevSpace = (hasLayoutedChild ? parent->layoutChildSpace() : 0);
        hasLayoutedChild = zftrue;

        zfint marginX = parentMarginX + ZFUIMarginGetX(layoutParam->layoutMargin());
        zfint marginY = parentMarginY + ZFUIMarginGetY(layoutParam->layoutMargin());
        zfint totalUsedSpace = prevSpace + marginY;
        if(layoutParam->layoutWeight() > 0)
        {
            child->layoutMeasure(
                ZFUISizeMake(
                    ZFUIViewLayoutParam::sizeHintMerge(
                        layoutParam->sizeHint().width,
                        ZFUIViewLayoutParam::sizeHintOffset(size.width, -marginX)),
                    ZFUIViewLayoutParam::sizeHintMerge(flexibleSize * layoutParam->layoutWeight() / totalWeight, layoutParam->sizeHint().height)
                ),
                ZFUISizeParamMake(
                    layoutParam->sizeParam().width,
                    ZFUISizeType::e_Fill
                ));
            flexibleSize -= child->layoutMeasuredSize().height;
            totalWeight -= layoutParam->layoutWeight();
        }
        else
        {
            if(layoutParam->sizeParam().height == ZFUISizeType::e_Fill)
            {
                child->layoutMeasure(
                    ZFUISizeMake(
                        ZFUIViewLayoutParam::sizeHintMerge(
                            layoutParam->sizeHint().width,
                            ZFUIViewLayoutParam::sizeHintOffset(size.width, -marginX)),
                        ZFUIViewLayoutParam::sizeHintMerge(
                            layoutParam->sizeHint().height,
                            positiveDirection ? zfmMax(size.height - offset - totalUsedSpace, 0) : zfmMax(offset - totalUsedSpace, 0))
                    ),
                    layoutParam->sizeParam());
            }
        }
        if(positiveDirection)
        {
            child->layout(ZFUIAlignApply(
                layoutParam->layoutAlign(),
                ZFUIRectMake(0, offset + prevSpace, size.width, child->layoutMeasuredSize().height + marginY),
                child->layoutMeasuredSize(),
                layoutParam->layoutMargin() + parent->layoutChildMargin()));
            offset += prevSpace + child->layoutMeasuredSize().height + marginY;
        }
        else
        {
            offset -= child->layoutMeasuredSize().height + marginY + prevSpace;
            child->layout(ZFUIAlignApply(
                layoutParam->layoutAlign(),
                ZFUIRectMake(0, offset, size.width, child->layoutMeasuredSize().height + marginY),
                child->layoutMeasuredSize(),
                layoutParam->layoutMargin() + parent->layoutChildMargin()));
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

