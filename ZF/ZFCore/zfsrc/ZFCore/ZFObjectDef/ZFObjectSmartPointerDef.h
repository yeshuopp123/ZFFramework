/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectSmartPointerDef.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_ZFObjectSmartPointerDef_h_
#define _ZFI_ZFObjectSmartPointerDef_h_

#include "ZFObjectDef.h"
#include "ZFObjectRetainDef.h"
#include "ZFAnyDef.h"
#include "zfautoObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFCOREPOINTER_DECLARE(ZFCorePointerForZFObject, {
        ZFCoreMutexLock();
        zflockfree_zfRetainWithoutLeakTest(p);
        zflockfree_ZFLeakTestLogAfterRetainVerbose(p, zfTextA("unknown"), zfTextA("ZFCorePointerOnRetain"), 0);
        ZFCoreMutexUnlock();
    }, {
        ZFCoreMutexLock();
        zflockfree_ZFLeakTestLogBeforeReleaseVerbose(p, zfTextA("unknown"), zfTextA("ZFCorePointerOnDelete"), 0);
        zflockfree_zfReleaseWithoutLeakTest(p);
        ZFCoreMutexUnlock();
    })
ZFCOREPOINTER_DECLARE(ZFCorePointerForZFObjectWithoutLeakTest, {
        zfRetainWithoutLeakTest(p);
    }, {
        zfReleaseWithoutLeakTest(p);
    })
ZFCOREPOINTER_DECLARE(ZFCorePointerForZFObjectMarkCached, {
        ZFCoreMutexLock();
        zflockfree_zfRetainWithoutLeakTest(p);
        p->objectCachedSet(zftrue);
        ZFCoreMutexUnlock();
    }, {
        ZFCoreMutexLock();
        p->objectCachedSet(zffalse);
        zflockfree_zfReleaseWithoutLeakTest(p);
        ZFCoreMutexUnlock();
    })

// ============================================================
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_zflineRelease
{
public:
    template<typename T_ZFObject>
    T_ZFObject *set(ZF_IN T_ZFObject *obj,
                    ZF_IN const zfcharA *callerFile,
                    ZF_IN const zfcharA *callerFunction,
                    ZF_IN zfindex callerLine)
    {
        this->obj = ZFCastZFObjectUnchecked(ZFObject *, obj);
        this->callerFile = callerFile;
        this->callerFunction = callerFunction;
        this->callerLine = callerLine;
        return obj;
    }
    const ZFAny &set(ZF_IN const ZFAny &any,
                     ZF_IN const zfcharA *callerFile,
                     ZF_IN const zfcharA *callerFunction,
                     ZF_IN zfindex callerLine)
    {
        this->obj = any.toObject();
        this->callerFile = callerFile;
        this->callerFunction = callerFunction;
        this->callerLine = callerLine;
        return any;
    }
    template<typename T_ZFObject>
    const ZFAnyT<T_ZFObject *> &set(ZF_IN const ZFAnyT<T_ZFObject *> &any,
                                    ZF_IN const zfcharA *callerFile,
                                    ZF_IN const zfcharA *callerFunction,
                                    ZF_IN zfindex callerLine)
    {
        this->obj = any.toObject();
        this->callerFile = callerFile;
        this->callerFunction = callerFunction;
        this->callerLine = callerLine;
        return any;
    }
public:
    _ZFP_zflineRelease(void)
    : obj(zfnull)
    , callerFile(zfnull)
    , callerFunction(zfnull)
    , callerLine(0)
    {
    }
    ~_ZFP_zflineRelease(void)
    {
        if(this->obj != zfnull)
        {
            ZFCoreMutexLock();
            zflockfree_ZFLeakTestLogBeforeReleaseVerbose(this->obj,
                this->callerFile, this->callerFunction, this->callerLine);
            zflockfree_zfReleaseWithoutLeakTest(this->obj);
            ZFCoreMutexUnlock();
        }
    }
public:
    ZFObject *obj;
private:
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_zflockfree_zflineRelease
{
public:
    template<typename T_ZFObject>
    T_ZFObject *set(ZF_IN T_ZFObject *obj,
                    ZF_IN const zfcharA *callerFile,
                    ZF_IN const zfcharA *callerFunction,
                    ZF_IN zfindex callerLine)
    {
        this->obj = ZFCastZFObjectUnchecked(ZFObject *, obj);
        this->callerFile = callerFile;
        this->callerFunction = callerFunction;
        this->callerLine = callerLine;
        return obj;
    }
    const ZFAny &set(ZF_IN const ZFAny &any,
                     ZF_IN const zfcharA *callerFile,
                     ZF_IN const zfcharA *callerFunction,
                     ZF_IN zfindex callerLine)
    {
        this->obj = any.toObject();
        this->callerFile = callerFile;
        this->callerFunction = callerFunction;
        this->callerLine = callerLine;
        return any;
    }
    template<typename T_ZFObject>
    const ZFAnyT<T_ZFObject *> &set(ZF_IN const ZFAnyT<T_ZFObject *> &any,
                                    ZF_IN const zfcharA *callerFile,
                                    ZF_IN const zfcharA *callerFunction,
                                    ZF_IN zfindex callerLine)
    {
        this->obj = any.toObject();
        this->callerFile = callerFile;
        this->callerFunction = callerFunction;
        this->callerLine = callerLine;
        return any;
    }
public:
    _ZFP_zflockfree_zflineRelease(void)
    : obj(zfnull)
    , callerFile(zfnull)
    , callerFunction(zfnull)
    , callerLine(0)
    {
    }
    ~_ZFP_zflockfree_zflineRelease(void)
    {
        if(this->obj != zfnull)
        {
            zflockfree_ZFLeakTestLogBeforeReleaseVerbose(this->obj,
                this->callerFile, this->callerFunction, this->callerLine);
            zflockfree_zfReleaseWithoutLeakTest(this->obj);
        }
    }
public:
    ZFObject *obj;
private:
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_zflineReleaseWithoutLeakTest
{
public:
    template<typename T_ZFObject>
    T_ZFObject *set(T_ZFObject *obj)
    {
        this->obj = ZFCastZFObjectUnchecked(ZFObject *, obj);
        return obj;
    }
    const ZFAny &set(ZF_IN const ZFAny &any)
    {
        this->obj = any.toObject();
        return any;
    }
    template<typename T_ZFObject>
    const ZFAnyT<T_ZFObject *> &set(ZF_IN const ZFAnyT<T_ZFObject *> &any)
    {
        this->obj = any.toObject();
        return any;
    }
public:
    _ZFP_zflineReleaseWithoutLeakTest(void)
    : obj(zfnull)
    {
    }
    ~_ZFP_zflineReleaseWithoutLeakTest(void)
    {
        zfReleaseWithoutLeakTest(obj);
    }
public:
    ZFObject *obj;
};
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_zflockfree_zflineReleaseWithoutLeakTest
{
public:
    template<typename T_ZFObject>
    T_ZFObject *set(T_ZFObject *obj)
    {
        this->obj = ZFCastZFObjectUnchecked(ZFObject *, obj);
        return obj;
    }
    const ZFAny &set(ZF_IN const ZFAny &any)
    {
        this->obj = any.toObject();
        return any;
    }
    template<typename T_ZFObject>
    const ZFAnyT<T_ZFObject *> &set(ZF_IN const ZFAnyT<T_ZFObject *> &any)
    {
        this->obj = any.toObject();
        return any;
    }
public:
    _ZFP_zflockfree_zflineReleaseWithoutLeakTest(void)
    : obj(zfnull)
    {
    }
    ~_ZFP_zflockfree_zflineReleaseWithoutLeakTest(void)
    {
        zflockfree_zfReleaseWithoutLeakTest(obj);
    }
public:
    ZFObject *obj;
};
/**
 * @brief convenient method to release the object after code line end
 *
 * usage:
 * @code
 *   ZFObject *allocObject(void)
 *   {
 *       return zfAlloc(ZFObject);
 *   }
 *
 *   ZFArray *array = ...;
 *   array->add( // step 2, array auto retain it, objectRetainCount 2
 *       zflineRelease(
 *               allocObject() // step 1, objectRetainCount 1
 *           )
 *       ); // step 3, zflineRelease auto release it, objectRetainCount 1
 *
 *   // equivalence:
 *   // ZFObject *obj = allocObject();
 *   // array->add(obj);
 *   // zfRelease(obj);
 * @endcode
 * usually, this method is used to save you some code,
 * or designed for chained code
 */
#define zflineRelease(obj) (_ZFP_zflineRelease().set(obj, \
    ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE))
/** @brief no lock version of #zflineRelease, use with causion */
#define zflockfree_zflineRelease(obj) (_ZFP_zflockfree_zflineRelease().set(obj, \
    ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE))
/**
 * @brief see #zflineRelease
 */
#define zflineReleaseWithoutLeakTest(obj) (_ZFP_zflineReleaseWithoutLeakTest().set(obj))
/** @brief no lock version of #zflineRelease, use with causion */
#define zflockfree_zflineReleaseWithoutLeakTest(obj) (_ZFP_zflockfree_zflineReleaseWithoutLeakTest().set(obj))

// ============================================================
/**
 * @brief same as zflineRelease(zfAlloc(...)), see #zflineRelease
 */
#define zflineAlloc(T_ZFObject, ...) \
    zflineRelease(zfAlloc(T_ZFObject, ##__VA_ARGS__))
/** @brief no lock version of #zflineAlloc, use with causion */
#define zflockfree_zflineAlloc(T_ZFObject, ...) \
    zflockfree_zflineRelease(zflockfree_zfAlloc(T_ZFObject, ##__VA_ARGS__))
/**
 * @brief see #zflineRelease
 */
#define zflineAllocWithoutLeakTest(T_ZFObject, ...) \
    zflineReleaseWithoutLeakTest(zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__))
/** @brief no lock version of #zflineAllocWithoutLeakTest, use with causion */
#define zflockfree_zflineAllocWithoutLeakTest(T_ZFObject, ...) \
    zflockfree_zflineReleaseWithoutLeakTest(zflockfree_zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__))

// ============================================================
template<typename T_ZFObject>
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zfblockedAllocContainer
{
public:
    _ZFP_zfblockedAllocContainer(ZF_IN T_ZFObject *obj,
                                 ZF_IN const zfcharA *callerFile,
                                 ZF_IN const zfcharA *callerFunction,
                                 ZF_IN zfindex callerLine)
    : obj(obj)
    , callerFile(callerFile)
    , callerFunction(callerFunction)
    , callerLine(callerLine)
    {
    }
    ~_ZFP_zfblockedAllocContainer(void)
    {
        ZFCoreMutexLock();
        zflockfree_ZFLeakTestLogBeforeReleaseVerbose(this->obj,
            this->callerFile, this->callerFunction, this->callerLine);
        zflockfree_zfReleaseWithoutLeakTest(this->obj);
        ZFCoreMutexUnlock();
    }
public:
    T_ZFObject *obj;
private:
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};
template<typename T_ZFObject>
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zflockfree_zfblockedAllocContainer
{
public:
    _ZFP_zflockfree_zfblockedAllocContainer(ZF_IN T_ZFObject *obj,
                                            ZF_IN const zfcharA *callerFile,
                                            ZF_IN const zfcharA *callerFunction,
                                            ZF_IN zfindex callerLine)
    : obj(obj)
    , callerFile(callerFile)
    , callerFunction(callerFunction)
    , callerLine(callerLine)
    {
    }
    ~_ZFP_zflockfree_zfblockedAllocContainer(void)
    {
        zflockfree_ZFLeakTestLogBeforeReleaseVerbose(this->obj,
            this->callerFile, this->callerFunction, this->callerLine);
        zflockfree_zfReleaseWithoutLeakTest(this->obj);
    }
public:
    T_ZFObject *obj;
private:
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};
template<typename T_ZFObject>
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zfblockedAllocContainerWithoutLeakTest
{
public:
    _ZFP_zfblockedAllocContainerWithoutLeakTest(ZF_IN T_ZFObject *obj)
    : obj(obj)
    {
    }
    ~_ZFP_zfblockedAllocContainerWithoutLeakTest(void)
    {
        zfReleaseWithoutLeakTest(this->obj);
    }
public:
    T_ZFObject *obj;
};
template<typename T_ZFObject>
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zflockfree_zfblockedAllocContainerWithoutLeakTest
{
public:
    _ZFP_zflockfree_zfblockedAllocContainerWithoutLeakTest(ZF_IN T_ZFObject *obj)
    : obj(obj)
    {
    }
    ~_ZFP_zflockfree_zfblockedAllocContainerWithoutLeakTest(void)
    {
        zflockfree_zfReleaseWithoutLeakTest(this->obj);
    }
public:
    T_ZFObject *obj;
};
/**
 * @brief alloc a ZFObject looks like on a stack
 *
 * @code
 *   ZFString *saved = zfnull;
 *   {
 *       zfblockedAlloc(
 *           ZFString, // object's type
 *           s, // object's name
 *           zfText("init value") // params passed to object's objectOnInit
 *       );
 *       s->objectInfo(); // access like normal ZFObject
 *       saved = zfRetain(s); // you may retain it and save it
 *   } // s would be released after this block
 *   saved->objectInfo(); // s is still alive since we retained it in the block
 *   zfRelease(saved); // release it
 * @endcode
 */
#define zfblockedAlloc(T_ZFObject, name, ...) \
    _ZFP_zfblockedAllocContainer<T_ZFObject> \
        ZFM_CAT(_ZFP_zfblockedAlloc_hold_, name) (zfAlloc(T_ZFObject, ##__VA_ARGS__), \
            ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE); \
    T_ZFObject *name = (ZFM_CAT(_ZFP_zfblockedAlloc_hold_, name).obj)
/** @brief no lock version of #zfblockedAlloc, use with causion */
#define zflockfree_zfblockedAlloc(T_ZFObject, name, ...) \
    _ZFP_zflockfree_zfblockedAllocContainer<T_ZFObject> \
        ZFM_CAT(_ZFP_zfblockedAlloc_hold_, name) (zflockfree_zfAlloc(T_ZFObject, ##__VA_ARGS__), \
            ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE); \
    T_ZFObject *name = (ZFM_CAT(_ZFP_zfblockedAlloc_hold_, name).obj)
/**
 * @brief see #zfblockedAlloc
 */
#define zfblockedAllocWithoutLeakTest(T_ZFObject, name, ...) \
    _ZFP_zfblockedAllocContainerWithoutLeakTest<T_ZFObject> \
    ZFM_CAT(_ZFP_zfblockedAlloc_hold_, name) (zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__)); \
    T_ZFObject *name = (ZFM_CAT(_ZFP_zfblockedAlloc_hold_, name).obj)
/** @brief no lock version of #zfblockedAllocWithoutLeakTest, use with causion */
#define zflockfree_zfblockedAllocWithoutLeakTest(T_ZFObject, name, ...) \
    _ZFP_zflockfree_zfblockedAllocContainerWithoutLeakTest<T_ZFObject> \
    ZFM_CAT(_ZFP_zfblockedAlloc_hold_, name) (zflockfree_zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__)); \
    T_ZFObject *name = (ZFM_CAT(_ZFP_zfblockedAlloc_hold_, name).obj)

// ============================================================
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zfblockedReleaseContainer
{
public:
    template<typename T_ZFObject>
    _ZFP_zfblockedReleaseContainer(ZF_IN T_ZFObject *obj,
                                   ZF_IN const zfcharA *callerFile,
                                   ZF_IN const zfcharA *callerFunction,
                                   ZF_IN zfindex callerLine)
    : obj(ZFCastZFObjectUnchecked(ZFObject *, obj))
    , callerFile(callerFile)
    , callerFunction(callerFunction)
    , callerLine(callerLine)
    {
    }
    ~_ZFP_zfblockedReleaseContainer(void)
    {
        ZFCoreMutexLock();
        zflockfree_ZFLeakTestLogBeforeReleaseVerbose(this->obj,
            this->callerFile, this->callerFunction, this->callerLine);
        zflockfree_zfReleaseWithoutLeakTest(this->obj);
        ZFCoreMutexUnlock();
    }
private:
    ZFObject *obj;
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zflockfree_zfblockedReleaseContainer
{
public:
    template<typename T_ZFObject>
    _ZFP_zflockfree_zfblockedReleaseContainer(ZF_IN T_ZFObject *obj,
                                              ZF_IN const zfcharA *callerFile,
                                              ZF_IN const zfcharA *callerFunction,
                                              ZF_IN zfindex callerLine)
    : obj(ZFCastZFObjectUnchecked(ZFObject *, obj))
    , callerFile(callerFile)
    , callerFunction(callerFunction)
    , callerLine(callerLine)
    {
    }
    ~_ZFP_zflockfree_zfblockedReleaseContainer(void)
    {
        zflockfree_ZFLeakTestLogBeforeReleaseVerbose(this->obj,
            this->callerFile, this->callerFunction, this->callerLine);
        zflockfree_zfReleaseWithoutLeakTest(this->obj);
    }
private:
    ZFObject *obj;
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zfblockedReleaseContainerWithoutLeakTest
{
public:
    template<typename T_ZFObject>
    _ZFP_zfblockedReleaseContainerWithoutLeakTest(ZF_IN T_ZFObject *obj)
    : obj(ZFCastZFObjectUnchecked(ZFObject *, obj))
    {
    }
    ~_ZFP_zfblockedReleaseContainerWithoutLeakTest(void)
    {
        zfReleaseWithoutLeakTest(this->obj);
    }
private:
    ZFObject *obj;
};
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zflockfree_zfblockedReleaseContainerWithoutLeakTest
{
public:
    template<typename T_ZFObject>
    _ZFP_zflockfree_zfblockedReleaseContainerWithoutLeakTest(ZF_IN T_ZFObject *obj)
    : obj(ZFCastZFObjectUnchecked(ZFObject *, obj))
    {
    }
    ~_ZFP_zflockfree_zfblockedReleaseContainerWithoutLeakTest(void)
    {
        zflockfree_zfReleaseWithoutLeakTest(this->obj);
    }
private:
    ZFObject *obj;
};
/**
 * @brief release the object after nearest block
 *
 * @code
 *   { // code block
 *       ZFObject *obj = ...;
 *       zfblockedRelease(obj); // mark release, must be single line
 *       return ; // safe to return, auto released after exiting code block
 *   } // obj would be released after here
 * @endcode
 */
#define zfblockedRelease(obj) \
    _ZFP_zfblockedReleaseContainer ZFUniqueName(zfblockedRelease) (obj, \
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/** @brief no lock version of #zfblockedRelease, use with causion */
#define zflockfree_zfblockedRelease(obj) \
    _ZFP_zflockfree_zfblockedReleaseContainer ZFUniqueName(zfblockedRelease) (obj, \
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/**
 * @brief see #zfblockedRelease
 */
#define zfblockedReleaseWithoutLeakTest(obj) \
    _ZFP_zfblockedReleaseContainerWithoutLeakTest ZFUniqueName(zfblockedRelease) (obj)
/** @brief no lock version of #zfblockedReleaseWithoutLeakTest, use with causion */
#define zflockfree_zfblockedReleaseWithoutLeakTest(obj) \
    _ZFP_zflockfree_zfblockedReleaseContainerWithoutLeakTest ZFUniqueName(zfblockedRelease) (obj)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectSmartPointerDef_h_

