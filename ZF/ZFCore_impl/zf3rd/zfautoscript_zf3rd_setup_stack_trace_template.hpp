#ifndef _ZFI_zf3rd_stack_trace_hpp_
#define _ZFI_zf3rd_stack_trace_hpp_

#include "ZFCore/protocol/ZFProtocolZFLogStackTrace.h"
#include "ZFCore/ZFLog_StackTrace.h"
#include "ZFImpl.h"

#if ZF_ENV_sys_iOS
    #undef ZF_EDD_STACK_TRACE
    #define ZF_EDD_STACK_TRACE 1
#endif

#if !ZF_EDD_STACK_TRACE
    #define ZF_EDD_STACK_TRACE_ENABLE 0
#else
    #if defined(_WIN32)
        #define ZF_EDD_STACK_TRACE_ENABLE 1
    #elif defined(__GNUC__)
        #if defined(__i386__) || defined(__amd64__)
            #define ZF_EDD_STACK_TRACE_ENABLE 1
        #elif defined(__ppc__)
            #define ZF_EDD_STACK_TRACE_ENABLE 1
        #else
            #define ZF_EDD_STACK_TRACE_ENABLE 0
        #endif
    #else
        #define ZF_EDD_STACK_TRACE_ENABLE 0
    #endif
#endif

#if ZF_EDD_STACK_TRACE_ENABLE
#include "../../../../zf3rd/_tmp/stack_trace/dbg/stack.hpp"
#endif

#endif // #ifndef _ZFI_zf3rd_stack_trace_hpp_

