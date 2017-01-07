/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPropertyDeclareDef.h"
#include "ZFObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFProperty *ZFPropertyGet(ZF_IN const zfchar *clsName,
                                ZF_IN const zfchar *propertyName)
{
    const ZFClass *cls = ZFClass::classForName(clsName);
    if(cls != zfnull)
    {
        return cls->propertyForName(propertyName);
    }
    return zfnull;
}
const ZFProperty *ZFPropertyGet(ZF_IN const ZFClass *cls,
                                ZF_IN const zfchar *propertyName)
{
    if(cls != zfnull)
    {
        return cls->propertyForName(propertyName);
    }
    return zfnull;
}

void _ZFP_ZFPropertyCallbackGetInfoRetainDefault(ZF_IN const ZFProperty *property,
                                                 ZF_IN ZFObject *ownerObj,
                                                 ZF_IN_OUT zfstring &ret)
{
    ZFObjectInfoT(ret, property->callbackRetainGet(property, ownerObj));
}

ZF_NAMESPACE_GLOBAL_END

