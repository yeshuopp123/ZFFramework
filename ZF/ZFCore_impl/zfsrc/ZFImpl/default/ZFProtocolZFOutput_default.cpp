/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFOutput.h"
#include "ZFCore/ZFString.h"
#include <stdio.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFOutputImpl_default, ZFOutput, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("C:printf"))
public:
    virtual zfindex outputCoreLog(ZF_IN const zfcharA *s)
    {
        printf(zfTextA("%s"), s);
        return zfindexMax;
    }
    virtual zfindex outputLog(ZF_IN const zfchar *s, ZF_IN_OPT zfindex count = zfindexMax)
    {
        if(count == zfindexMax)
        {
            printf(zfTextA("%s"), ZFStringZ2A(s));
        }
        else
        {
            zfstring tmp(s, count);
            printf(zfTextA("%s"), ZFStringZ2A(tmp.cString()));
        }
        return zfindexMax;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFOutputImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFOutputImpl_default)

ZF_NAMESPACE_GLOBAL_END

