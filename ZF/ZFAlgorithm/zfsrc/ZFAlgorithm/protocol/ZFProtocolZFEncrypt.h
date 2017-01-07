/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFEncrypt.h
 * @brief protocol for ZFEncrypt
 */

#ifndef _ZFI_ZFProtocolZFEncrypt_h_
#define _ZFI_ZFProtocolZFEncrypt_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFAlgorithm/ZFEncrypt.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFEncrypt
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFEncrypt)
public:
    /** @brief see #ZFEncrypt::encryptProtocol */
    virtual void encryptProtocol(ZF_OUT zfstring &ret) = 0;
    /** @brief see #ZFEncrypt::encryptProtocolVersion */
    virtual void encryptProtocolVersion(ZF_OUT zfstring &ret) = 0;

    /** @brief see #ZFEncrypt::encryptUpgrade */
    virtual zfbool encryptUpgrade(ZF_IN_OUT const ZFOutputCallback &output,
                                  ZF_IN const ZFInputCallback &input,
                                  ZF_IN const zfstring &key,
                                  ZF_IN const zfchar *encryptProtocolOld,
                                  ZF_IN const zfchar *encryptProtocolVersionOld)
    {
        // by default no upgrade supported
        return zffalse;
    }

public:
    /**
     * @brief see #ZFEncrypt::encryptCalcSize
     */
    virtual zfindex encryptCalcSize(ZF_IN zfindex srcLen, ZF_IN const zfstring &key) = 0;
    /**
     * @brief see #ZFEncrypt::decryptCalcSize
     */
    virtual zfindex decryptCalcSize(ZF_IN zfindex srcLen, ZF_IN const zfstring &key) = 0;

public:
    /**
     * @brief see #ZFEncrypt::encrypt
     *
     * result must contain printable chars only, encoded with base64 is recommended
     */
    virtual zfbool encrypt(ZF_IN_OUT const ZFOutputCallback &output, ZF_IN const ZFInputCallback &input, ZF_IN const zfstring &key) = 0;
    /**
     * @brief see #ZFEncrypt::decrypt
     */
    virtual zfbool decrypt(ZF_IN_OUT const ZFOutputCallback &output, ZF_IN const ZFInputCallback &input, ZF_IN const zfstring &key) = 0;
ZFPROTOCOL_INTERFACE_END(ZFEncrypt)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFEncrypt_h_

