/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSynchronizedDef.h
 * @brief type define for synchronize logic in ZFFramework
 */

#ifndef _ZFI_ZFSynchronizedDef_h_
#define _ZFI_ZFSynchronizedDef_h_

#include "ZFObjectDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT ZFObject *_ZFP_zfsynchronizeObject;

// ============================================================
// internal test only
/**
 * @brief whether enable log for ZFSynchronize
 *
 * for debug use only, may cause serious performance issue\n
 * this macro can safely be re-defined per complication unit,
 * use with caution, though\n
 * the debug output can be accessed by #ZFSynchronizeLogCallbackList
 */
#define ZFSYNCHRONIZE_LOG_ENABLE 0
/**
 * @brief see #ZFSYNCHRONIZE_LOG_ENABLE
 */
typedef void (*ZFSynchronizeLogCallback)(ZF_IN const zfcharA *callerFile,
                                         ZF_IN const zfcharA *callerFunction,
                                         ZF_IN zfindex callerLine,
                                         ZF_IN const zfcharA *text);
/**
 * @brief see #ZFSYNCHRONIZE_LOG_ENABLE
 */
extern ZF_ENV_EXPORT ZFCoreArrayPOD<ZFSynchronizeLogCallback> ZFSynchronizeLogCallbackList;

// ============================================================
// no log version
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFSynchronizedObjectContainer
{
public:
    _ZFP_ZFSynchronizedObjectContainer(ZF_IN ZFObject *obj)
    : m_obj(obj)
    {
        if(obj == zfnull)
        {
            zfCoreCriticalMessageTrim(zfTextA("[zfsynchronize] try synchronize a null object"));
        }
        else
        {
            zfRetainWithoutLeakTest(m_obj);
            m_obj->_ZFP_ZFObjectLock();
        }
    }
    ~_ZFP_ZFSynchronizedObjectContainer(void)
    {
        m_obj->_ZFP_ZFObjectUnlock();
        zfReleaseWithoutLeakTest(m_obj);
    }
private:
    ZFObject *m_obj;
};

inline void _ZFP_ZFSynchronizedObjectLock(ZF_IN ZFObject *obj)
{
    if(obj == zfnull)
    {
        zfCoreCriticalMessageTrim(zfTextA("[zfsynchronize] try lock a null object"));
    }
    else
    {
        zfRetainWithoutLeakTest(obj);
        obj->_ZFP_ZFObjectLock();
    }
}
inline void _ZFP_ZFSynchronizedObjectUnlock(ZF_IN ZFObject *obj)
{
    if(obj == zfnull)
    {
        zfCoreCriticalMessageTrim(zfTextA("[zfsynchronize] try unlock a null object"));
    }
    else
    {
        obj->_ZFP_ZFObjectUnlock();
        zfReleaseWithoutLeakTest(obj);
    }
}

zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFSynchronizeContainer
{
public:
    _ZFP_ZFSynchronizeContainer(void)
    : locked(zffalse)
    {
        if(_ZFP_zfsynchronizeObject)
        {
            zfRetainWithoutLeakTest(_ZFP_zfsynchronizeObject);
            _ZFP_zfsynchronizeObject->_ZFP_ZFObjectLock();
            this->locked = zftrue;
        }
    }
    ~_ZFP_ZFSynchronizeContainer(void)
    {
        if(this->locked && _ZFP_zfsynchronizeObject)
        {
            _ZFP_zfsynchronizeObject->_ZFP_ZFObjectUnlock();
            zfReleaseWithoutLeakTest(_ZFP_zfsynchronizeObject);
        }
    }
private:
    zfbool locked;
};

inline void _ZFP_ZFSynchronizeLock(void)
{
    if(_ZFP_zfsynchronizeObject)
    {
        zfRetainWithoutLeakTest(_ZFP_zfsynchronizeObject);
        _ZFP_zfsynchronizeObject->_ZFP_ZFObjectLock();
    }
}
inline void _ZFP_ZFSynchronizeUnlock(void)
{
    if(_ZFP_zfsynchronizeObject)
    {
        _ZFP_zfsynchronizeObject->_ZFP_ZFObjectUnlock();
        zfReleaseWithoutLeakTest(_ZFP_zfsynchronizeObject);
    }
}

// ============================================================
// log version
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFSynchronizedObjectContainerWithLog
{
public:
    _ZFP_ZFSynchronizedObjectContainerWithLog(ZF_IN ZFObject *obj,
                                              ZF_IN const zfcharA *callerFile,
                                              ZF_IN const zfcharA *callerFunction,
                                              ZF_IN zfindex callerLine);
    ~_ZFP_ZFSynchronizedObjectContainerWithLog(void);
private:
    ZFObject *m_obj;
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};

extern ZF_ENV_EXPORT void _ZFP_ZFSynchronizedObjectLockWithLog(ZF_IN ZFObject *obj,
                                                               ZF_IN const zfcharA *callerFile,
                                                               ZF_IN const zfcharA *callerFunction,
                                                               ZF_IN zfindex callerLine);
extern ZF_ENV_EXPORT void _ZFP_ZFSynchronizedObjectUnlockWithLog(ZF_IN ZFObject *obj,
                                                                 ZF_IN const zfcharA *callerFile,
                                                                 ZF_IN const zfcharA *callerFunction,
                                                                 ZF_IN zfindex callerLine);

zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFSynchronizeContainerWithLog
{
public:
    _ZFP_ZFSynchronizeContainerWithLog(ZF_IN const zfcharA *callerFile,
                                       ZF_IN const zfcharA *callerFunction,
                                       ZF_IN zfindex callerLine);
    ~_ZFP_ZFSynchronizeContainerWithLog(void);
private:
    zfbool locked;
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};

extern ZF_ENV_EXPORT void _ZFP_ZFSynchronizeLockWithLog(ZF_IN const zfcharA *callerFile,
                                                        ZF_IN const zfcharA *callerFunction,
                                                        ZF_IN zfindex callerLine);
extern ZF_ENV_EXPORT void _ZFP_ZFSynchronizeUnlockWithLog(ZF_IN const zfcharA *callerFile,
                                                          ZF_IN const zfcharA *callerFunction,
                                                          ZF_IN zfindex callerLine);

// ============================================================
/**
 * @brief similar to  synchronized in Java
 *
 * usage:
 * @code
 *   ZFObject *obj = ...;
 *   ...
 *   // not mutable
 *   {
 *       zfsynchronizedObject(obj);
 *       // mutable
 *   }
 *   // not mutable
 * @endcode
 */
#if ZFSYNCHRONIZE_LOG_ENABLE
    #define zfsynchronizedObject(obj) \
        _ZFP_ZFSynchronizedObjectContainerWithLog ZFUniqueName(zfsynchronizedObject_holder)(obj \
            , ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
#else
    #define zfsynchronizedObject(obj) \
        _ZFP_ZFSynchronizedObjectContainer ZFUniqueName(zfsynchronizedObject_holder)(obj)
#endif

/**
 * @brief similar to #zfsynchronizedObject, while you take the responsibility to close it by #zfsynchronizedObjectUnlock
 *
 * usage:
 * @code
 *   zfsynchronizedObjectLock(obj);
 *   // mutable operation
 *   zfsynchronizedObjectUnlock(obj);
 * @endcode
 * it's not necessary to put them in the same code block,
 * but you must ensure the lock and unlock are paired
 * @see zfsynchronizedObject
 */
#if ZFSYNCHRONIZE_LOG_ENABLE
    #define zfsynchronizedObjectLock(obj) _ZFP_ZFSynchronizedObjectLockWithLog(obj \
        , ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
#else
    #define zfsynchronizedObjectLock(obj) _ZFP_ZFSynchronizedObjectLock(obj)
#endif

/**
 * @brief see #zfsynchronizedObjectLock
 */
#if ZFSYNCHRONIZE_LOG_ENABLE
    #define zfsynchronizedObjectUnlock(obj) _ZFP_ZFSynchronizedObjectUnlockWithLog(obj \
        , ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
#else
    #define zfsynchronizedObjectUnlock(obj) _ZFP_ZFSynchronizedObjectUnlock(obj)
#endif

/**
 * @brief make current code block mutable
 *
 * usage:
 * @code
 *   void func()
 *   {
 *       zfsynchronize();
 *       // mutable operation
 *       if(condition) {return ;} // safe to return
 *       // still mutable
 *   } // end mutable when brace end
 * @endcode
 * @see zfsynchronizeLock, zfsynchronizeUnlock
 */
#if ZFSYNCHRONIZE_LOG_ENABLE
    #define zfsynchronize() _ZFP_ZFSynchronizeContainerWithLog ZFUniqueName(zfsynchronize)( \
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
#else
    #define zfsynchronize() _ZFP_ZFSynchronizeContainer ZFUniqueName(zfsynchronize)
#endif

/**
 * @brief similar to #zfsynchronize, while you take the responsibility to close it by #zfsynchronizeUnlock
 *
 * usage:
 * @code
 *   zfsynchronizeLock();
 *   // mutable operation
 *   zfsynchronizeUnlock();
 * @endcode
 * it's not necessary to put them in the same code block,
 * but you must ensure the lock and unlock are paired
 * @see zfsynchronize
 */
#if ZFSYNCHRONIZE_LOG_ENABLE
    #define zfsynchronizeLock() _ZFP_ZFSynchronizeLockWithLog(\
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
#else
    #define zfsynchronizeLock() _ZFP_ZFSynchronizeLock()
#endif

/**
 * @brief see #zfsynchronizeLock
 */
#if ZFSYNCHRONIZE_LOG_ENABLE
    #define zfsynchronizeUnlock() _ZFP_ZFSynchronizeUnlockWithLog(\
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
#else
    #define zfsynchronizeUnlock() _ZFP_ZFSynchronizeUnlock()
#endif

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSynchronizedDef_h_

