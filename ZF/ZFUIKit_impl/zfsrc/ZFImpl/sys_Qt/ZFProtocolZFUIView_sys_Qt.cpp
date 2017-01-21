/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIKeyboardState.h"

#include "ZFImpl_sys_Qt_QLayout.h"

#if ZF_ENV_sys_Qt

#include <QWidget>
#include <QLayout>
#include <QChildEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QCoreApplication>
#include <QGraphicsOpacityEffect>
#include <QSet>

// ============================================================
extern ZFUIKeyCodeEnum ZFUIViewImpl_sys_Qt_ZFUIKeyCodeFromQKeyCode(ZF_IN int qKeyCode);
extern zfbool ZFUIViewImpl_sys_Qt_isMouseCancel(ZF_IN QMouseEvent *event);

static zfbool _ZFP_ZFImpl_sys_Qt_mouseTracking(ZF_IN QWidget *view);
static void _ZFP_ZFImpl_sys_Qt_mouseTrackingSet(ZF_IN QWidget *view, ZF_IN zfbool mouseTracking);

class _ZFP_ZFUIViewImpl_sys_Qt_QLayout : public ZFImpl_sys_Qt_QLayout
{
    Q_OBJECT

public:
    ZFUIView *ownerZFUIView;

public:
    _ZFP_ZFUIViewImpl_sys_Qt_QLayout(void)
    : ownerZFUIView(zfnull)
    {
    }

public:
    virtual void setGeometry(const QRect &rect)
    {
        QLayout::setGeometry(rect);
        QWidget *nativeImplView = ZFCastStatic(QWidget *, this->ownerZFUIView->nativeImplView());
        for(zfindex i = 0; i < this->layoutItemList.count(); ++i)
        {
            const ZFImpl_sys_Qt_QLayoutItemHolder &item = this->layoutItemList[i];
            if(item.widget == nativeImplView)
            {
                ZFUIRect nativeImplViewRect;
                ZFPROTOCOL_ACCESS(ZFUIView)->notifyLayoutNativeImplView(this->ownerZFUIView, nativeImplViewRect);
                QRect t = ZFImpl_sys_Qt_ZFUIKit_ZFUIRectToQRect(nativeImplViewRect);
                if(item.layoutItem->geometry() != t)
                {
                    item.layoutItem->setGeometry(t);
                }
            }
        }
    }
};

class _ZFP_ZFUIViewImpl_sys_Qt_ChildChangeObserverHolder : public QObject
{
    Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        if(event->type() != QEvent::ChildAdded && event->type() != QEvent::ChildRemoved)
        {
            return QObject::eventFilter(obj, event);
        }

        QWidget *parent = qobject_cast<QWidget *>(obj);
        if(parent == NULL)
        {
            return QObject::eventFilter(obj, event);
        }
        if(_ZFP_ZFImpl_sys_Qt_mouseTracking(parent))
        {
            QChildEvent *childEvent = (QChildEvent *)event;
            QWidget *child = qobject_cast<QWidget *>(childEvent->child());
            if(child != NULL)
            {
                _ZFP_ZFImpl_sys_Qt_mouseTrackingSet(child, childEvent->added());
            }
        }

        return QObject::eventFilter(obj, event);
    }
};

// ============================================================
// focus
extern void *_ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_setup(ZF_IN ZFUIView *ownerZFUIView,
                                                       ZF_IN QWidget *nativeOwner,
                                                       ZF_IN QWidget *nativeImplViewOrNull);
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(ZF_IN void *token);
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusableSet(ZF_IN void *token, ZF_IN zfbool v);
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusRequest(ZF_IN void *token, ZF_IN zfbool v);
extern zfbool _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocused(ZF_IN void *token);

// ============================================================
// native view
class _ZFP_ZFUIViewImpl_sys_Qt_View : public QWidget
{
    Q_OBJECT

public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *_ZFP_impl;
    ZFUIView *_ZFP_ownerZFUIView;
    _ZFP_ZFUIViewImpl_sys_Qt_QLayout *_ZFP_layoutProxy;
    void *_ZFP_focusProxyToken;
    QWidget *_ZFP_nativeImplView;
    zfbool _ZFP_viewUIEnable;
    zfbool _ZFP_viewUIEnableTree;
    zfbool _ZFP_mousePressed;
    zfbool _ZFP_mouseEnterFlag;
    ZFUIPoint _ZFP_mouseMoveLastPoint;
    zfbool _ZFP_layoutRequested;

public:
    _ZFP_ZFUIViewImpl_sys_Qt_View(void) : QWidget()
    , _ZFP_impl(zfnull)
    , _ZFP_ownerZFUIView(zfnull)
    , _ZFP_layoutProxy(new _ZFP_ZFUIViewImpl_sys_Qt_QLayout())
    , _ZFP_focusProxyToken(zfnull)
    , _ZFP_nativeImplView(zfnull)
    , _ZFP_viewUIEnable(zftrue)
    , _ZFP_viewUIEnableTree(zftrue)
    , _ZFP_mousePressed(zffalse)
    , _ZFP_mouseEnterFlag(zffalse)
    , _ZFP_mouseMoveLastPoint(ZFUIPointZero)
    , _ZFP_layoutRequested(zftrue)
    {
        this->setLayout(_ZFP_layoutProxy);

        QPalette palette = this->palette();
        palette.setColor(QPalette::Background, QColor(0, 0, 0, 0));
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    }
    ~_ZFP_ZFUIViewImpl_sys_Qt_View(void)
    {
        zfCoreAssert(this->_ZFP_nativeImplView == zfnull);
    }

public:
    void _ZFP_frameSet(ZF_IN const ZFUIRect &v)
    {
        QRect frame = ZFImpl_sys_Qt_ZFUIKit_ZFUIRectToQRect(v);
        if(this->geometry() != frame)
        {
            this->setGeometry(frame);
        }
    }
    void _ZFP_viewUIEnableSet(ZF_IN zfbool v)
    {
        _ZFP_viewUIEnable = v;
    }
    void _ZFP_viewUIEnableTreeSet(ZF_IN zfbool v)
    {
        _ZFP_viewUIEnableTree = v;
        this->setEnabled(_ZFP_viewUIEnableTree);
    }

public:
    bool event(QEvent *event)
    {
        if(_ZFP_ownerZFUIView == zfnull)
        {
            return QWidget::event(event);
        }
        switch(event->type())
        {
            // mouse
            case QEvent::MouseButtonPress:
            case QEvent::MouseMove:
            case QEvent::MouseButtonRelease:
                if(!this->_ZFP_viewUIEnableTree)
                {
                    return false;
                }
                if(!this->_ZFP_viewUIEnable)
                {
                    QMouseEvent *mouseEvent = (QMouseEvent *)event;
                    if(this->childAt(mouseEvent->pos()) == zfnull)
                    {
                        return false;
                    }
                }
                return QWidget::event(event);

            // key
            case QEvent::KeyPress:
            case QEvent::KeyRelease:
                if(!this->_ZFP_viewUIEnableTree)
                {
                    return false;
                }
                return QWidget::event(event);

            // wheel
            case QEvent::Wheel:
            {
                QWheelEvent *eventTmp = (QWheelEvent *)event;
                ZFCACHEABLE_ACCESS(ZFUIWheelEvent, ZFUIWheelEvent, wheelEvent);
                QPoint eventSteps = eventTmp->angleDelta() / 8 / 15;
                wheelEvent->wheelX = -eventSteps.x();
                wheelEvent->wheelY = -eventSteps.y();
                if(wheelEvent->wheelX != 0 || wheelEvent->wheelY != 0)
                {
                    this->_ZFP_impl->notifyUIEvent(this->_ZFP_ownerZFUIView, wheelEvent);
                }
            }
                return true;

            // layout
            case QEvent::LayoutRequest:
            case QEvent::Resize:
                if(_ZFP_ownerZFUIView != zfnull
                    && _ZFP_ownerZFUIView->viewParent() == zfnull)
                {
                    if(!_ZFP_layoutRequested)
                    {
                        _ZFP_layoutRequested = zftrue;
                        _ZFP_impl->notifyNeedLayout(_ZFP_ownerZFUIView);
                    }
                    if(event->type() == QEvent::LayoutRequest)
                    {
                        QCoreApplication::processEvents();
                    }
                    if(_ZFP_layoutRequested)
                    {
                        _ZFP_impl->notifyLayoutRootView(_ZFP_ownerZFUIView, ZFImpl_sys_Qt_ZFUIKit_ZFUIRectFromQRect(this->geometry()));
                        _ZFP_layoutRequested = zffalse;
                    }
                    return true;
                }
                return QWidget::event(event);

            // default
            default:
                return QWidget::event(event);
        }
    }

public:
    virtual void mousePressEvent(QMouseEvent *event)
    {
        this->_ZFP_mousePressed = zftrue;
        if(_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QWidget::mousePressEvent(event);
            return ;
        }
        this->mouseEventResolve(event, ZFUIMouseAction::e_MouseDown);
        QWidget::mousePressEvent(event);
    }
    virtual void mouseMoveEvent(QMouseEvent *event)
    {
        this->_ZFP_mouseMoveLastPoint = ZFImpl_sys_Qt_ZFUIKit_ZFUIPointFromQPoint(event->pos());
        if(_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QWidget::mouseMoveEvent(event);
            return ;
        }
        if(this->_ZFP_mousePressed)
        {
            this->mouseEventResolve(event, ZFUIMouseAction::e_MouseMove);
        }
        else if(this->_ZFP_ownerZFUIView->viewMouseHoverEventEnable())
        {
            if(this->_ZFP_mouseEnterFlag)
            {
                this->_ZFP_mouseEnterFlag = zffalse;
                this->mouseHoverEventResolve(this->_ZFP_mouseMoveLastPoint, ZFUIMouseAction::e_MouseHoverEnter);
            }
            else
            {
                this->mouseHoverEventResolve(this->_ZFP_mouseMoveLastPoint, ZFUIMouseAction::e_MouseHover);
            }
        }
        QWidget::mouseMoveEvent(event);
    }
    virtual void mouseReleaseEvent(QMouseEvent *event)
    {
        this->_ZFP_mousePressed = zffalse;
        if(_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QWidget::mouseReleaseEvent(event);
            return ;
        }
        if(ZFUIViewImpl_sys_Qt_isMouseCancel(event))
        {
            this->mouseEventResolve(event, ZFUIMouseAction::e_MouseCancel);
        }
        else
        {
            this->mouseEventResolve(event, ZFUIMouseAction::e_MouseUp);
        }
        QWidget::mouseReleaseEvent(event);
    }
    virtual void enterEvent(QEvent *event)
    {
        this->_ZFP_mouseEnterFlag = zftrue;
        QWidget::enterEvent(event);
    }
    virtual void leaveEvent(QEvent *event)
    {
        if(_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            this->_ZFP_mouseEnterFlag = zffalse;
        }
        else
        {
            if(this->_ZFP_ownerZFUIView->viewMouseHoverEventEnable())
            {
                if(this->_ZFP_mouseEnterFlag)
                {
                    this->_ZFP_mouseEnterFlag = zffalse;
                    this->mouseHoverEventResolve(this->_ZFP_mouseMoveLastPoint, ZFUIMouseAction::e_MouseHoverEnter);
                }
                this->mouseHoverEventResolve(this->_ZFP_mouseMoveLastPoint, ZFUIMouseAction::e_MouseHoverExit);
            }
            this->_ZFP_mouseEnterFlag = zffalse;
        }
        QWidget::leaveEvent(event);
    }

private:
    void mouseEventResolve(QMouseEvent *event, ZFUIMouseActionEnum mouseAction)
    {
        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, ev);
        ev->mouseId = (zfidentity)event->button();
        ev->mouseAction = mouseAction;
        ev->mousePoint = ZFImpl_sys_Qt_ZFUIKit_ZFUIPointFromQPoint(event->pos());
        switch(event->button())
        {
            case Qt::RightButton:
                ev->mouseButton = ZFUIMouseButton::e_MouseButtonRight;
                break;
            case Qt::MiddleButton:
                ev->mouseButton = ZFUIMouseButton::e_MouseButtonCenter;
                break;
            case Qt::LeftButton:
            default:
                ev->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
                break;
        }

        this->_ZFP_impl->notifyUIEvent(this->_ZFP_ownerZFUIView, ev);
    }
    void mouseHoverEventResolve(const ZFUIPoint &pos, ZFUIMouseActionEnum mouseAction)
    {
        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, ev);
        ev->mouseId = 0;
        ev->mouseAction = mouseAction;
        ev->mousePoint = pos;
        ev->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
        this->_ZFP_impl->notifyUIEvent(this->_ZFP_ownerZFUIView, ev);
    }

public:
    virtual void keyPressEvent(QKeyEvent *event)
    {
        if(_ZFP_ownerZFUIView == zfnull)
        {
            QWidget::keyPressEvent(event);
            return ;
        }
        this->keyEventResolve(event, event->isAutoRepeat() ? ZFUIKeyAction::e_KeyRepeat : ZFUIKeyAction::e_KeyDown);
    }
    virtual void keyReleaseEvent(QKeyEvent *event)
    {
        if(_ZFP_ownerZFUIView == zfnull)
        {
            QWidget::keyReleaseEvent(event);
            return ;
        }
        this->keyEventResolve(event, event->isAutoRepeat() ? ZFUIKeyAction::e_KeyRepeat : ZFUIKeyAction::e_KeyUp);
    }
private:
    void keyEventResolve(QKeyEvent *event, ZFUIKeyActionEnum keyAction)
    {
        if(this->_ZFP_viewUIEnableTree && this->_ZFP_viewUIEnable)
        {
            ZFCACHEABLE_ACCESS(ZFUIKeyEvent, ZFUIKeyEvent, ev);
            ev->keyId = (zfidentity)event->key();
            ev->keyAction = keyAction;
            ev->keyCode = ZFUIViewImpl_sys_Qt_ZFUIKeyCodeFromQKeyCode(event->key());
            ev->keyCodeRaw = (zfflags)event->key();
            this->_ZFP_impl->notifyUIEvent(this->_ZFP_ownerZFUIView, ev);
            event->setAccepted(ev->eventResolved());
        }
    }

protected:
    virtual bool focusNextPrevChild(bool next)
    {
        return false;
    }
};

// ============================================================
// impl
ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_Qt, ZFUIView, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt:QWidget"))

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        QCoreApplication::instance()->installEventFilter(&(this->childChangeObserverHolder));
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        QCoreApplication::instance()->removeEventFilter(&(this->childChangeObserverHolder));
        zfsuper::protocolOnDealloc();
    }

    zfoverride
    virtual void protocolOnInitFinish(void)
    {
        zfsuper::protocolOnInitFinish();
        ZFUIKeyboardStateBuiltinImplRegister();
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        ZFUIKeyboardStateBuiltinImplUnregister();
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = new _ZFP_ZFUIViewImpl_sys_Qt_View();
        nativeView->_ZFP_impl = this;
        nativeView->_ZFP_ownerZFUIView = view;
        nativeView->_ZFP_layoutProxy->ownerZFUIView = view;
        nativeView->_ZFP_focusProxyToken = _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_setup(nativeView->_ZFP_ownerZFUIView, nativeView, zfnull);
        return nativeView;
    }
    virtual void nativeViewDestroy(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeView)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, nativeView);
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(nativeViewTmp->_ZFP_focusProxyToken);
        delete nativeViewTmp;
    }

    virtual void nativeImplViewSet(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeImplViewOld,
                                   ZF_IN void *nativeImplView,
                                   ZF_IN zfindex virtualIndex)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        QWidget *v = ZFCastStatic(QWidget*, nativeImplView);

        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(nativeView->_ZFP_focusProxyToken);

        if(nativeView->_ZFP_nativeImplView != zfnull)
        {
            nativeView->_ZFP_layoutProxy->childRemove(virtualIndex);
        }
        nativeView->_ZFP_nativeImplView = v;
        if(nativeView->_ZFP_nativeImplView != zfnull)
        {
            nativeView->_ZFP_layoutProxy->childAdd(zfnull, nativeView->_ZFP_nativeImplView, virtualIndex);
        }

        nativeView->_ZFP_focusProxyToken = _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_setup(view, nativeView, nativeView->_ZFP_nativeImplView);
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusableSet(nativeView->_ZFP_focusProxyToken, view->viewFocusable());
    }
    virtual zffloat nativeViewScaleForImpl(ZF_IN void *nativeView)
    {
        return 1;
    }
    virtual zffloat nativeViewScaleForPhysicalPixel(ZF_IN void *nativeView)
    {
        return 1;
    }

    // ============================================================
    // properties
public:
    virtual void viewVisibleSet(ZF_IN ZFUIView *view,
                                ZF_IN zfbool viewVisible)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View  *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        // set to visible when no parent would cause QWidget changed to a window
        // for this case, we would delay until added to parent
        if(nativeView->parentWidget() != zfnull)
        {
            nativeView->setVisible(viewVisible);
        }
    }
    virtual void viewAlphaSet(ZF_IN ZFUIView *view,
                              ZF_IN zffloat viewAlpha)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        if(viewAlpha == 1)
        {
            nativeViewTmp->setGraphicsEffect(zfnull);
        }
        else
        {
            QGraphicsOpacityEffect *effect = qobject_cast<QGraphicsOpacityEffect *>(nativeViewTmp->graphicsEffect());
            if(effect == zfnull)
            {
                effect = new QGraphicsOpacityEffect(nativeViewTmp);
                nativeViewTmp->setGraphicsEffect(effect);
            }
            effect->setOpacity(viewAlpha);
        }
    }
    virtual void viewUIEnableSet(ZF_IN ZFUIView *view,
                                 ZF_IN zfbool viewUIEnable)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        nativeViewTmp->_ZFP_viewUIEnableSet(viewUIEnable);
    }
    virtual void viewUIEnableTreeSet(ZF_IN ZFUIView *view,
                                     ZF_IN zfbool viewUIEnableTree)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        nativeViewTmp->_ZFP_viewUIEnableTreeSet(viewUIEnableTree);
    }
    virtual void viewMouseHoverEventEnableSet(ZF_IN ZFUIView *view,
                                              ZF_IN zfbool viewMouseHoverEventEnable)
    {
        _ZFP_ZFImpl_sys_Qt_mouseTrackingSet(ZFCastStatic(QWidget *, view->nativeView()), viewMouseHoverEventEnable);
    }
    virtual void viewBackgroundColorSet(ZF_IN ZFUIView *view,
                                        ZF_IN const ZFUIColor &viewBackgroundColor)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        QPalette palette = nativeViewTmp->palette();
        palette.setColor(QPalette::Background, ZFImpl_sys_Qt_ZFUIKit_ZFUIColorToQColor(viewBackgroundColor));
        nativeViewTmp->setPalette(palette);
    }

public:
    virtual void childAdd(ZF_IN ZFUIView *parent,
                          ZF_IN ZFUIView *child,
                          ZF_IN zfindex virtualIndex,
                          ZF_IN ZFUIViewChildLayerEnum childLayer,
                          ZF_IN zfindex childLayerIndex)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, parent->nativeView());
        QWidget *nativeChildView = ZFCastStatic(QWidget *, child->nativeView());
        nativeView->_ZFP_layoutProxy->childAdd(child, nativeChildView, virtualIndex);
    }
    virtual void childRemove(ZF_IN ZFUIView *parent,
                             ZF_IN ZFUIView *child,
                             ZF_IN zfindex virtualIndex,
                             ZF_IN ZFUIViewChildLayerEnum childLayer,
                             ZF_IN zfindex childLayerIndex)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, parent->nativeView());
        nativeView->_ZFP_layoutProxy->childRemove(virtualIndex);
    }

public:
    virtual void viewFrameSet(ZF_IN ZFUIView *view,
                              ZF_IN const ZFUIRect &rect)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        nativeViewTmp->_ZFP_frameSet(rect);
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view)
    {
        QWidget *nativeViewTmp = ZFCastStatic(QWidget *, view->nativeView());
        while(nativeViewTmp != zfnull)
        {
            if(nativeViewTmp->layout() != zfnull)
            {
                nativeViewTmp->layout()->invalidate();
            }
            nativeViewTmp = nativeViewTmp->parentWidget();
        }
    }

    virtual void measureNativeView(ZF_OUT ZFUISize &ret,
                                   ZF_IN void *nativeView,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
    {
        QWidget *nativeViewTmp = ZFCastStatic(QWidget *, nativeView);
        QSize maxSizeSaved = nativeViewTmp->maximumSize();
        if(sizeHint.width >= 0)
        {
            nativeViewTmp->setMaximumWidth(sizeHint.width);
        }
        if(sizeHint.height >= 0)
        {
            nativeViewTmp->setMaximumWidth(sizeHint.height);
        }
        QSize t = nativeViewTmp->sizeHint();
        nativeViewTmp->setMaximumSize(maxSizeSaved);
        ret = ZFImpl_sys_Qt_ZFUIKit_ZFUISizeFromQSize(t);
    }
private:
    _ZFP_ZFUIViewImpl_sys_Qt_ChildChangeObserverHolder childChangeObserverHolder;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewImpl_sys_Qt)

// ============================================================
// ZFUIViewFocus
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewFocusImpl_sys_Qt, ZFUIViewFocus, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt:QWidget"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("Qt:QWidget"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewFocusableSet(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocusable)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusableSet(nativeView->_ZFP_focusProxyToken, viewFocusable);
    }
    virtual zfbool viewFocused(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        return _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocused(nativeView->_ZFP_focusProxyToken);
    }
    virtual void viewFocusRequest(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocus)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusRequest(nativeView->_ZFP_focusProxyToken, viewFocus);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewFocusImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewFocusImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

const zfchar *_ZFP_ZFImpl_ZFUIView_mouseTrackingCount = zfText("_ZFP_ZFImpl_ZFUIView_mouseTrackingCount");
const zfchar *_ZFP_ZFImpl_ZFUIView_mouseTrackingSaved = zfText("_ZFP_ZFImpl_ZFUIView_mouseTrackingSaved");
static zfbool _ZFP_ZFImpl_sys_Qt_mouseTracking(ZF_IN QWidget *view)
{
    return ZFImpl_sys_Qt_QObjectTagGet(view, _ZFP_ZFImpl_ZFUIView_mouseTrackingCount).isValid();
}
static void _ZFP_ZFImpl_sys_Qt_mouseTrackingSet(ZF_IN QWidget *view, ZF_IN zfbool mouseTracking)
{
    zfindex countSaved = 0;
    zfbool valueSaved = zffalse;
    {
        QVariant tmp = ZFImpl_sys_Qt_QObjectTagGet(view, _ZFP_ZFImpl_ZFUIView_mouseTrackingCount);
        if(tmp.isValid())
        {
            countSaved = tmp.toInt();
            tmp = ZFImpl_sys_Qt_QObjectTagGet(view, _ZFP_ZFImpl_ZFUIView_mouseTrackingSaved);
            if(tmp.isValid())
            {
                valueSaved = tmp.toBool();
            }
        }
    }
    if(mouseTracking)
    {
        ++countSaved;
        if(countSaved == 1)
        {
            valueSaved = view->hasMouseTracking();
        }
        view->setMouseTracking(true);
    }
    else
    {
        --countSaved;
        if(countSaved == 0)
        {
            view->setMouseTracking(valueSaved);
        }
    }
    if(countSaved > 0)
    {
        ZFImpl_sys_Qt_QObjectTagSet(view, _ZFP_ZFImpl_ZFUIView_mouseTrackingCount, QVariant::fromValue((int)countSaved));
        ZFImpl_sys_Qt_QObjectTagSet(view, _ZFP_ZFImpl_ZFUIView_mouseTrackingSaved, QVariant::fromValue((bool)valueSaved));
    }
    else
    {
        ZFImpl_sys_Qt_QObjectTagSet(view, _ZFP_ZFImpl_ZFUIView_mouseTrackingCount, QVariant());
        ZFImpl_sys_Qt_QObjectTagSet(view, _ZFP_ZFImpl_ZFUIView_mouseTrackingSaved, QVariant());
    }

    const QObjectList &children = view->children();
    for(int i = children.count() - 1; i != -1; --i)
    {
        QWidget *child = qobject_cast<QWidget *>(children.at(i));
        if(child != NULL)
        {
            _ZFP_ZFImpl_sys_Qt_mouseTrackingSet(child, mouseTracking);
        }
    }
}

#include "ZFProtocolZFUIView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

