/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreStaticInitializerDef.h"
#include "ZFCoreArrayDef.h"
#include "ZFCoreMutexDef.h"
#include "ZFCorePointerDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassLikePOD _ZFP_ZFStaticInitializerItemData
{
public:
    zfindex refCount;
    zfstring name;
    _ZFP_ZFStaticInitializerDummyBase *instance;
public:
    _ZFP_ZFStaticInitializerItemData(void)
    : refCount(1)
    , name()
    , instance(zfnull)
    {
    }
    ~_ZFP_ZFStaticInitializerItemData(void)
    {
        if(this->instance != zfnull)
        {
            zfdelete(this->instance);
        }
    }
};
#define _ZFP_ZFStaticInitializerDataArrayType ZFCoreArray<ZFCorePointerForObject<_ZFP_ZFStaticInitializerItemData *> >

zfclassNotPOD _ZFP_ZFStaticInitializerData
{
public:
    _ZFP_ZFStaticInitializerDataArrayType data;

public:
    _ZFP_ZFStaticInitializerDummyBase *&access(ZF_IN const zfchar *name,
                                               ZF_IN _ZFP_ZFStaticInitializerConstructor constructor)
    {
        for(zfindex i = 0; i < this->data.count(); ++i)
        {
            _ZFP_ZFStaticInitializerItemData &itemData = *(this->data[i].pointerValueGet());
            if(zfscmpTheSame(itemData.name.cString(), name))
            {
                ++(itemData.refCount);
                return itemData.instance;
            }
        }
        _ZFP_ZFStaticInitializerItemData *itemData = zfnew(_ZFP_ZFStaticInitializerItemData);
        itemData->name = name;
        itemData->instance = constructor();
        this->data.add(ZFCorePointerForObject<_ZFP_ZFStaticInitializerItemData *>(itemData));
        return itemData->instance;
    }
    void cleanup(ZF_IN const zfchar *name)
    {
        for(zfindex i = 0; i < this->data.count(); ++i)
        {
            _ZFP_ZFStaticInitializerItemData &itemData = *(this->data[i].pointerValueGet());
            if(zfscmpTheSame(itemData.name.cString(), name))
            {
                --(itemData.refCount);
                if(itemData.refCount == 0)
                {
                    this->data.remove(i);
                }
                break;
            }
        }
    }
};
static _ZFP_ZFStaticInitializerData &_ZFP_ZFStaticInitializerDataRef_(void)
{
    static _ZFP_ZFStaticInitializerData d;
    return d;
}
#define _ZFP_ZFStaticInitializerDataRef _ZFP_ZFStaticInitializerDataRef_()

_ZFP_ZFStaticInitializerDummyBase *&_ZFP_ZFStaticInitializerInstanceAccess(ZF_IN const zfchar *name,
                                                                           ZF_IN _ZFP_ZFStaticInitializerConstructor constructor)
{
    ZFCoreMutexLocker();
    _ZFP_ZFStaticInitializerData &instance = _ZFP_ZFStaticInitializerDataRef;
    return instance.access(name, constructor);
}
void _ZFP_ZFStaticInitializerInstanceCleanup(ZF_IN const zfchar *name)
{
    ZFCoreMutexLocker();
    _ZFP_ZFStaticInitializerData &instance = _ZFP_ZFStaticInitializerDataRef;
    instance.cleanup(name);
}

ZF_NAMESPACE_GLOBAL_END

