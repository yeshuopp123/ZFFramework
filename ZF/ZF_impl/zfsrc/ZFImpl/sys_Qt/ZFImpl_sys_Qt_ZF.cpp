/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZF.h"

#if ZF_ENV_sys_Qt

class _ZFP_ZFImpl_sys_Qt_ValueWrapper
{
public:
    ZFObject *value;
public:
    _ZFP_ZFImpl_sys_Qt_ValueWrapper(void)
    : value(zfnull)
    {
    }
    _ZFP_ZFImpl_sys_Qt_ValueWrapper(ZF_IN ZFObject *obj)
    : value(zfRetainWithoutLeakTest(obj))
    {
    }
    _ZFP_ZFImpl_sys_Qt_ValueWrapper(ZF_IN const _ZFP_ZFImpl_sys_Qt_ValueWrapper &ref)
    : value(zfRetainWithoutLeakTest(ref.value))
    {
    }
    ~_ZFP_ZFImpl_sys_Qt_ValueWrapper(void)
    {
        zfReleaseWithoutLeakTest(this->value);
    }
    _ZFP_ZFImpl_sys_Qt_ValueWrapper &operator = (ZF_IN const _ZFP_ZFImpl_sys_Qt_ValueWrapper &ref)
    {
        ZFPropertyChangeWithoutLeakTest(this->value, ref.value);
        return *this;
    }
};
Q_DECLARE_METATYPE(_ZFP_ZFImpl_sys_Qt_ValueWrapper);

ZF_NAMESPACE_GLOBAL_BEGIN
void ZFImpl_sys_Qt_QObjectTagSet(ZF_IN_OUT QObject *obj, ZF_IN const zfchar *name, ZF_IN QVariant const &tag)
{
    obj->setProperty(zfsCoreZ2A(name), tag);
}
QVariant ZFImpl_sys_Qt_QObjectTagGet(ZF_IN_OUT QObject *obj, ZF_IN const zfchar *name)
{
    return obj->property(zfsCoreZ2A(name));
}

void ZFImpl_sys_Qt_QObjectTagSetZFObject(ZF_IN_OUT QObject *obj, ZF_IN const zfchar *name, ZF_IN ZFObject *tag)
{
    if(tag == zfnull)
    {
        ZFImpl_sys_Qt_QObjectTagSet(obj, name, QVariant());
    }
    else
    {
        _ZFP_ZFImpl_sys_Qt_ValueWrapper valueWrapper(tag);
        ZFImpl_sys_Qt_QObjectTagSet(obj, name, QVariant::fromValue(valueWrapper));
    }
}
ZFObject *ZFImpl_sys_Qt_QObjectTagGetZFObject(ZF_IN_OUT QObject *obj, ZF_IN const zfchar *name)
{
    QVariant tag = ZFImpl_sys_Qt_QObjectTagGet(obj, name);
    return ((tag.isValid()) ? tag.value<_ZFP_ZFImpl_sys_Qt_ValueWrapper>().value : zfnull);
}

ZF_NAMESPACE_GLOBAL_END
#endif

