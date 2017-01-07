/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFObjectNullDef.h"
#include "ZFSerializableDef.h"
#include "ZFCopyableDef.h"
#include "ZFObjectSmartPointerDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFNull : zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFNull, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable, ZFCopyable)

public:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        ret += zfText("zfnullObject");
    }
    zfoverride
    virtual zfidentity objectHash(void)
    {
        return zfidentityInvalid;
    }
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj)
    {
        if(this == anotherObj || (anotherObj != zfnull && anotherObj->classData() == ZFNull::ClassData()))
        {
            return ZFCompareTheSame;
        }
        return ZFCompareUncomparable;
    }
    zfoverride
    virtual void tagSet(ZF_IN const zfchar *key, ZF_IN ZFObject *tag)
    {
        zfCoreCriticalMessageTrim(zfTextA("setting tag to zfnullObject is not allowed, key: %s"), key);
    }

private:
    ZFMETHOD_DECLARE_DETAIL_0(private, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst,
                              zfautoObject, ZFSerializableKeyword_serializableNewInstanceSig, ZFMethodNoId)
    {
        return zfautoObjectCreate(zfnullObject);
    }

public:
    zfoverride
    virtual zfautoObject copy(void)
    {
        return zfautoObjectCreate(zfnullObject);
    }
};
ZFOBJECT_REGISTER(ZFNull)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfnullObjHolder, ZFLevelZFFrameworkHigh)
{
    this->nullObject = zfAlloc(ZFNull);
    this->nullObject->objectCachedSet(zftrue);
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfnullObjHolder)
{
    this->nullObject->objectCachedSet(zffalse);
    zfRelease(this->nullObject);
    this->nullObject = zfnull;
}
public:
    ZFObject *nullObject;
ZF_GLOBAL_INITIALIZER_END(zfnullObjHolder)
ZFObject *const &_ZFP_zfnullObject(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(zfnullObjHolder)->nullObject;
}

ZF_NAMESPACE_GLOBAL_END

