/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZF2048Core.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZF2048CorePrivate
{
public:
    ZF2048Core *owner;
    zfindex dataWidth;
    zfindex dataHeight;
    ZFCorePointerForPOD<ZF2048Value *> data;
    ZFCorePointerForPOD<ZF2048Value *> dataPrev;
    ZFCorePointerForPOD<ZF2048Value *> dataTmp;
    ZFCoreArrayPOD<ZF2048Value *> dataPointerCache;
    /*
     * older history at larger index
     */
    ZFCoreArray<ZFCoreArrayPOD<ZF2048Action> > actionHistory;

    zfbool gameOverNeedCheck;
    zfbool gameOverFlag;

    ZF2048Value score;
    ZF2048Value scoreMaxNumber;

public:
    _ZFP_ZF2048CorePrivate(ZF_IN ZF2048Core *owner)
    : owner(owner)
    , dataWidth(4)
    , dataHeight(4)
    , data()
    , dataPrev()
    , dataTmp()
    , dataPointerCache()
    , actionHistory()
    , gameOverNeedCheck(zftrue)
    , gameOverFlag(zffalse)
    , score(0)
    , scoreMaxNumber(0)
    {
    }

public:
    zfindex posToIndex(ZF_IN const ZF2048Point &pos)
    {
        return ZF2048PointToIndex(pos, this->dataWidth, this->dataHeight);
    }
    zfindex posToIndex(ZF_IN zfindex posX, ZF_IN zfindex posY)
    {
        return ZF2048PointToIndex(posX, posY, this->dataWidth, this->dataHeight);
    }
    ZF2048Point posFromIndex(ZF_IN zfindex index)
    {
        return ZF2048PointFromIndex(index, this->dataWidth, this->dataHeight);
    }
    void posFromIndex(ZF_OUT ZF2048Point &pos, ZF_IN zfindex index)
    {
        ZF2048PointFromIndex(pos, index, this->dataWidth, this->dataHeight);
    }

public:
    zfindex generateBlockIndex(void)
    {
        ZFCoreArrayPOD<zfindex> available;
        const ZF2048Value *p = this->data.pointerValueGet();
        const ZF2048Value *pEnd = this->data.pointerValueGet() + this->dataWidth * this->dataHeight;
        for( ; p != pEnd; ++p)
        {
            if(*p == 0)
            {
                available.add(p - this->data.pointerValueGet());
            }
        }
        zfCoreAssert(available.count() > 0);
        return available[zfmRand(available.count())];
    }
    ZF2048Point generateBlockPos(void)
    {
        return this->posFromIndex(this->generateBlockIndex());
    }
    ZF2048Value generateBlockValue(void)
    {
        return (ZF2048Value)((zfmRand(8) == 0) ? 4 : 2);
    }
    void addActionList(ZF_IN const ZFCoreArrayPOD<ZF2048Action> &actionList)
    {
        zfindex count = this->owner->historyCountMax();
        if(count > 0)
        {
            --count;
        }
        while(this->actionHistory.count() > count)
        {
            this->actionHistory.removeLast();
        }
        this->actionHistory.add(0, actionList);
    }

    void scoreUpdateQuickly(ZF_IN ZF2048Value newBlockValue)
    {
        this->score += newBlockValue;
        this->scoreMaxNumber = zfmMax(this->scoreMaxNumber, newBlockValue);
    }
    void scoreUpdate(void)
    {
        const ZF2048Value *data = this->data.pointerValueGet();
        const ZF2048Value *dataEnd = data + this->dataWidth * this->dataHeight;
        this->score = 0;
        this->scoreMaxNumber = 0;
        for( ; data != dataEnd; ++data)
        {
            this->score += *data;
            this->scoreMaxNumber = zfmMax(this->scoreMaxNumber, *data);
        }
    }

    void debugStatus(ZF_IN const ZFOutputCallback &outputCallback,
                     ZF_IN const ZF2048Value *data_)
    {
        if(!outputCallback.callbackIsValid())
        {
            return ;
        }

        for(zfindex i = 0; i < this->dataHeight; ++i)
        {
            outputCallback << zfText("------");
        }
        outputCallback << zfText("-\n");

        for(zfindex y = 0; y < this->dataHeight; ++y)
        {
            for(zfindex x = 0; x < this->dataWidth; ++x)
            {
                ZF2048Value value = data_[this->posToIndex(x, y)];
                if(value == 0)
                {
                    outputCallback << zfText("|     ");
                }
                else
                {
                    outputCallback << zfstringWithFormat(zfText("|%5s"), zfsFromInt(value).cString());
                }
            }
            if(y != this->dataHeight - 1)
            {
                outputCallback << zfText("|\n");
            }
        }

        outputCallback << zfText("|\n");
        for(zfindex i = 0; i < this->dataHeight; ++i)
        {
            outputCallback << zfText("------");
        }
        outputCallback << zfText("-\n");
    }
};

ZFOBJECT_REGISTER(ZF2048Core)

ZFOBSERVER_EVENT_REGISTER(ZF2048Core, GameDataOnChange)

zfindex ZF2048Core::dataWidth(void)
{
    return d->dataWidth;
}
zfindex ZF2048Core::dataHeight(void)
{
    return d->dataHeight;
}
const ZF2048Value *ZF2048Core::data(void)
{
    return d->data.pointerValueGet();
}
const ZF2048Value *ZF2048Core::dataPrev(void)
{
    return d->dataPrev.pointerValueGet();
}
ZFCoreArrayPOD<ZF2048Action> ZF2048Core::action(ZF_IN_OPT zfindex historyIndex /* = 0 */)
{
    if(historyIndex >= d->actionHistory.count())
    {
        return ZFCoreArrayPOD<ZF2048Action>();
    }
    return d->actionHistory[historyIndex];
}
zfindex ZF2048Core::historyCount(void)
{
    return d->actionHistory.count();
}

ZF2048Value ZF2048Core::score(void)
{
    return d->score;
}
ZF2048Value ZF2048Core::scoreMaxNumber(void)
{
    return d->scoreMaxNumber;
}

void ZF2048Core::reset(ZF_IN_OPT zfindex width /* = 4 */,
                       ZF_IN_OPT zfindex height /* = 4 */)
{
    d->actionHistory.removeAll();
    d->dataWidth = width;
    d->dataHeight = height;
    d->gameOverNeedCheck = zftrue;

    zfindex size = sizeof(ZF2048Value) * d->dataWidth * d->dataHeight;
    ZF2048Value *data = (ZF2048Value *)zfmalloc(size);
    zfmemset(data, 0, size);
    d->data.pointerValueSet(data);
    d->dataPointerCache.removeAll();
    for(zfindex i = 0; i < width; ++i)
    {
        d->dataPointerCache.add(data + d->posToIndex(i, 0));
    }
    data[d->generateBlockIndex()] = d->generateBlockValue();
    data[d->generateBlockIndex()] = d->generateBlockValue();

    d->scoreUpdate();

    ZF2048Value *dataPrev = (ZF2048Value *)zfmalloc(size);
    zfmemcpy(dataPrev, data, size);
    d->dataPrev.pointerValueSet(dataPrev);

    ZF2048Value *dataTmp = (ZF2048Value *)zfmalloc(size);
    zfmemcpy(dataTmp, data, size);
    d->dataTmp.pointerValueSet(dataTmp);

    this->gameDataOnChange();
}

zfbool ZF2048Core::canMoveLeft(void)
{
    for(zfindex y = 0; y < d->dataHeight; ++y)
    {
        zfindex pos = d->dataWidth - 1;
        for( ; pos != zfindexMax; --pos)
        {
            if(d->dataPointerCache[pos][y] != 0)
            {
                break;
            }
        }
        while(pos != 0 && pos != zfindexMax)
        {
            ZF2048Value t0 = d->dataPointerCache[pos][y];
            --pos;
            ZF2048Value t1 = d->dataPointerCache[pos][y];
            if(t1 == 0 || t0 == t1)
            {
                return zftrue;
            }
        }
    }
    return zffalse;
}
zfbool ZF2048Core::canMoveTop(void)
{
    for(zfindex x = 0; x < d->dataWidth; ++x)
    {
        zfindex pos = d->dataHeight - 1;
        for( ; pos != zfindexMax; --pos)
        {
            if(d->dataPointerCache[x][pos] != 0)
            {
                break;
            }
        }
        while(pos != 0 && pos != zfindexMax)
        {
            ZF2048Value t0 = d->dataPointerCache[x][pos];
            --pos;
            ZF2048Value t1 = d->dataPointerCache[x][pos];
            if(t1 == 0 || t0 == t1)
            {
                return zftrue;
            }
        }
    }
    return zffalse;
}
zfbool ZF2048Core::canMoveRight(void)
{
    for(zfindex y = 0; y < d->dataHeight; ++y)
    {
        zfindex pos = 0;
        for( ; pos < d->dataWidth; ++pos)
        {
            if(d->dataPointerCache[pos][y] != 0)
            {
                break;
            }
        }
        while(pos < d->dataWidth - 1)
        {
            ZF2048Value t0 = d->dataPointerCache[pos][y];
            ++pos;
            ZF2048Value t1 = d->dataPointerCache[pos][y];
            if(t1 == 0 || t0 == t1)
            {
                return zftrue;
            }
        }
    }
    return zffalse;
}
zfbool ZF2048Core::canMoveBottom(void)
{
    for(zfindex x = 0; x < d->dataWidth; ++x)
    {
        zfindex pos = 0;
        for( ; pos < d->dataHeight; ++pos)
        {
            if(d->dataPointerCache[x][pos] != 0)
            {
                break;
            }
        }
        while(pos < d->dataHeight - 1)
        {
            ZF2048Value t0 = d->dataPointerCache[x][pos];
            ++pos;
            ZF2048Value t1 = d->dataPointerCache[x][pos];
            if(t1 == 0 || t0 == t1)
            {
                return zftrue;
            }
        }
    }
    return zffalse;
}
zfbool ZF2048Core::canMove(void)
{
    if(!d->gameOverNeedCheck)
    {
        return !(d->gameOverFlag);
    }
    d->gameOverNeedCheck = zffalse;
    d->gameOverFlag = zftrue;

    for(zfindex y = 0; y < d->dataHeight; ++y)
    {
        for(zfindex x = 0; x < d->dataWidth; ++x)
        {
            if(d->dataPointerCache[x][y] == 0
                || (x + 1 < d->dataWidth && d->dataPointerCache[x][y] == d->dataPointerCache[x + 1][y])
                || (y + 1 < d->dataHeight && d->dataPointerCache[x][y] == d->dataPointerCache[x][y + 1])
                )
            {
                d->gameOverFlag = zffalse;
                return zftrue;
            }
        }
    }

    return !(d->gameOverFlag);
}

zfbool ZF2048Core::moveLeft(void)
{
    zfmemcpy(d->dataTmp.pointerValueGet(), d->data.pointerValueGet(), sizeof(ZF2048Value) * d->dataWidth * d->dataHeight);

    ZFCoreArrayPOD<ZF2048Action> actionList;
    for(zfindex y = 0; y < d->dataHeight; ++y)
    {
        zfindex posEnd = 0;
        zfindex pos = posEnd + 1;
        zfbool posEndMoved = zffalse;
        do
        {
            for( ; pos < d->dataWidth; ++pos)
            {
                if(d->dataPointerCache[pos][y] != 0)
                {
                    break;
                }
            }
            if(pos == d->dataWidth)
            {
                break;
            }
            if(pos != posEnd)
            {
                ZF2048Value value = d->dataPointerCache[pos][y];
                ZF2048Value valueEnd = d->dataPointerCache[posEnd][y];
                if(valueEnd == 0)
                {
                    posEndMoved = zftrue;
                    d->dataPointerCache[posEnd][y] = value;
                    d->dataPointerCache[pos][y] = 0;
                    actionList.add(ZF2048ActionMake(pos, y, posEnd, y, value));
                    --posEnd;
                }
                else if(value == valueEnd)
                {
                    d->dataPointerCache[posEnd][y] = value + valueEnd;
                    d->dataPointerCache[pos][y] = 0;
                    if(!posEndMoved)
                    {
                        actionList.add(ZF2048ActionMake(posEnd, y, posEnd, y, valueEnd));
                    }
                    actionList.add(ZF2048ActionMake(pos, y, posEnd, y, value));
                }
                else if(pos > posEnd + 1)
                {
                    posEndMoved = zftrue;
                    d->dataPointerCache[posEnd + 1][y] = value;
                    d->dataPointerCache[pos][y] = 0;
                    actionList.add(ZF2048ActionMake(pos, y, posEnd + 1, y, value));
                }
                else
                {
                    posEndMoved = zffalse;
                }
            }
            else
            {
                posEndMoved = zffalse;
            }
            ++pos;
            ++posEnd;
        } while(posEnd < d->dataWidth - 1);
    }

    if(actionList.isEmpty())
    {
        return zffalse;
    }

    ZF2048Action action;
    action.from = action.to = d->generateBlockPos();
    action.value = d->generateBlockValue();
    action.isNewBlock = zftrue;
    actionList.add(action);
    d->dataPointerCache[action.from.x][action.from.y] = action.value;
    d->addActionList(actionList);

    zfmemcpy(d->dataPrev.pointerValueGet(), d->dataTmp.pointerValueGet(), sizeof(ZF2048Value) * d->dataWidth * d->dataHeight);

    d->gameOverNeedCheck = zftrue;
    d->scoreUpdateQuickly(action.value);
    this->gameDataOnChange();
    return zftrue;
}
zfbool ZF2048Core::moveTop(void)
{
    zfmemcpy(d->dataTmp.pointerValueGet(), d->data.pointerValueGet(), sizeof(ZF2048Value) * d->dataWidth * d->dataHeight);

    ZFCoreArrayPOD<ZF2048Action> actionList;
    for(zfindex x = 0; x < d->dataWidth; ++x)
    {
        zfindex posEnd = 0;
        zfindex pos = posEnd + 1;
        zfbool posEndMoved = zffalse;
        do
        {
            for( ; pos < d->dataHeight; ++pos)
            {
                if(d->dataPointerCache[x][pos] != 0)
                {
                    break;
                }
            }
            if(pos == d->dataHeight)
            {
                break;
            }
            if(pos != posEnd)
            {
                ZF2048Value value = d->dataPointerCache[x][pos];
                ZF2048Value valueEnd = d->dataPointerCache[x][posEnd];
                if(valueEnd == 0)
                {
                    posEndMoved = zftrue;
                    d->dataPointerCache[x][posEnd] = value;
                    d->dataPointerCache[x][pos] = 0;
                    actionList.add(ZF2048ActionMake(x, pos, x, posEnd, value));
                    --posEnd;
                }
                else if(value == valueEnd)
                {
                    d->dataPointerCache[x][posEnd] = value + valueEnd;
                    d->dataPointerCache[x][pos] = 0;
                    if(!posEndMoved)
                    {
                        actionList.add(ZF2048ActionMake(x, posEnd, x, posEnd, valueEnd));
                    }
                    actionList.add(ZF2048ActionMake(x, pos, x, posEnd, value));
                }
                else if(pos > posEnd + 1)
                {
                    posEndMoved = zftrue;
                    d->dataPointerCache[x][posEnd + 1] = value;
                    d->dataPointerCache[x][pos] = 0;
                    actionList.add(ZF2048ActionMake(x, pos, x, posEnd + 1, value));
                }
                else
                {
                    posEndMoved = zffalse;
                }
            }
            else
            {
                posEndMoved = zffalse;
            }
            ++pos;
            ++posEnd;
        } while(posEnd < d->dataHeight - 1);
    }

    if(actionList.isEmpty())
    {
        return zffalse;
    }

    ZF2048Action action;
    action.from = action.to = d->generateBlockPos();
    action.value = d->generateBlockValue();
    action.isNewBlock = zftrue;
    actionList.add(action);
    d->dataPointerCache[action.from.x][action.from.y] = action.value;
    d->addActionList(actionList);

    zfmemcpy(d->dataPrev.pointerValueGet(), d->dataTmp.pointerValueGet(), sizeof(ZF2048Value) * d->dataWidth * d->dataHeight);

    d->gameOverNeedCheck = zftrue;
    d->scoreUpdateQuickly(action.value);
    this->gameDataOnChange();
    return zftrue;
}
zfbool ZF2048Core::moveRight(void)
{
    zfmemcpy(d->dataTmp.pointerValueGet(), d->data.pointerValueGet(), sizeof(ZF2048Value) * d->dataWidth * d->dataHeight);

    ZFCoreArrayPOD<ZF2048Action> actionList;
    for(zfindex y = 0; y < d->dataHeight; ++y)
    {
        zfindex posEnd = d->dataWidth - 1;
        zfindex pos = posEnd - 1;
        zfbool posEndMoved = zffalse;
        do
        {
            for( ; pos != zfindexMax; --pos)
            {
                if(d->dataPointerCache[pos][y] != 0)
                {
                    break;
                }
            }
            if(pos == zfindexMax)
            {
                break;
            }
            if(pos != posEnd)
            {
                ZF2048Value value = d->dataPointerCache[pos][y];
                ZF2048Value valueEnd = d->dataPointerCache[posEnd][y];
                if(valueEnd == 0)
                {
                    posEndMoved = zftrue;
                    d->dataPointerCache[posEnd][y] = value;
                    d->dataPointerCache[pos][y] = 0;
                    actionList.add(ZF2048ActionMake(pos, y, posEnd, y, value));
                    ++posEnd;
                }
                else if(value == valueEnd)
                {
                    d->dataPointerCache[posEnd][y] = value + valueEnd;
                    d->dataPointerCache[pos][y] = 0;
                    if(!posEndMoved)
                    {
                        actionList.add(ZF2048ActionMake(posEnd, y, posEnd, y, valueEnd));
                    }
                    actionList.add(ZF2048ActionMake(pos, y, posEnd, y, value));
                }
                else if(pos < posEnd - 1)
                {
                    posEndMoved = zftrue;
                    d->dataPointerCache[posEnd - 1][y] = value;
                    d->dataPointerCache[pos][y] = 0;
                    actionList.add(ZF2048ActionMake(pos, y, posEnd - 1, y, value));
                }
                else
                {
                    posEndMoved = zffalse;
                }
            }
            else
            {
                posEndMoved = zffalse;
            }
            --pos;
            --posEnd;
        } while(posEnd != 0);
    }

    if(actionList.isEmpty())
    {
        return zffalse;
    }

    ZF2048Action action;
    action.from = action.to = d->generateBlockPos();
    action.value = d->generateBlockValue();
    action.isNewBlock = zftrue;
    actionList.add(action);
    d->dataPointerCache[action.from.x][action.from.y] = action.value;
    d->addActionList(actionList);

    zfmemcpy(d->dataPrev.pointerValueGet(), d->dataTmp.pointerValueGet(), sizeof(ZF2048Value) * d->dataWidth * d->dataHeight);

    d->gameOverNeedCheck = zftrue;
    d->scoreUpdateQuickly(action.value);
    this->gameDataOnChange();
    return zftrue;
}
zfbool ZF2048Core::moveBottom(void)
{
    zfmemcpy(d->dataTmp.pointerValueGet(), d->data.pointerValueGet(), sizeof(ZF2048Value) * d->dataWidth * d->dataHeight);

    ZFCoreArrayPOD<ZF2048Action> actionList;
    for(zfindex x = 0; x < d->dataWidth; ++x)
    {
        zfindex posEnd = d->dataHeight - 1;
        zfindex pos = posEnd - 1;
        zfbool posEndMoved = zffalse;
        do
        {
            for( ; pos != zfindexMax; --pos)
            {
                if(d->dataPointerCache[x][pos] != 0)
                {
                    break;
                }
            }
            if(pos == zfindexMax)
            {
                break;
            }
            if(pos != posEnd)
            {
                ZF2048Value value = d->dataPointerCache[x][pos];
                ZF2048Value valueEnd = d->dataPointerCache[x][posEnd];
                if(valueEnd == 0)
                {
                    posEndMoved = zftrue;
                    d->dataPointerCache[x][posEnd] = value;
                    d->dataPointerCache[x][pos] = 0;
                    actionList.add(ZF2048ActionMake(x, pos, x, posEnd, value));
                    ++posEnd;
                }
                else if(value == valueEnd)
                {
                    d->dataPointerCache[x][posEnd] = value + valueEnd;
                    d->dataPointerCache[x][pos] = 0;
                    if(!posEndMoved)
                    {
                        actionList.add(ZF2048ActionMake(x, posEnd, x, posEnd, valueEnd));
                    }
                    actionList.add(ZF2048ActionMake(x, pos, x, posEnd, value));
                }
                else if(pos < posEnd - 1)
                {
                    posEndMoved = zftrue;
                    d->dataPointerCache[x][posEnd - 1] = value;
                    d->dataPointerCache[x][pos] = 0;
                    actionList.add(ZF2048ActionMake(x, pos, x, posEnd - 1, value));
                }
                else
                {
                    posEndMoved = zffalse;
                }
            }
            else
            {
                posEndMoved = zffalse;
            }
            --pos;
            --posEnd;
        } while(posEnd != 0);
    }

    if(actionList.isEmpty())
    {
        return zffalse;
    }

    ZF2048Action action;
    action.from = action.to = d->generateBlockPos();
    action.value = d->generateBlockValue();
    action.isNewBlock = zftrue;
    actionList.add(action);
    d->dataPointerCache[action.from.x][action.from.y] = action.value;
    d->addActionList(actionList);

    zfmemcpy(d->dataPrev.pointerValueGet(), d->dataTmp.pointerValueGet(), sizeof(ZF2048Value) * d->dataWidth * d->dataHeight);

    d->gameOverNeedCheck = zftrue;
    d->scoreUpdateQuickly(action.value);
    this->gameDataOnChange();
    return zftrue;
}

void ZF2048Core::undo(void)
{
    if(d->actionHistory.isEmpty())
    {
        return ;
    }

    zfmemcpy(d->dataPrev.pointerValueGet(), d->data.pointerValueGet(), sizeof(ZF2048Value) * d->dataWidth * d->dataHeight);

    ZFCoreArrayPOD<ZF2048Action> actionList = d->actionHistory.removeFirstAndGet();
    ZFCoreArrayPOD<zfindex> toBlockFix;
    for(zfindex i = actionList.count() - 1; i != zfindexMax; --i)
    {
        const ZF2048Action &action = actionList[i];
        if(action.from == action.to)
        {
            if(action.isNewBlock)
            {
                d->dataPointerCache[action.from.x][action.from.y] = 0;
            }
            else
            {
                d->dataPointerCache[action.from.x][action.from.y] = action.value;
                toBlockFix.removeElement(d->posToIndex(action.from));
            }
        }
        else
        {
            d->dataPointerCache[action.from.x][action.from.y] = action.value;
            zfindex fromIndex = d->posToIndex(action.from);
            zfindex toIndex = d->posToIndex(action.to);
            if(fromIndex != toIndex)
            {
                toBlockFix.removeElement(fromIndex);
                if(toBlockFix.find(toIndex) == zfindexMax)
                {
                    toBlockFix.add(toIndex);
                }
            }
        }
    }
    for(zfindex i = 0; i < toBlockFix.count(); ++i)
    {
        d->data[toBlockFix[i]] = 0;
    }

    d->gameOverNeedCheck = zffalse;
    d->gameOverFlag = zffalse;
    d->scoreUpdate();

    this->gameDataOnChange();
}

void ZF2048Core::debugStatus(ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */)
{
    d->debugStatus(outputCallback, d->data.pointerValueGet());
}

void ZF2048Core::gameDataOnChange(void)
{
    this->observerNotify(ZF2048Core::EventGameDataOnChange());
}

ZFObject *ZF2048Core::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfnew(_ZFP_ZF2048CorePrivate, this);

    this->reset();
    return this;
}
void ZF2048Core::objectOnDealloc(void)
{
    zfdelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

