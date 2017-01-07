/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZF2048Core_h_
#define _ZFI_ZF2048Core_h_

#include "ZF2048Types.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassPOD ZF2048Action
{
public:
    /**
     * (from == to) means this is a newly created block
     */
    ZF2048Point from;
    ZF2048Point to;
    /**
     * original value for merged block
     */
    ZF2048Value value;
    zfbool isNewBlock;
};
inline ZF2048Action ZF2048ActionMake(ZF_IN zfindex fromX, ZF_IN zfindex fromY,
                                     ZF_IN zfindex toX, ZF_IN zfindex toY,
                                     ZF_IN ZF2048Value value,
                                     ZF_IN_OPT zfbool isNewBlock = zffalse)
{
    ZF2048Action ret = {{fromX, fromY}, {toX, toY}, value, isNewBlock};
    return ret;
}

zfclassFwd _ZFP_ZF2048CorePrivate;
zfclass ZF2048Core : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZF2048Core, ZFObject)

    ZFOBSERVER_EVENT(GameDataOnChange)

public:
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, historyCountMax, ZFPropertyInitValue(10))

public:
    virtual zfindex dataWidth(void);
    virtual zfindex dataHeight(void);
    virtual const ZF2048Value *data(void);
    /**
     * last history if move, or the undid if undo
     */
    virtual const ZF2048Value *dataPrev(void);
    virtual ZFCoreArrayPOD<ZF2048Action> action(ZF_IN_OPT zfindex historyIndex = 0);
    virtual zfindex historyCount(void);

public:
    virtual ZF2048Value score(void);
    virtual ZF2048Value scoreMaxNumber(void);

public:
    virtual void reset(ZF_IN_OPT zfindex width = 4,
                       ZF_IN_OPT zfindex height = 4);

public:
    virtual zfbool canMoveLeft(void);
    virtual zfbool canMoveTop(void);
    virtual zfbool canMoveRight(void);
    virtual zfbool canMoveBottom(void);
    virtual zfbool canMove(void);

public:
    virtual zfbool moveLeft(void);
    virtual zfbool moveTop(void);
    virtual zfbool moveRight(void);
    virtual zfbool moveBottom(void);

public:
    virtual void undo(void);

public:
    virtual void debugStatus(ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault);

protected:
    virtual void gameDataOnChange(void);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZF2048CorePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048Core_h_

