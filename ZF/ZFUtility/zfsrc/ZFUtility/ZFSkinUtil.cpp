/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFSkinUtil.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassLikePOD _ZFP_zfSkinApplyData
{
public:
    zfstring objIdentity;
    zfstring skinKey;
    ZFSkinOnChangeCallback skinChangeCallback;
    zfautoObject userData;

public:
    _ZFP_zfSkinApplyData(void)
    : objIdentity(), skinKey(), skinChangeCallback(zfnull), userData(zfautoObjectNull)
    {
    }
    _ZFP_zfSkinApplyData(ZF_IN _ZFP_zfSkinApplyData const &ref)
    : objIdentity(ref.objIdentity), skinKey(ref.skinKey), skinChangeCallback(ref.skinChangeCallback), userData(ref.userData)
    {
    }
    _ZFP_zfSkinApplyData(ZF_IN const zfstring &objIdentity, ZF_IN const zfstring &skinKey, ZF_IN const ZFSkinOnChangeCallback &skinChangeCallback, ZF_IN zfautoObject const &userData)
    : objIdentity(objIdentity), skinKey(skinKey), skinChangeCallback(skinChangeCallback), userData(userData)
    {
    }
private:
    _ZFP_zfSkinApplyData &operator = (ZF_IN _ZFP_zfSkinApplyData const &ref);
};
typedef zfstlmap<ZFStyleable *, ZFCoreArrayPOD<_ZFP_zfSkinApplyData *> > _ZFP_zfSkinApplyDataMapType;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfSkinApplyDataHolder, ZFLevelZFFrameworkLow)
{
    this->objOnDeallocListener = ZFCallbackForRawFunction(zfself::objOnDealloc);
    this->skinOnChangeListener = ZFCallbackForRawFunction(zfself::skinOnChange);

    ZFObjectGlobalEventObserverAdd(ZFGlobalEvent::EventSkinOnChange(), this->skinOnChangeListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfSkinApplyDataHolder)
{
    ZFObjectGlobalEventObserverRemove(ZFGlobalEvent::EventSkinOnChange(), this->skinOnChangeListener);

    if(!this->dataMap.empty())
    {
        _ZFP_zfSkinApplyDataMapType tmp;
        tmp.swap(this->dataMap);
        for(_ZFP_zfSkinApplyDataMapType::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            it->first->toObject()->observerRemove(ZFObject::EventObjectBeforeDealloc(), this->objOnDeallocListener);
            for(zfindex i = 0; i < it->second.count(); ++i)
            {
                zfdelete(it->second[i]);
            }
        }
    }
}
public:
    _ZFP_zfSkinApplyDataMapType dataMap;
    ZFListener objOnDeallocListener;
    ZFListener skinOnChangeListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(objOnDealloc)
    {
        zfSkinApplyCancel(listenerData.sender->to<ZFStyleable *>(), zfnull);
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(skinOnChange)
    {
        _ZFP_zfSkinApplyDataMapType &m = ZF_GLOBAL_INITIALIZER_INSTANCE(zfSkinApplyDataHolder)->dataMap;
        for(_ZFP_zfSkinApplyDataMapType::iterator it = m.begin(); it != m.end(); ++it)
        {
            for(zfindex i = 0; i < it->second.count(); ++i)
            {
                _ZFP_zfSkinApplyData &d = *(it->second[i]);
                d.skinChangeCallback(it->first, d.objIdentity, d.skinKey, zfSkin(d.skinKey).to<ZFStyleable *>(), d.userData.toObject());
            }
        }
    }
ZF_GLOBAL_INITIALIZER_END(zfSkinApplyDataHolder)

void zfSkinApply(ZF_IN ZFStyleable *obj,
                 ZF_IN const zfchar *objIdentity,
                 ZF_IN const zfchar *skinKey,
                 ZF_IN ZFSkinOnChangeCallback skinChangeCallback,
                 ZF_IN_OPT ZFObject *userData /* = zfnull */)
{
    if(obj == zfnull || objIdentity == zfnull || skinKey == zfnull || skinChangeCallback == zfnull)
    {
        return ;
    }

    _ZFP_zfSkinApplyDataMapType &m = ZF_GLOBAL_INITIALIZER_INSTANCE(zfSkinApplyDataHolder)->dataMap;
    ZFCoreArrayPOD<_ZFP_zfSkinApplyData *> *dataList = zfnull;
    _ZFP_zfSkinApplyDataMapType::iterator it = m.find(obj);
    if(it != m.end())
    {
        dataList = &(it->second);
    }
    else
    {
        obj->toObject()->observerAdd(ZFObject::EventObjectBeforeDealloc(),
                         ZF_GLOBAL_INITIALIZER_INSTANCE(zfSkinApplyDataHolder)->objOnDeallocListener);
        dataList = &(m[obj] = ZFCoreArrayPOD<_ZFP_zfSkinApplyData *>());
    }
    _ZFP_zfSkinApplyData *d = zfnull;
    for(zfindex i = 0; i < dataList->count(); ++i)
    {
        if(dataList->get(i)->objIdentity.compare(objIdentity) == 0)
        {
            d = dataList->get(i);
            break;
        }
    }
    if(d == zfnull)
    {
        d = zfnew(_ZFP_zfSkinApplyData);
        dataList->add(d);
        d->objIdentity = objIdentity;
    }
    d->skinKey = skinKey;
    d->skinChangeCallback = skinChangeCallback;
    d->userData = zfautoObjectCreateMarkCached(userData);
    skinChangeCallback(obj, d->objIdentity, d->skinKey, zfSkin(d->skinKey).to<ZFStyleable *>(), d->userData.toObject());
}
void zfSkinApplyCancel(ZF_IN ZFStyleable *obj,
                       ZF_IN const zfchar *objIdentity)
{
    _ZFP_zfSkinApplyDataMapType &m = ZF_GLOBAL_INITIALIZER_INSTANCE(zfSkinApplyDataHolder)->dataMap;
    _ZFP_zfSkinApplyDataMapType::iterator it = m.find(obj);
    if(it != m.end())
    {
        if(objIdentity == zfnull)
        {
            for(zfindex i = 0; i < it->second.count(); ++i)
            {
                zfdelete(it->second[i]);
            }
            it->second.removeAll();
        }
        else
        {
            for(zfindex i = 0; i < it->second.count(); ++i)
            {
                if(it->second[i]->objIdentity.compare(objIdentity) == 0)
                {
                    zfdelete(it->second[i]);
                    it->second.remove(i);
                    break;
                }
            }
        }
        if(it->second.isEmpty())
        {
            it->first->toObject()->observerRemove(ZFObject::EventObjectBeforeDealloc(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(zfSkinApplyDataHolder)->objOnDeallocListener);
            m.erase(it);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

