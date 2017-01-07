/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIScroller.h"
#include "ZFUIScrollerDefault.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIScroller)

static const ZFClass *_ZFP_ZFUIScroller_cls = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIScrollerRegister, ZFLevelZFFrameworkNormal)
{
    _ZFP_ZFUIScroller_cls = ZFUIScrollerDefault::ClassData();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIScrollerRegister)
void ZFUIScrollerClassSet(ZF_IN const ZFClass *cls)
{
    if(cls != zfnull && !cls->classIsTypeOf(ZFUIScroller::ClassData()))
    {
        zfCoreLog(zfTextA("class %s not type of %s"),
            zfsCoreZ2A(cls->className()),
            zfsCoreZ2A(ZFUIScroller::ClassData()->className()));
        return ;
    }

    if(cls == zfnull)
    {
        cls = ZFUIScrollerDefault::ClassData();
    }
    _ZFP_ZFUIScroller_cls = cls;
}
const ZFClass *ZFUIScrollerClass(void)
{
    return _ZFP_ZFUIScroller_cls;
}

ZF_NAMESPACE_GLOBAL_END

