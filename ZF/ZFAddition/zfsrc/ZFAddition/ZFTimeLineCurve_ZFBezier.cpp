/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFTimeLineCurve_ZFBezier.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFTimeLineCurveBezier)

ZFOBJECT_REGISTER(ZFTimeLineCurveEaseIn)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFTimeLineCurveEaseIn, ZFTimeLineCurveEaseIn, ZFTimeLineCurveEaseIn, instance, ZFLevelZFFrameworkNormal)

ZFOBJECT_REGISTER(ZFTimeLineCurveEaseOut)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFTimeLineCurveEaseOut, ZFTimeLineCurveEaseOut, ZFTimeLineCurveEaseOut, instance, ZFLevelZFFrameworkNormal)

ZFOBJECT_REGISTER(ZFTimeLineCurveEaseInOut)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFTimeLineCurveEaseInOut, ZFTimeLineCurveEaseInOut, ZFTimeLineCurveEaseInOut, instance, ZFLevelZFFrameworkNormal)

ZFOBJECT_REGISTER(ZFTimeLineCurveBounceIn)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFTimeLineCurveBounceIn, ZFTimeLineCurveBounceIn, ZFTimeLineCurveBounceIn, instance, ZFLevelZFFrameworkNormal)

ZFOBJECT_REGISTER(ZFTimeLineCurveBounceOut)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFTimeLineCurveBounceOut, ZFTimeLineCurveBounceOut, ZFTimeLineCurveBounceOut, instance, ZFLevelZFFrameworkNormal)

ZFOBJECT_REGISTER(ZFTimeLineCurveBounceInOut)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFTimeLineCurveBounceInOut, ZFTimeLineCurveBounceInOut, ZFTimeLineCurveBounceInOut, instance, ZFLevelZFFrameworkNormal)

ZF_NAMESPACE_GLOBAL_END

