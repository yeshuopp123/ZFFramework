/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFLeakTestDef.h
 * @brief leak test utility
 */

#ifndef _ZFI_ZFLeakTestDef_h_
#define _ZFI_ZFLeakTestDef_h_

#include "ZFObjectCoreRetainDef.h"
#include "ZFLeakTestObjectFilterDef.h"
#include "ZFOutputCallbackDefault.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// global settings
/**
 * @brief whether ZFLeakTest enabled, true by default
 */
extern ZF_ENV_EXPORT zfbool ZFLeakTestEnable(void);
/** @brief see #ZFLeakTestEnable */
extern ZF_ENV_EXPORT void ZFLeakTestEnableSet(ZF_IN zfbool enable);

zfclassFwd ZFClass;
zfclassFwd ZFObject;

extern ZF_ENV_EXPORT zfbool _ZFP_ZFLeakTestEnableCache;

/**
 * @brief default output callback for ZFLeakTest
 */
#define ZFLeakTestOutputCallbackDefault ZFOutputCallbackDefault

/** @brief see #ZFLeakTestBegin */
zfclassLikePOD ZF_ENV_EXPORT ZFLeakTestBeginParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFLeakTestBeginParam)

    /** @brief see #ZFLeakTestBegin, the #ZFOutputCallback to use, #ZFLeakTestOutputCallbackDefault by default */
    ZFCORE_PARAM_WITH_INIT(ZFOutputCallback, outputCallback, ZFLeakTestOutputCallbackDefault)

    /** @brief see #ZFLeakTestBegin, whether to check over-released object, zffalse by default */
    ZFCORE_PARAM_WITH_INIT(zfbool, checkOverRelease, zffalse)

    /** @brief see #ZFLeakTestBegin, whether output verbose info, zffalse by default */
    ZFCORE_PARAM_WITH_INIT(zfbool, verboseMode, zffalse)
};

extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestBegin(ZF_IN const zfcharA *callerFile,
                                               ZF_IN const zfcharA *callerFunction,
                                               ZF_IN zfindex callerLine,
                                               ZF_IN_OPT const ZFLeakTestBeginParam &param = ZFLeakTestBeginParam());
/**
 * @brief begin the leak test, remember to call ZFLeakTestEnd
 *
 * typical usage:
 * @code
 *   int main(int argc, zfcharA **argv)
 *   {
 *       ZFLeakTestBegin();
 *       int retVal = 0;
 *       { // code block so that objects would be released
 *           Application app(argc, argv); // your app routine
 *           retVal = app.exec();
 *       }
 *       ZFLeakTestPrintStatus();
 *       ZFLeakTestEnd();
 *       return retVal;
 *   }
 * @endcode
 *
 * @note if you want something pass through ZFLeakTest, use zfAllocWithoutLeakTest series, but try not to abuse it
 * @note you may call ZFLeakTestBegin multiple times,
 *   but you must make sure ZFLeakTestBegin and ZFLeakTestEnd are paired
 * @note leak test consumes much CPU and memory, you should enable only if necessary,
 *   and should not enable for too long,
 *   since object may retain or release much frequently,
 *   the actions would be accumulated and may consume much memory to store
 */
#define ZFLeakTestBegin(...) _ZFP_ZFLeakTestBegin( \
    ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, \
    ##__VA_ARGS__)

extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestEnd(void);
/**
 * @brief end the leak test
 *
 * you may want to use #ZFLeakTestPrintStatus before end
 */
#define ZFLeakTestEnd() _ZFP_ZFLeakTestEnd()

// ============================================================
// global exclude list
/**
 * @brief add a global exclude to leak test
 *
 * internal use only, and usually used to exclude internal types,
 * take good care since it affect global range\n
 * after add, the leak test info would also be collected,
 * while no log would be printed when #ZFLeakTestPrintStatus\n
 * safe to be added more than one time
 * @see ZFLeakTestExcludeNameRemove, ZFLeakTestExcludeInstanceAdd
 */
extern ZF_ENV_EXPORT void ZFLeakTestExcludeNameAdd(ZF_IN const zfchar *name);
/**
 * @brief see #ZFLeakTestExcludeNameAdd
 */
extern ZF_ENV_EXPORT void ZFLeakTestExcludeNameRemove(ZF_IN const zfchar *name);
/**
 * @brief add a global exclude to leak test
 *
 * internal use only, and usually used to exclude internal types,
 * take good care since it affect global range\n
 * after add, the leak test info would also be collected,
 * while no log would be printed when #ZFLeakTestPrintStatus\n
 * safe to be added more than one time\n
 * unlike #ZFLeakTestExcludeNameRemove,
 * excluded instance would be automatically removed after the object deallocated,
 * and you may also remove it manually by #ZFLeakTestExcludeInstanceRemove
 * @see ZFLeakTestExcludeInstanceRemove, ZFLeakTestExcludeNameAdd
 */
extern ZF_ENV_EXPORT void ZFLeakTestExcludeInstanceAdd(ZF_IN ZFObject *obj);
/**
 * @brief see #ZFLeakTestExcludeInstanceAdd
 */
extern ZF_ENV_EXPORT void ZFLeakTestExcludeInstanceRemove(ZF_IN ZFObject *obj);

/**
 * @brief util macro to declare a global exclude name for ZFLeakTest
 *
 * add it to cpp file to make it active
 */
#define ZFLEAKTEST_EXCLUDENAME_DEFINE(Name) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFM_CAT(ZFLeakTestGlobalExcludeName, Name), ZFLevelZFFrameworkNormal) \
    { \
        ZFLeakTestExcludeNameAdd(ZFM_TOSTRING(Name)); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFM_CAT(ZFLeakTestGlobalExcludeName, Name)) \
    { \
        ZFLeakTestExcludeNameRemove(ZFM_TOSTRING(Name)); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFM_CAT(ZFLeakTestGlobalExcludeName, Name))

// ============================================================
extern ZF_ENV_EXPORT ZFLeakTestObjectFilter _ZFP_ZFLeakTestObjectFilterNull;
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestPrintStatus(ZF_IN const zfcharA *callerFile,
                                                     ZF_IN const zfcharA *callerFunction,
                                                     ZF_IN zfindex callerLine,
                                                     ZF_IN_OPT const ZFLeakTestObjectFilter &filter = _ZFP_ZFLeakTestObjectFilterNull);
/**
 * @brief print current status
 *
 * print all object with non-zero objectRetainCount, along with the information of alloc/retain/release\n
 * must be called between ZFLeakTestBegin and ZFLeakTestEnd\n
 * may take 0 or 1 param, which is ZFLeakTestObjectFilter,
 * for how to use filter, see #ZFLeakTestObjectFilter
 */
#define ZFLeakTestPrintStatus(...) _ZFP_ZFLeakTestPrintStatus( \
    ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, \
    ##__VA_ARGS__)

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogAfterAlloc(ZF_IN ZFObject *obj,
                                                       ZF_IN const zfcharA *callerFile,
                                                       ZF_IN const zfcharA *callerFunction,
                                                       ZF_IN zfindex callerLine);
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogAfterRetain(ZF_IN ZFObject *obj,
                                                        ZF_IN const zfcharA *callerFile,
                                                        ZF_IN const zfcharA *callerFunction,
                                                        ZF_IN zfindex callerLine);
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogBeforeRelease(ZF_IN ZFObject *obj,
                                                          ZF_IN const zfcharA *callerFile,
                                                          ZF_IN const zfcharA *callerFunction,
                                                          ZF_IN zfindex callerLine);
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogAutoReleaseAdd(ZF_IN ZFObject *obj,
                                                           ZF_IN const zfcharA *callerFile,
                                                           ZF_IN const zfcharA *callerFunction,
                                                           ZF_IN zfindex callerLine);
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogAutoReleaseBeforeRelease(ZFObject *obj);
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogBeforeDealloc(ZFObject *obj);

// ============================================================
/**
 * @brief manually log alloc
 *
 * log only, not actually modify the retain count\n
 * usually used internally, try not to use this in your application
 */
#define ZFLeakTestLogAfterAllocVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        ZFCoreMutexLock(); \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAfterAlloc(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
        ZFCoreMutexUnlock(); \
    } while(0)
/** @brief see #ZFLeakTestLogAfterAllocVerbose */
#define ZFLeakTestLogAfterAlloc(obj) \
    ZFLeakTestLogAfterAllocVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/** @brief no lock version of #ZFLeakTestLogAfterAllocVerbose, use with causion */
#define zflockfree_ZFLeakTestLogAfterAllocVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAfterAlloc(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
    } while(0)
/** @brief no lock version of #ZFLeakTestLogAfterAlloc, use with causion */
#define zflockfree_ZFLeakTestLogAfterAlloc(obj) \
    zflockfree_ZFLeakTestLogAfterAllocVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

/**
 * @brief manually log retain
 *
 * log only, not actually modify the retain count\n
 * usually used internally, try not to use this in your application
 */
#define ZFLeakTestLogAfterRetainVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        ZFCoreMutexLock(); \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAfterRetain(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
        ZFCoreMutexUnlock(); \
    } while(0)
/** @brief see #ZFLeakTestLogAfterRetainVerbose */
#define ZFLeakTestLogAfterRetain(obj) \
    ZFLeakTestLogAfterRetainVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/** @brief no lock version of #ZFLeakTestLogAfterRetainVerbose, use with causion */
#define zflockfree_ZFLeakTestLogAfterRetainVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAfterRetain(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
    } while(0)
/** @brief no lock version of #ZFLeakTestLogAfterRetain, use with causion */
#define zflockfree_ZFLeakTestLogAfterRetain(obj) \
    zflockfree_ZFLeakTestLogAfterRetainVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

/**
 * @brief manually log release
 *
 * log only, not actually modify the retain count\n
 * usually used internally, try not to use this in your application
 */
#define ZFLeakTestLogBeforeReleaseVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        ZFCoreMutexLock(); \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogBeforeRelease(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
        ZFCoreMutexUnlock(); \
    } while(0)
/** @brief see #ZFLeakTestLogBeforeReleaseVerbose */
#define ZFLeakTestLogBeforeRelease(obj) \
    ZFLeakTestLogBeforeReleaseVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/** @brief no lock version of #ZFLeakTestLogBeforeReleaseVerbose, use with causion */
#define zflockfree_ZFLeakTestLogBeforeReleaseVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogBeforeRelease(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
    } while(0)
/** @brief no lock version of #ZFLeakTestLogBeforeRelease, use with causion */
#define zflockfree_ZFLeakTestLogBeforeRelease(obj) \
    zflockfree_ZFLeakTestLogBeforeReleaseVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

/**
 * @brief manually log autorelease or add to #ZFAutoReleasePool
 *
 * log only, not actually modify the retain count\n
 * usually used internally, try not to use this in your application
 */
#define ZFLeakTestLogAutoReleaseAddVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        ZFCoreMutexLock(); \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAutoReleaseAdd(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
        ZFCoreMutexUnlock(); \
    } while(0)
/** @brief see #ZFLeakTestLogAutoReleaseAddVerbose */
#define ZFLeakTestLogAutoReleaseAdd(obj) \
    ZFLeakTestLogAutoReleaseAddVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/** @brief no lock version of #ZFLeakTestLogAutoReleaseAddVerbose, use with causion */
#define zflockfree_ZFLeakTestLogAutoReleaseAddVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAutoReleaseAdd(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
    } while(0)
/** @brief no lock version of #ZFLeakTestLogAutoReleaseAdd, use with causion */
#define zflockfree_ZFLeakTestLogAutoReleaseAdd(obj) \
    zflockfree_ZFLeakTestLogAutoReleaseAddVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

/**
 * @brief log that a autoreleased object is being released
 *
 * log only, not actually modify the retain count\n
 * usually used internally, try not to use this in your application
 */
#define ZFLeakTestLogAutoReleaseBeforeRelease(obj) \
    do \
    { \
        ZFCoreMutexLock(); \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAutoReleaseBeforeRelease(ZFCastZFObjectUnchecked(ZFObject *, obj)); \
        } \
        ZFCoreMutexUnlock(); \
    } while(0)
/** @brief no lock version of #ZFLeakTestLogAutoReleaseBeforeRelease, use with causion */
#define zflockfree_ZFLeakTestLogAutoReleaseBeforeRelease(obj) \
    do \
    { \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAutoReleaseBeforeRelease(ZFCastZFObjectUnchecked(ZFObject *, obj)); \
        } \
    } while(0)

/**
 * @brief log that a object is being deallocated
 *
 * log only, not actually modify the retain count\n
 * usually used internally, try not to use this in your application
 */
#define ZFLeakTestLogBeforeDealloc(obj) \
    do \
    { \
        ZFCoreMutexLock(); \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogBeforeDealloc(ZFCastZFObjectUnchecked(ZFObject *, obj)); \
        } \
        ZFCoreMutexUnlock(); \
    } while(0)
/** @brief no lock version of #ZFLeakTestLogBeforeDealloc, use with causion */
#define zflockfree_ZFLeakTestLogBeforeDealloc(obj) \
    do \
    { \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogBeforeDealloc(ZFCastZFObjectUnchecked(ZFObject *, obj)); \
        } \
    } while(0)

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogCacheAdd(ZF_IN ZFObject *obj);
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogCacheRemove(ZF_IN ZFObject *obj);

// ============================================================
template<typename T_ZFObject>
inline T_ZFObject *_ZFP_ZFLeakTestAlloc(ZF_IN T_ZFObject *obj,
                                        ZF_IN const zfcharA *callerFile,
                                        ZF_IN const zfcharA *callerFunction,
                                        ZF_IN zfindex callerLine)
{
    if(_ZFP_ZFLeakTestEnableCache)
    {
        _ZFP_ZFLeakTestLogAfterAlloc(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine);
    }
    return obj;
}
template<typename T_ZFObject>
inline T_ZFObject *_ZFP_ZFLeakTestRetain(ZF_IN T_ZFObject *obj,
                                         ZF_IN const zfcharA *callerFile,
                                         ZF_IN const zfcharA *callerFunction,
                                         ZF_IN zfindex callerLine)
{
    zflockfree_zfRetainWithoutLeakTest(obj);
    _ZFP_ZFLeakTestLogAfterRetain(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine);
    return obj;
}
inline const ZFAny &_ZFP_ZFLeakTestRetain(ZF_IN const ZFAny &any,
                                          ZF_IN const zfcharA *callerFile,
                                          ZF_IN const zfcharA *callerFunction,
                                          ZF_IN zfindex callerLine)
{
    zflockfree_zfRetainWithoutLeakTest(any);
    _ZFP_ZFLeakTestLogAfterRetain(any.toObject(), callerFile, callerFunction, callerLine);
    return any;
}
template<typename T_ZFObject>
inline const ZFAnyT<T_ZFObject *> &_ZFP_ZFLeakTestRetain(ZF_IN const ZFAnyT<T_ZFObject *> &any,
                                                         ZF_IN const zfcharA *callerFile,
                                                         ZF_IN const zfcharA *callerFunction,
                                                         ZF_IN zfindex callerLine)
{
    zflockfree_zfRetainWithoutLeakTest(any);
    _ZFP_ZFLeakTestLogAfterRetain(any.toObject(), callerFile, callerFunction, callerLine);
    return any;
}
template<typename T_ZFObject>
inline void _ZFP_ZFLeakTestRelease(T_ZFObject *obj,
                                   ZF_IN const zfcharA *callerFile,
                                   ZF_IN const zfcharA *callerFunction,
                                   ZF_IN zfindex callerLine)
{
    _ZFP_ZFLeakTestLogBeforeRelease(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine);
    zflockfree_zfReleaseWithoutLeakTest(obj);
}
inline void _ZFP_ZFLeakTestRelease(ZF_IN const ZFAny &any,
                                   ZF_IN const zfcharA *callerFile,
                                   ZF_IN const zfcharA *callerFunction,
                                   ZF_IN zfindex callerLine)
{
    _ZFP_ZFLeakTestRelease(any.toObject(), callerFile, callerFunction, callerLine);
}
template<typename T_ZFObject>
inline void _ZFP_ZFLeakTestRelease(ZF_IN const ZFAnyT<T_ZFObject *> &any,
                                   ZF_IN const zfcharA *callerFile,
                                   ZF_IN const zfcharA *callerFunction,
                                   ZF_IN zfindex callerLine)
{
    _ZFP_ZFLeakTestRelease(any.toObject(), callerFile, callerFunction, callerLine);
}

// ============================================================
/**
 * @brief alloc an object, see #ZFObject
 */
#define zfAlloc(T_ZFObject, ...) \
    (ZFCoreMutexLockerHolder(), zflockfree_zfAlloc(T_ZFObject, ##__VA_ARGS__))
/** @brief no lock version of #zfAlloc, use with causion */
#define zflockfree_zfAlloc(T_ZFObject, ...) \
    _ZFP_ZFLeakTestAlloc( \
        zflockfree_zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__), \
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

/**
 * @brief retain an object, see #ZFObject
 */
#define zfRetain(obj) \
    (ZFCoreMutexLockerHolder(), zflockfree_zfRetain(obj))
/** @brief no lock version of #zfRetain, use with causion */
#define zflockfree_zfRetain(obj) \
    (_ZFP_ZFLeakTestEnableCache \
        ? _ZFP_ZFLeakTestRetain(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE) \
        : zflockfree_zfRetainWithoutLeakTest(obj))

/**
 * @brief release an object, see #ZFObject
 */
#define zfRelease(obj) \
    (ZFCoreMutexLockerHolder(), zflockfree_zfRelease(obj))
/** @brief no lock version of #zfRetain, use with causion */
#define zflockfree_zfRelease(obj) \
    (_ZFP_ZFLeakTestEnableCache \
        ? _ZFP_ZFLeakTestRelease(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE) \
        : zflockfree_zfReleaseWithoutLeakTest(obj))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLeakTestDef_h_

