/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCallbackSettingDef.h"
#include "ZFObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFCallbackSettingHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFCallbackSettingHolder, ZFObject)

public:
    ZFCorePointerBase *value;

public:
    virtual void objectOnDealloc(void)
    {
        if(this->value)
        {
            this->value->refDelete();
            this->value = zfnull;
        }
        zfsuper::objectOnDealloc();
    }
};

void _ZFP_ZFCallbackSettingSet(ZF_IN_OUT const ZFCallback &o, ZF_IN const zfchar *name, ZF_IN const ZFCorePointerBase &sp)
{
    if(sp.pointerValue() == zfnull)
    {
        ZFCallback(o).callbackTagSet(name, zfnull);
    }
    else
    {
        _ZFP_I_ZFCallbackSettingHolder *holder = zfAllocWithoutLeakTest(_ZFP_I_ZFCallbackSettingHolder);
        holder->value = sp.refNew();
        ZFCallback(o).callbackTagSet(name, holder);
        zfReleaseWithoutLeakTest(holder);
    }
}
const ZFCorePointerBase *_ZFP_ZFCallbackSettingGet(ZF_IN const ZFCallback &o, ZF_IN const zfchar *name)
{
    _ZFP_I_ZFCallbackSettingHolder *holder = o.callbackTagGet<_ZFP_I_ZFCallbackSettingHolder *>(name);
    return (holder ? holder->value : zfnull);
}

ZF_NAMESPACE_GLOBAL_END

