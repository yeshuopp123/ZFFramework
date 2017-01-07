/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFAnimationNativeView.h"

#if ZF_ENV_sys_Qt

#include <QAbstractAnimation>
#include <QEvent>
#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QCoreApplication>
#include <QVariant>
#include <QLayout>

class _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani;
class _ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniDrawingTarget : public QWidget
{
    Q_OBJECT

public:
    ZFCoreArrayPOD<_ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *> ani;

public:
    _ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniDrawingTarget(void)
    : QWidget()
    , ani()
    {
        this->setEnabled(false);
        this->setFocusPolicy(Qt::NoFocus);
        this->setAttribute(Qt::WA_TransparentForMouseEvents);
        this->setAutoFillBackground(false);
    }

protected:
    virtual void paintEvent(QPaintEvent *);
};

class _ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniData
{
public:
    _ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniDrawingTarget *aniDrawingTarget;
};
Q_DECLARE_METATYPE(_ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniData *)

typedef zffloat (*_ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani_curveProgressFunc)(ZF_IN zffloat progress);
class _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani : public QAbstractAnimation
{
    Q_OBJECT

public:
    ZFAnimationNativeView *ownerZFAnimation;

    // for ani impl
    zffloat nativeAniScale;
    zffloat aniCurStep;
    _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani_curveProgressFunc aniCurveFunc;
    zfbool aniRunning;

    // for ani drawing logic
    QWidget *aniTargetCached;
    QPixmap aniTargetSnapshot;
    zfbool aniTargetSnapshotFirstTimeFlag; // used to delay render
    QRect aniTargetGeometrySaved;
    zfbool aniTargetGeometryOverrideFlag;
    _ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniDrawingTarget *aniDrawingTarget;
    QWidget *aniDrawingTargetWindow;
    QEvent::Type aniEventSchedulePaint;
    QEvent::Type aniEventCaptureTarget;
    zfbool aniCaptureFlag;

public:
    _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani(ZF_IN ZFAnimationNativeView *ownerZFAnimation)
    : QAbstractAnimation()
    , ownerZFAnimation(ownerZFAnimation)
    , nativeAniScale(1)
    , aniCurStep(0)
    , aniCurveFunc(zfnull)
    , aniRunning(zffalse)
    , aniTargetCached(zfnull)
    , aniTargetSnapshot()
    , aniTargetSnapshotFirstTimeFlag(zffalse)
    , aniTargetGeometrySaved()
    , aniTargetGeometryOverrideFlag(zffalse)
    , aniDrawingTarget(zfnull)
    , aniDrawingTargetWindow(zfnull)
    , aniEventSchedulePaint((QEvent::Type)(QEvent::User + 4000))
    , aniEventCaptureTarget((QEvent::Type)(QEvent::User + 4001))
    , aniCaptureFlag(zffalse)
    {
        this->connect(this, SIGNAL(finished()), this, SLOT(nativeAniOnStop()));
    }
    ~_ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani(void)
    {
        this->disconnect(this, SIGNAL(finished()), this, SLOT(nativeAniOnStop()));
    }

public:
    virtual int duration(void) const
    {
        return this->ownerZFAnimation->aniDuration();
    }
protected:
    virtual void updateCurrentTime(int currentTime)
    {
        if(!this->aniRunning)
        {
            return ;
        }

        if(currentTime >= this->duration())
        {
            this->aniCurStep = 1;
            this->nativeAniCleanup();
            this->aniRunning = zffalse;
            this->stop();
        }
        else
        {
            this->aniCurStep = this->aniCurveFunc((zffloat)currentTime / this->duration());
            this->aniDrawingTarget->update();
        }
    }
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
    {
        QAbstractAnimation::updateState(newState, oldState);
    }
    virtual void updateDirection(QAbstractAnimation::Direction direction)
    {
        QAbstractAnimation::updateDirection(direction);
    }

public:
    virtual bool eventFilter(QObject *obj, QEvent *event)
    {
        if(obj == this->aniTargetCached->parentWidget())
        {
            if(event->type() == QEvent::ChildRemoved)
            {
                QChildEvent *eventTmp = (QChildEvent *)event;
                if(eventTmp->child() == this->aniTargetCached)
                {
                    // child removed during animation,
                    // stop it manually
                    this->stop();
                    this->nativeAniOnStop();
                }
            }
            return zffalse;
        }

        if(event->type() == QEvent::Resize || event->type() == QEvent::Move)
        {
            if(!this->aniTargetGeometryOverrideFlag)
            {
                this->aniTargetOverrideGeometry();
            }
            return true;
        }
        if(event->type() == aniEventCaptureTarget)
        {
            if(!this->aniCaptureFlag)
            {
                this->aniCaptureFlag = zftrue;
                this->aniCaptureAndStart();
            }
            return false;
        }
        if(event->type() != aniEventSchedulePaint || !this->aniTargetSnapshotFirstTimeFlag)
        {
            return false;
        }
        this->aniTargetSnapshotFirstTimeFlag = zffalse;
        QCoreApplication::instance()->postEvent(obj, new QEvent(aniEventCaptureTarget), Qt::HighEventPriority);
        this->aniDrawingTarget->raise();
        return false;
    }
    void doPaint(void)
    {
        this->aniDoPaint();
    }
public slots:
    void nativeAniOnStop(void)
    {
        if(this->aniTargetCached != zfnull)
        {
            this->nativeAniCleanup();
        }
        if(this->ownerZFAnimation->aniRunning())
        {
            ZFPROTOCOL_ACCESS(ZFAnimationNativeView)->notifyAniStop(this->ownerZFAnimation);
        }
    }

private:
    /*
     * move target to invisible area during animation
     */
    void aniTargetOverrideGeometry(void)
    {
        this->aniTargetGeometrySaved = this->aniTargetCached->geometry();
        QSize size = this->aniTargetCached->geometry().size();
        this->aniTargetGeometryOverrideFlag = zftrue;
        this->aniTargetCached->setGeometry(
            -size.width() + 1,
            -size.height() + 1,
            size.width(),
            size.height());
        this->aniTargetGeometryOverrideFlag = zffalse;
    }

    void aniCaptureAndStart(void)
    {
        this->aniTargetCapture();
        this->aniDrawingTarget->update();
        this->aniTargetCached->setVisible(false);

        this->aniRunning = zftrue;
        this->start();
    }

    QPoint aniTargetCenterOffset(void)
    {
        zfint x = this->aniTargetGeometrySaved.x() + (this->aniTargetGeometrySaved.width() + 1) / 2;
        zfint y = this->aniTargetGeometrySaved.y() + (this->aniTargetGeometrySaved.height() + 1) / 2;
        QWidget *t = this->aniTargetCached->parentWidget();
        QWidget *parent = this->aniDrawingTargetWindow;
        while(t != parent)
        {
            x += t->geometry().x();
            y += t->geometry().y();
            t = t->parentWidget();
        }
        return QPoint(x, y);
    }
    void aniTargetCapture(void)
    {
#if 1
        this->aniTargetSnapshot = this->aniTargetCached->grab();
#else
        this->aniTargetSnapshot = QPixmap(this->aniTargetGeometrySaved.size());
        this->aniTargetSnapshot.fill(QColor(0, 0, 0, 0));
        this->aniTargetCached->render(
            &(this->aniTargetSnapshot),
            QPoint(),
            QRegion(),
            QWidget::DrawChildren);
#endif
    }

    void aniDrawingTargetPrepare(void)
    {
        this->aniDrawingTargetWindow = this->aniTargetCached->window();
        _ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniData *v = zfnull;
        {
            QVariant tag = ZFImpl_sys_Qt_QObjectTagGet(this->aniDrawingTargetWindow, zfText("_ZFP_ZFAnimationNativeViewImpl_sys_Qt_aniDrawingTarget"));
            if(tag.isValid())
            {
                v = tag.value<_ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniData *>();
            }
            else
            {
                v = zfnew(_ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniData);
                v->aniDrawingTarget = new _ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniDrawingTarget();
                this->aniDrawingTargetWindow->layout()->addWidget(v->aniDrawingTarget);
                ZFImpl_sys_Qt_QObjectTagSet(this->aniDrawingTargetWindow, zfText("_ZFP_ZFAnimationNativeViewImpl_sys_Qt_aniDrawingTarget"), QVariant::fromValue(v));
            }
        }
        v->aniDrawingTarget->ani.add(this);
        this->aniDrawingTarget = v->aniDrawingTarget;
        this->aniDrawingTarget->raise();
    }
    void aniDrawingTargetCleanup(void)
    {
        QVariant tag = ZFImpl_sys_Qt_QObjectTagGet(this->aniDrawingTargetWindow, zfText("_ZFP_ZFAnimationNativeViewImpl_sys_Qt_aniDrawingTarget"));
        zfassert(tag.isValid());
        _ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniData *v = tag.value<_ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniData *>();
        v->aniDrawingTarget->ani.removeElement(this, ZFComparerCheckEqualOnly);
        if(v->aniDrawingTarget->ani.isEmpty())
        {
            ZFImpl_sys_Qt_QObjectTagSet(this->aniDrawingTargetWindow, zfText("_ZFP_ZFAnimationNativeViewImpl_sys_Qt_aniDrawingTarget"), QVariant());
            this->aniDrawingTargetWindow->layout()->removeWidget(v->aniDrawingTarget);
            delete v->aniDrawingTarget;
        }

        this->aniDrawingTarget = zfnull;
    }

    void aniDoPaint(void)
    {
        if(this->aniDrawingTarget == zfnull)
        {
            return ;
        }
        QPainter painter(this->aniDrawingTarget);
        painter.translate(this->aniTargetCenterOffset());
        this->aniApplyTransform(painter);
        {
#if 0 // no anitialiasing for performance
            painter.setRenderHint(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
#endif
            painter.drawPixmap(
                QPoint(
                   -((this->aniTargetGeometrySaved.width() + 1) / 2),
                   -((this->aniTargetGeometrySaved.height() + 1) / 2)),
                this->aniTargetSnapshot);
        }
    }
    void aniApplyTransform(ZF_IN QPainter &painter)
    {
        ZFAnimationNativeView *ani = this->ownerZFAnimation;

        if(ani->aniAlphaFrom() != 1 || ani->aniAlphaTo() != 1)
        {
            painter.setOpacity(zfmApplyProgress(ani->aniAlphaFrom(), ani->aniAlphaTo(), this->aniCurStep));
        }

        if(ani->aniScaleXFrom() != 0 || ani->aniScaleXTo() != 0
            || ani->aniScaleYFrom() != 0 || ani->aniScaleYTo() != 0)
        {
            painter.scale(
                zfmApplyProgress(ani->aniScaleXFrom(), ani->aniScaleXTo(), this->aniCurStep),
                zfmApplyProgress(ani->aniScaleYFrom(), ani->aniScaleYTo(), this->aniCurStep)
                );
        }

        if(ani->aniTranslateXFrom() != 0 || ani->aniTranslateXTo() != 0
            || ani->aniTranslateYFrom() != 0 || ani->aniTranslateYTo() != 0)
        {
            painter.translate(
                zfmApplyProgress(ani->aniTranslateXFrom(), ani->aniTranslateXTo(), this->aniCurStep) * this->aniTargetGeometrySaved.size().width() * this->nativeAniScale,
                zfmApplyProgress(ani->aniTranslateYFrom(), ani->aniTranslateYTo(), this->aniCurStep) * this->aniTargetGeometrySaved.size().height() * this->nativeAniScale
                );
        }
        if(ani->aniTranslatePixelXFrom() != 0 || ani->aniTranslatePixelXTo() != 0
            || ani->aniTranslatePixelYFrom() != 0 || ani->aniTranslatePixelYTo() != 0)
        {
            painter.translate(
                zfmApplyProgress(ani->aniTranslatePixelXFrom(), ani->aniTranslatePixelXTo(), this->aniCurStep) * this->nativeAniScale,
                zfmApplyProgress(ani->aniTranslatePixelYFrom(), ani->aniTranslatePixelYTo(), this->aniCurStep) * this->nativeAniScale
                );
        }

        if(ani->aniRotateZFrom() != 0 || ani->aniRotateZTo() != 0)
        {
            painter.rotate(zfmApplyProgress(ani->aniRotateZFrom(), ani->aniRotateZTo(), this->aniCurStep));
        }
    }

public:
    static zffloat _aniCurveFuncLinear(ZF_IN zffloat progress)
    {
        return progress;
    }
    static zffloat _aniCurveFuncEaseIn(ZF_IN zffloat progress)
    {
        return ZFBezierEaseIn.y_by_x(progress);
    }
    static zffloat _aniCurveFuncEaseOut(ZF_IN zffloat progress)
    {
        return ZFBezierEaseOut.y_by_x(progress);
    }
    static zffloat _aniCurveFuncEaseInOut(ZF_IN zffloat progress)
    {
        return ZFBezierEaseInOut.y_by_x(progress);
    }
    void nativeAniStart(void)
    {
        this->aniCurStep = 0;
        switch(this->ownerZFAnimation->aniCurve())
        {
            case ZFAnimationNativeViewCurve::e_Linear:
                this->aniCurveFunc = _aniCurveFuncLinear;
                break;
            case ZFAnimationNativeViewCurve::e_EaseIn:
                this->aniCurveFunc = _aniCurveFuncEaseIn;
                break;
            case ZFAnimationNativeViewCurve::e_EaseOut:
                this->aniCurveFunc = _aniCurveFuncEaseOut;
                break;
            case ZFAnimationNativeViewCurve::e_EaseInOut:
                this->aniCurveFunc = _aniCurveFuncEaseInOut;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }

        this->aniTargetCached = ZFCastStatic(QWidget *, this->ownerZFAnimation->aniTarget()->to<ZFUIView *>()->nativeView());
        zfCoreAssert(this->aniTargetCached != zfnull);
        this->aniTargetSnapshotFirstTimeFlag = zftrue;

        this->aniCaptureFlag = zffalse;
        this->aniDrawingTargetPrepare();

        QRect rect = this->aniTargetCached->geometry();
        this->aniTargetGeometrySaved = rect;
        this->aniTargetCached->setGeometry(QRect(rect.x(), rect.y(), rect.width() + 1, rect.height() + 1));
        this->aniTargetCached->installEventFilter(this);
        this->aniTargetCached->parentWidget()->installEventFilter(this);
        this->aniTargetCached->parentWidget()->updateGeometry();
        this->aniTargetCached->update();
        QCoreApplication::instance()->postEvent(this->aniTargetCached, new QEvent(aniEventSchedulePaint), Qt::LowEventPriority);
    }
    void nativeAniStop(void)
    {
        this->nativeAniCleanup();
        this->aniRunning = zffalse;
        this->stop();
    }
private:
    void nativeAniCleanup(void)
    {
        this->aniTargetCached->removeEventFilter(this);
        this->aniTargetCached->parentWidget()->removeEventFilter(this);
        if(this->aniDrawingTarget != zfnull)
        {
            this->aniDrawingTargetCleanup();
        }

        this->aniTargetSnapshot = QPixmap();
        this->aniTargetCached->setGeometry(this->aniTargetGeometrySaved);
        this->aniTargetCached->setVisible(true);
        this->aniTargetCached = zfnull;

        this->aniCurveFunc = zfnull;
    }
};

void _ZFP_ZFAnimationNativeViewImpl_sys_Qt_AniDrawingTarget::paintEvent(QPaintEvent *event)
{
    // ensure the ani drawing target at top most
    this->raise();

    for(zfindex i = 0; i < this->ani.count(); ++i)
    {
        this->ani[i]->doPaint();
    }
}

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAnimationNativeViewImpl_sys_Qt, ZFAnimationNativeView, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt:QAbstractAnimation"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("Qt:QWidget"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void *nativeAniCreate(ZF_IN ZFAnimationNativeView *ani)
    {
        return new _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani(ani);
    }
    virtual void nativeAniDestroy(ZF_IN ZFAnimationNativeView *ani,
                                  ZF_IN void *nativeAni)
    {
        _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *nativeAniTmp = ZFCastStatic(_ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *, nativeAni);
        nativeAniTmp->deleteLater();
    }

    virtual void nativeAniStart(ZF_IN ZFAnimationNativeView *ani,
                                ZF_IN zffloat nativeAniScale)
    {
        ZFUIView *view = ZFCastZFObject(ZFUIView *, ani->aniTarget());
        if(view != zfnull)
        {
            ZFUIViewUtil::viewRoot(view)->layoutIfNeed();
        }

        _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *nativeAni = ZFCastStatic(_ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *, ani->nativeAnimation());
        nativeAni->nativeAniScale = nativeAniScale;
        nativeAni->nativeAniStart();
    }
    virtual void nativeAniStop(ZF_IN ZFAnimationNativeView *ani)
    {
        _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *nativeAni = ZFCastStatic(_ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *, ani->nativeAnimation());
        nativeAni->nativeAniStop();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAnimationNativeViewImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFAnimationNativeViewImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFAnimationNativeView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

