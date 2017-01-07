/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFObjectCache.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_list.h"
#include "ZFCore/ZFSTLWrapper/zfstl_hashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
typedef zfstlhashmap<ZFObjectCache *, zfbool,
        zfpointer_zfstlHasher<ZFObjectCache *>, zfpointer_zfstlHashComparer<ZFObjectCache *>
    > _ZFP_ZFObjectCacheAtachedObjectMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObjectCacheDataHolder, ZFLevelZFFrameworkHigh)
{
    this->cacheTrimListener = ZFCallbackForRawFunction(zfself::cacheTrim);
}
public:
    ZFListener cacheTrimListener;
    _ZFP_ZFObjectCacheAtachedObjectMapType attachedObject;
public:
    static ZFLISTENER_PROTOTYPE_EXPAND(cacheTrim)
    {
        const _ZFP_ZFObjectCacheAtachedObjectMapType &attachedObject = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectCacheDataHolder)->attachedObject;
        for(_ZFP_ZFObjectCacheAtachedObjectMapType::const_iterator it = attachedObject.begin(); it != attachedObject.end(); ++it)
        {
            it->first->cacheTrim();
        }
    }
    void cacheTrimListenerSetup(ZF_IN ZFObjectCache *objectCache, ZF_IN zfbool autoTrim)
    {
        ZFCoreMutexLocker();

        if(autoTrim)
        {
            this->attachedObject[objectCache] = zftrue;
        }
        else
        {
            this->attachedObject.erase(objectCache);
        }

        if(autoTrim && this->attachedObject.size() == 1)
        {
            ZFObjectGlobalEventObserverAdd(
                ZFGlobalEvent::EventAppOnReceiveMemoryWarning(),
                this->cacheTrimListener);
        }
        else if(!autoTrim && this->attachedObject.size() == 0)
        {
            ZFObjectGlobalEventObserverRemove(
                ZFGlobalEvent::EventAppOnReceiveMemoryWarning(),
                this->cacheTrimListener);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFObjectCacheDataHolder)

// ============================================================
zfclassNotPOD _ZFP_ZFObjectCacheData
{
public:
    zfstlstringZ cacheKey;
    zfautoObject cacheValue;
    zfstllist<_ZFP_ZFObjectCacheData *>::iterator cacheListIt;
};

typedef zfstllist<_ZFP_ZFObjectCacheData *> _ZFP_ZFObjectCacheListType;
typedef zfstlhashmap<const zfchar *, _ZFP_ZFObjectCacheData *, zfcharConst_zfstlHasher, zfcharConst_zfstlHashComparer> _ZFP_ZFObjectCacheMapType;
zfclassNotPOD _ZFP_ZFObjectCachePrivate
{
public:
    _ZFP_ZFObjectCacheListType cacheList;
    _ZFP_ZFObjectCacheMapType cacheMap;

public:
    void cacheRemoveAll(ZF_IN ZFObjectCache *owner)
    {
        _ZFP_ZFObjectCacheListType list;
        list.swap(this->cacheList);
        this->cacheList.clear();
        this->cacheMap.clear();
        for(_ZFP_ZFObjectCacheListType::iterator it = list.begin(); it != list.end(); ++it)
        {
            owner->cacheOnRemove((*it)->cacheKey.c_str(), (*it)->cacheValue.toObject());
            zfdelete(*it);
        }
    }
};

// ============================================================
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFObjectCache, zfbool, cacheTrimWhenReceiveMemoryWarning)
{
    this->cacheTrimWhenReceiveMemoryWarningSetInternal(newValue);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectCacheDataHolder)->cacheTrimListenerSetup(this, this->cacheTrimWhenReceiveMemoryWarning());
}

void ZFObjectCache::cacheSave(ZF_IN const zfchar *cacheKey,
                              ZF_IN ZFObject *cacheValue)
{
    zfsynchronizedObject(this);
    if(cacheValue == zfnull)
    {
        this->cacheRemove(cacheKey);
        return ;
    }
    if(cacheKey == zfnull)
    {
        return ;
    }

    _ZFP_ZFObjectCacheMapType::iterator it = d->cacheMap.find(cacheKey);
    if(it == d->cacheMap.end())
    {
        _ZFP_ZFObjectCacheData *cacheData = zfnew(_ZFP_ZFObjectCacheData);
        cacheData->cacheKey = cacheKey;
        cacheData->cacheValue = zfautoObjectCreateMarkCached(cacheValue);

        d->cacheList.push_front(cacheData);
        cacheData->cacheListIt = d->cacheList.begin();

        d->cacheMap[cacheData->cacheKey.c_str()] = cacheData;

        this->cacheOnAdd(cacheData->cacheKey.c_str(), cacheValue);
    }
    else
    {
        _ZFP_ZFObjectCacheData *cacheData = it->second;
        zfautoObject old = cacheData->cacheValue;
        ZFUNUSED(old);
        cacheData->cacheValue = zfautoObjectCreateMarkCached(cacheValue);
        d->cacheList.erase(cacheData->cacheListIt);
        d->cacheList.push_front(cacheData);
        cacheData->cacheListIt = d->cacheList.begin();

        this->cacheOnReplace(cacheData->cacheKey.c_str(), old.toObject(), cacheValue);
    }

    while((zfindex)d->cacheList.size() > this->cacheMaxSize())
    {
        _ZFP_ZFObjectCacheData *cacheData = *(d->cacheList.rbegin());
        d->cacheList.erase(cacheData->cacheListIt);
        d->cacheMap.erase(cacheData->cacheKey.c_str());
        this->cacheOnRemove(cacheData->cacheKey.c_str(), cacheData->cacheValue.toObject());
        zfdelete(cacheData);
    }
}
zfautoObject ZFObjectCache::cacheAccess(ZF_IN const zfchar *cacheKey)
{
    zfsynchronizedObject(this);
    if(cacheKey != zfnull)
    {
        _ZFP_ZFObjectCacheMapType::iterator it = d->cacheMap.find(cacheKey);
        if(it != d->cacheMap.end())
        {
            return it->second->cacheValue;
        }
    }
    return zfautoObjectNull;
}

zfautoObject ZFObjectCache::cacheRemove(ZF_IN const zfchar *cacheKey)
{
    zfsynchronizedObject(this);
    if(cacheKey != zfnull)
    {
        _ZFP_ZFObjectCacheMapType::iterator it = d->cacheMap.find(cacheKey);
        if(it != d->cacheMap.end())
        {
            _ZFP_ZFObjectCacheData *cacheData = it->second;
            zfautoObject ret = cacheData->cacheValue;
            d->cacheList.erase(cacheData->cacheListIt);
            d->cacheMap.erase(cacheData->cacheKey.c_str());
            this->cacheOnRemove(cacheData->cacheKey.c_str(), cacheData->cacheValue.toObject());
            return ret;
        }
    }
    return zfautoObjectNull;
}
void ZFObjectCache::cacheRemoveAll(void)
{
    zfsynchronizedObject(this);
    d->cacheRemoveAll(this);
}

void ZFObjectCache::cacheTrim(void)
{
    this->cacheTrimBySize(this->cacheTrimThreshold());
}

void ZFObjectCache::cacheTrimBySize(ZF_IN zfindex size)
{
    zfsynchronizedObject(this);
    if(size >= (zfindex)(d->cacheList.size()))
    {
        return ;
    }
    _ZFP_ZFObjectCacheListType::iterator it = d->cacheList.begin();
    for(zfindex i = 0; i < size; ++i, ++it)
    {
        // nothing to do
    }
    ZFCoreArray<zfstlstringZ> tmp;
    tmp.capacitySet((zfindex)(d->cacheList.size()) - size);
    for( ; it != d->cacheList.end(); ++it)
    {
        tmp.add((*it)->cacheKey);
    }
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        this->cacheRemove(tmp[i].c_str());
    }
}

void ZFObjectCache::allCache(ZF_IN_OUT ZFCoreArray<zfstring> &cacheKeys,
                             ZF_IN_OUT ZFCoreArray<zfautoObject> &cacheValues)
{
    zfsynchronizedObject(this);
    for(_ZFP_ZFObjectCacheListType::iterator it = d->cacheList.begin(); it != d->cacheList.end(); ++it)
    {
        cacheKeys.add((*it)->cacheKey.c_str());
        cacheValues.add((*it)->cacheValue);
    }
}
ZFObject *ZFObjectCache::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFObjectCachePrivate);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectCacheDataHolder)->cacheTrimListenerSetup(this, this->cacheTrimWhenReceiveMemoryWarning());
    return this;
}
void ZFObjectCache::objectOnDealloc(void)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectCacheDataHolder)->cacheTrimListenerSetup(this, zffalse);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}
void ZFObjectCache::objectOnDeallocPrepare(void)
{
    d->cacheRemoveAll(this);
    zfsuper::objectOnDeallocPrepare();
}

ZF_NAMESPACE_GLOBAL_END

