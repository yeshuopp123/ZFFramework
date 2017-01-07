/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFObjectSmartPointerDef.h"
#include "ZFSynchronizedDef.h"
#include "ZFClassUtilDef.h"
#include "ZFPropertyDef.h"
#include "ZFObjectUtilTypeDef.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// mutex
static ZFObjectMutexImplCheckCallback _ZFP_ZFObjectMutexImplCheckCallbackRef = zfnull;
static ZFObjectMutexImplInitCallback _ZFP_ZFObjectMutexImplInitCallbackRef = zfnull;
static ZFObjectMutexImplCleanupCallback _ZFP_ZFObjectMutexImplCleanupCallbackRef = zfnull;
void ZFObjectMutexImplSet(ZF_IN_OPT ZFObjectMutexImplCheckCallback checkCallback /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplInitCallback initCallback /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplCleanupCallback cleanupCallback /* = zfnull */)
{
    _ZFP_ZFObjectMutexImplCheckCallbackRef = checkCallback;
    _ZFP_ZFObjectMutexImplInitCallbackRef = initCallback;
    _ZFP_ZFObjectMutexImplCleanupCallbackRef = cleanupCallback;
}

// ============================================================
// observer
zfclassLikePOD _ZFP_ZFObjectObserverData
{
public:
    zfidentity eventId;
    ZFListener observer;
    ZFObject *userData; // no auto retain, manually retain or release for readable leak test
    ZFObject *owner;
    zfbool autoRemoveAfterActivate;
    ZFLevel observerLevel;

public:
    _ZFP_ZFObjectObserverData(void)
    : eventId(zfidentityInvalid)
    , observer()
    , userData(zfnull)
    , owner(zfnull)
    , autoRemoveAfterActivate(zffalse)
    , observerLevel(ZFLevelAppNormal)
    {
    }
    _ZFP_ZFObjectObserverData(ZF_IN zfidentity eventId,
                              ZF_IN const ZFListener &observer,
                              ZF_IN_OPT ZFObject *userData = zfnull,
                              ZF_IN_OPT ZFObject *owner = zfnull,
                              ZF_IN_OPT zfbool autoRemoveAfterActivate = zffalse,
                              ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal)
    : eventId(eventId)
    , observer(observer)
    , userData(userData)
    , owner(owner)
    , autoRemoveAfterActivate(autoRemoveAfterActivate)
    , observerLevel(observerLevel)
    {
    }
private:
    _ZFP_ZFObjectObserverData(ZF_IN const _ZFP_ZFObjectObserverData &ref);
    _ZFP_ZFObjectObserverData &operator = (ZF_IN const _ZFP_ZFObjectObserverData &ref);
};
typedef zfstlvector<ZFCorePointerForObject<_ZFP_ZFObjectObserverData *> > _ZFP_ZFObjectObserverListType;
typedef zfstlmap<zfidentity, _ZFP_ZFObjectObserverListType> _ZFP_ZFObjectObserverMapType;
zfclassNotPOD _ZFP_ZFObjectObserverHolder
{
private:
    _ZFP_ZFObjectObserverMapType observerMap;

public:
    void observerAdd(ZF_IN ZFObject *observerOwner,
                     ZF_IN const zfidentity &eventId,
                     ZF_IN const ZFListener &observer,
                     ZF_IN_OPT ZFObject *userData /* = zfnull */,
                     ZF_IN_OPT ZFObject *owner /* = zfnull */,
                     ZF_IN_OPT zfbool autoRemoveAfterActivate /* = zffalse */,
                     ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */)
    {
        if(!zfidentityIsValid(eventId)
            || !observer.callbackIsValid())
        {
            return ;
        }

        if(userData != zfnull)
        {
            userData->objectCachedSet(zftrue);
        }

        _ZFP_ZFObjectObserverListType &observerList = this->observerMap[eventId];
        zfstlsize index = observerList.size();
        while(index > 0 && observerList.at(index - 1).pointerValueGet()->observerLevel > observerLevel)
        {
            --index;
        }
        _ZFP_ZFObjectObserverData *observerData = zfnew(_ZFP_ZFObjectObserverData
                , eventId
                , observer
                , zflockfree_zfRetain(userData)
                , owner
                , autoRemoveAfterActivate
                , observerLevel
            );
        observerList.insert(observerList.begin() + index,
            ZFCorePointerForObject<_ZFP_ZFObjectObserverData *>(observerData));
        if(observerOwner)
        {
            zfbool first = (observerList.size() == 1);
            observerOwner->observerOnAdd(eventId, observer);
            if(first)
            {
                observerOwner->observerOnAddFirst(eventId);
            }
        }
    }
    void observerRemove(ZF_IN ZFObject *observerOwner,
                        ZF_IN const zfidentity &eventId,
                        ZF_IN const ZFListener &callback,
                        ZF_IN_OPT zfbool userDataNeedCompare = zffalse,
                        ZF_IN_OPT ZFObject *userData = zfnull,
                        ZF_IN_OPT ZFComparer<ZFObject *>::Comparer userDataComparer = ZFComparerCheckEqualOnly)
    {
        _ZFP_ZFObjectObserverMapType::iterator it = this->observerMap.find(eventId);
        if(it != this->observerMap.end())
        {
            for(zfstlsize iObserver = 0; iObserver < it->second.size(); ++iObserver)
            {
                _ZFP_ZFObjectObserverData *observerData = it->second[iObserver].pointerValueGet();
                if(observerData->observer.objectCompareByInstance(callback) == ZFCompareTheSame
                    && (!userDataNeedCompare || userDataComparer(userData, observerData->userData) == ZFCompareTheSame))
                {
                    ZFObject *toRemove = observerData->userData;
                    it->second.erase(it->second.begin() + iObserver);

                    zfbool removedLast = zffalse;
                    if(it->second.empty())
                    {
                        this->observerMap.erase(it);
                        removedLast = zftrue;
                    }
                    if(observerOwner)
                    {
                        observerOwner->observerOnRemove(eventId, callback);
                        if(removedLast)
                        {
                            observerOwner->observerOnRemoveLast(eventId);
                        }
                    }
                    if(toRemove != zfnull)
                    {
                        toRemove->objectCachedSet(zffalse);
                        zflockfree_zfRelease(toRemove);
                    }
                    break;
                }
            }
        }
    }
    void observerRemoveByOwner(ZF_IN ZFObject *observerOwner,
                               ZF_IN ZFObject *owner)
    {
        if(owner != zfnull && !this->observerMap.empty())
        {
            zfstlvector<ZFCorePointerForObject<_ZFP_ZFObjectObserverData *> > removed;
            zfstlvector<zfidentity> removedLast;
            for(_ZFP_ZFObjectObserverMapType::iterator it = this->observerMap.begin();
                it != this->observerMap.end();)
            {
                for(zfstlsize iObserver = 0; iObserver < it->second.size(); ++iObserver)
                {
                    if(it->second[iObserver].pointerValueGet()->owner == owner)
                    {
                        removed.push_back(it->second[iObserver]);
                        it->second.erase(it->second.begin() + iObserver);
                        --iObserver;
                    }
                }
                if(it->second.empty())
                {
                    removedLast.push_back(it->first);
                    this->observerMap.erase(it++);
                }
                else
                {
                    ++it;
                }
            }
            if(observerOwner)
            {
                for(zfstlsize i = 0; i < removed.size(); ++i)
                {
                    observerOwner->observerOnRemove(removed[i].pointerValueGet()->eventId, removed[i].pointerValueGet()->observer);
                }
                for(zfstlsize i = 0; i < removedLast.size(); ++i)
                {
                    observerOwner->observerOnRemoveLast(removedLast[i]);
                }
            }
            if(!removed.empty())
            {
                for(zfstlsize i = 0; i < removed.size(); ++i)
                {
                    ZFObject *userDataTmp = removed[i].pointerValueGet()->userData;
                    if(userDataTmp != zfnull)
                    {
                        userDataTmp->objectCachedSet(zffalse);
                        zflockfree_zfRelease(userDataTmp);
                    }
                }
            }
        }
    }
    void observerRemoveAll(ZF_IN ZFObject *observerOwner,
                           ZF_IN const zfidentity &eventId)
    {
        if(!this->observerMap.empty())
        {
            _ZFP_ZFObjectObserverMapType::iterator it = this->observerMap.find(eventId);
            if(it != this->observerMap.end())
            {
                _ZFP_ZFObjectObserverListType removed = it->second;
                this->observerMap.erase(it);
                if(observerOwner)
                {
                    for(zfstlsize i = 0; i < removed.size(); ++i)
                    {
                        observerOwner->observerOnRemove(eventId, removed[i].pointerValueGet()->observer);
                    }
                    observerOwner->observerOnRemoveLast(eventId);
                }
                if(!removed.empty())
                {
                    for(zfstlsize i = 0; i < removed.size(); ++i)
                    {
                        ZFObject *userDataTmp = removed[i].pointerValueGet()->userData;
                        if(userDataTmp != zfnull)
                        {
                            userDataTmp->objectCachedSet(zffalse);
                            zflockfree_zfRelease(userDataTmp);
                        }
                    }
                }
            }
        }
    }
    void observerRemoveAll(ZF_IN ZFObject *observerOwner)
    {
        if(!this->observerMap.empty())
        {
            _ZFP_ZFObjectObserverMapType tmp;
            tmp.swap(this->observerMap);

            if(observerOwner)
            {
                for(_ZFP_ZFObjectObserverMapType::iterator it = tmp.begin();
                        it != tmp.end();
                        ++it)
                {
                    for(zfstlsize iObserver = 0; iObserver < it->second.size(); ++iObserver)
                    {
                        observerOwner->observerOnRemove(it->first, it->second[iObserver].pointerValueGet()->observer);
                    }
                    observerOwner->observerOnRemoveLast(it->first);
                }
            }

            for(_ZFP_ZFObjectObserverMapType::iterator it = tmp.begin();
                it != tmp.end();
                ++it)
            {
                for(zfstlsize iObserver = 0; iObserver < it->second.size(); ++iObserver)
                {
                    ZFObject *userDataTmp = it->second[iObserver].pointerValueGet()->userData;
                    if(userDataTmp != zfnull)
                    {
                        userDataTmp->objectCachedSet(zffalse);
                        zflockfree_zfRelease(userDataTmp);
                    }
                }
            }
        }
    }
    zfbool observerHasAdd(void)
    {
        return !this->observerMap.empty();
    }
    zfbool observerHasAdd(ZF_IN const zfidentity &eventId)
    {
        return (this->observerMap.find(eventId) != this->observerMap.end());
    }
    void observerNotifyPrepare(ZF_IN ZFObject *observerOwner,
                               ZF_IN_OUT _ZFP_ZFObjectObserverListType &toNotify,
                               ZF_IN_OUT ZFCoreArrayPOD<ZFObject *> &toRelease,
                               ZF_IN const zfidentity &eventId)
    {
        if(!zfidentityIsValid(eventId) || this->observerMap.empty())
        {
            return ;
        }
        _ZFP_ZFObjectObserverMapType::iterator it = this->observerMap.find(eventId);
        if(it != this->observerMap.end())
        {
            for(zfstlsize i = 0; i < it->second.size(); ++i)
            {
                ZFCorePointerForObject<_ZFP_ZFObjectObserverData *> &observerDataHolder = it->second[i];
                _ZFP_ZFObjectObserverData &observerData = *(observerDataHolder.pointerValueGet());
                toNotify.push_back(observerDataHolder);
                if(observerData.autoRemoveAfterActivate)
                {
                    if(observerData.userData != zfnull)
                    {
                        toRelease.add(observerData.userData);
                    }
                    it->second.erase(it->second.begin() + i);
                    --i;

                    if(observerOwner)
                    {
                        observerOwner->observerOnRemove(observerData.eventId, observerData.observer);
                    }
                }
            }
            if(it->second.empty())
            {
                this->observerMap.erase(it);
                if(observerOwner)
                {
                    observerOwner->observerOnRemoveLast(eventId);
                }
            }
        }
    }
    void observerInfo(ZF_OUT zfstring &ret)
    {
        zfbool hasPrev = zffalse;
        for(_ZFP_ZFObjectObserverMapType::iterator it = this->observerMap.begin();
            it != this->observerMap.end();
            ++it)
        {
            if(hasPrev)
            {
                ret += zfText("\n");
            }
            hasPrev = zftrue;

            ret += zfText("  ");
            ret += ZFObserverEventGetName(it->first);
            ret += zfText(":");

            for(zfstlsize iObserver = 0; iObserver < it->second.size(); ++iObserver)
            {
                ret += zfText("\n    ");
                ret += it->second[iObserver].pointerValueGet()->observer.objectInfo();
            }
        }
    }
};

// ============================================================
// ZFObjectGlobalEventObserver
static _ZFP_ZFObjectObserverHolder _ZFP_ZFObjectGlobalEventObserver; // use raw static object for performance

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObjectGlobalEventObserverCleanup, ZFLevelZFFrameworkEssential)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFObjectGlobalEventObserverCleanup)
{
    _ZFP_ZFObjectGlobalEventObserver.observerRemoveAll(zfnull);
}
ZF_GLOBAL_INITIALIZER_END(ZFObjectGlobalEventObserverCleanup)

void ZFObjectGlobalEventObserverAdd(ZF_IN const ZFObject::ObserverAddParam &param)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObjectGlobalEventObserver.observerAdd(zfnull, param.eventId(), param.observer(), param.userData(), param.owner(), param.autoRemoveAfterActivate(), param.observerLevel());
}
void ZFObjectGlobalEventObserverAdd(ZF_IN const zfidentity &eventId,
                                    ZF_IN const ZFListener &observer,
                                    ZF_IN_OPT ZFObject *userData /* = zfnull */,
                                    ZF_IN_OPT ZFObject *owner /* = zfnull */,
                                    ZF_IN_OPT zfbool autoRemoveAfterActivate /* = zffalse */,
                                    ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObjectGlobalEventObserver.observerAdd(zfnull, eventId, observer, userData, owner, autoRemoveAfterActivate, observerLevel);
}
void ZFObjectGlobalEventObserverRemove(ZF_IN const zfidentity &eventId,
                                       ZF_IN const ZFListener &callback)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObjectGlobalEventObserver.observerRemove(zfnull, eventId, callback);
}
void ZFObjectGlobalEventObserverRemove(ZF_IN const zfidentity &eventId,
                                       ZF_IN const ZFListener &callback,
                                       ZF_IN ZFObject *userData,
                                       ZF_IN_OPT ZFComparer<ZFObject *>::Comparer userDataComparer /* = ZFComparerCheckEqualOnly */)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObjectGlobalEventObserver.observerRemove(zfnull, eventId, callback, zftrue, userData, userDataComparer);
}
void ZFObjectGlobalEventObserverRemoveByOwner(ZF_IN ZFObject *owner)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObjectGlobalEventObserver.observerRemoveByOwner(zfnull, owner);
}
void ZFObjectGlobalEventObserverRemoveAll(ZF_IN const zfidentity &eventId)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObjectGlobalEventObserver.observerRemoveAll(zfnull, eventId);
}
void ZFObjectGlobalEventObserverRemoveAll(void)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObjectGlobalEventObserver.observerRemoveAll(zfnull);
}
zfbool ZFObjectGlobalEventObserverHasAdd(void)
{
    ZFCoreMutexLocker();
    return _ZFP_ZFObjectGlobalEventObserver.observerHasAdd();
}
zfbool ZFObjectGlobalEventObserverHasAdd(ZF_IN const zfidentity &eventId)
{
    ZFCoreMutexLocker();
    return _ZFP_ZFObjectGlobalEventObserver.observerHasAdd(eventId);
}
void ZFObjectGlobalEventObserverInfo(ZF_OUT zfstring &ret)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObjectGlobalEventObserver.observerInfo(ret);
}

// ============================================================
// _ZFP_ZFObjectPrivate
typedef zfstlmap<zfstlstringZ, zfautoObject> _ZFP_ZFObjectTagMapType;
zfclassNotPOD _ZFP_ZFObjectPrivate
{
public:
    zfindex objectRetainCount;
    ZFObject::ObjectInstanceState objectInstanceState;
    ZFObjectHolder *objectHolder;
    zfbool mutexImplAvailable;
    ZFObjectMutexImpl *mutexImpl;
    _ZFP_ZFObjectTagMapType tagMap;
    _ZFP_ZFObjectObserverHolder observerHolder;
    zfindex observerNotifyingFlag;
    zfchar *objectDebugInfo;
    zfbool objectDeallocating;
    zfbool objectIsPrivate;
    zfbool objectIsInternal;
    zfindex objectCachedFlag;

public:
    _ZFP_ZFObjectPrivate(void)
    : objectRetainCount(1)
    , objectInstanceState(ZFObject::ObjectInstanceStateOnInit)
    , objectHolder(zfnull)
    , mutexImplAvailable(_ZFP_ZFObjectMutexImplCheckCallbackRef != zfnull && _ZFP_ZFObjectMutexImplCheckCallbackRef())
    , mutexImpl(zfnull)
    , tagMap()
    , observerHolder()
    , observerNotifyingFlag(0)
    , objectDebugInfo(zfnull)
    , objectDeallocating(zffalse)
    , objectIsPrivate(zffalse)
    , objectIsInternal(zffalse)
    , objectCachedFlag(0)
    {
    }
};

// ============================================================
// ZFObject
ZFOBJECT_REGISTER(ZFObject)

ZFOBSERVER_EVENT_REGISTER(ZFObject, ObjectAfterAlloc)
ZFOBSERVER_EVENT_REGISTER(ZFObject, ObjectBeforeDealloc)
ZFOBSERVER_EVENT_REGISTER(ZFObject, ObjectPropertyValueBeforeChange)
ZFOBSERVER_EVENT_REGISTER(ZFObject, ObjectPropertyValueAfterChange)
ZFOBSERVER_EVENT_REGISTER(ZFObject, ObjectCachedOnChange)

zfindex ZFObject::objectRetainCount(void)
{
    return d->objectRetainCount;
}

ZFObjectHolder *ZFObject::objectHolder(void)
{
    if(d->objectHolder == zfnull)
    {
        d->objectHolder = zfAllocWithoutLeakTest(ZFObjectHolder, this);
    }
    return d->objectHolder;
}

void ZFObject::objectInfoOfInstanceT(ZF_IN_OUT zfstring &ret)
{
    ret += this->classData()->className();
    ret += zfText("(");
    zfsFromPointerT(ret, this);
    ret += zfText(")");
}
void ZFObject::objectInfoForDebugT(ZF_IN_OUT zfstring &ret)
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    this->objectInfoOfInstanceT(ret);
    if(this->objectDebugInfo() != zfnull)
    {
        ret += zfText(" (");
        ret += this->objectDebugInfo();
        ret += zfText(")");
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}
void ZFObject::objectInfoT(ZF_IN_OUT zfstring &ret)
{
    this->objectInfoOnAppendTokenLeft(ret);
    this->objectInfoOnAppend(ret);
    this->objectInfoOnAppendTokenRight(ret);
}
void ZFObject::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    this->objectInfoOfInstanceT(ret);
}
zfidentity ZFObject::objectHash(void)
{
    return zfidentityCalcPointer(this);
}
ZFCompareResult ZFObject::objectCompare(ZF_IN ZFObject *anotherObj)
{
    return ((this == anotherObj) ? ZFCompareTheSame : ZFCompareUncomparable);
}

zfbool ZFObject::tagHasSet(void)
{
    ZFCoreMutexLocker();
    return !(d->tagMap.empty());
}
void ZFObject::tagSet(ZF_IN const zfchar *key,
                      ZF_IN ZFObject *tag,
                      ZF_IN_OPT zfbool autoMarkCached /* = zffalse */)
{
    ZFCoreMutexLocker();
    if(d->objectDeallocating && tag != zfnull)
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] you must not set tag while object is deallocating, class: %s, tag: %s"),
            zfsCoreZ2A(this->classData()->className()),
            zfsCoreZ2A(key));
        return ;
    }
    if(key == zfnull)
    {
        return ;
    }

    if(this->objectCached() && tag != zfnull)
    {
        tag->objectCachedSet(zftrue);
    }

    _ZFP_ZFObjectTagMapType &m = d->tagMap;
    _ZFP_ZFObjectTagMapType::iterator it = m.find(key);
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
                m[key] = zflockfree_zfautoObjectCreate(tag);
            }
        }
    }
    else
    {
        zfautoObject holder = it->second;
        if(tag == zfnull)
        {
            m.erase(it);
        }
        else
        {
            if(autoMarkCached)
            {
                it->second = zflockfree_zfautoObjectCreateMarkCached(tag);
            }
            else
            {
                it->second = zflockfree_zfautoObjectCreate(tag);
            }
        }
        if(this->objectCached())
        {
            holder.toObject()->objectCachedSet(zffalse);
        }
    }
}
ZFObject *ZFObject::tagGet(ZF_IN const zfchar *key)
{
    if(key == zfnull)
    {
        return zfnull;
    }
    ZFCoreMutexLocker();
    _ZFP_ZFObjectTagMapType::iterator it = d->tagMap.find(key);
    if(it != d->tagMap.end())
    {
        return it->second.toObject();
    }
    else
    {
        return zfnull;
    }
}
void ZFObject::tagGetAllKeyValue(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &allKey,
                                 ZF_IN_OUT ZFCoreArrayPOD<ZFObject *> &allValue)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObjectTagMapType &m = d->tagMap;
    allKey.capacitySet(allKey.count() + m.size());
    allValue.capacitySet(allValue.count() + m.size());
    for(_ZFP_ZFObjectTagMapType::iterator it = m.begin(); it != m.end(); ++it)
    {
        allKey.add(it->first.c_str());
        allValue.add(it->second.toObject());
    }
}
zfautoObject ZFObject::tagRemoveAndGet(ZF_IN const zfchar *key)
{
    if(key != zfnull)
    {
        ZFCoreMutexLocker();
        _ZFP_ZFObjectTagMapType::iterator it = d->tagMap.find(key);
        if(it != d->tagMap.end())
        {
            zfautoObject ret = zfautoObjectCreate(it->second.toObject());
            d->tagMap.erase(it);
            return ret;
        }
    }
    return zfautoObjectNull;
}
void ZFObject::tagRemoveAll(void)
{
    ZFCoreMutexLocker();
    if(!d->tagMap.empty())
    {
        _ZFP_ZFObjectTagMapType tmp;
        tmp.swap(d->tagMap);
    }
}

void ZFObject::objectDebugInfoSet(ZF_IN const zfchar *debugInfo)
{
    zfsChange(d->objectDebugInfo, debugInfo);
}
const zfchar *ZFObject::objectDebugInfo(void)
{
    return d->objectDebugInfo;
}

void ZFObject::observerAdd(ZF_IN const zfidentity &eventId,
                           ZF_IN const ZFListener &observer,
                           ZF_IN_OPT ZFObject *userData /* = zfnull */,
                           ZF_IN_OPT ZFObject *owner /* = zfnull */,
                           ZF_IN_OPT zfbool autoRemoveAfterActivate /* = zffalse */,
                           ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */)
{
    ZFCoreMutexLocker();
    if(d->objectDeallocating)
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] you must not add observer while object is deallocating, class: %s, event: %s"),
            zfsCoreZ2A(this->classData()->className()),
            ZFObserverEventGetName(eventId));
        return ;
    }

    d->observerHolder.observerAdd(this, eventId, observer, userData, owner, autoRemoveAfterActivate, observerLevel);
}
void ZFObject::observerRemove(ZF_IN const zfidentity &eventId,
                              ZF_IN const ZFListener &callback)
{
    ZFCoreMutexLocker();
    d->observerHolder.observerRemove(this, eventId, callback);
}
void ZFObject::observerRemove(ZF_IN const zfidentity &eventId,
                              ZF_IN const ZFListener &callback,
                              ZF_IN ZFObject *userData,
                              ZF_IN_OPT ZFComparer<ZFObject *>::Comparer userDataComparer /* = ZFComparerCheckEqualOnly */)
{
    ZFCoreMutexLocker();
    d->observerHolder.observerRemove(this, eventId, callback, zftrue, userData, userDataComparer);
}
void ZFObject::observerRemoveByOwner(ZF_IN ZFObject *owner)
{
    ZFCoreMutexLocker();
    d->observerHolder.observerRemoveByOwner(this, owner);
}
void ZFObject::observerRemoveAll(ZF_IN const zfidentity &eventId)
{
    ZFCoreMutexLocker();
    d->observerHolder.observerRemoveAll(this, eventId);
}
void ZFObject::observerRemoveAll(void)
{
    ZFCoreMutexLocker();
    d->observerHolder.observerRemoveAll(this);
}
zfbool ZFObject::observerHasAdd(void)
{
    ZFCoreMutexLocker();
    return d->observerHolder.observerHasAdd();
}
zfbool ZFObject::observerHasAdd(ZF_IN const zfidentity &eventId)
{
    ZFCoreMutexLocker();
    return d->observerHolder.observerHasAdd(eventId);
}
void ZFObject::observerNotifyWithCustomSender(ZF_IN ZFObject *customSender,
                                              ZF_IN const zfidentity &eventId,
                                              ZF_IN_OPT ZFObject *param0 /* = zfnull */,
                                              ZF_IN_OPT ZFObject *param1 /* = zfnull */)
{
    _ZFP_ZFObjectObserverListType toNotify;
    ZFCoreArrayPOD<ZFObject *> toRelease;

    ZFCoreMutexLock();
    d->observerHolder.observerNotifyPrepare(this, toNotify, toRelease, eventId);
    this->observerOnEvent(eventId, param0, param1);
    _ZFP_ZFObjectGlobalEventObserver.observerNotifyPrepare(zfnull, toNotify, toRelease, eventId);
    ZFCoreMutexUnlock();

    if(!toNotify.empty())
    {
        ZFListenerData listenerData(eventId, customSender, param0, param1);
        for(zfstlsize i = 0; i < toNotify.size(); ++i)
        {
            const _ZFP_ZFObjectObserverData &observerData = *(toNotify[i].pointerValueGet());
            observerData.observer.execute(listenerData, observerData.userData);
        }
    }

    ZFCoreMutexLock();
    for(zfindex i = 0; i < toRelease.count(); ++i)
    {
        ZFObject *userDataTmp = toRelease[i];
        userDataTmp->objectCachedSet(zffalse);
        zflockfree_zfRelease(userDataTmp);
    }
    toRelease.removeAll();
    toNotify.clear(); // must be cleared within mutex to ensure thread safe for callback desctruction
    ZFCoreMutexUnlock();
}
void ZFObject::observerInfo(ZF_OUT zfstring &ret)
{
    ZFCoreMutexLocker();
    d->observerHolder.observerInfo(ret);
}

zfbool ZFObject::_ZFP_ZFObjectLockIsAvailable(void)
{
    return d->mutexImplAvailable;
}
void ZFObject::_ZFP_ZFObjectLock(void)
{
    if(d->mutexImpl)
    {
        d->mutexImpl->mutexImplLock();
    }
    else if(d->mutexImplAvailable)
    {
        d->mutexImpl = _ZFP_ZFObjectMutexImplInitCallbackRef();
        d->mutexImpl->mutexImplLock();
    }
}
void ZFObject::_ZFP_ZFObjectUnlock(void)
{
    if(d->mutexImpl)
    {
        d->mutexImpl->mutexImplUnlock();
    }
    // else should not go here
}
zfbool ZFObject::_ZFP_ZFObjectTryLock(void)
{
    if(d->mutexImpl)
    {
        return d->mutexImpl->mutexImplTryLock();
    }
    else if(d->mutexImplAvailable)
    {
        d->mutexImpl = _ZFP_ZFObjectMutexImplInitCallbackRef();
        return d->mutexImpl->mutexImplTryLock();
    }
    else
    {
        return zffalse;
    }
}

ZFObject *ZFObject::_ZFP_ZFObjectAlloc(ZFObject *obj)
{
    obj->classData()->_ZFP_ZFClass_internalTypesRegister();
    return obj;
}
ZFObject *ZFObject::_ZFP_ZFObjectCheckOnInit(void)
{
    if(d == zfnull)
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] ZFObject::objectOnInit() not called"));
        return zfnull;
    }
    this->classData()->_ZFP_ZFClass_propertyInitStepCheckAutoInit(this);
    d->objectInstanceState = ZFObject::ObjectInstanceStateOnInitFinish;
    this->_ZFP_ZFObject_interfaceOnInit();
    this->objectOnInitFinish();
    d->objectInstanceState = ZFObject::ObjectInstanceStateIdle;

    if(!this->objectIsInternal())
    {
        this->classData()->_ZFP_ZFClass_instanceObserverNotify(this);
        this->observerNotify(ZFObject::EventObjectAfterAlloc());
    }

    return this;
}
void ZFObject::_ZFP_ZFObjectDealloc(ZFObject *obj)
{
    if(!obj->objectIsInternal())
    {
        obj->observerNotify(ZFObject::EventObjectBeforeDealloc());
    }
    if(obj->d->observerNotifyingFlag > 0)
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] deallocating an object during observerNotify, you may want to use zfautoRelease?"));
        return ;
    }
    obj->d->objectDeallocating = zftrue;
    obj->d->objectInstanceState = ZFObject::ObjectInstanceStateOnDeallocPrepare;
    obj->objectOnDeallocPrepare();
    obj->_ZFP_ZFObject_interfaceOnDealloc();
    obj->d->objectInstanceState = ZFObject::ObjectInstanceStateOnDealloc;
    zflockfree_ZFLeakTestLogBeforeDealloc(obj);
    obj->objectOnDealloc();
    if(obj->d != zfnull)
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] ZFObject::objectOnDealloc() not called"));
        return ;
    }
    obj->classData()->_ZFP_ZFClass_objectDesctuct(obj);
}

ZFObject *ZFObject::objectOnInit(void)
{
    if(d != zfnull)
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] ZFObject::objectOnInit() called twice"));
        return zfnull;
    }

    // note that (d != zfnull) is also used to check whether ZFObject::objectOnInit() is called
    d = zfpoolNew(_ZFP_ZFObjectPrivate);

    return this;
}
void ZFObject::objectOnDealloc(void)
{
    if(d == zfnull)
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] ZFObject::objectOnDealloc() called twice"));
        return ;
    }

    if(this->objectCached())
    {
        zfCoreLogTrim(zfText("[ZFObject] you should reset objectCached before destroy the object: %s, current cached count: %zi"),
            this->objectInfoOfInstance().cString(), this->objectCachedCount());
    }

    this->observerRemoveAll();

    zfsChange(d->objectDebugInfo, (const zfchar *)zfnull);

    if(d->mutexImpl)
    {
        _ZFP_ZFObjectMutexImplCleanupCallbackRef(d->mutexImpl);
        d->mutexImpl = zfnull;
    }

    if(d->objectHolder)
    {
        d->objectHolder->holdedObj = (ZFObject *)zfnull;
        zfReleaseWithoutLeakTest(d->objectHolder);
    }

    zfpoolDelete(d);
    // note that (d == zfnull) is also used to check whether ZFObject::objectOnDealloc() is called
    d = zfnull;
}
void ZFObject::objectOnInitFinish(void)
{
}
void ZFObject::objectOnDeallocPrepare(void)
{
    this->tagRemoveAll();
    this->observerRemoveAll();
}
void ZFObject::objectOnRetain(void)
{
    zfCoreAssertWithMessageTrim(d->objectRetainCount > 0,
        zfTextA("retain an object while deallocating: %s"), zfsCoreZ2A(this->objectInfoOfInstance().cString()));
    ++(d->objectRetainCount);
}
void ZFObject::objectOnRelease(void)
{
    --(d->objectRetainCount);
}

ZFObject::ObjectInstanceState ZFObject::objectInstanceState(void)
{
    return d->objectInstanceState;
}

zfbool ZFObject::objectIsPrivate(void)
{
    return d->objectIsPrivate || d->objectIsInternal || this->classData()->classIsPrivate();
}
void ZFObject::objectIsPrivateSet(ZF_IN zfbool value)
{
    d->objectIsPrivate = zftrue;
}
zfbool ZFObject::objectIsInternal(void)
{
    return d->objectIsInternal || this->classData()->classIsInternal();
}
void ZFObject::objectIsInternalSet(ZF_IN zfbool value)
{
    d->objectIsInternal = zftrue;
}

void ZFObject::_ZFP_ZFObject_objectPropertyValueCheckMarkCache(ZF_IN const ZFProperty *property,
                                                               ZF_IN zfbool objectCached)
{
    if(this->objectCached() && property->propertyIsRetainProperty())
    {
        ZFObject *value = property->callbackRetainGet(property, this);
        if(value != zfnull)
        {
            value->objectCachedSet(zftrue);
        }
    }
}
void ZFObject::_ZFP_ZFObject_objectPropertyValueBeforeChange(ZF_IN const ZFProperty *property)
{
    if(this->objectCached() && property->propertyIsRetainProperty())
    {
        ZFObject *value = property->callbackRetainGet(property, this);
        if(value != zfnull)
        {
            value->objectCachedSet(zffalse);
        }
    }
    this->objectPropertyValueBeforeChange(property);
}
void ZFObject::_ZFP_ZFObject_objectPropertyValueAfterChange(ZF_IN const ZFProperty *property)
{
    if(this->objectCached() && property->propertyIsRetainProperty())
    {
        ZFObject *value = property->callbackRetainGet(property, this);
        if(value != zfnull)
        {
            value->objectCachedSet(zftrue);
        }
    }
    this->objectPropertyValueAfterChange(property);
}

void ZFObject::objectPropertyValueBeforeChange(ZF_IN const ZFProperty *property)
{
    ZFPointerHolder *holder = ZFPointerHolder::cacheAccess();
    holder->holdedData = property;
    this->observerNotify(ZFObject::EventObjectPropertyValueBeforeChange(), holder);
    ZFPointerHolder::cacheRelease(holder);
}
void ZFObject::objectPropertyValueAfterChange(ZF_IN const ZFProperty *property)
{
    ZFPointerHolder *holder = ZFPointerHolder::cacheAccess();
    holder->holdedData = property;
    this->observerNotify(ZFObject::EventObjectPropertyValueAfterChange(), holder);
    ZFPointerHolder::cacheRelease(holder);
}

zfclass _ZFP_I_ZFObject_objectAllRetainPropertyCache : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFObject_objectAllRetainPropertyCache, ZFObject)

public:
    ZFCoreArrayPOD<const ZFProperty *> allProperty;
};
ZFOBJECT_REGISTER(_ZFP_I_ZFObject_objectAllRetainPropertyCache)
static void _ZFP_ZFObject_objectCachedMarkAllProperty(ZF_IN ZFObject *ownerObject, ZF_IN zfbool objectCached)
{
    _ZFP_I_ZFObject_objectAllRetainPropertyCache *holder = ownerObject->classData()
        ->classTagGet<_ZFP_I_ZFObject_objectAllRetainPropertyCache *>(_ZFP_I_ZFObject_objectAllRetainPropertyCache::ClassData()->className());
    if(holder == zfnull)
    {
        holder = zflockfree_zfAllocWithoutLeakTest(_ZFP_I_ZFObject_objectAllRetainPropertyCache);
        ownerObject->classData()->classTagSet(_ZFP_I_ZFObject_objectAllRetainPropertyCache::ClassData()->className(), holder);
        ownerObject->classData()->classDataChangeAutoRemoveTagAdd(_ZFP_I_ZFObject_objectAllRetainPropertyCache::ClassData()->className());
        holder->allProperty = ZFClassUtil::allProperty(ownerObject->classData());
        for(zfindex i = 0; i < holder->allProperty.count(); ++i)
        {
            if(!holder->allProperty[i]->propertyIsRetainProperty())
            {
                holder->allProperty.remove(i);
                --i;
            }
        }
        zflockfree_zfReleaseWithoutLeakTest(holder);
    }
    ZFCoreArrayPOD<const ZFProperty *> &allProperty = holder->allProperty;
    for(zfindex i = allProperty.count() - 1; i != zfindexMax; --i)
    {
        const ZFProperty *property = allProperty[i];
        if(property->callbackIsValueAccessed(property, ownerObject))
        {
            ZFObject *value = property->callbackRetainGet(property, ownerObject);
            if(value != zfnull)
            {
                value->objectCachedSet(objectCached);
            }
        }
    }
}
void ZFObject::objectCachedSet(ZF_IN zfbool objectCached)
{
    ZFCoreMutexLocker();
    if(objectCached)
    {
        ++(d->objectCachedFlag);
        _ZFP_ZFLeakTestLogCacheAdd(this);

        if(d->objectCachedFlag == 1)
        {
            _ZFP_ZFObject_objectCachedMarkAllProperty(this, zftrue);

            {
                _ZFP_ZFObjectTagMapType &m = d->tagMap;
                for(_ZFP_ZFObjectTagMapType::iterator it = m.begin(); it != m.end(); ++it)
                {
                    it->second.toObject()->objectCachedSet(zftrue);
                }
            }

            this->objectCachedOnChange();
        }
    }
    else
    {
        zfCoreAssert(d->objectCachedFlag > 0);
        _ZFP_ZFLeakTestLogCacheRemove(this);
        --(d->objectCachedFlag);

        if(d->objectCachedFlag == 0)
        {
            _ZFP_ZFObject_objectCachedMarkAllProperty(this, zffalse);

            {
                _ZFP_ZFObjectTagMapType &m = d->tagMap;
                for(_ZFP_ZFObjectTagMapType::iterator it = m.begin(); it != m.end(); ++it)
                {
                    it->second.toObject()->objectCachedSet(zffalse);
                }
            }

            this->objectCachedOnChange();
        }
    }
}
zfbool ZFObject::objectCached(void)
{
    return (d->objectCachedFlag > 0);
}
zfindex ZFObject::objectCachedCount(void)
{
    return d->objectCachedFlag;
}
void ZFObject::objectCachedOnChange(void)
{
    this->observerNotify(ZFObject::EventObjectCachedOnChange());
}

ZF_NAMESPACE_GLOBAL_END

