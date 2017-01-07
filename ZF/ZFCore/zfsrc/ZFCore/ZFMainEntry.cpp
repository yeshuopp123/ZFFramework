/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFMainEntry.h"
#include "ZFArray.h"
#include "ZFString.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static _ZFP_ZFMainFuncType &_ZFP_ZFMainFunc_(void)
{
    static _ZFP_ZFMainFuncType _func = zfnull;
    return _func;
}
#define _ZFP_ZFMainFunc (_ZFP_ZFMainFunc_())
void _ZFP_ZFMainRegister(ZF_IN _ZFP_ZFMainFuncType func)
{
    _ZFP_ZFMainFunc = func;
}
zfint ZFMainExecute(ZF_IN_OPT ZFArray *params)
{
    _ZFP_ZFMainFuncType &func = _ZFP_ZFMainFunc;
    if(func != zfnull)
    {
        return func(params);
    }
    else
    {
        zfCoreLog(zfTextA("ZFMAIN_ENTRY not set"));
        return -1;
    }
}

int ZFMainCommonEntry(ZF_IN int argc /* = 0 */, char **argv /* = zfnull */)
{
    ZFFrameworkInit();
    ZFArrayEditable *params = zfAllocWithoutLeakTest(ZFArrayEditable);

    for(int i = 0; i < argc; ++i)
    {
        ZFString *s = zfAllocWithoutLeakTest(ZFString, ZFStringA2Z(argv[i]));
        params->add(s);
        zfReleaseWithoutLeakTest(s);
    }

    zfint result = ZFMainExecute(params);
    zfReleaseWithoutLeakTest(params);
    ZFFrameworkCleanup();
    return result;
}

ZF_NAMESPACE_GLOBAL_END

