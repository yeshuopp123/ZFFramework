/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFEncrypt.h"
#include "protocol/ZFProtocolZFEncrypt.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassLikePOD _ZFP_ZFEncryptUpgraderData
{
public:
    zfstring encryptProtocolOld;
    zfstring encryptProtocolVersionOld;
    zfstring encryptProtocol;
    zfstring encryptProtocolVersion;
    ZFEncryptUpgradeCallback upgradeCallback;
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFEncryptUpgradeDataHolder, ZFLevelZFFrameworkNormal)
{
}
public:
    ZFCoreArray<_ZFP_ZFEncryptUpgraderData> datas;
ZF_GLOBAL_INITIALIZER_END(ZFEncryptUpgradeDataHolder)

void ZFEncryptUpgraderRegister(ZF_IN const zfchar *encryptProtocolOld,
                               ZF_IN const zfchar *encryptProtocolVersionOld,
                               ZF_IN const zfchar *encryptProtocol,
                               ZF_IN const zfchar *encryptProtocolVersion,
                               ZF_IN const ZFEncryptUpgradeCallback &upgradeCallback)
{
    if(!upgradeCallback.callbackIsValid())
    {
        return ;
    }
    ZFCoreArray<_ZFP_ZFEncryptUpgraderData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFEncryptUpgradeDataHolder)->datas;
    _ZFP_ZFEncryptUpgraderData data;
    data.encryptProtocolOld = encryptProtocolOld;
    data.encryptProtocolVersionOld = encryptProtocolVersionOld;
    data.encryptProtocol = encryptProtocol;
    data.encryptProtocolVersion = encryptProtocolVersion;
    data.upgradeCallback = upgradeCallback;
    datas.add(data);
}
void ZFEncryptUpgraderUnregister(ZF_IN const zfchar *encryptProtocolOld,
                                 ZF_IN const zfchar *encryptProtocolVersionOld,
                                 ZF_IN const zfchar *encryptProtocol,
                                 ZF_IN const zfchar *encryptProtocolVersion,
                                 ZF_IN const ZFEncryptUpgradeCallback &upgradeCallback)
{
    if(!upgradeCallback.callbackIsValid())
    {
        return ;
    }
    ZFCoreArray<_ZFP_ZFEncryptUpgraderData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFEncryptUpgradeDataHolder)->datas;
    for(zfindex i = 0; i < datas.count(); ++i)
    {
        const _ZFP_ZFEncryptUpgraderData &data = datas[i];
        if(zftrue
            && data.encryptProtocolOld.compare(encryptProtocolOld) == 0
            && data.encryptProtocolVersionOld.compare(encryptProtocolVersionOld) == 0
            && data.encryptProtocol.compare(encryptProtocol) == 0
            && data.encryptProtocolVersion.compare(encryptProtocolVersion) == 0
            && data.upgradeCallback == upgradeCallback
            )
        {
            datas.remove(i);
            break;
        }
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFEncrypt)

zfindex ZFEncrypt::encryptCalcSize(ZF_IN zfindex srcLen)
{
    if(srcLen == zfindexMax)
    {
        return 0;
    }
    return ZFPROTOCOL_ACCESS(ZFEncrypt)->encryptCalcSize(srcLen, this->encryptKey());
}
zfindex ZFEncrypt::decryptCalcSize(ZF_IN zfindex srcLen)
{
    if(srcLen == zfindexMax)
    {
        return 0;
    }
    return ZFPROTOCOL_ACCESS(ZFEncrypt)->decryptCalcSize(srcLen, this->encryptKey());
}

zfbool ZFEncrypt::encrypt(ZF_IN_OUT const ZFOutputCallback &output, ZF_IN const ZFInputCallback &input)
{
    if(!input.callbackIsValid() || !output.callbackIsValid())
    {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFEncrypt)->encrypt(output, input, this->encryptKey());
}
zfbool ZFEncrypt::decrypt(ZF_IN_OUT const ZFOutputCallback &output, ZF_IN const ZFInputCallback &input)
{
    if(!input.callbackIsValid() || !output.callbackIsValid())
    {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFEncrypt)->decrypt(output, input, this->encryptKey());
}

void ZFEncrypt::encryptProtocol(ZF_OUT zfstring &ret)
{
    ZFPROTOCOL_ACCESS(ZFEncrypt)->encryptProtocol(ret);
}
void ZFEncrypt::encryptProtocolVersion(ZF_OUT zfstring &ret)
{
    ZFPROTOCOL_ACCESS(ZFEncrypt)->encryptProtocolVersion(ret);
}

zfbool ZFEncrypt::encryptUpgrade(ZF_IN_OUT const ZFOutputCallback &output,
                                 ZF_IN const ZFInputCallback &input,
                                 ZF_IN const zfchar *encryptProtocolOld,
                                 ZF_IN const zfchar *encryptProtocolVersionOld,
                                 ZF_IN_OPT const zfchar *encryptKey /* = zfnull */)
{
    if(encryptKey == zfnull)
    {
        encryptKey = this->encryptKey();
    }
    if(ZFPROTOCOL_ACCESS(ZFEncrypt)->encryptUpgrade(
            output, input,
            encryptKey,
            encryptProtocolOld, encryptProtocolVersionOld
        ))
    {
        return zftrue;
    }

    const ZFCoreArray<_ZFP_ZFEncryptUpgraderData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFEncryptUpgradeDataHolder)->datas;
    if(!datas.isEmpty())
    {
        ZFEncryptUpgradeTaskData upgradeTaskData;
        upgradeTaskData.encryptProtocolOld = encryptProtocolOld;
        upgradeTaskData.encryptProtocolVersionOld = encryptProtocolVersionOld;
        upgradeTaskData.encryptProtocol = this->encryptProtocol();
        upgradeTaskData.encryptProtocolVersion = this->encryptProtocolVersion();
        upgradeTaskData.encryptKey = encryptKey;
        upgradeTaskData.input = input;
        upgradeTaskData.output = output;
        for(zfindex i = 0; i < datas.count(); ++i)
        {
            const _ZFP_ZFEncryptUpgraderData &data = datas[i];
            if(zftrue
                    && (data.encryptProtocolOld.isEmpty() || data.encryptProtocolOld.compare(encryptProtocolOld) == 0)
                    && (data.encryptProtocolVersionOld.isEmpty() || data.encryptProtocolVersionOld.compare(encryptProtocolVersionOld) == 0)
                    && (data.encryptProtocol.isEmpty() || data.encryptProtocol.compare(upgradeTaskData.encryptProtocol) == 0)
                    && (data.encryptProtocolVersion.isEmpty() || data.encryptProtocolVersion.compare(upgradeTaskData.encryptProtocolVersion) == 0)
              )
            {
                if(data.upgradeCallback.execute(upgradeTaskData))
                {
                    return zftrue;
                }
            }
        }
    }
    return zffalse;
}

ZF_NAMESPACE_GLOBAL_END

