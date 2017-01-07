/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFObjectMutex.h"

#if ZF_ENV_sys_Qt
#include <QMutex>

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFObjectMutexImpl_sys_iOS_MutexImpl : zfextendsNotPOD ZFObjectMutexImpl
{
public:
    QMutex *nativeMutex;
public:
    _ZFP_ZFObjectMutexImpl_sys_iOS_MutexImpl(void)
    : ZFObjectMutexImpl()
    , nativeMutex(zfnew(QMutex, QMutex::Recursive))
    {
    }
    virtual ~_ZFP_ZFObjectMutexImpl_sys_iOS_MutexImpl(void)
    {
        zfdelete(this->nativeMutex);
    }
public:
    virtual void mutexImplLock(void)
    {
        this->nativeMutex->lock();
    }
    virtual void mutexImplUnlock(void)
    {
        this->nativeMutex->unlock();
    }
    virtual zfbool mutexImplTryLock(void)
    {
        return this->nativeMutex->tryLock();
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFObjectMutexImpl_sys_Qt, ZFObjectMutex, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt:QMutex"))
public:
    virtual ZFObjectMutexImpl *nativeMutexCreate(void)
    {
        return zfnew(_ZFP_ZFObjectMutexImpl_sys_iOS_MutexImpl);
    }
    virtual void nativeMutexDestroy(ZF_IN ZFObjectMutexImpl *nativeMutex)
    {
        zfdelete(nativeMutex);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFObjectMutexImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFObjectMutexImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Qt

