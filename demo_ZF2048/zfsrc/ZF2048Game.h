/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZF2048Game_h_
#define _ZFI_ZF2048Game_h_

#include "ZF2048Core/ZF2048Types.h"
#include "ZFUIKit.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZF2048GamePrivate;
zfclass ZF2048Game : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZF2048Game, ZFUIView)

    ZFOBSERVER_EVENT(GameDataOnChange)
    ZFOBSERVER_EVENT(GameOnOver)

public:
    virtual void gameReset(ZF_IN_OPT zfindex dataWidth = 4,
                           ZF_IN_OPT zfindex dataHeight = 4);
    virtual zfbool gameCanMoveLeft(void);
    virtual zfbool gameCanMoveTop(void);
    virtual zfbool gameCanMoveRight(void);
    virtual zfbool gameCanMoveBottom(void);
    virtual zfbool gameCanMove(void);
    virtual zfbool gameMoveLeft(void);
    virtual zfbool gameMoveTop(void);
    virtual zfbool gameMoveRight(void);
    virtual zfbool gameMoveBottom(void);
    virtual void gameUndo(void);

public:
    virtual void gameFocus(void);

public:
    virtual zfindex gameDataWidth(void);
    virtual zfindex gameDataHeight(void);

public:
    virtual ZF2048Value gameScore(void);
    virtual ZF2048Value gameMaxNumber(void);

protected:
    virtual void gameDataOnChange(void)
    {
        this->observerNotify(ZF2048Game::EventGameDataOnChange());
    }
    virtual void gameOnOver(void)
    {
        this->observerNotify(ZF2048Game::EventGameOnOver());
    }

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZF2048GamePrivate *d;
    friend zfclassFwd _ZFP_ZF2048GamePrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048Game_h_

