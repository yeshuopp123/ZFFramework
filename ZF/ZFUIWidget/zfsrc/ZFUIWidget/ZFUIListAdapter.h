/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIListAdapter.h
 * @brief abstract list adpater to supply cells for list view
 */

#ifndef _ZFI_ZFUIListAdapter_h_
#define _ZFI_ZFUIListAdapter_h_

#include "ZFUIListCell.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFUIListViewPrivate;
zfclassFwd ZFUIListView;
/**
 * @brief abstract list adpater to supply cells for list view
 */
zfinterface ZF_ENV_EXPORT ZFUIListAdapter : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIListAdapter, ZFInterface)

    // ============================================================
    // events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when update list cell,
     * param0 is the #ZFUIListCell,
     * param1 is the index of the list cell (as #ZFValue::indexValue)
     */
    ZFOBSERVER_EVENT(ListCellOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when recycle list cell,
     * param0 is the list cell
     */
    ZFOBSERVER_EVENT(ListCellCacheOnRecycle)

    // ============================================================
    // owner state
private:
    ZFUIOrientationEnum _ZFP_ZFUIListAdapter_listOrientation;
public:
    /**
     * @brief list's orientation
     */
    zffinal inline ZFUIOrientationEnum listOrientation(void)
    {
        return this->_ZFP_ZFUIListAdapter_listOrientation;
    }

private:
    ZFUISize _ZFP_ZFUIListAdapter_listContainerSize;
public:
    /**
     * @brief list container's size
     */
    zffinal inline const ZFUISize &listContainerSize(void)
    {
        return this->_ZFP_ZFUIListAdapter_listContainerSize;
    }

private:
    zfint _ZFP_ZFUIListAdapter_cellSizeHint;
public:
    /**
     * @brief list cell's size hint
     */
    zffinal inline zfint cellSizeHint(void)
    {
        return this->_ZFP_ZFUIListAdapter_cellSizeHint;
    }

    // ============================================================
    // basic list cell access
public:
    /**
     * @brief cell count
     */
    virtual zfindex cellCount(void) = 0;
    /**
     * @brief access list cell at index, assert fail if return null object
     */
    virtual zfautoObject cellAtIndex(ZF_IN zfindex index) = 0;
    /**
     * @brief check cell size at index
     *
     * for performance, this method is ensured to be called for visible cells only,
     * to check the cell's actual size\n
     * while cell is not visible,
     * a predicted hint size (or previous size) would be used
     * util it's coming to visible\n
     * return a -1 size to measure the cell automatically,
     * otherwise, the cell's size is fixed\n
     * return #cellSizeHint by default
     */
    virtual inline zfint cellSizeAtIndex(ZF_IN zfindex index,
                                         ZF_IN ZFUIListCell *cell)
    {
        return this->cellSizeHint();
    }

    // ============================================================
    // list cell update callback
protected:
    /**
     * @brief see #EventListCellOnUpdate
     */
    virtual inline void cellOnUpdate(ZF_IN zfindex atIndex,
                                     ZF_IN ZFUIListCell *cell)
    {
        this->toObject()->observerNotify(zfself::EventListCellOnUpdate(), cell, ZFValue::indexValueCreate(atIndex).toObject());
    }

    // ============================================================
    // cache logic
protected:
    /**
     * @brief try access cached list cell, return null means no cache available,
     *   by default, no cache logic support
     *
     * see #cellCacheOnRecycle for more info
     */
    virtual zfautoObject cellCacheOnAccess(ZF_IN zfindex index)
    {
        return zfautoObjectNull;
    }
    zffinal inline void _ZFP_ZFUIListAdapter_cellCacheOnRecycle(ZF_IN ZFUIListCell *cell)
    {
        this->cellCacheOnRecycle(cell);
        this->toObject()->observerNotify(zfself::EventListCellCacheOnRecycle(), cell);
    }
    /**
     * @brief recycle cell or do nothing if you don't need recycle logic
     *
     * to achieve recycle logic, you should:
     * -# override #cellCacheOnRecycle and store the cell manually
     * -# during #cellCacheOnAccess, return your cached cell if exist
     *
     * by default, we have #cellCacheDefaultAccess and #cellCacheDefaultRecycle
     * that would suit for most case
     */
    virtual inline void cellCacheOnRecycle(ZF_IN ZFUIListCell *cell)
    {
    }
    /** @brief see #cellCacheOnRecycle */
    zffinal zfautoObject cellCacheDefaultAccess(ZF_IN const zfchar *key);
    /** @brief see #cellCacheOnRecycle */
    zffinal void cellCacheDefaultRecycle(ZF_IN const zfchar *key, ZF_IN ZFUIListCell *cell);

private:
    friend zfclassFwd _ZFP_ZFUIListViewPrivate;
    friend zfclassFwd ZFUIListView;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListAdapter_h_

