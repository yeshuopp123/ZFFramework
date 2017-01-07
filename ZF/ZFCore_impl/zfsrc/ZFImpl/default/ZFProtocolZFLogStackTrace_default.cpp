/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFLogStackTrace.h"
#include "ZFCore/ZFLog_StackTrace.h"

#include "ZFImpl/_repo/stack_trace/stack.hpp"

#if ZF_EDD_STACK_TRACE_ENABLE

ZF_NAMESPACE_GLOBAL_BEGIN

/*
 * you must:
 * -  add ZF_EDD_STACK_TRACE to your precompiler
 * -  have DbgHelp support (DbgHelp.lib), if under Windows without MinGW
 * -  link "libbfd.a, libiberty.a, libimagehlp.a, libpsapi.a",
 *    and add "-gstabs" to compiler, if under Windows MinGW
 * -  add "-rdynamic" as compiler flags, add "-ldl" as linker flags, if under Posix
 * -  add LIBS and QMAKE_CXXFLAGS depending the OS, if under Qt
 *
 * also note that this may not work for release build
 */
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFLogStackTraceImpl_default, ZFLogStackTrace, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("edd_stacktrace"))
public:
    virtual void stackTrace(ZF_IN_OUT zfstring &ret,
                            ZF_IN_OPT const zfchar *prefix = zfnull,
                            ZF_IN_OPT zfindex ignoreLevel = 0,
                            ZF_IN_OPT zfindex maxLevel = 20)
    {
        const zfchar *fixedPrefix = ((prefix == zfnull) ? zfText("") : prefix);

        ret += fixedPrefix;
        ret += zfText("======================== stack begin =======================");
        ret += '\n';

        dbg::stack s;
        dbg::stack::const_iterator it = s.begin();
        ++it;
        for(zfindex i = 0; i < ignoreLevel && it != s.end(); ++i, ++it);
        for(zfindex i = 0; i < maxLevel && it != s.end(); ++i, ++it)
        {
            ret += fixedPrefix;
            ret += zfText("| ");
#if 0
            ZF_CALLER_PATH_TO_NAME_REF(ret, it->module.cString());
            ret += zfText("::");
#endif
            ret += this->trimFunc(it->function.c_str());
            ret += '\n';
        }

        ret += fixedPrefix;
        ret += zfText("========================  stack end  =======================");
    }
    virtual void callerInfo(ZF_IN_OUT zfstring &ret,
                            ZF_IN_OPT zfindex ignoreLevel = 0)
    {
        dbg::stack s;
        dbg::stack::const_iterator it = s.begin();
        ++it;
        for(zfindex i = 0; i < ignoreLevel && it != s.end(); ++i, ++it);
        if(it != s.end())
        {
            ret = zfText("[");
#if 0
            ZF_CALLER_PATH_TO_NAME_REF(ret, zfsCoreA2Z(it->module.cString()));
            ret += zfText("::");
#endif
            ret += zfsCoreA2Z(this->trimFunc(it->function.c_str()).cString());
            ret += zfText("]");
        }
    }
private:
    zfstring trimFunc(ZF_IN const zfchar *s)
    {
        zfstring ret(zfslen(s));
        const zfchar *last = (s - 1);
        zfbool namespaceMatched = zffalse;
        while(*s)
        {
            if(*s == ':' && *(s+1) == ':')
            {
                if(namespaceMatched)
                {
                    s += 2;
                }
                else
                {
                    namespaceMatched = zftrue;
                    last = s + 1;
                    s += 2;
                }
            }
            else if(!(
                (*s >= 'a' && *s <= 'z')
                || (*s >= 'A' && *s <= 'Z')
                || (*s >= '0' && *s <= '9')
                || *s == '_'
                ))
            {
                namespaceMatched = zffalse;
                ret.append(last + 1, s - last);
                last = s;
                ++s;
            }
            else
            {
                ++s;
            }
        }
        if(last != s - 1)
        {
            ret.append(last + 1, s - last - 1);
        }
        return ret;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFLogStackTraceImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFLogStackTraceImpl_default)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_EDD_STACK_TRACE_ENABLE

