/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFUIView.h
 * @brief protocol for ZFUIView
 */

#ifndef _ZFI_ZFProtocolZFUIView_h_
#define _ZFI_ZFProtocolZFUIView_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIView)
public:
    /**
     * @brief called when a ZFUIView created,
     *   to register native view to ZFUIView
     *
     * return a native view token,
     * which is automatically saved to ZFUIView.nativeView
     */
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view) = 0;
    /**
     * @brief called when a ZFUIView destroyed,
     *   the internal nativeView could be accessed by ZFUIView.nativeView
     *
     * it's ensured that view has been remove from parent before destroy
     */
    virtual void nativeViewDestroy(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeView) = 0;

    /**
     * @brief attach a native view to this view, see #ZFUINativeViewWrapper
     *
     * set null to remove the attached native view,
     * and it's ensured set null before view destroy\n
     * \n
     * nativeImplView has two use:
     * -  used to store native view for #ZFUINativeViewWrapper
     * -  used to store native view for different view's implementation
     *   such as EditText
     */
    virtual void nativeImplViewSet(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeImplViewOld,
                                   ZF_IN void *nativeImplView,
                                   ZF_IN zfindex virtualIndex) = 0;
    /**
     * @brief see #ZFUIView::nativeImplViewMarginUpdate
     */
    virtual void nativeImplViewMarginSet(ZF_IN ZFUIView *view,
                                         ZF_IN const ZFUIMargin &nativeImplViewMargin)
    {
        // optional
    }

    /**
     * @brief get proper scale for the view
     */
    virtual zffloat nativeViewScaleForImpl(ZF_IN void *nativeView) = 0;
    /**
     * @brief get proper scale for physical pixel
     */
    virtual zffloat nativeViewScaleForPhysicalPixel(ZF_IN void *nativeView) = 0;

    // ============================================================
    // properties
public:
    /** @brief see #ZFUIView */
    virtual void viewVisibleSet(ZF_IN ZFUIView *view,
                                ZF_IN zfbool viewVisible) = 0;
    /** @brief see #ZFUIView */
    virtual void viewAlphaSet(ZF_IN ZFUIView *view,
                              ZF_IN zffloat viewAlpha) = 0;
    /** @brief see #ZFUIView */
    virtual void viewUIEnableSet(ZF_IN ZFUIView *view,
                                 ZF_IN zfbool viewUIEnable) = 0;
    /** @brief see #ZFUIView */
    virtual void viewUIEnableTreeSet(ZF_IN ZFUIView *view,
                                     ZF_IN zfbool viewUIEnableTree) = 0;
    /** @brief see #ZFUIView */
    virtual void viewMouseHoverEventEnableSet(ZF_IN ZFUIView *view,
                                              ZF_IN zfbool viewMouseHoverEventEnable)
    {
        // no hover event support by default
    }
    /** @brief see #ZFUIView */
    virtual void viewBackgroundColorSet(ZF_IN ZFUIView *view,
                                        ZF_IN const ZFUIColor &viewBackgroundColor) = 0;

    // ============================================================
    // children
public:
    /**
     * @brief add view, no need to worry about layout param or internal views
     *
     * you should not retain or release those views\n
     * virtualIndex shows the absolute index of the child,
     * impl should not care about the actual meanning of this index,
     * simply add to the specified index
     */
    virtual void childAdd(ZF_IN ZFUIView *parent,
                          ZF_IN ZFUIView *child,
                          ZF_IN zfindex virtualIndex,
                          ZF_IN ZFUIViewChildLayerEnum childLayer,
                          ZF_IN zfindex childLayerIndex) = 0;
    /**
     * @brief remove view, no need to worry about layout param or ZFUIView's internal views
     *
     * see #childAdd
     */
    virtual void childRemove(ZF_IN ZFUIView *parent,
                             ZF_IN ZFUIView *child,
                             ZF_IN zfindex virtualIndex,
                             ZF_IN ZFUIViewChildLayerEnum childLayer,
                             ZF_IN zfindex childLayerIndex) = 0;

    // ============================================================
    // frame and layout
public:
    /**
     * @brief set view's frame, no need to worry about layout param or auto resizing
     */
    virtual void viewFrameSet(ZF_IN ZFUIView *view,
                              ZF_IN const ZFUIRect &rect) = 0;
    /**
     * @brief called by ZFUIView to notify the implementation that the view needs layout
     *
     * implementations should ensure next layout step would layout all views from root to children,
     * otherwise, you should request layout recursively from child to all of its parent\n
     * \n
     * this method is called by ZFUIView to notify implementations,
     * and #notifyNeedLayout is called by implementations to notify ZFUIView,
     * take good care of them
     */
    virtual void layoutRequest(ZF_IN ZFUIView *view) = 0;

    /**
     * @brief measure a native view, only called by #ZFUINativeViewWrapper
     */
    virtual void measureNativeView(ZF_OUT ZFUISize &ret,
                                   ZF_IN void *nativeView,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam) = 0;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementations must notify when native view needs layout
     *   if view's parent isn't ZFUIView
     *
     * typically, you should call this method only if the view's parent is not type of ZFUIView
     */
    zffinal void notifyNeedLayout(ZF_IN ZFUIView *view)
    {
        view->layoutRequest();
    }
    /**
     * @brief implementations must notify when need layout if view's parent isn't ZFUIView
     *
     * typically, you should call this method only if the view's parent is not type of ZFUIView
     */
    zffinal void notifyLayoutRootView(ZF_IN ZFUIView *view,
                                      ZF_IN const ZFUIRect &rect)
    {
        view->_ZFP_ZFUIView_notifyLayoutRootView(ZFUIRectApplyScaleReversely(rect, view->scaleGetFixed()));
    }
    /**
     * @brief implementations must notify just before layout native impl view
     */
    zffinal void notifyLayoutNativeImplView(ZF_IN ZFUIView *view,
                                            ZF_IN_OUT ZFUIRect &result)
    {
        view->_ZFP_ZFUIView_notifyLayoutNativeImplView(result);
        result = ZFUIRectApplyScale(result, view->scaleGetFixed());
    }
    /**
     * @brief implementation must notify when UI event occurred
     *
     * here is a list of UI events you may want to take care:
     * -  #ZFUIMouseEvent
     * -  #ZFUIKeyEvent
     */
    zffinal void notifyUIEvent(ZF_IN ZFUIView *view,
                               ZF_IN ZFUIEvent *uiEvent)
    {
        uiEvent->_ZFP_ZFUIEvent_eventOnApplyScaleReversely(view->scaleGetFixed());
        view->viewEventSend(uiEvent);
    }
ZFPROTOCOL_INTERFACE_END(ZFUIView)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIView_h_

