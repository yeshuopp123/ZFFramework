/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIListAdapterArray.h
 * @brief basic list adapter which hold all list cells directly (with no recycle logic)
 */

#ifndef _ZFI_ZFUIListAdapterArray_h_
#define _ZFI_ZFUIListAdapterArray_h_

#include "ZFUIListAdapter.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief basic list adapter which hold all list cells directly (with no recycle logic)
 */
zfclass ZF_ENV_EXPORT ZFUIListAdapterArray : zfextends ZFObject, zfimplements ZFUIListAdapter
{
    ZFOBJECT_DECLARE(ZFUIListAdapterArray, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIListAdapter)

public:
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        d = zfAllocWithoutLeakTest(ZFArrayEditable);
        return this;
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfReleaseWithoutLeakTest(d);
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        this->cellRemoveAll();
        zfsuper::objectOnDeallocPrepare();
    }

public:
    /**
     * @brief directly access internal cell array,
     *   you may modify it directly, with caution
     */
    virtual inline ZFArrayEditable *cellArray(void)
    {
        return d;
    }
    /**
     * @brief add cell
     */
    virtual inline void cellAdd(ZF_IN ZFUIListCell *cell)
    {
        d->add(cell);
    }
    /**
     * @brief add cell at index
     */
    virtual inline void cellAdd(ZF_IN zfindex index,
                                ZF_IN ZFUIListCell *cell)
    {
        d->add(index, cell);
    }
    /**
     * @brief remove cell
     */
    virtual inline void cellRemove(ZF_IN zfindex index,
                                   ZF_IN_OPT zfindex count = 1)
    {
        d->remove(index, count);
    }
    /**
     * @brief remove cell
     */
    virtual inline void cellRemoveElement(ZF_IN ZFUIListCell *cell)
    {
        d->removeElement(cell);
    }
    /**
     * @brief remove all cell
     */
    virtual inline void cellRemoveAll(void)
    {
        d->removeAll();
    }

public:
    /**
     * @brief whether to override default list cell size hint, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, cellSizeHintOverride)
    /**
     * @brief valid only if #cellSizeHintOverride, -1 by default, see #ZFUIListAdapter::cellSizeAtIndex
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, cellSizeHint, ZFPropertyInitValue(-1))

public:
    zfoverride
    virtual zfindex cellCount(void)
    {
        return d->count();
    }
    zfoverride
    virtual zfautoObject cellCacheOnAccess(ZF_IN zfindex index)
    {
        return zfautoObjectCreate(d->get(index));
    }
    zfoverride
    virtual zfautoObject cellAtIndex(ZF_IN zfindex index)
    {
        return zfautoObjectCreate(d->get(index));
    }
    zfoverride
    virtual zfint cellSizeAtIndex(ZF_IN zfindex index,
                                  ZF_IN ZFUIListCell *cell)
    {
        if(this->cellSizeHintOverride())
        {
            return this->cellSizeHint();
        }
        else
        {
            return zfsuperI(ZFUIListAdapter)::cellSizeAtIndex(index, cell);
        }
    }

private:
    ZFArrayEditable *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListAdapterArray_h_

