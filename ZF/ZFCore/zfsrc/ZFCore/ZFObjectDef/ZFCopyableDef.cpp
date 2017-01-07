/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCopyableDef.h"
#include "ZFSerializableDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFCopyable)

zfautoObject ZFCopyable::copy(void)
{
    zfautoObject ret = this->classData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    if(ret != zfautoObjectNull)
    {
        ZFCastZFObjectUnchecked(zfself *, ret.toObject())->copyableOnCopyFrom(this->toObject());
        if(ret.toObject()->classData()->classIsTypeOf(ZFSerializable::ClassData()))
        {
            ZFCastZFObjectUnchecked(ZFSerializable *, ret.toObject())->serializableCopyInfoFrom(ZFCastZFObjectUnchecked(ZFSerializable *, this));
        }
    }
    return ret;
}

void ZFCopyable::copyFrom(ZF_IN ZFObject *anotherObj)
{
    if(anotherObj != zfnull && anotherObj != this->toObject() && anotherObj->classData() == this->classData())
    {
        this->copyableOnCopyFrom(anotherObj);
        if(this->classData()->classIsTypeOf(ZFSerializable::ClassData()))
        {
            ZFCastZFObjectUnchecked(ZFSerializable *, this)->serializableCopyInfoFrom(ZFCastZFObjectUnchecked(ZFSerializable *, anotherObj));
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

