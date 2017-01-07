/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFAutoReleasePool.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassPOD _ZFP_ZFAutoReleasePoolData
{
public:
    ZFObject *obj;
    zfbool enableLeakTest;
};

zfclassNotPOD _ZFP_ZFAutoReleasePoolPrivate
{
public:
    zfindex maxSize;
    ZFCoreArrayPOD<_ZFP_ZFAutoReleasePoolData> array;
public:
    _ZFP_ZFAutoReleasePoolPrivate(void)
    : maxSize(zfindexMax)
    , array()
    {
    }
};

ZFOBJECT_REGISTER(ZFAutoReleasePool)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFAutoReleasePool, ZFAutoReleasePool, ZFAutoReleasePool, instance, ZFLevelZFFrameworkLow)

ZFObject *ZFAutoReleasePool::objectOnInit(ZF_IN zfindex maxSize)
{
    this->objectOnInit();
    d->maxSize = maxSize;
    return this;
}
ZFObject *ZFAutoReleasePool::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAutoReleasePoolPrivate);
    return this;
}
void ZFAutoReleasePool::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFAutoReleasePool::objectOnDeallocPrepare(void)
{
    this->poolDrain();
    zfsuper::objectOnDeallocPrepare();
}
void ZFAutoReleasePool::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ret += zfText(", count: ");
    zfsFromIntT(ret, d->array.count());
}
void ZFAutoReleasePool::poolAdd(ZF_IN ZFObject *obj,
                                ZF_IN_OPT zfbool enableLeakTest /* = zftrue */)
{
    this->poolAdd(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, enableLeakTest);
}
void ZFAutoReleasePool::poolAdd(ZF_IN ZFObject *obj,
                                ZF_IN const zfcharA *callerFile,
                                ZF_IN const zfcharA *callerFunction,
                                ZF_IN zfindex callerLine,
                                ZF_IN_OPT zfbool enableLeakTest /* = zftrue */)
{
    zfCoreAssertWithMessage(obj != this, zfTextA("add autorelease pool to itself isn't allowed"));

    if(obj != zfnull)
    {
        if(d->array.count() >= d->maxSize)
        {
            zfCoreLogTrim(zfTextA("warning, auto release pool full, trying to release old object"));
            _ZFP_ZFAutoReleasePoolData data = d->array.get(0);
            if(data.enableLeakTest)
            {
                ZFLeakTestLogAutoReleaseBeforeRelease(data.obj);
            }
            d->array.remove(0);
            zfReleaseWithoutLeakTest(data.obj);
        }
        if(enableLeakTest)
        {
            ZFLeakTestLogAutoReleaseAddVerbose(obj, callerFile, callerFunction, callerLine);
        }
        _ZFP_ZFAutoReleasePoolData data;
        data.obj = obj;
        data.enableLeakTest = enableLeakTest;
        d->array.add(data);
    }
}
void ZFAutoReleasePool::poolDrain(void)
{
    if(!d->array.isEmpty())
    {
        zfindex count = d->array.count();
        _ZFP_ZFAutoReleasePoolData *tmp = (_ZFP_ZFAutoReleasePoolData *)zfmalloc(sizeof(_ZFP_ZFAutoReleasePoolData) * count);
        zfmemcpy(tmp, d->array.arrayBuf(), sizeof(_ZFP_ZFAutoReleasePoolData) * count);
        d->array.removeAll();

        for(zfindex i = 0; i < count; ++i)
        {
            if(tmp[i].enableLeakTest)
            {
                ZFLeakTestLogAutoReleaseBeforeRelease(tmp[i].obj);
            }
            zfReleaseWithoutLeakTest(tmp[i].obj);
        }
        zffree(tmp);
    }
}

ZF_NAMESPACE_GLOBAL_END

