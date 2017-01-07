/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIViewFocus.h"
#include "ZFUIWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFUIViewFocus_tag_nextFocus zfText("_ZFP_ZFUIViewFocus_tag_nextFocus")
#define _ZFP_ZFUIViewFocus_tag_nextFocusOwner zfText("_ZFP_ZFUIViewFocus_tag_nextFocusOwner")

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewFocusNextSetDataHolder, ZFLevelZFFrameworkNormal)
{
    this->nextFocusOnDeallocListener = ZFCallbackForRawFunction(zfself::nextFocusOnDealloc);
}
public:
    ZFListener nextFocusOnDeallocListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(nextFocusOnDealloc)
    {
        ZFObjectHolder *nextFocusOwner = listenerData.sender->tagGet<ZFObjectHolder *>(_ZFP_ZFUIViewFocus_tag_nextFocusOwner);
        if(nextFocusOwner == zfnull)
        {
            return ;
        }
        nextFocusOwner->tagRemove(_ZFP_ZFUIViewFocus_tag_nextFocus);
        listenerData.sender->observerRemove(ZFObject::EventObjectBeforeDealloc(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewFocusNextSetDataHolder)->nextFocusOnDeallocListener);
        listenerData.sender->tagRemove(_ZFP_ZFUIViewFocus_tag_nextFocusOwner);
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusNextSetDataHolder)

void ZFUIViewFocusNextSet(ZF_IN ZFUIView *from, ZF_IN ZFUIView *nextFocus)
{
    if(from == zfnull)
    {
        return ;
    }

    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewFocusNextSetDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewFocusNextSetDataHolder);

    ZFObjectHolder *nextFocusHolderOld = from->tagGet<ZFObjectHolder *>(_ZFP_ZFUIViewFocus_tag_nextFocus);
    if(nextFocusHolderOld != zfnull)
    {
        nextFocusHolderOld->holdedObj.toObject()->observerRemove(ZFObject::EventObjectBeforeDealloc(), d->nextFocusOnDeallocListener);
        nextFocusHolderOld->holdedObj.toObject()->tagRemove(_ZFP_ZFUIViewFocus_tag_nextFocusOwner);
        from->tagRemove(_ZFP_ZFUIViewFocus_tag_nextFocus);
    }

    if(nextFocus != zfnull)
    {
        from->tagSetMarkCached(_ZFP_ZFUIViewFocus_tag_nextFocus, nextFocus->objectHolder());
        nextFocus->tagSetMarkCached(_ZFP_ZFUIViewFocus_tag_nextFocusOwner, from->objectHolder());
        nextFocus->observerAdd(ZFObject::EventObjectBeforeDealloc(), d->nextFocusOnDeallocListener);
    }
}

void _ZFP_ZFUIViewFocusNextSetChain(ZF_IN ZFUIView *view0, ZF_IN ZFUIView *view1, ...)
{
    ZFUIView *from = view0;
    ZFUIView *to = view1;

    va_list vaList;
    va_start(vaList, view1);
    while(to != _ZFP_ZFUIViewFocusNextSetChainEndPtr)
    {
        ZFUIViewFocusNextSet(from, to);
        from = to;
        to = va_arg(vaList, ZFUIView *);
    }
    va_end(vaList);
}

// ============================================================
ZFFilterForZFObject ZFUIViewFocusNextFilter;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewFocusNextFilterDataHolder, ZFLevelZFFrameworkHigh)
{
    ZFUIViewFocusNextFilter = ZFFilterForZFObject();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusNextFilterDataHolder)

zfclassPOD _ZFP_ZFUIViewFocusData
{
public:
    ZFUIView *view;
    ZFUIPoint center;
};
ZFOUTPUT_TYPE(_ZFP_ZFUIViewFocusData, {
    output
        << v.center
        << zfText(" ") << v.view
    ;
})

static void _ZFP_ZFUIViewFocusNextFind(ZF_OUT ZFCoreArrayPOD<_ZFP_ZFUIViewFocusData> &focusDatas,
                                       ZF_IN ZFUIView *view,
                                       ZF_IN zfbool includingInternalViews,
                                       ZF_IN zfint offsetX,
                                       ZF_IN zfint offsetY)
{
    if(view->viewDelegateForParent() || !ZFUIViewFocusNextFilter.filterCheckActive(view))
    {
        return ;
    }

    if(view->viewFocusable())
    {
        _ZFP_ZFUIViewFocusData focusData;
        focusData.view = view;
        focusData.center = ZFUIRectGetCenter(view->layoutedFrame());
        focusData.center.x += offsetX;
        focusData.center.y += offsetY;
        focusDatas.add(focusData);
    }

    for(zfindex i = 0; i < view->childCount(); ++i)
    {
        _ZFP_ZFUIViewFocusNextFind(
            focusDatas,
            view->childAtIndex(i),
            includingInternalViews,
            offsetX + view->layoutedFrame().point.x,
            offsetY + view->layoutedFrame().point.y);
    }
    if(includingInternalViews)
    {
        ZFCoreArrayPOD<ZFUIView *> viewArray = view->internalBackgroundViewArray();
        for(zfindex i = 0; i < viewArray.count(); ++i)
        {
            _ZFP_ZFUIViewFocusNextFind(
                focusDatas,
                viewArray[i],
                includingInternalViews,
                offsetX + view->layoutedFrame().point.x,
                offsetY + view->layoutedFrame().point.y);
        }
        viewArray = view->internalForegroundViewArray();
        for(zfindex i = 0; i < viewArray.count(); ++i)
        {
            _ZFP_ZFUIViewFocusNextFind(
                focusDatas,
                viewArray[i],
                includingInternalViews,
                offsetX + view->layoutedFrame().point.x,
                offsetY + view->layoutedFrame().point.y);
        }
    }
}

static ZFCompareResult _ZFP_ZFUIViewFocusNextFind_comparer_tab(ZF_IN _ZFP_ZFUIViewFocusData const &v0, ZF_IN _ZFP_ZFUIViewFocusData const &v1)
{
    if(v0.center.y < v1.center.y)
    {
        return ZFCompareSmaller;
    }
    else if(v0.center.y > v1.center.y)
    {
        return ZFCompareGreater;
    }
    else
    {
        return ZFComparerDefault(v0.center.x, v1.center.x);
    }
}
static ZFCompareResult _ZFP_ZFUIViewFocusNextFind_comparer_x(ZF_IN _ZFP_ZFUIViewFocusData const &v0, ZF_IN _ZFP_ZFUIViewFocusData const &v1)
{
    if(v0.center.x < v1.center.x)
    {
        return ZFCompareSmaller;
    }
    else if(v0.center.x > v1.center.x)
    {
        return ZFCompareGreater;
    }
    else
    {
        return ZFComparerDefault(v0.center.y, v1.center.y);
    }
}
static ZFCompareResult _ZFP_ZFUIViewFocusNextFind_comparer_y(ZF_IN _ZFP_ZFUIViewFocusData const &v0, ZF_IN _ZFP_ZFUIViewFocusData const &v1)
{
    if(v0.center.y < v1.center.y)
    {
        return ZFCompareSmaller;
    }
    else if(v0.center.y > v1.center.y)
    {
        return ZFCompareGreater;
    }
    else
    {
        return ZFComparerDefault(v0.center.x, v1.center.x);
    }
}
static ZFCompareResult _ZFP_ZFUIViewFocusNextFind_comparerForFind(ZF_IN _ZFP_ZFUIViewFocusData const &v0, ZF_IN ZFUIView * const &v1)
{
    if(v0.view == v1)
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareUncomparable;
    }
}

#define _ZFP_ZFUIViewFocusRatioFix 2
void _ZFP_ZFUIViewFocusDataRemoveForX(ZF_IN ZFCoreArrayPOD<_ZFP_ZFUIViewFocusData> &focusDatas,
                                      ZF_IN ZFUIView *refView,
                                      ZF_IN const ZFUIPoint &refCenter)
{
    for(zfindex i = focusDatas.count() - 1; i != zfindexMax; --i)
    {
        if(focusDatas[i].center.x == refCenter.x
            || zfmAbs(focusDatas[i].center.y - refCenter.y) * _ZFP_ZFUIViewFocusRatioFix > zfmAbs(focusDatas[i].center.x - refCenter.x))
        {
            if(focusDatas[i].view != refView)
            {
                focusDatas.remove(i);
            }
            continue;
        }
        if(i > 0 && focusDatas[i].center.x == focusDatas[i - 1].center.x)
        {
            if(zfmAbs(focusDatas[i].center.y - refCenter.y) <= zfmAbs(focusDatas[i - 1].center.y - refCenter.y))
            {
                focusDatas.remove(i - 1);
            }
            else
            {
                focusDatas.remove(i);
            }
        }
    }
}
void _ZFP_ZFUIViewFocusDataRemoveForY(ZF_IN ZFCoreArrayPOD<_ZFP_ZFUIViewFocusData> &focusDatas,
                                      ZF_IN ZFUIView *refView,
                                      ZF_IN const ZFUIPoint &refCenter)
{
    for(zfindex i = focusDatas.count() - 1; i != zfindexMax; --i)
    {
        if(focusDatas[i].center.y == refCenter.y
            || zfmAbs(focusDatas[i].center.x - refCenter.x) > zfmAbs(focusDatas[i].center.y - refCenter.y) * _ZFP_ZFUIViewFocusRatioFix)
        {
            if(focusDatas[i].view != refView)
            {
                focusDatas.remove(i);
            }
            continue;
        }
        if(i > 0 && focusDatas[i].center.y == focusDatas[i - 1].center.y)
        {
            if(zfmAbs(focusDatas[i].center.x - refCenter.x) <= zfmAbs(focusDatas[i - 1].center.x - refCenter.x))
            {
                focusDatas.remove(i - 1);
            }
            else
            {
                focusDatas.remove(i);
            }
        }
    }
}

ZFUIView *_ZFP_ZFUIViewFocusDataFindNext(ZF_IN const ZFCoreArrayPOD<_ZFP_ZFUIViewFocusData> &focusDatas,
                                         ZF_IN zfbool ascending,
                                         ZF_IN ZFUIView *view)
{
    if(ascending)
    {
        zfindex index = focusDatas.find(view, _ZFP_ZFUIViewFocusNextFind_comparerForFind);
        if(index == zfindexMax)
        {
            return (focusDatas.isEmpty() ? zfnull : focusDatas[0].view);
        }
        if(index != focusDatas.count() - 1)
        {
            return focusDatas[index + 1].view;
        }
        return zfnull;
    }
    else
    {
        zfindex index = focusDatas.find(view, _ZFP_ZFUIViewFocusNextFind_comparerForFind);
        if(index == zfindexMax)
        {
            return (focusDatas.isEmpty() ? zfnull : focusDatas[focusDatas.count() - 1].view);
        }
        if(index != 0)
        {
            return focusDatas[index - 1].view;
        }
        return zfnull;
    }
}
ZFUIView *_ZFP_ZFUIViewFocusDataFindFirst(ZF_IN const ZFCoreArrayPOD<_ZFP_ZFUIViewFocusData> &focusDatas,
                                          ZF_IN zfbool ascending,
                                          ZF_IN ZFUIView *view)
{
    if(ascending)
    {
        zfindex index = focusDatas.find(view, _ZFP_ZFUIViewFocusNextFind_comparerForFind);
        if(index == zfindexMax)
        {
            return (focusDatas.isEmpty() ? zfnull : focusDatas[0].view);
        }
        if(index != 0)
        {
            return focusDatas[0].view;
        }
        return zfnull;
    }
    else
    {
        zfindex index = focusDatas.find(view, _ZFP_ZFUIViewFocusNextFind_comparerForFind);
        if(index == zfindexMax)
        {
            return (focusDatas.isEmpty() ? zfnull : focusDatas[focusDatas.count() - 1].view);
        }
        if(index != focusDatas.count() - 1)
        {
            return focusDatas[focusDatas.count() - 1].view;
        }
        return zfnull;
    }
}

// ============================================================
ZFUIView *ZFUIViewFocusNextFind(ZF_IN ZFUIView *view,
                                ZF_IN_OPT const ZFUIViewFocusNextParam &param /* = ZFUIViewFocusNextParam() */)
{
    if(view == zfnull)
    {
        return zfnull;
    }

    {
        ZFObjectHolder *t = view->tagGet<ZFObjectHolder *>(_ZFP_ZFUIViewFocus_tag_nextFocus);
        if(t != zfnull)
        {
            return t->holdedObj;
        }
    }

    ZFUIView *root = view;
    ZFUIPoint viewCenter = ZFUIRectGetCenter(view->layoutedFrame());
    while(root->viewParentVirtual() != zfnull
        && !root->classData()->classIsSubclassOf(ZFUIWindow::ClassData())
        && root != param.focusEndParent())
    {
        viewCenter.x += root->layoutedFrame().point.x;
        viewCenter.y += root->layoutedFrame().point.y;
        root = root->viewParentVirtual();
    }
    ZFCoreArrayPOD<_ZFP_ZFUIViewFocusData> focusDatas;
    _ZFP_ZFUIViewFocusNextFind(focusDatas, root, param.focusInternalViews(), 0, 0);

    ZFUIView *ret = zfnull;
    if(ZFBitTest(param.focusDirection(), ZFUIOrientation::e_Right) && ZFBitTest(param.focusDirection(), ZFUIOrientation::e_Bottom))
    { // tab
        focusDatas.sort(_ZFP_ZFUIViewFocusNextFind_comparer_tab);
        ret = _ZFP_ZFUIViewFocusDataFindNext(focusDatas, zftrue, view);
        if(ret != zfnull)
        {
            return ret;
        }
        return _ZFP_ZFUIViewFocusDataFindFirst(focusDatas, zftrue, view);
    }
    if(ZFBitTest(param.focusDirection(), ZFUIOrientation::e_Left) && ZFBitTest(param.focusDirection(), ZFUIOrientation::e_Top))
    { // shift tab
        focusDatas.sort(_ZFP_ZFUIViewFocusNextFind_comparer_tab);
        ret = _ZFP_ZFUIViewFocusDataFindNext(focusDatas, zffalse, view);
        if(ret != zfnull)
        {
            return ret;
        }
        return _ZFP_ZFUIViewFocusDataFindFirst(focusDatas, zffalse, view);
    }

    ZFCoreArrayPOD<_ZFP_ZFUIViewFocusData> focusDatasX = focusDatas;
    ZFCoreArrayPOD<_ZFP_ZFUIViewFocusData> focusDatasY;
    focusDatasY.copyFrom(focusDatas);

    focusDatasX.sort(_ZFP_ZFUIViewFocusNextFind_comparer_x);
    _ZFP_ZFUIViewFocusDataRemoveForX(focusDatasX, view, viewCenter);

    focusDatasY.sort(_ZFP_ZFUIViewFocusNextFind_comparer_y);
    _ZFP_ZFUIViewFocusDataRemoveForY(focusDatasY, view, viewCenter);

    if(ZFBitTest(param.focusDirection(), ZFUIOrientation::e_Right))
    {
        ret = _ZFP_ZFUIViewFocusDataFindNext(focusDatasX, zftrue, view);
        if(ret != zfnull)
        {
            return ret;
        }
    }
    if(ZFBitTest(param.focusDirection(), ZFUIOrientation::e_Bottom))
    {
        ret = _ZFP_ZFUIViewFocusDataFindNext(focusDatasY, zftrue, view);
        if(ret != zfnull)
        {
            return ret;
        }
    }
    if(ZFBitTest(param.focusDirection(), ZFUIOrientation::e_Left))
    {
        ret = _ZFP_ZFUIViewFocusDataFindNext(focusDatasX, zffalse, view);
        if(ret != zfnull)
        {
            return ret;
        }
    }
    if(ZFBitTest(param.focusDirection(), ZFUIOrientation::e_Top))
    {
        ret = _ZFP_ZFUIViewFocusDataFindNext(focusDatasY, zffalse, view);
        if(ret != zfnull)
        {
            return ret;
        }
    }

    if(param.focusLoopMode())
    {
        if(ZFBitTest(param.focusDirection(), ZFUIOrientation::e_Right))
        {
            ret = _ZFP_ZFUIViewFocusDataFindFirst(focusDatasX, zftrue, view);
            if(ret != zfnull)
            {
                return ret;
            }
        }
        if(ZFBitTest(param.focusDirection(), ZFUIOrientation::e_Bottom))
        {
            ret = _ZFP_ZFUIViewFocusDataFindFirst(focusDatasY, zftrue, view);
            if(ret != zfnull)
            {
                return ret;
            }
        }
        if(ZFBitTest(param.focusDirection(), ZFUIOrientation::e_Left))
        {
            ret = _ZFP_ZFUIViewFocusDataFindFirst(focusDatasX, zffalse, view);
            if(ret != zfnull)
            {
                return ret;
            }
        }
        if(ZFBitTest(param.focusDirection(), ZFUIOrientation::e_Top))
        {
            ret = _ZFP_ZFUIViewFocusDataFindFirst(focusDatasY, zffalse, view);
            if(ret != zfnull)
            {
                return ret;
            }
        }
    }

    return zfnull;
}

ZFUIView *ZFUIViewFocusNextMove(ZF_IN ZFUIView *view,
                                ZF_IN_OPT const ZFUIViewFocusNextParam &param /* = ZFUIViewFocusNextParam() */)
{
    ZFUIView *next = ZFUIViewFocusNextFind(view, param);
    if(next != zfnull)
    {
        next->viewFocusRequest(zftrue);
    }
    return next;
}

// ============================================================
static zfbool _ZFP_ZFUIViewFocusResolveKeyEvent_shiftPressed = zffalse;
zfbool ZFUIViewFocusResolveKeyEvent(ZF_IN ZFUIView *view,
                                    ZF_IN ZFUIKeyEvent *keyEvent,
                                    ZF_OUT_OPT ZFUIView **nextFocus /* = zfnull */,
                                    ZF_IN_OPT ZFUIView *endParent /* = zfnull */)
{
    if(nextFocus != zfnull)
    {
        *nextFocus = zfnull;
    }
    ZFUIViewFocusNextParam param;
    param.focusEndParentSet(endParent);
    switch(keyEvent->keyCode)
    {
        case ZFUIKeyCode::e_kLeft:
            param.focusDirectionSet(ZFUIOrientation::e_Left);
            break;
        case ZFUIKeyCode::e_kUp:
            param.focusDirectionSet(ZFUIOrientation::e_Top);
            break;
        case ZFUIKeyCode::e_kRight:
            param.focusDirectionSet(ZFUIOrientation::e_Right);
            break;
        case ZFUIKeyCode::e_kDown:
            param.focusDirectionSet(ZFUIOrientation::e_Bottom);
            break;
        case ZFUIKeyCode::e_kTab:
        {
            param.focusLoopModeSet(zftrue);
            if(_ZFP_ZFUIViewFocusResolveKeyEvent_shiftPressed)
            {
                param.focusDirectionSet(ZFUIOrientation::e_Left | ZFUIOrientation::e_Top);
            }
            else
            {
                param.focusDirectionSet(ZFUIOrientation::e_Right | ZFUIOrientation::e_Bottom);
            }
        }
            break;
        case ZFUIKeyCode::e_kShift:
            switch(keyEvent->keyAction)
            {
                case ZFUIKeyAction::e_KeyDown:
                case ZFUIKeyAction::e_KeyRepeat:
                    _ZFP_ZFUIViewFocusResolveKeyEvent_shiftPressed = zftrue;
                    break;
                case ZFUIKeyAction::e_KeyUp:
                case ZFUIKeyAction::e_KeyCancel:
                    _ZFP_ZFUIViewFocusResolveKeyEvent_shiftPressed = zffalse;
                    break;
            }
            keyEvent->eventResolvedSet(zftrue);
            return zffalse;
        default:
            return zffalse;
    }
    keyEvent->eventResolvedSet(zftrue);
    if(keyEvent->keyAction != ZFUIKeyAction::e_KeyRepeat && keyEvent->keyAction != ZFUIKeyAction::e_KeyUp)
    {
        return zftrue;
    }
    ZFUIView *next = ZFUIViewFocusNextMove(view, param);
    if(nextFocus != zfnull)
    {
        *nextFocus = next;
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

