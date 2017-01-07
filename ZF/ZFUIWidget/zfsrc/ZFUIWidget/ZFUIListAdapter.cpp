/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIListAdapter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUIListAdapter)

ZFOBSERVER_EVENT_REGISTER(ZFUIListAdapter, ListCellOnUpdate)
ZFOBSERVER_EVENT_REGISTER(ZFUIListAdapter, ListCellCacheOnRecycle)

#define _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key) \
    zfchar *cacheKey = zfsConnect(zfText("_ZFP_ZFUIListAdapter_cacheKey"), key); \
    zfblockedFree(cacheKey)
zfautoObject ZFUIListAdapter::cellCacheDefaultAccess(ZF_IN const zfchar *key)
{
    _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key);
    ZFArrayEditable *cacheList = this->toObject()->tagGet<ZFArrayEditable *>(cacheKey);
    if(cacheList != zfnull && !cacheList->isEmpty())
    {
        zfautoObject ret = zfautoObjectCreate(cacheList->getLast());
        cacheList->removeLast();
        return ret;
    }
    else
    {
        return zfautoObjectNull;
    }
}
void ZFUIListAdapter::cellCacheDefaultRecycle(ZF_IN const zfchar *key, ZF_IN ZFUIListCell *cell)
{
    _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key);
    ZFArrayEditable *cacheList = this->toObject()->tagGet<ZFArrayEditable *>(cacheKey);
    if(cacheList == zfnull)
    {
        cacheList = zfAllocWithoutLeakTest(ZFArrayEditable);
        this->toObject()->tagSetMarkCached(cacheKey, cacheList);
        zfReleaseWithoutLeakTest(cacheList);
    }
    cacheList->add(cell);
}

ZF_NAMESPACE_GLOBAL_END

