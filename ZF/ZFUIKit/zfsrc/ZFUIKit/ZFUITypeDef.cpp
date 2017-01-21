/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUITypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIPoint
const ZFUIPoint ZFUIPointZero = {0, 0};
ZFCORETYPE_STRING_CONVERTER_DEFINE(ZFUIPoint, ZFUIPoint, {
        zfCoreAssert(src != zfnull);
        ZFCoreArrayPOD<zfint> buf;
        const zfchar *errPos = zfCoreDataPairSplitInt(buf, 2, src, srcLen);
        if(errPos != zfnull)
        {
            return errPos;
        }
        v.x = buf[0];
        v.y = buf[1];
        return errPos;
    }, {
        zfstringAppend(s, zfText("(%d, %d)"), v.x, v.y);
    })

ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(ZFUIPoint, ZFUIPoint)

ZFINPUT_TYPE_DEFINE(ZFUIPoint, ZFUIPoint, {
    v = ZFUIPointZero;
    ZFCoreArrayPOD<zfint> elementsTmp;
    if(!ZFInputCallbackReadDataPairInt(elementsTmp, input, 2))
    {
        return zffalse;
    }
    v.x = elementsTmp[0];
    v.y = elementsTmp[1];
    return zftrue;
})

// ============================================================
// ZFUIMargin
const ZFUIMargin ZFUIMarginZero = {0, 0, 0, 0};
ZFCORETYPE_STRING_CONVERTER_DEFINE(ZFUIMargin, ZFUIMargin, {
        zfCoreAssert(src != zfnull);
        ZFCoreArrayPOD<zfint> buf;
        const zfchar *errPos = zfCoreDataPairSplitInt(buf, 4, src, srcLen);
        if(errPos != zfnull)
        {
            return errPos;
        }
        v.left = buf[0];
        v.top = buf[1];
        v.right = buf[2];
        v.bottom = buf[3];
        return errPos;
    }, {
        zfstringAppend(s, zfText("(%d, %d, %d, %d)"), v.left, v.top, v.right, v.bottom);
    })

ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(ZFUIMargin, ZFUIMargin)

ZFINPUT_TYPE_DEFINE(ZFUIMargin, ZFUIMargin, {
    v = ZFUIMarginZero;
    ZFCoreArrayPOD<zfint> elementsTmp;
    if(!ZFInputCallbackReadDataPairInt(elementsTmp, input, 2))
    {
        return zffalse;
    }
    v.left = elementsTmp[0];
    v.top = elementsTmp[1];
    v.right = elementsTmp[2];
    v.bottom = elementsTmp[3];
    return zftrue;
})

// ============================================================
// ZFUISize
const ZFUISize ZFUISizeZero = {0, 0};
const ZFUISize ZFUISizeInvalid = {-1, -1};
ZFCORETYPE_STRING_CONVERTER_DEFINE(ZFUISize, ZFUISize, {
        zfCoreAssert(src != zfnull);
        ZFCoreArrayPOD<zfint> buf;
        const zfchar *errPos = zfCoreDataPairSplitInt(buf, 2, src, srcLen);
        if(errPos != zfnull)
        {
            return errPos;
        }
        v.width = buf[0];
        v.height = buf[1];
        return errPos;
    }, {
        zfstringAppend(s, zfText("(%d, %d)"), v.width, v.height);
    })

void ZFUISizeApplyAspectRatio(ZF_OUT ZFUISize &ret,
                              ZF_IN const ZFUISize &size,
                              ZF_IN const ZFUISize &refSize)
{
    if(size.width <= 0 || size.height <= 0
        || refSize.width == 0 || refSize.height == 0)
    {
        ret = ZFUISizeZero;
    }
    else if(refSize.width < 0 && refSize.height < 0)
    {
        ret = size;
    }
    else if(refSize.height < 0 || size.width * refSize.height > size.height * refSize.width)
    {
        ret = ZFUISizeMake(
            refSize.width,
            size.height * refSize.width / size.width
        );
    }
    else
    {
        ret = ZFUISizeMake(
            size.width * refSize.height / size.height,
            refSize.height
        );
    }
}

ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(ZFUISize, ZFUISize)

ZFINPUT_TYPE_DEFINE(ZFUISize, ZFUISize, {
    v = ZFUISizeZero;
    ZFCoreArrayPOD<zfint> elementsTmp;
    if(!ZFInputCallbackReadDataPairInt(elementsTmp, input, 2))
    {
        return zffalse;
    }
    v.width = elementsTmp[0];
    v.height = elementsTmp[1];
    return zftrue;
})

// ============================================================
// ZFUIRect
const ZFUIRect ZFUIRectZero = {{0, 0}, {0, 0}};
ZFCORETYPE_STRING_CONVERTER_DEFINE(ZFUIRect, ZFUIRect, {
        zfCoreAssert(src != zfnull);
        ZFCoreArrayPOD<zfint> buf;
        const zfchar *errPos = zfCoreDataPairSplitInt(buf, 4, src, srcLen);
        if(errPos != zfnull)
        {
            return errPos;
        }
        v.point.x = buf[0];
        v.point.y = buf[1];
        v.size.width = buf[2];
        v.size.height = buf[3];
        return errPos;
    }, {
        zfstringAppend(s, zfText("(%d, %d, %d, %d)"), v.point.x, v.point.y, v.size.width, v.size.height);
    })

ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(ZFUIRect, ZFUIRect)

ZFINPUT_TYPE_DEFINE(ZFUIRect, ZFUIRect, {
    v = ZFUIRectZero;
    ZFCoreArrayPOD<zfint> elementsTmp;
    if(!ZFInputCallbackReadDataPairInt(elementsTmp, input, 4))
    {
        return zffalse;
    }
    v.point.x = elementsTmp[0];
    v.point.y = elementsTmp[1];
    v.size.width = elementsTmp[2];
    v.size.height = elementsTmp[3];
    return zftrue;
})

// ============================================================
// ZFUIAlignFlags
ZFENUM_DEFINE(ZFUIAlign)
ZFENUM_FLAGS_DEFINE(ZFUIAlign, ZFUIAlignFlags)
static const ZFUIAlignFlags &_ZFP_ZFUIAlignMask(void)
{
    static ZFUIAlignFlags _alignMask = (0
        | ZFUIAlign::e_Center
        | ZFUIAlign::e_Left
        | ZFUIAlign::e_LeftInner
        | ZFUIAlign::e_Top
        | ZFUIAlign::e_TopInner
        | ZFUIAlign::e_Right
        | ZFUIAlign::e_RightInner
        | ZFUIAlign::e_Bottom
        | ZFUIAlign::e_BottomInner
        );
    return _alignMask;
}
zfbool ZFUIAlignIsValid(ZF_IN const ZFUIAlignFlags &align)
{
    return (
            ZFBitGet(align, ~(_ZFP_ZFUIAlignMask())) == 0
            && ((zfint)ZFBitTest(align, ZFUIAlign::e_Left)
                + (zfint)ZFBitTest(align, ZFUIAlign::e_LeftInner)
                + (zfint)ZFBitTest(align, ZFUIAlign::e_Right)
                + (zfint)ZFBitTest(align, ZFUIAlign::e_RightInner)
                ) <= 1
            && ((zfint)ZFBitTest(align, ZFUIAlign::e_Top)
                + (zfint)ZFBitTest(align, ZFUIAlign::e_TopInner)
                + (zfint)ZFBitTest(align, ZFUIAlign::e_Bottom)
                + (zfint)ZFBitTest(align, ZFUIAlign::e_BottomInner)
                ) <= 1
        );
}
void ZFUIAlignApply(ZF_OUT ZFUIRect &ret,
                    ZF_IN const ZFUIAlignFlags &align,
                    ZF_IN const ZFUIRect &refRect,
                    ZF_IN const ZFUISize &itemSize,
                    ZF_IN_OPT const ZFUIMargin &margin /* = ZFUIMarginZero */)
{
    ret = ZFUIRectZero;
    if(!ZFUIAlignIsValid(align))
    {
        return ;
    }
    ret.size = itemSize;

    if(ZFBitTest(align, ZFUIAlign::e_LeftInner))
    {
        ret.point.x = refRect.point.x + margin.left;
    }
    else if(ZFBitTest(align, ZFUIAlign::e_RightInner))
    {
        ret.point.x = refRect.point.x + refRect.size.width - margin.right - itemSize.width;
    }
    else if(ZFBitTest(align, ZFUIAlign::e_Left))
    {
        ret.point.x = refRect.point.x - margin.right - itemSize.width;
    }
    else if(ZFBitTest(align, ZFUIAlign::e_Right))
    {
        ret.point.x = refRect.point.x + refRect.size.width + margin.left;
    }
    else
    {
        ret.point.x = refRect.point.x + margin.left
            + (refRect.size.width - margin.left - margin.right - itemSize.width) / 2;
    }

    if(ZFBitTest(align, ZFUIAlign::e_TopInner))
    {
        ret.point.y = refRect.point.y + margin.top;
    }
    else if(ZFBitTest(align, ZFUIAlign::e_BottomInner))
    {
        ret.point.y = refRect.point.y + refRect.size.height - margin.bottom - itemSize.height;
    }
    else if(ZFBitTest(align, ZFUIAlign::e_Top))
    {
        ret.point.y = refRect.point.y - margin.bottom - itemSize.height;
    }
    else if(ZFBitTest(align, ZFUIAlign::e_Bottom))
    {
        ret.point.y = refRect.point.y + refRect.size.height + margin.top;
    }
    else
    {
        ret.point.y = refRect.point.y + margin.top
            + (refRect.size.height - margin.top - margin.bottom - itemSize.height) / 2;
    }
}
ZFUIAlignEnum ZFUIAlignGetX(ZF_IN const ZFUIAlignFlags &align)
{
    if(!ZFUIAlignIsValid(align))
    {
        return ZFUIAlign::e_Center;
    }

    if(ZFBitTest(align, ZFUIAlign::e_Left))
    {
        return ZFUIAlign::e_Left;
    }
    if(ZFBitTest(align, ZFUIAlign::e_LeftInner))
    {
        return ZFUIAlign::e_LeftInner;
    }
    if(ZFBitTest(align, ZFUIAlign::e_Right))
    {
        return ZFUIAlign::e_Right;
    }
    if(ZFBitTest(align, ZFUIAlign::e_RightInner))
    {
        return ZFUIAlign::e_RightInner;
    }
    return ZFUIAlign::e_Center;
}
ZFUIAlignEnum ZFUIAlignGetY(ZF_IN const ZFUIAlignFlags &align)
{
    if(!ZFUIAlignIsValid(align))
    {
        return ZFUIAlign::e_Center;
    }

    if(ZFBitTest(align, ZFUIAlign::e_Top))
    {
        return ZFUIAlign::e_Top;
    }
    if(ZFBitTest(align, ZFUIAlign::e_TopInner))
    {
        return ZFUIAlign::e_TopInner;
    }
    if(ZFBitTest(align, ZFUIAlign::e_Bottom))
    {
        return ZFUIAlign::e_Bottom;
    }
    if(ZFBitTest(align, ZFUIAlign::e_BottomInner))
    {
        return ZFUIAlign::e_BottomInner;
    }
    return ZFUIAlign::e_Center;
}

// ============================================================
// ZFUIColor
const ZFUIColor ZFUIColorZero = ((ZFUIColor)0x00000000);

ZFCORETYPE_STRING_CONVERTER_DEFINE(ZFUIColor, ZFUIColor, {
        zft_zfuint32 c = 0;
        do
        {
            if(src == zfnull
                || (srcLen != zfindexMax && srcLen != 9))
            {
                return src;
            }
            if(*src != '#') {return src;} ++src;
            zfuint tmp = 0;
            const zfchar *tmpPos = zfnull;

            tmp = 0;
            tmpPos = zfsToInt(tmp, src, 2, 16);
            if(tmpPos != zfnull) {return tmpPos;} src += 2;
            ZFBitSet(c, (tmp << 24));

            tmp = 0;
            tmpPos = zfsToInt(tmp, src, 2, 16);
            if(tmpPos != zfnull) {return tmpPos;} src += 2;
            ZFBitSet(c, (tmp << 16));

            tmp = 0;
            tmpPos = zfsToInt(tmp, src, 2, 16);
            if(tmpPos != zfnull) {return tmpPos;} src += 2;
            ZFBitSet(c, (tmp << 8));

            tmp = 0;
            tmpPos = zfsToInt(tmp, src, 2, 16);
            if(tmpPos != zfnull) {return tmpPos;} src += 2;
            ZFBitSet(c, (tmp));

            ZFUNUSED(src);
        } while(zffalse);

        v = c;

        return zfnull;
    }, {
        zfstringAppend(s, zfText("#%02X%02X%02X%02X"),
            ZFUIColorGetA(v),
            ZFUIColorGetR(v),
            ZFUIColorGetG(v),
            ZFUIColorGetB(v));
    })

ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(ZFUIColor, ZFUIColor)

ZFINPUT_TYPE_DEFINE(ZFUIColor, ZFUIColor, {
    v = ZFUIColorZero;
    zfchar buf[16] = {0};
    if((buf[0] = ZFInputCallbackSkipChars(input)) == 0)
    {
        return zffalse;
    }
    if(input.execute(buf + 1, 8) != 8)
    {
        return zffalse;
    }
    return (ZFUIColorFromString(v, buf) == zfnull);
})

// ============================================================
ZFENUM_DEFINE(ZFUIOrientation)
ZFENUM_FLAGS_DEFINE(ZFUIOrientation, ZFUIOrientationFlags)

const ZFUIOrientationFlags ZFUIOrientationFlagsAll = (ZFUIOrientation::e_Left | ZFUIOrientation::e_Top | ZFUIOrientation::e_Right | ZFUIOrientation::e_Bottom);
const ZFUIOrientationFlags ZFUIOrientationFlagsHorizontal = (ZFUIOrientation::e_Left | ZFUIOrientation::e_Right);
const ZFUIOrientationFlags ZFUIOrientationFlagsVertical = (ZFUIOrientation::e_Top | ZFUIOrientation::e_Bottom);

ZFENUM_DEFINE(ZFUITextAppearance)

ZFENUM_DEFINE(ZFUITextTruncateMode)

// ============================================================
ZFENUM_DEFINE(ZFUIContentScaleType)

static void _ZFP_ZFUIContentScaleTypeApply_Center(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &bounds, ZF_IN const ZFUISize &contentSize)
{
    ret = ZFUIAlignApply(ZFUIAlign::e_Center, bounds, contentSize);
}
static void _ZFP_ZFUIContentScaleTypeApply_FillX(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &bounds, ZF_IN const ZFUISize &contentSize)
{
    ret = ZFUIAlignApply(ZFUIAlign::e_Center, bounds, ZFUISizeMake(
        bounds.size.width,
        bounds.size.width * contentSize.height / contentSize.width
        ));
}
static void _ZFP_ZFUIContentScaleTypeApply_FillY(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &bounds, ZF_IN const ZFUISize &contentSize)
{
    ret = ZFUIAlignApply(ZFUIAlign::e_Center, bounds, ZFUISizeMake(
        contentSize.width * bounds.size.height / contentSize.height,
        bounds.size.height
        ));
}
static void _ZFP_ZFUIContentScaleTypeApply_FillCenter(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &bounds, ZF_IN const ZFUISize &contentSize)
{
    if(contentSize.width * bounds.size.height >= bounds.size.width * contentSize.height)
    {
        _ZFP_ZFUIContentScaleTypeApply_FillX(ret, bounds, contentSize);
    }
    else
    {
        _ZFP_ZFUIContentScaleTypeApply_FillY(ret, bounds, contentSize);
    }
}
static void _ZFP_ZFUIContentScaleTypeApply_FillCenterClipped(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &bounds, ZF_IN const ZFUISize &contentSize)
{
    if(contentSize.width * bounds.size.height >= bounds.size.width * contentSize.height)
    {
        _ZFP_ZFUIContentScaleTypeApply_FillY(ret, bounds, contentSize);
    }
    else
    {
        _ZFP_ZFUIContentScaleTypeApply_FillX(ret, bounds, contentSize);
    }
}
void ZFUIContentScaleTypeApply(ZF_OUT ZFUIRect &ret,
                               ZF_IN ZFUIContentScaleTypeEnum scaleType,
                               ZF_IN const ZFUIRect &bounds,
                               ZF_IN const ZFUISize &contentSize)
{
    switch(scaleType)
    {
        case ZFUIContentScaleType::e_Fill:
            ret = bounds;
            break;
        case ZFUIContentScaleType::e_Center:
            _ZFP_ZFUIContentScaleTypeApply_Center(ret, bounds, contentSize);
            break;
        case ZFUIContentScaleType::e_FillCenter:
            _ZFP_ZFUIContentScaleTypeApply_FillCenter(ret, bounds, contentSize);
            break;
        case ZFUIContentScaleType::e_FillCenterClipped:
            _ZFP_ZFUIContentScaleTypeApply_FillCenterClipped(ret, bounds, contentSize);
            break;
        case ZFUIContentScaleType::e_FitCenter:
            if(contentSize.width <= bounds.size.width && contentSize.height <= bounds.size.height)
            {
                _ZFP_ZFUIContentScaleTypeApply_Center(ret, bounds, contentSize);
            }
            else
            {
                _ZFP_ZFUIContentScaleTypeApply_FillCenter(ret, bounds, contentSize);
            }
            break;
        case ZFUIContentScaleType::e_FillX:
            _ZFP_ZFUIContentScaleTypeApply_FillX(ret, bounds, contentSize);
            break;
        case ZFUIContentScaleType::e_FitX:
            if(contentSize.width <= bounds.size.width)
            {
                _ZFP_ZFUIContentScaleTypeApply_Center(ret, bounds, contentSize);
            }
            else
            {
                _ZFP_ZFUIContentScaleTypeApply_FillX(ret, bounds, contentSize);
            }
            break;
        case ZFUIContentScaleType::e_FillY:
            _ZFP_ZFUIContentScaleTypeApply_FillY(ret, bounds, contentSize);
            break;
        case ZFUIContentScaleType::e_FitY:
            if(contentSize.height <= bounds.size.height)
            {
                _ZFP_ZFUIContentScaleTypeApply_Center(ret, bounds, contentSize);
            }
            else
            {
                _ZFP_ZFUIContentScaleTypeApply_FillY(ret, bounds, contentSize);
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

ZF_NAMESPACE_GLOBAL_END

