/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreUtilMathDef.h"
#include "ZFCoreStaticInitializerDef.h"
#include <time.h>
#include <stdlib.h>

ZF_ENV_SENSITIVE
#if defined(WINCE)
    #include <Windows.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

const zffloat zffloatEpsilonT<zffloat>::v = zffloatEpsilon;
const zfdouble zffloatEpsilonT<zfdouble>::v = zfdoubleEpsilon;
const zflongdouble zffloatEpsilonT<zflongdouble>::v = zflongdoubleEpsilon;

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFCoreUtilMathDataHolder)
{
    ZF_ENV_SENSITIVE
    #if defined(WINCE)
        SYSTEMTIME st;
        ::GetLocalTime(&st);
        ::srand((zfuint)(
            st.wHour * 60 * 60 * 1000
            + st.wMinute * 60 * 1000
            + st.wSecond * 1000
            + st.wMilliseconds));
    #else
        ::srand((zfuint)time(zfnull));
    #endif
}
ZF_STATIC_INITIALIZER_END(ZFCoreUtilMathDataHolder)

zfuint zfmRand(void)
{
    return (zfuint)::rand();
}

ZF_NAMESPACE_GLOBAL_END

