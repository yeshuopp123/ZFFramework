/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file zfautoObjectDef.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_zfautoObjectDef_h_
#define _ZFI_zfautoObjectDef_h_

#include "ZFObjectDef.h"
#include "ZFObjectRetainDef.h"
#include "ZFAnyDef.h"
#include "zfautoObjectFwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// sub type impl
#define _ZFP_ZFAUTOOBJECT_DECLARE_BEGIN(createFuncName, retainAction, releaseAction) \
    /** @cond ZFPrivateDoc */ \
    zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfautoObjectPrivate_##createFuncName : zfextendsNotPOD _ZFP_zfautoObjectPrivateBase \
    { \
    public: \
        zfoverride \
        virtual _ZFP_zfautoObjectPrivateBase *onRetain(void) \
        { \
            if(this->_ZFP_obj) \
            { \
                ZFObject *p = this->_ZFP_obj; \
                { \
                    retainAction \
                } \
            } \
            return this; \
        } \
        zfoverride \
        virtual void onRelease(void) \
        { \
            if(this->_ZFP_obj) \
            { \
                ZFObject *p = this->_ZFP_obj; \
                this->_ZFP_obj = zfnull; \
                { \
                    releaseAction \
                } \
            } \
        }
#define _ZFP_ZFAUTOOBJECT_DECLARE_END(createFuncName) \
    }; \
    /** @endcond */

/**
 * @brief for impl to supply custom zfautoObject impl
 *
 * usage:
 * @code
 *   // declare in header file
 *   // params in retainAction and releaseAction:
 *   // ZFObject *p
 *   // const zfcharA *callerFile
 *   // const zfcharA *callerFunction
 *   // zfindex callerLine
 *   ZFAUTOOBJECT_DECLARE_WITH_CALLER_INFO(myCreateFunc, {
 *           zfLogTrimT() << callerFile << callerFunction << callerLine;
 *           myRetain(p);
 *       }, {
 *           zfLogTrimT() << callerFile << callerFunction << callerLine;
 *           myRelease(p);
 *       })
 *
 *   // use it
 *   zfautoObject obj = myCreateFunc(someObject);
 * @endcode
 */
#define ZFAUTOOBJECT_DECLARE(createFuncName, retainAction, releaseAction) \
    _ZFP_ZFAUTOOBJECT_DECLARE_BEGIN(createFuncName, retainAction, releaseAction) \
    public: \
        _ZFP_zfautoObjectPrivate_##createFuncName(ZF_IN ZFObject *obj) \
        : _ZFP_zfautoObjectPrivateBase(obj) \
        { \
        } \
    _ZFP_ZFAUTOOBJECT_DECLARE_END(createFuncName) \
    inline zfautoObject createFuncName(ZF_IN ZFObject *obj) \
    { \
        return zfautoObject(zfnew(_ZFP_zfautoObjectPrivate_##createFuncName, obj)->onRetain()); \
    }
/**
 * @brief see #ZFAUTOOBJECT_DECLARE
 */
#define ZFAUTOOBJECT_DECLARE_WITH_CALLER_INFO(createFuncName, retainAction, releaseAction) \
    _ZFP_ZFAUTOOBJECT_DECLARE_BEGIN(createFuncName, retainAction, releaseAction) \
    public: \
        const zfcharA *callerFile; \
        const zfcharA *callerFunction; \
        zfindex callerLine; \
    public: \
        _ZFP_zfautoObjectPrivate_##createFuncName(ZF_IN ZFObject *obj, \
                                                  ZF_IN const zfcharA *callerFile, \
                                                  ZF_IN const zfcharA *callerFunction, \
                                                  ZF_IN zfindex callerLine) \
        : _ZFP_zfautoObjectPrivateBase(obj) \
        , callerFile(callerFile) \
        , callerFunction(callerFunction) \
        , callerLine(callerLine) \
        { \
        } \
    _ZFP_ZFAUTOOBJECT_DECLARE_END(createFuncName) \
    inline zfautoObject createFuncName(ZF_IN ZFObject *obj, \
                                       ZF_IN const zfcharA *callerFile, \
                                       ZF_IN const zfcharA *callerFunction, \
                                       ZF_IN zfindex callerLine) \
    { \
        return zfautoObject(zfnew(_ZFP_zfautoObjectPrivate_##createFuncName, obj, callerFile, callerFunction, callerLine)->onRetain()); \
    }

// ============================================================
/** @brief see #zfautoObject */
ZFAUTOOBJECT_DECLARE(zfautoObjectCreateWithoutLeakTest, {
        zfRetainWithoutLeakTest(p);
    }, {
        zfReleaseWithoutLeakTest(p);
    })

/** @brief see #zfautoObject */
ZFAUTOOBJECT_DECLARE(zflockfree_zfautoObjectCreateWithoutLeakTest, {
        zflockfree_zfRetainWithoutLeakTest(p);
    }, {
        zfReleaseWithoutLeakTest(p);
    })

// ============================================================
/** @brief see #zfautoObject */
ZFAUTOOBJECT_DECLARE_WITH_CALLER_INFO(zfautoObjectCreateVerbose, {
        ZFCoreMutexLock();
        zflockfree_zfRetainWithoutLeakTest(p);
        zflockfree_ZFLeakTestLogAfterRetainVerbose(p, callerFile, callerFunction, callerLine);
        ZFCoreMutexUnlock();
    }, {
        ZFCoreMutexLock();
        zflockfree_ZFLeakTestLogBeforeReleaseVerbose(p, callerFile, callerFunction, callerLine);
        zflockfree_zfReleaseWithoutLeakTest(p);
        ZFCoreMutexUnlock();
    })
/** @brief see #zfautoObject */
#define zfautoObjectCreate(obj) \
    zfautoObjectCreateVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

/** @brief see #zfautoObject */
ZFAUTOOBJECT_DECLARE_WITH_CALLER_INFO(zflockfree_zfautoObjectCreateVerbose, {
        zflockfree_zfRetainWithoutLeakTest(p);
        zflockfree_ZFLeakTestLogAfterRetainVerbose(p, callerFile, callerFunction, callerLine);
    }, {
        ZFCoreMutexLock();
        zflockfree_ZFLeakTestLogBeforeReleaseVerbose(p, callerFile, callerFunction, callerLine);
        zflockfree_zfReleaseWithoutLeakTest(p);
        ZFCoreMutexUnlock();
    })
/** @brief see #zfautoObject */
#define zflockfree_zfautoObjectCreate(obj) \
    zflockfree_zfautoObjectCreateVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

// ============================================================
/** @brief see #zfautoObject */
ZFAUTOOBJECT_DECLARE(zfautoObjectCreateMarkCached, {
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
/** @brief see #zfautoObject */
ZFAUTOOBJECT_DECLARE(zflockfree_zfautoObjectCreateMarkCached, {
        zflockfree_zfRetainWithoutLeakTest(p);
        p->objectCachedSet(zftrue);
    }, {
        ZFCoreMutexLock();
        p->objectCachedSet(zffalse);
        zflockfree_zfReleaseWithoutLeakTest(p);
        ZFCoreMutexUnlock();
    })

// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
zfautoObject &zfautoObject::operator = (ZF_IN T_ZFObject const &p)
{
    return this->operator = (zfautoObjectCreate(ZFCastZFObjectUnchecked(ZFObject *, p)));
}
/** @endcond */

// ============================================================
// ZFCastZFObject fix for ZFAny
inline ZFObject *_ZFP_ZFCastZFObjectFromUnknown(zfautoObject const &obj)
{
    return obj.toObject();
}
inline void _ZFP_ZFCastZFObjectToUnknown(zfautoObject &ret,
                                         ZFObject * const &obj)
{
    ret = zfautoObjectCreateVerbose(obj, ZF_CALLER_FILE, zfTextA("ZFCastZFObject"), ZF_CALLER_LINE);
}

inline ZFObject *_ZFP_ZFCastZFObjectFromUnknownUnchecked(zfautoObject const &obj)
{
    return obj.toObject();
}
inline void _ZFP_ZFCastZFObjectToUnknownUnchecked(zfautoObject &ret,
                                                  ZFObject * const &obj)
{
    ret = zfautoObjectCreateVerbose(obj, ZF_CALLER_FILE, zfTextA("ZFCastZFObjectUnchecked"), ZF_CALLER_LINE);
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfautoObjectDef_h_

