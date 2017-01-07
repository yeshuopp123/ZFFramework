/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPropertyUserRegisterDef.h"
#include "ZFClassDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfautoObject _ZFP_I_ZFPropertyUserRegisterInitValueSetupCallbackHolder::create(ZF_IN _ZFP_ZFPropertyUserRegisterInitValueSetupCallback callback)
{
    zfblockedAllocWithoutLeakTest(_ZFP_I_ZFPropertyUserRegisterInitValueSetupCallbackHolder, holder);
    holder->callback = callback;
    return zfautoObjectCreateWithoutLeakTest(holder);
}

zfautoObject _ZFP_I_ZFPropertyUserRegisterDefaultImplValueHolder::create(ZF_IN const ZFCorePointerBase &p)
{
    zfblockedAllocWithoutLeakTest(_ZFP_I_ZFPropertyUserRegisterDefaultImplValueHolder, holder);
    holder->v = p.refNew();
    return zfautoObjectCreateWithoutLeakTest(holder);
}

void _ZFP_ZFPropertyUserUnregister(ZF_IN const ZFClass *cls,
                                   ZF_IN const zfchar *propertyName)
{
    ZFCoreMutexLocker();
    const ZFProperty *zfproperty = cls->propertyForNameIgnoreParent(propertyName);
    if(zfproperty == zfnull || !zfproperty->propertyIsUserRegister())
    {
        return ;
    }

    ZFMethodUserUnregister(cls, zfproperty->setterMethod()->methodName(), zfnull);
    ZFMethodUserUnregister(cls, zfproperty->getterMethod()->methodName(), zfnull);

    cls->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_propertyUnregister(zfproperty);
    _ZFP_ZFPropertyInstanceCleanup(zfstringWithFormat(
            zfText("%s::%s"),
            cls->className(),
            propertyName ? propertyName : zfText("")
        ));
}

ZF_NAMESPACE_GLOBAL_END

