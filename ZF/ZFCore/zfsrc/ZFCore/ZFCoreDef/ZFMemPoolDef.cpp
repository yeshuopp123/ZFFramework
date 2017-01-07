/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFMemPoolDef.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_deque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_zfpoolObjectPrivate
{
public:
    void objectSizeSet(ZF_IN zfindex objectSize)
    {
        this->objectSize = objectSize;
        this->objectPerBuffer = zfmMin<zfindex>(8, 4096 / this->objectSize);
    }

public:
    zfindex objectSize;
    zfindex objectPerBuffer;
    ZFCoreArrayPOD<void *> buffers;
    ZFCoreQueuePOD<void *> available;

public:
    ~_ZFP_zfpoolObjectPrivate(void)
    {
        for(zfindex i = 0; i < this->buffers.count(); ++i)
        {
            zffree(this->buffers[i]);
        }
    }
};

typedef zfstlmap<zfindex, ZFCorePointerForObject<_ZFP_zfpoolObject *> > _ZFP_zfpoolObjectMapType;
ZF_STATIC_INITIALIZER_INIT(zfpoolPoolHolder)
{
}
_ZFP_zfpoolObjectMapType pools;
ZF_STATIC_INITIALIZER_END(zfpoolPoolHolder)

void *_ZFP_zfpoolObject::poolMalloc(void)
{
    if(d->available.isEmpty())
    {
        zfindex bufferSize = d->objectSize * d->objectPerBuffer;
        void *buffer = zfmalloc(bufferSize);
        zfmemset(buffer, 0, bufferSize);
        d->buffers.add(buffer);

        zfbyte *p = (zfbyte *)buffer;
        for(zfindex i = 0; i < d->objectPerBuffer; ++i, p += d->objectSize)
        {
            d->available.queuePut(p);
        }
    }
    return d->available.queueTake();
}
void _ZFP_zfpoolObject::poolFree(ZF_IN void *obj)
{
    d->available.queuePut(obj);
}
_ZFP_zfpoolObject::_ZFP_zfpoolObject(void)
{
    d = zfnew(_ZFP_zfpoolObjectPrivate);
}
_ZFP_zfpoolObject::~_ZFP_zfpoolObject(void)
{
    zfdelete(d);
}

_ZFP_zfpoolObject *_ZFP_zfpoolObjectAccess(ZF_IN zfindex objectSize)
{
    // align to size
    const zfindex alignSize = (sizeof(void *) * 2);
    if((objectSize % alignSize) == 0)
    {
        objectSize = (objectSize / alignSize) * alignSize;
    }
    else
    {
        objectSize = (objectSize / alignSize + 1) * alignSize;
    }

    ZFCoreMutexLocker();
    _ZFP_zfpoolObjectMapType &pools = ZF_STATIC_INITIALIZER_INSTANCE(zfpoolPoolHolder)->pools;
    _ZFP_zfpoolObjectMapType::iterator it = pools.find(objectSize);
    if(it != pools.end())
    {
        return it->second.pointerValueGet();
    }
    _ZFP_zfpoolObject *pool = zfnew(_ZFP_zfpoolObject);
    pool->d->objectSizeSet(objectSize);
    pools[objectSize] = ZFCorePointerForObject<_ZFP_zfpoolObject *>(pool);
    return pool;
}

ZF_NAMESPACE_GLOBAL_END

