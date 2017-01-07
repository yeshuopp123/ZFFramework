/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFLog_StackTrace.h"
#include "protocol/ZFProtocolZFLogStackTrace.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfbool zfLogStackTraceAvailable(void)
{
    return (ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull);
}

void zfLogStackTrace(ZF_OUT zfstring &ret,
                     ZF_IN_OPT const zfchar *prefix /* = zfnull */,
                     ZF_IN_OPT zfindex ignoreLevel /* = 0 */,
                     ZF_IN_OPT zfindex maxLevel /* = 20 */)
{
    if(ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull)
    {
        ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace)->stackTrace(ret, prefix, ignoreLevel + 2, maxLevel);
    }
    else
    {
        const zfchar *fixedPrefix = ((prefix == zfnull) ? zfText("") : prefix);

        ret += fixedPrefix;
        ret += zfText("======================== stack begin =======================");
        ret += '\n';

        ret += fixedPrefix;
        ret += zfText("| zfLogStackTrace is currently unsupported");
        ret += '\n';

        ret += fixedPrefix;
        ret += zfText("========================  stack end  =======================");
        ret += '\n';
    }
}
zfstring zfLogStackTrace(ZF_IN_OPT const zfchar *prefix /* = zfnull */,
                         ZF_IN_OPT zfindex ignoreLevel /* = 0 */,
                         ZF_IN_OPT zfindex maxLevel /* = 20 */)
{
    zfstring ret;
    if(ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull)
    {
        ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace)->stackTrace(ret, prefix, ignoreLevel + 2, maxLevel);
    }
    else
    {
        const zfchar *fixedPrefix = ((prefix == zfnull) ? zfText("") : prefix);

        ret += fixedPrefix;
        ret += zfText("======================== stack begin =======================");
        ret += '\n';

        ret += fixedPrefix;
        ret += zfText("| zfLogStackTrace is currently unsupported");
        ret += '\n';

        ret += fixedPrefix;
        ret += zfText("========================  stack end  =======================");
        ret += '\n';
    }
    return ret;
}

void zfLogCallerInfo(ZF_OUT zfstring &ret, ZF_IN_OPT zfindex ignoreLevel /* = 0 */)
{
    if(ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull)
    {
        ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace)->callerInfo(ret, ignoreLevel + 2);
    }
    else
    {
        ret += zfText("<zfLogCallerInfo is currently unsupported>");
    }
}
zfstring zfLogCallerInfo(ZF_IN_OPT zfindex ignoreLevel /* = 0 */)
{
    zfstring ret;
    if(ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull)
    {
        ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace)->callerInfo(ret, ignoreLevel + 2);
    }
    else
    {
        ret += zfText("<zfLogCallerInfo is currently unsupported>");
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

