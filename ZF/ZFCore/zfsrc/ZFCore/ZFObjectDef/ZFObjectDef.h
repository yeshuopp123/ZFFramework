/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectDef.h
 * @brief base class of all objects
 */

#ifndef _ZFI_ZFObjectDef_h_
#define _ZFI_ZFObjectDef_h_

#include "ZFObjectClassTypeFwd.h"
#include "ZFObjectCastDef.h"
#include "ZFAnyDef.h"
#include "ZFCallbackDef.h"
#include "ZFClassDef.h"
#include "ZFMethodDef.h"
#include "ZFObjectClassTypeDef.h"
#include "ZFObjectDeclareDef.h"
#include "ZFObjectInterfaceDef.h"
#include "ZFPropertyDef.h"
#include "zfautoObjectFwd.h"
#include "ZFObjectObserverDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief left bracket when output ZFObject's info
 * @see ZFObject::objectInfo
 */
#define ZFTOKEN_ZFObjectInfoLeft zfText("<")
/**
 * @brief right bracket when output ZFObject's info
 * @see ZFObject::objectInfo
 */
#define ZFTOKEN_ZFObjectInfoRight zfText(">")

/**
 * @brief lock object for mutex's implementation
 */
zfclassNotPOD ZF_ENV_EXPORT ZFObjectMutexImpl : zfextendsNotPOD ZFCoreMutexImpl
{
public:
    /**
     * @brief lock (must be reentrant)
     */
    virtual void mutexImplLock(void) zfpurevirtual;
    /**
     * @brief unlock
     */
    virtual void mutexImplUnlock(void) zfpurevirtual;
    /**
     * @brief try lock
     */
    virtual zfbool mutexImplTryLock(void) zfpurevirtual;
};

/**
 * @brief see #ZFObjectMutexImplSet
 */
typedef zfbool (*ZFObjectMutexImplCheckCallback)(void);
/**
 * @brief see #ZFObjectMutexImplSet
 */
typedef ZFObjectMutexImpl *(*ZFObjectMutexImplInitCallback)(void);
/**
 * @brief see #ZFObjectMutexImplSet
 */
typedef void (*ZFObjectMutexImplCleanupCallback)(ZF_IN ZFObjectMutexImpl *mutexImpl);
/**
 * @brief change the implementation for #zfsynchronizedObject
 *
 * the sync for ZFObject is an optional protocol,
 * each newly allocated object would have #ZFObjectMutexImplCheckCallback been called
 * to check whether the protocol is available,
 * if not, any other sync function to this object is disabled automatically,
 * otherwise, a mutex impl would be created by #ZFObjectMutexImplInitCallback when you
 * request to the sync logic,
 * and would be cleaned up during object's deallocation
 */
extern ZF_ENV_EXPORT void ZFObjectMutexImplSet(ZF_IN_OPT ZFObjectMutexImplCheckCallback checkCallback = zfnull,
                                               ZF_IN_OPT ZFObjectMutexImplInitCallback initCallback = zfnull,
                                               ZF_IN_OPT ZFObjectMutexImplCleanupCallback cleanupCallback = zfnull);

// ============================================================
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFObjectDummyParent
{
public:
    static const ZFClass *ClassData(void)
    {
        return zfnull;
    }
    static void _ZFP_ZFObjectInitImplementationList(ZFClass *cls) {}
};
zfclassFwd _ZFP_ZFObjectObserverHolder;
zfclassFwd _ZFP_ZFObjectPrivate;
zfclassFwd ZFObjectHolder;
/**
 * @brief base class of all objects
 *
 * -  ZFObject is a Object-C-like class, using objectRetainCount to manage memory:
 *   @code
 *     ZFObject *myObj = zfAlloc(ZFObject); // objectRetainCount = 1
 *     anotherObj = zfRetain(myObj);        // objectRetainCount = 2
 *     zfRelease(myObj);                    // objectRetainCount = 1
 *     zfRelease(anotherObject);            // objectRetainCount = 0, delete
 *   @endcode
 *   you should always use #zfAlloc to create a ZFObject or its subclass,
 *   and use #zfRetain and #zfRelease to manage memory
 *   @see zfRetain, zfRelease, ZFAutoReleasePool
 *
 *   ADVANCED:\n
 *   we are trying hard to simulate the autorelease in Object-C world:
 *   -  #zfautoRelease:
 *     the most powerful and most similar one to autorelease in Object-C,
 *     however it depends on ZFThread,
 *     so most of functions won't use this to return a autoreleased object,
 *     see #zfautoRelease for more info
 *   -  #ZFAutoReleasePool:
 *     similar to NSAutoReleasePool, release after pool release,
 *     zfautoRelease depends on it, and all ZFThread has a pool internally
 *   -  #zflineRelease:
 *     release after code line end, usually used for temporary object
 *   -  #zflineAlloc:
 *     same as zflineRelease(zfAlloc(...)) defined for short
 *   -  #zfblockedRelease:
 *     release after nearest code block
 *   -  #zfblockedAlloc:
 *     declare a ZFObject which looks like allocated in stack,
 *     would be released after nearest code block
 *   -  #zfautoObject:
 *     declare a holder object which would release its content ZFObject when destroyed,
 *     much like the auto_ptr in C++ world
 * -  when inherit from ZFObject,
 *   you must add ZFOBJECT_DECLARE or ZFOBJECT_DECLARE_ABSTRACT to your subclass,
 *   and use zfextends:
 *   @code
 *     zfclass YourClassChild : zfextends YourClassBase
 *     {
 *         ZFOBJECT_DECLARE(YourClassChild, YourClassBase)
 *         // or ZFOBJECT_DECLARE_ABSTRACT if YourClassChild is abstract class
 *     };
 *   @endcode
 *   and, no multiple inheritance is allowed while using ZFObject\n
 *   additionally, you should add #ZFOBJECT_REGISTER if you want the ZFClass map function
 *   @see ZFOBJECT_DECLARE, zfextends
 * -  embeded class can be declared,
 *   however, you must make sure it's name is unique,
 *   we doesn't support class names with namespace
 *   @code
 *     zfclass YourOutterClass : zfextends ZFObject
 *     {
 *         ZFOBJECT_DECLARE(YourOutterClass, ZFObject)
 *
 *         // inner class is allowed,
 *         // but you must ensure it's class name is unique,
 *         // since no namespace qualifier is supported,
 *         // its full class name is "YourInnerClass" instead of "YourOutterClass::YourInnerClass"
 *         zfclass YourInnerClass : zfextends ZFObject
 *         {
 *             ZFOBJECT_DECLARE(YourInnerClass, ZFObject)
 *         };
 *     };
 *   @endcode
 * -  when inherit from ZFObject,
 *   member method is recommended declared as virtual method,
 *   unless you do want to prevent subclass to override,
 *   at this case, use #zffinal to mark the method is final and subclass should not try to override or shadow it
 * -  ZFObject use #objectOnInit and #objectOnDealloc to init and destory object,
 *   instead of constructors and destructors,
 *   for how to use it in subclass, see #objectOnInit
 * -  while override member method of ZFObject type,
 *   you should use zfsuper to call super's method:
 *   @code
 *     zfclass YourClass : zfextends Parent
 *     {
 *         ZFOBJECT_DECLARE(YourClass, Parent)
 *     public:
 *         zfoverride
 *         virtual void funcInParent(void)
 *         {
 *             // override and call super's method by zfsuper
 *             zfsuper::funcInParent();
 *         }
 *     };
 *   @endcode
 *   if overrided method is declared in super interface,
 *   you should use zfsuperI:
 *   @code
 *     zfclass Parent : zfextends ZFObject, zfimplements ParentInterface0
 *     {
 *         ZFOBJECT_DECLARE(Parent, ZFObject)
 *         ZFIMPLEMENTS_DECLARE(ParentInterface0)
 *     public:
 *         zfoverride
 *         virtual void funcInParentInterface0(void)
 *         {
 *             // would call ParentInterface0::funcInParentInterface0
 *             zfsuperI(ParentInterface0)::funcInParentInterface0();
 *         }
 *     };
 *     zfclass YourClass : zfextends Parent, zfimplements ParentInterface1
 *     {
 *         ZFOBJECT_DECLARE(YourClass, Parent)
 *         ZFIMPLEMENTS_DECLARE(ParentInterface1)
 *     public:
 *         zfoverride
 *         virtual void funcInParentInterface0(void)
 *         {
 *             // would call Parent::funcInParentInterface0
 *             zfsuperI(ParentInterface0)::funcInParentInterface0();
 *         }
 *         zfoverride
 *         virtual void funcInParentInterface1(void)
 *         {
 *             // would call ParentInterface1::funcInParentInterface1
 *             zfsuperI(ParentInterface1)::funcInParentInterface1();
 *         }
 *     };
 *   @endcode
 * -  due to some limitation, you should always use ZFObject as a pointer
 *   but without const qualifier,
 *   for const operations, use the const version objects,
 *   such as ZFArray instead of ZFArrayEditable
 *   for const arrays
 * -  you can access class info by:
 *   @code
 *     YourClass::ClassData();
 *     yourClassInstance->classData();
 *     ZFClass::classForName(name);
 *   @endcode
 * -  ZFObject is synchronizable object, you can make it mutable by:
 *   @code
 *     zfsynchronized(obj)
 *     {
 *         // mutable operation
 *     }
 *   @endcode
 *   for advanced mutable operation, use ZFMutex instead\n
 *   for global mutable operation, use #zfsynchronize instead
 *
 * \n
 * ADVANCED:\n
 * we use macros everywhere to achieve goals,
 * there are many types and functions and whatever-it-is,
 * which are generated by macros and are private,
 * you should never ever access it in your code\n
 * here's a list or them:
 * -  "_ZFP_*": shows it's private (maybe types or functions or anything)
 * -  "_ZFI_*": conditional code wrappers
 * -  "_ZFT_*": conditional type defines,
 *   must not be accessed by codes,
 *   but can be modified by adding precompile flags
 *   (usually have no need, and modify at your own risk)
 */
zfclass ZF_ENV_EXPORT ZFObject
{
    _ZFP_ZFOBJECT_DECLARE(ZFObject, _ZFP_ZFObjectDummyParent)
    _ZFP_ZFOBJECT_DECLARE_OBJECT(ZFObject, _ZFP_ZFObjectDummyParent)

public:
    static void _ZFP_ZFObjectInitImplementationList(ZFClass *cls) {}
    virtual inline void _ZFP_ZFObject_interfaceOnInit(void) {}
    virtual inline void _ZFP_ZFObject_interfaceOnDealloc(void) {}
protected:
    /** @cond ZFPrivateDoc */
    ZFObject(void)
    : d(zfnull)
    {
    }
    virtual ~ZFObject(void)
    {
    }
    /** @endcond */

    // ============================================================
    // observer events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when object allocated (after #objectOnInitFinish\n
     * this event is only designed for convenient and for debug use only
     */
    ZFOBSERVER_EVENT(ObjectAfterAlloc)
    /**
     * @brief see #ZFObject::observerNotify
     */
    ZFOBSERVER_EVENT(ObjectBeforeDealloc)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is a #ZFPointerHolder holds a (const ZFProperty *) value\n
     * see #EventObjectPropertyValueAfterChange for more info
     * @note won't be called when owner object deallocated
     */
    ZFOBSERVER_EVENT(ObjectPropertyValueBeforeChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is a #ZFPointerHolder holds a (const ZFProperty *) value
     * @note called when property's setter is called,
     *   whether the property value really change or not,
     *   is not ensured
     * @note won't be called when property is first time accessed and initialized
     */
    ZFOBSERVER_EVENT(ObjectPropertyValueAfterChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * see #objectCachedSet
     */
    ZFOBSERVER_EVENT(ObjectCachedOnChange)

public:
    /**
     * @brief return the object's retain count
     * @see zfRetain, zfRelease
     */
    virtual zfindex objectRetainCount(void);

    /**
     * @brief return an object holder that hold this object without affecting retain count
     */
    virtual ZFObjectHolder *objectHolder(void);

    /**
     * @brief return a short string describe the object instance
     *
     * may looks like this:
     *   ClassName(0x123456)
     * @see objectInfo
     */
    virtual void objectInfoOfInstanceT(ZF_IN_OUT zfstring &ret);
    /** @brief see #objectInfoOfInstanceT */
    virtual inline zfstring objectInfoOfInstance(void)
    {
        zfstring ret;
        this->objectInfoOfInstanceT(ret);
        return ret;
    }

    /**
     * @brief for debug use only
     *
     * may looks like this:
     *   <ClassName(0x123456) additional debug info>
     * @see objectInfo
     */
    virtual void objectInfoForDebugT(ZF_IN_OUT zfstring &ret);
    /** @brief see #objectInfoForDebugT */
    virtual inline zfstring objectInfoForDebug(void)
    {
        zfstring ret;
        this->objectInfoForDebugT(ret);
        return ret;
    }

    /**
     * @brief return a short string describe the object
     */
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);
    /** @brief see #objectInfoT */
    virtual inline zfstring objectInfo(void)
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

protected:
    /**
     * @brief called by #objectInfoT to append object info as a chain
     */
    virtual inline void objectInfoOnAppendTokenLeft(ZF_IN_OUT zfstring &ret)
    {
        ret += ZFTOKEN_ZFObjectInfoLeft;
    }
    /**
     * @brief called by #objectInfoT to append object info as a chain
     */
    virtual inline void objectInfoOnAppendTokenRight(ZF_IN_OUT zfstring &ret)
    {
        ret += ZFTOKEN_ZFObjectInfoRight;
    }
    /**
     * @brief called by #objectInfoT to append object info as a chain
     */
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief get hash of this object
     *
     * by default, this method would return a hash value of the instance's pointer\n
     * \n
     * if you override this method, you must ensure
     * two objects have same hash if they are regarded as same
     * (i.e. #objectCompare return #ZFCompareTheSame)\n
     * this method may or may not be called frequently,
     * you should always try to make the implementation
     * have good performance
     */
    virtual zfidentity objectHash(void);
    /**
     * @brief compare with anotherObj
     * @return ZFCompareTheSame if this == anotherObj\n
     *         ZFCompareUncomparable otherwise
     * @warning if your override #objectCompare,
     *   you must also override #objectHash,
     *   and follow the rules described in #objectHash
     */
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief see #tagSet, true if this object has tag,
     *   and tag can be checked by #tagGetAllKeyValue
     */
    virtual zfbool tagHasSet(void);
    /**
     * @brief used to hold a object for app's use, auto retained
     *
     * replace if existing, remove if tag is null
     */
    virtual void tagSet(ZF_IN const zfchar *key,
                        ZF_IN ZFObject *tag,
                        ZF_IN_OPT zfbool autoMarkCached = zffalse);
    /**
     * @brief #tagSet and automatically mark tag as #objectCached
     */
    virtual inline void tagSetMarkCached(ZF_IN const zfchar *key,
                                         ZF_IN ZFObject *tag)
    {
        this->tagSet(key, tag, zftrue);
    }
    /**
     * @brief see #tagSet
     */
    virtual ZFObject *tagGet(ZF_IN const zfchar *key);
    /**
     * @brief see #tagSet
     */
    template<typename T_ZFObject>
    T_ZFObject tagGet(ZF_IN const zfchar *key)
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->tagGet(key));
    }
    /**
     * @brief get all key value
     */
    virtual void tagGetAllKeyValue(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &allKey,
                                   ZF_IN_OUT ZFCoreArrayPOD<ZFObject *> &allValue);
    /**
     * @brief remove tag, same as set tag to null
     */
    virtual inline void tagRemove(ZF_IN const zfchar *key)
    {
        this->tagSet(key, zfnull);
    }
    /**
     * @brief remove tag, return removed tag or #zfautoObjectNull if not exist
     */
    virtual zfautoObject tagRemoveAndGet(ZF_IN const zfchar *key);
    /**
     * @brief remove all tag
     *
     * @note it's unsafe to remove all tag manually,
     *   which may break unrelated modules' additional logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    virtual void tagRemoveAll(void);

public:
    /**
     * @brief for debug use only, set a hint string for this object,
     *   set null to remove
     *
     * recommended to use chars only if #zfsCoreZ2A supported
     */
    virtual void objectDebugInfoSet(ZF_IN const zfchar *debugInfo);
    /**
     * @brief see #objectDebugInfoSet
     */
    virtual const zfchar *objectDebugInfo(void);

public:
    /** @brief see #ZFObject::observerNotify */
    zffinal zfclassLikePOD ZF_ENV_EXPORT ObserverAddParam
    {
        ZFCORE_PARAM_DECLARE_SELF(ObserverAddParam)

        /** @brief see #ZFObject::observerNotify */
        ZFCORE_PARAM_WITH_INIT(zfidentity, eventId, zfidentityInvalid)

        /** @brief see #ZFObject::observerNotify */
        ZFCORE_PARAM(ZFListener, observer)

        /** @brief see #ZFObject::observerNotify */
        ZFCORE_PARAM_WITH_INIT(ZFObject *, userData, zfnull)

        /** @brief see #ZFObject::observerNotify */
        ZFCORE_PARAM_WITH_INIT(ZFObject *, owner, zfnull)

        /** @brief see #ZFObject::observerNotify */
        ZFCORE_PARAM_WITH_INIT(zfbool, autoRemoveAfterActivate, zffalse)

        /** @brief see #ZFObject::observerNotify */
        ZFCORE_PARAM_WITH_INIT(ZFLevel, observerLevel, ZFLevelAppNormal)
    };
    /**
     * @brief see #observerNotify
     */
    zffinal void observerAdd(ZF_IN const zfidentity &eventId,
                             ZF_IN const ZFListener &observer,
                             ZF_IN_OPT ZFObject *userData = zfnull,
                             ZF_IN_OPT ZFObject *owner = zfnull,
                             ZF_IN_OPT zfbool autoRemoveAfterActivate = zffalse,
                             ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal);
    /**
     * @brief see #observerNotify
     */
    zffinal inline void observerAdd(ZF_IN const ZFObject::ObserverAddParam &param)
    {
        this->observerAdd(param.eventId(), param.observer(), param.userData(), param.owner(), param.autoRemoveAfterActivate(), param.observerLevel());
    }
    /**
     * @brief see #observerNotify
     */
    zffinal void observerRemove(ZF_IN const zfidentity &eventId,
                                ZF_IN const ZFListener &callback);
    /**
     * @brief see #observerNotify
     */
    zffinal void observerRemove(ZF_IN const zfidentity &eventId,
                                ZF_IN const ZFListener &callback,
                                ZF_IN ZFObject *userData,
                                ZF_IN_OPT ZFComparer<ZFObject *>::Comparer userDataComparer = ZFComparerCheckEqualOnly);
    /**
     * @brief see #observerNotify
     */
    zffinal void observerRemoveByOwner(ZF_IN ZFObject *owner);
    /**
     * @brief see #observerNotify
     *
     * @note it's unsafe to remove all observer manually,
     *   which may break unrelated modules' observer logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    zffinal void observerRemoveAll(ZF_IN const zfidentity &eventId);
    /**
     * @brief see #observerNotify
     *
     * @note it's unsafe to remove all observer manually,
     *   which may break unrelated modules' observer logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    zffinal void observerRemoveAll(void);
    /**
     * @brief true if any observer has been added
     */
    zffinal zfbool observerHasAdd(void);
    /**
     * @brief true if any observer with eventId has been added
     */
    zffinal zfbool observerHasAdd(ZF_IN const zfidentity &eventId);
    /**
     * @brief notify the observer with eventId
     *
     * the #ZFListener would be executed\n
     * it's ensured the first added observer would be executed first\n
     * use #ZFGlobalEventCenter or #ZFObjectGlobalEventObserverAdd for global observer\n
     * for instance observer:
     * @code
     *   zfclass YourClass
     *   {
     *       // declare your event here,
     *       // which would generate a method:
     *       //   static const zfchar *EventYourEvent(void)
     *       ZFOBSERVER_EVENT(YourEvent)
     *   };
     *   // notify using declared event
     *   yourClass->observerNotify(YourClass::EventYourEvent(), params...);
     * @endcode
     * @note observers would be compared
     *   by #ZFCallback::objectCompareByInstance when add or remove
     *
     * \n
     * due to lack of lambda capture support,
     * it's hard to pass param outside of a standalone callback into it,
     * so it's recommended to pass by attaching tags to the userData:
     * @code
     *   ZFObject *paramToPass = ...;
     *   ZFObject *userData = ...;
     *   // store param as tag
     *   // make sure the tag name is unique and would be removed properly
     *   userData->tagSet(paramToPass, uniqueTagName);
     *   ZFListener yourStandaloneListener = SOME_CREATE_LOGIC {
     *       // here you can used the passed param
     *       ZFObject *paramPassed = userData->tagGet(uniqueTagName);
     *   };
     *   someEventSender->observerAdd(someEvent, yourStandaloneListener, userData);
     * @endcode
     */
    zffinal inline void observerNotify(ZF_IN const zfidentity &eventId,
                                       ZF_IN_OPT ZFObject *param0 = zfnull,
                                       ZF_IN_OPT ZFObject *param1 = zfnull)
    {
        this->observerNotifyWithCustomSender(this, eventId, param0, param1);
    }
    /**
     * @brief see #observerNotify
     */
    zffinal void observerNotifyWithCustomSender(ZF_IN ZFObject *customSender,
                                                ZF_IN const zfidentity &eventId,
                                                ZF_IN_OPT ZFObject *param0 = zfnull,
                                                ZF_IN_OPT ZFObject *param1 = zfnull);
    /**
     * @brief get info of all observers currently registered, for debug use only
     */
    zffinal void observerInfo(ZF_OUT zfstring &ret);
    /** @brief see #observerInfo */
    zffinal zfstring observerInfo(void)
    {
        zfstring ret;
        this->observerInfo(ret);
        return ret;
    }

protected:
    /**
     * @brief called when add first observer
     */
    virtual inline void observerOnAddFirst(ZF_IN const zfidentity &eventId)
    {
    }
    /**
     * @brief called when remove last observer
     */
    virtual inline void observerOnRemoveLast(ZF_IN const zfidentity &eventId)
    {
    }
    /**
     * @brief called when add observer
     */
    virtual inline void observerOnAdd(ZF_IN const zfidentity &eventId,
                                      ZF_IN const ZFListener &callback)
    {
    }
    /**
     * @brief called when remove observer
     */
    virtual inline void observerOnRemove(ZF_IN const zfidentity &eventId,
                                         ZF_IN const ZFListener &callback)
    {
    }
    /**
     * @brief notified when #observerNotify
     *
     * ensured called before any other registered observer
     */
    virtual inline void observerOnEvent(ZF_IN const zfidentity &eventId,
                                        ZF_IN ZFObject *param0,
                                        ZF_IN ZFObject *param1)
    {
    }

public:
    zfbool _ZFP_ZFObjectLockIsAvailable(void);
    void _ZFP_ZFObjectLock(void);
    void _ZFP_ZFObjectUnlock(void);
    zfbool _ZFP_ZFObjectTryLock(void);

    static ZFObject *_ZFP_ZFObjectAlloc(ZFObject *obj);
    ZFObject *_ZFP_ZFObjectCheckOnInit(void);
    static void _ZFP_ZFObjectDealloc(ZFObject *obj);

public:
    /**
     * @brief override this to init your object
     *
     * subclass must call superclass's objectOnInit before any other code if override\n
     * due to the limitation of C++,
     * you must declare it as public function,
     * but never call it in your application manually\n
     * you may also declare objectOnInit with params like this:
     * @code
     *   zfclass Parent : ...
     *   {
     *       ...
     *       virtual ZFObject *objectOnInit(void) {...};
     *   };
     *   zfclass Child : Parent
     *   {
     *   public:
     *       virtual ZFObject *objectOnInit(void)
     *       {
     *           zfsuper::objectOnInit(); // since Parent1 doesn't override objectOnInit
     *           return this;
     *       }
     *       // init with params
     *       virtual ZFObject *objectOnInit(Params...)
     *       {
     *           this->objectOnInit(); // call objectOnInit with no params first
     *           // your init steps
     *           return this;
     *       }
     *   };
     * @endcode
     * @warning objectOnInit and objectOnDealloc is called as a virtual function,
     *   take good care of other virtual function call,
     *   use zfself::func() instead of this->func() if necessary,
     *   or use #objectOnInitFinish/#objectOnDeallocPrepare,
     *   or declare your own constructor with #ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR
     * @note due to limitations of C++,
     *   if your parent type declared objectOnInit with different params,
     *   and your child type want to override part of those objectOnInit,
     *   then your child type must also override all objectOnInit that parent declared,
     *   otherwise, some may be hidden
     * @note you may declare objectOnInit as protected method
     *   while declaring subclass of ZFObject,
     *   which means the subclass should not be allocated normally,
     *   typically usage:
     *   @code
     *     zfclass MyObject : zfextends ZFObject
     *     {
     *         ZFOBJECT_DECLARE(MyObject, ZFObject)
     *
     *     public:
     *         static zfautoObject instanceForXxx(xxx)
     *         {
     *             return zfautoObjectCreate(zflineAlloc(MyObject));
     *         }
     *     protected:
     *         ZFCLASS_PRIVATE_ALLOC("should be created by MyObject::instanceForXxx only")
     *         zfoverride
     *         virtual ZFObject *objectOnInit(void) {...}
     *     };
     *     static void func(void)
     *     {
     *         // MyObject *obj = zfAlloc(MyObject); // would compile error
     *         zfautoObject obj = MyObject::instanceForXxx(xxx); // OK
     *     }
     *   @endcode
     */
    virtual ZFObject *objectOnInit(void);
    /**
     * @brief called after #objectOnInit, safe to call virtual functions here
     *
     * usually used to initialize independent resources other than self's internal resources
     */
    virtual void objectOnInitFinish(void);
    /**
     * @brief called before #objectOnDealloc, safe to call virtual functions here
     *
     * usually used to cleanup resources attached to this object other than self's internal resources
     */
    virtual void objectOnDeallocPrepare(void);
    /**
     * @brief override this to destroy your object
     *
     * subclass must call superclass's objectOnDealloc after any other code if override\n
     * see #objectOnInit for more info
     */
    virtual void objectOnDealloc(void);

    /**
     * @brief called to retain object
     *
     * subclass must call superclass's objectOnRetain before any other code if override\n
     * usually you should not override this method
     */
    virtual void objectOnRetain(void);
    /**
     * @brief called to release object
     *
     * subclass must call superclass's onRelase after any other code if override\n
     * usually you should not override this method
     */
    virtual void objectOnRelease(void);

public:
    /**
     * @brief object state
     *
     * note, it's ensured that:
     * @code
     *   ObjectInstanceState state0 = ObjectInstanceStateOnInitFinish;
     *   zfbool t0 = ZFBitTest(state0, ObjectInstanceStateOnInit); // ensured true
     *
     *   ObjectInstanceState state1 = ObjectInstanceStateOnDeallocPrepare;
     *   zfbool t1 = ZFBitTest(state1, ObjectInstanceStateOnDealloc); // ensured true
     * @endcode
     */
    typedef enum {
        ObjectInstanceStateOnInit = 0x01, /**< @brief object is under #objectOnInit */
        ObjectInstanceStateOnInitFinish = 0x03, /**< @brief object is under #objectOnInitFinish */
        ObjectInstanceStateIdle = 0, /**< @brief object is constructed successfully */
        ObjectInstanceStateOnDeallocPrepare = 0x30, /**< @brief object is under #objectOnDeallocPrepare */
        ObjectInstanceStateOnDealloc = 0x10, /**< @brief object is under #objectOnDealloc */
    } ObjectInstanceState;
    /**
     * @brief object instance's state
     */
    virtual ZFObject::ObjectInstanceState objectInstanceState(void);

public:
    /**
     * @brief whether this object is private object or its class is #ZFClass::classIsPrivate
     *
     * if #objectIsInternal, then it is also #objectIsPrivate
     */
    virtual zfbool objectIsPrivate(void);
    /**
     * @brief see #objectIsPrivate
     */
    virtual void objectIsPrivateSet(ZF_IN zfbool value);
    /**
     * @brief whether this object is internal object or its class is #ZFClass::classIsInternal
     *
     * if #objectIsInternal, then it is also #objectIsPrivate
     */
    virtual zfbool objectIsInternal(void);
    /**
     * @brief see #objectIsInternal
     */
    virtual void objectIsInternalSet(ZF_IN zfbool value);

public:
    zffinal void _ZFP_ZFObject_objectPropertyValueCheckMarkCache(ZF_IN const ZFProperty *property,
                                                                 ZF_IN zfbool objectCached);
    zffinal void _ZFP_ZFObject_objectPropertyValueBeforeChange(ZF_IN const ZFProperty *property);
    zffinal void _ZFP_ZFObject_objectPropertyValueAfterChange(ZF_IN const ZFProperty *property);
    zffinal inline void _ZFP_ZFObject_objectPropertyValueOnInit(ZF_IN const ZFProperty *property)
    {
        this->objectPropertyValueOnInit(property);
    }
    zffinal inline void _ZFP_ZFObject_objectPropertyValueOnReset(ZF_IN const ZFProperty *property)
    {
        this->objectPropertyValueOnReset(property);
    }
protected:
    /**
     * @brief see #EventObjectPropertyValueBeforeChange
     */
    virtual void objectPropertyValueBeforeChange(ZF_IN const ZFProperty *property);
    /**
     * @brief see #EventObjectPropertyValueAfterChange
     */
    virtual void objectPropertyValueAfterChange(ZF_IN const ZFProperty *property);
    /**
     * @brief called when property value first time accessed and initialized
     */
    virtual inline void objectPropertyValueOnInit(ZF_IN const ZFProperty *property)
    {
    }
    /**
     * @brief called when property value would be reset by #ZFProperty::callbackResetInitValue
     *
     * @note won't be called when owner object would be deallocated,
     *   you should override #objectOnDeallocPrepare if necessary to do extra cleanup step
     */
    virtual inline void objectPropertyValueOnReset(ZF_IN const ZFProperty *property)
    {
    }

public:
    /**
     * @brief mark the object being cached or not
     *
     * can be called more than once,
     * but must be paired\n
     * a cached object would be filtered from leak test,
     * you may also do other logic depending on cached state\n
     * by default, all retain property would also be marked as cached
     * if its owner object is cached,
     * you may override #objectCachedOnChange to mark other internal content being cached or not
     * @note no thread safe ensured
     */
    virtual void objectCachedSet(ZF_IN zfbool objectCached);
    /** @brief see #objectCachedSet */
    virtual zfbool objectCached(void);
    /** @brief see #objectCachedSet */
    virtual zfindex objectCachedCount(void);
protected:
    /** @brief see #EventObjectCachedOnChange */
    virtual void objectCachedOnChange(void);

private:
    _ZFP_ZFObjectPrivate *d;
    friend zfclassFwd _ZFP_ZFObjectObserverHolder;
};

/**
 * @brief all event notified by #ZFObject::observerNotify would also be notified to the observers added by this method,
 *   see #ZFObject::observerNotify
 *
 * use only if necessary, which may cause performance issue
 */
extern ZF_ENV_EXPORT void ZFObjectGlobalEventObserverAdd(ZF_IN const ZFObject::ObserverAddParam &param);
/** @brief see #ZFObjectGlobalEventObserverAdd */
extern ZF_ENV_EXPORT void ZFObjectGlobalEventObserverAdd(ZF_IN const zfidentity &eventId,
                                                         ZF_IN const ZFListener &observer,
                                                         ZF_IN_OPT ZFObject *userData = zfnull,
                                                         ZF_IN_OPT ZFObject *owner = zfnull,
                                                         ZF_IN_OPT zfbool autoRemoveAfterActivate = zffalse,
                                                         ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal);
/** @brief see #ZFObjectGlobalEventObserverAdd */
extern ZF_ENV_EXPORT void ZFObjectGlobalEventObserverRemove(ZF_IN const zfidentity &eventId,
                                                            ZF_IN const ZFListener &callback);
/** @brief see #ZFObjectGlobalEventObserverAdd */
extern ZF_ENV_EXPORT void ZFObjectGlobalEventObserverRemove(ZF_IN const zfidentity &eventId,
                                                            ZF_IN const ZFListener &callback,
                                                            ZF_IN ZFObject *userData,
                                                            ZF_IN_OPT ZFComparer<ZFObject *>::Comparer userDataComparer = ZFComparerCheckEqualOnly);
/** @brief see #ZFObjectGlobalEventObserverAdd */
extern ZF_ENV_EXPORT void ZFObjectGlobalEventObserverRemoveByOwner(ZF_IN ZFObject *owner);
/** @brief see #ZFObjectGlobalEventObserverAdd */
extern ZF_ENV_EXPORT void ZFObjectGlobalEventObserverRemoveAll(ZF_IN const zfidentity &eventId);
/** @brief see #ZFObjectGlobalEventObserverAdd */
extern ZF_ENV_EXPORT void ZFObjectGlobalEventObserverRemoveAll(void);
/** @brief see #ZFObjectGlobalEventObserverAdd */
extern ZF_ENV_EXPORT zfbool ZFObjectGlobalEventObserverHasAdd(void);
/** @brief see #ZFObjectGlobalEventObserverAdd */
extern ZF_ENV_EXPORT zfbool ZFObjectGlobalEventObserverHasAdd(ZF_IN const zfidentity &eventId);
/** @brief see #ZFObjectGlobalEventObserverAdd */
extern ZF_ENV_EXPORT void ZFObjectGlobalEventObserverInfo(ZF_OUT zfstring &ret);
/** @brief see #ZFObjectGlobalEventObserverAdd */
inline zfstring ZFObjectGlobalEventObserverInfo(void)
{
    zfstring ret;
    ZFObjectGlobalEventObserverInfo(ret);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectDef_h_

#include "ZFObjectRetainDef.h"
#include "ZFObjectFilterDef.h"
#include "ZFObjectNullDef.h"
#include "ZFObjectUtilDef.h"

