/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFObjectMutex.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
    #include <pthread.h>
    #include <sys/syscall.h>
    #include <sys/types.h>
#endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFObjectMutexImpl_default_MutexImpl : zfextendsNotPOD ZFObjectMutexImpl
{
public:
    _ZFP_ZFObjectMutexImpl_default_MutexImpl(void)
    : ZFObjectMutexImpl()
    {
        this->init();
        this->runningThreadId = this->INVALID_THREAD_ID;
    }
    virtual ~_ZFP_ZFObjectMutexImpl_default_MutexImpl(void)
    {
        this->destroy();
    }
public:
    virtual void mutexImplLock(void)
    {
        this->paramLock();
        _ZFP_ZFObjectMutexImpl_default_ThreadIdType curThreadId = this->currentThreadId();
        if(this->runningThreadId == this->INVALID_THREAD_ID)
        {
            this->lockedThreadCountMap[curThreadId] = 1;
            this->paramUnlock();
            // first time to lock the object
            this->doLock();

            // now we owned the lock
            this->runningThreadId = curThreadId;
        }
        else
        {
            _ZFP_ZFObjectMutexImpl_default_LockedThreadCountMapType::iterator it = this->lockedThreadCountMap.find(curThreadId);
            if(curThreadId == this->runningThreadId)
            {
                ++(it->second);
                this->paramUnlock();
                // lock the same thread more than one time, no need to lock again
            }
            else
            {
                if(it == this->lockedThreadCountMap.end())
                {
                    this->lockedThreadCountMap[curThreadId] = 1;
                }
                else
                {
                    ++(it->second);
                }
                this->paramUnlock();
                // lock while another thread acquiring the lock,
                // should be locked here until another thread release the lock
                this->doLock();

                // now we owned the lock
                this->runningThreadId = curThreadId;
            }
        }
    }
    virtual void mutexImplUnlock(void)
    {
        this->paramLock();
        _ZFP_ZFObjectMutexImpl_default_ThreadIdType curThreadId = this->currentThreadId();
        _ZFP_ZFObjectMutexImpl_default_LockedThreadCountMapType::iterator it = this->lockedThreadCountMap.find(curThreadId);
        if(curThreadId == this->runningThreadId)
        {
            --(it->second);
            if(it->second == 0)
            {
                this->lockedThreadCountMap.erase(it);
                this->runningThreadId = this->INVALID_THREAD_ID;
                this->paramUnlock();
                // all locks locked by current thread are relesed, unlock
                this->doUnlock();
            }
            else
            {
                this->paramUnlock();
                // current thread still hold some locks, just return
            }
        }
        else
        {
            this->paramUnlock();

            // current thread doesn't lock, it's a error state
            zfCoreCriticalShouldNotGoHere();
            return ;
        }
    }
    virtual zfbool mutexImplTryLock(void)
    {
        this->paramLock();
        _ZFP_ZFObjectMutexImpl_default_ThreadIdType curThreadId = this->currentThreadId();
        if(this->runningThreadId == this->INVALID_THREAD_ID)
        {
            this->runningThreadId = curThreadId;
            this->lockedThreadCountMap[this->runningThreadId] = 1;
            this->paramUnlock();
            // first time to lock the object
            this->doLock();
            return zftrue;
        }
        else
        {
            _ZFP_ZFObjectMutexImpl_default_LockedThreadCountMapType::iterator it = this->lockedThreadCountMap.find(curThreadId);
            if(curThreadId == this->runningThreadId)
            {
                ++(it->second);
                this->paramUnlock();
                // lock the same thread more than one time, no need to lock again
                return zftrue;
            }
            else
            {
                this->paramUnlock();
                // lock while another thread acquiring the lock,
                // just return false for mutexTryLock
                return zffalse;
            }
        }
    }

#if ZF_ENV_sys_Windows
private:
    typedef DWORD _ZFP_ZFObjectMutexImpl_default_ThreadIdType;
    typedef zfstlmap<_ZFP_ZFObjectMutexImpl_default_ThreadIdType, zfindex> _ZFP_ZFObjectMutexImpl_default_LockedThreadCountMapType;

    CRITICAL_SECTION mutex;
    CRITICAL_SECTION mutexParamLocker;
    _ZFP_ZFObjectMutexImpl_default_LockedThreadCountMapType lockedThreadCountMap;
    _ZFP_ZFObjectMutexImpl_default_ThreadIdType runningThreadId;
    _ZFP_ZFObjectMutexImpl_default_ThreadIdType INVALID_THREAD_ID;

    void init(void)
    {
        this->INVALID_THREAD_ID = (_ZFP_ZFObjectMutexImpl_default_ThreadIdType)INVALID_HANDLE_VALUE;
        ::InitializeCriticalSection(&(this->mutex));
        ::InitializeCriticalSection(&(this->mutexParamLocker));
    }
    void destroy(void)
    {
        ::DeleteCriticalSection(&(this->mutex));
        ::DeleteCriticalSection(&(this->mutexParamLocker));
    }
    _ZFP_ZFObjectMutexImpl_default_ThreadIdType currentThreadId(void)
    {
        return ::GetCurrentThreadId();
    }
    void paramLock(void)
    {
        ::EnterCriticalSection(&(this->mutexParamLocker));
    }
    void paramUnlock(void)
    {
        ::LeaveCriticalSection(&(this->mutexParamLocker));
    }
    void doLock(void)
    {
        ::EnterCriticalSection(&(this->mutex));
    }
    void doUnlock(void)
    {
        ::LeaveCriticalSection(&(this->mutex));
    }
#elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
private:
    typedef pthread_t _ZFP_ZFObjectMutexImpl_default_ThreadIdType;
    typedef zfstlmap<_ZFP_ZFObjectMutexImpl_default_ThreadIdType, zfindex> _ZFP_ZFObjectMutexImpl_default_LockedThreadCountMapType;

    pthread_mutex_t mutex;
    pthread_mutex_t mutexParamLocker;
    _ZFP_ZFObjectMutexImpl_default_LockedThreadCountMapType lockedThreadCountMap;
    _ZFP_ZFObjectMutexImpl_default_ThreadIdType runningThreadId;
    _ZFP_ZFObjectMutexImpl_default_ThreadIdType INVALID_THREAD_ID;

    void init(void)
    {
        this->INVALID_THREAD_ID = (_ZFP_ZFObjectMutexImpl_default_ThreadIdType)-1;
        ::pthread_mutex_init(&(this->mutex), NULL);
        ::pthread_mutex_init(&(this->mutexParamLocker), NULL);
    }
    void destroy(void)
    {
        ::pthread_mutex_destroy(&(this->mutex));
        ::pthread_mutex_destroy(&(this->mutexParamLocker));
    }
    _ZFP_ZFObjectMutexImpl_default_ThreadIdType currentThreadId(void)
    {
        return ::pthread_self();
    }
    void paramLock(void)
    {
        (void)::pthread_mutex_lock(&(this->mutexParamLocker));
    }
    void paramUnlock(void)
    {
        ::pthread_mutex_unlock(&(this->mutexParamLocker));
    }
    void doLock(void)
    {
        (void)::pthread_mutex_lock(&(this->mutex));
    }
    void doUnlock(void)
    {
        ::pthread_mutex_unlock(&(this->mutex));
    }
#endif // #if ZF_ENV_sys_Windows
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFObjectMutexImpl_default, ZFObjectMutex, ZFProtocolLevel::e_Default)
public:
    virtual ZFObjectMutexImpl *nativeMutexCreate(void)
    {
        return zfnew(_ZFP_ZFObjectMutexImpl_default_MutexImpl);
    }
    virtual void nativeMutexDestroy(ZF_IN ZFObjectMutexImpl *nativeMutex)
    {
        zfdelete(nativeMutex);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFObjectMutexImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFObjectMutexImpl_default)

ZF_NAMESPACE_GLOBAL_END

