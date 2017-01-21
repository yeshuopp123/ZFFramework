/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZF2048Game.h"
#include "ZF2048Core/ZF2048Core.h"
#include "ZF2048UI/ZF2048UIFrame.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZF2048GamePrivateAniDuration ZFUIGlobalStyle::DefaultStyle()->aniDurationShort()
zfclassNotPOD _ZFP_ZF2048GamePrivate
{
public:
    ZF2048Game *owner;
    ZF2048Core *gameCore;
    ZF2048UIFrame *gameUI;
    ZFListener gameOnMoveListener;
    ZFListener gameDataOnChangeListener;
    ZFAnimationGroup *animating;
    zfbool undoFlag;

public:
    _ZFP_ZF2048GamePrivate(ZF_IN ZF2048Game *owner)
    : owner(owner)
    , gameCore(zfnull)
    , gameUI(zfnull)
    , gameOnMoveListener(ZFCallbackForRawFunction(_ZFP_ZF2048GamePrivate::gameOnMove))
    , gameDataOnChangeListener(ZFCallbackForRawFunction(_ZFP_ZF2048GamePrivate::gameDataOnChange))
    , animating(zfnull)
    , undoFlag(zffalse)
    {
    }

public:
    void aniStop(void)
    {
        if(this->animating != zfnull)
        {
            this->animating->aniStop();
        }
        ZFPropertyChange(this->animating, zfnull);
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(gameOnMove)
    {
        ZF2048Game *owner = userData->to<ZFObjectHolder *>()->holdedObj;
        owner->d->aniStop();

        ZFUIOrientation *direction = listenerData.param0->toAny();
        switch(direction->enumValue<ZFUIOrientationEnum>())
        {
            case ZFUIOrientation::e_Left:
                owner->d->gameCore->moveLeft();
                break;
            case ZFUIOrientation::e_Top:
                owner->d->gameCore->moveTop();
                break;
            case ZFUIOrientation::e_Right:
                owner->d->gameCore->moveRight();
                break;
            case ZFUIOrientation::e_Bottom:
                owner->d->gameCore->moveBottom();
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
    void gameDataOnChange_update(void)
    {
        if(!this->undoFlag)
        {
            this->gameUIUpdate();
        }
        this->undoFlag = zffalse;
        this->owner->gameDataOnChange();
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(gameDataOnChange)
    {
        ZF2048Game *owner = userData->to<ZFObjectHolder *>()->holdedObj;
        owner->d->gameDataOnChange_update();
    }
public:
    void gameUIUpdateForUndo(void)
    {
        this->aniStop();
        this->gameCore->undo();
        this->gameUI->update(this->gameCore->data(), this->gameCore->dataWidth(), this->gameCore->dataHeight());
    }
    void gameUIUpdate(void)
    {
        this->aniStop();

        if(this->owner->objectInstanceState() != ZFObject::ObjectInstanceStateIdle)
        { // no animation for first time
            this->gameUI->update(this->gameCore->dataPrev(), this->gameCore->dataWidth(), this->gameCore->dataHeight());
            return ;
        }

        if(this->gameCore->historyCount() <= 0)
        {
            this->gameUIUpdateForReset();
        }
        else
        {
            this->gameUIUpdateForMove();
        }
    }
    void gameUIUpdateForReset(void)
    {
        ZFPropertyChange(this->animating, zflineAlloc(ZFAnimationGroup));
        this->animating->aniParallelSet(zftrue);

        this->gameUI->update(this->gameCore->data(), this->gameCore->dataWidth(), this->gameCore->dataHeight());
        for(zfindex y = 0; y < this->gameCore->dataHeight(); ++y)
        {
            for(zfindex x = 0; x < this->gameCore->dataWidth(); ++x)
            {
                ZF2048UIBlock *block = this->gameUI->blockAt(x, y);
                if(block != zfnull)
                {
                    zfautoObject aniHolder = this->aniForBlockAppear();
                    ZFAnimation *ani = aniHolder.toAny();
                    this->animating->childAniAdd(ani, zfHint("childAutoCopyTarget")zffalse);
                    ani->aniTargetSet(block);
                }
            }
        }
        this->animating->aniStart();
    }
    void gameUIUpdateForMove(void)
    {
        ZFPropertyChange(this->animating, zflineAlloc(ZFAnimationGroup));
        this->animating->aniParallelSet(zftrue);

        this->gameUI->update(this->gameCore->dataPrev(), this->gameCore->dataWidth(), this->gameCore->dataHeight());
        ZFCoreArrayPOD<ZF2048Action> actions = this->gameCore->action();
        for(zfindex i = 0; i < actions.count(); ++i)
        {
            const ZF2048Action &action = actions[i];
            if(action.from == action.to)
            {
                continue;
            }
            zfautoObject aniHolder = this->aniForBlockMoveTo(action.from, action.to);
            ZFAnimation *ani = aniHolder.toAny();
            this->animating->childAniAdd(ani, zfHint("childAutoCopyTarget")zffalse);
            ani->aniTargetSet(this->gameUI->blockAt(action.from.x, action.from.y));
        }
        ZFLISTENER_LOCAL(moveAniOnStop, {
            ZF2048Game *game = userData->to<ZFObjectHolder *>()->holdedObj;
            game->d->aniStop();
            game->d->gameUI->update(game->d->gameCore->data(), game->d->gameCore->dataWidth(), game->d->gameCore->dataHeight());
            if(listenerData.sender->to<ZFAnimation *>()->aniStoppedByUser())
            {
                return ;
            }

            ZFPropertyChange(game->d->animating, zflineAlloc(ZFAnimationGroup));
            game->d->animating->aniParallelSet(zftrue);

            ZFCoreArrayPOD<ZF2048Point> mergedPosCheck;
            ZFCoreArrayPOD<ZF2048Action> actions = game->d->gameCore->action();
            for(zfindex i = 0; i < actions.count(); ++i)
            {
                const ZF2048Action &action = actions[i];
                if(action.isNewBlock)
                {
                    zfautoObject aniHolder = game->d->aniForBlockAppear();
                    ZFAnimation *ani = aniHolder.toAny();
                    game->d->animating->childAniAdd(ani, zfHint("childAutoCopyTarget")zffalse);
                    ani->aniTargetSet(game->d->gameUI->blockAt(action.from.x, action.from.y));
                    continue;
                }

                zfindex movedTo = mergedPosCheck.find(action.to);
                if(movedTo != zfindexMax)
                {
                    mergedPosCheck.remove(movedTo);
                    zfautoObject aniHolder = game->d->aniForBlockMerge();
                    ZFAnimation *ani = aniHolder.toAny();
                    game->d->animating->childAniAdd(ani, zfHint("childAutoCopyTarget")zffalse);
                    ani->aniTargetSet(game->d->gameUI->blockAt(action.to.x, action.to.y));
                }
                else
                {
                    mergedPosCheck.add(action.to);
                }
            }
            game->d->animating->aniStart();
        })
        this->animating->observerAdd(ZFObject::ObserverAddParam()
                                     .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
                                     .observerSet(moveAniOnStop)
                                     .userDataSet(this->owner->objectHolder())
                                     .autoRemoveAfterActivateSet(zftrue));
        this->animating->aniStart();
    }
    zfautoObject aniForBlockAppear(void)
    {
        zfblockedAlloc(ZFAnimationNativeView, ani);
        ani->aniDurationSet(_ZFP_ZF2048GamePrivateAniDuration);
        ani->aniScaleXFromSet(0);
        ani->aniScaleYFromSet(0);
        ani->aniAlphaFromSet(0);
        return zfautoObjectCreate(ani);
    }
    zfautoObject aniForBlockDisappear(void)
    {
        zfblockedAlloc(ZFAnimationNativeView, ani);
        ani->aniDurationSet(_ZFP_ZF2048GamePrivateAniDuration);
        ani->aniScaleXToSet(0);
        ani->aniScaleYToSet(0);
        ani->aniAlphaToSet(0);
        return zfautoObjectCreate(ani);
    }
    zfautoObject aniForBlockMerge(void)
    {
        zfblockedAlloc(ZFAnimationGroup, ani);

        zfblockedAlloc(ZFAnimationNativeView, aniFlashUp);
        ani->childAniAdd(aniFlashUp);
        aniFlashUp->aniDurationSet(_ZFP_ZF2048GamePrivateAniDuration);
        aniFlashUp->aniScaleXToSet(1.2f);
        aniFlashUp->aniScaleYToSet(1.2f);
        aniFlashUp->aniDurationSet((zftimet)(aniFlashUp->aniDuration() / 2));

        zfblockedAlloc(ZFAnimationNativeView, aniFlashDown);
        ani->childAniAdd(aniFlashDown);
        aniFlashDown->aniDurationSet(_ZFP_ZF2048GamePrivateAniDuration);
        aniFlashDown->aniScaleXFromSet(1.2f);
        aniFlashDown->aniScaleYFromSet(1.2f);
        aniFlashDown->aniDurationSet((zftimet)(aniFlashDown->aniDuration() / 2));

        return zfautoObjectCreate(ani);
    }
    zfautoObject aniForBlockMoveTo(ZF_IN const ZF2048Point &posFrom, ZF_IN const ZF2048Point &posTo)
    {
        ZFUIRect rectFrom = this->gameUI->blockRectAt(posFrom.x, posFrom.y);
        ZFUIRect rectTo = this->gameUI->blockRectAt(posTo.x, posTo.y);
        zfblockedAlloc(ZFAnimationNativeView, ani);
        ani->aniDurationSet(_ZFP_ZF2048GamePrivateAniDuration);
        ani->aniTranslatePixelXToSet(rectTo.point.x - rectFrom.point.x);
        ani->aniTranslatePixelYToSet(rectTo.point.y - rectFrom.point.y);
        return zfautoObjectCreate(ani);
    }
    zfautoObject aniForBlockMoveFrom(ZF_IN const ZF2048Point &posFrom, ZF_IN const ZF2048Point &posTo)
    {
        ZFUIRect rectFrom = this->gameUI->blockRectAt(posFrom.x, posFrom.y);
        ZFUIRect rectTo = this->gameUI->blockRectAt(posTo.x, posTo.y);
        zfblockedAlloc(ZFAnimationNativeView, ani);
        ani->aniDurationSet(_ZFP_ZF2048GamePrivateAniDuration);
        ani->aniTranslatePixelXFromSet(rectFrom.point.x - rectTo.point.x);
        ani->aniTranslatePixelYFromSet(rectFrom.point.y - rectTo.point.y);
        return zfautoObjectCreate(ani);
    }
};

ZFOBJECT_REGISTER(ZF2048Game)

ZFOBSERVER_EVENT_REGISTER(ZF2048Game, GameDataOnChange)
ZFOBSERVER_EVENT_REGISTER(ZF2048Game, GameOnOver)

void ZF2048Game::gameReset(ZF_IN_OPT zfindex dataWidth /* = 4 */,
                           ZF_IN_OPT zfindex dataHeight /* = 4 */)
{
    d->aniStop();
    d->gameCore->reset(dataWidth, dataHeight);
}
zfbool ZF2048Game::gameCanMoveLeft(void)
{
    return d->gameCore->canMoveLeft();
}
zfbool ZF2048Game::gameCanMoveTop(void)
{
    return d->gameCore->canMoveTop();
}
zfbool ZF2048Game::gameCanMoveRight(void)
{
    return d->gameCore->canMoveRight();
}
zfbool ZF2048Game::gameCanMoveBottom(void)
{
    return d->gameCore->canMoveBottom();
}
zfbool ZF2048Game::gameCanMove(void)
{
    return d->gameCore->canMove();
}
zfbool ZF2048Game::gameMoveLeft(void)
{
    d->aniStop();
    return d->gameCore->moveLeft();
}
zfbool ZF2048Game::gameMoveTop(void)
{
    d->aniStop();
    return d->gameCore->moveTop();
}
zfbool ZF2048Game::gameMoveRight(void)
{
    d->aniStop();
    return d->gameCore->moveRight();
}
zfbool ZF2048Game::gameMoveBottom(void)
{
    d->aniStop();
    return d->gameCore->moveBottom();
}
void ZF2048Game::gameUndo(void)
{
    d->aniStop();
    d->undoFlag = zftrue;
    d->gameUIUpdateForUndo();
}

void ZF2048Game::gameFocus(void)
{
    d->gameUI->viewFocusRequest(zftrue);
}

zfindex ZF2048Game::gameDataWidth(void)
{
    return d->gameCore->dataWidth();
}
zfindex ZF2048Game::gameDataHeight(void)
{
    return d->gameCore->dataHeight();
}

ZF2048Value ZF2048Game::gameScore(void)
{
    return d->gameCore->score();
}
ZF2048Value ZF2048Game::gameMaxNumber(void)
{
    return d->gameCore->scoreMaxNumber();
}

ZFObject *ZF2048Game::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfnew(_ZFP_ZF2048GamePrivate, this);

    d->gameCore = zfAlloc(ZF2048Core);
    d->gameCore->observerAdd(ZF2048Core::EventGameDataOnChange(), d->gameDataOnChangeListener, this->objectHolder());

    d->gameUI = zfAlloc(ZF2048UIFrame);
    this->childAdd(d->gameUI);
    d->gameUI->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
    d->gameUI->observerAdd(ZF2048UIFrame::EventFrameOnMove(), d->gameOnMoveListener, this->objectHolder());

    return this;
}
void ZF2048Game::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    this->gameReset();
}
void ZF2048Game::objectOnDealloc(void)
{
    d->aniStop();

    d->gameCore->observerRemove(ZF2048Core::EventGameDataOnChange(), d->gameDataOnChangeListener);
    d->gameUI->observerRemove(ZF2048UIFrame::EventFrameOnMove(), d->gameOnMoveListener);

    zfdelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

