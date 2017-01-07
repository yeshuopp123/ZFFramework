/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIListCellUpdater.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIListCellUpdater)

#define _ZFP_ZFUIListCellUpdater_cacheKey(cacheKey, key) \
    zfchar *cacheKey = zfsConnect(zfText("_ZFP_ZFUIListCellUpdater_cacheKey"), key); \
    zfblockedFree(cacheKey)
zfautoObject ZFUIListCellUpdater::itemCacheAccess(ZF_IN const zfchar *key)
{
    _ZFP_ZFUIListCellUpdater_cacheKey(cacheKey, key);
    ZFArrayEditable *cacheList = this->tagGet<ZFArrayEditable *>(cacheKey);
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
void ZFUIListCellUpdater::itemCacheRecycle(ZF_IN const zfchar *key, ZF_IN ZFObject *cache)
{
    _ZFP_ZFUIListCellUpdater_cacheKey(cacheKey, key);
    ZFArrayEditable *cacheList = this->tagGet<ZFArrayEditable *>(cacheKey);
    if(cacheList == zfnull)
    {
        cacheList = zfAllocWithoutLeakTest(ZFArrayEditable);
        this->tagSetMarkCached(cacheKey, cacheList);
        zfReleaseWithoutLeakTest(cacheList);
    }
    cacheList->add(cache);
}

ZF_NAMESPACE_GLOBAL_END

