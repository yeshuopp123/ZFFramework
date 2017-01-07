/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFEncrypt.h
 * @brief encrypt util
 */

#ifndef _ZFI_ZFEncrypt_h_
#define _ZFI_ZFEncrypt_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief upgrade task data for #ZFEncryptUpgraderRegister
 */
zfclassLikePOD ZF_ENV_EXPORT ZFEncryptUpgradeTaskData
{
public:
    zfstring encryptProtocolOld; /**< @brief empty means support any */
    zfstring encryptProtocolVersionOld; /**< @brief empty means support any */
    zfstring encryptProtocol; /**< @brief empty means support any */
    zfstring encryptProtocolVersion; /**< @brief empty means support any */
    zfstring encryptKey; /**< @brief encrypt key */
    ZFInputCallback input; /**< @brief input encrypted data */
    ZFOutputCallback output; /**< @brief output decrypted data */
};
/**
 * @brief callback to upgrade encrypted data
 */
typedef ZFCallbackT<zfbool, const ZFEncryptUpgradeTaskData &> ZFEncryptUpgradeCallback;
/**
 * @brief register a callback to upgrade encrypted data
 */
extern ZF_ENV_EXPORT void ZFEncryptUpgraderRegister(ZF_IN const zfchar *encryptProtocolOld,
                                                    ZF_IN const zfchar *encryptProtocolVersionOld,
                                                    ZF_IN const zfchar *encryptProtocol,
                                                    ZF_IN const zfchar *encryptProtocolVersion,
                                                    ZF_IN const ZFEncryptUpgradeCallback &upgradeCallback);
/**
 * @brief see #ZFEncryptUpgraderRegister
 */
extern ZF_ENV_EXPORT void ZFEncryptUpgraderUnregister(ZF_IN const zfchar *encryptProtocolOld,
                                                      ZF_IN const zfchar *encryptProtocolVersionOld,
                                                      ZF_IN const zfchar *encryptProtocol,
                                                      ZF_IN const zfchar *encryptProtocolVersion,
                                                      ZF_IN const ZFEncryptUpgradeCallback &upgradeCallback);

/**
 * @brief encrypt util
 */
zfclass ZF_ENV_EXPORT ZFEncrypt : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(ZFEncrypt, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

    /**
     * @brief encrypt key, empty by default
     */
    ZFPROPERTY_ASSIGN(zfstring, encryptKey)

public:
    /**
     * @brief init with encrypt key
     */
    virtual ZFObject *objectOnInit(ZF_IN const zfchar *encryptKey)
    {
        this->objectOnInit();
        this->encryptKeySet(encryptKey);
        return this;
    }
public:
    zfoverride
    virtual ZFObject *objectOnInit(void) {return zfsuper::objectOnInit();}

public:
    /** @brief calc size for encrypt */
    virtual zfindex encryptCalcSize(ZF_IN zfindex srcLen);
    /** @brief calc size for decrypt */
    virtual zfindex decryptCalcSize(ZF_IN zfindex srcLen);

public:
    /**
     * @brief encrypt, result value is ensured to have printable chars only
     *
     * which encrypt algorithm would be used, is depends on ZFEncrypt protocol
     */
    virtual zfbool encrypt(ZF_IN_OUT const ZFOutputCallback &output, ZF_IN const ZFInputCallback &input);
    /**
     * @brief decrypt
     */
    virtual zfbool decrypt(ZF_IN_OUT const ZFOutputCallback &output, ZF_IN const ZFInputCallback &input);

public:
    /** @brief see #encrypt */
    virtual zfbool encrypt(ZF_OUT zfchar *buf,
                           ZF_IN const void *src,
                           ZF_IN_OPT zfindex srcLen = zfindexMax)
    {
        return this->encrypt(ZFOutputCallbackForBuffer(buf), ZFInputCallbackForBuffer(src, srcLen));
    }
    /** @brief see #decrypt */
    virtual zfbool decrypt(ZF_OUT zfchar *buf,
                           ZF_IN const void *src,
                           ZF_IN_OPT zfindex srcLen = zfindexMax)
    {
        return this->decrypt(ZFOutputCallbackForBuffer(buf), ZFInputCallbackForBuffer(src, srcLen));
    }
public:
    /** @brief see #encrypt */
    virtual zfbool encrypt(ZF_IN_OUT const ZFOutputCallback &output,
                           ZF_IN const void *src,
                           ZF_IN_OPT zfindex srcLen = zfindexMax)
    {
        return this->encrypt(output, ZFInputCallbackForBuffer(src, srcLen));
    }
    /** @brief see #decrypt */
    virtual zfbool decrypt(ZF_IN_OUT const ZFOutputCallback &output,
                           ZF_IN const void *src,
                           ZF_IN_OPT zfindex srcLen = zfindexMax)
    {
        return this->decrypt(output, ZFInputCallbackForBuffer(src, srcLen));
    }

public:
    /**
     * @brief get encrypt implementation's name
     */
    virtual void encryptProtocol(ZF_OUT zfstring &ret);
    /** @brief see #encryptProtocol */
    zffinal inline zfstring encryptProtocol(void)
    {
        zfstring ret;
        this->encryptProtocol(ret);
        return ret;
    }
    /**
     * @brief get encrypt implementation's version
     */
    virtual void encryptProtocolVersion(ZF_OUT zfstring &ret);
    /** @brief see #encryptProtocolVersion */
    zffinal inline zfstring encryptProtocolVersion(void)
    {
        zfstring ret;
        this->encryptProtocolVersion(ret);
        return ret;
    }

public:
    /**
     * @brief try to upgrade encrypted data
     *
     * you may register your own upgrader by #ZFEncryptUpgraderRegister
     */
    virtual zfbool encryptUpgrade(ZF_IN_OUT const ZFOutputCallback &output,
                                  ZF_IN const ZFInputCallback &input,
                                  ZF_IN const zfchar *encryptProtocolOld,
                                  ZF_IN const zfchar *encryptProtocolVersionOld,
                                  ZF_IN_OPT const zfchar *encryptKey = zfnull);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEncrypt_h_

