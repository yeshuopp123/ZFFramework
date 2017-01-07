/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFObjectDef.h"
#include "ZFObjectSmartPointerDef.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_deque.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFClass's global data
ZF_STATIC_INITIALIZER_INIT(ZFClassDataHolder)
{
}
ZF_STATIC_INITIALIZER_DESTROY(ZFClassDataHolder)
{
    ZFCoreArrayPOD<ZFClass *> allClassDelayed;
    this->delayDeleteMap.allValue(allClassDelayed);
    this->delayDeleteMap.removeAll();

    ZFCoreArrayPOD<ZFClass *> allClass;
    this->classMap.allValue(allClass);
    this->classMap.removeAll();

    for(zfindex i = 0; i < allClassDelayed.count(); ++i)
    {
        zfdelete(allClassDelayed[i]);
    }
    for(zfindex i = 0; i < allClass.count(); ++i)
    {
        zfdelete(allClass[i]);
    }
}
ZFCoreMap classMap; // ZFClass *
ZFCoreMap delayDeleteMap; // ZFClass *
/*
 * delay delete a class
 * ZFClass may be registered by different module,
 * while unloading a module,
 * _ZFP_ZFClassUnregister would be called to unload the class,
 * however, it may be called earlier than object instance's dealloc step,
 * so we cache the ZFClass object,
 * remove from the activating classMap,
 * and delete them all during ZFClassDataHolder's dealloc step
 *
 * this can't resolve the problem perfectly,
 * but did resolve most cases
 */
ZF_STATIC_INITIALIZER_END(ZFClassDataHolder)
#define _ZFP_ZFClassMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFClassDataHolder)->classMap)
#define _ZFP_ZFClassDelayDeleteMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFClassDataHolder)->delayDeleteMap)

// ============================================================
// _ZFP_ZFClassPrivate
zfclassPOD _ZFP_ZFPropertyInitStepData
{
public:
    zfbool autoInitOrNot;
    _ZFP_ZFPropertyInitStepCallback callback;
};
zfclassLikePOD _ZFP_ZFClassDataChangeObserverData
{
public:
    ZFListener observer;
    zfautoObject userData;
};
typedef zfstlmap<const ZFProperty *, _ZFP_ZFPropertyInitStepData> _ZFP_ZFPropertyInitStepMapType;
typedef zfstlmap<zfstlstringZ, zfautoObject> _ZFP_ZFClassTagMapType;
typedef zfstldeque<_ZFP_ZFClassDataChangeObserverData> _ZFP_ZFClassDataChangeObserverListType;
zfclassNotPOD _ZFP_ZFClassPrivate
{
public:
    zfindex refCount;
    /*
     * here's a memo for ZFCoreLibDestroyFlag:
     *
     * ZFFramework is designed as plugin type framework,
     * which can be load dynamically
     *
     * it's all right for most case if use
     * ZF_GLOBAL_INITIALIZER_INIT/ZF_STATIC_INITIALIZER_INIT,
     * however, for core meta-object types (class map, method map, etc),
     * it require additional register and unregister steps
     * to ensure initialize and destroy order
     *
     * here's a typical case of the questions mentioned above:
     *
     * * libraries that depends on ZFFramework needs to be load and unload at runtime
     *
     *   at this case, we need some static holder object, to unregister class map from ZFFramework,
     *   during the library being unloaded
     *
     * * ZFFramework would be unloaded before app's destruction,
     *   this is typical if under Qt using QLibrary
     *
     *   at this case, simply use static holder object would cause crash during app's destruction,
     *   since ZFFramework already unloaded when the holder object try to unregister class map
     */
    ZFCoreArrayPOD<zfbool *> ZFCoreLibDestroyFlag;
    ZFClass *pimplOwner;
    _ZFP_ZFObjectConstructor constructor;
    _ZFP_ZFObjectDestructor destructor;
    zfstring className;
    const ZFClass *classParent;
    zfbool isInterface;
    zfbool isPrivateClass;
    zfbool isInternalClass;

public:
    zfbool needRegisterImplementedInterface;
    zfbool internalTypesNeedAutoRegister; // used to register ZFMethod and ZFProperty
    zfbool internalTypesNeedRegister;
    zfbool needFinalInit;
    ZFCoreArrayPOD<const ZFClass *> implementedInterface;
    ZFCoreMap methodMap; // ZFCoreMap<methodName, ZFCoreMap<methodId, const ZFMethod *> >, methods with same name and different method ids
    ZFCoreArrayPOD<const ZFMethod *> methodList;
    ZFCoreArrayPOD<const ZFProperty *> propertyList;
    ZFCoreMap propertyMap; // map to const ZFProperty *
    _ZFP_ZFPropertyInitStepMapType propertyInitStepMap;

public:
    _ZFP_ZFClassTagMapType classTagMap;

public:
    _ZFP_ZFClassDataChangeObserverListType classDataChangeObserverList;
    zfstlmap<zfstlstringZ, zfbool> classDataChangeAutoRemoveTagList;

public:
    const ZFClass **parentListCache; // all parent including self
    const ZFClass **parentInterfaceListCache; // all parent interface, count may differ from interfaceCastListCache
    const ZFClass **interfaceCastListCache; // all parent implemented interface, for interface cast
    _ZFP_ZFObjectToInterfaceCastCallback *interfaceCastCallbackListCache;

public:
    /*
     * cache all parent and children for future use
     */
    zfstlmap<const ZFClass *, zfbool> allParent; // all parent and interface excluding self
    zfstlmap<const ZFClass *, zfbool> allChildren; // all children excluding self

public:
    /*
     * cache all parent to search method and property for performance
     *
     * unlike allParent, this array ensured ordered by: self > parent interface > parent
     */
    zfstldeque<const ZFClass *> methodAndPropertyFindCache;

public:
    zfclassLikePOD InstanceObserverData
    {
    public:
        ZFListener observer;
        zfautoObject userData;
        ZFObject *owner;
        ZFLevel observerLevel;
    };
    zfstldeque<ZFCorePointerForObject<_ZFP_ZFClassPrivate::InstanceObserverData *> > instanceObserver;
    zfstldeque<_ZFP_ZFClassPrivate::InstanceObserverData *> instanceObserverCached;
    void _instanceObserverDoAdd(ZF_IN_OUT zfstldeque<_ZFP_ZFClassPrivate::InstanceObserverData *> &buf,
                                ZF_IN _ZFP_ZFClassPrivate::InstanceObserverData *data)
    {
        zfstlsize index = buf.size();
        while(index > 0 && buf[index - 1]->observerLevel > data->observerLevel)
        {
            --index;
        }
        buf.insert(buf.begin() + index, data);
    }
    void instanceObserverDoAdd(ZF_IN _ZFP_ZFClassPrivate::InstanceObserverData *data,
                               ZF_IN zfbool observeAllChildType)
    {
        this->_instanceObserverDoAdd(this->instanceObserverCached, data);
        if(!observeAllChildType)
        {
            return ;
        }

        for(zfstlmap<const ZFClass *, zfbool>::iterator childIt = this->allChildren.begin(); childIt != this->allChildren.end(); ++childIt)
        {
            if(!childIt->first->classIsInternal())
            {
                this->_instanceObserverDoAdd(childIt->first->d->instanceObserverCached, data);
            }
        }
    }
    void instanceObserverDoRemove(ZF_IN _ZFP_ZFClassPrivate::InstanceObserverData *data)
    {
        const ZFClass *cls = this->pimplOwner;
        zfstlmap<const ZFClass *, zfbool>::iterator childIt = this->allChildren.begin();
        do
        {
            for(zfstlsize i = 0; i < cls->d->instanceObserverCached.size(); ++i)
            {
                if(cls->d->instanceObserverCached[i] == data)
                {
                    cls->d->instanceObserverCached.erase(cls->d->instanceObserverCached.begin() + i);
                    break;
                }
            }

            if(childIt != this->allChildren.end())
            {
                cls = childIt->first;
                ++childIt;
            }
            else
            {
                break;
            }
        } while(zftrue);
    }

public:
    _ZFP_ZFClassPrivate(void)
    : refCount(1)
    , ZFCoreLibDestroyFlag()
    , pimplOwner(zfnull)
    , constructor(zfnull)
    , destructor(zfnull)
    , className()
    , classParent(zfnull)
    , isInterface(zffalse)
    , isPrivateClass(zffalse)
    , isInternalClass(zffalse)
    , needRegisterImplementedInterface(zftrue)
    , internalTypesNeedAutoRegister(zftrue)
    , internalTypesNeedRegister(zftrue)
    , needFinalInit(zftrue)
    , implementedInterface()
    , methodMap()
    , methodList()
    , propertyList()
    , propertyMap()
    , propertyInitStepMap()
    , classTagMap()
    , classDataChangeObserverList()
    , classDataChangeAutoRemoveTagList()
    , parentListCache(zfnull)
    , parentInterfaceListCache(zfnull)
    , interfaceCastListCache(zfnull)
    , interfaceCastCallbackListCache(zfnull)
    , allParent()
    , allChildren()
    , methodAndPropertyFindCache()
    , instanceObserver()
    , instanceObserverCached()
    {
    }
    ~_ZFP_ZFClassPrivate(void)
    {
        for(zfindex i = 0; i < this->ZFCoreLibDestroyFlag.count(); ++i)
        {
            *(this->ZFCoreLibDestroyFlag[i]) = zftrue;
        }
        zffree(this->parentListCache);
        this->parentListCache = zfnull;
        zffree(this->parentInterfaceListCache);
        this->parentInterfaceListCache = zfnull;
        zffree(this->interfaceCastListCache);
        this->interfaceCastListCache = zfnull;
        zffree(this->interfaceCastCallbackListCache);
        this->interfaceCastCallbackListCache = zfnull;
    }
};

// ============================================================
// clear class tag both in ZFLevelZFFrameworkEssential and ZFLevelZFFrameworkHigh
static void _ZFP_ZFClass_classTagClear(void)
{
    ZFCoreArrayPOD<const ZFClass *> allClass;
    _ZFP_ZFClassMap.allValue(allClass);
    for(zfindex i = 0; i < allClass.count(); ++i)
    {
        allClass.get(i)->classTagRemoveAll();
    }
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFClassTagClearLevelEssential, ZFLevelZFFrameworkEssential)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFClassTagClearLevelEssential)
{
    _ZFP_ZFClass_classTagClear();
}
ZF_GLOBAL_INITIALIZER_END(ZFClassTagClearLevelEssential)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFClassTagClearLevelHigh, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFClassTagClearLevelHigh)
{
    _ZFP_ZFClass_classTagClear();
}
ZF_GLOBAL_INITIALIZER_END(ZFClassTagClearLevelHigh)

// ============================================================
// static methods
const ZFClass *ZFClass::classForName(ZF_IN const zfchar *className)
{
    ZFCoreMutexLocker();
    return _ZFP_ZFClassMap.get<const ZFClass *>(className);
}
zfautoObject ZFClass::newInstanceForName(ZF_IN const zfchar *className,
                                         ZF_IN_OPT const zfcharA *callerFile /* = zfnull */,
                                         ZF_IN_OPT const zfcharA *callerFunction /* = zfnull */,
                                         ZF_IN_OPT zfindex callerLine /* = 0 */)
{
    const ZFClass *cls = ZFClass::classForName(className);
    return ((cls != zfnull) ? cls->newInstance(callerFile, callerFunction, callerLine) : zfautoObjectNull);
}
zfautoObject ZFClass::newInstanceForNameWithoutLeakTest(ZF_IN const zfchar *className)
{
    const ZFClass *cls = ZFClass::classForName(className);
    return ((cls != zfnull) ? cls->newInstanceWithoutLeakTest() : zfautoObjectNull);
}
void ZFClass::allClass(ZF_OUT ZFCoreArray<const ZFClass *> &ret)
{
    ZFCoreMutexLocker();
    _ZFP_ZFClassMap.allValue(ret);
}

// ============================================================
// instance observer
void ZFClass::instanceObserverAdd(ZF_IN const ZFListener &observer,
                                  ZF_IN_OPT ZFObject *userData /* = zfnull */,
                                  ZF_IN_OPT ZFObject *owner /* = zfnull */,
                                  ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */,
                                  ZF_IN_OPT zfbool observeAllChildType /* = zftrue */) const
{
    if(this->classIsInternal())
    {
        return ;
    }

    _ZFP_ZFClassPrivate::InstanceObserverData *data = zfnew(_ZFP_ZFClassPrivate::InstanceObserverData);
    data->observer = observer;
    data->userData = zfautoObjectCreateMarkCached(userData);
    data->owner = owner;
    data->observerLevel = observerLevel;
    d->instanceObserver.push_back(ZFCorePointerForObject<_ZFP_ZFClassPrivate::InstanceObserverData *>(data));
    d->instanceObserverDoAdd(data, observeAllChildType);
}
void ZFClass::instanceObserverRemove(ZF_IN const ZFListener &observer) const
{
    ZFCoreMutexLocker();
    for(zfstlsize i = 0; i < d->instanceObserver.size(); ++i)
    {
        _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserver[i];
        if(data->observer == observer)
        {
            d->instanceObserverDoRemove(data);
            d->instanceObserver.erase(d->instanceObserver.begin() + i);
            break;
        }
    }
}
void ZFClass::instanceObserverRemoveByOwner(ZF_IN ZFObject *owner) const
{
    ZFCoreMutexLocker();
    for(zfstlsize i = 0; i < d->instanceObserver.size(); ++i)
    {
        _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserver[i];
        if(data->owner == owner)
        {
            d->instanceObserverDoRemove(data);
            d->instanceObserver.erase(d->instanceObserver.begin() + i);
            --i;
        }
    }
}
void ZFClass::instanceObserverRemoveAll(void) const
{
    ZFCoreMutexLocker();
    while(!d->instanceObserver.empty())
    {
        _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserver[0];
        d->instanceObserverDoRemove(data);
        d->instanceObserver.erase(d->instanceObserver.begin());
    }
}
void ZFClass::_ZFP_ZFClass_instanceObserverNotify(ZF_IN ZFObject *obj) const
{
    if(!d->instanceObserverCached.empty())
    {
        ZFListenerData listenerData(zfidentityInvalid, obj);
        for(zfstlsize i = 0; i < d->instanceObserverCached.size(); ++i)
        {
            _ZFP_ZFClassPrivate::InstanceObserverData &data = *(d->instanceObserverCached[i]);
            data.observer.execute(listenerData, data.userData.toObject());
        }
    }
}

// ============================================================
// class data change observer
void ZFClass::classDataChangeObserverAdd(ZF_IN const ZFListener &observer,
                                         ZF_IN_OPT ZFObject *userData /* = zfnull */) const
{
    ZFCoreMutexLocker();
    zfCoreAssert(observer.callbackIsValid());
    _ZFP_ZFClassDataChangeObserverData data;
    data.observer = observer;
    data.userData = zfautoObjectCreateMarkCached(userData);
    d->classDataChangeObserverList.push_back(data);
}
void ZFClass::classDataChangeObserverRemove(ZF_IN const ZFListener &observer) const
{
    ZFCoreMutexLocker();
    for(zfstlsize i = 0; i < d->classDataChangeObserverList.size(); ++i)
    {
        const _ZFP_ZFClassDataChangeObserverData &data = d->classDataChangeObserverList[i];
        if(data.observer == observer)
        {
            d->classDataChangeObserverList.erase(d->classDataChangeObserverList.begin() + i);
            break;
        }
    }
}
void ZFClass::classDataChangeNotify(void) const
{
    ZFCoreMutexLocker();
    zfblockedAllocWithoutLeakTest(ZFPointerHolder, sender, this);
    for(zfstlsize i = 0; i < d->classDataChangeObserverList.size(); ++i)
    {
        const _ZFP_ZFClassDataChangeObserverData &data = d->classDataChangeObserverList[i];
        data.observer.execute(ZFListenerData(zfidentityInvalid, sender), data.userData.toObject());
    }

    if(!d->classDataChangeAutoRemoveTagList.empty())
    {
        zfstlmap<zfstlstringZ, zfbool> t = d->classDataChangeAutoRemoveTagList;
        for(zfstlmap<zfstlstringZ, zfbool>::iterator it = t.begin(); it != t.end(); ++it)
        {
            this->classTagRemove(it->first.c_str());
        }
    }
}
void ZFClass::classDataChangeAutoRemoveTagAdd(ZF_IN const zfchar *tag) const
{
    zfCoreAssert(tag != zfnull);
    d->classDataChangeAutoRemoveTagList[tag] = zftrue;
}
void ZFClass::classDataChangeAutoRemoveTagRemove(ZF_IN const zfchar *tag) const
{
    zfCoreAssert(tag != zfnull);
    d->classDataChangeAutoRemoveTagList.erase(tag);
}

// ============================================================
// class info
static void _ZFP_ZFClassGetInfo(ZF_IN_OUT zfstring &s,
                                ZF_IN const ZFClass *cls)
{
    if(cls->classIsAbstract() && !cls->classIsInterface())
    {
        s += zfText("(abstract)");
    }
    s += cls->className();
}
void ZFClass::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    _ZFP_ZFClassGetInfo(ret, this);
    if(this->implementedInterfaceCount() > 0)
    {
        ret += '<';
        for(zfindex i = 0; i < this->implementedInterfaceCount(); ++i)
        {
            if(i != 0)
            {
                ret += zfText(", ");
            }
            ret += this->implementedInterfaceAtIndex(i)->className();
        }
        ret += '>';
    }
}
void ZFClass::objectInfoOfInheritTreeT(ZF_IN_OUT zfstring &ret) const
{
    const ZFClass *cls = this;
    while(cls != zfnull)
    {
        if(cls != this)
        {
            ret += zfText(" : ");
        }

        _ZFP_ZFClassGetInfo(ret, cls);

        if(cls->implementedInterfaceCount() > 0)
        {
            ret += '<';
            for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i)
            {
                if(i != 0)
                {
                    ret += zfText(", ");
                }
                cls->implementedInterfaceAtIndex(i)->objectInfoOfInheritTreeT(ret);
            }
            ret += '>';
        }

        cls = cls->parentClass();
    }
}

zfbool ZFClass::classIsSubclassOf(ZF_IN const ZFClass *cls) const
{
    const ZFClass **p = d->parentListCache;
    do
    {
        if(*p == cls)
        {
            return zftrue;
        }
        ++p;
    } while(*p);
    return zffalse;
}
zfbool ZFClass::classIsTypeOf(ZF_IN const ZFClass *cls) const
{
    const ZFClass **p = (cls->d->isInterface ? d->parentInterfaceListCache : d->parentListCache);
    do
    {
        if(*p == cls)
        {
            return zftrue;
        }
        ++p;
    } while(*p);
    return zffalse;
}

zfbool ZFClass::classIsAbstract(void) const
{
    return (d->constructor == zfnull);
}

zfbool ZFClass::classIsInterface(void) const
{
    return d->isInterface;
}

zfbool ZFClass::classIsPrivate(void) const
{
    return d->isPrivateClass;
}
zfbool ZFClass::classIsInternal(void) const
{
    return d->isInternalClass;
}

zfautoObject ZFClass::newInstance(ZF_IN const zfcharA *callerFile /* = zfnull */,
                                  ZF_IN const zfcharA *callerFunction /* = zfnull */,
                                  ZF_IN zfindex callerLine /* = 0 */) const
{
    if(callerFile)
    {
        ZFCoreMutexLocker();
        ZFObject *obj = zfnull;
        if(d->constructor != zfnull)
        {
            obj = d->constructor();
        }
        if(obj != zfnull)
        {
            ZFObject::_ZFP_ZFObjectAlloc(obj);
            obj->objectOnInit();
            obj->_ZFP_ZFObjectCheckOnInit();
            zflockfree_ZFLeakTestLogAfterAllocVerbose(obj, callerFile, callerFunction, callerLine);
        }
        zfautoObject ret = zflockfree_zfautoObjectCreateVerbose(obj, callerFile, callerFunction, callerLine);
        zflockfree_ZFLeakTestLogBeforeReleaseVerbose(obj, callerFile, callerFunction, callerLine);
        zflockfree_zfReleaseWithoutLeakTest(obj);
        return ret;
    }
    else
    {
        return this->newInstanceWithoutLeakTest();
    }
}
zfautoObject ZFClass::newInstanceWithoutLeakTest(void) const
{
    ZFCoreMutexLocker();
    ZFObject *obj = zfnull;
    if(d->constructor != zfnull)
    {
        obj = d->constructor();
    }
    if(obj != zfnull)
    {
        ZFObject::_ZFP_ZFObjectAlloc(obj);
        obj->objectOnInit();
        obj->_ZFP_ZFObjectCheckOnInit();
    }
    zflockfree_zfblockedReleaseWithoutLeakTest(obj);
    return zflockfree_zfautoObjectCreateWithoutLeakTest(obj);
}

zfindex ZFClass::implementedInterfaceCount(void) const
{
    return d->implementedInterface.count();
}
const ZFClass *ZFClass::implementedInterfaceAtIndex(ZF_IN zfindex index) const
{
    return d->implementedInterface.get(index);
}

// ============================================================
// ZFMethod
zfindex ZFClass::methodCount(void) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    return d->methodList.count();
}
const ZFMethod *ZFClass::methodAtIndex(ZF_IN zfindex index) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    if(index >= d->methodList.count())
    {
        zfCoreCriticalIndexOutOfRange(index, this->methodCount());
        return zfnull;
    }
    return d->methodList[index];
}
const ZFMethod *ZFClass::methodForNameIgnoreParent(ZF_IN const zfchar *methodName,
                                                   ZF_IN_OPT const zfchar *methodId /* = zfnull */) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();

    if(!d->methodMap.isEmpty())
    {
        ZFCoreMap *methodList = d->methodMap.get<ZFCoreMap *>(methodName);
        if(methodList != zfnull)
        {
            return methodList->get<const ZFMethod *>(methodId);
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::methodForName(ZF_IN const zfchar *methodName,
                                       ZF_IN_OPT const zfchar *methodId /* = zfnull */) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    const ZFMethod *ret = zfnull;
    for(zfstlsize i = 0; i < d->methodAndPropertyFindCache.size() && ret == zfnull; ++i)
    {
        ret = d->methodAndPropertyFindCache[i]->methodForNameIgnoreParent(methodName, methodId);
    }
    return ret;
}

// ============================================================
// ZFProperty
zfindex ZFClass::propertyCount(void) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    return d->propertyList.count();
}
const ZFProperty *ZFClass::propertyAtIndex(ZF_IN zfindex index) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    if(index >= d->propertyList.count())
    {
        zfCoreCriticalIndexOutOfRange(index, this->propertyCount());
        return zfnull;
    }
    return d->propertyList.get(index);
}
const ZFProperty *ZFClass::propertyForNameIgnoreParent(const zfchar *propertyName) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    if(!d->propertyMap.isEmpty())
    {
        const ZFProperty *t = d->propertyMap.get<const ZFProperty *>(propertyName);
        return t;
    }
    return zfnull;
}
const ZFProperty *ZFClass::propertyForName(const zfchar *propertyName) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    const ZFProperty *ret = zfnull;
    for(zfstlsize i = 0; i < d->methodAndPropertyFindCache.size() && ret == zfnull; ++i)
    {
        ret = d->methodAndPropertyFindCache[i]->propertyForNameIgnoreParent(propertyName);
    }
    return ret;
}

// ============================================================
// class instance methods
void ZFClass::classTagSet(ZF_IN const zfchar *key,
                          ZF_IN ZFObject *tag,
                          ZF_IN_OPT zfbool autoMarkCached /* = zffalse */) const
{
    if(key == zfnull)
    {
        return ;
    }
    if(tag != zfnull && ZFFrameworkStateCheck(ZFLevelZFFrameworkEssential) != ZFFrameworkStateAvailable)
    {
        // must not store any new tags during cleanup
        abort();
    }
    ZFCoreMutexLocker();
    _ZFP_ZFClassTagMapType &m = d->classTagMap;
    _ZFP_ZFClassTagMapType::iterator it = m.find(key);
    if(it == m.end())
    {
        if(tag != zfnull)
        {
            if(autoMarkCached)
            {
                m[key] = zflockfree_zfautoObjectCreateMarkCached(tag);
            }
            else
            {
                m[key] = zflockfree_zfautoObjectCreateWithoutLeakTest(tag);
            }
        }
    }
    else
    {
        if(tag == zfnull)
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            m.erase(it);
        }
        else
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            if(autoMarkCached)
            {
                it->second = zflockfree_zfautoObjectCreateMarkCached(tag);
            }
            else
            {
                it->second = zflockfree_zfautoObjectCreateWithoutLeakTest(tag);
            }
        }
    }
}
ZFObject *ZFClass::classTagGet(ZF_IN const zfchar *key) const
{
    if(key != zfnull)
    {
        ZFCoreMutexLocker();
        _ZFP_ZFClassTagMapType::iterator it = d->classTagMap.find(key);
        if(it != d->classTagMap.end())
        {
            return it->second.toObject();
        }
    }
    return zfnull;
}
void ZFClass::classTagGetAllKeyValue(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &allKey,
                                     ZF_IN_OUT ZFCoreArrayPOD<ZFObject *> &allValue) const
{
    _ZFP_ZFClassTagMapType &m = d->classTagMap;
    allKey.capacitySet(allKey.count() + m.size());
    allValue.capacitySet(allValue.count() + m.size());
    for(_ZFP_ZFClassTagMapType::iterator it = m.begin(); it != m.end(); ++it)
    {
        allKey.add(it->first.c_str());
        allValue.add(it->second.toObject());
    }
}
zfautoObject ZFClass::classTagRemoveAndGet(ZF_IN const zfchar *key) const
{
    if(key != zfnull)
    {
        _ZFP_ZFClassTagMapType &m = d->classTagMap;
        _ZFP_ZFClassTagMapType::iterator it = m.find(key);
        if(it != m.end())
        {
            zfautoObject ret = zfautoObjectCreate(it->second.toObject());
            m.erase(it);
            return ret;
        }
    }
    return zfautoObjectNull;
}
void ZFClass::classTagRemoveAll(void) const
{
    if(!d->classTagMap.empty())
    {
        _ZFP_ZFClassTagMapType tmp;
        tmp.swap(d->classTagMap);
    }
}

// ============================================================
// private
/** @cond ZFPrivateDoc */
ZFClass::ZFClass(void)
: d(zfnull)
{
    d = zfnew(_ZFP_ZFClassPrivate);
    d->pimplOwner = this;
    this->_ZFP_ZFClassNeedInitImplementationList = zftrue;
}
ZFClass::~ZFClass(void)
{
    if(!d->classTagMap.empty())
    {
        // class tags must be removed before destroying a ZFClass
        abort();
    }

    for(zfstlmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it)
    {
        it->first->d->allParent.erase(this);
    }
    for(zfstlmap<const ZFClass *, zfbool>::iterator it = d->allParent.begin(); it != d->allParent.end(); ++it)
    {
        it->first->d->allChildren.erase(this);
    }

    zfdelete(d);
    d = zfnull;
}
ZFClass *ZFClass::_ZFP_ZFClassRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                                       ZF_IN const zfchar *name,
                                       ZF_IN const ZFClass *parent,
                                       ZF_IN _ZFP_ZFObjectConstructor constructor,
                                       ZF_IN _ZFP_ZFObjectDestructor destructor,
                                       ZF_IN_OPT zfbool isInterface /* = zffalse */)
{
    ZFCoreMutexLocker();
    ZFCorePointerBase *d = _ZFP_ZFClassMap.get(name);
    ZFClass *cls = zfnull;
    if(d == zfnull)
    {
        cls = zfnew(ZFClass);
        _ZFP_ZFClassMap.set(name, ZFCorePointerForPointerRef<ZFClass *>(cls));
    }
    else
    {
        cls = d->pointerValueT<ZFClass *>();
        ++(cls->d->refCount);
        return cls;
    }

    cls->d->ZFCoreLibDestroyFlag.add(ZFCoreLibDestroyFlag);

    cls->d->constructor = constructor;
    cls->d->destructor = destructor;

    cls->d->className = name;
    cls->classNameCache = cls->d->className.cString();

    cls->d->classParent = parent;
    cls->classParentCache = parent;

    cls->d->isInterface = isInterface;

    {
        const zfchar *filter = zfText("_ZFP_");
        const zfindex filterLen = zfslen(filter);
        cls->d->isPrivateClass = (zfsncmp(name, filter, filterLen) == 0);
    }
    {
        const zfchar *filter = zfText("_ZFP_I_");
        const zfindex filterLen = zfslen(filter);
        cls->d->isInternalClass = (zfsncmp(name, filter, filterLen) == 0);
    }

    return cls;
}
void ZFClass::_ZFP_ZFClassUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN const zfchar *name)
{
    ZFCoreMutexLocker();
    zfiterator it = _ZFP_ZFClassMap.iteratorForKey(name);
    if(!_ZFP_ZFClassMap.iteratorIsValid(it))
    {
        zfCoreCriticalShouldNotGoHere();
        return ;
    }
    ZFClass *cls = _ZFP_ZFClassMap.iteratorGetValue<ZFClass *>(it);
    cls->d->ZFCoreLibDestroyFlag.removeElement(ZFCoreLibDestroyFlag);
    --(cls->d->refCount);
    if(cls->d->refCount == 0)
    {
        _ZFP_ZFClassMap.iteratorRemove(it);
        _ZFP_ZFClassDelayDeleteMap.set(name, ZFCorePointerForPointerRef<ZFClass *>(cls));
    }
}

zfbool ZFClass::_ZFP_ZFClass_interfaceNeedRegister(void)
{
    return d->needRegisterImplementedInterface;
}
void ZFClass::_ZFP_ZFClass_interfaceRegister(ZF_IN const ZFClass *cls,
                                             ...)
{
    va_list vaList;
    va_start(vaList, cls);
    const ZFClass *tmp = cls;
    while(tmp != zfnull)
    {
        d->implementedInterface.add(tmp);
        tmp = va_arg(vaList, const ZFClass *);
    }
    va_end(vaList);
}
void ZFClass::_ZFP_ZFClass_interfaceRegisterCastCallback(ZF_IN _ZFP_ZFObjectToInterfaceCastCallback callback,
                                                         ...)
{
    va_list vaList;
    va_start(vaList, callback);
    _ZFP_ZFObjectToInterfaceCastCallback callbackTmp = callback;
    ZFCoreArrayPOD<_ZFP_ZFObjectToInterfaceCastCallback> parentInterfaceCastList;
    while(callbackTmp != zfnull)
    {
        parentInterfaceCastList.add(callbackTmp);
        callbackTmp = va_arg(vaList, _ZFP_ZFObjectToInterfaceCastCallback);
    }
    va_end(vaList);

    // remove ZFInterface
    for(zfindex i = 0; i < d->implementedInterface.count(); ++i)
    {
        if(d->implementedInterface[i] == ZFInterface::ClassData())
        {
            d->implementedInterface.remove(i);
            parentInterfaceCastList.remove(i);
            --i;
        }
    }

    d->interfaceCastListCache = (const ZFClass **)zfrealloc(d->interfaceCastListCache, sizeof(const ZFClass *) * (d->implementedInterface.count() + 1));
    zfmemcpy(d->interfaceCastListCache, d->implementedInterface.arrayBuf(), sizeof(const ZFClass *) * d->implementedInterface.count());
    d->interfaceCastListCache[d->implementedInterface.count()] = zfnull;

    d->interfaceCastCallbackListCache = (_ZFP_ZFObjectToInterfaceCastCallback *)zfrealloc(
        d->interfaceCastCallbackListCache, sizeof(_ZFP_ZFObjectToInterfaceCastCallback) * (parentInterfaceCastList.count() + 1));
    zfmemcpy(d->interfaceCastCallbackListCache, parentInterfaceCastList.arrayBuf(), sizeof(_ZFP_ZFObjectToInterfaceCastCallback) * parentInterfaceCastList.count());
    d->interfaceCastCallbackListCache[parentInterfaceCastList.count()] = zfnull;
}
ZFInterface *ZFClass::_ZFP_ZFClass_interfaceCast(ZF_IN ZFObject * const &obj,
                                                 ZF_IN const ZFClass *interfaceClass) const
{
    if(obj)
    {
        const ZFClass **parentInterfaceListTmp = d->interfaceCastListCache;
        _ZFP_ZFObjectToInterfaceCastCallback *parentInterfaceCastListTmp = d->interfaceCastCallbackListCache;
        do
        {
            if(*parentInterfaceListTmp == interfaceClass)
            {
                return (*parentInterfaceCastListTmp)(obj);
            }
            ++parentInterfaceListTmp;
            ++parentInterfaceCastListTmp;
        } while(*parentInterfaceListTmp);
    }
    return zfnull;
}

ZFClass *ZFClass::_ZFP_ZFClassInitFinish(ZF_IN ZFClass *cls)
{
    ZFCoreMutexLocker();
    if(cls->d->needFinalInit)
    {
        cls->d->needFinalInit = zffalse;

        ZFClass::_ZFP_ZFClassInitFinish_parentListCache(cls);
        ZFClass::_ZFP_ZFClassInitFinish_parentInterfaceListCache(cls);
        ZFClass::_ZFP_ZFClassInitFinish_interfaceCastListCache(cls);
        ZFClass::_ZFP_ZFClassInitFinish_allParentAndChildrenCache(cls);
        ZFClass::_ZFP_ZFClassInitFinish_methodAndPropertyFindCache(cls);
        ZFClass::_ZFP_ZFClassInitFinish_instanceObserverCache(cls);
    }
    return cls;
}
void ZFClass::_ZFP_ZFClassInitFinish_parentListCache(ZF_IN ZFClass *cls)
{ // init parent list for better search performance
    ZFCoreArrayPOD<const ZFClass *> parentList;
    {
        parentList.add(cls);
        const ZFClass *parentTmp = cls->parentClass();
        while(parentTmp != zfnull)
        {
            parentList.add(parentTmp);
            parentTmp = parentTmp->parentClass();
        }
    }
    cls->d->parentListCache = (const ZFClass **)zfmalloc(sizeof(const ZFClass *) * (parentList.count() + 1));
    zfmemcpy(cls->d->parentListCache, parentList.arrayBuf(), sizeof(const ZFClass *) * parentList.count());
    cls->d->parentListCache[parentList.count()] = zfnull;
}
void ZFClass::_ZFP_ZFClassInitFinish_parentInterfaceListCache(ZF_IN ZFClass *cls)
{ // init parent type list for better search performance
    ZFCoreArrayPOD<const ZFClass *> parentList;
    {
        ZFCoreArrayPOD<const ZFClass *> clsToCheck;
        clsToCheck.add(cls);
        do
        {
            const ZFClass *clsTmp = clsToCheck.getFirst();
            clsToCheck.removeFirst();
            if(parentList.find(clsTmp) == zfindexMax)
            {
                parentList.add(clsTmp);

                if(clsTmp->parentClass() != zfnull)
                {
                    clsToCheck.add(clsTmp->parentClass());
                }
                clsToCheck.addFrom(clsTmp->d->implementedInterface);
            }
        } while(!clsToCheck.isEmpty());
        for(zfindex i = 0; i < parentList.count(); ++i)
        {
            if(!parentList[i]->classIsInterface())
            {
                parentList.remove(i);
                --i;
            }
        }
    }
    cls->d->parentInterfaceListCache = (const ZFClass **)zfmalloc(sizeof(const ZFClass *) * (parentList.count() + 1));
    zfmemcpy(cls->d->parentInterfaceListCache, parentList.arrayBuf(), sizeof(const ZFClass *) * parentList.count());
    cls->d->parentInterfaceListCache[parentList.count()] = zfnull;
}
void ZFClass::_ZFP_ZFClassInitFinish_interfaceCastListCache(ZF_IN ZFClass *cls)
{ // copy parent's interface cast datas
    ZFCoreArrayPOD<const ZFClass *> parentInterfaceList;
    if(cls->parentClass() != zfnull)
    {
        for(const ZFClass **p = cls->parentClass()->d->interfaceCastListCache; *p != zfnull; ++p)
        {
            if(parentInterfaceList.find(*p, ZFComparerCheckEqualOnly) == zfindexMax)
            {
                parentInterfaceList.add(*p);
            }
        }
    }
    if(cls->d->interfaceCastListCache != zfnull)
    {
        for(const ZFClass **p = cls->d->interfaceCastListCache; *p != zfnull; ++p)
        {
            if(parentInterfaceList.find(*p, ZFComparerCheckEqualOnly) == zfindexMax)
            {
                parentInterfaceList.add(*p);
            }
        }
    }
    cls->d->interfaceCastListCache = (const ZFClass **)zfrealloc(cls->d->interfaceCastListCache, sizeof(const ZFClass *) * (parentInterfaceList.count() + 1));
    zfmemcpy(cls->d->interfaceCastListCache, parentInterfaceList.arrayBuf(), sizeof(const ZFClass *) * parentInterfaceList.count());
    cls->d->interfaceCastListCache[parentInterfaceList.count()] = zfnull;

    ZFCoreArrayPOD<_ZFP_ZFObjectToInterfaceCastCallback> parentInterfaceCastList;
    if(cls->parentClass() != zfnull)
    {
        for(_ZFP_ZFObjectToInterfaceCastCallback *p = cls->parentClass()->d->interfaceCastCallbackListCache; *p != zfnull; ++p)
        {
            if(parentInterfaceCastList.find(*p, ZFComparerCheckEqualOnly) == zfindexMax)
            {
                parentInterfaceCastList.add(*p);
            }
        }
    }
    if(cls->d->interfaceCastCallbackListCache != zfnull)
    {
        for(_ZFP_ZFObjectToInterfaceCastCallback *p = cls->d->interfaceCastCallbackListCache; *p != zfnull; ++p)
        {
            if(parentInterfaceCastList.find(*p, ZFComparerCheckEqualOnly) == zfindexMax)
            {
                parentInterfaceCastList.add(*p);
            }
        }
    }
    cls->d->interfaceCastCallbackListCache = (_ZFP_ZFObjectToInterfaceCastCallback *)zfrealloc(
        cls->d->interfaceCastCallbackListCache, sizeof(_ZFP_ZFObjectToInterfaceCastCallback) * (parentInterfaceCastList.count() + 1));
    zfmemcpy(cls->d->interfaceCastCallbackListCache, parentInterfaceCastList.arrayBuf(), sizeof(_ZFP_ZFObjectToInterfaceCastCallback) * parentInterfaceCastList.count());
    cls->d->interfaceCastCallbackListCache[parentInterfaceCastList.count()] = zfnull;
}
void ZFClass::_ZFP_ZFClassInitFinish_allParentAndChildrenCache(ZF_IN ZFClass *cls)
{ // all parent and children
    ZFCoreArrayPOD<const ZFClass *> clsToCheck;
    clsToCheck.add(cls);
    do
    {
        const ZFClass *clsTmp = clsToCheck.getFirst();
        clsToCheck.removeFirst();
        if(cls->d->allParent.find(clsTmp) == cls->d->allParent.end())
        {
            cls->d->allParent[clsTmp] = zftrue;
            clsTmp->d->allChildren[cls] = zftrue;

            if(clsTmp->parentClass() != zfnull)
            {
                clsToCheck.add(clsTmp->parentClass());
            }
            clsToCheck.addFrom(clsTmp->d->implementedInterface);
        }
    } while(!clsToCheck.isEmpty());
}
void ZFClass::_ZFP_ZFClassInitFinish_methodAndPropertyFindCache(ZF_IN ZFClass *cls)
{ // all class to find property and method
    zfstldeque<const ZFClass *> clsToCheck;
    zfstlmap<const ZFClass *, zfbool> alreadyChecked;
    clsToCheck.push_back(cls);
    do
    {
        const ZFClass *clsTmp = clsToCheck.front();
        clsToCheck.pop_front();
        if(alreadyChecked.find(clsTmp) != alreadyChecked.end())
        {
            continue;
        }
        cls->d->methodAndPropertyFindCache.push_back(clsTmp);
        alreadyChecked[clsTmp] = zftrue;

        for(zfindex i = clsTmp->implementedInterfaceCount() - 1; i != zfindexMax; --i)
        {
            clsToCheck.push_back(clsTmp->implementedInterfaceAtIndex(i));
        }
        if(clsTmp->parentClass())
        {
            clsToCheck.push_back(clsTmp->parentClass());
        }
    } while(!clsToCheck.empty());
}
void ZFClass::_ZFP_ZFClassInitFinish_instanceObserverCache(ZF_IN ZFClass *cls)
{ // copy all parent's instance observer to self
    for(zfstlmap<const ZFClass *, zfbool>::iterator it = cls->d->allParent.begin(); it != cls->d->allParent.end(); ++it)
    {
        zfstldeque<ZFCorePointerForObject<_ZFP_ZFClassPrivate::InstanceObserverData *> > &parentInstanceObserver = it->first->d->instanceObserver;
        for(zfstlsize i = 0; i < parentInstanceObserver.size(); ++i)
        {
            cls->d->instanceObserverCached.push_back(parentInstanceObserver[i].pointerValueGet());
        }
    }
}

zfbool ZFClass::_ZFP_ZFClass_methodRegister(ZF_IN const ZFMethod *method)
{
    ZFCoreMap *methodList = d->methodMap.get<ZFCoreMap *>(method->methodName());
    if(methodList == zfnull)
    {
        methodList = zfnew(ZFCoreMap);
        d->methodMap.set(method->methodName(), ZFCorePointerForObject<ZFCoreMap *>(methodList));
    }
    else
    {
        if(methodList->isContain(method->methodId()))
        {
            return zffalse;
        }
    }
    methodList->set(method->methodId(), ZFCorePointerForPointerRef<const ZFMethod *>(method));
    d->methodList.add(method);

    this->classDataChangeNotify();
    return zftrue;
}
void ZFClass::_ZFP_ZFClass_methodUnregister(ZF_IN const ZFMethod *method)
{
    ZFCoreMap *methodList = d->methodMap.get<ZFCoreMap *>(method->methodName());
    if(methodList != zfnull)
    {
        zfbool removed = d->methodList.removeElement(method);
        methodList->remove(method->methodId());
        if(removed)
        {
            this->classDataChangeNotify();
        }
    }
}

zfbool ZFClass::_ZFP_ZFClass_propertyRegister(ZF_IN const ZFProperty *zfproperty)
{
    if(d->propertyMap.isContain(zfproperty->propertyName()))
    {
        return zffalse;
    }
    else
    {
        d->propertyMap.set(zfproperty->propertyName(), ZFCorePointerForPointerRef<const ZFProperty *>(zfproperty));
        d->propertyList.add(zfproperty);
        this->classDataChangeNotify();
        return zftrue;
    }
}
void ZFClass::_ZFP_ZFClass_propertyUnregister(ZF_IN const ZFProperty *zfproperty)
{
    d->propertyMap.remove(zfproperty->propertyName());
    zfbool removed = d->propertyList.removeElement(zfproperty);
    if(removed)
    {
        this->classDataChangeNotify();
    }
}

static void _ZFP_ZFClass_propertyInitStepMapMerge(ZF_IN_OUT _ZFP_ZFPropertyInitStepMapType &mergeTo,
                                                  ZF_IN const _ZFP_ZFPropertyInitStepMapType &mergeFrom)
{
    for(_ZFP_ZFPropertyInitStepMapType::const_iterator it = mergeFrom.begin(); it != mergeFrom.end(); ++it)
    {
        _ZFP_ZFPropertyInitStepMapType::iterator itTo = mergeTo.find(it->first);
        if(itTo == mergeTo.end())
        {
            mergeTo[it->first] = it->second;
        }
        else
        {
            zfbool autoInitOrNot = (it->second.autoInitOrNot || itTo->second.autoInitOrNot);
            itTo->second = it->second;
            if(autoInitOrNot)
            {
                itTo->second.autoInitOrNot = zftrue;
            }
        }
    }
}

zfbool ZFClass::_ZFP_ZFClass_propertyInitStepIsTheSame(ZF_IN const ZFProperty *property,
                                                       ZF_IN const ZFClass *refClass) const
{
    if(this == refClass)
    {
        return zftrue;
    }
    _ZFP_ZFPropertyInitStepMapType::iterator it = d->propertyInitStepMap.find(property);
    _ZFP_ZFPropertyInitStepMapType::iterator itRef = refClass->d->propertyInitStepMap.find(property);
    if(it == d->propertyInitStepMap.end())
    {
        if(itRef == refClass->d->propertyInitStepMap.end())
        {
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }
    else
    {
        if(itRef == refClass->d->propertyInitStepMap.end())
        {
            return zffalse;
        }
        else
        {
            return (it->second.callback == itRef->second.callback);
        }
    }
}

void ZFClass::_ZFP_ZFClass_propertyInitStepRegister(ZF_IN const ZFProperty *property,
                                                    ZF_IN _ZFP_ZFPropertyInitStepCallback callback,
                                                    ZF_IN zfbool autoInitOrNot) const
{
    _ZFP_ZFPropertyInitStepData data;
    data.autoInitOrNot = autoInitOrNot;
    data.callback = callback;
    d->propertyInitStepMap[property] = data;
}
void ZFClass::_ZFP_ZFClass_propertyInitStepDataInit(void) const
{
    _ZFP_ZFPropertyInitStepMapType propertyInitStepMapTmp;

    for(zfindex i = 0; i < this->implementedInterfaceCount(); ++i)
    {
        const ZFClass *interfaceClass = this->implementedInterfaceAtIndex(i);
        interfaceClass->_ZFP_ZFClass_propertyInitStepDataInit();
        _ZFP_ZFClass_propertyInitStepMapMerge(propertyInitStepMapTmp, interfaceClass->d->propertyInitStepMap);
    }

    if(this->parentClass() != zfnull)
    {
        this->parentClass()->_ZFP_ZFClass_propertyInitStepDataInit();
        _ZFP_ZFClass_propertyInitStepMapMerge(propertyInitStepMapTmp, this->parentClass()->d->propertyInitStepMap);
    }

    _ZFP_ZFClass_propertyInitStepMapMerge(propertyInitStepMapTmp, d->propertyInitStepMap);

    d->propertyInitStepMap.swap(propertyInitStepMapTmp);
}
void ZFClass::_ZFP_ZFClass_propertyInitStepCheckAutoInit(ZF_IN ZFObject *obj) const
{
    for(_ZFP_ZFPropertyInitStepMapType::const_iterator it = d->propertyInitStepMap.begin(); it != d->propertyInitStepMap.end(); ++it)
    {
        if(!it->second.autoInitOrNot)
        {
            continue;
        }
        const ZFProperty *property = it->first;
        // access property value to ensure custom init step called
        //
        // here's a typical use case to show why auto init is required:
        // * base class has a property
        // * child class has override the property's setter and custom init step,
        //   and custom setter changes base class's internal state
        //
        // if no auto init logic and property value wasn't accessed,
        // then the setter would never be called,
        // causing base class's state remaining in old
        if(property->propertyIsRetainProperty())
        {
            (void)property->callbackRetainGet(property, obj);
        }
        else
        {
            (void)property->callbackAssignGet(property, obj);
        }
    }
}
zfbool ZFClass::_ZFP_ZFClass_propertyInitStepAction(ZF_IN ZFObject *obj,
                                                    ZF_IN const ZFProperty *property,
                                                    ZF_IN_OUT void *valueAddr) const
{
    _ZFP_ZFPropertyInitStepMapType::const_iterator it = d->propertyInitStepMap.find(property);
    if(it != d->propertyInitStepMap.end())
    {
        it->second.callback(it->first, obj, valueAddr);
        return it->second.autoInitOrNot;
    }
    return zffalse;
}

void ZFClass::_ZFP_ZFClass_objectDesctuct(ZF_IN ZFObject *obj) const
{
    d->destructor(obj);
}
void ZFClass::_ZFP_ZFClass_internalTypesRegister(void) const
{
    if(d->internalTypesNeedRegister)
    {
        d->internalTypesNeedRegister = zffalse;
        d->internalTypesNeedAutoRegister = zffalse;

        this->_ZFP_ZFClass_propertyInitStepDataInit();
    }
}
void ZFClass::_ZFP_ZFClass_methodAndPropertyAutoRegister(void) const
{
    if(d->internalTypesNeedAutoRegister)
    {
        d->internalTypesNeedAutoRegister = zffalse;

        // create dummy instance to ensure static init of the object would take effect
        // including method and property register
        if(d->constructor != zfnull)
        {
            d->destructor(d->constructor());
        }
    }
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

