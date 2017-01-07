/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFMethodDeclareFuncDef.h"
#include "ZFClassDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFMethod *ZFMethodFuncGet(ZF_IN const zfchar *methodNamespace,
                                ZF_IN const zfchar *methodName,
                                ZF_IN_OPT const zfchar *methodId /* = zfnull */)
{
    return _ZFP_ZFMethodInstanceFind(_ZFP_ZFMethodFuncSig(methodNamespace, methodName, methodId));
}

zfstring _ZFP_ZFMethodFuncSig(ZF_IN const zfchar *methodNamespace,
                              ZF_IN const zfchar *methodName,
                              ZF_IN_OPT const zfchar *methodId /* = zfnull */)
{
    return zfstringWithFormat(
        zfText("%s::%s_%s"),
        methodNamespace ? methodNamespace : zfText(""),
        methodName ? methodName : zfText(""),
        methodId ? methodId : zfText("")
    );
}

ZF_NAMESPACE_GLOBAL_END

