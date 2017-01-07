/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFAlgorithm.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFEncrypt.h"

#include "ZFAlgorithm.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEncryptImpl_default, ZFEncrypt, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("ZFEncrypt"))
public:
    /*
     * encrypt logic:
     *   "hex content size" + "+" + "contents in hashed base64"
     */
    virtual void encryptProtocol(ZF_OUT zfstring &ret)
    {
        ret += zfText("ZFEncrypt");
    }
    virtual void encryptProtocolVersion(ZF_OUT zfstring &ret)
    {
        ret += zfText("0");
    }

public:
    virtual zfindex encryptCalcSize(ZF_IN zfindex srcLen, ZF_IN const zfstring &key)
    {
        return (0
            + zfsFromInt(srcLen, 16).length() // header size
            + 1 // header size '+' token
            + ZFBase64EncodeCalcSize(srcLen) // content
            + 1 // ensure tail '\0'
            );
    }
    virtual zfindex decryptCalcSize(ZF_IN zfindex srcLen, ZF_IN const zfstring &key)
    {
        zfindex minSize = (0
            + 1 // header min size
            + 1 // header size '+' token
            );
        return ((srcLen > minSize) ? (srcLen - minSize) : 0);
    }

public:
    virtual zfbool encrypt(ZF_IN_OUT const ZFOutputCallback &output, ZF_IN const ZFInputCallback &input, ZF_IN const zfstring &key)
    {
        zfindex inputSize = input.ioSize();
        if(inputSize != zfindexMax)
        {
            ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
            zfstring sizeCheck = zfstringWithFormat(zfText("%s+"), zfsFromInt(inputSize, 16).cString());
            output.execute(sizeCheck.cString(), sizeCheck.length());
            return ZFBase64Encode(output, input, zfnull, this->tableForKey(key));
        }
        else
        {
            ZFBuffer inputBuf = ZFInputCallbackReadToBuffer(input);
            if(inputBuf.buffer() == zfnull)
            {
                return zffalse;
            }
            ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
            zfstring sizeCheck = zfstringWithFormat(zfText("%s+"), zfsFromInt(inputBuf.bufferSize(), 16).cString());
            output.execute(sizeCheck.cString(), sizeCheck.length());
            return ZFBase64Encode(output, ZFInputCallbackForBuffer(inputBuf.buffer(), inputBuf.bufferSize()), zfnull, this->tableForKey(key));
        }
    }
    virtual zfbool decrypt(ZF_IN_OUT const ZFOutputCallback &output, ZF_IN const ZFInputCallback &input, ZF_IN const zfstring &key)
    {
        ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
        zfchar sizeCheckBuf[16] = {0};
        for(zfindex i = 0; i < 10; ++i)
        {
            if(input.execute(sizeCheckBuf + i, 1) != 1)
            {
                return zffalse;
            }
            if(sizeCheckBuf[i] == '+')
            {
                sizeCheckBuf[i] = '\0';
                break;
            }
        }
        zfindex sizeCheck = 0;
        if(zfsToInt(sizeCheck, sizeCheckBuf, zfindexMax, 16) != zfnull)
        {
            return zffalse;
        }
        zfindex size = 0;
        if(!ZFBase64Decode(output, input, &size, this->tableForKey(key)))
        {
            return zffalse;
        }
        return (size == sizeCheck);
    }
private:
    zfstring tableForKey(ZF_IN const zfstring &key)
    {
        ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
        zfstring table = ZFBase64TableDefault;
        zfstring keyTmp = zfstringWithFormat(zfText("%s%zi"), key.cString(), key.length());
        keyTmp = zfMd5Calc(keyTmp.cString(), keyTmp.length());
        const zfbyte *p = (const zfbyte *)keyTmp.cString();
        const zfbyte *pEnd = p + keyTmp.length();
        for( ; p < pEnd; p += 2)
        {
            zfindex pos0 = (zfindex)(p[0] % 64);
            zfindex pos1 = (zfindex)(p[1] % 64);
            zfchar t = table[pos0];
            table[pos0] = table[pos1];
            table[pos1] = t;
        }
        return table;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEncryptImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFEncryptImpl_default)

ZF_NAMESPACE_GLOBAL_END

