/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUILayoutParam.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUISizeType)

// ============================================================
// ZFUISizeParam
ZFVAR_CONVERT_WRAPPER_DEFINE(ZFUISizeParam)

void ZFUISizeParamToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFUISizeParam const &value)
{
    ret += zfText("(");
    ZFUISizeTypeEnumToString(ret, value.width);
    ret += zfText(", ");
    ZFUISizeTypeEnumToString(ret, value.height);
    ret += zfText(")");
}
const zfchar *ZFUISizeParamFromString(ZF_OUT ZFUISizeParam &ret,
                                      ZF_IN const zfchar *src,
                                      ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    zfCoreAssert(src != zfnull);
    ZFCoreArrayPOD<zfindexRange> pos;
    const zfchar *errPos = zfCoreDataPairSplitString(pos, 2, src, srcLen);
    if(errPos != zfnull)
    {
        return errPos;
    }

    errPos = ZFUISizeTypeEnumFromString(ret.width, src + pos[0].start, pos[0].count);
    if(errPos != zfnull)
    {
        return errPos;
    }

    errPos = ZFUISizeTypeEnumFromString(ret.height, src + pos[1].start, pos[1].count);
    if(errPos != zfnull)
    {
        return errPos;
    }

    return zfnull;
}

// ============================================================
ZFPROPERTY_TYPE_DEFINE_BY_STRING_CONVERTER(ZFUISizeParam, ZFUISizeParam)

const ZFUISizeParam ZFUISizeParamWrapWidthWrapHeight = ZFUISizeParamMake(ZFUISizeType::e_Wrap, ZFUISizeType::e_Wrap);
const ZFUISizeParam ZFUISizeParamFillWidthWrapHeight = ZFUISizeParamMake(ZFUISizeType::e_Fill, ZFUISizeType::e_Wrap);
const ZFUISizeParam ZFUISizeParamWrapWidthFillHeight = ZFUISizeParamMake(ZFUISizeType::e_Wrap, ZFUISizeType::e_Fill);
const ZFUISizeParam ZFUISizeParamFillWidthFillHeight = ZFUISizeParamMake(ZFUISizeType::e_Fill, ZFUISizeType::e_Fill);

// ============================================================

ZFOUTPUT_TYPE_DEFINE(ZFUISizeParam, {output << ZFUISizeParamToString(v);})
ZFINPUT_TYPE_DEFINE(ZFUISizeParam, ZFUISizeParam, {
    v = ZFUISizeParamWrapWidthWrapHeight;
    ZFCoreArray<zfstring> elementsTmp;
    if(!ZFInputCallbackReadDataPair(elementsTmp, input, 2))
    {
        return zffalse;
    }
    if(ZFUISizeTypeEnumFromString(v.width, elementsTmp[0]) != zfnull)
    {
        return zffalse;
    }
    if(ZFUISizeTypeEnumFromString(v.height, elementsTmp[1]) != zfnull)
    {
        return zffalse;
    }
    return zftrue;
})

// ============================================================
ZFOBJECT_REGISTER(ZFUILayoutParam)

ZFOBSERVER_EVENT_REGISTER(ZFUILayoutParam, LayoutParamOnChange)

ZF_NAMESPACE_GLOBAL_END

