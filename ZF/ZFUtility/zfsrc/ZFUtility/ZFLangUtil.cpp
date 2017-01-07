/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFLangUtil.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassLikePOD _ZFP_zfLangApplyData
{
public:
    zfstring objIdentity;
    zfstring langKey;
    ZFLangOnChangeCallback langChangeCallback;
    zfautoObject userData;

public:
    _ZFP_zfLangApplyData(void)
    : objIdentity(), langKey(), langChangeCallback(zfnull), userData(zfautoObjectNull)
    {
    }
    _ZFP_zfLangApplyData(ZF_IN _ZFP_zfLangApplyData const &ref)
    : objIdentity(ref.objIdentity), langKey(ref.langKey), langChangeCallback(ref.langChangeCallback), userData(ref.userData)
    {
    }
    _ZFP_zfLangApplyData(ZF_IN const zfstring &objIdentity, ZF_IN const zfstring &langKey, ZF_IN const ZFLangOnChangeCallback &langChangeCallback, ZF_IN zfautoObject const &userData)
    : objIdentity(objIdentity), langKey(langKey), langChangeCallback(langChangeCallback), userData(userData)
    {
    }
private:
    _ZFP_zfLangApplyData &operator = (ZF_IN _ZFP_zfLangApplyData const &ref);
};
typedef zfstlmap<ZFObject *, ZFCoreArrayPOD<_ZFP_zfLangApplyData *> > _ZFP_zfLangApplyDataMapType;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfLangApplyDataHolder, ZFLevelZFFrameworkLow)
{
    this->objOnDeallocListener = ZFCallbackForRawFunction(zfself::objOnDealloc);
    this->langOnChangeListener = ZFCallbackForRawFunction(zfself::langOnChange);

    ZFObjectGlobalEventObserverAdd(ZFGlobalEvent::EventLangOnChange(), this->langOnChangeListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfLangApplyDataHolder)
{
    ZFObjectGlobalEventObserverRemove(ZFGlobalEvent::EventLangOnChange(), this->langOnChangeListener);

    if(!this->dataMap.empty())
    {
        _ZFP_zfLangApplyDataMapType tmp;
        tmp.swap(this->dataMap);
        for(_ZFP_zfLangApplyDataMapType::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            it->first->observerRemove(ZFObject::EventObjectBeforeDealloc(), this->objOnDeallocListener);
            for(zfindex i = 0; i < it->second.count(); ++i)
            {
                zfdelete(it->second[i]);
            }
        }
    }
}
public:
    _ZFP_zfLangApplyDataMapType dataMap;
    ZFListener objOnDeallocListener;
    ZFListener langOnChangeListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(objOnDealloc)
    {
        zfLangApplyCancel(listenerData.sender, zfnull);
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(langOnChange)
    {
        _ZFP_zfLangApplyDataMapType &m = ZF_GLOBAL_INITIALIZER_INSTANCE(zfLangApplyDataHolder)->dataMap;
        for(_ZFP_zfLangApplyDataMapType::iterator it = m.begin(); it != m.end(); ++it)
        {
            for(zfindex i = 0; i < it->second.count(); ++i)
            {
                _ZFP_zfLangApplyData &d = *(it->second[i]);
                d.langChangeCallback(it->first, d.objIdentity, d.langKey, zfLang(d.langKey), d.userData.toObject());
            }
        }
    }
ZF_GLOBAL_INITIALIZER_END(zfLangApplyDataHolder)

void zfLangApply(ZF_IN ZFObject *obj,
                 ZF_IN const zfchar *objIdentity,
                 ZF_IN const zfchar *langKey,
                 ZF_IN ZFLangOnChangeCallback langChangeCallback,
                 ZF_IN_OPT ZFObject *userData /* = zfnull */)
{
    if(obj == zfnull || objIdentity == zfnull || langKey == zfnull || langChangeCallback == zfnull)
    {
        return ;
    }

    _ZFP_zfLangApplyDataMapType &m = ZF_GLOBAL_INITIALIZER_INSTANCE(zfLangApplyDataHolder)->dataMap;
    ZFCoreArrayPOD<_ZFP_zfLangApplyData *> *dataList = zfnull;
    _ZFP_zfLangApplyDataMapType::iterator it = m.find(obj);
    if(it != m.end())
    {
        dataList = &(it->second);
    }
    else
    {
        obj->observerAdd(ZFObject::EventObjectBeforeDealloc(),
                         ZF_GLOBAL_INITIALIZER_INSTANCE(zfLangApplyDataHolder)->objOnDeallocListener);
        dataList = &(m[obj] = ZFCoreArrayPOD<_ZFP_zfLangApplyData *>());
    }
    _ZFP_zfLangApplyData *d = zfnull;
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
        d = zfnew(_ZFP_zfLangApplyData);
        dataList->add(d);
        d->objIdentity = objIdentity;
    }
    d->langKey = langKey;
    d->langChangeCallback = langChangeCallback;
    d->userData = zfautoObjectCreateMarkCached(userData);
    langChangeCallback(obj, d->objIdentity, d->langKey, zfLang(d->langKey), d->userData.toObject());
}
void zfLangApplyCancel(ZF_IN ZFObject *obj,
                       ZF_IN const zfchar *objIdentity)
{
    _ZFP_zfLangApplyDataMapType &m = ZF_GLOBAL_INITIALIZER_INSTANCE(zfLangApplyDataHolder)->dataMap;
    _ZFP_zfLangApplyDataMapType::iterator it = m.find(obj);
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
            it->first->observerRemove(
                ZFObject::EventObjectBeforeDealloc(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(zfLangApplyDataHolder)->objOnDeallocListener);
            m.erase(it);
        }
    }
}

// ============================================================
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfLangApplyProperty_zfstring
{
public:
    static void _ZFP_action(ZF_IN ZFObject *obj,
                            ZF_IN const zfchar *objIdentity,
                            ZF_IN const zfchar *langKey,
                            ZF_IN const zfchar *langValue,
                            ZF_IN_OPT ZFObject *userData)
    {
        userData->to<ZFPointerHolder *>()->holdedDataPointer<const ZFProperty *>()
            ->setterMethod()->execute<void, const zfstring &>(obj, langValue);
    }
};
#define _ZFP_zfLangApplyProperty_zfstring_objIdentity(property) \
    zfstringWithFormat(zfText("Property_zfstring:%s"), property->objectInfo().cString())
void zfLangApplyProperty_zfstring(ZF_IN ZFObject *obj,
                                  ZF_IN const ZFProperty *property,
                                  ZF_IN const zfchar *langKey)
{
    if(obj == zfnull || property == zfnull || langKey == zfnull
        || !obj->classData()->classIsTypeOf(property->ownerClass()))
    {
        return ;
    }

    zfLangApply(obj,
        _ZFP_zfLangApplyProperty_zfstring_objIdentity(property),
        langKey,
        _ZFP_zfLangApplyProperty_zfstring::_ZFP_action,
        zflineAllocWithoutLeakTest(ZFPointerHolder, property));
}
void zfLangApplyCancelProperty_zfstring(ZF_IN ZFObject *obj,
                                        ZF_IN const ZFProperty *property)
{
    zfLangApplyCancel(obj, _ZFP_zfLangApplyProperty_zfstring_objIdentity(property));
}

ZF_NAMESPACE_GLOBAL_END

