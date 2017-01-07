/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZF2048AppAutoMoveRunner.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZF2048AppAutoMoveRunnerPrivate
{
public:
    zfbool runnerStarted;
    zfbool runnerUnableToMove;
    zfbool loopRunFlag;
    zfindex loopIndex;
    ZFTimer *loopTimer;

public:
    _ZFP_ZF2048AppAutoMoveRunnerPrivate(void)
    : runnerStarted(zffalse)
    , runnerUnableToMove(zffalse)
    , loopRunFlag(zffalse)
    , loopIndex(0)
    , loopTimer(zfnull)
    {
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(timerEvent)
    {
        ZF2048AppAutoMoveRunner *owner = userData->to<ZFObjectHolder *>()->holdedObj;
        if(owner->d->loopIndex >= owner->actionList.count())
        {
            owner->d->loopIndex = 0;
            zfbool unableToMove = (!owner->d->loopRunFlag || owner->d->loopIndex >= owner->actionList.count());
            if(unableToMove && owner->actionList.find(ZF2048AppAutoMoveAction::e_Random) != zfindexMax && owner->game->gameCanMove())
            {
                unableToMove = zffalse;
            }
            if(unableToMove)
            {
                owner->d->runnerUnableToMove = zftrue;
                owner->runnerStop();
                return ;
            }
            owner->d->loopRunFlag = zffalse;
        }
        ZF2048AppAutoMoveActionEnum action = owner->actionList[owner->d->loopIndex];
        if(action == ZF2048AppAutoMoveAction::e_Random)
        {
            switch(zfmRand(4))
            {
                case 0:
                    action = ZF2048AppAutoMoveAction::e_Left;
                    break;
                case 1:
                    action = ZF2048AppAutoMoveAction::e_Top;
                    break;
                case 2:
                    action = ZF2048AppAutoMoveAction::e_Right;
                    break;
                case 3:
                    action = ZF2048AppAutoMoveAction::e_Bottom;
                    break;
            }
        }
        zfbool runFlag = zffalse;
        switch(action)
        {
            case ZF2048AppAutoMoveAction::e_Left:
                runFlag = owner->game->gameMoveLeft();
                break;
            case ZF2048AppAutoMoveAction::e_Top:
                runFlag = owner->game->gameMoveTop();
                break;
            case ZF2048AppAutoMoveAction::e_Right:
                runFlag = owner->game->gameMoveRight();
                break;
            case ZF2048AppAutoMoveAction::e_Bottom:
                runFlag = owner->game->gameMoveBottom();
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
        if(!owner->d->loopRunFlag)
        {
            owner->d->loopRunFlag = runFlag;
        }
        ++(owner->d->loopIndex);
    }
};

ZFOBJECT_REGISTER(ZF2048AppAutoMoveRunner)

ZFOBSERVER_EVENT_REGISTER(ZF2048AppAutoMoveRunner, RunnerOnStop)

void ZF2048AppAutoMoveRunner::runnerStart(void)
{
    if(d->runnerStarted)
    {
        return ;
    }
    d->runnerStarted = zftrue;
    zfCoreAssert(this->game != zfnull);

    d->runnerUnableToMove = zffalse;
    d->loopRunFlag = zffalse;
    d->loopIndex = 0;
    d->loopTimer->timerStart();
}
void ZF2048AppAutoMoveRunner::runnerStop(void)
{
    if(!d->runnerStarted)
    {
        return ;
    }
    d->runnerStarted = zffalse;
    d->loopTimer->timerStop();

    this->runnerOnStop();
}
zfbool ZF2048AppAutoMoveRunner::runningStarted(void)
{
    return d->runnerStarted;
}
zfbool ZF2048AppAutoMoveRunner::runnerUnableToMove(void)
{
    return d->runnerUnableToMove;
}

ZFObject *ZF2048AppAutoMoveRunner::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfnew(_ZFP_ZF2048AppAutoMoveRunnerPrivate);
    d->loopTimer = zfAlloc(ZFTimer);
    d->loopTimer->timerIntervalSet((zftimet)(ZFUIGlobalStyle::DefaultStyle()->aniDurationNormal() * 1.5f));
    d->loopTimer->timerActivateInMainThreadSet(zftrue);
    d->loopTimer->observerAdd(ZFTimer::EventTimerOnActivate(), ZFCallbackForRawFunction(_ZFP_ZF2048AppAutoMoveRunnerPrivate::timerEvent), this->objectHolder());
    return this;
}
void ZF2048AppAutoMoveRunner::objectOnDealloc(void)
{
    d->loopTimer->timerStop();
    ZFPropertyChange(d->loopTimer, zfnull);
    zfdelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

