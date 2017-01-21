/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFString.h
 * @brief protocol for ZFString
 */

#ifndef _ZFI_ZFProtocolZFString_h_
#define _ZFI_ZFProtocolZFString_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFString.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFString
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFString)
public:
    /**
     * @brief create a new instance of nativeString, init with s
     *
     * newly created native string would be released by #nativeStringRelease
     */
    virtual void *nativeStringCreate(ZF_IN_OPT const zfchar *s = zfnull) zfpurevirtual;

    /**
     * @brief retain nativeString, return the retained or new instance if necessary
     *
     * unlike #nativeStringCreate,
     * this method just retain the nativeString for performance,
     * however, if your impl does not have retain count logic,
     * you may simply deep copy to create new instance\n
     * retained nativeString would be cleaned up by nativeStringRelease
     */
    virtual void *nativeStringRetain(ZF_IN void *nativeString) zfpurevirtual;
    /**
     * @brief see #nativeStringRetain
     */
    virtual void nativeStringRelease(ZF_IN void *nativeString) zfpurevirtual;

    /**
     * @brief get string from nativeString
     *
     * result would be cached and #stringValueAccessCleanup would be called if necessary,
     * so there's no need for implementation to care about performance and clean up
     */
    virtual void stringValueAccess(ZF_IN void *nativeString,
                                   ZF_OUT const zfchar *&stringValue,
                                   ZF_OUT void *&stringValueToken) zfpurevirtual;
    /**
     * @brief see #stringValueAccess
     */
    virtual void stringValueAccessCleanup(ZF_IN void *nativeString,
                                          ZF_IN const zfchar *stringValue,
                                          ZF_IN void *stringValueToken) zfpurevirtual;

    /**
     * @brief see #ZFString::toUTF8
     */
    virtual zfbool toUTF8(ZF_OUT zfstringA &result,
                          ZF_IN const void *s,
                          ZF_IN ZFStringEncodingEnum srcEncoding) zfpurevirtual;
    /**
     * @brief see #ZFString::toUTF16
     */
    virtual zfbool toUTF16(ZF_OUT zfstringW &result,
                           ZF_IN const void *s,
                           ZF_IN ZFStringEncodingEnum srcEncoding) zfpurevirtual;
    /**
     * @brief see #ZFString::toUTF16BE
     */
    virtual zfbool toUTF16BE(ZF_OUT zfstringW &result,
                             ZF_IN const void *s,
                             ZF_IN ZFStringEncodingEnum srcEncoding) zfpurevirtual;

    /**
     * @brief see #ZFString::wordCount
     */
    virtual zfindex wordCountOfUTF8(ZF_IN const zfcharA *utf8String) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFString)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFString_h_

