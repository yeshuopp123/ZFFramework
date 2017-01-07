/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIDialog.h"
#include "ZFUIButton.h"
#include "ZFUIViewLayout.h"

#include "ZFUIOnScreenKeyboardAutoResize.h"
#include "ZFUIOnScreenKeyboardAutoFit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFObjectCreator ZFUIDialogDefaultLayoutParamCreator = zfnull;
zfautoObject ZFUIDialogDefaultLayoutParamCreatorDefault(void)
{
    zfblockedAlloc(ZFUIViewLayoutParam, lp);
    lp->layoutAlignSet(ZFUIAlign::e_Center);
    lp->layoutMarginSet(ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemMargin()));
    return zfautoObjectCreate(lp);
}
ZFObjectCreator ZFUIDialogDefaultAniShowCreator = zfnull;
zfautoObject ZFUIDialogDefaultAniShowCreatorDefault(void)
{
    zfblockedAlloc(ZFAnimationNativeView, ani);
    ani->aniAlphaFromSet(0);
    return zfautoObjectCreate(ani);
}
ZFObjectCreator ZFUIDialogDefaultAniHideCreator = zfnull;
zfautoObject ZFUIDialogDefaultAniHideCreatorDefault(void)
{
    zfblockedAlloc(ZFAnimationNativeView, ani);
    ani->aniAlphaToSet(0);
    return zfautoObjectCreate(ani);
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogDefaultCreatorInit, ZFLevelZFFrameworkLow)
{
    ZFUIDialogDefaultLayoutParamCreator = ZFUIDialogDefaultLayoutParamCreatorDefault;
    ZFUIDialogDefaultAniShowCreator = ZFUIDialogDefaultAniShowCreatorDefault;
    ZFUIDialogDefaultAniHideCreator = ZFUIDialogDefaultAniHideCreatorDefault;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogDefaultCreatorInit)

// ============================================================
// ZFUIDialog
ZFSTYLE_DEFAULT_DEFINE(ZFUIDialog)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogAllDialog, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIDialogAllDialog)
{
    ZFCoreArrayPOD<ZFUIDialog *> tmp;
    tmp.copyFrom(this->allDialog);
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->dialogShowing())
        {
            tmp[i]->dialogHideForce();
        }
    }
}
public:
    ZFCoreArrayPOD<ZFUIDialog *> allDialog;
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogAllDialog)
#define _ZFP_ZFUIDialogAllDialog ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogAllDialog)->allDialog

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogAutoHide, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIDialogAutoHide)
{
    ZFCoreArrayPOD<ZFUIDialog *> tmp;
    tmp.copyFrom(_ZFP_ZFUIDialogAllDialog);
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->dialogShowing())
        {
            tmp[i]->dialogHideForce();
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogAutoHide)

// ============================================================
// _ZFP_ZFUIDialogPrivate
/*
 * memo:
 *
 * _ZFP_ZFUIDialogPrivate (ZFUIWindow, handle all key event)
 * ^ dialogWindowBg (internal bg, window ani target)
 *   _ZFP_ZFUIDialogContentHolder (Ui disable, supply layout logic)
 *   ^ dialogClickMask (focusable, exclude from ZFUIViewFocusNextFind)
 *     dialogBg
 *     ^ dialogContainer
 *       ^ dialogView
 */
zfclass _ZFP_ZFUIDialogPrivate : zfextends ZFUIWindow
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIDialogPrivate, ZFUIWindow)

public:
    ZFUIDialog *pimplOwner;
    ZFUIView *dialogWindowBg;
    ZFAnimationNativeView *dialogWindowAniShow;
    ZFAnimationNativeView *dialogWindowAniHide;
    ZFUIButton *dialogClickMask;
    ZFUIImageView *dialogBg;
    ZFUIView *dialogContainer;
    ZFListener aniShowOnStopListener;
    ZFListener aniHideOnStopListener;

public:
    void aniStop(void)
    {
        if(this->dialogWindowAniShow != zfnull)
        {
            this->dialogWindowAniShow->aniStop();
        }
        if(this->dialogWindowAniHide != zfnull)
        {
            this->dialogWindowAniHide->aniStop();
        }
        if(this->pimplOwner->dialogAniShow() != zfnull)
        {
            this->pimplOwner->dialogAniShow()->aniStop();
        }
        if(this->pimplOwner->dialogAniHide() != zfnull)
        {
            this->pimplOwner->dialogAniHide()->aniStop();
        }
    }

public:
    ZFLISTENER_DECLARE(dialogClickMaskOnClick)
    {
        if(this->pimplOwner->dialogHideWhenTouchOutside())
        {
            this->pimplOwner->dialogHide();
        }
    }
    ZFLISTENER_DECLARE(aniShowOnStop)
    {
        if(!this->dialogWindowAniShow->aniRunning()
            && !this->dialogWindowAniHide->aniRunning()
            && (this->pimplOwner->dialogAniShow() == zfnull || !this->pimplOwner->dialogAniShow()->aniRunning())
            && (this->pimplOwner->dialogAniHide() == zfnull || !this->pimplOwner->dialogAniHide()->aniRunning())
            )
        {
            this->viewUIEnableTreeSet(zftrue);
            ZFUIViewBlinkWhenFocusAutoApplyPause();
            if(this->pimplOwner->dialogFocusAutomatically())
            {
                ZFUIView *next = ZFUIViewFocusNextMove(this->dialogContainer,
                    ZFUIViewFocusNextParam().focusDirectionSet(ZFUIOrientation::e_Left | ZFUIOrientation::e_Top));
                if(next == zfnull)
                {
                    this->dialogClickMask->viewFocusRequest(zftrue);
                }
            }
            else
            {
                this->dialogClickMask->viewFocusRequest(zftrue);
            }
            ZFUIViewBlinkWhenFocusAutoApplyResume();
            this->pimplOwner->dialogAfterShow();
        }
    }
    ZFLISTENER_DECLARE(aniHideOnStop)
    {
        if(!this->dialogWindowAniShow->aniRunning()
            && !this->dialogWindowAniHide->aniRunning()
            && (this->pimplOwner->dialogAniShow() == zfnull || !this->pimplOwner->dialogAniShow()->aniRunning())
            && (this->pimplOwner->dialogAniHide() == zfnull || !this->pimplOwner->dialogAniHide()->aniRunning())
            )
        {
            this->viewUIEnableTreeSet(zftrue);
            this->windowHide();
            this->pimplOwner->dialogAfterHide();
            zfRelease(this->pimplOwner);
        }
    }

public:
    void layoutDialogPrepare(ZF_IN const ZFUIRect &bounds)
    {
        if(this->pimplOwner->dialogBackgroundImage() != zfnull)
        {
            this->dialogContainer->layoutParam()->layoutMarginSet(this->pimplOwner->dialogBackgroundImage()->imageNinePatch());
        }
    }
    void layoutDialog(ZF_IN const ZFUIRect &bounds)
    {
        ZFUIMargin dialogMargin = this->pimplOwner->dialogLayoutParam()->layoutMargin();
        const ZFUIMargin &contentMargin = this->dialogContainer->layoutParam()->layoutMargin();

        ZFUIMargin totalMargin = ZFUIMarginInc(dialogMargin, contentMargin);

        this->dialogContainer->layoutMeasure(ZFUISizeApplyMargin(bounds.size, totalMargin), this->pimplOwner->dialogLayoutParam()->sizeParam());
        ZFUISize dialogContainerSize = this->dialogContainer->layoutMeasuredSize();

        this->dialogClickMask->layout(bounds);

        ZFUISize dialogBgSize = dialogContainerSize;
        dialogBgSize.width += ZFUIMarginGetX(contentMargin);
        dialogBgSize.height += ZFUIMarginGetY(contentMargin);
        this->dialogBg->layout(ZFUIAlignApply(
            this->pimplOwner->dialogLayoutParam()->layoutAlign(),
            bounds,
            dialogBgSize,
            dialogMargin));
    }

    // event handle
protected:
    zfoverride
    virtual void viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
    {
        // dialog would always resolve key event
        keyEvent->eventResolvedSet(zftrue);

        if(!this->pimplOwner->dialogHideWhenClickBack()
            || keyEvent->keyAction != ZFUIKeyAction::e_KeyUp)
        {
            return ;
        }

        switch(keyEvent->keyCode)
        {
            case ZFUIKeyCode::e_kPhoneBack:
            case ZFUIKeyCode::e_kEsc:
                this->pimplOwner->dialogHide();
                break;
            default:
                break;
        }
    }
};
ZFOBJECT_REGISTER(_ZFP_ZFUIDialogPrivate)

zfclass _ZFP_ZFUIDialogContentHolder : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIDialogContentHolder, ZFUIView)

public:
    _ZFP_ZFUIDialogPrivate *pimplOwner;

protected:
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
    {
        this->pimplOwner->layoutDialogPrepare(bounds);
    }
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds)
    {
        this->pimplOwner->layoutDialog(bounds);
    }
};

zfclass _ZFP_I_ZFUIDialog_DialogClickMask : zfextends ZFUIButton
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIDialog_DialogClickMask, ZFUIButton)
};

// ============================================================
// ZFUIDialog
ZFOBJECT_REGISTER(ZFUIDialog)

ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogBeforeShow)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogAfterShow)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogBeforeHide)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogAfterHide)

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIDialog, ZFUIColor, dialogWindowColor)
{
    this->dialogWindowColorSetInternal(newValue);
    d->dialogWindowBg->viewBackgroundColorSet(this->dialogWindowColor());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIDialog, ZFUIImage *, dialogBackgroundImage)
{
    this->dialogBackgroundImageSetInternal(newValue);
    d->dialogBg->imageContentSet(this->dialogBackgroundImage());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIDialog, ZFUIView *, dialogView)
{
    if(this->dialogView() == newValue)
    {
        return ;
    }
    if(this->dialogView() != zfnull)
    {
        d->dialogContainer->childRemove(this->dialogView());
    }
    this->dialogViewSetInternal(newValue);
    if(this->dialogView() != zfnull)
    {
        d->dialogContainer->childAdd(this->dialogView());
        this->dialogView()->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
    }
}

zfbool ZFUIDialog::dialogShowing(void)
{
    return d->windowShowing();
}
void ZFUIDialog::dialogShow(void)
{
    if(d->windowShowing())
    {
        return ;
    }
    zfRetain(this);
    d->aniStop();
    d->windowShow();

    if(this->dialogWindowAutoResize())
    {
        ZFUIOnScreenKeyboardAutoResizeStart(this->dialogWindow());
    }
    if(this->dialogWindowAutoFit())
    {
        ZFUIOnScreenKeyboardAutoFitStart(this->dialogWindow());
    }

    d->viewUIEnableTreeSet(zffalse);
    this->dialogBeforeShow();
    if(this->dialogWindowColor() != ZFUIColorTransparent && this->dialogWindowAutoDim())
    {
        d->dialogWindowAniShow->aniTargetSet(d->dialogWindowBg);
        d->dialogWindowAniShow->observerAdd(ZFObject::ObserverAddParam()
            .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
            .observerSet(d->aniShowOnStopListener)
            .autoRemoveAfterActivateSet(zftrue)
        );
        d->dialogWindowAniShow->aniStart();
    }
    if(this->dialogAniShow() != zfnull)
    {
        this->dialogAniShow()->aniTargetSet(d->dialogBg);
        this->dialogAniShow()->observerAdd(ZFObject::ObserverAddParam()
            .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
            .observerSet(d->aniShowOnStopListener)
            .autoRemoveAfterActivateSet(zftrue)
        );
        this->dialogAniShow()->aniStart();
    }
}
void ZFUIDialog::dialogHide(void)
{
    if(!d->windowShowing())
    {
        return ;
    }
    d->aniStop();
    if(!d->windowShowing())
    {
        return ;
    }

    if(this->dialogWindowAutoResize())
    {
        ZFUIOnScreenKeyboardAutoResizeStop(this->dialogWindow());
    }
    if(this->dialogWindowAutoFit())
    {
        ZFUIOnScreenKeyboardAutoFitStop(this->dialogWindow());
    }

    d->viewUIEnableTreeSet(zffalse);
    this->dialogBeforeHide();
    if(this->dialogWindowColor() != ZFUIColorTransparent && this->dialogWindowAutoDim())
    {
        d->dialogWindowAniHide->aniTargetSet(d->dialogWindowBg);
        d->dialogWindowAniHide->observerAdd(ZFObject::ObserverAddParam()
            .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
            .observerSet(d->aniHideOnStopListener)
            .autoRemoveAfterActivateSet(zftrue)
        );
        d->dialogWindowAniHide->aniStart();
    }
    if(this->dialogAniHide() != zfnull)
    {
        this->dialogAniHide()->aniTargetSet(d->dialogBg);
        this->dialogAniHide()->observerAdd(ZFObject::ObserverAddParam()
            .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
            .observerSet(d->aniHideOnStopListener)
            .autoRemoveAfterActivateSet(zftrue)
        );
        this->dialogAniHide()->aniStart();
    }
}
void ZFUIDialog::dialogHideForce(void)
{
    if(d->windowShowing())
    {
        zfRetainWithoutLeakTest(this);
        d->aniStop();
        if(d->windowShowing())
        {
            this->dialogBeforeHide();
            d->windowHide();
            this->dialogAfterHide();
        }
        zfReleaseWithoutLeakTest(this);
    }
}

void ZFUIDialog::dialogApplyAutoHide(ZF_IN ZFUIButton *button)
{
    if(button == zfnull)
    {
        return ;
    }
    ZFLISTENER_LOCAL(buttonOnClick, {
        userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIDialog *>()->dialogHide();
    })
    button->observerAdd(ZFObject::ObserverAddParam()
            .eventIdSet(ZFUIButton::EventButtonOnClick())
            .observerSet(buttonOnClick)
            .userDataSet(this->objectHolder())
            .observerLevelSet(ZFLevelZFFrameworkPostNormal)
        );
}

ZFUIWindow *ZFUIDialog::dialogWindow(void)
{
    return d;
}

ZFUIView *ZFUIDialog::dialogInternalContainer(void)
{
    return d->dialogContainer;
}
ZFUIView *ZFUIDialog::dialogInternalBackgroundContainer(void)
{
    return d->dialogBg;
}

ZFObject *ZFUIDialog::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAllocWithoutLeakTest(_ZFP_ZFUIDialogPrivate);
    d->pimplOwner = this;
    d->windowLevelSet(ZFUIWindowLevel::e_AppHigh);

    d->dialogWindowBg = zfAllocWithoutLeakTest(ZFUIView);
    d->internalBackgroundViewAdd(d->dialogWindowBg);
    d->dialogWindowBg->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);

    zfblockedAlloc(_ZFP_ZFUIDialogContentHolder, dialogContentHolder);
    d->childAdd(dialogContentHolder);
    dialogContentHolder->pimplOwner = d;
    dialogContentHolder->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
    dialogContentHolder->viewUIEnableSet(zffalse);

    d->dialogClickMask = zfAllocWithoutLeakTest(_ZFP_I_ZFUIDialog_DialogClickMask);
    dialogContentHolder->childAdd(d->dialogClickMask);
    d->dialogClickMask->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
    d->dialogClickMask->observerAdd(ZFUIButton::EventButtonOnClick(),
        ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFUIDialogPrivate, dialogClickMaskOnClick)));

    d->dialogBg = zfAllocWithoutLeakTest(ZFUIImageView);
    dialogContentHolder->childAdd(d->dialogBg);
    d->dialogBg->viewUIEnableTreeSet(zftrue);
    d->dialogBg->viewUIEnableSet(zftrue);

    d->dialogContainer = zfAllocWithoutLeakTest(ZFUIViewLayout);
    d->dialogBg->childAdd(d->dialogContainer);

    d->dialogWindowAniShow = zfAllocWithoutLeakTest(ZFAnimationNativeView);
    d->dialogWindowAniShow->aniAlphaFromSet(0);
    d->dialogWindowAniHide = zfAllocWithoutLeakTest(ZFAnimationNativeView);
    d->dialogWindowAniHide->aniAlphaToSet(0);

    d->aniShowOnStopListener = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFUIDialogPrivate, aniShowOnStop));
    d->aniHideOnStopListener = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFUIDialogPrivate, aniHideOnStop));

    d->dialogWindowBg->viewBackgroundColorSet(this->dialogWindowColor());
    d->dialogBg->imageContentSet(this->dialogBackgroundImage());

    _ZFP_ZFUIDialogAllDialog.add(this);
    return this;
}
void ZFUIDialog::objectOnDealloc(void)
{
    _ZFP_ZFUIDialogAllDialog.removeElement(this);

    ZFPropertyChangeWithoutLeakTest(d->dialogWindowAniShow, zfnull);
    ZFPropertyChangeWithoutLeakTest(d->dialogWindowAniHide, zfnull);

    zfReleaseWithoutLeakTest(d->dialogContainer);
    zfReleaseWithoutLeakTest(d->dialogBg);
    zfReleaseWithoutLeakTest(d->dialogClickMask);
    zfReleaseWithoutLeakTest(d->dialogWindowBg);
    zfReleaseWithoutLeakTest(d);
    d = zfnull;

    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

