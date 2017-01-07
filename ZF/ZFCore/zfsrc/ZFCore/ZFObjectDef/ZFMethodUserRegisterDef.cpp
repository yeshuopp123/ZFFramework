/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFMethodUserRegisterDef.h"
#include "ZFClassDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFMethodUserUnregister(ZF_IN const ZFClass *cls,
                                 ZF_IN const zfchar *methodName,
                                 ZF_IN const zfchar *methodId)
{
    ZFCoreMutexLocker();
    const ZFMethod *method = cls->methodForNameIgnoreParent(methodName, methodId);
    if(method == zfnull || !method->methodIsUserRegister())
    {
        return ;
    }
    cls->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_methodUnregister(method);
    _ZFP_ZFMethodInstanceCleanup(zfstringWithFormat(
            zfText("%s::%s_%s"),
            cls->className(),
            methodName ? methodName : zfText(""),
            methodId ? methodId : zfText("")
        ));
}

ZF_NAMESPACE_GLOBAL_END

