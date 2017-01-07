/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFUIKit.h"

#include "ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"

#if ZF_ENV_sys_Qt

#include <QWidget>
#include <QEvent>

class _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken : public QObject
{
    Q_OBJECT

public:
    ZFUIView *ownerZFUIView;
    QWidget *nativeOwner;
    QWidget *nativeImplView;

public:
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken(ZF_IN ZFUIView *ownerZFUIView,
                                            ZF_IN QWidget *nativeOwner,
                                            ZF_IN QWidget *nativeImplView)
    : QObject()
    , ownerZFUIView(ownerZFUIView)
    , nativeOwner(nativeOwner)
    , nativeImplView(nativeImplView)
    {
        this->nativeOwner->installEventFilter(this);
        if(this->nativeImplView != zfnull)
        {
            this->nativeImplView->installEventFilter(this);
        }
    }
    ~_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken(void)
    {
        this->nativeOwner->removeEventFilter(this);
        if(this->nativeImplView != zfnull)
        {
            this->nativeImplView->removeEventFilter(this);
        }
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        static ZFUIView *_lastFocused = zfnull;
        switch(event->type())
        {
            case QEvent::FocusIn:
                if(this->ownerZFUIView != _lastFocused)
                {
                    _lastFocused = this->ownerZFUIView;
                    ZFPROTOCOL_ACCESS(ZFUIViewFocus)->notifyViewFocusChanged(this->ownerZFUIView);
                }
                break;
            case QEvent::FocusOut:
                if(this->ownerZFUIView == _lastFocused)
                {
                    _lastFocused = zfnull;
                    ZFPROTOCOL_ACCESS(ZFUIViewFocus)->notifyViewFocusChanged(this->ownerZFUIView);
                }
                break;
            default:
                break;
        }
        return false;
    }
};

void *_ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_setup(ZF_IN ZFUIView *ownerZFUIView,
                                                ZF_IN QWidget *nativeOwner,
                                                ZF_IN QWidget *nativeImplViewOrNull)
{
    return new _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken(ownerZFUIView, nativeOwner, nativeImplViewOrNull);
}
void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(ZF_IN void *token)
{
    delete ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
}
void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusableSet(ZF_IN void *token, ZF_IN zfbool v)
{
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *t = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
    if(v)
    {
        t->nativeOwner->setFocusPolicy(Qt::ClickFocus);
        if(t->nativeImplView != zfnull &&  t->nativeImplView->focusPolicy() != Qt::NoFocus)
        {
            t->nativeImplView->setFocusPolicy(Qt::ClickFocus);
            t->nativeOwner->setFocusProxy(t->nativeImplView);
        }
    }
    else
    {
        t->nativeOwner->setFocusPolicy(Qt::NoFocus);
        t->nativeOwner->setFocusProxy(NULL);
    }
}
void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusRequest(ZF_IN void *token, ZF_IN zfbool v)
{
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *t = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
    if(v)
    {
        t->nativeOwner->setFocus();
    }
    else
    {
        t->nativeOwner->clearFocus();
    }
}
zfbool _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocused(ZF_IN void *token)
{
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *t = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
    return t->nativeOwner->hasFocus();
}

#include "ZFProtocolZFUIView_sys_Qt_FocusProxy.moc"

#endif // #if ZF_ENV_sys_Qt

