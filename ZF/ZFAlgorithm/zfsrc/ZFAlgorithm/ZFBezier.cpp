/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFBezier.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFBezier &ZFBezier::controlPointSet(ZF_IN zffloat p0x, ZF_IN zffloat p0y,
                                    ZF_IN zffloat p1x, ZF_IN zffloat p1y)
{
    this->p0x = p0x;
    this->p0y = p0y;
    this->p1x = p1x;
    this->p1y = p1y;

    cx = 3.0f * p0x;
    bx = 3.0f * (p1x - p0x) - cx;
    ax = 1.0f - cx -bx;

    cy = 3.0f * p0y;
    by = 3.0f * (p1y - p0y) - cy;
    ay = 1.0f - cy - by;
    return *this;
}

zffloat ZFBezier::y_by_t(ZF_IN zffloat t) const
{
    return ((ay * t + by) * t + cy) * t;
}

zffloat ZFBezier::y_by_x(ZF_IN zffloat x) const
{
    return this->y_by_t(this->t_by_x(x));
}

zffloat ZFBezier::x_by_t(ZF_IN zffloat t) const
{
    return ((ax * t + bx) * t + cx) * t;
}

zffloat ZFBezier::x_by_y(ZF_IN zffloat y) const
{
    return this->x_by_t(this->t_by_y(y));
}

zffloat ZFBezier::t_by_x(ZF_IN zffloat x) const
{
    zffloat t0;
    zffloat t1;
    zffloat t2 = x;
    zffloat x2;
    zffloat d2;

    for(zfindex i = 0; i < 8; ++i)
    {
        x2 = this->x_by_t(t2) - x;
        if(zfmAbs(x2) < zffloatEpsilon)
        {
            return t2;
        }
        d2 = ((3.0f * ax * t2 + 2.0f * bx) * t2 + cx);
        if(zfmAbs(d2) < 1e-6)
        {
            break;
        }
        t2 = t2 - x2 / d2;
    }

    t0 = 0.0f;
    t1 = 1.0f;
    t2 = x;

    if(t2 < t0)
    {
        return t0;
    }
    if(t2 > t1)
    {
        return t1;
    }

    while(t0 < t1)
    {
        x2 = this->x_by_t(t2);
        if(zfmAbs(x2 - x) < zffloatEpsilon)
        {
            return t2;
        }
        if(x > x2)
        {
            t0 = t2;
        }
        else
        {
            t1 = t2;
        }
        t2 = (t1 - t0) * 0.5f + t0;
    }

    return t2;
}

zffloat ZFBezier::t_by_y(ZF_IN zffloat y) const
{
    zffloat t0;
    zffloat t1;
    zffloat t2 = y;
    zffloat y2;
    zffloat d2;

    for(zfindex i = 0; i < 8; ++i)
    {
        y2 = this->y_by_t(t2) - y;
        if(zfmAbs(y2) < zffloatEpsilon)
        {
            return t2;
        }
        d2 = ((3.0f * ay * t2 + 2.0f * by) * t2 + cy);
        if(zfmAbs(d2) < 1e-6)
        {
            break;
        }
        t2 = t2 - y2 / d2;
    }

    t0 = 0.0f;
    t1 = 1.0f;
    t2 = y;

    if(t2 < t0)
    {
        return t0;
    }
    if(t2 > t1)
    {
        return t1;
    }

    while(t0 < t1)
    {
        y2 = this->y_by_t(t2);
        if(zfmAbs(y2 - y) < zffloatEpsilon)
        {
            return t2;
        }
        if(y > y2)
        {
            t0 = t2;
        }
        else
        {
            t1 = t2;
        }
        t2 = (t1 - t0) * 0.5f + t0;
    }

    return t2;
}

// ============================================================
const ZFBezier ZFBezierZero(0, 0, 1, 1);
const ZFBezier ZFBezierLinear(0, 0, 1, 1);
const ZFBezier ZFBezierEaseIn(0.8f, 0, 1, 1);
const ZFBezier ZFBezierEaseOut(0, 0, 0.2f, 1);
const ZFBezier ZFBezierEaseInOut(0.8f, 0, 0.2f, 1);
const ZFBezier ZFBezierBounceIn(0.2f, -0.5f, 1, 1);
const ZFBezier ZFBezierBounceOut(0, 0, 0.8f, 1.5f);
const ZFBezier ZFBezierBounceInOut(0.2f, -0.5f, 0.8f, 1.5f);

// ============================================================
void ZFBezierToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFBezier const &value)
{
    ret += zfText("(");
    zfsFromFloatT(ret, value.p0x);
    ret += zfText(", ");
    zfsFromFloatT(ret, value.p0y);
    ret += zfText(", ");
    zfsFromFloatT(ret, value.p1x);
    ret += zfText(", ");
    zfsFromFloatT(ret, value.p1y);
    ret += zfText(")");
}
const zfchar *ZFBezierFromString(ZF_OUT ZFBezier &ret,
                                 ZF_IN const zfchar *src,
                                 ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    ZFCoreArrayPOD<zffloat> tmp;
    const zfchar *errPos = zfCoreDataPairSplitFloat(tmp, 4, src, srcLen);
    if(errPos != zfnull)
    {
        return errPos;
    }
    ret.controlPointSet(tmp[0], tmp[1], tmp[2], tmp[3]);
    return zfnull;
}

// ============================================================
ZFVAR_CONVERT_WRAPPER_DEFINE(ZFBezier)
ZFPROPERTY_TYPE_DEFINE(ZFBezier, ZFBezier)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_FROM_DEFINE(ZFBezier, ZFBezier)
{
    if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZFBezier, serializableData, outErrorHintToAppend, outErrorPos) == zfnull)
    {
        return zffalse;
    }
    const zfchar *element = zfnull;
    zffloat p0x = 0;
    zffloat p0y = 0;
    zffloat p1x = 1;
    zffloat p1y = 1;

    element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFBezier_p0x);
    if(element != zfnull && zffloatFromString(p0x, element) != zfnull)
    {
        ZFSerializableUtil::errorOccurredWhile(outErrorHintToAppend, outErrorPos, serializableData, ZFSerializableKeyword_ZFBezier_p0x, element);
        return zffalse;
    }

    element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFBezier_p0y);
    if(element != zfnull && zffloatFromString(p0y, element) != zfnull)
    {
        ZFSerializableUtil::errorOccurredWhile(outErrorHintToAppend, outErrorPos, serializableData, ZFSerializableKeyword_ZFBezier_p0y, element);
        return zffalse;
    }

    element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFBezier_p1x);
    if(element != zfnull && zffloatFromString(p1x, element) != zfnull)
    {
        ZFSerializableUtil::errorOccurredWhile(outErrorHintToAppend, outErrorPos, serializableData, ZFSerializableKeyword_ZFBezier_p1x, element);
        return zffalse;
    }

    element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFBezier_p1y);
    if(element != zfnull && zffloatFromString(p1y, element) != zfnull)
    {
        ZFSerializableUtil::errorOccurredWhile(outErrorHintToAppend, outErrorPos, serializableData, ZFSerializableKeyword_ZFBezier_p1y, element);
        return zffalse;
    }

    result.controlPointSet(p0x, p0y, p1x, p1y);

    serializableData.resolveMark();
    return zftrue;
}
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_TO_DEFINE(ZFBezier, ZFBezier)
{
    serializableData.itemClassSet(ZFPropertyTypeId_ZFBezier);

    if(v.p0x != 0 || v.p0y != 0)
    {
        serializableData.attributeSet(ZFSerializableKeyword_ZFBezier_p0x, zffloatToString(v.p0x));
        serializableData.attributeSet(ZFSerializableKeyword_ZFBezier_p0y, zffloatToString(v.p0y));
    }

    if(v.p1x != 1 || v.p1y != 1)
    {
        serializableData.attributeSet(ZFSerializableKeyword_ZFBezier_p1x, zffloatToString(v.p1x));
        serializableData.attributeSet(ZFSerializableKeyword_ZFBezier_p1y, zffloatToString(v.p1y));
    }

    return zftrue;
}

// ============================================================
ZFINPUT_TYPE_DEFINE(ZFBezier, ZFBezier, {
    v = ZFBezierZero;
    ZFCoreArrayPOD<zffloat> tmp;
    if(!ZFInputCallbackReadDataPairFloat(tmp, input, 4))
    {
        return zffalse;
    }
    v.controlPointSet(tmp[0], tmp[1], tmp[2], tmp[3]);
    return zftrue;
})

ZF_NAMESPACE_GLOBAL_END

