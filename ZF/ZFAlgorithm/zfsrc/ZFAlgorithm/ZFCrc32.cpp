/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCrc32.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFCrc32 initializer
static zfbool _ZFP_ZFCrc32_prepareCrc32Table(ZF_IN_OUT zfuint32 *table)
{
    zfuint32 t = 0;
    for(zfuint i = 0; i < 256; ++i)
    {
        t = i;
        for(zfuint j = 0; j < 8; ++j)
        {
            if(t & 1)
                t = (t >> 1) ^ 0xEDB88320L;
            else
                t >>= 1;
        }
        table[i] = t;
    }
    return zftrue;
}
static zfuint32 *_ZFP_ZFCrc32TableRef(void)
{
    static zfuint32 d[256] = {0};
    static zfbool dummy = _ZFP_ZFCrc32_prepareCrc32Table(d);
    (void)dummy;
    return d;
}
#define _ZFP_ZFCrc32Table (_ZFP_ZFCrc32TableRef())

// ============================================================
// ZFCrc32
zfuint32 zfCrc32Calc(ZF_IN const void *src,
                     ZF_IN zfindex len,
                     ZF_IN_OPT zfuint32 prevResult /* = ZFCrc32InitValue */)
{
    if(src == zfnull) {return ZFCrc32Invalid;}
    const zfbyte *p = (const zfbyte *)src;
    prevResult ^= 0xFFFFFFFFL;
    for(const zfbyte *pEnd = p + len; p != pEnd; ++p)
    {
        prevResult = _ZFP_ZFCrc32Table[(prevResult ^ (*p)) & 0xFF] ^ (prevResult >> 8);
    }
    prevResult ^= 0xFFFFFFFFL;
    return prevResult;
}
zfuint32 zfCrc32Calc(ZF_IN const ZFInputCallback &callback,
                     ZF_IN_OPT zfuint32 prevResult /* = ZFCrc32InitValue */)
{
    if(!callback.callbackIsValid()) {return ZFCrc32Invalid;}

    zfbyte buf[1024] = {0};
    zfindex readCount = 0;
    prevResult ^= 0xFFFFFFFFL;
    while((readCount = callback.execute(buf, 1024)) > 0)
    {
        const zfbyte *p = buf;
        const zfbyte *pEnd = buf + readCount;
        for(; p != pEnd; ++p)
        {
            prevResult = _ZFP_ZFCrc32Table[(prevResult ^ (*p)) & 0xFF] ^ (prevResult >> 8);
        }
    }
    prevResult ^= 0xFFFFFFFFL;
    return prevResult;
}

ZF_NAMESPACE_GLOBAL_END

