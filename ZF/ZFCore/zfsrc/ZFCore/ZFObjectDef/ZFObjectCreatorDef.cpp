/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFObjectCreatorDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObjectCreatorDataHolder, ZFLevelZFFrameworkNormal)
{
}
public:
    ZFCoreMap creatorMap; // _ZFP_ZFObjectCreatorCallback *
ZF_GLOBAL_INITIALIZER_END(ZFObjectCreatorDataHolder)
#define _ZFP_ZFObjectCreatorMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectCreatorDataHolder)->creatorMap)

zfautoObject ZFObjectCreate(ZF_IN const zfchar *type,
                            ZF_IN const zfchar *data)
{
    _ZFP_ZFObjectCreatorCallback *creatorCallback = _ZFP_ZFObjectCreatorMap.get<_ZFP_ZFObjectCreatorCallback *>(type);
    if(creatorCallback == zfnull)
    {
        return zfautoObjectNull;
    }
    return (*creatorCallback)(data);
}

void ZFObjectCreatorGetAllType(ZF_OUT ZFCoreArray<const zfchar *> &ret)
{
    _ZFP_ZFObjectCreatorMap.allKey(ret);
}

void _ZFP_ZFObjectCreatorRegister(ZF_IN const zfchar *type,
                                  ZF_IN _ZFP_ZFObjectCreatorCallback creatorCallback)
{
    _ZFP_ZFObjectCreatorMap.set(type, ZFCorePointerForObject<_ZFP_ZFObjectCreatorCallback *>(zfnew(_ZFP_ZFObjectCreatorCallback, creatorCallback)));
}
void _ZFP_ZFObjectCreatorUnregister(ZF_IN const zfchar *type)
{
    _ZFP_ZFObjectCreatorMap.remove(type);
}

ZF_NAMESPACE_GLOBAL_END

