/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIView.h"
#include "protocol/ZFProtocolZFUIView.h"
#include "protocol/ZFProtocolZFUIViewFocus.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIView)

// ============================================================
// attached listeners
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewListenerHolder, ZFLevelZFFrameworkNormal)
{
    this->layoutParamChangedListener = ZFCallbackForRawFunction(zfself::layoutParamChanged);
    this->viewPropertyOnUpdateListener = ZFCallbackForRawFunction(zfself::viewPropertyOnUpdate);
}
public:
    ZFListener layoutParamChangedListener;
    ZFListener viewPropertyOnUpdateListener;
private:
    static ZFLISTENER_PROTOTYPE_EXPAND(layoutParamChanged)
    {
        userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIView *>()->layoutRequest();
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(viewPropertyOnUpdate)
    {
        userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIView *>()->_ZFP_ZFUIView_viewPropertyNotifyUpdate();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewListenerHolder)

// ============================================================
// _ZFP_ZFUIViewPrivate
static zfindex _ZFP_ZFUIView_layoutRequestOverrideFlag = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewLayoutRequestOverrideInit, ZFLevelZFFrameworkHigh)
{
    _ZFP_ZFUIView_layoutRequestOverrideFlag = 0;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewLayoutRequestOverrideInit)

zfclassLikePOD _ZFP_ZFUIViewLayerData
{
public:
    ZFCoreArrayPOD<ZFUIView *> views;
};
typedef zfstlmap<zfstlstringZ, zfbool> _ZFP_ZFUIViewInternalViewAutoSerializeTagMapType;
zfclassNotPOD _ZFP_ZFUIViewPrivate
{
public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *impl;
    ZFUIView *pimplOwner;
    void *nativeView;
    void *nativeImplView;
    ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback;
    ZFUIMargin nativeImplViewMargin;

    zfbool viewDelegateForParent;
    ZFUIView *viewDelegateParent;
    ZFUIView *viewDelegate;

    ZFUIView *viewParent;
    ZFUIViewChildLayerEnum viewLayer;
    ZFUIViewLayoutParam *layoutParam; // retain
    ZFUIViewLayoutParam *serializableRefLayoutParam; // retain

    _ZFP_ZFUIViewInternalViewAutoSerializeTagMapType internalViewAutoSerializeTags;

    // scale visible to app, 1 by default
    // used for app to manage scale, set by ZFUIRootView
    zffloat scaleForApp;
    // impl's scale, ZFUIViewImpl::nativeViewScaleForImpl by default
    // this value depends on impl's screen's scale, may have different value on different screen,
    // would be changed by impl while adding to native view
    zffloat scaleForImpl;
    // final scale value, scaleFixed = scaleForApp * scaleForImpl
    // (sizeInZFFramework * scaleFixed) would be the final pixel size that passed to implementation
    zffloat scaleFixed;
    /*
     * here's a memo for the scale logic:
     *
     * while adding to parent,
     * scaleForApp, scaleForImpl and scaleFixed would be automatically changed to parent's setting (during viewOnAddToParent)
     *
     * while adding to native view,
     * only scaleForImpl would be automatically changed to impl's scale (during nativeViewScaleForImpl),
     * but app's scale would be kept
     *
     * ZFUISysWindow would have its ZFUIRootView's scale setting set from ZFUISysWindow's implementation
     *
     * to change scale, app would use ZFUIRootView from ZFUISysWindow
     *
     * scaleOnChange would be called each time scaleFixed changed
     */

    ZFUISize lastMeasuredSize;
    ZFUIViewMeasureResult *viewLayoutOnMeasureFinishParamCache;
    ZFUISize lastMeasuredSizeHint;
    ZFUISizeParam lastMeasuredSizeParam;
    zfbool layoutRequested;
    zfbool layouting;
    zfindex layoutRequestOverrideFlag;
    ZFUIRect layoutedFramePrev;
    zfbool layoutedFramePrevResetFlag;
    ZFUIRect layoutedFrame;
    _ZFP_ZFUIViewLayerData layerInternalImpl;
    _ZFP_ZFUIViewLayerData layerInternalBg;
    _ZFP_ZFUIViewLayerData layerNormal;
    _ZFP_ZFUIViewLayerData layerInternalFg;

public:
    _ZFP_ZFUIViewPrivate(void)
    : impl(ZFPROTOCOL_ACCESS(ZFUIView))
    , pimplOwner(zfnull)
    , nativeView(zfnull)
    , nativeImplView(zfnull)
    , nativeImplViewDeleteCallback(zfnull)
    , nativeImplViewMargin(ZFUIMarginZero)
    , viewDelegateForParent(zffalse)
    , viewDelegateParent(zfnull)
    , viewDelegate(zfnull)
    , viewParent(zfnull)
    , viewLayer(ZFUIViewChildLayer::e_Normal)
    , layoutParam(zfnull)
    , serializableRefLayoutParam(zfnull)
    , internalViewAutoSerializeTags()
    , scaleForApp(1)
    , scaleForImpl(1)
    , scaleFixed(1)
    , lastMeasuredSize()
    , viewLayoutOnMeasureFinishParamCache(zfnull)
    , lastMeasuredSizeHint()
    , lastMeasuredSizeParam()
    , layoutRequested(zftrue)
    , layouting(zffalse)
    , layoutRequestOverrideFlag(0)
    , layoutedFramePrev(ZFUIRectZero)
    , layoutedFramePrevResetFlag(zffalse)
    , layoutedFrame(ZFUIRectZero)
    , layerInternalImpl()
    , layerInternalBg()
    , layerNormal()
    , layerInternalFg()
    {
    }

public:
    ZFUIView *virtualParent(ZF_IN ZFUIView *parent)
    {
        while(parent->viewDelegateForParent())
        {
            parent = parent->viewParent();
        }
        return parent;
    }
    void layoutParamSet(ZF_IN ZFUIViewLayoutParam *newLayoutParam)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewListenerHolder) *listenerHolder = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewListenerHolder);
        zfRetain(newLayoutParam);
        if(newLayoutParam)
        {
            newLayoutParam->observerAdd(
                ZFUILayoutParam::EventLayoutParamOnChange(),
                listenerHolder->layoutParamChangedListener,
                this->pimplOwner->objectHolder());
        }
        if(this->pimplOwner->objectCached())
        {
            if(newLayoutParam)
            {
                newLayoutParam->objectCachedSet(zftrue);
            }
            if(this->layoutParam)
            {
                this->layoutParam->objectCachedSet(zffalse);
            }
        }
        if(this->layoutParam)
        {
            this->layoutParam->observerRemove(ZFUILayoutParam::EventLayoutParamOnChange(), listenerHolder->layoutParamChangedListener);
        }
        zfRelease(this->layoutParam);
        this->layoutParam = newLayoutParam;
    }
    zfindex viewLayerPrevCount(ZF_IN _ZFP_ZFUIViewLayerData &layer)
    {
        zfindex nativeImplFix = (this->nativeImplView ? 1 : 0);
        if(&layer == &(this->layerNormal))
        {
            return this->layerInternalImpl.views.count() + nativeImplFix
                + this->layerInternalBg.views.count();
        }
        else if(&layer == &(this->layerInternalImpl))
        {
            return 0;
        }
        else if(&layer == &(this->layerInternalBg))
        {
            return this->layerInternalImpl.views.count() + nativeImplFix;
        }
        else if(&layer == &(this->layerInternalFg))
        {
            return this->layerInternalImpl.views.count() + nativeImplFix
                + this->layerInternalBg.views.count()
                + this->layerNormal.views.count();
        }
        else
        {
            zfCoreCriticalShouldNotGoHere();
            return 0;
        }
    }
    void checkUpdateChildScale(ZF_IN ZFUIView *child)
    {
        if(child->scaleGet() != this->pimplOwner->scaleGet()
           || child->scaleGetForImpl() != this->pimplOwner->scaleGetForImpl())
        {
            child->_ZFP_ZFUIView_scaleSetRecursively(this->pimplOwner->scaleGet(), this->pimplOwner->scaleGetForImpl());
        }
    }
    void childAdd(ZF_IN ZFUIViewChildLayerEnum childLayer,
                  ZF_IN _ZFP_ZFUIViewLayerData &layer,
                  ZF_IN ZFUIView *view,
                  ZF_IN ZFUIViewLayoutParam *layoutParam,
                  ZF_IN zfindex atIndex)
    {
        zfCoreAssertWithMessageTrim(!ZFBitTest(this->pimplOwner->objectInstanceState(), ZFObject::ObjectInstanceStateOnDealloc),
            zfTextA("[ZFUIView] add child during parent's dealloc is not allowed"));
        zfCoreAssertWithMessageTrim(view != zfnull, zfTextA("[ZFUIView] add null child"));
        zfCoreAssertWithMessageTrim(view->viewParent() == zfnull, zfTextA("[ZFUIView] add child which already has parent, you should remove it first"));
        zfRetain(view);

        if(childLayer != ZFUIViewChildLayer::e_Normal)
        {
            view->serializableRefLayoutParamSet(this->pimplOwner->layoutParamCreate().to<ZFUIViewLayoutParam *>());
        }
        else
        {
            view->serializableRefLayoutParamSet(zfnull);
        }

        zfbool layoutParamNeedRelease = zffalse;
        if(layoutParam == zfnull)
        {
            layoutParam = view->layoutParam();
        }
        if(layoutParam == zfnull)
        {
            layoutParam = zfRetain(this->pimplOwner->layoutParamCreate().to<ZFUIViewLayoutParam *>());
            layoutParamNeedRelease = zftrue;
        }
        else
        {
            if(!layoutParam->classData()->classIsSubclassOf(this->pimplOwner->layoutParamClass()))
            {
                layoutParamNeedRelease = zftrue;
                ZFUIViewLayoutParam *tmp = zfRetain(this->pimplOwner->layoutParamCreate().to<ZFUIViewLayoutParam *>());
                tmp->styleableCopyFrom(layoutParam);
                layoutParam = tmp;
            }
        }
        view->_ZFP_ZFUIView_parentChanged(this->pimplOwner, layoutParam, childLayer);
        if(atIndex == zfindexMax)
        {
            atIndex = layer.views.count();
        }
        layer.views.add(atIndex, view);
        this->pimplOwner->implChildOnAdd(view, this->viewLayerPrevCount(layer) + atIndex, childLayer, atIndex);

        this->pimplOwner->layoutRequest();
        view->layoutRequest();

        if(layoutParamNeedRelease)
        {
            zfRelease(layoutParam);
        }

        this->checkUpdateChildScale(view);
        ZFUIView *virtualParent = this->virtualParent(this->pimplOwner);
        virtualParent->viewChildOnAdd(view, childLayer);
        view->viewOnAddToParent(virtualParent);
        virtualParent->viewChildOnChange();
    }
    void childRemove(ZF_IN ZFUIViewChildLayerEnum childLayer,
                     ZF_IN _ZFP_ZFUIViewLayerData &layer,
                     ZF_IN ZFUIView *view)
    {
        zfindex index = this->childFind(layer, view);
        if(index != zfindexMax)
        {
            this->childRemoveAtIndex(childLayer, layer, index);
        }
    }
    void childRemoveAtIndex(ZF_IN ZFUIViewChildLayerEnum childLayer,
                            ZF_IN _ZFP_ZFUIViewLayerData &layer,
                            ZF_IN zfindex index)
    {
        ZFUIView *child = layer.views.get(index);

        child->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
        layer.views.remove(index);
        this->pimplOwner->implChildOnRemove(child, this->viewLayerPrevCount(layer) + index, childLayer, index);

        this->pimplOwner->layoutRequest();

        ZFUIView *virtualParent = this->virtualParent(this->pimplOwner);
        virtualParent->viewChildOnRemove(child, childLayer);
        child->viewOnRemoveFromParent(virtualParent);
        virtualParent->viewChildOnChange();
        zfRelease(child);
    }
    void childRemoveAll(ZF_IN ZFUIViewChildLayerEnum childLayer,
                        ZF_IN _ZFP_ZFUIViewLayerData &layer)
    {
        if(layer.views.isEmpty())
        {
            return ;
        }

        this->pimplOwner->layoutRequest();

        ZFCoreArrayPOD<ZFUIView *> tmp = layer.views;
        layer.views = ZFCoreArrayPOD<ZFUIView *>();

        zfindex prevLayerCount = this->viewLayerPrevCount(layer);
        for(zfindex i = tmp.count() - 1; i != zfindexMax; --i)
        {
            tmp[i]->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
            this->pimplOwner->implChildOnRemove(tmp[i], prevLayerCount + i, childLayer, i);
        }

        ZFUIView *virtualParent = this->virtualParent(this->pimplOwner);
        for(zfindex i = tmp.count() - 1; i != zfindexMax; --i)
        {
            ZFUIView *child = tmp[i];
            virtualParent->viewChildOnRemove(child, childLayer);
            child->viewOnRemoveFromParent(virtualParent);
            zfRelease(child);
        }
        virtualParent->viewChildOnChange();
    }
    void childMove(ZF_IN ZFUIViewChildLayerEnum childLayer,
                   ZF_IN _ZFP_ZFUIViewLayerData &layer,
                   ZF_IN zfindex fromIndex,
                   ZF_IN zfindex toIndexOrIndexMax)
    {
        if(toIndexOrIndexMax == zfindexMax)
        {
            toIndexOrIndexMax = this->childCount(layer) - 1;
        }
        if(fromIndex >= this->childCount(layer))
        {
            zfCoreCriticalIndexOutOfRange(fromIndex, this->childCount(layer));
            return ;
        }
        if(toIndexOrIndexMax >= this->childCount(layer))
        {
            zfCoreCriticalIndexOutOfRange(fromIndex, this->childCount(layer));
            return ;
        }
        if(fromIndex == toIndexOrIndexMax)
        {
            return ;
        }

        ZFUIView *child = layer.views.get(fromIndex);
        layer.views.move(fromIndex, toIndexOrIndexMax);

        zfindex prevLayerCount = this->viewLayerPrevCount(layer);
        this->pimplOwner->implChildOnRemove(child, prevLayerCount + fromIndex, childLayer, fromIndex);
        this->pimplOwner->implChildOnAdd(child, prevLayerCount + toIndexOrIndexMax, childLayer, toIndexOrIndexMax);

        ZFUIView *virtualParent = this->virtualParent(this->pimplOwner);
        virtualParent->viewChildOnChange();
    }
    void childMove(ZF_IN ZFUIViewChildLayerEnum childLayer,
                   ZF_IN _ZFP_ZFUIViewLayerData &layer,
                   ZF_IN ZFUIView *child,
                   ZF_IN zfindex toIndexOrIndexMax)
    {
        zfindex fromIndex = this->childFind(layer, child);
        if(fromIndex != zfindexMax)
        {
            this->childMove(childLayer, layer, fromIndex, toIndexOrIndexMax);
        }
    }
    void childReplace(ZF_IN ZFUIViewChildLayerEnum childLayer,
                      ZF_IN _ZFP_ZFUIViewLayerData &layer,
                      ZF_IN zfindex atIndex,
                      ZF_IN ZFUIView *toReplace)
    {
        zfCoreAssert(atIndex < layer.views.count());
        zfCoreAssert(toReplace != zfnull);
        zfCoreAssert(toReplace->viewParent() == zfnull);

        ZFUIView *old = layer.views[atIndex];
        layer.views[atIndex] = toReplace;
        zfindex fixedIndex = this->viewLayerPrevCount(layer) + atIndex;
        this->pimplOwner->implChildOnRemove(old, fixedIndex, childLayer, atIndex);
        this->pimplOwner->implChildOnAdd(toReplace, fixedIndex, childLayer, atIndex);

        this->checkUpdateChildScale(toReplace);
        toReplace->_ZFP_ZFUIView_parentChanged(this->pimplOwner, old->layoutParam(), childLayer);
        old->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);

        ZFUIView *virtualParent = this->virtualParent(this->pimplOwner);
        virtualParent->viewChildOnRemove(old, childLayer);
        old->viewOnRemoveFromParent(virtualParent);
        virtualParent->viewChildOnAdd(toReplace, childLayer);
        toReplace->viewOnAddToParent(virtualParent);
        virtualParent->viewChildOnChange();

        zfRelease(old);
    }

    zfindex childCount(ZF_IN _ZFP_ZFUIViewLayerData &layer)
    {
        return layer.views.count();
    }
    ZFUIView *childAtIndex(ZF_IN _ZFP_ZFUIViewLayerData &layer,
                           ZF_IN zfindex index)
    {
        return layer.views.get(index);
    }
    zfindex childFind(ZF_IN _ZFP_ZFUIViewLayerData &layer,
                      ZF_IN ZFUIView *view)
    {
        return layer.views.find(view);
    }
    zfbool childArrayIsTheSame(ZF_IN ZFUIView *view0,
                               ZF_IN ZFUIView *view1,
                               ZF_IN ZFUIViewChildLayerEnum layer)
    {
        const ZFCoreArrayPOD<ZFUIView *> *children0 = zfnull;
        const ZFCoreArrayPOD<ZFUIView *> *children1 = zfnull;
        switch(layer)
        {
            case ZFUIViewChildLayer::e_Normal:
                children0 = &(view0->d->layerNormal.views);
                children1 = &(view1->d->layerNormal.views);
                break;
            case ZFUIViewChildLayer::e_Impl:
                children0 = &(view0->d->layerInternalImpl.views);
                children1 = &(view1->d->layerInternalImpl.views);
                break;
            case ZFUIViewChildLayer::e_Background:
                children0 = &(view0->d->layerInternalBg.views);
                children1 = &(view1->d->layerInternalBg.views);
                break;
            case ZFUIViewChildLayer::e_Foreground:
                children0 = &(view0->d->layerInternalFg.views);
                children1 = &(view1->d->layerInternalFg.views);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }

        if(children0->count() != children1->count())
        {
            return zffalse;
        }
        for(zfindex i = children0->count() - 1; i != zfindexMax; --i)
        {
            if(ZFObjectCompare(children0->get(i), children1->get(i)) != ZFCompareTheSame)
            {
                return zffalse;
            }
        }
        return zftrue;
    }
    ZFUIViewLayoutParam *childLayoutParamAtIndex(ZF_IN _ZFP_ZFUIViewLayerData &layer,
                                                 ZF_IN zfindex index)
    {
        return layer.views.get(index)->layoutParam();
    }

    zfbool serializeInternalViewFromCategoryData(ZF_IN ZFUIViewChildLayerEnum childLayer,
                                                 ZF_IN const ZFSerializableData &categoryData,
                                                 ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                 ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
    {
        zfautoObject internalView;
        if(!ZFObjectFromSerializableData(internalView, categoryData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        if(internalView == zfautoObjectNull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                zfText("null view"));
            return zffalse;
        }
        if(!internalView.toObject()->classData()->classIsSubclassOf(ZFUIView::ClassData()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                zfText("%s not type of %s"),
                internalView.toObject()->objectInfoOfInstance().cString(), ZFUIView::ClassData()->className());
            return zffalse;
        }
        ZFUIView *internalViewTmp = internalView.to<ZFUIView *>();
        if(internalViewTmp->viewId().isEmpty())
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                zfText("auto serialized internal view %s has no viewId"),
                internalViewTmp->objectInfoOfInstance().cString());
            return zffalse;
        }
        ZFUIView *exist = zfnull;
        const ZFCoreArrayPOD<ZFUIView *> *views = zfnull;
        switch(childLayer)
        {
            case ZFUIViewChildLayer::e_Normal:
                zfCoreCriticalShouldNotGoHere();
                break;
            case ZFUIViewChildLayer::e_Impl:
                views = &(this->layerInternalImpl.views);
                break;
            case ZFUIViewChildLayer::e_Background:
                views = &(this->layerInternalBg.views);
                break;
            case ZFUIViewChildLayer::e_Foreground:
                views = &(this->layerInternalFg.views);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
        for(zfindex i = views->count() - 1; i != zfindexMax; --i)
        {
            ZFUIView *tmp = views->get(i)->to<ZFUIView *>();
            if(tmp->viewId().compare(internalViewTmp->viewId()) == 0)
            {
                exist = tmp;
                break;
            }
        }
        if(exist == zfnull)
        {
            this->pimplOwner->internalViewAutoSerializeTagAdd(internalViewTmp->viewId().cString());
            switch(childLayer)
            {
                case ZFUIViewChildLayer::e_Normal:
                    zfCoreCriticalShouldNotGoHere();
                    break;
                case ZFUIViewChildLayer::e_Impl:
                    this->pimplOwner->internalImplViewAdd(internalViewTmp);
                    break;
                case ZFUIViewChildLayer::e_Background:
                    this->pimplOwner->internalBackgroundViewAdd(internalViewTmp);
                    break;
                case ZFUIViewChildLayer::e_Foreground:
                    this->pimplOwner->internalForegroundViewAdd(internalViewTmp);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zffalse;
            }
        }
        else
        {
            exist->styleableCopyFrom(internalViewTmp);
        }
        return zftrue;
    }
    zfbool serializeInternalViewToCategoryData(ZF_IN ZFUIViewChildLayerEnum childLayer,
                                               ZF_IN_OUT ZFSerializableData &serializableData,
                                               ZF_IN ZFUIView *ref,
                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
    {
        const ZFCoreArrayPOD<ZFUIView *> *views = zfnull;
        const ZFCoreArrayPOD<ZFUIView *> *viewsRef = zfnull;
        const zfchar *categoryTag = zfnull;
        switch(childLayer)
        {
            case ZFUIViewChildLayer::e_Normal:
                zfCoreCriticalShouldNotGoHere();
                break;
            case ZFUIViewChildLayer::e_Impl:
                views = &(this->layerInternalImpl.views);
                if(ref != zfnull)
                {
                    viewsRef = &(ref->d->layerInternalImpl.views);
                }
                break;
                categoryTag = ZFSerializableKeyword_ZFUIView_internalImplView;
            case ZFUIViewChildLayer::e_Background:
                views = &(this->layerInternalBg.views);
                if(ref != zfnull)
                {
                    viewsRef = &(ref->d->layerInternalBg.views);
                }
                categoryTag = ZFSerializableKeyword_ZFUIView_internalBackgroundView;
                break;
            case ZFUIViewChildLayer::e_Foreground:
                views = &(this->layerInternalFg.views);
                if(ref != zfnull)
                {
                    viewsRef = &(ref->d->layerInternalFg.views);
                }
                categoryTag = ZFSerializableKeyword_ZFUIView_internalForegroundView;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
        if(ref == zfnull || viewsRef->count() == 0)
        {
            for(zfindex i = 0; i < views->count(); ++i)
            {
                ZFUIView *tmp = views->get(i)->to<ZFUIView *>();
                if(tmp->viewId().isEmpty()
                    || this->internalViewAutoSerializeTags.find(tmp->viewId().cString()) == this->internalViewAutoSerializeTags.end())
                {
                    continue;
                }
                ZFSerializableData childData;
                if(!ZFObjectToSerializableData(childData, tmp, outErrorHint))
                {
                    return zffalse;
                }
                childData.categorySet(categoryTag);
                serializableData.elementAdd(childData);
            }
        }
        else
        {
            for(zfindex i = 0; i < views->count(); ++i)
            {
                ZFUIView *tmp = views->get(i)->to<ZFUIView *>();
                if(tmp->viewId().isEmpty()
                    || this->internalViewAutoSerializeTags.find(tmp->viewId().cString()) == this->internalViewAutoSerializeTags.end())
                {
                    continue;
                }
                zfbool exist = zffalse;
                for(zfindex iRef = viewsRef->count() - 1; iRef != zfindexMax; --iRef)
                {
                    if(viewsRef->get(iRef)->to<ZFUIView *>()->viewId().compare(tmp->viewId().cString()) == 0)
                    {
                        exist = zftrue;
                        break;
                    }
                }
                if(exist)
                {
                    continue;
                }
                ZFSerializableData childData;
                if(!ZFObjectToSerializableData(childData, tmp, outErrorHint))
                {
                    return zffalse;
                }
                childData.categorySet(categoryTag);
                serializableData.elementAdd(childData);
            }
        }
        return zftrue;
    }
};

// ============================================================
// ZFUIView
ZFOBJECT_REGISTER(ZFUIView)

ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewChildOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewChildOnAdd)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewChildOnRemove)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewOnAddToParent)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewOnRemoveFromParent)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewScaleOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewFocusOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewOnEventFilter)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewOnEvent)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnLayoutRequest)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnMeasureFinish)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnLayoutPrepare)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnLayout)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnLayoutFinish)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewPropertyOnUpdate)

// ============================================================
// serialize
void ZFUIView::serializableRefLayoutParamSet(ZF_IN ZFUIViewLayoutParam *serializableRefLayoutParam)
{
    zfRetain(serializableRefLayoutParam);
    if(this->objectCached())
    {
        if(serializableRefLayoutParam)
        {
            serializableRefLayoutParam->objectCachedSet(zftrue);
        }
        if(d->serializableRefLayoutParam)
        {
            d->serializableRefLayoutParam->objectCachedSet(zffalse);
        }
    }
    zfRelease(d->serializableRefLayoutParam);
    d->serializableRefLayoutParam = serializableRefLayoutParam;
}
ZFUIViewLayoutParam *ZFUIView::serializableRefLayoutParam(void)
{
    return d->serializableRefLayoutParam;
}

zfbool ZFUIView::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                 ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                 ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    if(this->serializableOnCheckNeedSerializeChildren())
    {
        this->childRemoveAll();
    }
    d->layoutParamSet(zfnull);

    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = serializableData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_child))
        {
            zfautoObject element;
            if(!ZFObjectFromSerializableData(element, categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(element == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    zfText("null view"));
                return zffalse;
            }
            if(!element.toObject()->classData()->classIsSubclassOf(ZFUIView::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    zfText("%s not type of %s"),
                    element.toObject()->objectInfoOfInstance().cString(), ZFUIView::ClassData()->className());
                return zffalse;
            }
            ZFUIView *child = element.to<ZFUIView *>();
            this->childAdd(child, child->layoutParam());

            categoryData.resolveMark();
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_layoutParam))
        {
            zfautoObject layoutParam;
            if(!ZFObjectFromSerializableData(layoutParam, categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(layoutParam == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    zfText("null layoutParam"));
                return zffalse;
            }
            if(!layoutParam.toObject()->classData()->classIsSubclassOf(ZFUIViewLayoutParam::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    zfText("%s not type of %s"),
                    layoutParam.toObject()->objectInfoOfInstance().cString(), ZFUIViewLayoutParam::ClassData()->className());
                return zffalse;
            }

            d->layoutParamSet(ZFCastZFObjectUnchecked(ZFUIViewLayoutParam *, layoutParam));
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_internalImplView))
        {
            if(!d->serializeInternalViewFromCategoryData(ZFUIViewChildLayer::e_Impl,
                categoryData,
                outErrorHint,
                outErrorPos))
            {
                return zffalse;
            }
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_internalBackgroundView))
        {
            if(!d->serializeInternalViewFromCategoryData(ZFUIViewChildLayer::e_Background,
                categoryData,
                outErrorHint,
                outErrorPos))
            {
                return zffalse;
            }
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_internalForegroundView))
        {
            if(!d->serializeInternalViewFromCategoryData(ZFUIViewChildLayer::e_Foreground,
                categoryData,
                outErrorHint,
                outErrorPos))
            {
                return zffalse;
            }
        }
    }
    return zftrue;
}
zfbool ZFUIView::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                               ZF_IN ZFSerializable *referencedOwnerOrNull,
                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    // layoutParam
    if(d->layoutParam != zfnull && this->viewParent() != zfnull)
    {
        ZFUIViewLayoutParam *refLayoutParam = ((ref == zfnull) ? zfnull : ref->d->layoutParam);
        zfbool needAdd = zffalse;

        if(refLayoutParam != zfnull)
        {
            if(ZFObjectCompare(d->layoutParam, refLayoutParam) != ZFCompareTheSame)
            {
                needAdd = zftrue;
            }
        }
        else
        {
            if(d->serializableRefLayoutParam == zfnull || ZFObjectCompare(d->layoutParam, d->serializableRefLayoutParam) != ZFCompareTheSame)
            {
                refLayoutParam = d->serializableRefLayoutParam;
                needAdd = zftrue;
            }
        }

        if(needAdd)
        {
            ZFSerializableData categoryData;
            if(!ZFObjectToSerializableData(categoryData, d->layoutParam, outErrorHint, refLayoutParam))
            {
                return zffalse;
            }
            categoryData.categorySet(ZFSerializableKeyword_ZFUIView_layoutParam);
            serializableData.elementAdd(categoryData);
        }
    }

    { // internal views
        if(!d->internalViewAutoSerializeTags.empty())
        {
            if(!d->serializeInternalViewToCategoryData(ZFUIViewChildLayer::e_Impl, serializableData, ref, outErrorHint))
            {
                return zffalse;
            }
            if(!d->serializeInternalViewToCategoryData(ZFUIViewChildLayer::e_Background, serializableData, ref, outErrorHint))
            {
                return zffalse;
            }
            if(!d->serializeInternalViewToCategoryData(ZFUIViewChildLayer::e_Foreground, serializableData, ref, outErrorHint))
            {
                return zffalse;
            }
        }
    }

    // all children
    if(this->serializableOnCheckNeedSerializeChildren())
    {
        if(ref == zfnull || ref->childCount() == 0)
        {
            for(zfindex i = 0, count = this->childCount(); i < count; ++i)
            {
                ZFSerializableData childData;
                if(!ZFObjectToSerializableData(childData, this->childAtIndex(i), outErrorHint))
                {
                    return zffalse;
                }
                childData.categorySet(ZFSerializableKeyword_ZFUIView_child);
                serializableData.elementAdd(childData);
            }
        }
        else
        {
            if(!d->childArrayIsTheSame(this, ref, ZFUIViewChildLayer::e_Normal))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    zfText("child mismatch, this: %s, ref: %s"),
                    this->objectInfoOfInstance().cString(), ref->objectInfoOfInstance().cString());
                return zffalse;
            }
        }
    }
    return zftrue;
}
zfbool ZFUIView::serializableOnCheckNeedSerializeChildren(void)
{
    return zftrue;
}

// ============================================================
// properties
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIView, zfstring, viewDelegateClass)
{
    this->viewDelegateClassSetInternal(newValue);

    zfautoObject viewDelegateTmp = ZFClass::newInstanceForName(this->viewDelegateClass(),
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    if(!this->viewDelegateSupported() && viewDelegateTmp != zfautoObjectNull)
    {
        zfCoreCriticalMessage(zfText("viewDelegate not supported"));
        return ;
    }
    this->viewDelegateSet(ZFCastZFObject(ZFUIView *, viewDelegateTmp));
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIView, zfbool, viewVisible)
{
    zfbool changed = (this->viewVisible() != newValue);
    this->viewVisibleSetInternal(newValue);
    d->impl->viewVisibleSet(this, this->viewVisible());
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIView, zffloat, viewAlpha)
{
    zffloat tmp = zfmApplyRange<zffloat>(newValue, 0, 1);
    this->viewAlphaSetInternal(tmp);
    d->impl->viewAlphaSet(this, this->viewAlpha());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIView, zfbool, viewFocusable)
{
    this->viewFocusableSetInternal(newValue);
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        impl->viewFocusableSet(this, this->viewFocusable());
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIView, zfbool, viewUIEnable)
{
    this->viewUIEnableSetInternal(newValue);
    d->impl->viewUIEnableSet(this, this->viewUIEnable());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIView, zfbool, viewUIEnableTree)
{
    this->viewUIEnableTreeSetInternal(newValue);
    d->impl->viewUIEnableTreeSet(this, this->viewUIEnableTree());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIView, zfbool, viewMouseHoverEventEnable)
{
    this->viewMouseHoverEventEnableSetInternal(newValue);
    d->impl->viewMouseHoverEventEnableSet(this, this->viewMouseHoverEventEnable());
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIView, ZFUISize, viewSizePrefered)
{
    zfbool changed = (this->viewSizePrefered() != newValue);
    this->viewSizePreferedSetInternal(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIView, ZFUISize, viewSizeMin)
{
    zfbool changed = (this->viewSizeMin() != newValue);
    this->viewSizeMinSetInternal(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIView, ZFUISize, viewSizeMax)
{
    zfbool changed = (this->viewSizeMax() != newValue);
    this->viewSizeMaxSetInternal(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIView, ZFUIColor, viewBackgroundColor)
{
    this->viewBackgroundColorSetInternal(newValue);
    d->impl->viewBackgroundColorSet(this, this->viewBackgroundColor());
}

// ============================================================
// init and dealloc
ZFObject *ZFUIView::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfpoolNew(_ZFP_ZFUIViewPrivate);
    d->pimplOwner = this;

    d->viewLayoutOnMeasureFinishParamCache = zfAllocWithoutLeakTest(ZFUIViewMeasureResult);
    d->nativeView = d->impl->nativeViewCreate(this);
    d->scaleForImpl = d->impl->nativeViewScaleForImpl(d->nativeView);
    d->scaleFixed = d->scaleForImpl;

    return this;
}
void ZFUIView::objectOnDealloc(void)
{
    if(d->viewDelegate != zfnull)
    {
        ZFUIView *viewDelegate = d->viewDelegate;
        d->viewDelegate = zfnull;
        viewDelegate->d->viewDelegateForParent = zffalse;
        zfRelease(viewDelegate);
    }

    this->nativeImplViewSet(zfnull, zfnull);
    d->impl->nativeViewDestroy(this, d->nativeView);
    d->nativeView = zfnull;

    for(zfindex i = d->layerNormal.views.count() - 1; i != zfindexMax; --i)
    {
        zfRelease(d->layerNormal.views.get(i));
    }
    for(zfindex i = d->layerInternalImpl.views.count() - 1; i != zfindexMax; --i)
    {
        zfRelease(d->layerInternalImpl.views.get(i));
    }
    for(zfindex i = d->layerInternalBg.views.count() - 1; i != zfindexMax; --i)
    {
        zfRelease(d->layerInternalBg.views.get(i));
    }
    for(zfindex i = d->layerInternalFg.views.count() - 1; i != zfindexMax; --i)
    {
        zfRelease(d->layerInternalFg.views.get(i));
    }
    d->layoutParamSet(zfnull);
    zfReleaseWithoutLeakTest(d->viewLayoutOnMeasureFinishParamCache);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIView::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();

    // auto update view property when object init finished
    this->observerAdd(ZFObject::ObserverAddParam()
            .eventIdSet(ZFObject::EventObjectAfterAlloc())
            .observerSet(ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewListenerHolder)->viewPropertyOnUpdateListener)
            .userDataSet(this->objectHolder())
            .observerLevelSet(ZFLevelZFFrameworkPostNormal)
            .autoRemoveAfterActivateSet(zftrue)
        );
}
void ZFUIView::objectOnDeallocPrepare(void)
{
    if(this->viewFocused())
    {
        this->viewFocusRequest(zffalse);
    }

    ZFThreadTaskCancelWithOwner(this);

    // normal child should be removed first due to viewDelegate
    this->childRemoveAll();
    d->childRemoveAll(ZFUIViewChildLayer::e_Foreground, d->layerInternalFg);
    d->childRemoveAll(ZFUIViewChildLayer::e_Background, d->layerInternalBg);
    d->childRemoveAll(ZFUIViewChildLayer::e_Impl, d->layerInternalImpl);

    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFUIView::objectHash(void)
{
    zfidentity hash = zfidentityHash(zfidentityCalcString(this->classData()->className()) , this->childCount());
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        hash = zfidentityHash(hash, this->childAtIndex(i)->objectHash());
    }
    return hash;
}
ZFCompareResult ZFUIView::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->childCount() != another->childCount())
    {
        return ZFCompareUncomparable;
    }
    if(ZFObjectCompare(d->layoutParam, another->d->layoutParam) != ZFCompareTheSame)
    {
        if((d->layoutParam == zfnull || ZFObjectCompare(d->layoutParam, d->serializableRefLayoutParam) == ZFCompareTheSame)
            != (another->d->layoutParam == zfnull || ZFObjectCompare(another->d->layoutParam, another->d->serializableRefLayoutParam) == ZFCompareTheSame))
        {
            return ZFCompareUncomparable;
        }
    }
    if(!ZFClassUtil::allPropertyIsEqual(this, another)
        || !d->childArrayIsTheSame(this, another, ZFUIViewChildLayer::e_Impl)
        || !d->childArrayIsTheSame(this, another, ZFUIViewChildLayer::e_Background)
        || !d->childArrayIsTheSame(this, another, ZFUIViewChildLayer::e_Foreground)
        || !d->childArrayIsTheSame(this, another, ZFUIViewChildLayer::e_Normal))
    {
        return ZFCompareUncomparable;
    }
    return ZFCompareTheSame;
}

static void _ZFP_ZFUIView_objectInfo_viewDelegate(ZF_IN_OUT zfstring &ret, ZF_IN ZFUIView *view)
{
    if(view->viewDelegate() == zfnull)
    {
        return ;
    }
    zfindex count = 1;
    {
        ZFUIView *parent = view;
        while(parent->viewDelegateForParent())
        {
            ++count;
            parent = parent->viewParent();
        }
    }
    ret += zfText("\n");
    for(zfindex i = 0; i < count; ++i)
    {
        ret += zfText("    ");
    }
    ret += zfText("(delegate) ");
    view->viewDelegate()->objectInfoT(ret);
}
void ZFUIView::objectInfoT(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoT(ret);
    _ZFP_ZFUIView_objectInfo_viewDelegate(ret, this);
}
void ZFUIView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(!this->viewId().isEmpty())
    {
        zfstringAppend(ret, zfText(" [%s]"), this->viewId().cString());
    }

    ret += zfText(" ");
    ZFUIRectToString(ret, this->layoutedFrame());

    if(!this->viewVisible())
    {
        ret += zfText(" Hidden");
    }

    if(!this->viewUIEnableTree())
    {
        ret += zfText(" UITreeDisabled");
    }
    else if(!this->viewUIEnable())
    {
        ret += zfText(" UIDisabled");
    }
}

void ZFUIView::objectCachedOnChange(void)
{
    zfsuper::objectCachedOnChange();
    zfbool objectCached = this->objectCached();
    if(d->layoutParam)
    {
        d->layoutParam->objectCachedSet(objectCached);
    }
    if(d->serializableRefLayoutParam)
    {
        d->serializableRefLayoutParam->objectCachedSet(objectCached);
    }
    if(d->viewDelegate)
    {
        d->viewDelegate->objectCachedSet(objectCached);
    }
    for(zfindex i = 0; i < d->layerInternalImpl.views.count(); ++i)
    {
        d->layerInternalImpl.views[i]->objectCachedSet(objectCached);
    }
    for(zfindex i = 0; i < d->layerInternalBg.views.count(); ++i)
    {
        d->layerInternalBg.views[i]->objectCachedSet(objectCached);
    }
    for(zfindex i = 0; i < d->layerNormal.views.count(); ++i)
    {
        d->layerNormal.views[i]->objectCachedSet(objectCached);
    }
    for(zfindex i = 0; i < d->layerInternalFg.views.count(); ++i)
    {
        d->layerInternalFg.views[i]->objectCachedSet(objectCached);
    }
}

void ZFUIView::_ZFP_ZFUIView_notifyLayoutNativeImplView(ZF_OUT ZFUIRect &result)
{
    ZFUIRect bounds = ZFUIRectGetBounds(this->layoutedFrame());
    ZFUIRectApplyMargin(bounds, bounds, this->nativeImplViewMargin());
    result = bounds;
    this->nativeImplViewOnLayout(result, bounds);
}
void *ZFUIView::nativeImplView(void)
{
    return d->nativeImplView;
}
void ZFUIView::nativeImplViewSet(ZF_IN void *nativeImplView,
                                 ZF_IN ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback)
{
    void *nativeImplViewOld = d->nativeImplView;
    ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallbackOld = d->nativeImplViewDeleteCallback;
    d->nativeImplView = nativeImplView;
    d->nativeImplViewDeleteCallback = nativeImplViewDeleteCallback;
    d->impl->nativeImplViewSet(this, nativeImplViewOld, d->nativeImplView, d->layerInternalImpl.views.count());
    if(nativeImplViewOld && nativeImplViewDeleteCallbackOld)
    {
        nativeImplViewDeleteCallbackOld(this, nativeImplViewOld);
    }
}
void ZFUIView::nativeImplViewMarginUpdate(void)
{
    ZFUIMargin old = d->nativeImplViewMargin;
    d->nativeImplViewMargin = ZFUIMarginZero;
    this->nativeImplViewMarginOnUpdate(d->nativeImplViewMargin);
    if(d->nativeImplViewMargin != old)
    {
        d->impl->nativeImplViewMarginSet(this, d->nativeImplViewMargin);
        this->layoutRequest();
    }
}
const ZFUIMargin &ZFUIView::nativeImplViewMargin(void)
{
    return d->nativeImplViewMargin;
}

// ============================================================
void ZFUIView::implChildOnAdd(ZF_IN ZFUIView *child,
                              ZF_IN zfindex virtualIndex,
                              ZF_IN ZFUIViewChildLayerEnum childLayer,
                              ZF_IN zfindex childLayerIndex)
{
    d->impl->childAdd(this, child, virtualIndex, childLayer, childLayerIndex);
}
void ZFUIView::implChildOnRemove(ZF_IN ZFUIView *child,
                                 ZF_IN zfindex virtualIndex,
                                 ZF_IN ZFUIViewChildLayerEnum childLayer,
                                 ZF_IN zfindex childLayerIndex)
{
    d->impl->childRemove(this, child, virtualIndex, childLayer, childLayerIndex);
}

// ============================================================
// parent
void ZFUIView::_ZFP_ZFUIView_parentChanged(ZF_IN ZFUIView *viewParent,
                                           ZF_IN ZFUIViewLayoutParam *layoutParam,
                                           ZF_IN ZFUIViewChildLayerEnum viewLayer)
{
    if(d->viewParent != zfnull && d->viewParent->objectCached())
    {
        if(!(viewParent != zfnull && viewParent->objectCached()))
        {
            this->objectCachedSet(zffalse);
        }
    }
    else
    {
        if(viewParent != zfnull && viewParent->objectCached())
        {
            this->objectCachedSet(zftrue);
        }
    }

    if(viewParent == zfnull)
    {
        d->viewParent = zfnull;
        d->viewDelegateParent = zfnull;
        this->layoutedFramePrevReset();
    }
    else
    {
        d->viewParent = d->virtualParent(viewParent);
        if(viewParent->d->viewDelegateForParent)
        {
            d->viewDelegateParent = viewParent;
        }
        else
        {
            d->viewDelegateParent = zfnull;
        }
    }

    if(layoutParam != zfnull)
    {
        // only change layoutParam if not null
        // keep old one for performance
        d->layoutParamSet(layoutParam);
    }
    else
    {
        this->serializableRefLayoutParamSet(zfnull);
    }

    d->viewLayer = viewLayer;
}
void *ZFUIView::nativeView(void)
{
    return d->nativeView;
}
void ZFUIView::nativeViewNotifyBeforeAdd(ZF_IN ZFUIView *view,
                                         ZF_IN void *nativeParentView)
{
    zfCoreAssert(view != zfnull && nativeParentView != zfnull);

    zfRetain(view);
    view->_ZFP_ZFUIView_scaleSetRecursively(view->scaleGet(), view->d->impl->nativeViewScaleForImpl(nativeParentView));
}
void ZFUIView::nativeViewNotifyAfterRemove(ZF_IN ZFUIView *view)
{
    zfCoreAssert(view != zfnull);
    zfRelease(view);
}

// ============================================================
// view delegate logic
void ZFUIView::viewDelegateSet(ZF_IN ZFUIView *viewDelegate)
{
    if(viewDelegate == d->viewDelegate)
    {
        return ;
    }
    zfCoreAssertWithMessage(viewDelegate != this, zfTextA("you must not set viewDelegate to self"));
    zfCoreAssertWithMessage(viewDelegate == zfnull || viewDelegate->viewParent() == zfnull,
        zfTextA("setting a viewDelegate which already has parent"));

    ZFUIView *viewDelegateOld = d->viewDelegate;
    d->viewDelegate = zfnull;
    zfblockedAllocWithoutLeakTest(ZFArrayEditable, children);
    zfblockedAllocWithoutLeakTest(ZFArrayEditable, childLayoutParams);
    if(viewDelegateOld != zfnull)
    {
        for(zfindex i = 0; i < viewDelegateOld->childCount(); ++i)
        {
            children->add(viewDelegateOld->childAtIndex(i));
            childLayoutParams->add(viewDelegateOld->childAtIndex(i)->layoutParam());
        }
        viewDelegateOld->childRemoveAll();
        viewDelegateOld->d->viewDelegateForParent = zffalse;
        viewDelegateOld->viewRemoveFromParent();
    }

    if(viewDelegate != zfnull)
    {
        for(zfindex i = 0; i < this->childCount(); ++i)
        {
            children->add(this->childAtIndex(i));
            childLayoutParams->add(this->childAtIndex(i)->layoutParam());
        }
        this->childRemoveAll();

        d->viewDelegate = zfRetain(viewDelegate);
    }

    ZFUIView *viewToAdd = this;
    if(d->viewDelegate != zfnull)
    {
        this->internalForegroundViewAdd(d->viewDelegate,
            zfHint("layoutParam")zfnull,
            zfHint("addAsTopMost")zffalse);
        viewToAdd = d->viewDelegate;
        d->viewDelegate->d->viewDelegateForParent = zftrue;
    }
    for(zfindex i = 0; i < children->count(); ++i)
    {
        viewToAdd->childAdd(
            children->get<ZFUIView *>(i),
            childLayoutParams->get<ZFUIViewLayoutParam *>(i));
    }
    if(this->objectCached())
    {
        d->viewDelegate->objectCachedSet(zftrue);
        viewDelegateOld->objectCachedSet(zffalse);
    }

    if(viewDelegateOld != zfnull)
    {
        this->viewDelegateOnDealloc(viewDelegateOld);
    }
    if(d->viewDelegate != zfnull)
    {
        this->viewDelegateOnInit(d->viewDelegate);
    }
    zfRelease(viewDelegateOld);
}

zfbool ZFUIView::viewDelegateForParent(void)
{
    return d->viewDelegateForParent;
}
ZFUIView *ZFUIView::viewDelegateParent(void)
{
    return d->viewDelegateParent;
}
ZFUIView *ZFUIView::viewDelegate(void)
{
    return d->viewDelegate;
}
void ZFUIView::viewDelegateLayoutOnMeasure(ZF_OUT ZFUISize &ret,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN const ZFUISizeParam &sizeParam)
{
    const ZFUIMargin &viewDelegateMargin = d->viewDelegate->layoutParam()->layoutMargin();
    ret = d->viewDelegate->layoutMeasure(
        ZFUIViewLayoutParam::sizeHintOffset(sizeHint, ZFUISizeMake(
            ZFUIMarginGetX(viewDelegateMargin),
            ZFUIMarginGetY(viewDelegateMargin))),
        sizeParam);
    ret = ZFUISizeApplyMarginReversely(ret, viewDelegateMargin);
}
void ZFUIView::viewDelegateLayoutOnMeasureFinish(ZF_IN_OUT ZFUISize &measuredSize,
                                                 ZF_IN const ZFUISize &sizeHint,
                                                 ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUISize tmp = ZFUISizeApplyMargin(measuredSize, d->viewDelegate->layoutParam()->layoutMargin());
    d->viewDelegate->layoutOnMeasureFinish(tmp, sizeHint, sizeParam);
    measuredSize = ZFUISizeApplyMarginReversely(tmp, d->viewDelegate->layoutParam()->layoutMargin());
}
void ZFUIView::viewDelegateLayoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    // nothing to do
}
void ZFUIView::viewDelegateLayoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    d->viewDelegate->layout(
        ZFUIRectApplyMargin(bounds, d->viewDelegate->layoutParam()->layoutMargin()));
}
void ZFUIView::viewDelegateLayoutOnLayoutFinish(ZF_IN const ZFUIRect &bounds)
{
    // nothing to do
}

// ============================================================
// view focus
zfbool ZFUIView::viewFocused(void)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        return impl->viewFocused(this);
    }
    return zffalse;
}
void ZFUIView::viewFocusRequest(ZF_IN zfbool viewFocus)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        if(!viewFocus || this->viewFocusable())
        {
            impl->viewFocusRequest(this, viewFocus);
        }
    }
}
ZFUIView *ZFUIView::viewFocusedChild(void)
{
    if(!ZFPROTOCOL_IS_AVAILABLE(ZFUIViewFocus))
    {
        return zfnull;
    }
    if(this->viewFocused())
    {
        return this;
    }
    ZFUIView *ret = zfnull;
    for(zfindex i = this->childCount() - 1; i != zfindexMax; --i)
    {
        ret = this->childAtIndex(i)->viewFocusedChild();
        if(ret != zfnull)
        {
            return ret;
        }
    }
    for(zfindex i = d->layerInternalFg.views.count() - 1; i != zfindexMax; --i)
    {
        ret = d->layerInternalFg.views[i]->viewFocusedChild();
        if(ret != zfnull)
        {
            return ret;
        }
    }
    for(zfindex i = d->layerInternalBg.views.count() - 1; i != zfindexMax; --i)
    {
        ret = d->layerInternalBg.views[i]->viewFocusedChild();
        if(ret != zfnull)
        {
            return ret;
        }
    }
    for(zfindex i = d->layerInternalBg.views.count() - 1; i != zfindexMax; --i)
    {
        ret = d->layerInternalBg.views[i]->viewFocusedChild();
        if(ret != zfnull)
        {
            return ret;
        }
    }
    for(zfindex i = d->layerInternalImpl.views.count() - 1; i != zfindexMax; --i)
    {
        ret = d->layerInternalImpl.views[i]->viewFocusedChild();
        if(ret != zfnull)
        {
            return ret;
        }
    }
    return zfnull;
}

// ============================================================
// parent
ZFUIView *ZFUIView::viewParent(void)
{
    return d->viewParent;
}
ZFUIView *ZFUIView::viewParentVirtual(void)
{
    if(d->viewDelegateParent != zfnull)
    {
        return d->viewDelegateParent;
    }
    else
    {
        return d->viewParent;
    }
}

void ZFUIView::viewRemoveFromParent(void)
{
    if(this->viewParent() != zfnull)
    {
        switch(this->viewLayer())
        {
            case ZFUIViewChildLayer::e_Normal:
                this->viewParent()->childRemove(this);
                break;
            case ZFUIViewChildLayer::e_Impl:
                this->viewParent()->internalImplViewRemove(this);
                break;
            case ZFUIViewChildLayer::e_Background:
                this->viewParent()->internalBackgroundViewRemove(this);
                break;
            case ZFUIViewChildLayer::e_Foreground:
                this->viewParent()->internalForegroundViewRemove(this);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
}

// ============================================================
// scale settings
zffloat ZFUIView::scaleGet(void)
{
    return d->scaleForApp;
}
zffloat ZFUIView::scaleGetForImpl(void)
{
    return d->scaleForImpl;
}
zffloat ZFUIView::scaleGetFixed(void)
{
    return d->scaleFixed;
}
zffloat ZFUIView::scaleGetForImplPhysicalPixel(void)
{
    return d->impl->nativeViewScaleForPhysicalPixel(d->nativeView);
}
void ZFUIView::scaleOnChange(void)
{
    this->observerNotify(ZFUIView::EventViewScaleOnChange());
}
void ZFUIView::_ZFP_ZFUIView_scaleSet(ZF_IN zffloat scaleForApp,
                                      ZF_IN zffloat scaleForImpl)
{
    d->scaleForApp = scaleForApp;
    d->scaleForImpl = scaleForImpl;
    zffloat scaleFixed = scaleForApp * scaleForImpl;
    if(zffloatNotEqual(d->scaleFixed, scaleFixed))
    {
        d->scaleFixed = scaleFixed;
        this->scaleOnChange();
    }
}
void ZFUIView::_ZFP_ZFUIView_scaleSetRecursively(ZF_IN zffloat scaleForApp,
                                                 ZF_IN zffloat scaleForImpl)
{
    if(zffloatNotEqual(d->scaleForApp, scaleForApp)
        || zffloatNotEqual(d->scaleForImpl, scaleForImpl))
    {
        this->_ZFP_ZFUIView_scaleSet(scaleForApp, scaleForImpl);
        for(zfindex i = d->layerInternalImpl.views.count() - 1; i != zfindexMax; --i)
        {
            d->layerInternalImpl.views.get(i)->to<ZFUIView *>()->_ZFP_ZFUIView_scaleSetRecursively(scaleForApp, scaleForImpl);
        }
        for(zfindex i = d->layerInternalBg.views.count() - 1; i != zfindexMax; --i)
        {
            d->layerInternalBg.views.get(i)->to<ZFUIView *>()->_ZFP_ZFUIView_scaleSetRecursively(scaleForApp, scaleForImpl);
        }
        for(zfindex i = d->layerInternalFg.views.count() - 1; i != zfindexMax; --i)
        {
            d->layerInternalFg.views.get(i)->to<ZFUIView *>()->_ZFP_ZFUIView_scaleSetRecursively(scaleForApp, scaleForImpl);
        }
        for(zfindex i = this->childCount() - 1; i != zfindexMax; --i)
        {
            this->childAtIndex(i)->_ZFP_ZFUIView_scaleSetRecursively(scaleForApp, scaleForImpl);
        }
    }
}

// ============================================================
// layout logic
zfautoObject ZFUIView::layoutParamCreate(void)
{
    zfautoObject layoutParam = this->layoutParamClass()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    if(layoutParam == zfautoObjectNull || !layoutParam.toObject()->classData()->classIsSubclassOf(ZFUIViewLayoutParam::ClassData()))
    {
        return zfautoObjectNull;
    }
    this->layoutParamOnUpdate(layoutParam.to<ZFUIViewLayoutParam *>());
    return layoutParam;
}
const ZFClass *ZFUIView::layoutParamClass(void)
{
    return ZFUIViewLayoutParam::ClassData();
}

ZFUIViewLayoutParam *ZFUIView::layoutParam(void)
{
    return d->layoutParam;
}
void ZFUIView::layoutParamSet(ZF_IN ZFUIViewLayoutParam *layoutParam)
{
    if(this->viewParent() != zfnull && layoutParam != zfnull && !layoutParam->classData()->classIsTypeOf(this->layoutParamClass()))
    {
        zfautoObject layoutParamHolder = this->layoutParamCreate();
        ZFUIViewLayoutParam *layoutParamTmp = layoutParamHolder.to<ZFUIViewLayoutParam *>();
        layoutParamTmp->styleableCopyFrom(layoutParam);
        d->layoutParamSet(layoutParamTmp);
    }
    else
    {
        d->layoutParamSet(layoutParam);
    }
}

void ZFUIView::layoutRequestOverrideSet(ZF_IN zfbool layoutRequestOverride)
{
    if(layoutRequestOverride)
    {
        ++(d->layoutRequestOverrideFlag);
    }
    else
    {
        zfCoreAssert(d->layoutRequestOverrideFlag > 0);
        --(d->layoutRequestOverrideFlag);
    }
}
zfindex ZFUIView::layoutRequestOverride(void)
{
    return d->layoutRequestOverrideFlag;
}

void ZFUIView::_ZFP_ZFUIView_notifyLayoutRootView(ZF_IN const ZFUIRect &bounds)
{
    ++_ZFP_ZFUIView_layoutRequestOverrideFlag;
    this->layoutMeasure(bounds.size, ZFUISizeParamFillWidthFillHeight);
    this->layout(bounds);
    --_ZFP_ZFUIView_layoutRequestOverrideFlag;
}

void ZFUIView::layoutRequest(void)
{
    if(!d->layoutRequested && _ZFP_ZFUIView_layoutRequestOverrideFlag == 0 && d->layoutRequestOverrideFlag == 0)
    {
        ZFCoreArrayPOD<ZFUIView *> toNotify;
        toNotify.add(this);
        d->layoutRequested = zftrue;
        if(d->viewDelegate != zfnull)
        {
            if(!d->viewDelegate->d->layoutRequested)
            {
                d->viewDelegate->d->layoutRequested = zftrue;
                toNotify.add(d->viewDelegate);
            }
        }
        ZFUIView *view = this->viewParentVirtual();
        while(view != zfnull && !view->d->layoutRequested)
        {
            toNotify.add(view);
            view->d->layoutRequested = zftrue;
            view = view->viewParentVirtual();
        }
        d->impl->layoutRequest(this);
        for(zfindex i = 0; i < toNotify.count(); ++i)
        {
            toNotify[i]->layoutOnLayoutRequest(this);
        }
    }
}
zfbool ZFUIView::layoutRequested(void)
{
    return d->layoutRequested;
}
zfbool ZFUIView::layouting(void)
{
    return d->layouting;
}
const ZFUISize &ZFUIView::layoutMeasure(ZF_IN const ZFUISize &sizeHint,
                                        ZF_IN const ZFUISizeParam &sizeParam)
{
    if(d->layoutRequested
        || !ZFUISizeIsEqual(d->lastMeasuredSizeHint, sizeHint)
        || !ZFUISizeParamIsEqual(d->lastMeasuredSizeParam, sizeParam))
    {
        d->lastMeasuredSize = ZFUISizeInvalid;
        d->lastMeasuredSizeHint = sizeHint;
        d->lastMeasuredSizeParam = sizeParam;
        if(sizeParam.width == ZFUISizeType::e_Fill && sizeParam.height == ZFUISizeType::e_Fill)
        {
            d->lastMeasuredSize = sizeHint;
            d->lastMeasuredSize.width = zfmMax(0, d->lastMeasuredSize.width);
            d->lastMeasuredSize.height = zfmMax(0, d->lastMeasuredSize.height);
        }
        else if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnMeasure(d->lastMeasuredSize, sizeHint, sizeParam);
        }
        else
        {
            this->layoutOnMeasure(d->lastMeasuredSize, sizeHint, sizeParam);
        }

        if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnMeasureFinish(d->lastMeasuredSize, sizeHint, sizeParam);
        }
        this->layoutOnMeasureFinish(d->lastMeasuredSize, sizeHint, sizeParam);
        {
            d->viewLayoutOnMeasureFinishParamCache->sizeHint = sizeHint;
            d->viewLayoutOnMeasureFinishParamCache->sizeParam = sizeParam;
            d->viewLayoutOnMeasureFinishParamCache->measuredSize = d->lastMeasuredSize;
            this->observerNotify(ZFUIView::EventViewLayoutOnMeasureFinish(), d->viewLayoutOnMeasureFinishParamCache);
            d->lastMeasuredSize = d->viewLayoutOnMeasureFinishParamCache->measuredSize;
        }

        if(d->lastMeasuredSize.width < 0)
        {
            if(this->viewSizePrefered().width >= 0)
            {
                d->lastMeasuredSize.width = this->viewSizePrefered().width;
            }
            else
            {
                d->lastMeasuredSize.width = sizeHint.width;
            }
        }
        if(d->lastMeasuredSize.height < 0)
        {
            if(this->viewSizePrefered().height >= 0)
            {
                d->lastMeasuredSize.height = this->viewSizePrefered().height;
            }
            else
            {
                d->lastMeasuredSize.height = sizeHint.height;
            }
        }

        ZFUIViewLayoutParam::sizeHintApply(d->lastMeasuredSize, d->lastMeasuredSize, sizeHint, sizeParam);
        ZFUISizeApplyRange(d->lastMeasuredSize, d->lastMeasuredSize, this->viewSizeMin(), this->viewSizeMax());
    }
    return d->lastMeasuredSize;
}
const ZFUISize &ZFUIView::layoutMeasuredSize(void)
{
    return d->lastMeasuredSize;
}
void ZFUIView::layout(ZF_IN const ZFUIRect &rect)
{
    if(d->layoutRequested
        || !ZFUISizeIsEqual(d->layoutedFrame.size, rect.size))
    {
        if(!d->layouting && !d->layoutedFramePrevResetFlag)
        {
            d->layoutedFramePrev = d->layoutedFrame;
        }
        d->layoutedFramePrevResetFlag = zffalse;
        d->layoutedFrame = rect;
        d->impl->viewFrameSet(this, ZFUIRectApplyScale(rect, this->scaleGetFixed()));
        ZFUIRect bounds = ZFUIRectGetBounds(rect);

        d->layouting = zftrue;

        // layout prepare
        if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnLayoutPrepare(bounds);
        }
        this->layoutOnLayoutPrepare(bounds);
        this->observerNotify(ZFUIView::EventViewLayoutOnLayoutPrepare());

        // layout
        if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnLayout(bounds);
        }
        else
        {
            this->layoutOnLayout(bounds);
        }
        this->observerNotify(ZFUIView::EventViewLayoutOnLayout());

        // internal views
        this->internalImplViewOnLayout(bounds);
        this->internalBackgroundViewOnLayout(bounds);
        this->internalForegroundViewOnLayout(bounds);

        // layout finish
        if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnLayoutFinish(bounds);
        }
        this->layoutOnLayoutFinish(bounds);
        this->observerNotify(ZFUIView::EventViewLayoutOnLayoutFinish());

        d->layouting = zffalse;
    }
    else if(!ZFUIRectIsEqual(d->layoutedFrame, rect)) {
        // size not changed but point changed, notify impl to move the view is enough
        if(!d->layouting && !d->layoutedFramePrevResetFlag)
        {
            d->layoutedFramePrev = d->layoutedFrame;
        }
        d->layoutedFramePrevResetFlag = zffalse;
        d->layoutedFrame = rect;
        d->impl->viewFrameSet(this, ZFUIRectApplyScale(rect, this->scaleGetFixed()));
    }
    d->layoutRequested = zffalse;
}
void ZFUIView::layoutIfNeed(void)
{
    this->layout(this->layoutedFrame());
}
const ZFUIRect &ZFUIView::layoutedFrame(void)
{
    return d->layoutedFrame;
}
const ZFUIRect &ZFUIView::layoutedFramePrev(void)
{
    return d->layoutedFramePrev;
}
void ZFUIView::layoutedFramePrevReset(void)
{
    d->layoutedFramePrev = ZFUIRectZero;
    d->layoutedFramePrevResetFlag = zftrue;
}
void ZFUIView::layoutedFrameFixed(ZF_OUT ZFUIRect &ret)
{
    ret = d->layoutedFrame;
    ZFUIView *viewParent = d->viewParent;
    if(d->viewDelegateParent != zfnull)
    {
        ZFUIView *viewDelegateParent = d->viewDelegateParent;
        do
        {
            ZFUIRect tmp = ret;
            viewDelegateParent->layoutedFrameFixedOnUpdateForChild(ret, tmp);
            viewParent = viewDelegateParent->d->viewParent;
            viewDelegateParent = viewDelegateParent->d->viewDelegateParent;
        } while(viewDelegateParent != zfnull);
    }
    if(viewParent != zfnull)
    {
        viewParent->layoutedFrameFixedOnUpdateForChild(ret, this->layoutedFrame());
    }
}

void ZFUIView::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        ZFUIView *child = this->childAtIndex(i);
        child->layout(
            ZFUIViewLayoutParam::layoutParamApply(
                bounds,
                child,
                child->layoutParam()));
    }
}

// ============================================================
// child views
ZFUIView *ZFUIView::childFindById(ZF_IN const zfchar *viewId,
                                  ZF_IN_OPT zfbool findRecursively /* = zftrue */,
                                  ZF_IN_OPT zfbool includeInternalViews /* = zffalse */)
{
    if(viewId == zfnull || *viewId == '\0')
    {
        return zfnull;
    }

    ZFCoreArrayPOD<ZFUIView *> toFind;
    toFind.add(this);
    while(!toFind.isEmpty())
    {
        ZFUIView *view = toFind.get(0);
        toFind.remove(0);
        if(zfscmpTheSame(view->viewId().cString(), viewId))
        {
            return view;
        }
        if(!findRecursively)
        {
            continue;
        }
        toFind.addFrom(view->childArray());
        if(includeInternalViews)
        {
            toFind.addFrom(view->d->layerInternalImpl.views);
            toFind.addFrom(view->d->layerInternalBg.views);
            toFind.addFrom(view->d->layerInternalFg.views);
        }
    }
    return zfnull;
}

void ZFUIView::childAdd(ZF_IN ZFUIView *view,
                        ZF_IN_OPT ZFUIViewLayoutParam *layoutParam /* = zfnull */,
                        ZF_IN_OPT zfindex atIndex /* = zfindexMax */)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childAdd(view, layoutParam, atIndex);
    }
    else
    {
        d->childAdd(ZFUIViewChildLayer::e_Normal, d->layerNormal, view, layoutParam, atIndex);
    }
}
void ZFUIView::childRemove(ZF_IN ZFUIView *view)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childRemove(view);
    }
    else
    {
        d->childRemove(ZFUIViewChildLayer::e_Normal, d->layerNormal, view);
    }
}
void ZFUIView::childRemoveAtIndex(ZF_IN zfindex index)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childRemoveAtIndex(index);
    }
    else
    {
        d->childRemoveAtIndex(ZFUIViewChildLayer::e_Normal, d->layerNormal, index);
    }
}
void ZFUIView::childRemoveAll(void)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childRemoveAll();
    }
    else
    {
        d->childRemoveAll(ZFUIViewChildLayer::e_Normal, d->layerNormal);
    }
}

void ZFUIView::childMove(ZF_IN zfindex fromIndex, ZF_IN zfindex toIndexOrIndexMax)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childMove(fromIndex, toIndexOrIndexMax);
    }
    else
    {
        d->childMove(ZFUIViewChildLayer::e_Normal, d->layerNormal, fromIndex, toIndexOrIndexMax);
    }
}
void ZFUIView::childMove(ZF_IN ZFUIView *child, ZF_IN zfindex toIndexOrIndexMax)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childMove(child, toIndexOrIndexMax);
    }
    else
    {
        d->childMove(ZFUIViewChildLayer::e_Normal, d->layerNormal, child, toIndexOrIndexMax);
    }
}
void ZFUIView::childReplaceAtIndex(ZF_IN zfindex atIndex, ZF_IN ZFUIView *toReplace)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childReplaceAtIndex(atIndex, toReplace);
    }
    else
    {
        d->childReplace(ZFUIViewChildLayer::e_Normal, d->layerNormal, atIndex, toReplace);
    }
}

zfindex ZFUIView::childCount(void)
{
    if(d->viewDelegate)
    {
        return d->viewDelegate->childCount();
    }
    else
    {
        return d->childCount(d->layerNormal);
    }
}
ZFUIView *ZFUIView::childAtIndex(ZF_IN zfindex index)
{
    if(d->viewDelegate)
    {
        return d->viewDelegate->childAtIndex(index);
    }
    else
    {
        return d->childAtIndex(d->layerNormal, index);
    }
}
zfindex ZFUIView::childFind(ZF_IN ZFUIView *view)
{
    if(d->viewDelegate)
    {
        return d->viewDelegate->childFind(view);
    }
    else
    {
        return d->childFind(d->layerNormal, view);
    }
}
ZFCoreArrayPOD<ZFUIView *> ZFUIView::childArray(void)
{
    if(d->viewDelegate)
    {
        return d->viewDelegate->childArray();
    }
    else
    {
        return d->layerNormal.views;
    }
}
ZFUIViewChildLayerEnum ZFUIView::viewLayer(void)
{
    return d->viewLayer;
}
ZFCoreArrayPOD<ZFUIView *> ZFUIView::childRawArray(void)
{
    ZFCoreArrayPOD<ZFUIView *> ret;
    ret.capacitySet(
        d->layerInternalBg.views.count()
        + d->layerInternalBg.views.count()
        + this->childCount()
        + d->layerInternalFg.views.count());
    ret.addFrom(d->layerInternalImpl.views);
    ret.addFrom(d->layerInternalBg.views);
    ret.addFrom(this->childCount());
    ret.addFrom(d->layerInternalFg.views);
    return ret;
}

// ============================================================
// internal impl views
void ZFUIView::internalImplViewAdd(ZF_IN ZFUIView *view,
                                   ZF_IN_OPT ZFUIViewLayoutParam *layoutParam /* = zfnull */,
                                   ZF_IN_OPT zfbool addAsTopMost /* = zftrue */)
{
    d->childAdd(ZFUIViewChildLayer::e_Impl, d->layerInternalImpl, view, layoutParam, (addAsTopMost ? zfindexMax : 0));
}
void ZFUIView::internalImplViewRemove(ZF_IN ZFUIView *view)
{
    d->childRemove(ZFUIViewChildLayer::e_Impl, d->layerInternalImpl, view);
}
void ZFUIView::internalImplViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < d->childCount(d->layerInternalImpl); ++i)
    {
        ZFUIView *child = d->childAtIndex(d->layerInternalImpl, i);
        if(this->internalViewShouldLayout(child))
        {
            child->layout(
                ZFUIViewLayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->childLayoutParamAtIndex(d->layerInternalImpl, i)));
        }
    }
}
ZFCoreArrayPOD<ZFUIView *> ZFUIView::internalImplViewArray(void)
{
    return d->layerInternalImpl.views;
}

// ============================================================
// internal background views
void ZFUIView::internalBackgroundViewAdd(ZF_IN ZFUIView *view,
                                         ZF_IN_OPT ZFUIViewLayoutParam *layoutParam /* = zfnull */,
                                         ZF_IN_OPT zfbool addAsTopMost /* = zftrue */)
{
    d->childAdd(ZFUIViewChildLayer::e_Background, d->layerInternalBg, view, layoutParam, (addAsTopMost ? zfindexMax : 0));
}
void ZFUIView::internalBackgroundViewRemove(ZF_IN ZFUIView *view)
{
    d->childRemove(ZFUIViewChildLayer::e_Background, d->layerInternalBg, view);
}
void ZFUIView::internalBackgroundViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < d->childCount(d->layerInternalBg); ++i)
    {
        ZFUIView *child = d->childAtIndex(d->layerInternalBg, i);
        if(this->internalViewShouldLayout(child))
        {
            child->layout(
                ZFUIViewLayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->childLayoutParamAtIndex(d->layerInternalBg, i)));
        }
    }
}
ZFCoreArrayPOD<ZFUIView *> ZFUIView::internalBackgroundViewArray(void)
{
    return d->layerInternalBg.views;
}

// ============================================================
// internal foreground views
void ZFUIView::internalForegroundViewAdd(ZF_IN ZFUIView *view,
                                         ZF_IN_OPT ZFUIViewLayoutParam *layoutParam /* = zfnull */,
                                         ZF_IN_OPT zfbool addAsTopMost /* = zftrue */)
{
    d->childAdd(ZFUIViewChildLayer::e_Foreground, d->layerInternalFg, view, layoutParam, (addAsTopMost ? zfindexMax : 0));
}
void ZFUIView::internalForegroundViewRemove(ZF_IN ZFUIView *view)
{
    d->childRemove(ZFUIViewChildLayer::e_Foreground, d->layerInternalFg, view);
}
void ZFUIView::internalForegroundViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < d->childCount(d->layerInternalFg); ++i)
    {
        ZFUIView *child = d->childAtIndex(d->layerInternalFg, i);
        if(this->internalViewShouldLayout(child))
        {
            child->layout(
                ZFUIViewLayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->childLayoutParamAtIndex(d->layerInternalFg, i)));
        }
    }
}
ZFCoreArrayPOD<ZFUIView *> ZFUIView::internalForegroundViewArray(void)
{
    return d->layerInternalFg.views;
}

// ============================================================
// other internal view logic
void ZFUIView::internalViewAutoSerializeTagAdd(ZF_IN const zfchar *tag)
{
    if(zfscmpTheSame(tag, zfText("")))
    {
        return ;
    }
    d->internalViewAutoSerializeTags[tag] = zftrue;
}
void ZFUIView::internalViewAutoSerializeTagRemove(ZF_IN const zfchar *tag)
{
    if(zfscmpTheSame(tag, zfText("")))
    {
        return ;
    }
    d->internalViewAutoSerializeTags.erase(tag);
}
void ZFUIView::internalViewAutoSerializeTagRemoveAll(void)
{
    d->internalViewAutoSerializeTags.clear();
}
void ZFUIView::internalViewAutoSerializeTagGetAll(ZF_OUT ZFCoreArray<zfstring> &ret)
{
    ret.capacitySet(ret.count() + d->internalViewAutoSerializeTags.size());
    for(_ZFP_ZFUIViewInternalViewAutoSerializeTagMapType::iterator it = d->internalViewAutoSerializeTags.begin();
        it != d->internalViewAutoSerializeTags.end();
        ++it)
    {
        ret.add(it->first.c_str());
    }
}

zfbool ZFUIView::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    return (internalView != d->viewDelegate);
}

// ============================================================
// events
void ZFUIView::viewEventSend(ZF_IN ZFUIEvent *event)
{
    if(event == zfnull)
    {
        return ;
    }

    zfRetainWithoutLeakTest(this);
    zfblockedReleaseWithoutLeakTest(this);

    this->viewEventOnEventFilter(event);
    this->observerNotify(ZFUIView::EventViewOnEventFilter(), event);

    if(event->eventResolved())
    {
        return ;
    }

    this->viewEventOnEvent(event);
    this->observerNotify(ZFUIView::EventViewOnEvent(), event);
}
void ZFUIView::viewEventOnEvent(ZF_IN ZFUIEvent *event)
{
    const ZFClass *eventClass = event->classData();
    if(eventClass->classIsSubclassOf(ZFUIMouseEvent::ClassData()))
    {
        ZFUIMouseEvent *mouseEvent = ZFCastZFObjectUnchecked(ZFUIMouseEvent *, event);
        this->viewEventOnMouseEvent(mouseEvent);
        mouseEvent->eventResolvedSet(zftrue);
    }
    else if(eventClass->classIsSubclassOf(ZFUIKeyEvent::ClassData()))
    {
        ZFUIKeyEvent *eventTmp = ZFCastZFObjectUnchecked(ZFUIKeyEvent *, event);
        this->viewEventOnKeyEvent(eventTmp);
        {
            ZFUIView *view = this->viewParent();
            while(view != zfnull && !event->eventResolved())
            {
                view->viewEventOnKeyEvent(eventTmp);
                view = view->viewParent();
            }
        }
    }
    else if(eventClass->classIsSubclassOf(ZFUIWheelEvent::ClassData()))
    {
        ZFUIWheelEvent *eventTmp = ZFCastZFObjectUnchecked(ZFUIWheelEvent *, event);
        this->viewEventOnWheelEvent(eventTmp);
        {
            ZFUIView *view = this->viewParent();
            while(view != zfnull && !event->eventResolved())
            {
                view->viewEventOnWheelEvent(eventTmp);
                view = view->viewParent();
            }
        }
    }
}

void ZFUIView::viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent)
{
    if(this->viewUIEnable())
    {
        if(mouseEvent->mouseAction == ZFUIMouseAction::e_MouseDown
            && this->viewFocusable()
            && this->viewFocusObtainWhenClick())
        {
            this->viewFocusRequest(zftrue);
        }
        mouseEvent->eventResolvedSet(zftrue);
    }
}

void ZFUIView::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
{
    this->viewEventOnKeyEventResolveFocus(keyEvent);
}
void ZFUIView::viewEventOnKeyEventResolveFocus(ZF_IN ZFUIKeyEvent *keyEvent)
{
    ZFUIViewFocusResolveKeyEvent(this, keyEvent);
}

void ZFUIView::viewEventOnWheelEvent(ZF_IN ZFUIWheelEvent *wheelEvent)
{
    // nothing to do
}

// ============================================================
// view property async update
void ZFUIView::viewPropertyUpdateRequest(void)
{
    ZFThreadTaskRequest(
        zfHint("task")ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewListenerHolder)->viewPropertyOnUpdateListener,
        zfHint("userData")this->objectHolder(),
        zfHint("param0")zfnull,
        zfHint("param1")zfnull,
        zfHint("owner")this,
        zfHint("mergeCallback")ZFThreadTaskRequestMergeCallbackIgnoreOldTask);
}

ZF_NAMESPACE_GLOBAL_END

