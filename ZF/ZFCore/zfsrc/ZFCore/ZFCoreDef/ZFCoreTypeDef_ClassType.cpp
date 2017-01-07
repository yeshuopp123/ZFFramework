/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreTypeDef_ClassType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void *zfmallocZero(zfindex size)
{
    void *ret = zfmalloc(size);
    if(ret)
    {
        zfmemset(ret, 0, size);
    }
    return ret;
}
void *zfreallocZero(void *oldPtr, zfindex newSize, zfindex oldSize)
{
    void *ret = zfrealloc(oldPtr, newSize);
    if(ret && newSize > oldSize)
    {
        zfmemset(((zfbyte *)ret) + oldSize, 0, newSize - oldSize);
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

