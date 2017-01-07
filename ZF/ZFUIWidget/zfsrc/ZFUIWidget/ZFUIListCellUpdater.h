/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIListCellUpdater.h
 * @brief utility to update #ZFUIListCell
 */

#ifndef _ZFI_ZFUIListCellUpdater_h_
#define _ZFI_ZFUIListCellUpdater_h_

#include "ZFUIListCell.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief update param used by #ZFUIListCellUpdater::cellOnUpdate
 */
zfclassPOD ZF_ENV_EXPORT ZFUIListCellUpdaterParam
{
public:
    ZFUIListCell *cell; /**< @brief the cell */
    zfindex cellIndex; /**< @brief cell's index */
    zfindex cellCount; /**< @brief cell's total count */
    ZFUIOrientationEnum listOrientation; /**< @brief owner list container's orientation */
    ZFUISize listContainerSize; /**< @brief owner list container's size */
    zfint cellSizeHint; /**< @brief list cell's size hint */
};

// ============================================================
/**
 * @brief abstract list cell updater to update #ZFUIListCell
 */
zfclass ZF_ENV_EXPORT ZFUIListCellUpdater : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUIListCellUpdater, ZFStyleableObject)

public:
    /**
     * @brief called to update cell style
     */
    virtual inline void cellOnUpdate(ZF_IN const ZFUIListCellUpdaterParam &updateParam)
    {
    }
    /**
     * @brief called to recycle cell
     */
    virtual inline void cellOnRecycle(ZF_IN ZFUIListCell *cell)
    {
    }

public:
    /**
     * @brief util method for impl to achieve cache logic
     */
    zffinal zfautoObject itemCacheAccess(ZF_IN const zfchar *key);
    /** @brief see #itemCacheAccess */
    zffinal void itemCacheRecycle(ZF_IN const zfchar *key, ZF_IN ZFObject *cache);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListCellUpdater_h_

