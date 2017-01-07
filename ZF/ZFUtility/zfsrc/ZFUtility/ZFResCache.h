/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFResCache.h
 * @brief utility to load serializable from resource with cache logic
 */

#ifndef _ZFI_ZFResCache_h_
#define _ZFI_ZFResCache_h_

#include "ZFObjectCache.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief cache for serializable from resources
 */
zfclass ZF_ENV_EXPORT ZFResCache : zfextends ZFObjectCache
{
    ZFOBJECT_DECLARE(ZFResCache, ZFObjectCache)

    ZFOBJECT_SINGLETON_DECLARE(ZFResCache, instance)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFResCache_h_

