/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectCache.h
 * @brief util object to hold #ZFObject as cache
 */

#ifndef _ZFI_ZFObjectCache_h_
#define _ZFI_ZFObjectCache_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFObjectCachePrivate;
/**
 * @brief util object to hold #ZFObject as cache
 */
zfclass ZF_ENV_EXPORT ZFObjectCache : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFObjectCache, ZFObject)

    /**
     * @brief max cache size, 100 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, cacheMaxSize, ZFPropertyInitValue(100))

    /**
     * @brief whether invoke #cacheTrim when receive #ZFGlobalEvent::EventAppOnReceiveMemoryWarning, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, cacheTrimWhenReceiveMemoryWarning, ZFPropertyInitValue(zftrue))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfbool, cacheTrimWhenReceiveMemoryWarning);

    /**
     * @brief leave how many cache alive while #cacheTrim, 3 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, cacheTrimThreshold, ZFPropertyInitValue(3))

public:
    /**
     * @brief add cache, replace and renew to cache queue head if exist
     *
     * newly cached object would be added to queue's head,
     * and tail's cache would be removed if exceeds #cacheMaxSize
     */
    virtual void cacheSave(ZF_IN const zfchar *cacheKey,
                           ZF_IN ZFObject *cacheValue);
    /**
     * @brief access cache, or return #zfautoObjectNull if not exist
     */
    virtual zfautoObject cacheAccess(ZF_IN const zfchar *cacheKey);

    /**
     * @brief remove cache or do nothing if not exist, return removed cache
     */
    virtual zfautoObject cacheRemove(ZF_IN const zfchar *cacheKey);
    /**
     * @brief remove all cache
     */
    virtual void cacheRemoveAll(void);
    /**
     * @brief trim the cache to reduce memory
     *
     * by default, this method would call #cacheTrimBySize
     */
    virtual void cacheTrim(void);
    /**
     * @brief util method to trim the cache
     *   so that the cached data won't exceeds specified size
     */
    virtual void cacheTrimBySize(ZF_IN zfindex size);

public:
    /**
     * @brief access all cache data, used for debug only
     *
     * ensured ordered from new to old
     */
    virtual void allCache(ZF_IN_OUT ZFCoreArray<zfstring> &cacheKeys,
                          ZF_IN_OUT ZFCoreArray<zfautoObject> &cacheValues);

protected:
    /**
     * @brief called to check whether the cache is valid
     */
    virtual inline zfbool cacheOnCheckValid(ZF_IN const zfchar *cacheKey,
                                            ZF_IN ZFObject *cacheValue)
    {
        return zftrue;
    }
    /**
     * @brief called when cache would be replaced
     */
    virtual inline void cacheOnReplace(ZF_IN const zfchar *cacheKey,
                                       ZF_IN ZFObject *cacheValue,
                                       ZF_IN ZFObject *cacheValueNew)
    {
    }
    /**
     * @brief called when cache would be added
     */
    virtual inline void cacheOnAdd(ZF_IN const zfchar *cacheKey,
                                   ZF_IN ZFObject *cacheValue)
    {
    }
    /**
     * @brief called when cache would be removed
     */
    virtual inline void cacheOnRemove(ZF_IN const zfchar *cacheKey,
                                      ZF_IN ZFObject *cacheValue)
    {
    }

protected:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

private:
    _ZFP_ZFObjectCachePrivate *d;
    friend zfclassFwd _ZFP_ZFObjectCachePrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectCache_h_

