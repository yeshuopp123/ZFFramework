/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUITypeDef.h
 * @brief necessary header file for all ZFUIKit member
 */

#ifndef _ZFI_ZFUITypeDef_h_
#define _ZFI_ZFUITypeDef_h_

#include "ZFUIKitDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// basic size
/**
 * @brief return a scaled int
 *
 * note, to suit most case, apply scale on 1 pixel would always result to 1 pixel
 */
template<typename T_Int>
inline void ZFUISizeApplyScale(ZF_OUT T_Int &ret,
                               ZF_IN T_Int const &v,
                               ZF_IN zffloat scale)
{
    if(v == 1)
    {
        ret = 1;
    }
    else
    {
        if(scale < 1 - zffloatEpsilon)
        {
            scale = v * scale;
            if(scale > (T_Int)scale + 0.01f)
            {
                ret = (T_Int)scale + 1;
            }
            else
            {
                ret = (T_Int)scale;
            }
        }
        else
        {
            ret = (T_Int)zfmRound(v * scale);
        }
    }
}
/**
 * @brief return a scaled int
 *
 * note, to suit most case, apply scale on 1 pixel would always result to 1 pixel
 */
template<typename T_Int>
inline T_Int ZFUISizeApplyScale(ZF_IN T_Int const &v,
                                ZF_IN zffloat scale)
{
    T_Int ret = 0;
    ZFUISizeApplyScale(ret, v, scale);
    return ret;
}
/**
 * @brief return a scaled int reversely
 *
 * note, to suit most case, apply scale on 1 pixel would always result to 1 pixel
 */
template<typename T_Int>
inline void ZFUISizeApplyScaleReversely(ZF_OUT T_Int &ret,
                                        ZF_IN T_Int const &v,
                                        ZF_IN zffloat scale)
{
    if(v == 1)
    {
        ret = 1;
    }
    else
    {
        if(scale > 1 + zffloatEpsilon)
        {
            scale = v / scale;
            if(scale > (T_Int)scale + 0.01f)
            {
                ret = (T_Int)scale + 1;
            }
            else
            {
                ret = (T_Int)scale;
            }
        }
        else
        {
            ret = (T_Int)zfmRound(v / scale);
        }
    }
}
/**
 * @brief return a scaled int reversely
 *
 * note, to suit most case, apply scale on 1 pixel would always result to 1 pixel
 */
template<typename T_Int>
inline T_Int ZFUISizeApplyScaleReversely(ZF_IN T_Int const &v,
                                         ZF_IN zffloat scale)
{
    T_Int ret = 0;
    ZFUISizeApplyScaleReversely(ret, v, scale);
    return ret;
}

// ============================================================
// ZFUIPoint
/**
 * @brief 2D point
 */
zfclassPOD ZF_ENV_EXPORT ZFUIPoint
{
public:
    zfint x;   /**< @brief x */
    zfint y;   /**< @brief y */
};
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIPoint
 *       value="(0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFUIPoint, ZFUIPoint)

ZFVAR_CONVERT_WRAPPER_DECLARE(ZFUIPoint, ZFUIPoint)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFUIPoint, ZFUIPoint)

ZFOUTPUT_TYPE_DECLARE(ZFUIPoint)
ZFOUTPUT_TYPE(const ZFUIPoint *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFUIPoint *, {output << (const ZFUIPoint *)v;})
ZFINPUT_TYPE_DECLARE(ZFUIPoint, ZFUIPoint)

ZFCORE_POD_COMPARER_DECLARE(ZFUIPoint)
ZFCOMPARER_DEFAULT_DECLARE(ZFUIPoint, ZFUIPoint, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

/**
 * @brief (0, 0)
 */
extern ZF_ENV_EXPORT const ZFUIPoint ZFUIPointZero;

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFUIPoint, ZFUIPoint)

/**
 * @brief make a ZFUIPoint
 */
inline ZFUIPoint ZFUIPointMake(ZF_IN zfint const &x,
                               ZF_IN zfint const &y)
{
    ZFUIPoint ret = {x, y};
    return ret;
}

/**
 * @brief return a scaled point
 */
inline void ZFUIPointApplyScale(ZF_OUT ZFUIPoint &ret,
                                ZF_IN const ZFUIPoint &point,
                                ZF_IN zffloat scale)
{
    ret = ZFUIPointMake(ZFUISizeApplyScale(point.x, scale), ZFUISizeApplyScale(point.y, scale));
}
/**
 * @brief return a scaled point
 */
inline ZFUIPoint ZFUIPointApplyScale(ZF_IN const ZFUIPoint &point,
                                     ZF_IN zffloat scale)
{
    return ZFUIPointMake(ZFUISizeApplyScale(point.x, scale), ZFUISizeApplyScale(point.y, scale));
}
/**
 * @brief return a scaled point reversely
 */
inline void ZFUIPointApplyScaleReversely(ZF_OUT ZFUIPoint &ret,
                                         ZF_IN const ZFUIPoint &point,
                                         ZF_IN zffloat scale)
{
    ret = ZFUIPointMake(ZFUISizeApplyScaleReversely(point.x, scale), ZFUISizeApplyScaleReversely(point.y, scale));
}
/**
 * @brief return a scaled point reversely
 */
inline ZFUIPoint ZFUIPointApplyScaleReversely(ZF_IN const ZFUIPoint &point,
                                              ZF_IN zffloat scale)
{
    return ZFUIPointMake(ZFUISizeApplyScaleReversely(point.x, scale), ZFUISizeApplyScaleReversely(point.y, scale));
}

// ============================================================
// ZFUIMargin
/**
 * @brief margin
 */
zfclassPOD ZF_ENV_EXPORT ZFUIMargin
{
public:
    zfint left;    /**< @brief left margin */
    zfint top;     /**< @brief top margin */
    zfint right;   /**< @brief right margin */
    zfint bottom;  /**< @brief bottom margin */
};
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIMargin
 *       value="(0, 0, 0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFUIMargin, ZFUIMargin)

ZFVAR_CONVERT_WRAPPER_DECLARE(ZFUIMargin, ZFUIMargin)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFUIMargin, ZFUIMargin)

ZFOUTPUT_TYPE_DECLARE(ZFUIMargin)
ZFOUTPUT_TYPE(const ZFUIMargin *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFUIMargin *, {output << (const ZFUIMargin *)v;})
ZFINPUT_TYPE_DECLARE(ZFUIMargin, ZFUIMargin)

ZFCORE_POD_COMPARER_DECLARE(ZFUIMargin)
ZFCOMPARER_DEFAULT_DECLARE(ZFUIMargin, ZFUIMargin, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

/**
 * @brief (0, 0, 0, 0)
 */
extern ZF_ENV_EXPORT const ZFUIMargin ZFUIMarginZero;

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFUIMargin, ZFUIMargin)

/**
 * @brief make a margin
 */
inline ZFUIMargin ZFUIMarginMake(ZF_IN zfint const &left,
                                 ZF_IN zfint const &top,
                                 ZF_IN zfint const &right,
                                 ZF_IN zfint const &bottom)
{
    ZFUIMargin ret = {left, top, right, bottom};
    return ret;
}
/**
 * @brief make a margin with all sides set to margin
 */
inline ZFUIMargin ZFUIMarginMake(ZF_IN zfint const &margin)
{
    ZFUIMargin ret = {margin, margin, margin, margin};
    return ret;
}
/**
 * @brief make a margin with left/right to x and top/bottom to y
 */
inline ZFUIMargin ZFUIMarginMake(ZF_IN zfint const &x, ZF_IN zfint const &y)
{
    ZFUIMargin ret = {x, y, x, y};
    return ret;
}

/**
 * @brief return a scaled margin
 */
inline void ZFUIMarginApplyScale(ZF_OUT ZFUIMargin &ret,
                                 ZF_IN const ZFUIMargin &margin,
                                 ZF_IN zffloat scale)
{
    ret = ZFUIMarginMake(
        ZFUISizeApplyScale(margin.left, scale),
        ZFUISizeApplyScale(margin.top, scale),
        ZFUISizeApplyScale(margin.right, scale),
        ZFUISizeApplyScale(margin.bottom, scale)
        );
}
/**
 * @brief return a scaled margin
 */
inline ZFUIMargin ZFUIMarginApplyScale(ZF_IN const ZFUIMargin &margin,
                                       ZF_IN zffloat scale)
{
    return ZFUIMarginMake(
        ZFUISizeApplyScale(margin.left, scale),
        ZFUISizeApplyScale(margin.top, scale),
        ZFUISizeApplyScale(margin.right, scale),
        ZFUISizeApplyScale(margin.bottom, scale)
        );
}
/**
 * @brief return a scaled margin reversely
 */
inline void ZFUIMarginApplyScaleReversely(ZF_OUT ZFUIMargin &ret,
                                          ZF_IN const ZFUIMargin &margin,
                                          ZF_IN zffloat scale)
{
    ret = ZFUIMarginMake(ZFUISizeApplyScaleReversely(margin.left, scale),
        ZFUISizeApplyScaleReversely(margin.top, scale),
        ZFUISizeApplyScaleReversely(margin.right, scale),
        ZFUISizeApplyScaleReversely(margin.bottom, scale));
}
/**
 * @brief return a scaled margin reversely
 */
inline ZFUIMargin ZFUIMarginApplyScaleReversely(ZF_IN const ZFUIMargin &margin,
                                                ZF_IN zffloat scale)
{
    return ZFUIMarginMake(ZFUISizeApplyScaleReversely(margin.left, scale),
        ZFUISizeApplyScaleReversely(margin.top, scale),
        ZFUISizeApplyScaleReversely(margin.right, scale),
        ZFUISizeApplyScaleReversely(margin.bottom, scale));
}
/**
 * @brief left + right margin
 */
inline zfint ZFUIMarginGetX(ZF_IN const ZFUIMargin &margin)
{
    return margin.left + margin.right;
}
/**
 * @brief top + bottom margin
 */
inline zfint ZFUIMarginGetY(ZF_IN const ZFUIMargin &margin)
{
    return margin.top + margin.bottom;
}
/**
 * @brief v0 + v1
 */
inline void ZFUIMarginInc(ZF_IN_OUT ZFUIMargin &ret,
                          ZF_IN const ZFUIMargin &v0,
                          ZF_IN const ZFUIMargin &v1)
{
    ret.left = v0.left + v1.left;
    ret.top = v0.top + v1.top;
    ret.right = v0.right + v1.right;
    ret.bottom = v0.bottom + v1.bottom;
}
/**
 * @brief v0 + v1
 */
inline ZFUIMargin ZFUIMarginInc(ZF_IN const ZFUIMargin &v0,
                                ZF_IN const ZFUIMargin &v1)
{
    ZFUIMargin ret;
    ZFUIMarginInc(ret, v0, v1);
    return ret;
}
/**
 * @brief v0 + v1
 */
inline ZFUIMargin operator + (ZF_IN const ZFUIMargin &v0,
                              ZF_IN const ZFUIMargin &v1)
{
    return ZFUIMarginInc(v0, v1);
}
/**
 * @brief v0 += v1
 */
inline ZFUIMargin &operator += (ZF_IN_OUT ZFUIMargin &v0,
                                ZF_IN const ZFUIMargin &v1)
{
    v0.left += v1.left;
    v0.top += v1.top;
    v0.right += v1.right;
    v0.bottom += v1.bottom;
    return v0;
}
/**
 * @brief v0 - v1
 */
inline void ZFUIMarginDec(ZF_IN_OUT ZFUIMargin &ret,
                          ZF_IN const ZFUIMargin &v0,
                          ZF_IN const ZFUIMargin &v1)
{
    ret.left = v0.left - v1.left;
    ret.top = v0.top - v1.top;
    ret.right = v0.right - v1.right;
    ret.bottom = v0.bottom - v1.bottom;
}
/**
 * @brief v0 - v1
 */
inline ZFUIMargin ZFUIMarginDec(ZF_IN const ZFUIMargin &v0,
                                ZF_IN const ZFUIMargin &v1)
{
    ZFUIMargin ret;
    ZFUIMarginDec(ret, v0, v1);
    return ret;
}
/**
 * @brief v0 - v1
 */
inline ZFUIMargin operator - (ZF_IN const ZFUIMargin &v0,
                              ZF_IN const ZFUIMargin &v1)
{
    return ZFUIMarginDec(v0, v1);
}
/**
 * @brief v0 -= v1
 */
inline ZFUIMargin &operator -= (ZF_IN_OUT ZFUIMargin &v0,
                                ZF_IN const ZFUIMargin &v1)
{
    v0.left -= v1.left;
    v0.top -= v1.top;
    v0.right -= v1.right;
    v0.bottom -= v1.bottom;
    return v0;
}

// ============================================================
// ZFUISize
/**
 * @brief 2D size
 */
zfclassPOD ZF_ENV_EXPORT ZFUISize
{
public:
    zfint width;   /**< @brief width */
    zfint height;  /**< @brief height */
};
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUISize
 *       value="(0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFUISize, ZFUISize)

ZFVAR_CONVERT_WRAPPER_DECLARE(ZFUISize, ZFUISize)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFUISize, ZFUISize)

ZFOUTPUT_TYPE_DECLARE(ZFUISize)
ZFOUTPUT_TYPE(const ZFUISize *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFUISize *, {output << (const ZFUISize *)v;})
ZFINPUT_TYPE_DECLARE(ZFUISize, ZFUISize)

ZFCORE_POD_COMPARER_DECLARE(ZFUISize)
ZFCOMPARER_DEFAULT_DECLARE(ZFUISize, ZFUISize, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

/**
 * @brief (0, 0)
 */
extern ZF_ENV_EXPORT const ZFUISize ZFUISizeZero;
/**
 * @brief (-1, -1)
 *
 * since zero size is valid on some cases,
 * we supply -1 as special size to indicate an invalid size
 */
extern ZF_ENV_EXPORT const ZFUISize ZFUISizeInvalid;

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFUISize, ZFUISize)

/**
 * @brief make a ZFUISize
 */
inline ZFUISize ZFUISizeMake(ZF_IN zfint const &width,
                             ZF_IN zfint const &height)
{
    ZFUISize ret = {width, height};
    return ret;
}
/**
 * @brief make a ZFUISize
 */
inline ZFUISize ZFUISizeMake(ZF_IN zfint const &v)
{
    ZFUISize ret = {v, v};
    return ret;
}

/**
 * @brief check and apply size range
 *
 * return value in range [minSize, maxSize] if minSize < maxSize,
 * or return value in range [minSize, infinite) minSize > maxSize
 */
inline void ZFUISizeApplyRange(ZF_OUT ZFUISize &ret,
                               ZF_IN const ZFUISize &orgSize,
                               ZF_IN const ZFUISize &minSize,
                               ZF_IN const ZFUISize &maxSize)
{
    ret.width = zfmApplyRange(orgSize.width, minSize.width, maxSize.width);
    ret.height = zfmApplyRange(orgSize.height, minSize.height, maxSize.height);
}
/**
 * @brief check and apply size range
 *
 * return value in range [minSize, maxSize] if minSize < maxSize,
 * or return value in range [minSize, infinite) minSize > maxSize
 */
inline ZFUISize ZFUISizeApplyRange(ZF_IN const ZFUISize &orgSize,
                                   ZF_IN const ZFUISize &minSize,
                                   ZF_IN const ZFUISize &maxSize)
{
    return ZFUISizeMake(
        zfmApplyRange(orgSize.width, minSize.width, maxSize.width),
        zfmApplyRange(orgSize.height, minSize.height, maxSize.height)
    );
}
/**
 * @brief return a scaled size
 */
inline void ZFUISizeApplyScale(ZF_OUT ZFUISize &ret,
                               ZF_IN const ZFUISize &size,
                               ZF_IN zffloat scale)
{
    ret = ZFUISizeMake(ZFUISizeApplyScale(size.width, scale), ZFUISizeApplyScale(size.height, scale));
}
/**
 * @brief return a scaled size
 */
inline ZFUISize ZFUISizeApplyScale(ZF_IN const ZFUISize &size,
                                   ZF_IN zffloat scale)
{
    return ZFUISizeMake(ZFUISizeApplyScale(size.width, scale), ZFUISizeApplyScale(size.height, scale));
}
/**
 * @brief return a scaled size reversely
 */
inline void ZFUISizeApplyScaleReversely(ZF_OUT ZFUISize &ret,
                                        ZF_IN const ZFUISize &size,
                                        ZF_IN zffloat scale)
{
    ret = ZFUISizeMake(ZFUISizeApplyScaleReversely(size.width, scale), ZFUISizeApplyScaleReversely(size.height, scale));
}
/**
 * @brief return a scaled size reversely
 */
inline ZFUISize ZFUISizeApplyScaleReversely(ZF_IN const ZFUISize &size,
                                            ZF_IN zffloat scale)
{
    return ZFUISizeMake(ZFUISizeApplyScaleReversely(size.width, scale), ZFUISizeApplyScaleReversely(size.height, scale));
}

/**
 * @brief scale size to fit refSize and keep aspect ratio
 *
 * input size must be valid, while negative refSize means no limit
 */
extern ZF_ENV_EXPORT void ZFUISizeApplyAspectRatio(ZF_OUT ZFUISize &ret,
                                                   ZF_IN const ZFUISize &size,
                                                   ZF_IN const ZFUISize &refSize);
/**
 * @brief scale size to fit refSize and keep aspect ratio
 *
 * input size must be valid, while negative refSize means no limit
 */
inline ZFUISize ZFUISizeApplyAspectRatio(ZF_IN const ZFUISize &size,
                                         ZF_IN const ZFUISize &refSize)
{
    ZFUISize ret = ZFUISizeZero;
    ZFUISizeApplyAspectRatio(ret, size, refSize);
    return ret;
}

/**
 * @brief return a size excluding the margin
 */
inline void ZFUISizeApplyMargin(ZF_OUT ZFUISize &ret,
                                ZF_IN const ZFUISize &size,
                                ZF_IN const ZFUIMargin &margin)
{
    ret = ZFUISizeMake(
        size.width - margin.left - margin.right,
        size.height - margin.top - margin.bottom);
}
/**
 * @brief return a size excluding the margin
 */
inline ZFUISize ZFUISizeApplyMargin(ZF_IN const ZFUISize &size,
                                    ZF_IN const ZFUIMargin &margin)
{
    return ZFUISizeMake(
        size.width - margin.left - margin.right,
        size.height - margin.top - margin.bottom);
}
/**
 * @brief revert excluding the margin
 */
inline void ZFUISizeApplyMarginReversely(ZF_OUT ZFUISize &ret,
                                         ZF_IN const ZFUISize &size,
                                         ZF_IN const ZFUIMargin &margin)
{
    ret = ZFUISizeMake(
        size.width + margin.left + margin.right,
        size.height + margin.top + margin.bottom);
}
/**
 * @brief revert excluding the margin
 */
inline ZFUISize ZFUISizeApplyMarginReversely(ZF_IN const ZFUISize &size,
                                             ZF_IN const ZFUIMargin &margin)
{
    return ZFUISizeMake(
        size.width + margin.left + margin.right,
        size.height + margin.top + margin.bottom);
}

// ============================================================
// ZFUIRect
/**
 * @brief 2D rectangle
 */
zfclassPOD ZF_ENV_EXPORT ZFUIRect
{
public:
    ZFUIPoint point;      /**< @brief top-left point */
    ZFUISize size;        /**< @brief size of rect */
};
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIRect
 *       value="(0, 0, 0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFUIRect, ZFUIRect)

ZFVAR_CONVERT_WRAPPER_DECLARE(ZFUIRect, ZFUIRect)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFUIRect, ZFUIRect)

ZFOUTPUT_TYPE_DECLARE(ZFUIRect)
ZFOUTPUT_TYPE(const ZFUIRect *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFUIRect *, {output << (const ZFUIRect *)v;})
ZFINPUT_TYPE_DECLARE(ZFUIRect, ZFUIRect)

ZFCORE_POD_COMPARER_DECLARE(ZFUIRect)
ZFCOMPARER_DEFAULT_DECLARE(ZFUIRect, ZFUIRect, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

/**
 * @brief (0, 0, 0, 0)
 */
extern ZF_ENV_EXPORT const ZFUIRect ZFUIRectZero;

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFUIRect, ZFUIRect)

/**
 * @brief make a ZFUIRect
 */
inline ZFUIRect ZFUIRectMake(ZF_IN zfint const &x,
                             ZF_IN zfint const &y,
                             ZF_IN zfint const &w,
                             ZF_IN zfint const &h)
{
    ZFUIRect ret = {{x, y}, {w, h}};
    return ret;
}
/**
 * @brief make a ZFUIRect
 */
inline ZFUIRect ZFUIRectMake(ZF_IN const ZFUIPoint &point,
                             ZF_IN const ZFUISize &size)
{
    ZFUIRect ret = {point, size};
    return ret;
}

/**
 * @brief is rect empty
 *
 * rect is empty if width or height <= 0
 */
inline zfbool ZFUIRectIsEmpty(ZF_IN const ZFUIRect &rect)
{
    return (rect.size.width <= 0 || rect.size.height <= 0);
}
/**
 * @brief union two rect, return a rect that contains both rect1 and rect2
 */
inline ZFUIRect ZFUIRectUnion(ZF_IN const ZFUIRect &rect1,
                              ZF_IN const ZFUIRect &rect2)
{
    ZFUIRect ret = {
        {(rect1.point.x < rect2.point.x) ? rect1.point.x : rect2.point.x,
        (rect1.point.y < rect2.point.y) ? rect1.point.y : rect2.point.y},
        {(rect1.point.x + rect1.size.width > rect2.point.x + rect2.size.width)
            ? (rect1.point.x + rect1.size.width)
            : (rect2.point.x + rect2.size.width),
        (rect1.point.y + rect1.size.height > rect2.point.y + rect2.size.height)
            ? (rect1.point.y + rect1.size.height)
            : (rect2.point.y + rect2.size.height)}
    };
    if(ZFUIRectIsEmpty(ret))
    {
        return ZFUIRectZero;
    }
    return ret;
}
/**
 * @brief intersect two rect, return ZFUIRectZero if not intersected
 */
inline ZFUIRect ZFUIRectIntersection(ZF_IN const ZFUIRect &rect1,
                                     ZF_IN const ZFUIRect &rect2)
{
    ZFUIRect ret = {
        {(rect1.point.x > rect2.point.x) ? rect1.point.x : rect2.point.x,
        (rect1.point.y > rect2.point.y) ? rect1.point.y : rect2.point.y},
        {(rect1.point.x + rect1.size.width < rect2.point.x + rect2.size.width)
            ? (rect1.point.x + rect1.size.width)
            : (rect2.point.x + rect2.size.width),
        (rect1.point.y + rect1.size.height < rect2.point.y + rect2.size.height)
            ? (rect1.point.y + rect1.size.height)
            : (rect2.point.y + rect2.size.height)}
    };
    if(ZFUIRectIsEmpty(ret))
    {
        return ZFUIRectZero;
    }
    return ret;
}
/**
 * @brief is two rect intersected,
 *   two tangented rect is not intersected
 */
inline zfbool ZFUIRectIsIntersectToRect(ZF_IN const ZFUIRect &rect1,
                                        ZF_IN const ZFUIRect &rect2)
{
    return !ZFUIRectIsEmpty(ZFUIRectIntersection(rect1, rect2));
}
/**
 * @brief is rect contains point
 */
inline zfbool ZFUIRectIsContainPoint(ZF_IN const ZFUIRect &rect,
                                     ZF_IN const ZFUIPoint &point)
{
    return (point.x >= rect.point.x
        && point.x <= rect.point.x + rect.size.width
        && point.y >= rect.point.y
        && point.y <= rect.point.y + rect.size.height);
}
/**
 * @brief see #ZFUIRectIsContainPoint
 *
 * if margin is positive, rect's area is reduced by margin,
 * if margin is negative, rect's area is increased by margin
 */
inline zfbool ZFUIRectIsContainPoint(ZF_IN const ZFUIRect &rect,
                                     ZF_IN const ZFUIPoint &point,
                                     ZF_IN const ZFUIMargin &margin)
{
    return (point.x >= rect.point.x + margin.left
        && point.x <= rect.point.x + rect.size.width - margin.right
        && point.y >= rect.point.y + margin.top
        && point.y <= rect.point.y + rect.size.height - margin.bottom);
}
/**
 * @brief return true if rect2 is completely contained by rect1,
 *   two equal rect is regarded as contain each other
 */
inline zfbool ZFUIRectIsContainRect(ZF_IN const ZFUIRect &rect1,
                                    ZF_IN const ZFUIRect &rect2)
{
    return (rect2.point.x >= rect1.point.x
        && rect2.point.x + rect2.size.width <= rect1.point.x + rect1.size.width
        && rect2.point.y >= rect1.point.y
        && rect2.point.y + rect2.size.height <= rect1.point.y + rect1.size.height);
}
/**
 * @brief see #ZFUIRectIsContainRect
 *
 * if margin is positive, rect's area is reduced by margin,
 * if margin is negative, rect's area is increased by margin
 */
inline zfbool ZFUIRectIsContainRect(ZF_IN const ZFUIRect &rect1,
                                    ZF_IN const ZFUIRect &rect2,
                                    ZF_IN const ZFUIMargin &margin)
{
    return (rect2.point.x >= rect1.point.x + margin.left
        && rect2.point.x + rect2.size.width <= rect1.point.x + rect1.size.width - margin.right
        && rect2.point.y >= rect1.point.y + margin.top
        && rect2.point.y + rect2.size.height <= rect1.point.y + rect1.size.height - margin.bottom);
}
/**
 * @brief return a scaled rect
 */
inline void ZFUIRectApplyScale(ZF_OUT ZFUIRect &ret,
                               ZF_IN const ZFUIRect &rect,
                               ZF_IN zffloat scale)
{
    ret = ZFUIRectMake(ZFUISizeApplyScale(rect.point.x, scale),
        ZFUISizeApplyScale(rect.point.y, scale),
        ZFUISizeApplyScale(rect.size.width, scale),
        ZFUISizeApplyScale(rect.size.height, scale));
}
/**
 * @brief return a scaled rect
 */
inline ZFUIRect ZFUIRectApplyScale(ZF_IN const ZFUIRect &rect,
                                   ZF_IN zffloat scale)
{
    return ZFUIRectMake(ZFUISizeApplyScale(rect.point.x, scale),
        ZFUISizeApplyScale(rect.point.y, scale),
        ZFUISizeApplyScale(rect.size.width, scale),
        ZFUISizeApplyScale(rect.size.height, scale));
}
/**
 * @brief return a scaled rect reversely
 */
inline void ZFUIRectApplyScaleReversely(ZF_OUT ZFUIRect &ret,
                                        ZF_IN const ZFUIRect &rect,
                                        ZF_IN zffloat scale)
{
    ret = ZFUIRectMake(
        ZFUISizeApplyScaleReversely(rect.point.x, scale),
        ZFUISizeApplyScaleReversely(rect.point.y, scale),
        ZFUISizeApplyScaleReversely(rect.size.width, scale),
        ZFUISizeApplyScaleReversely(rect.size.height, scale)
        );
}
/**
 * @brief return a scaled rect reversely
 */
inline ZFUIRect ZFUIRectApplyScaleReversely(ZF_IN const ZFUIRect &rect,
                                            ZF_IN zffloat scale)
{
    return ZFUIRectMake(
        ZFUISizeApplyScaleReversely(rect.point.x, scale),
        ZFUISizeApplyScaleReversely(rect.point.y, scale),
        ZFUISizeApplyScaleReversely(rect.size.width, scale),
        ZFUISizeApplyScaleReversely(rect.size.height, scale)
        );
}
/**
 * @brief get bounds for rect, i.e. (0, 0, width, height)
 */
inline ZFUIRect ZFUIRectGetBounds(ZF_IN const ZFUIRect &rect)
{
    return ZFUIRectMake(0, 0, rect.size.width, rect.size.height);
}
/**
 * @brief get center point of the rect
 */
inline ZFUIPoint ZFUIRectGetCenter(ZF_IN const ZFUIRect &rect)
{
    return ZFUIPointMake(rect.point.x + rect.size.width / 2, rect.point.y + rect.size.height / 2);
}
/**
 * @brief get left edge of the rect
 */
inline zfint ZFUIRectGetLeft(ZF_IN const ZFUIRect &rect)
{
    return rect.point.x;
}
/**
 * @brief get top edge of the rect
 */
inline zfint ZFUIRectGetTop(ZF_IN const ZFUIRect &rect)
{
    return rect.point.y;
}
/**
 * @brief get right edge of the rect
 */
inline zfint ZFUIRectGetRight(ZF_IN const ZFUIRect &rect)
{
    return (rect.point.x + rect.size.width);
}
/**
 * @brief get bottom edge of the rect
 */
inline zfint ZFUIRectGetBottom(ZF_IN const ZFUIRect &rect)
{
    return (rect.point.y + rect.size.height);
}

/**
 * @brief return a rect excluding the margin
 */
inline void ZFUIRectApplyMargin(ZF_OUT ZFUIRect &ret,
                                ZF_IN const ZFUIRect &rect,
                                ZF_IN const ZFUIMargin &margin)
{
    ret = ZFUIRectMake(
        rect.point.x + margin.left,
        rect.point.y + margin.top,
        rect.size.width - margin.left - margin.right,
        rect.size.height - margin.top - margin.bottom);
}
/**
 * @brief return a rect excluding the margin
 */
inline ZFUIRect ZFUIRectApplyMargin(ZF_IN const ZFUIRect &rect,
                                    ZF_IN const ZFUIMargin &margin)
{
    return ZFUIRectMake(
        rect.point.x + margin.left,
        rect.point.y + margin.top,
        rect.size.width - margin.left - margin.right,
        rect.size.height - margin.top - margin.bottom);
}
/**
 * @brief revert excluding the margin
 */
inline void ZFUIRectApplyMarginReversely(ZF_OUT ZFUIRect &ret,
                                         ZF_IN const ZFUIRect &rect,
                                         ZF_IN const ZFUIMargin &margin)
{
    ret = ZFUIRectMake(
        rect.point.x - margin.left,
        rect.point.y - margin.top,
        rect.size.width + margin.left + margin.right,
        rect.size.height + margin.top + margin.bottom);
}
/**
 * @brief revert excluding the margin
 */
inline ZFUIRect ZFUIRectApplyMarginReversely(ZF_IN const ZFUIRect &rect,
                                             ZF_IN const ZFUIMargin &margin)
{
    return ZFUIRectMake(
        rect.point.x - margin.left,
        rect.point.y - margin.top,
        rect.size.width + margin.left + margin.right,
        rect.size.height + margin.top + margin.bottom);
}

// ============================================================
// ZFUIAlignFlags
/**
 * @brief align types for UI in ZFFramework
 * @see ZFUIAlignFlags
 */
ZFENUM_BEGIN(ZFUIAlign)
    ZFENUM_VALUE_WITH_INIT(Center, 0) /**< @brief ensured 0 */
    ZFENUM_VALUE_WITH_INIT(Left, 1)
    ZFENUM_VALUE_WITH_INIT(LeftInner, 2)
    ZFENUM_VALUE_WITH_INIT(Top, 4)
    ZFENUM_VALUE_WITH_INIT(TopInner, 8)
    ZFENUM_VALUE_WITH_INIT(Right, 16)
    ZFENUM_VALUE_WITH_INIT(RightInner, 32)
    ZFENUM_VALUE_WITH_INIT(Bottom, 64)
    ZFENUM_VALUE_WITH_INIT(BottomInner, 128)
ZFENUM_SEPARATOR(ZFUIAlign)
    ZFENUM_VALUE_REGISTER(Center)
    ZFENUM_VALUE_REGISTER(Left)
    ZFENUM_VALUE_REGISTER(LeftInner)
    ZFENUM_VALUE_REGISTER(Top)
    ZFENUM_VALUE_REGISTER(TopInner)
    ZFENUM_VALUE_REGISTER(Right)
    ZFENUM_VALUE_REGISTER(RightInner)
    ZFENUM_VALUE_REGISTER(Bottom)
    ZFENUM_VALUE_REGISTER(BottomInner)
ZFENUM_END(ZFUIAlign)

ZFENUM_FLAGS_DECLARE(ZFUIAlign, ZFUIAlignFlags)

/**
 * @brief true if align is valid
 *
 * a align is valid only if:
 * -  only bits in #ZFUIAlign are set
 * -  x or y align type must not conflict, i.e.
 *   "Left, LeftInner, Right, RightInner" could only exist at most one,
 *   "Top, TopInner, Bottom, BottomInner" could only exist at most one
 */
extern ZF_ENV_EXPORT zfbool ZFUIAlignIsValid(ZF_IN const ZFUIAlignFlags &align);
/**
 * @brief apply align and calculate result, or ZFUIRectZero if error
 */
extern ZF_ENV_EXPORT void ZFUIAlignApply(ZF_OUT ZFUIRect &ret,
                                         ZF_IN const ZFUIAlignFlags &align,
                                         ZF_IN const ZFUIRect &refRect,
                                         ZF_IN const ZFUISize &itemSize,
                                         ZF_IN_OPT const ZFUIMargin &margin = ZFUIMarginZero);
/**
 * @brief apply align and calculate result, or ZFUIRectZero if error
 */
inline ZFUIRect ZFUIAlignApply(ZF_IN const ZFUIAlignFlags &align,
                               ZF_IN const ZFUIRect &refRect,
                               ZF_IN const ZFUISize &itemSize,
                               ZF_IN_OPT const ZFUIMargin &margin = ZFUIMarginZero)
{
    ZFUIRect ret = ZFUIRectZero;
    ZFUIAlignApply(ret, align, refRect, itemSize, margin);
    return ret;
}
/**
 * @brief get horizontal align or #ZFUIAlign::e_Center if invalid
 */
extern ZF_ENV_EXPORT ZFUIAlignEnum ZFUIAlignGetX(ZF_IN const ZFUIAlignFlags &align);
/**
 * @brief get vertical align or #ZFUIAlign::e_Center if invalid
 */
extern ZF_ENV_EXPORT ZFUIAlignEnum ZFUIAlignGetY(ZF_IN const ZFUIAlignFlags &align);

// ============================================================
// ZFUIOrientation
/**
 * @brief orientation types for UI in ZFFramework
 *
 * for example, for a mobile phone,
 * Top means user hold the phone normally,
 * Left means user rotate the phone 90 degrees counter clockwise,
 * Right means user rotate the phone 90 degrees clockwise,
 * and Bottom means user rotate the phone 180 degrees
 */
ZFENUM_BEGIN(ZFUIOrientation)
    ZFENUM_VALUE_WITH_INIT(Left, 1)
    ZFENUM_VALUE_WITH_INIT(Top, 2)
    ZFENUM_VALUE_WITH_INIT(Right, 4)
    ZFENUM_VALUE_WITH_INIT(Bottom, 8)
ZFENUM_SEPARATOR(ZFUIOrientation)
    ZFENUM_VALUE_REGISTER(Left)
    ZFENUM_VALUE_REGISTER(Top)
    ZFENUM_VALUE_REGISTER(Right)
    ZFENUM_VALUE_REGISTER(Bottom)
ZFENUM_END(ZFUIOrientation)

ZFENUM_FLAGS_DECLARE(ZFUIOrientation, ZFUIOrientationFlags)

/** @brief (ZFUIOrientation::e_Left | ZFUIOrientation::e_Top | ZFUIOrientation::e_Right | ZFUIOrientation::e_Bottom) */
extern ZF_ENV_EXPORT const ZFUIOrientationFlags ZFUIOrientationFlagsAll;
/** @brief (ZFUIOrientation::e_Left | ZFUIOrientation::e_Right) */
extern ZF_ENV_EXPORT const ZFUIOrientationFlags ZFUIOrientationFlagsHorizontal;
/** @brief (ZFUIOrientation::e_Top | ZFUIOrientation::e_Bottom) */
extern ZF_ENV_EXPORT const ZFUIOrientationFlags ZFUIOrientationFlagsVertical;

/**
 * @brief true if vertical orientation
 */
inline ZF_ENV_EXPORT zfbool ZFUIOrientationIsVertical(ZF_IN ZFUIOrientationEnum orientation)
{
    return (orientation == ZFUIOrientation::e_Top || orientation == ZFUIOrientation::e_Bottom);
}
/**
 * @brief true if vertical orientation
 */
inline ZF_ENV_EXPORT zfbool ZFUIOrientationIsHorizontal(ZF_IN ZFUIOrientationEnum orientation)
{
    return (orientation == ZFUIOrientation::e_Left || orientation == ZFUIOrientation::e_Right);
}

// ============================================================
// ZFUIColor
/**
 * @brief 32 bit color with (AARRGGBB) format
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFUIColor
{
public:
    /** @cond ZFPrivateDoc */
    ZFUIColor(void)
    : color(0)
    {
    }
    ZFUIColor(ZF_IN zfuint32 color)
    : color(color)
    {
    }
    ZFUIColor(ZF_IN const ZFUIColor &ref)
    : color(ref.color)
    {
    }
public:
    inline zfuint32 colorValue(void) const
    {
        return this->color;
    }
    inline void colorValueSet(ZF_IN zfuint32 color)
    {
        this->color = color;
    }

public:
    inline operator zfuint32 (void) const
    {
        return this->color;
    }
    inline ZFUIColor &operator = (ZF_IN zfuint32 color)
    {
        this->color = color;
        return *this;
    }
    inline ZFUIColor &operator = (ZF_IN const ZFUIColor &ref)
    {
        this->color = ref.color;
        return *this;
    }
    inline zfbool operator == (ZF_IN zfuint32 color) const
    {
        return (this->color == color);
    }
    inline zfbool operator == (ZF_IN const ZFUIColor &ref) const
    {
        return (this->color == ref.color);
    }
    inline zfbool operator != (ZF_IN zfuint32 color) const
    {
        return (this->color != color);
    }
    inline zfbool operator != (ZF_IN const ZFUIColor &ref) const
    {
        return (this->color != ref.color);
    }
    /** @endcond */

private:
    zfuint32 color;
};
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIColor value="value">
 *   </ZFUIColor>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFUIColor, ZFUIColor)

ZFVAR_CONVERT_WRAPPER_DECLARE(ZFUIColor, ZFUIColor)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFUIColor, ZFUIColor)

ZFOUTPUT_TYPE_DECLARE(ZFUIColor)
ZFOUTPUT_TYPE(const ZFUIColor *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFUIColor *, {output << (const ZFUIColor *)v;})
ZFINPUT_TYPE_DECLARE(ZFUIColor, ZFUIColor)

/**
 * @brief true if two color is equal
 */
inline zfbool ZFUIColorIsEqual(ZF_IN ZFUIColor const &v0, ZF_IN ZFUIColor const &v1)
{
    return (v0 == v1);
}

ZFCOMPARER_DEFAULT_DECLARE(ZFUIColor, ZFUIColor, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

/**
 * @brief make a color
 */
#define ZFUIColorMake(a, r, g, b) \
    ZFUIColor((((zfuint)(a) & 0xFF) << 24) | (((zfuint)(r) & 0xFF) << 16) | (((zfuint)(g) & 0xFF) << 8) | ((zfuint)(b) & 0xFF))

/**
 * @brief get alpha from a #ZFUIColor as zfuint
 */
#define ZFUIColorGetA(c) ((zfuint)(((c) >> 24) & 0xFF))
/**
 * @brief get red from a #ZFUIColor as zfuint
 */
#define ZFUIColorGetR(c) ((zfuint)(((c) >> 16) & 0xFF))
/**
 * @brief get green from a #ZFUIColor as zfuint
 */
#define ZFUIColorGetG(c) ((zfuint)(((c) >> 8) & 0xFF))
/**
 * @brief get blue from a #ZFUIColor as zfuint
 */
#define ZFUIColorGetB(c) ((zfuint)((c) & 0xFF))

/**
 * @brief set alpha to a #ZFUIColor
 */
#define ZFUIColorSetA(c, a) ((c) = (zfuint32)(((c) & 0x00FFFFFF) | (((a) & 0xFF) << 24)))
/**
 * @brief set red from a #ZFUIColor
 */
#define ZFUIColorSetR(c, r) ((c) = (zfuint32)(((c) & 0xFF00FFFF) | (((r) & 0xFF) << 16)))
/**
 * @brief set green from a #ZFUIColor
 */
#define ZFUIColorSetG(c, g) ((c) = (zfuint32)(((c) & 0xFFFF00FF) | (((g) & 0xFF) << 8)))
/**
 * @brief set blue from a #ZFUIColor
 */
#define ZFUIColorSetB(c, b) ((c) = (zfuint32)(((c) & 0xFFFFFF00) | ((b) & 0xFF)))

/**
 * @brief return a copy of color with changed alpha
 */
#define ZFUIColorChangeA(c, a) (((c) & 0x00FFFFFF) | (((a) & 0xFF) << 24))
/**
 * @brief return a copy of color with changed red
 */
#define ZFUIColorChangeR(c, r) (((c) & 0xFF00FFFF) | (((r) & 0xFF) << 16))
/**
 * @brief return a copy of color with changed green
 */
#define ZFUIColorChangeG(c, g) (((c) & 0xFFFF00FF) | (((g) & 0xFF) << 8))
/**
 * @brief return a copy of color with changed blue
 */
#define ZFUIColorChangeB(c, b) (((c) & 0xFFFFFF00) | ((b) & 0xFF))

/** @brief 0x00000000 */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorZero;

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFUIColor, ZFUIColor)

// ============================================================
// ZFUITextAppearance
/**
 * @brief text appearance type, normal, bold or italic
 */
ZFENUM_BEGIN(ZFUITextAppearance)
    ZFENUM_VALUE(Normal) /**< @brief normal */
    ZFENUM_VALUE(Bold) /**< @brief bold */
    ZFENUM_VALUE(Italic) /**< @brief italic */
    ZFENUM_VALUE(BoldItalic) /**< @brief bold and italic */
ZFENUM_SEPARATOR(ZFUITextAppearance)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Bold)
    ZFENUM_VALUE_REGISTER(Italic)
    ZFENUM_VALUE_REGISTER(BoldItalic)
ZFENUM_END_WITH_DEFAULT(ZFUITextAppearance, Normal)

// ============================================================
// ZFUITextTruncateMode
/**
 * @brief text truncate mode
 */
ZFENUM_BEGIN(ZFUITextTruncateMode)
    ZFENUM_VALUE(Disable) /**< @brief never truncate */
    ZFENUM_VALUE(Head) /**< @brief truncate at head */
    ZFENUM_VALUE(Middle) /**< @brief truncate at middle */
    ZFENUM_VALUE(Tail) /**< @brief truncate at end */
ZFENUM_SEPARATOR(ZFUITextTruncateMode)
    ZFENUM_VALUE_REGISTER(Disable)
    ZFENUM_VALUE_REGISTER(Head)
    ZFENUM_VALUE_REGISTER(Middle)
    ZFENUM_VALUE_REGISTER(Tail)
ZFENUM_END_WITH_DEFAULT(ZFUITextTruncateMode, Disable)

// ============================================================
/**
 * @brief scale type when stretch items
 */
ZFENUM_BEGIN(ZFUIContentScaleType)
    /**
     * @brief simply fill to parent
     */
    ZFENUM_VALUE(Fill = 0)
    /**
     * @brief simply center the content without scale,
     *   may clip if content too large
     */
    ZFENUM_VALUE(Center)
    /**
     * @brief always keep aspect ratio and fill to min bounds,
     *   may leave blank area if aspect ratio differs from parent
     */
    ZFENUM_VALUE(FillCenter)
    /**
     * @brief always keep aspect ratio and fill to max bounds,
     *   may clip if aspect ratio differs from parent
     */
    ZFENUM_VALUE(FillCenterClipped)
    /**
     * @brief same as #e_Center if content not exceeds parent,
     *   same as #e_FillCenter if content exceeds parent
     */
    ZFENUM_VALUE(FitCenter)
    /**
     * @brief always fill x, may clip or leave blank if aspect ratio differs from parent
     */
    ZFENUM_VALUE(FillX)
    /**
     * @brief same as #e_Center if x not exceeds parent,
     *   same as #e_FillX if x exceeds parent
     */
    ZFENUM_VALUE(FitX)
    /**
     * @brief always fill y, may clip or leave blank if aspect ratio differs from parent
     */
    ZFENUM_VALUE(FillY)
    /**
     * @brief same as #e_Center if y not exceeds parent,
     *   same as #e_FillY if y exceeds parent
     */
    ZFENUM_VALUE(FitY)
ZFENUM_SEPARATOR(ZFUIContentScaleType)
    ZFENUM_VALUE_REGISTER(Fill)
    ZFENUM_VALUE_REGISTER(Center)
    ZFENUM_VALUE_REGISTER(FillCenter)
    ZFENUM_VALUE_REGISTER(FillCenterClipped)
    ZFENUM_VALUE_REGISTER(FitCenter)
    ZFENUM_VALUE_REGISTER(FillX)
    ZFENUM_VALUE_REGISTER(FitX)
    ZFENUM_VALUE_REGISTER(FillY)
    ZFENUM_VALUE_REGISTER(FitY)
ZFENUM_END_WITH_DEFAULT(ZFUIContentScaleType, Fill)

/**
 * @brief apply scale for #ZFUIContentScaleType
 */
extern ZF_ENV_EXPORT void ZFUIContentScaleTypeApply(ZF_OUT ZFUIRect &ret,
                                                    ZF_IN ZFUIContentScaleTypeEnum scaleType,
                                                    ZF_IN const ZFUIRect &bounds,
                                                    ZF_IN const ZFUISize &contentSize);
/**
 * @brief apply scale for #ZFUIContentScaleType
 */
inline ZFUIRect ZFUIContentScaleTypeApply(ZF_IN ZFUIContentScaleTypeEnum scaleType,
                                          ZF_IN const ZFUIRect &bounds,
                                          ZF_IN const ZFUISize &contentSize)
{
    ZFUIRect ret = ZFUIRectZero;
    ZFUIContentScaleTypeApply(ret, scaleType, bounds, contentSize);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUITypeDef_h_

#include "ZFUIColor_common.h"

