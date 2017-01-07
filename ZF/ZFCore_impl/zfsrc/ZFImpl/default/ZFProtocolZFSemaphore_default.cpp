/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFSemaphore.h"

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    #include <pthread.h>
    #include <sys/time.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFSemaphoreImpl_default_Token
{
public:
    zfindex waiterCount;

#if ZF_ENV_sys_Windows
    HANDLE sema;
    CRITICAL_SECTION semaMutex;
    CRITICAL_SECTION mutexParamLocker;
#elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    pthread_cond_t sema;
    pthread_mutex_t semaLocker;
    pthread_mutex_t mutexParamLocker;
#endif
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFSemaphoreImpl_default, ZFSemaphore, ZFProtocolLevel::e_Default)
public:
    virtual void *nativeSemaphoreCreate(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken = zfnew(_ZFP_ZFSemaphoreImpl_default_Token);
        semaphoreToken->waiterCount = 0;
        this->init(semaphoreToken);
        return semaphoreToken;
    }
    virtual void nativeSemaphoreDestroy(ZF_IN ZFSemaphore *semaphore,
                                        ZF_IN void *nativeSemaphore)
    {
        _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken = ZFCastStatic(_ZFP_ZFSemaphoreImpl_default_Token *, nativeSemaphore);
        this->destroy(semaphoreToken);
        zfdelete(semaphoreToken);
    }

    virtual void semaphoreLock(ZF_IN ZFSemaphore *semaphore)
    {
        this->paramLock(ZFCastStatic(_ZFP_ZFSemaphoreImpl_default_Token *, semaphore->nativeSemaphore()));
    }
    virtual void semaphoreUnlock(ZF_IN ZFSemaphore *semaphore)
    {
        this->paramUnlock(ZFCastStatic(_ZFP_ZFSemaphoreImpl_default_Token *, semaphore->nativeSemaphore()));
    }

    virtual void semaphoreSignal(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken = ZFCastStatic(_ZFP_ZFSemaphoreImpl_default_Token *, semaphore->nativeSemaphore());

        this->paramLock(semaphoreToken);
        if(semaphoreToken->waiterCount > 0)
        {
            --(semaphoreToken->waiterCount);
            this->paramUnlock(semaphoreToken);
            this->semaSignal(semaphoreToken, 1);
        }
        else
        {
            this->paramUnlock(semaphoreToken);
        }
    }
    virtual void semaphoreBroadcast(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken = ZFCastStatic(_ZFP_ZFSemaphoreImpl_default_Token *, semaphore->nativeSemaphore());

        this->paramLock(semaphoreToken);
        if(semaphoreToken->waiterCount > 0)
        {
            zfindex tmp = semaphoreToken->waiterCount;
            semaphoreToken->waiterCount = 0;
            this->paramUnlock(semaphoreToken);
            this->semaSignal(semaphoreToken, tmp);
        }
        else
        {
            this->paramUnlock(semaphoreToken);
        }
    }
    virtual void semaphoreWait(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken = ZFCastStatic(_ZFP_ZFSemaphoreImpl_default_Token *, semaphore->nativeSemaphore());

        this->paramLock(semaphoreToken);
        ++(semaphoreToken->waiterCount);
        this->paramUnlock(semaphoreToken);
        this->semaWait(semaphoreToken);
    }
    virtual zfbool semaphoreWait(ZF_IN ZFSemaphore *semaphore,
                                 ZF_IN const zftimet &miliSecsTimeout)
    {
        _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken = ZFCastStatic(_ZFP_ZFSemaphoreImpl_default_Token *, semaphore->nativeSemaphore());

        this->paramLock(semaphoreToken);
        ++(semaphoreToken->waiterCount);
        this->paramUnlock(semaphoreToken);

        if(this->semaWait(semaphoreToken, miliSecsTimeout))
        {
            return zftrue;
        }
        else
        {
            this->paramLock(semaphoreToken);
            if(semaphoreToken->waiterCount > 0)
            {
                --(semaphoreToken->waiterCount);
            }
            this->paramUnlock(semaphoreToken);
            return zffalse;
        }
    }

#if ZF_ENV_sys_Windows
public:
    void init(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        semaphoreToken->sema = ::CreateSemaphore(zfnull, 0, MAXLONG, zfnull);
        ::InitializeCriticalSection(&(semaphoreToken->semaMutex));
        ::InitializeCriticalSection(&(semaphoreToken->mutexParamLocker));
    }
    void destroy(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        ::CloseHandle(semaphoreToken->sema);
        ::DeleteCriticalSection(&(semaphoreToken->semaMutex));
        ::DeleteCriticalSection(&(semaphoreToken->mutexParamLocker));
    }
    void paramLock(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        ::EnterCriticalSection(&(semaphoreToken->mutexParamLocker));
    }
    void paramUnlock(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        ::LeaveCriticalSection(&(semaphoreToken->mutexParamLocker));
    }
    void semaLock(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        ::EnterCriticalSection(&(semaphoreToken->semaMutex));
    }
    void semaUnlock(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        ::LeaveCriticalSection(&(semaphoreToken->semaMutex));
    }
    void semaSignal(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken,
                    ZF_IN zfindex num1orBroadcastNum)
    {
        ::LeaveCriticalSection(&(semaphoreToken->semaMutex));
        ::ReleaseSemaphore(semaphoreToken->sema, num1orBroadcastNum, zfnull);
        ::EnterCriticalSection(&(semaphoreToken->semaMutex));
    }
    void semaWait(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        ::LeaveCriticalSection(&(semaphoreToken->semaMutex));
        ::WaitForSingleObject(semaphoreToken->sema, INFINITE);
        ::EnterCriticalSection(&(semaphoreToken->semaMutex));
    }
    zfbool semaWait(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken,
                    ZF_IN const zftimet &miliSecs)
    {
        ::LeaveCriticalSection(&(semaphoreToken->semaMutex));
        zfbool ret = (::WaitForSingleObject(semaphoreToken->sema, (DWORD)(miliSecs)) == WAIT_OBJECT_0);
        ::EnterCriticalSection(&(semaphoreToken->semaMutex));
        return ret;
    }
#elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
public:
    void init(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        ::pthread_cond_init(&(semaphoreToken->sema), zfnull);
        ::pthread_mutex_init(&(semaphoreToken->semaLocker), zfnull);
        ::pthread_mutex_init(&(semaphoreToken->mutexParamLocker), zfnull);
    }
    void destroy(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        ::pthread_cond_destroy(&(semaphoreToken->sema));
        ::pthread_mutex_destroy(&(semaphoreToken->semaLocker));
        ::pthread_mutex_destroy(&(semaphoreToken->mutexParamLocker));
    }
    void paramLock(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        (void)::pthread_mutex_lock(&(semaphoreToken->mutexParamLocker));
    }
    void paramUnlock(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        ::pthread_mutex_unlock(&(semaphoreToken->mutexParamLocker));
    }
    void semaLock(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        (void)::pthread_mutex_lock(&(semaphoreToken->semaLocker));
    }
    void semaUnlock(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        ::pthread_mutex_unlock(&(semaphoreToken->semaLocker));
    }
    void semaSignal(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken,
                    ZF_IN zfindex num1orBroadcastNum)
    {
        if(num1orBroadcastNum == 1)
        {
            ::pthread_cond_signal(&(semaphoreToken->sema));
        }
        else
        {
            ::pthread_cond_broadcast(&(semaphoreToken->sema));
        }
    }
    void semaWait(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken)
    {
        ::pthread_cond_wait(&(semaphoreToken->sema), &(semaphoreToken->semaLocker));
    }
    zfbool semaWait(ZF_IN _ZFP_ZFSemaphoreImpl_default_Token *semaphoreToken,
                   ZF_IN const zftimet &miliSecs)
    {
        timeval timev;
        timespec t;
        gettimeofday(&timev, zfnull);
        t.tv_sec = (time_t)(timev.tv_sec + (miliSecs / 1000));
        t.tv_nsec = (miliSecs % 1000) * 1000 * 1000;
        return (::pthread_cond_timedwait(&(semaphoreToken->sema), &(semaphoreToken->semaLocker), &t) == 0);
    }
#endif
ZFPROTOCOL_IMPLEMENTATION_END(ZFSemaphoreImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFSemaphoreImpl_default)

ZF_NAMESPACE_GLOBAL_END

