/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTimeLineCurve_ZFBezier.h
 * @brief see #ZFTimeLineCurve
 */

#ifndef _ZFI_ZFTimeLineCurve_ZFBezier_h_
#define _ZFI_ZFTimeLineCurve_ZFBezier_h_

#include "ZFAdditionDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief bezier time line, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveBezier : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveBezier, ZFTimeLineCurve)

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuper::copyableOnCopyFrom(anotherObj);
        this->bezierSet(ZFCastZFObjectUnchecked(zfself *, anotherObj)->bezier());
    }

public:
    /** @brief bezier value */
    ZFPROPERTY_ASSIGN(ZFBezier, bezier)

public:
    /**
     * @brief init with bezier
     */
    virtual ZFObject *objectOnInit(ZF_IN const ZFBezier &bezier)
    {
        this->objectOnInit();
        this->bezierSet(bezier);
        return this;
    }
    /**
     * @brief init with bezier control point
     */
    virtual ZFObject *objectOnInit(ZF_IN zffloat p0x, ZF_IN zffloat p0y,
                                   ZF_IN zffloat p1x, ZF_IN zffloat p1y)
    {
        this->objectOnInit();
        this->bezierSet(ZFBezier(p0x, p0y, p1x, p1y));
        return this;
    }
public:
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        return zfsuper::objectOnInit();
    }

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return this->bezier().x_by_y(time);
    }
};

// ============================================================
/**
 * @brief slow to fast, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveEaseIn : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveEaseIn, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

public:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFBezierEaseIn.x_by_y(time);
    }
};

// ============================================================
/**
 * @brief fast to slow, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveEaseOut : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveEaseOut, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

public:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFBezierEaseOut.x_by_y(time);
    }
};

// ============================================================
/**
 * @brief slow to fast to slow, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveEaseInOut : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveEaseInOut, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

public:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFBezierEaseInOut.x_by_y(time);
    }
};

// ============================================================
/**
 * @brief bounce to fast, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveBounceIn : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveBounceIn, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

public:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFBezierBounceIn.x_by_y(time);
    }
};

// ============================================================
/**
 * @brief fast to bounce, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveBounceOut : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveBounceOut, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

public:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFBezierBounceOut.x_by_y(time);
    }
};

// ============================================================
/**
 * @brief bounce to fast to bounce, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveBounceInOut : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveBounceInOut, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

public:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFBezierBounceInOut.x_by_y(time);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLineCurve_ZFBezier_h_

