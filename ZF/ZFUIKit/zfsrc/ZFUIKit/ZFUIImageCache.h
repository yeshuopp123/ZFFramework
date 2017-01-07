/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIImageCache.h
 * @brief image load with cache logic
 */

#ifndef _ZFI_ZFUIImageCache_h_
#define _ZFI_ZFUIImageCache_h_

#include "ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief cache for #ZFUIImage, see #ZFObjectCache
 */
zfclass ZF_ENV_EXPORT ZFUIImageCache : zfextends ZFObjectCache
{
    ZFOBJECT_DECLARE(ZFUIImageCache, ZFObjectCache)

    ZFOBJECT_SINGLETON_DECLARE(ZFUIImageCache, instance)

    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN_NO_AUTO_INIT(zfindex, cacheMaxSize, 50)

    /**
     * @brief max image size to cache, 0 means no limit, (320, 320) by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISize, cacheMaxImageSize, ZFPropertyInitValue(ZFUISizeMake(320)))

protected:
    zfoverride
    virtual zfbool cacheOnCheckValid(ZF_IN const zfchar *cacheKey,
                                     ZF_IN ZFObject *cacheValue)
    {
        if(!zfsuper::cacheOnCheckValid(cacheKey, cacheValue))
        {
            return zffalse;
        }
        ZFUIImage *image = ZFCastZFObjectUnchecked(ZFUIImage *, cacheValue);
        if(image == zfnull)
        {
            return zffalse;
        }
        const ZFUISize &maxImageSize = this->cacheMaxImageSize();
        if((maxImageSize.width > 0 && image->imageSize().width > maxImageSize.width)
            || (maxImageSize.height > 0 && image->imageSize().height > maxImageSize.height))
        {
            return zffalse;
        }
        return zftrue;
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageCache_h_

#include "ZFUIImageCache_common.h"

