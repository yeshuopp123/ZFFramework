/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFLogStackTrace.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFLogStackTraceImpl_sys_Android, ZFLogStackTrace, ZFProtocolLevel::e_SystemNormal)
public:
#if 0
    virtual void stackTrace(ZF_IN_OUT zfstring &ret,
                            ZF_IN_OPT const zfchar *prefix = zfnull,
                            ZF_IN_OPT zfindex ignoreLevel = 0,
                            ZF_IN_OPT zfindex maxLevel = 20)
    {
    }
    virtual void callerInfo(ZF_IN_OUT zfstring &ret,
                            ZF_IN_OPT zfindex ignoreLevel = 0)
    {
    }
#else
    virtual void stackTrace(ZF_IN_OUT zfstring &ret,
                            ZF_IN_OPT const zfchar *prefix = zfnull,
                            ZF_IN_OPT zfindex ignoreLevel = 0,
                            ZF_IN_OPT zfindex maxLevel = 20)
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
    virtual void callerInfo(ZF_IN_OUT zfstring &ret,
                            ZF_IN_OPT zfindex ignoreLevel = 0)
    {
        ret += zfText("<zfLogCallerInfo is currently unsupported>");
    }
#endif
ZFPROTOCOL_IMPLEMENTATION_END(ZFLogStackTraceImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFLogStackTraceImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif

