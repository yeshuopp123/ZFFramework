/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIViewUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFUIViewUtil)

ZFUIView *viewRoot(ZF_IN ZFUIView *view)
{
    if(view != zfnull)
    {
        while(view->viewParent() != zfnull)
        {
            view = view->viewParent();
        }
    }
    return view;
}

zfbool viewIsChildOf(ZF_IN ZFUIView *view, ZF_IN ZFUIView *parentToCheck)
{
    if(view != zfnull)
    {
        ZFUIView *tmp = parentToCheck;
        do
        {
            if(tmp == view)
            {
                return zftrue;
            }
            tmp = tmp->viewParentVirtual();
        } while(tmp != zfnull);
    }
    return zffalse;
}

ZFUIView *viewChildAt(ZF_IN ZFUIView *view,
                      ZF_IN const ZFUIPoint &pos,
                      ZF_IN_OPT zfbool filterDisabledView /* = zftrue */,
                      ZF_IN_OPT const ZFFilterForZFObject *filter /* = zfnull */)
{
    if(view == zfnull
        || (filterDisabledView && !view->viewUIEnableTree())
        || (filter != zfnull && !filter->filterCheckActive(view)))
    {
        return zfnull;
    }
    if(pos.x < 0 || pos.y < 0 || pos.x > view->layoutedFrame().size.width || pos.y > view->layoutedFrame().size.height)
    {
        return zfnull;
    }

    ZFUIRect layoutedFrameFixed = ZFUIRectZero;

    ZFCoreArrayPOD<ZFUIView *> childList = view->internalForegroundViewArray();
    for(zfindex i = childList.count() - 1; i != zfindexMax; --i)
    {
        ZFUIView *child = childList[i];
        child->layoutedFrameFixed(layoutedFrameFixed);
        ZFUIView *tmp = ZFUIViewUtil::viewChildAt(childList[i], ZFUIPointMake(
                pos.x - layoutedFrameFixed.point.x,
                pos.y - layoutedFrameFixed.point.y
            ),
            filterDisabledView,
            filter);
        if(tmp != zfnull)
        {
            return tmp;
        }
    }

    childList = view->childArray();
    for(zfindex i = childList.count() - 1; i != zfindexMax; --i)
    {
        ZFUIView *child = childList[i];
        child->layoutedFrameFixed(layoutedFrameFixed);
        ZFUIView *tmp = ZFUIViewUtil::viewChildAt(childList[i], ZFUIPointMake(
                pos.x - layoutedFrameFixed.point.x,
                pos.y - layoutedFrameFixed.point.y
            ),
            filterDisabledView,
            filter);
        if(tmp != zfnull)
        {
            return tmp;
        }
    }

    childList = view->internalBackgroundViewArray();
    for(zfindex i = childList.count() - 1; i != zfindexMax; --i)
    {
        ZFUIView *child = childList[i];
        child->layoutedFrameFixed(layoutedFrameFixed);
        ZFUIView *tmp = ZFUIViewUtil::viewChildAt(childList[i], ZFUIPointMake(
                pos.x - layoutedFrameFixed.point.x,
                pos.y - layoutedFrameFixed.point.y
            ),
            filterDisabledView,
            filter);
        if(tmp != zfnull)
        {
            return tmp;
        }
    }

    if(filterDisabledView && !view->viewUIEnable())
    {
        return zfnull;
    }

    return view;
}

void viewRectToParent(ZF_OUT ZFUIRect &rect, ZF_IN ZFUIView *view, ZF_IN ZFUIView *parent)
{
    if(view == zfnull || parent == zfnull)
    {
        rect = ZFUIRectZero;
        return ;
    }
    view->layoutedFrameFixed(rect);
    ZFUIRect layoutedFrameFixed = ZFUIRectZero;
    while(view->viewParent() != zfnull && view != parent)
    {
        view = view->viewParent();
        view->layoutedFrameFixed(layoutedFrameFixed);
        rect.point.x += layoutedFrameFixed.point.x;
        rect.point.y += layoutedFrameFixed.point.y;
    }
    if(view != parent)
    {
        rect = ZFUIRectZero;
    }
}

ZF_NAMESPACE_END(ZFUIViewUtil)
ZF_NAMESPACE_GLOBAL_END

