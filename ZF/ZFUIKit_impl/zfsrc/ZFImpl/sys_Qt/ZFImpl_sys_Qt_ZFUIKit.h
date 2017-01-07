/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZFImpl_sys_Qt_ZFUIKit_h_
#define _ZFI_ZFImpl_sys_Qt_ZFUIKit_h_

#include "ZFImpl.h"

#include "ZFImpl/sys_Qt/ZFImpl_sys_Qt_ZF.h"
#include "ZFUIKit/ZFUITypeDef.h"

#if ZF_ENV_sys_Qt
#include <QPoint>
#include <QSize>
#include <QRect>
#include <QColor>
#include <QWidget>

ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_QWidgetGetViewInfo(ZF_OUT zfstring &ret, ZF_IN QWidget *view);
inline zfstring ZFImpl_sys_Qt_ZFUIKit_QWidgetGetViewInfo(ZF_IN QWidget *view)
{
    zfstring ret;
    ZFImpl_sys_Qt_ZFUIKit_QWidgetGetViewInfo(ret, view);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_QWidgetGetViewTree(ZF_OUT zfstring &ret, ZF_IN QWidget *view);
inline zfstring ZFImpl_sys_Qt_ZFUIKit_QWidgetGetViewTree(ZF_IN QWidget *view)
{
    zfstring ret;
    ZFImpl_sys_Qt_ZFUIKit_QWidgetGetViewTree(ret, view);
    return ret;
}

// convert utility
extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_ZFUIPointToQPoint(ZF_OUT QPoint &ret, ZF_IN const ZFUIPoint &point);
inline QPoint ZFImpl_sys_Qt_ZFUIKit_ZFUIPointToQPoint(ZF_IN const ZFUIPoint &point)
{
    QPoint ret;
    ZFImpl_sys_Qt_ZFUIKit_ZFUIPointToQPoint(ret, point);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_ZFUIPointFromQPoint(ZF_OUT ZFUIPoint &ret, ZF_IN const QPoint &qPoint);
inline ZFUIPoint ZFImpl_sys_Qt_ZFUIKit_ZFUIPointFromQPoint(ZF_IN const QPoint &qPoint)
{
    ZFUIPoint ret = ZFUIPointZero;
    ZFImpl_sys_Qt_ZFUIKit_ZFUIPointFromQPoint(ret, qPoint);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_ZFUISizeToQSize(ZF_OUT QSize &ret, ZF_IN const ZFUISize &size);
inline QSize ZFImpl_sys_Qt_ZFUIKit_ZFUISizeToQSize(ZF_IN const ZFUISize &size)
{
    QSize ret;
    ZFImpl_sys_Qt_ZFUIKit_ZFUISizeToQSize(ret, size);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_ZFUISizeFromQSize(ZF_OUT ZFUISize &ret, ZF_IN const QSize &qSize);
inline ZFUISize ZFImpl_sys_Qt_ZFUIKit_ZFUISizeFromQSize(ZF_IN const QSize &qSize)
{
    ZFUISize ret = ZFUISizeZero;
    ZFImpl_sys_Qt_ZFUIKit_ZFUISizeFromQSize(ret, qSize);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_ZFUIRectToQRect(ZF_OUT QRect &ret, ZF_IN const ZFUIRect &rect);
inline QRect ZFImpl_sys_Qt_ZFUIKit_ZFUIRectToQRect(ZF_IN const ZFUIRect &rect)
{
    QRect ret;
    ZFImpl_sys_Qt_ZFUIKit_ZFUIRectToQRect(ret, rect);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_ZFUIRectFromQRect(ZF_OUT ZFUIRect &ret, ZF_IN const QRect &qRect);
inline ZFUIRect ZFImpl_sys_Qt_ZFUIKit_ZFUIRectFromQRect(ZF_IN const QRect &qRect)
{
    ZFUIRect ret = ZFUIRectZero;
    ZFImpl_sys_Qt_ZFUIKit_ZFUIRectFromQRect(ret, qRect);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_ZFUIColorToQColor(ZF_IN QColor &ret, ZF_IN const ZFUIColor &color);
inline QColor ZFImpl_sys_Qt_ZFUIKit_ZFUIColorToQColor(ZF_IN const ZFUIColor &color)
{
    QColor ret;
    ZFImpl_sys_Qt_ZFUIKit_ZFUIColorToQColor(ret, color);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_ZFUIColorFromQColor(ZF_OUT ZFUIColor &ret, ZF_IN const QColor qColor);
inline ZFUIColor ZFImpl_sys_Qt_ZFUIKit_ZFUIColorFromQColor(ZF_IN const QColor qColor)
{
    ZFUIColor ret = ZFUIColorZero;
    ZFImpl_sys_Qt_ZFUIKit_ZFUIColorFromQColor(ret, qColor);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_ZFUIAlignFlagsToQAlignment(ZF_OUT Qt::Alignment &ret, ZF_IN const ZFUIAlignFlags &align);
inline Qt::Alignment ZFImpl_sys_Qt_ZFUIKit_ZFUIAlignFlagsToQAlignment(ZF_IN const ZFUIAlignFlags &align)
{
    Qt::Alignment ret = 0;
    ZFImpl_sys_Qt_ZFUIKit_ZFUIAlignFlagsToQAlignment(ret, align);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_ZFUIAlignFlagsFromQAlignment(ZF_OUT ZFUIAlignFlags &ret, ZF_IN const Qt::Alignment qAlign);
inline ZFUIAlignFlags ZFImpl_sys_Qt_ZFUIKit_ZFUIAlignFlagsFromQAlignment(ZF_IN const Qt::Alignment qAlign)
{
    ZFUIAlignFlags ret = 0;
    ZFImpl_sys_Qt_ZFUIKit_ZFUIAlignFlagsFromQAlignment(ret, qAlign);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Qt
#endif // #ifndef _ZFI_ZFImpl_sys_Qt_ZFUIKit_h_

