/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIFlowLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIFlowLayout)

// ============================================================
// ZFUIFlowLayoutParam
ZFOBJECT_REGISTER(ZFUIFlowLayoutParam)

// ============================================================
// ZFUIFlowLayout
ZFOBJECT_REGISTER(ZFUIFlowLayout)

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIFlowLayout, ZFUIOrientationEnum, layoutOrientationMain)
{
    zfbool changed = (this->layoutOrientationMain() != newValue);
    this->layoutOrientationMainSetInternal(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIFlowLayout, ZFUIOrientationEnum, layoutOrientationSecondary)
{
    zfbool changed = (this->layoutOrientationSecondary() != newValue);
    this->layoutOrientationSecondarySetInternal(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIFlowLayout, ZFUIMargin, layoutChildMargin)
{
    zfbool changed = (this->layoutChildMargin() != newValue);
    this->layoutChildMarginSetInternal(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIFlowLayout, zfint, layoutChildSpaceX)
{
    zfbool changed = (this->layoutChildSpaceX() != newValue);
    this->layoutChildSpaceXSetInternal(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIFlowLayout, zfint, layoutChildSpaceY)
{
    zfbool changed = (this->layoutChildSpaceY() != newValue);
    this->layoutChildSpaceYSetInternal(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}

// ============================================================
// override ZFUIView
static ZFUISize _ZFP_ZFUIFlowLayout_measureHorizontal(ZF_IN ZFUIFlowLayout *parent,
                                                      ZF_IN const ZFUISize &sizeHint,
                                                      ZF_IN const ZFUISizeParam &sizeParam);
static ZFUISize _ZFP_ZFUIFlowLayout_measureVertical(ZF_IN ZFUIFlowLayout *parent,
                                                    ZF_IN const ZFUISize &sizeHint,
                                                    ZF_IN const ZFUISizeParam &sizeParam);
void ZFUIFlowLayout::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                     ZF_IN const ZFUISize &sizeHint,
                                     ZF_IN const ZFUISizeParam &sizeParam)
{
    switch(this->layoutOrientationMain())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            ret = _ZFP_ZFUIFlowLayout_measureHorizontal(this, sizeHint, sizeParam);
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            ret = _ZFP_ZFUIFlowLayout_measureVertical(this, sizeHint, sizeParam);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

static void _ZFP_ZFUIFlowLayout_layoutHorizontal(ZF_IN ZFUIFlowLayout *parent,
                                                 ZF_IN const ZFUISize &size,
                                                 ZF_IN zfbool positiveDirection);
static void _ZFP_ZFUIFlowLayout_layoutVertical(ZF_IN ZFUIFlowLayout *parent,
                                               ZF_IN const ZFUISize &size,
                                               ZF_IN zfbool positiveDirection);
void ZFUIFlowLayout::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    switch(this->layoutOrientationMain())
    {
        case ZFUIOrientation::e_Left:
            _ZFP_ZFUIFlowLayout_layoutHorizontal(this, bounds.size, zftrue);
            break;
        case ZFUIOrientation::e_Right:
            _ZFP_ZFUIFlowLayout_layoutHorizontal(this, bounds.size, zffalse);
            break;
        case ZFUIOrientation::e_Top:
            _ZFP_ZFUIFlowLayout_layoutVertical(this, bounds.size, zftrue);
            break;
        case ZFUIOrientation::e_Bottom:
            _ZFP_ZFUIFlowLayout_layoutVertical(this, bounds.size, zffalse);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

// ============================================================
// util
ZFUIOrientationEnum _ZFP_ZFUIFlowLayout_layoutOrientationSecondary(ZF_IN ZFUIOrientationEnum layoutOrientationMain,
                                                                   ZF_IN ZFUIOrientationEnum layoutOrientationSecondary)
{
    switch(layoutOrientationMain)
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            switch(layoutOrientationSecondary)
            {
                case ZFUIOrientation::e_Left:
                case ZFUIOrientation::e_Right:
                    return ZFUIOrientation::e_Top;
                case ZFUIOrientation::e_Top:
                case ZFUIOrientation::e_Bottom:
                    return layoutOrientationSecondary;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ZFUIOrientation::e_Left;
            }
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            switch(layoutOrientationSecondary)
            {
                case ZFUIOrientation::e_Left:
                case ZFUIOrientation::e_Right:
                    return layoutOrientationSecondary;
                case ZFUIOrientation::e_Top:
                case ZFUIOrientation::e_Bottom:
                    return ZFUIOrientation::e_Left;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ZFUIOrientation::e_Left;
            }
        default:
            zfCoreCriticalShouldNotGoHere();
            return ZFUIOrientation::e_Left;
    }
}

// ============================================================
// measure
static ZFUISize _ZFP_ZFUIFlowLayout_measureHorizontal(ZF_IN ZFUIFlowLayout *parent,
                                                      ZF_IN const ZFUISize &sizeHint,
                                                      ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUISize ret = ZFUISizeZero;
    zfindex wrapIndex = 0;
    zfint lineSizeUsed = 0;
    while(wrapIndex < parent->childCount()) // for each line
    {
        ZFUISize lineSize = ZFUISizeZero;
        zfbool hasLayoutedChild = zffalse;
        zfint parentMarginX = ZFUIMarginGetX(parent->layoutChildMargin());
        zfint parentMarginY = ZFUIMarginGetY(parent->layoutChildMargin());
        zfint sizeHintLast = -1;
        zfindex wrapIndexTmp = -1;
        zfint prevLineSpace = (wrapIndex > 0 ? parent->layoutChildSpaceY() : 0);
        do
        {
            lineSize.width = 0;
            sizeHintLast = -1;
            wrapIndexTmp = parent->childCount();
            for(zfindex i = wrapIndex; i < parent->childCount(); ++i)
            {
                ZFUIView *child = parent->childAtIndex(i);
                ZFUIFlowLayoutParam *layoutParam = child->layoutParamT();
                if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
                {
                    continue ;
                }
                zfint prevSpace = (hasLayoutedChild ? parent->layoutChildSpaceX() : 0);
                hasLayoutedChild = zftrue;

                zfint marginX = parentMarginX + ZFUIMarginGetX(layoutParam->layoutMargin());
                zfint marginY = parentMarginY + ZFUIMarginGetY(layoutParam->layoutMargin());
                zfint sizeHintTmp = ZFUIViewLayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().height,
                    ZFUIViewLayoutParam::sizeHintOffset(sizeHint.height, 0 - lineSizeUsed - marginY - prevLineSpace));
                if(sizeParam.height == ZFUISizeType::e_Wrap && layoutParam->sizeParam().height == ZFUISizeType::e_Fill)
                {
                    child->layoutMeasure(
                        ZFUISizeMake(layoutParam->sizeHint().width, sizeHintTmp),
                        ZFUISizeParamWrapWidthWrapHeight);
                    sizeHintTmp = child->layoutMeasuredSize().height;
                    if(sizeHintTmp < lineSize.height)
                    {
                        sizeHintTmp = lineSize.height;
                    }
                    if(sizeHintLast == -1)
                    {
                        sizeHintLast = sizeHintTmp;
                    }
                }
                child->layoutMeasure(
                    ZFUISizeMake(layoutParam->sizeHint().width, sizeHintTmp),
                    layoutParam->sizeParam());
                if(i > wrapIndex && sizeHint.width >= 0 && lineSize.width + prevSpace + child->layoutMeasuredSize().width + marginX > sizeHint.width)
                {
                    wrapIndexTmp = i;
                    break;
                }
                lineSize.width += prevSpace + child->layoutMeasuredSize().width + marginX;
                lineSize.height = zfmMax(lineSize.height, child->layoutMeasuredSize().height + marginY + prevLineSpace);

                if(layoutParam->sizeParam().width == ZFUISizeType::e_Fill
                    || (sizeHint.width >= 0 && lineSize.width == sizeHint.width))
                {
                    wrapIndexTmp = i + 1;
                    break;
                }
            }
        } while(sizeHintLast != -1 && sizeHintLast != lineSize.height);
        lineSizeUsed += lineSize.height;
        wrapIndex = wrapIndexTmp;
        ret.width = zfmMax(ret.width, lineSize.width);
        ret.height += lineSize.height;
    } // for each line
    return ret;
}
static ZFUISize _ZFP_ZFUIFlowLayout_measureVertical(ZF_IN ZFUIFlowLayout *parent,
                                                    ZF_IN const ZFUISize &sizeHint,
                                                    ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUISize ret = ZFUISizeZero;
    zfindex wrapIndex = 0;
    zfint lineSizeUsed = 0;
    while(wrapIndex < parent->childCount()) // for each line
    {
        ZFUISize lineSize = ZFUISizeZero;
        zfbool hasLayoutedChild = zffalse;
        zfint parentMarginX = ZFUIMarginGetX(parent->layoutChildMargin());
        zfint parentMarginY = ZFUIMarginGetY(parent->layoutChildMargin());
        zfint sizeHintLast = -1;
        zfindex wrapIndexTmp = -1;
        zfint prevLineSpace = (wrapIndex ? parent->layoutChildSpaceX() : 0);
        do
        {
            lineSize.height = 0;
            sizeHintLast = -1;
            wrapIndexTmp = parent->childCount();
            for(zfindex i = wrapIndex; i < parent->childCount(); ++i)
            {
                ZFUIView *child = parent->childAtIndex(i);
                ZFUIFlowLayoutParam *layoutParam = child->layoutParamT();
                if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
                {
                    continue ;
                }
                zfint prevSpace = (hasLayoutedChild ? parent->layoutChildSpaceY() : 0);
                hasLayoutedChild = zftrue;

                zfint marginX = parentMarginX + ZFUIMarginGetX(layoutParam->layoutMargin());
                zfint marginY = parentMarginY + ZFUIMarginGetY(layoutParam->layoutMargin());
                zfint sizeHintTmp = ZFUIViewLayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().width,
                    ZFUIViewLayoutParam::sizeHintOffset(sizeHint.width, 0 - lineSizeUsed - marginX - prevLineSpace));
                if(sizeParam.width == ZFUISizeType::e_Wrap && layoutParam->sizeParam().width == ZFUISizeType::e_Fill)
                {
                    child->layoutMeasure(
                        ZFUISizeMake(sizeHintTmp, layoutParam->sizeHint().height),
                        ZFUISizeParamWrapWidthWrapHeight);
                    sizeHintTmp = child->layoutMeasuredSize().width;
                    if(sizeHintTmp < lineSize.width)
                    {
                        sizeHintTmp = lineSize.width;
                    }
                    if(sizeHintLast == -1)
                    {
                        sizeHintLast = sizeHintTmp;
                    }
                }
                child->layoutMeasure(
                    ZFUISizeMake(sizeHintTmp, layoutParam->sizeHint().height),
                    layoutParam->sizeParam());
                if(i > wrapIndex && sizeHint.height >= 0 && lineSize.height + prevSpace + child->layoutMeasuredSize().height + marginY > sizeHint.height)
                {
                    wrapIndexTmp = i;
                    break;
                }
                lineSize.height += prevSpace + child->layoutMeasuredSize().height + marginY;
                lineSize.width = zfmMax(lineSize.width, child->layoutMeasuredSize().width + marginX + prevLineSpace);

                if(layoutParam->sizeParam().height == ZFUISizeType::e_Fill
                    || (sizeHint.height >= 0 && lineSize.height == sizeHint.height))
                {
                    wrapIndexTmp = i + 1;
                    break;
                }
            }
        } while(sizeHintLast != -1 && sizeHintLast != lineSize.width);
        lineSizeUsed += lineSize.width;
        wrapIndex = wrapIndexTmp;
        ret.height = zfmMax(ret.height, lineSize.height);
        ret.width += lineSize.width;
    } // for each line
    return ret;
}

// ============================================================
// layout
static void _ZFP_ZFUIFlowLayout_layoutHorizontal(ZF_IN ZFUIFlowLayout *parent,
                                                 ZF_IN const ZFUISize &size,
                                                 ZF_IN zfbool positiveDirection)
{
    zfindex wrapIndex = 0;
    zfint lineSizeUsed = 0;
    while(wrapIndex < parent->childCount()) // for each line
    {
        zfint requiredSize = 0;
        zfint totalWeight = 0;
        zfbool hasLayoutedChild = zffalse;
        zfint parentMarginX = ZFUIMarginGetX(parent->layoutChildMargin());
        zfint parentMarginY = ZFUIMarginGetY(parent->layoutChildMargin());
        zfindex wrapIndexTmp = parent->childCount();
        zfint lineSize = 0;
        zfint prevLineSpace = (wrapIndex > 0 ? parent->layoutChildSpaceY() : 0);
        zfint flexableWrapSize = 0;
        for(zfindex i = wrapIndex; i < parent->childCount(); ++i)
        {
            ZFUIView *child = parent->childAtIndex(i);
            ZFUIFlowLayoutParam *layoutParam = child->layoutParamT();
            if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
            {
                continue ;
            }
            zfint prevSpace = (hasLayoutedChild ? parent->layoutChildSpaceX() : 0);
            hasLayoutedChild = zftrue;

            zfint marginX = parentMarginX + ZFUIMarginGetX(layoutParam->layoutMargin());
            zfint marginY = parentMarginY + ZFUIMarginGetY(layoutParam->layoutMargin());
            zfint childRequiredSize = 0;
            if(layoutParam->layoutWeight() > 0)
            {
                child->layoutMeasure(
                    ZFUISizeMake(
                        layoutParam->sizeHint().width,
                        ZFUIViewLayoutParam::sizeHintMerge(
                            layoutParam->sizeHint().height,
                            ZFUIViewLayoutParam::sizeHintOffset(size.height, 0 - lineSizeUsed - marginY - prevLineSpace))),
                    layoutParam->sizeParam());
                childRequiredSize += child->layoutMeasuredSize().width;
            }
            else
            {
                switch(layoutParam->sizeParam().width)
                {
                    case ZFUISizeType::e_Wrap:
                        child->layoutMeasure(
                            ZFUISizeMake(
                                layoutParam->sizeHint().width,
                                ZFUIViewLayoutParam::sizeHintMerge(
                                    layoutParam->sizeHint().height,
                                    ZFUIViewLayoutParam::sizeHintOffset(size.height, 0 - lineSizeUsed - marginY - prevLineSpace))),
                            layoutParam->sizeParam());
                        childRequiredSize += child->layoutMeasuredSize().width;
                        break;
                    case ZFUISizeType::e_Fill:
                        child->layoutMeasure(
                            ZFUISizeMake(
                                layoutParam->sizeHint().width,
                                ZFUIViewLayoutParam::sizeHintMerge(
                                    layoutParam->sizeHint().height,
                                    ZFUIViewLayoutParam::sizeHintOffset(size.height, 0 - lineSizeUsed - marginY - prevLineSpace))),
                            layoutParam->sizeParam());
                        childRequiredSize = zfmMax(
                            child->layoutMeasuredSize().width,
                            size.width - requiredSize - prevSpace - marginX);
                        break;
                    default:
                        zfCoreCriticalShouldNotGoHere();
                        return ;
                }
            }
            childRequiredSize += prevSpace + marginX;
            if(i > wrapIndex && requiredSize + childRequiredSize > size.width)
            {
                wrapIndexTmp = i;
                break;
            }

            if(layoutParam->layoutWeight() > 0)
            {
                flexableWrapSize += childRequiredSize;
            }
            requiredSize += childRequiredSize;
            lineSize = zfmMax(lineSize, child->layoutMeasuredSize().height + marginY);
            if(layoutParam->layoutWeight() > 0)
            {
                totalWeight += layoutParam->layoutWeight();
            }
            if(requiredSize >= size.width)
            {
                wrapIndexTmp = i + 1;
                break;
            }
        }
        hasLayoutedChild = zffalse;
        zfint flexibleSize = zfmMax(0, size.width - requiredSize + flexableWrapSize);
        zfint offset = (positiveDirection ? 0 : size.width);
        zfint lineY = ((_ZFP_ZFUIFlowLayout_layoutOrientationSecondary(
                parent->layoutOrientationMain(), parent->layoutOrientationSecondary()) == ZFUIOrientation::e_Top)
            ? lineSizeUsed + prevLineSpace
            : size.height - lineSizeUsed - lineSize - prevLineSpace);
        for(zfindex i = wrapIndex; i < wrapIndexTmp; ++i)
        {
            ZFUIView *child = parent->childAtIndex(i);
            ZFUIFlowLayoutParam *layoutParam = child->layoutParamT();
            if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
            {
                continue ;
            }
            zfint prevSpace = (hasLayoutedChild ? parent->layoutChildSpaceX() : 0);
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
                            ZFUIViewLayoutParam::sizeHintOffset(size.height, 0 - lineSizeUsed - marginY - prevLineSpace))
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
                                ZFUIViewLayoutParam::sizeHintOffset(size.height, 0 - lineSizeUsed - marginY - prevLineSpace))
                            ),
                        layoutParam->sizeParam());
                }
            }
            if(positiveDirection)
            {
                child->layout(ZFUIAlignApply(
                    layoutParam->layoutAlign(),
                    ZFUIRectMake(offset + prevSpace, lineY, child->layoutMeasuredSize().width + marginX, lineSize),
                    child->layoutMeasuredSize(),
                    layoutParam->layoutMargin() + parent->layoutChildMargin()));
                offset += prevSpace + child->layoutMeasuredSize().width + marginX;
            }
            else
            {
                offset -= child->layoutMeasuredSize().width + marginX + prevSpace;
                child->layout(ZFUIAlignApply(
                    layoutParam->layoutAlign(),
                    ZFUIRectMake(offset, lineY, child->layoutMeasuredSize().width + marginX, lineSize),
                    child->layoutMeasuredSize(),
                    layoutParam->layoutMargin() + parent->layoutChildMargin()));
            }
        } // for(zfindex i = wrapIndex; i < wrapIndexTmp; ++i)
        wrapIndex = wrapIndexTmp;
        lineSizeUsed += lineSize + prevLineSpace;
    } // for each line
}
static void _ZFP_ZFUIFlowLayout_layoutVertical(ZF_IN ZFUIFlowLayout *parent,
                                               ZF_IN const ZFUISize &size,
                                               ZF_IN zfbool positiveDirection)
{
    zfindex wrapIndex = 0;
    zfint lineSizeUsed = 0;
    while(wrapIndex < parent->childCount()) // for each line
    {
        zfint requiredSize = 0;
        zfint totalWeight = 0;
        zfbool hasLayoutedChild = zffalse;
        zfint parentMarginX = ZFUIMarginGetX(parent->layoutChildMargin());
        zfint parentMarginY = ZFUIMarginGetY(parent->layoutChildMargin());
        zfindex wrapIndexTmp = parent->childCount();
        zfint lineSize = 0;
        zfint prevLineSpace = (wrapIndex > 0 ? parent->layoutChildSpaceX() : 0);
        zfint flexableWrapSize = 0;
        for(zfindex i = wrapIndex; i < parent->childCount(); ++i)
        {
            ZFUIView *child = parent->childAtIndex(i);
            ZFUIFlowLayoutParam *layoutParam = child->layoutParamT();
            if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
            {
                continue ;
            }
            zfint prevSpace = (hasLayoutedChild ? parent->layoutChildSpaceY() : 0);
            hasLayoutedChild = zftrue;

            zfint marginX = parentMarginX + ZFUIMarginGetX(layoutParam->layoutMargin());
            zfint marginY = parentMarginY + ZFUIMarginGetY(layoutParam->layoutMargin());
            zfint childRequiredSize = 0;
            if(layoutParam->layoutWeight() > 0)
            {
                child->layoutMeasure(
                    ZFUISizeMake(
                        ZFUIViewLayoutParam::sizeHintMerge(
                            layoutParam->sizeHint().width,
                            ZFUIViewLayoutParam::sizeHintOffset(size.width, 0 - lineSizeUsed - marginX - prevLineSpace)),
                        layoutParam->sizeHint().height),
                    layoutParam->sizeParam());
                childRequiredSize += child->layoutMeasuredSize().height;
            }
            else
            {
                switch(layoutParam->sizeParam().height)
                {
                    case ZFUISizeType::e_Wrap:
                        child->layoutMeasure(
                            ZFUISizeMake(
                                ZFUIViewLayoutParam::sizeHintMerge(
                                    layoutParam->sizeHint().width,
                                    ZFUIViewLayoutParam::sizeHintOffset(size.width, 0 - lineSizeUsed - marginX - prevLineSpace)),
                                layoutParam->sizeHint().height),
                            layoutParam->sizeParam());
                        childRequiredSize += child->layoutMeasuredSize().height;
                        break;
                    case ZFUISizeType::e_Fill:
                        child->layoutMeasure(
                            ZFUISizeMake(
                                ZFUIViewLayoutParam::sizeHintMerge(
                                    layoutParam->sizeHint().width,
                                    ZFUIViewLayoutParam::sizeHintOffset(size.width, 0 - lineSizeUsed - marginX - prevLineSpace)),
                                layoutParam->sizeHint().height),
                            layoutParam->sizeParam());
                        childRequiredSize = zfmMax(
                            child->layoutMeasuredSize().height,
                            size.height - requiredSize - prevSpace - marginY);
                        break;
                    default:
                        zfCoreCriticalShouldNotGoHere();
                        return ;
                }
            }
            childRequiredSize += prevSpace + marginY;
            if(i > wrapIndex && requiredSize + childRequiredSize > size.height)
            {
                wrapIndexTmp = i;
                break;
            }

            if(layoutParam->layoutWeight() > 0)
            {
                flexableWrapSize += childRequiredSize;
            }
            requiredSize += childRequiredSize;
            lineSize = zfmMax(lineSize, child->layoutMeasuredSize().width + marginX);
            if(layoutParam->layoutWeight() > 0)
            {
                totalWeight += layoutParam->layoutWeight();
            }
            if(requiredSize >= size.height)
            {
                wrapIndexTmp = i + 1;
                break;
            }
        }
        hasLayoutedChild = zffalse;
        zfint flexibleSize = zfmMax(0, size.height - requiredSize + flexableWrapSize);
        zfint offset = (positiveDirection ? 0 : size.height);
        zfint lineX = ((_ZFP_ZFUIFlowLayout_layoutOrientationSecondary(
                parent->layoutOrientationMain(), parent->layoutOrientationSecondary()) == ZFUIOrientation::e_Left)
            ? lineSizeUsed + prevLineSpace
            : size.width - lineSizeUsed - lineSize - prevLineSpace);
        for(zfindex i = wrapIndex; i < wrapIndexTmp; ++i)
        {
            ZFUIView *child = parent->childAtIndex(i);
            ZFUIFlowLayoutParam *layoutParam = child->layoutParamT();
            if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
            {
                continue ;
            }
            zfint prevSpace = (hasLayoutedChild ? parent->layoutChildSpaceY() : 0);
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
                            ZFUIViewLayoutParam::sizeHintOffset(size.width, 0 - lineSizeUsed - marginX - prevLineSpace)),
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
                                ZFUIViewLayoutParam::sizeHintOffset(size.width, 0 - lineSizeUsed - marginX - prevLineSpace)),
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
                    ZFUIRectMake(lineX, offset + prevSpace, lineSize, child->layoutMeasuredSize().height + marginY),
                    child->layoutMeasuredSize(),
                    layoutParam->layoutMargin() + parent->layoutChildMargin()));
                offset += prevSpace + child->layoutMeasuredSize().height + marginY;
            }
            else
            {
                offset -= child->layoutMeasuredSize().height + marginY + prevSpace;
                child->layout(ZFUIAlignApply(
                    layoutParam->layoutAlign(),
                    ZFUIRectMake(lineX, offset, lineSize, child->layoutMeasuredSize().height + marginY),
                    child->layoutMeasuredSize(),
                    layoutParam->layoutMargin() + parent->layoutChildMargin()));
            }
        } // for(zfindex i = wrapIndex; i < wrapIndexTmp; ++i)
        wrapIndex = wrapIndexTmp;
        lineSizeUsed += lineSize + prevLineSpace;
    } // for each line
}

ZF_NAMESPACE_GLOBAL_END

