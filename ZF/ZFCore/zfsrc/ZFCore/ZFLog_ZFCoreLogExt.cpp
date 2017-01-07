/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFLog.h"
#include "ZFString.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static void _ZFP_ZFLog_ZFCoreLogExtFunc(ZF_IN const zfchar *callerFile,
                                        ZF_IN const zfchar *callerFunction,
                                        ZF_IN zfindex callerLine)
{
    if(zfLogStackTraceAvailable())
    {
        zfCoreLogTrim(ZFStringZ2A(zfLogStackTrace(zfnull, 2).cString()));
    }
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLog_ZFCoreLogExt, ZFLevelZFFrameworkNormal)
{
    zfCoreCriticalErrorCallbackAdd(_ZFP_ZFLog_ZFCoreLogExtFunc);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLog_ZFCoreLogExt)
{
    zfCoreCriticalErrorCallbackRemove(_ZFP_ZFLog_ZFCoreLogExtFunc);
}
ZF_GLOBAL_INITIALIZER_END(ZFLog_ZFCoreLogExt)

ZF_NAMESPACE_GLOBAL_END

