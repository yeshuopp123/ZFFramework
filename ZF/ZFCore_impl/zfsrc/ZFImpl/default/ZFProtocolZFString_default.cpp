/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFString.h"
#include "ZFCore/ZFString.h"
#include "ZFImpl/tools/UTFCodeUtil/UTFCodeUtil.h"

#include <string.h>

#if !ZF_ENV_sys_Android

#if ZF_ENV_sys_WindowsCE
    #if (_WIN32_WCE < 0x500) && (defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))
        #pragma comment(lib, "ccrtrtti.lib")
    #endif // #if (_WIN32_WCE < 0x500) && (defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))
#endif // #if !ZF_ENV_sys_WindowsCE #else

ZF_NAMESPACE_GLOBAL_BEGIN
ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFStringImpl_default, ZFString, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("ZFFramework:zfstring"))
public:
    virtual void *nativeStringCreate(ZF_IN_OPT const zfchar *s = zfnull)
    {
        zfstring *nativeString = zfnew(zfstring);
        if(s != zfnull)
        {
            *nativeString = s;
        }
        return nativeString;
    }

    virtual void *nativeStringRetain(ZF_IN void *nativeString)
    {
        // we don't have retain count logic
        return zfnew(zfstring, *ZFCastStatic(zfstring *, nativeString));
    }
    virtual void nativeStringRelease(ZF_IN void *nativeString)
    {
        zfdelete(ZFCastStatic(zfstring *, nativeString));
    }

    virtual void stringValueAccess(ZF_IN void *nativeString,
                                   ZF_OUT const zfchar *&stringValue,
                                   ZF_OUT void *&stringValueToken)
    {
        stringValue = ZFCastStatic(zfstring *, nativeString)->cString();
    }
    virtual void stringValueAccessCleanup(ZF_IN void *nativeString,
                                          ZF_IN const zfchar *stringValue,
                                          ZF_IN void *stringValueToken)
    {
        // nothing to do
    }

    virtual zfbool toUTF8(ZF_OUT zfstringA &result,
                          ZF_IN const void *s,
                          ZF_IN ZFStringEncodingEnum srcEncoding)
    {
        switch(srcEncoding)
        {
            case ZFStringEncoding::e_UTF8:
                result += (const zfcharA *)s;
                return zftrue;
            case ZFStringEncoding::e_UTF16LE:
            {
                zfcharA *sUTF8 = UTFCodeUtil::UTF16ToUTF8((const zfcharW *)s);
                if(sUTF8 == zfnull) {return zffalse;}
                result += sUTF8;
                zffree(sUTF8);
                return zftrue;
            }
            case ZFStringEncoding::e_UTF16BE:
            {
                zfcharW *sUTF16 = UTFCodeUtil::UTF16BEToUTF16((const zfcharW *)s);
                if(sUTF16 == zfnull) {return zffalse;}
                zfcharA *sUTF8 = UTFCodeUtil::UTF16ToUTF8(sUTF16);
                zffree(sUTF16);
                if(sUTF8 == zfnull) {return zffalse;}
                result += sUTF8;
                zffree(sUTF8);
                return zftrue;
            }
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    virtual zfbool toUTF16(ZF_OUT zfstringW &result,
                           ZF_IN const void *s,
                           ZF_IN ZFStringEncodingEnum srcEncoding)
    {
        switch(srcEncoding)
        {
            case ZFStringEncoding::e_UTF8:
            {
                zfcharW *sUTF16 = UTFCodeUtil::UTF8ToUTF16((const zfcharA *)s);
                if(sUTF16 == zfnull) {return zffalse;}
                result += sUTF16;
                zffree(sUTF16);
                return zftrue;
            }
            case ZFStringEncoding::e_UTF16LE:
                result += (const zfcharW *)s;
                return zftrue;
            case ZFStringEncoding::e_UTF16BE:
            {
                zfcharW *sUTF16 = UTFCodeUtil::UTF16BEToUTF16((const zfcharW *)s);
                if(sUTF16 == zfnull) {return zffalse;}
                result += sUTF16;
                zffree(sUTF16);
                return zftrue;
            }
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    virtual zfbool toUTF16BE(ZF_OUT zfstringW &result,
                             ZF_IN const void *s,
                             ZF_IN ZFStringEncodingEnum srcEncoding)
    {
        switch(srcEncoding)
        {
            case ZFStringEncoding::e_UTF8:
            {
                zfcharW *sUTF16 = UTFCodeUtil::UTF8ToUTF16((const zfcharA *)s);
                if(sUTF16 == zfnull) {return zffalse;}
                zfcharW *sUTF16BE = UTFCodeUtil::UTF16ToUTF16BE(sUTF16);
                zffree(sUTF16);
                if(sUTF16BE == zfnull) {return zffalse;}
                result += sUTF16BE;
                zffree(sUTF16BE);
                return zftrue;
            }
            case ZFStringEncoding::e_UTF16LE:
            {
                zfcharW *sUTF16BE = UTFCodeUtil::UTF16ToUTF16BE((const zfcharW *)s);
                if(sUTF16BE == zfnull) {return zffalse;}
                result += sUTF16BE;
                zffree(sUTF16BE);
                return zftrue;
            }
            case ZFStringEncoding::e_UTF16BE:
                result += (const zfcharW *)s;
                return zftrue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    virtual zfindex wordCountOfUTF8(ZF_IN const zfcharA *utf8String)
    {
        return UTFCodeUtil::UTF8GetWordCount(utf8String);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFStringImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFStringImpl_default)

ZF_NAMESPACE_GLOBAL_END

#endif // #if !ZF_ENV_sys_Android

