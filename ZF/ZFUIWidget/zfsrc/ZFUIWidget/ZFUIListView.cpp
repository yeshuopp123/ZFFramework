/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIListView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIListView)

// ============================================================
// _ZFP_ZFUIListViewPrivate
zfclassNotPOD _ZFP_ZFUIListViewPrivate
{
public:
    ZFUIListView *pimplOwner;
    zfbool childAddOverrideFlag;
    zfbool scrollContentFrameOverrideFlag;
    ZFUIListAdapter *listAdapter;
    zfbool listReloadRequested;
    zfbool listQuickReloadRequested;
    zfbool cellNeedUpdate;
    zfindex cellCount;
    ZFCoreArrayPOD<zfint> cellSizeList;
    ZFCoreArrayPOD<ZFUIListCell *> listVisibleCell; // retain manually
    zfindexRange listVisibleCellIndexRange;
    /*
     * left: left most cell's x
     * top: top most cell's y
     * right: right most cell's (x + width)
     * bottom: bottom most cell's (y + height)
     */
    zfint listVisibleCellOffset;
    zfbool listVisibleCellOffsetNeedUpdate;
    zfbool listReloadByChangeListOrientation;

    /*
     * used by scrollListCellToHead/Tail
     * if activating, we will recursively scrollByPoint until reach desired position
     * task would be canceled if content frame changed manually
     */
    zfindex scrollListCellIndex; // zfindexMax if not activating
    zfint scrollListCellOffset;
    zfbool scrollListCellToHead;
    zfbool scrollListCellAnimated;
    zfint scrollListCellDesiredPosSaved;

public:
    _ZFP_ZFUIListViewPrivate(void)
    : pimplOwner(zfnull)
    , childAddOverrideFlag(zffalse)
    , scrollContentFrameOverrideFlag(zffalse)
    , listAdapter(zfnull)
    , listReloadRequested(zftrue)
    , listQuickReloadRequested(zftrue)
    , cellNeedUpdate(zftrue)
    , cellCount(0)
    , cellSizeList()
    , listVisibleCell()
    , listVisibleCellIndexRange(zfindexRangeZero)
    , listVisibleCellOffset(0)
    , listVisibleCellOffsetNeedUpdate(zftrue)
    , listReloadByChangeListOrientation(zftrue)
    , scrollListCellIndex(zfindexMax)
    , scrollListCellOffset(0)
    , scrollListCellToHead(zftrue)
    , scrollListCellAnimated(zftrue)
    , scrollListCellDesiredPosSaved(0)
    {
    }

public:
    void listBounceUpdate(void)
    {
        if(this->pimplOwner->listBounce())
        {
            switch(this->pimplOwner->listOrientation())
            {
                case ZFUIOrientation::e_Left:
                case ZFUIOrientation::e_Right:
                    this->pimplOwner->scrollBounceVerticalAlwaysSet(zffalse);
                    this->pimplOwner->scrollBounceHorizontalAlwaysSet(zftrue);
                    break;
                case ZFUIOrientation::e_Top:
                case ZFUIOrientation::e_Bottom:
                    this->pimplOwner->scrollBounceVerticalAlwaysSet(zftrue);
                    this->pimplOwner->scrollBounceHorizontalAlwaysSet(zffalse);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
        }
    }

public:
    void childAdd(ZF_IN ZFUIView *child)
    {
        this->childAddOverrideFlag = zftrue;
        this->pimplOwner->childAdd(child);
        this->childAddOverrideFlag = zffalse;
    }
    void childAdd(ZF_IN ZFUIView *child, ZFUIViewLayoutParam *layoutParam, ZF_IN zfindex index)
    {
        this->childAddOverrideFlag = zftrue;
        this->pimplOwner->childAdd(child, layoutParam, index);
        this->childAddOverrideFlag = zffalse;
    }
    void childRemoveAtIndex(ZF_IN zfindex index)
    {
        this->childAddOverrideFlag = zftrue;
        this->pimplOwner->childRemoveAtIndex(index);
        this->childAddOverrideFlag = zffalse;
    }
    zfautoObject cellLoadAtIndex(ZF_IN zfindex index)
    {
        zfautoObject ret = this->listAdapter->cellCacheOnAccess(index);
        if(ret != zfautoObjectNull)
        {
            zfCoreAssertWithMessage(ZFCastZFObject(ZFUIListCell *, ret.toObject()) != zfnull, zfTextA("list cell %s not type of %s"),
                zfsCoreZ2A(ret.toObject()->classData()->className()),
                zfsCoreZ2A(ZFUIListCell::ClassData()->className()));
            return ret;
        }
        ret = this->listAdapter->cellAtIndex(index);
        zfCoreAssertWithMessage(ret != zfautoObjectNull, zfTextA("cellAtIndex must return a %s"), zfsCoreZ2A(ZFUIListCell::ClassData()->className()));
        ZFUIListCell *cell = ZFCastZFObject(ZFUIListCell *, ret.toObject());
        zfCoreAssertWithMessage(cell != zfnull, zfTextA("list cell %s not type of %s"),
            zfsCoreZ2A(ret.toObject()->classData()->className()),
            zfsCoreZ2A(ZFUIListCell::ClassData()->className()));
        return ret;
    }
    void cellOnUpdate(ZF_IN zfindex index, ZF_IN ZFUIListCell *cell)
    {
        if(!this->pimplOwner->cellUpdater()->isEmpty())
        {
            ZFUIListCellUpdaterParam updateParam;
            updateParam.cell = cell;
            updateParam.cellIndex = index;
            updateParam.cellCount = this->listAdapter->cellCount();
            updateParam.listOrientation = this->listAdapter->listOrientation();
            updateParam.listContainerSize = this->listAdapter->listContainerSize();
            updateParam.cellSizeHint = this->listAdapter->cellSizeHint();
            for(zfindex i = 0; i < this->pimplOwner->cellUpdater()->count(); ++i)
            {
                this->pimplOwner->cellUpdater()->get<ZFUIListCellUpdater *>(i)->cellOnUpdate(updateParam);
            }
        }
        this->listAdapter->cellOnUpdate(index, cell);
    }
    void cellOnRecycle(ZF_IN ZFUIListCell *cell)
    {
        if(!this->pimplOwner->cellUpdater()->isEmpty())
        {
            for(zfindex i = 0; i < this->pimplOwner->cellUpdater()->count(); ++i)
            {
                this->pimplOwner->cellUpdater()->get<ZFUIListCellUpdater *>(i)->cellOnRecycle(cell);
            }
        }
        this->listAdapter->cellCacheOnRecycle(cell);
    }
    zfint cellSizeAtIndex(ZF_IN zfindex index, ZF_IN ZFUIListCell *cell)
    {
        zfint ret = this->listAdapter->cellSizeAtIndex(index, cell);
        if(ret < 0)
        {
            switch(this->pimplOwner->listOrientation())
            {
                case ZFUIOrientation::e_Left:
                case ZFUIOrientation::e_Right:
                    ret = cell->layoutMeasure(
                            ZFUISizeMake(-1, this->pimplOwner->scrollAreaHeight()),
                            ZFUISizeParamWrapWidthFillHeight
                        ).width;
                    break;
                case ZFUIOrientation::e_Top:
                case ZFUIOrientation::e_Bottom:
                    ret = cell->layoutMeasure(
                            ZFUISizeMake(this->pimplOwner->scrollAreaWidth(), -1),
                            ZFUISizeParamFillWidthWrapHeight
                        ).height;
                    break;
            }
        }
        return ret;
    }
    void removeAll(void)
    {
        this->cellCount = 0;
        this->cellSizeList.removeAll();
        this->listVisibleCellIndexRange = zfindexRangeZero;
        this->listVisibleCellOffset = 0;
        if(!this->listVisibleCell.isEmpty())
        {
            for(zfindex i = this->listVisibleCell.count() - 1; i != zfindexMax; --i)
            {
                ZFUIListCell *cell = this->listVisibleCell[i];
                this->childRemoveAtIndex(i);
                if(this->listAdapter != zfnull)
                {
                    this->cellOnRecycle(cell);
                }
                this->pimplOwner->cellOnDetach(cell);
                zfRelease(cell);
            }
            this->listVisibleCell.removeAll();
        }
    }
    void cellRemoveBefore(ZF_IN zfindex index)
    {
        if(index < this->listVisibleCellIndexRange.start || index == zfindexMax)
        {
            return ;
        }

        zfindex indexOfVisibleCell = index - this->listVisibleCellIndexRange.start;
        for(zfindex i = indexOfVisibleCell; i != zfindexMax; --i)
        {
            this->childRemoveAtIndex(i);
            ZFUIListCell *cell = this->listVisibleCell[i];
            this->cellOnRecycle(cell);
            this->pimplOwner->cellOnDetach(cell);
            zfRelease(cell);
        }
        this->listVisibleCell.remove(0, indexOfVisibleCell + 1);
        this->listVisibleCellIndexRange.start = index + 1;
        this->listVisibleCellIndexRange.count = this->listVisibleCell.count();
        if(this->listVisibleCellIndexRange.count == 0)
        {
            this->listVisibleCellIndexRange.start = 0;
        }
    }
    void cellRemoveAfter(ZF_IN zfindex index)
    {
        if(index >= this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
        {
            return ;
        }

        zfindex indexOfVisibleCell = index - this->listVisibleCellIndexRange.start;
        for(zfindex i = this->listVisibleCellIndexRange.count - 1; i != zfindexMax && i >= indexOfVisibleCell; --i)
        {
            this->childRemoveAtIndex(i);
            ZFUIListCell *cell = this->listVisibleCell[i];
            this->cellOnRecycle(cell);
            this->pimplOwner->cellOnDetach(cell);
            zfRelease(cell);
        }
        this->listVisibleCell.remove(indexOfVisibleCell, zfindexMax);
        this->listVisibleCellIndexRange.count = this->listVisibleCell.count();
        if(this->listVisibleCellIndexRange.count == 0)
        {
            this->listVisibleCellIndexRange.start = 0;
        }
    }
    void updateHeadCellBeforeIndex(ZF_IN zfindex index,
                                   ZF_IN const ZFUIRect &cellFrame)
    {
        this->scrollContentFrameOverrideFlag = zftrue;

        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
            {
                zfint offset = cellFrame.point.x + cellFrame.size.width;
                zfint offsetEnd = -this->pimplOwner->scrollContentFrame().point.x;
                zfint offsetBegin = offsetEnd + this->pimplOwner->scrollAreaWidth();
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; --index)
                {
                    if(offset < offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveBefore(index);
                        break;
                    }
                    offset -= this->cellSizeList[index];
                }
                // skip
                for( ; index != zfindexMax && offset - this->cellSizeList[index] > offsetBegin; --index)
                {
                    offset -= this->cellSizeList[index];
                }
                // load
                if(index != zfindexMax)
                {
                    zfint sizeDelta = 0;
                    for( ; index != zfindexMax && offset >= offsetEnd; --index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset -= this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(0, cellNew);
                        this->listVisibleCellIndexRange.start = index;
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew, zfnull, 0);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        offset -= cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.point.x -= sizeDelta;
                        contentFrame.size.width += sizeDelta;
                        this->pimplOwner->scrollContentFrameSetWhileAnimating(contentFrame);
                    }
                    if(index == zfindexMax)
                    {
                        this->listVisibleCellOffset = 0;
                    }
                    else
                    {
                        this->listVisibleCellOffset = offset + sizeDelta;
                    }
                }
            }
                break;
            case ZFUIOrientation::e_Top:
            {
                zfint offset = cellFrame.point.y + cellFrame.size.height;
                zfint offsetEnd = -this->pimplOwner->scrollContentFrame().point.y;
                zfint offsetBegin = offsetEnd + this->pimplOwner->scrollAreaHeight();
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; --index)
                {
                    if(offset < offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveBefore(index);
                        break;
                    }
                    offset -= this->cellSizeList[index];
                }
                // skip
                for( ; index != zfindexMax && offset - this->cellSizeList[index] > offsetBegin; --index)
                {
                    offset -= this->cellSizeList[index];
                }
                // load
                if(index != zfindexMax)
                {
                    zfint sizeDelta = 0;
                    for( ; index != zfindexMax && offset >= offsetEnd; --index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset -= this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(0, cellNew);
                        this->listVisibleCellIndexRange.start = index;
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew, zfnull, 0);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        offset -= cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.point.y -= sizeDelta;
                        contentFrame.size.height += sizeDelta;
                        this->pimplOwner->scrollContentFrameSetWhileAnimating(contentFrame);
                    }
                    if(index == zfindexMax)
                    {
                        this->listVisibleCellOffset = 0;
                    }
                    else
                    {
                        this->listVisibleCellOffset = offset + sizeDelta;
                    }
                }
            }
                break;
            case ZFUIOrientation::e_Right:
            {
                zfint offset = cellFrame.point.x;
                zfint offsetEnd = this->pimplOwner->scrollAreaWidth() - this->pimplOwner->scrollContentFrame().point.x;
                zfint offsetBegin = offsetEnd - this->pimplOwner->scrollAreaWidth();
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; --index)
                {
                    if(offset > offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveBefore(index);
                        break;
                    }
                    offset += this->cellSizeList[index];
                }
                // skip
                for( ; index != zfindexMax && offset + this->cellSizeList[index] < offsetBegin; --index)
                {
                    offset += this->cellSizeList[index];
                }
                // load
                if(index != zfindexMax)
                {
                    zfint sizeDelta = 0;
                    for( ; index != zfindexMax && offset <= offsetEnd; --index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset += this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(0, cellNew);
                        this->listVisibleCellIndexRange.start = index;
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew, zfnull, 0);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        offset += cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.size.width += sizeDelta;
                        this->pimplOwner->scrollContentFrameSetWhileAnimating(contentFrame);
                    }
                    this->listVisibleCellOffset = offset;
                }
            }
                break;
            case ZFUIOrientation::e_Bottom:
            {
                zfint offset = cellFrame.point.y;
                zfint offsetEnd = this->pimplOwner->scrollAreaHeight() - this->pimplOwner->scrollContentFrame().point.y;
                zfint offsetBegin = offsetEnd - this->pimplOwner->scrollAreaHeight();
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; --index)
                {
                    if(offset > offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveBefore(index);
                        break;
                    }
                    offset += this->cellSizeList[index];
                }
                // skip
                for( ; index != zfindexMax && offset + this->cellSizeList[index] < offsetBegin; --index)
                {
                    offset += this->cellSizeList[index];
                }
                // load
                if(index != zfindexMax)
                {
                    zfint sizeDelta = 0;
                    for( ; index != zfindexMax && offset <= offsetEnd; --index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset += this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(0, cellNew);
                        this->listVisibleCellIndexRange.start = index;
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew, zfnull, 0);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        offset += cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.size.height += sizeDelta;
                        this->pimplOwner->scrollContentFrameSetWhileAnimating(contentFrame);
                    }
                    this->listVisibleCellOffset = offset;
                }
            }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }

        this->listVisibleCellOffsetNeedUpdate = zffalse;
        this->scrollContentFrameOverrideFlag = zffalse;
    }
    void updateTailCellAfterIndex(ZF_IN zfindex index,
                                  ZF_IN const ZFUIRect &cellFrame)
    {
        this->scrollContentFrameOverrideFlag = zftrue;

        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
            {
                zfint offset = cellFrame.point.x;
                zfint offsetEnd = this->pimplOwner->scrollAreaWidth() - this->pimplOwner->scrollContentFrame().point.x;
                zfint offsetBegin = offsetEnd - this->pimplOwner->scrollAreaWidth();
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; ++index)
                {
                    if(offset > offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveAfter(index);
                        break;
                    }
                    offset += this->cellSizeList[index];
                }
                // skip
                for( ; index < this->cellCount && offset + this->cellSizeList[index] < offsetBegin; ++index)
                {
                    offset += this->cellSizeList[index];
                }
                // load
                if(index < this->cellCount)
                {
                    zfint sizeDelta = 0;
                    for( ; index < this->cellCount && offset <= offsetEnd; ++index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset += this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(cellNew);
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        if(this->listVisibleCellIndexRange.count == 1)
                        {
                            this->listVisibleCellIndexRange.start = index;
                            if(index == 0)
                            {
                                this->listVisibleCellOffset = 0;
                            }
                            else
                            {
                                this->listVisibleCellOffset = offset;
                            }
                        }
                        offset += cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.size.width += sizeDelta;
                        this->pimplOwner->scrollContentFrameSetWhileAnimating(contentFrame);
                    }
                }
            }
                break;
            case ZFUIOrientation::e_Top:
            {
                zfint offset = cellFrame.point.y;
                zfint offsetEnd = this->pimplOwner->scrollAreaHeight() - this->pimplOwner->scrollContentFrame().point.y;
                zfint offsetBegin = offsetEnd - this->pimplOwner->scrollAreaHeight();
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; ++index)
                {
                    if(offset > offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveAfter(index);
                        break;
                    }
                    offset += this->cellSizeList[index];
                }
                // skip
                for( ; index < this->cellCount && offset + this->cellSizeList[index] < offsetBegin; ++index)
                {
                    offset += this->cellSizeList[index];
                }
                // load
                if(index < this->cellCount)
                {
                    zfint sizeDelta = 0;
                    for( ; index < this->cellCount && offset <= offsetEnd; ++index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset += this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(cellNew);
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        if(this->listVisibleCellIndexRange.count == 1)
                        {
                            this->listVisibleCellIndexRange.start = index;
                            if(index == 0)
                            {
                                this->listVisibleCellOffset = 0;
                            }
                            else
                            {
                                this->listVisibleCellOffset = offset;
                            }
                        }
                        offset += cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.size.height += sizeDelta;
                        this->pimplOwner->scrollContentFrameSetWhileAnimating(contentFrame);
                    }
                }
            }
                break;
            case ZFUIOrientation::e_Right:
            {
                zfint offset = cellFrame.point.x + cellFrame.size.width;
                zfint offsetEnd = -this->pimplOwner->scrollContentFrame().point.x;
                zfint offsetBegin = offsetEnd + this->pimplOwner->scrollAreaWidth();
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; ++index)
                {
                    if(offset < offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveAfter(index);
                        break;
                    }
                    offset -= this->cellSizeList[index];
                }
                // skip
                for( ; index < this->cellCount && offset - this->cellSizeList[index] > offsetBegin; ++index)
                {
                    offset -= this->cellSizeList[index];
                }
                // load
                if(index < this->cellCount)
                {
                    zfint sizeDelta = 0;
                    for( ; index < this->cellCount && offset >= offsetEnd; ++index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset -= this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(cellNew);
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        if(this->listVisibleCellIndexRange.count == 1)
                        {
                            this->listVisibleCellOffset = offset;
                        }
                        offset -= cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        this->listVisibleCellOffset += sizeDelta;
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.point.x -= sizeDelta;
                        contentFrame.size.width += sizeDelta;
                        this->pimplOwner->scrollContentFrameSetWhileAnimating(contentFrame);
                    }
                }
            }
                break;
            case ZFUIOrientation::e_Bottom:
            {
                zfint offset = cellFrame.point.y + cellFrame.size.height;
                zfint offsetEnd = -this->pimplOwner->scrollContentFrame().point.y;
                zfint offsetBegin = offsetEnd + this->pimplOwner->scrollAreaHeight();
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; ++index)
                {
                    if(offset < offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveAfter(index);
                        break;
                    }
                    offset -= this->cellSizeList[index];
                }
                // skip
                for( ; index < this->cellCount && offset - this->cellSizeList[index] > offsetBegin; ++index)
                {
                    offset -= this->cellSizeList[index];
                }
                // load
                if(index < this->cellCount)
                {
                    zfint sizeDelta = 0;
                    for( ; index < this->cellCount && offset >= offsetEnd; ++index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset -= this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(cellNew);
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        if(this->listVisibleCellIndexRange.count == 1)
                        {
                            this->listVisibleCellOffset = offset;
                        }
                        offset -= cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        this->listVisibleCellOffset += sizeDelta;
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.point.y -= sizeDelta;
                        contentFrame.size.height += sizeDelta;
                        this->pimplOwner->scrollContentFrameSetWhileAnimating(contentFrame);
                    }
                }
            }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }

        this->listVisibleCellOffsetNeedUpdate = zffalse;
        this->scrollContentFrameOverrideFlag = zffalse;
    }
    void updateFromFirstCell(void)
    {
        if(this->cellCount == 0)
        {
            return ;
        }

        zfint cellSize = this->cellSizeList[0];
        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
                this->updateTailCellAfterIndex(0, ZFUIRectMake(
                    0,
                    0,
                    cellSize,
                    this->pimplOwner->scrollAreaHeight()));
                break;
            case ZFUIOrientation::e_Top:
                this->updateTailCellAfterIndex(0, ZFUIRectMake(
                    0,
                    0,
                    this->pimplOwner->scrollAreaWidth(),
                    cellSize));
                break;
            case ZFUIOrientation::e_Right:
                this->updateTailCellAfterIndex(0, ZFUIRectMake(
                    this->pimplOwner->scrollContentFrame().size.width - cellSize,
                    0,
                    cellSize,
                    this->pimplOwner->scrollAreaHeight()));
                break;
            case ZFUIOrientation::e_Bottom:
                this->updateTailCellAfterIndex(0, ZFUIRectMake(
                    0,
                    this->pimplOwner->scrollContentFrame().size.height - cellSize,
                    this->pimplOwner->scrollAreaWidth(),
                    cellSize));
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
    ZFUIRect listVisibleCellFrame(ZF_IN zfindex cellIndex)
    {
        ZFUIRect ret = ZFUIRectZero;
        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
                ret.point.x = this->listVisibleCellOffset;
                ret.size.height = this->pimplOwner->scrollAreaHeight();
                for(zfindex i = this->listVisibleCellIndexRange.start; ; ++i)
                {
                    if(i == cellIndex)
                    {
                        ret.size.width = this->cellSizeList[i];
                        break;
                    }
                    ret.point.x += this->cellSizeList[i];
                }
                break;
            case ZFUIOrientation::e_Top:
                ret.point.y = this->listVisibleCellOffset;
                ret.size.width = this->pimplOwner->scrollAreaWidth();
                for(zfindex i = this->listVisibleCellIndexRange.start; ; ++i)
                {
                    if(i == cellIndex)
                    {
                        ret.size.height = this->cellSizeList[i];
                        break;
                    }
                    ret.point.y += this->cellSizeList[i];
                }
                break;
            case ZFUIOrientation::e_Right:
                ret.point.x = this->listVisibleCellOffset;
                ret.size.height = this->pimplOwner->scrollAreaHeight();
                for(zfindex i = this->listVisibleCellIndexRange.start; ; ++i)
                {
                    ret.point.x -= this->cellSizeList[i];
                    if(i == cellIndex)
                    {
                        ret.size.width = this->cellSizeList[i];
                        break;
                    }
                }
                break;
            case ZFUIOrientation::e_Bottom:
                ret.point.y = this->listVisibleCellOffset;
                ret.size.width = this->pimplOwner->scrollAreaWidth();
                for(zfindex i = this->listVisibleCellIndexRange.start; ; ++i)
                {
                    ret.point.y -= this->cellSizeList[i];
                    if(i == cellIndex)
                    {
                        ret.size.height = this->cellSizeList[i];
                        break;
                    }
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ZFUIRectZero;
        }
        return ret;
    }
    void listCheckReload(void)
    {
        if(!this->listReloadRequested)
        {
            if(this->listQuickReloadRequested)
            {
                this->listQuickReloadRequested = zffalse;
                ZFUIRect scrollContentFrame = this->pimplOwner->scrollContentFrame();
                switch(this->pimplOwner->listOrientation())
                {
                    case ZFUIOrientation::e_Left:
                    case ZFUIOrientation::e_Right:
                        scrollContentFrame.size.height = this->pimplOwner->scrollAreaHeight();
                        break;
                    case ZFUIOrientation::e_Top:
                    case ZFUIOrientation::e_Bottom:
                        scrollContentFrame.size.width = this->pimplOwner->scrollAreaWidth();
                        break;
                    default:
                        zfCoreCriticalShouldNotGoHere();
                        return ;
                }
                if(scrollContentFrame != this->pimplOwner->scrollContentFrame())
                {
                    this->scrollContentFrameOverrideFlag = zftrue;
                    this->pimplOwner->scrollContentFrameSetWhileAnimating(scrollContentFrame);
                    this->scrollContentFrameOverrideFlag = zffalse;
                }
                if(!this->listVisibleCell.isEmpty())
                {
                    zfindex cellIndex = this->listVisibleCellIndexRange.start;
                    this->updateTailCellAfterIndex(
                        cellIndex,
                        this->listVisibleCellFrame(cellIndex));
                }
                else
                {
                    this->updateFromFirstCell();
                }
            }
            return ;
        }

        if(this->listAdapter == zfnull)
        {
            this->removeAll();
            this->scrollContentFrameOverrideFlag = zftrue;
            this->pimplOwner->scrollContentFrameSet(ZFUIRectZero);
            this->scrollContentFrameOverrideFlag = zffalse;
            this->listReloadRequested = zffalse;
            this->listQuickReloadRequested = zffalse;
            this->pimplOwner->listVisibleCellOnChange();
            return ;
        }

        this->cellNeedUpdate = zftrue;

        this->cellCount = this->listAdapter->cellCount();
        if(this->cellSizeList.count() > this->cellCount)
        {
            this->cellSizeList.remove(this->cellCount, zfindexMax);
        }
        else
        {
            zfint cellSizeHint = zfmMax(this->pimplOwner->cellSizeHint(), ZFUIGlobalStyle::DefaultStyle()->itemSizeListCell());
            this->cellSizeList.capacitySet(this->cellCount);
            for(zfindex i = this->cellSizeList.count(); i < this->cellCount; ++i)
            {
                this->cellSizeList.add(cellSizeHint);
            }
        }
        zfint totalSize = 0;
        {
            const zfint *buf = this->cellSizeList.arrayBuf();
            for(zfindex i = 0, iEnd = this->cellSizeList.count(); i < iEnd; ++i)
            {
                totalSize += *buf;
                ++buf;
            }
        }
        this->scrollContentFrameOverrideFlag = zftrue;
        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
            case ZFUIOrientation::e_Right:
            {
                ZFUIRect scrollContentFrameNew = ZFUIRectMake(
                    this->pimplOwner->scrollContentFrame().point.x,
                    this->pimplOwner->scrollAreaMargin().top,
                    zfmMax(totalSize, this->pimplOwner->scrollAreaWidth()),
                    this->pimplOwner->scrollAreaHeight());
                if(this->listReloadByChangeListOrientation)
                {
                    if(this->pimplOwner->listOrientation() == ZFUIOrientation::e_Left)
                    {
                        scrollContentFrameNew.point.x = 0;
                    }
                    else
                    {
                        scrollContentFrameNew.point.x = this->pimplOwner->scrollAreaWidth() - scrollContentFrameNew.size.width;
                    }
                }
                this->pimplOwner->scrollContentFrameSetWhileAnimating(scrollContentFrameNew);
            }
                break;
            case ZFUIOrientation::e_Top:
            case ZFUIOrientation::e_Bottom:
            {
                ZFUIRect scrollContentFrameNew = ZFUIRectMake(
                    this->pimplOwner->scrollAreaMargin().left,
                    this->pimplOwner->scrollContentFrame().point.y,
                    this->pimplOwner->scrollAreaWidth(),
                    zfmMax(totalSize, this->pimplOwner->scrollAreaHeight()));
                if(this->listReloadByChangeListOrientation)
                {
                    if(this->pimplOwner->listOrientation() == ZFUIOrientation::e_Top)
                    {
                        scrollContentFrameNew.point.y = 0;
                    }
                    else
                    {
                        scrollContentFrameNew.point.y = this->pimplOwner->scrollAreaHeight() - scrollContentFrameNew.size.height;
                    }
                }
                this->pimplOwner->scrollContentFrameSetWhileAnimating(scrollContentFrameNew);
            }
                break;
        }
        this->scrollContentFrameOverrideFlag = zffalse;

        zfindex cellIndex = zfindexMax;
        if(!this->listVisibleCell.isEmpty())
        {
            cellIndex = this->listVisibleCellIndexRange.start;
        }

        this->listVisibleCellIndexRange = zfindexRangeZero;
        for(zfindex i = this->listVisibleCell.count() - 1; i != zfindexMax; --i)
        {
            ZFUIListCell *cell = this->listVisibleCell[i];
            this->childRemoveAtIndex(i);
            this->cellOnRecycle(cell);
            this->pimplOwner->cellOnDetach(cell);
            zfRelease(cell);
        }
        this->listVisibleCell.removeAll();

        if(cellIndex != zfindexMax && !this->listVisibleCellOffsetNeedUpdate)
        {
            ZFUIRect cellFrame = this->listVisibleCellFrame(cellIndex);
            this->updateHeadCellBeforeIndex(cellIndex, cellFrame);
            if(!this->listVisibleCell.isEmpty())
            {
                cellIndex = this->listVisibleCellIndexRange.start;
                cellFrame = this->listVisibleCellFrame(cellIndex);
                this->updateTailCellAfterIndex(cellIndex, cellFrame);
            }
            else
            {
                this->updateFromFirstCell();
            }
        }
        else
        {
            this->updateFromFirstCell();
        }

        this->listReloadRequested = zffalse;
        this->listQuickReloadRequested = zffalse;
        this->listReloadByChangeListOrientation = zffalse;
        this->pimplOwner->listVisibleCellOnChange();

        // fix content range
        ZFUIRect scrollContentFrame = this->pimplOwner->scrollContentFrame();
        if(scrollContentFrame.point.x > 0)
        {
            scrollContentFrame.point.x = 0;
        }
        if(scrollContentFrame.point.y > 0)
        {
            scrollContentFrame.point.y = 0;
        }
        if(scrollContentFrame.size.width > this->pimplOwner->scrollAreaWidth()
            && scrollContentFrame.point.x + scrollContentFrame.size.width < this->pimplOwner->scrollAreaWidth())
        {
            scrollContentFrame.point.x = this->pimplOwner->scrollAreaWidth() - scrollContentFrame.size.width;
        }
        if(scrollContentFrame.size.height > this->pimplOwner->scrollAreaHeight()
            && scrollContentFrame.point.y + scrollContentFrame.size.height < this->pimplOwner->scrollAreaHeight())
        {
            scrollContentFrame.point.y = this->pimplOwner->scrollAreaHeight() - scrollContentFrame.size.height;
        }
        this->pimplOwner->scrollContentFrameSetWhileAnimating(scrollContentFrame);

        this->pimplOwner->scrollToFitRange();
    }
    void updateCellLayout(void)
    {
        if(!this->cellNeedUpdate)
        {
            return ;
        }
        this->cellNeedUpdate = zffalse;

        zfint offset = 0;
        zfint fillSize = 0;
        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
                offset = this->listVisibleCellOffset;
                fillSize = this->pimplOwner->scrollAreaHeight();
                for(zfindex i = 0; i < this->listVisibleCell.count(); ++i)
                {
                    zfint cellWidth = this->cellSizeList[this->listVisibleCellIndexRange.start + i];
                    this->listVisibleCell[i]->layout(ZFUIRectMake(
                            offset,
                            this->pimplOwner->scrollAreaMargin().top,
                            cellWidth,
                            fillSize
                        ));
                    offset += cellWidth;
                }
                break;
            case ZFUIOrientation::e_Top:
                offset = this->listVisibleCellOffset;
                fillSize = this->pimplOwner->scrollAreaWidth();
                for(zfindex i = 0; i < this->listVisibleCell.count(); ++i)
                {
                    zfint cellHeight = this->cellSizeList[this->listVisibleCellIndexRange.start + i];
                    this->listVisibleCell[i]->layout(ZFUIRectMake(
                            this->pimplOwner->scrollAreaMargin().left,
                            offset,
                            fillSize,
                            cellHeight
                        ));
                    offset += cellHeight;
                }
                break;
            case ZFUIOrientation::e_Right:
                offset = this->listVisibleCellOffset;
                fillSize = this->pimplOwner->scrollAreaHeight();
                for(zfindex i = 0; i < this->listVisibleCell.count(); ++i)
                {
                    zfint cellWidth = this->cellSizeList[this->listVisibleCellIndexRange.start + i];
                    offset -= cellWidth;
                    this->listVisibleCell[i]->layout(ZFUIRectMake(
                            offset,
                            this->pimplOwner->scrollAreaMargin().top,
                            cellWidth,
                            fillSize
                        ));
                }
                break;
            case ZFUIOrientation::e_Bottom:
                offset = this->listVisibleCellOffset;
                fillSize = this->pimplOwner->scrollAreaWidth();
                for(zfindex i = 0; i < this->listVisibleCell.count(); ++i)
                {
                    zfint cellHeight = this->cellSizeList[this->listVisibleCellIndexRange.start + i];
                    offset -= cellHeight;
                    this->listVisibleCell[i]->layout(ZFUIRectMake(
                            this->pimplOwner->scrollAreaMargin().left,
                            offset,
                            fillSize,
                            cellHeight
                        ));
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }

public:
    void scrollListCellCheckUpdate(void)
    {
        if(this->scrollListCellIndex == zfindexMax)
        {
            return ;
        }

        zfint desiredPos = this->scrollListCellDesiredPosCalc();
        if(!this->scrollListCellAnimated)
        {
            while(this->scrollListCellIndex != zfindexMax && desiredPos != this->scrollListCellDesiredPosSaved)
            {
                this->scrollListCellScrollToPos(desiredPos, zffalse);
            }
            this->scrollListCellIndex = zfindexMax;
            return ;
        }
        if(desiredPos != this->scrollListCellDesiredPosSaved)
        {
            this->scrollListCellScrollToPos(desiredPos, zftrue);
        }
        else
        {
            this->scrollListCellIndex = zfindexMax;
        }
    }
private:
    zfint scrollListCellDesiredPosCalc(void)
    {
        zfint offset = 0;
        for(const zfint *p = this->cellSizeList.arrayBuf(), *pEnd = p + this->scrollListCellIndex; p != pEnd; ++p)
        {
            offset += *p;
        }
        if(this->scrollListCellToHead)
        {
            return this->scrollListCellOffset - offset;
        }
        else
        {
            if(ZFUIOrientationIsHorizontal(this->pimplOwner->listOrientation()))
            {
                return this->pimplOwner->scrollAreaWidth() - (offset + this->cellSizeList[this->scrollListCellIndex] + this->scrollListCellOffset);
            }
            else
            {
                return this->pimplOwner->scrollAreaHeight() - (offset + this->cellSizeList[this->scrollListCellIndex] + this->scrollListCellOffset);
            }
        }
    }
    void scrollListCellScrollToPos(ZF_IN zfint pos,
                                   ZF_IN zfbool animated)
    {
        this->scrollListCellDesiredPosSaved = pos;
        this->pimplOwner->scrollOverrideSet(zftrue);
        const ZFUIRect &scrollContentFrame = this->pimplOwner->scrollContentFrame();
        zfint posX = 0;
        zfint posY = 0;
        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
                posX = pos;
                posY = scrollContentFrame.point.y;
                break;
            case ZFUIOrientation::e_Top:
                posX = scrollContentFrame.point.x;
                posY = pos;
                break;
            case ZFUIOrientation::e_Right:
                posX = this->pimplOwner->scrollAreaWidth() - pos - scrollContentFrame.size.width;
                posY = scrollContentFrame.point.y;
                break;
            case ZFUIOrientation::e_Bottom:
                posX = scrollContentFrame.point.x;
                posY = this->pimplOwner->scrollAreaHeight() - pos - scrollContentFrame.size.height;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        if(animated)
        {
            this->pimplOwner->scrollByPoint(posX, posY);
        }
        else
        {
            this->pimplOwner->scrollContentFrameSet(ZFUIRectMake(posX, posY, scrollContentFrame.size.width, scrollContentFrame.size.height));
        }
        this->pimplOwner->scrollOverrideSet(zffalse);
    }
};

// ============================================================
// ZFUIListView
ZFOBJECT_REGISTER(ZFUIListView)

ZFOBSERVER_EVENT_REGISTER(ZFUIListView, ListCellOnAttach)
ZFOBSERVER_EVENT_REGISTER(ZFUIListView, ListCellOnDetach)
ZFOBSERVER_EVENT_REGISTER(ZFUIListView, ListVisibleCellOnChange)

#define _ZFP_ZFUIListView_listAdapterHolderTag zfText("_ZFP_ZFUIListView_listAdapterHolderTag")
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIListView, ZFUIListAdapter *, listAdapter)
{
    zfbool changed = (this->listAdapter() != newValue);
    this->tagRemove(_ZFP_ZFUIListView_listAdapterHolderTag);
    this->listAdapterSetInternal(newValue);
    d->listAdapter = this->listAdapter();
    if(changed)
    {
        this->listReload();
    }
}
void ZFUIListView::listAdapterSetAutoRetain(ZF_IN ZFUIListAdapter *listAdapter)
{
    zfRetainWithoutLeakTest(listAdapter);
    this->listAdapterSet(listAdapter);
    if(listAdapter != zfnull)
    {
        this->toObject()->tagSet(_ZFP_ZFUIListView_listAdapterHolderTag, listAdapter->toObject());
    }
    zfReleaseWithoutLeakTest(listAdapter);
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIListView, ZFUIOrientationEnum, listOrientation)
{
    if(this->listOrientation() != newValue)
    {
        this->listOrientationSetInternal(newValue);
        d->listBounceUpdate();
        d->listReloadByChangeListOrientation = zftrue;
        this->listReload();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIListView, zfint, cellSizeHint)
{
    if(this->cellSizeHint() != newValue)
    {
        this->cellSizeHintSetInternal(newValue);
        this->listReload();
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIListView, zfbool, listBounce)
{
    if(this->listBounce() != newValue)
    {
        this->listBounceSetInternal(newValue);
        d->listBounceUpdate();
        if(!this->listBounce())
        {
            this->scrollBounceVerticalAlwaysSet(zffalse);
            this->scrollBounceHorizontalAlwaysSet(zffalse);
        }
    }
}

// ============================================================
ZFObject *ZFUIListView::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIListViewPrivate);
    d->pimplOwner = this;
    d->listBounceUpdate();
    return this;
}
void ZFUIListView::objectOnDealloc(void)
{
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}
void ZFUIListView::objectOnDeallocPrepare(void)
{
    d->childAddOverrideFlag = zftrue;
    this->listAdapterSet(zfnull);
    zfsuper::objectOnDeallocPrepare();
    d->childAddOverrideFlag = zffalse;
}

ZFSerializable::PropertyType ZFUIListView::serializableOnCheckPropertyType(ZF_IN const ZFProperty *property)
{
    if(property == ZFPropertyAccess(ZFUIScrollView, scrollContentFrame)
        || property == ZFPropertyAccess(ZFUIScrollView, scrollBounceVertical)
        || property == ZFPropertyAccess(ZFUIScrollView, scrollBounceHorizontal)
        || property == ZFPropertyAccess(ZFUIScrollView, scrollBounceVerticalAlways)
        || property == ZFPropertyAccess(ZFUIScrollView, scrollBounceHorizontalAlways)
        )
    {
        return ZFSerializable::PropertyTypeNotSerializable;
    }
    else
    {
        return zfsuperI(ZFSerializable)::serializableOnCheckPropertyType(property);
    }
}

void ZFUIListView::layoutOnLayoutRequest(ZF_IN ZFUIView *causedByThisView)
{
    zfsuper::layoutOnLayoutRequest(causedByThisView);
    d->cellNeedUpdate = zftrue;
}
void ZFUIListView::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    if(!d->listQuickReloadRequested && this->layoutedFrame().size != this->layoutedFramePrev().size)
    {
        d->listQuickReloadRequested = zftrue;
        d->cellNeedUpdate = zftrue;
    }
    if((d->listReloadRequested || d->listQuickReloadRequested) && d->listAdapter != zfnull)
    {
        // update list adapter's settings
        d->listAdapter->_ZFP_ZFUIListAdapter_listOrientation = this->listOrientation();
        d->listAdapter->_ZFP_ZFUIListAdapter_cellSizeHint = this->cellSizeHint();
        d->listAdapter->_ZFP_ZFUIListAdapter_listContainerSize = this->scrollArea().size;
    }
    zfsuper::layoutOnLayoutPrepare(bounds);
    d->listCheckReload();
}
void ZFUIListView::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    d->updateCellLayout();
}

void ZFUIListView::viewChildOnAdd(ZF_IN ZFUIView *child,
                                  ZF_IN ZFUIViewChildLayerEnum layer)
{
    if(layer == ZFUIViewChildLayer::e_Normal)
    {
        zfCoreAssertWithMessage(d->childAddOverrideFlag || layer != ZFUIViewChildLayer::e_Normal, zfTextA("you must not add child to a list view"));
    }
    zfsuper::viewChildOnAdd(child, layer);
}
void ZFUIListView::viewChildOnRemove(ZF_IN ZFUIView *child,
                                     ZF_IN ZFUIViewChildLayerEnum layer)
{
    if(layer == ZFUIViewChildLayer::e_Normal)
    {
        zfCoreAssertWithMessage(d->childAddOverrideFlag || layer != ZFUIViewChildLayer::e_Normal, zfTextA("you must not remove child from a list view"));
    }
    zfsuper::viewChildOnRemove(child, layer);
}

void ZFUIListView::scrollAreaMarginOnChange(void)
{
    zfsuper::scrollAreaMarginOnChange();
    if(!d->listQuickReloadRequested)
    {
        d->listQuickReloadRequested = zftrue;
        this->layoutRequest();
    }
}
void ZFUIListView::scrollContentFrameOnChange(void)
{
    zfsuper::scrollContentFrameOnChange();
    if(d->scrollContentFrameOverrideFlag)
    {
        return ;
    }

    d->listCheckReload();

    if(d->listAdapter == zfnull)
    {
        return ;
    }

    if(!d->listVisibleCell.isEmpty() && !d->listVisibleCellOffsetNeedUpdate)
    {
        zfindex cellIndex = d->listVisibleCellIndexRange.start + d->listVisibleCellIndexRange.count - 1;
        ZFUIRect cellFrame = d->listVisibleCellFrame(cellIndex);
        d->updateHeadCellBeforeIndex(cellIndex, cellFrame);
        if(!d->listVisibleCell.isEmpty())
        {
            cellIndex = d->listVisibleCellIndexRange.start;
            cellFrame = d->listVisibleCellFrame(cellIndex);
            d->updateTailCellAfterIndex(cellIndex, cellFrame);
        }
        else
        {
            d->updateFromFirstCell();
        }
    }
    else
    {
        d->updateFromFirstCell();
    }

    if(d->cellNeedUpdate)
    {
        this->listVisibleCellOnChange();
    }
}
void ZFUIListView::scrollOnScrolledByUser(void)
{
    zfsuper::scrollOnScrolledByUser();

    // cancel scrollListCellToHead/Tail task
    d->scrollListCellIndex = zfindexMax;
}
void ZFUIListView::scrollOnScrollEnd(void)
{
    zfsuper::scrollOnScrollEnd();

    // cancel scrollListCellToHead/Tail task
    d->scrollListCellIndex = zfindexMax;
}

// ============================================================
void ZFUIListView::listReload(void)
{
    if(!d->listReloadRequested)
    {
        d->listReloadRequested = zftrue;
        d->listQuickReloadRequested = zftrue;
        d->cellNeedUpdate = zftrue;
        d->listVisibleCellOffsetNeedUpdate = zftrue;
        this->layoutRequest();
    }
}
zfbool ZFUIListView::listReloadRequested(void)
{
    return d->listReloadRequested;
}
void ZFUIListView::listReloadCellAtIndex(ZF_IN zfindex index)
{
    if(d->listReloadRequested || !zfindexRangeContain(d->listVisibleCellIndexRange, index))
    {
        return ;
    }

    d->cellNeedUpdate = zftrue;

    zfindex indexOfVisibleCell = index - d->listVisibleCellIndexRange.start;
    ZFUIListCell *cellOld = d->listVisibleCell[indexOfVisibleCell];
    ZFUIRect cellOldFrame = d->listVisibleCellFrame(index);

    ZFUIListCell *cellNew = zfRetain(d->cellLoadAtIndex(index).to<ZFUIListCell *>());
    d->listVisibleCell[indexOfVisibleCell] = cellNew;

    this->childReplaceAtIndex(indexOfVisibleCell, cellNew);

    d->cellOnRecycle(cellOld);
    this->cellOnDetach(cellOld);
    this->cellOnAttach(cellNew);
    d->cellOnUpdate(index, cellNew);

    // update cell size at index
    zfint cellNewSize = d->cellSizeAtIndex(index, cellNew);
    d->cellSizeList[index] = cellNewSize;

    // update all cells after the reloaded one
    switch(this->listOrientation())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            cellOldFrame.size.width = cellNewSize;
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            cellOldFrame.size.height = cellNewSize;
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
    d->updateTailCellAfterIndex(index, cellOldFrame);

    // finally notify visible cell changed
    this->listVisibleCellOnChange();

    zfRelease(cellOld);
}

ZFCoreArrayPOD<ZFUIListCell *> ZFUIListView::listVisibleCell(void)
{
    return d->listVisibleCell;
}
const zfindexRange &ZFUIListView::listVisibleCellIndexRange(void)
{
    return d->listVisibleCellIndexRange;
}

void ZFUIListView::scrollListCellToHead(ZF_IN zfindex cellIndex,
                                        ZF_IN_OPT zfint offset /* = 0 */,
                                        ZF_IN_OPT zfbool animated /* = zftrue */)
{
    if(cellIndex >= d->cellCount)
    {
        d->scrollListCellIndex = zfindexMax;
    }
    else
    {
        d->scrollListCellIndex = cellIndex;
    }
    d->scrollListCellOffset = offset;
    d->scrollListCellAnimated = animated;
    d->scrollListCellToHead = zftrue;
    d->scrollListCellDesiredPosSaved = 30000;
    d->scrollListCellCheckUpdate();
}
void ZFUIListView::scrollListCellToTail(ZF_IN zfindex cellIndex,
                                        ZF_IN_OPT zfint offset /* = 0 */,
                                        ZF_IN_OPT zfbool animated /* = zftrue */)
{
    if(cellIndex >= d->cellCount)
    {
        d->scrollListCellIndex = zfindexMax;
    }
    else
    {
        d->scrollListCellIndex = cellIndex;
    }
    d->scrollListCellOffset = offset;
    d->scrollListCellAnimated = animated;
    d->scrollListCellToHead = zffalse;
    d->scrollListCellDesiredPosSaved = 30000;
    d->scrollListCellCheckUpdate();
}

void ZFUIListView::listVisibleCellOnChange(void)
{
    this->observerNotify(ZFUIListView::EventListVisibleCellOnChange());

    if(!this->scrollOverride())
    {
        d->scrollListCellCheckUpdate();
    }
}

ZF_NAMESPACE_GLOBAL_END

