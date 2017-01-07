/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreMutexDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static void _ZFP_ZFCoreMutexImpl_available_lock(void)
{
    _ZFP_ZFCoreMutexImplObject->mutexImplLock();
}
static void _ZFP_ZFCoreMutexImpl_available_unlock(void)
{
    _ZFP_ZFCoreMutexImplObject->mutexImplUnlock();
}
static void _ZFP_ZFCoreMutexImpl_notavailable_lock(void)
{
}
static void _ZFP_ZFCoreMutexImpl_notavailable_unlock(void)
{
}

ZFCoreMutexImpl *_ZFP_ZFCoreMutexImplObject = zfnull;
void ZFCoreMutexImplSet(ZF_IN ZFCoreMutexImpl *impl)
{
    _ZFP_ZFCoreMutexImplObject = impl;
    if(_ZFP_ZFCoreMutexImplObject != zfnull)
    {
        _ZFP_ZFCoreMutexImpl_lock = _ZFP_ZFCoreMutexImpl_available_lock;
        _ZFP_ZFCoreMutexImpl_unlock = _ZFP_ZFCoreMutexImpl_available_unlock;
    }
    else
    {
        _ZFP_ZFCoreMutexImpl_lock = _ZFP_ZFCoreMutexImpl_notavailable_lock;
        _ZFP_ZFCoreMutexImpl_unlock = _ZFP_ZFCoreMutexImpl_notavailable_unlock;
    }
}
ZFCoreMutexImpl *ZFCoreMutexImplGet(void)
{
    return _ZFP_ZFCoreMutexImplObject;
}

ZFFuncAddrType _ZFP_ZFCoreMutexImpl_lock = _ZFP_ZFCoreMutexImpl_notavailable_lock;
ZFFuncAddrType _ZFP_ZFCoreMutexImpl_unlock = _ZFP_ZFCoreMutexImpl_notavailable_unlock;

ZF_NAMESPACE_GLOBAL_END

