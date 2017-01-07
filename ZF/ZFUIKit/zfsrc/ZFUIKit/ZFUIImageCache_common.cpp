/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIImageCache_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfautoObject ZFUIImageLoadFromInputWithCache(ZF_IN const ZFInputCallback &input)
{
    if(input.callbackId() == zfnull)
    {
        return ZFUIImageLoadFromInput(input);
    }

    zfstring key = zfstringWithFormat(zfText("ZFUIImageLoadFromInputWithCache:%s"), input.callbackId());
    zfautoObject cached = ZFUIImageCache::instance()->cacheAccess(key);
    if(cached != zfautoObjectNull)
    {
        return cached;
    }
    cached = ZFUIImageLoadFromInput(input);
    if(cached == zfautoObjectNull)
    {
        return zfautoObjectNull;
    }
    ZFUIImageCache::instance()->cacheSave(key, cached.toObject());
    return cached;
}

zfautoObject ZFUIImageLoadFromColorWithCache(ZF_IN const ZFUIColor &color,
                                             ZF_IN_OPT const ZFUISize &size /* = ZFUISizeZero */)
{
    zfstring key = zfstringWithFormat(zfText("ZFUIImageLoadFromInputWithCache:%s %s"), ZFUIColorToString(color).cString(), ZFUISizeToString(size).cString());
    zfautoObject cached = ZFUIImageCache::instance()->cacheAccess(key);
    if(cached != zfautoObjectNull)
    {
        return cached;
    }
    cached = ZFUIImageLoadFromColor(color, size);
    if(cached == zfautoObjectNull)
    {
        return zfautoObjectNull;
    }
    ZFUIImageCache::instance()->cacheSave(key, cached.toObject());
    return cached;
}

ZF_NAMESPACE_GLOBAL_END

