/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFMutex.h"
#include "protocol/ZFProtocolZFObjectMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 1
// ============================================================
// ZFCoreMutexImpl
static ZFObjectMutexImpl *_ZFP_ZFCoreMutexImpl_ZFMutex_mutexImpl = zfnull;
static ZFPROTOCOL_INTERFACE_CLASS(ZFObjectMutex) *_ZFP_ZFCoreMutexImpl_ZFMutex_impl = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFCoreMutexImpl_ZFMutex, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFCoreMutexImpl_ZFMutex_mutexImpl = zfnull;
    _ZFP_ZFCoreMutexImpl_ZFMutex_impl = ZFPROTOCOL_TRY_ACCESS(ZFObjectMutex);
    if(_ZFP_ZFCoreMutexImpl_ZFMutex_impl != zfnull)
    {
        _ZFP_ZFCoreMutexImpl_ZFMutex_mutexImpl = _ZFP_ZFCoreMutexImpl_ZFMutex_impl->nativeMutexCreate();
        ZFCoreMutexImplSet(_ZFP_ZFCoreMutexImpl_ZFMutex_mutexImpl);
    }
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFCoreMutexImpl_ZFMutex)
{
    ZFCoreMutexImplSet(zfnull);

    if(_ZFP_ZFCoreMutexImpl_ZFMutex_mutexImpl != zfnull)
    {
        _ZFP_ZFCoreMutexImpl_ZFMutex_impl->nativeMutexDestroy(_ZFP_ZFCoreMutexImpl_ZFMutex_mutexImpl);
        _ZFP_ZFCoreMutexImpl_ZFMutex_mutexImpl = zfnull;
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFCoreMutexImpl_ZFMutex)

// ============================================================
// ZFObjectMutexImpl
static zfbool _ZFP_ZFObjectMutexImplCheckCallback_ZFMutex(void)
{
    return ZFPROTOCOL_IS_AVAILABLE(ZFObjectMutex);
}
static ZFObjectMutexImpl *_ZFP_ZFObjectMutexImplInitCallback_ZFMutex(void)
{
    return ZFPROTOCOL_ACCESS(ZFObjectMutex)->nativeMutexCreate();
}
static void _ZFP_ZFObjectMutexImplCleanupCallback_ZFMutex(ZF_IN ZFObjectMutexImpl *mutexImpl)
{
    ZFPROTOCOL_ACCESS(ZFObjectMutex)->nativeMutexDestroy(mutexImpl);
}
ZF_STATIC_INITIALIZER_INIT(ZFObjectMutexImpl_ZFMutex)
{
    ZFObjectMutexImplSet(_ZFP_ZFObjectMutexImplCheckCallback_ZFMutex,
                         _ZFP_ZFObjectMutexImplInitCallback_ZFMutex,
                         _ZFP_ZFObjectMutexImplCleanupCallback_ZFMutex);
}
ZF_STATIC_INITIALIZER_DESTROY(ZFObjectMutexImpl_ZFMutex)
{
    ZFObjectMutexImplSet();
}
ZF_STATIC_INITIALIZER_END(ZFObjectMutexImpl_ZFMutex)
#endif

// ============================================================
// ZFMutex
ZFOBJECT_REGISTER(ZFMutex)
ZFObject *ZFMutex::objectOnInit(ZF_IN zfbool noWarningIfImplementationNotAvailable)
{
    this->objectOnInit();
    this->noWarningIfImplementationNotAvailable = noWarningIfImplementationNotAvailable;
    return this;
}
ZFObject *ZFMutex::objectOnInit(void)
{
    zfsuper::objectOnInit();
    this->noWarningIfImplementationNotAvailable = zffalse;
    return this;
}
void ZFMutex::objectOnDealloc(void)
{
    zfsuper::objectOnDealloc();
}

void ZFMutex::mutexLock(void)
{
    if(this->_ZFP_ZFObjectLockIsAvailable())
    {
        this->_ZFP_ZFObjectLock();
    }
    else if(!this->noWarningIfImplementationNotAvailable)
    {
        zfCoreLogTrim(zfTextA("[ZFMutex] try mutexLock a object while no implementation for lock"));
    }
}
zfbool ZFMutex::mutexTryLock(void)
{
    if(this->_ZFP_ZFObjectLockIsAvailable())
    {
        return this->_ZFP_ZFObjectTryLock();
    }
    else if(!this->noWarningIfImplementationNotAvailable)
    {
        zfCoreLogTrim(zfTextA("[ZFMutex] try mutexTryLock a object while no implementation for lock"));
    }
    return zffalse;
}
void ZFMutex::mutexUnlock(void)
{
    if(this->_ZFP_ZFObjectLockIsAvailable())
    {
        this->_ZFP_ZFObjectUnlock();
    }
    else if(!this->noWarningIfImplementationNotAvailable)
    {
        zfCoreLogTrim(zfTextA("[ZFMutex] try mutexUnlock a object while no implementation for lock"));
    }
}

ZF_NAMESPACE_GLOBAL_END

