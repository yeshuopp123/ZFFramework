/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFProtocol.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFProtocolLevel)

// ============================================================
ZFOUTPUT_TYPE_DEFINE(const ZFProtocol *, {
    if(v)
    {
        output << v->objectInfo();
    }
    else
    {
        output << (const void *)zfnull;
    }
})

// ============================================================
zfclassLikePOD _ZFP_ZFProtocolDataPrivateData
{
public:
    _ZFP_ZFProtocolData data;
    ZFCoreArrayPOD<zfbool *> ZFCoreLibDestroyFlag;
};
ZF_STATIC_INITIALIZER_INIT(ZFProtocolDataHolder)
{
}
ZFCoreMap implDataMap; // _ZFP_ZFProtocolDataPrivateData *
ZF_STATIC_INITIALIZER_END(ZFProtocolDataHolder)
#define _ZFP_ZFProtocolDataMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFProtocolDataHolder)->implDataMap)

// ============================================================
// cleanup to ensure ZF's cleanup step queued before global static cleanup,
// which may break other framework's cleanup order,
// not reliable, but may solve most cases
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFProtocolImplCleanup_protocolOnDeallocPrepare, ZFLevelZFFrameworkEssential)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFProtocolImplCleanup_protocolOnDeallocPrepare)
{
    ZFCoreMap &implDataMap = _ZFP_ZFProtocolDataMap;
    ZFCoreArrayPOD<_ZFP_ZFProtocolDataPrivateData *> allValue;
    implDataMap.allValue(allValue);
    for(zfindex i = 0; i < allValue.count(); ++i)
    {
        _ZFP_ZFProtocolDataPrivateData *dataHolder = allValue[i];
        if(dataHolder->data.implInstance != zfnull)
        {
            dataHolder->data.implInstance->_ZFP_ZFProtocol_protocolInstanceState = ZFProtocol::ProtocolInstanceStateOnDeallocPrepare;
            dataHolder->data.implInstance->protocolOnDeallocPrepare();
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFProtocolImplCleanup_protocolOnDeallocPrepare)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFProtocolImplCleanup_protocolOnDealloc, ZFLevelZFFrameworkStatic)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFProtocolImplCleanup_protocolOnDealloc)
{
    ZFCoreMap &implDataMap = _ZFP_ZFProtocolDataMap;
    ZFCoreArrayPOD<_ZFP_ZFProtocolDataPrivateData *> allValue;
    implDataMap.allValue(allValue);
    for(zfindex i = 0; i < allValue.count(); ++i)
    {
        _ZFP_ZFProtocolDataPrivateData *dataHolder = allValue[i];
        if(dataHolder->data.implInstance != zfnull)
        {
            dataHolder->data.implInstance->_ZFP_ZFProtocol_protocolInstanceState = ZFProtocol::ProtocolInstanceStateOnDealloc;
            dataHolder->data.implInstance->protocolOnDealloc();
            dataHolder->data.implInstance->_ZFP_ZFProtocol_protocolInstanceState = ZFProtocol::ProtocolInstanceStateOnInit;
            dataHolder->data.implCleanupCallback(&(dataHolder->data));
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFProtocolImplCleanup_protocolOnDealloc)

// ============================================================
_ZFP_ZFProtocolData &_ZFP_ZFProtocolImplDataRegister(zfbool *ZFCoreLibDestroyFlag,
                                                     const zfchar *protocolName,
                                                     _ZFP_ZFProtocolTryAccessCallback implTryAccessCallback,
                                                     zfbool protocolOptional)
{
    _ZFP_ZFProtocolDataPrivateData *dataHolder = _ZFP_ZFProtocolDataMap.get<_ZFP_ZFProtocolDataPrivateData *>(protocolName);
    if(dataHolder == zfnull)
    {
        dataHolder = zfnew(_ZFP_ZFProtocolDataPrivateData);
        dataHolder->data.protocolName = protocolName;
        dataHolder->data.protocolOptional = protocolOptional;
        dataHolder->data.implTryAccessCallback = implTryAccessCallback;
        dataHolder->data.implConstructor = zfnull;
        dataHolder->data.implLevel = ZFProtocolLevel::e_Default;
        dataHolder->data.implCleanupCallback = zfnull;
        dataHolder->data.implInstance = zfnull;
        _ZFP_ZFProtocolDataMap.set(protocolName, ZFCorePointerForObject<_ZFP_ZFProtocolDataPrivateData *>(dataHolder));
    }

    dataHolder->ZFCoreLibDestroyFlag.add(ZFCoreLibDestroyFlag);

    return dataHolder->data;
}
void _ZFP_ZFProtocolImplDataUnregister(zfbool *ZFCoreLibDestroyFlag, const zfchar *protocolName)
{
    _ZFP_ZFProtocolDataPrivateData *dataHolder = _ZFP_ZFProtocolDataMap.get<_ZFP_ZFProtocolDataPrivateData *>(protocolName);
    if(dataHolder != zfnull)
    {
        dataHolder->ZFCoreLibDestroyFlag.removeElement(ZFCoreLibDestroyFlag);
    }
}
void _ZFP_ZFProtocolImplAccess(void)
{
    // access to ensure init order
    (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFProtocolImplCleanup_protocolOnDealloc);
    (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFProtocolImplCleanup_protocolOnDeallocPrepare);
}

// ============================================================
ZFProtocol *ZFProtocolForName(ZF_IN const zfchar *name,
                              ZF_IN_OPT const zfchar *desiredImpl /* = zfnull */)
{
    ZFCoreArrayPOD<_ZFP_ZFProtocolDataPrivateData *> allValue;
    _ZFP_ZFProtocolDataMap.allValue(allValue);
    for(zfindex i = 0; i < allValue.count(); ++i)
    {
        _ZFP_ZFProtocolData &implData = allValue[i]->data;
        if(zfscmpTheSame(implData.protocolName.cString(), name))
        {
            implData.implTryAccessCallback();
            if(implData.implInstance != zfnull
                && (desiredImpl == zfnull || zfscmpTheSame(desiredImpl, implData.implInstance->protocolImplementationPlatformHint())))
            {
                return implData.implInstance;
            }
            break;
        }
    }
    return zfnull;
}
zfbool ZFProtocolIsAvailable(ZF_IN const zfchar *name,
                             ZF_IN_OPT const zfchar *desiredImpl /* = zfnull */)
{
    return (ZFProtocolForName(name, desiredImpl) != zfnull);
}

// ============================================================
static void _ZFP_ZFProtocolImplDataGet(ZF_IN_OUT ZFProtocolImplInfoData &data,
                                       ZF_IN _ZFP_ZFProtocolData &implData)
{
    data.protocolName = implData.protocolName;
    data.protocolOptional = implData.protocolOptional;
    data.protocolImpl = implData.implTryAccessCallback();
}
ZFCoreArray<ZFProtocolImplInfoData> ZFProtocolImplInfoDataGet(void)
{
    ZFCoreArray<ZFProtocolImplInfoData> ret;

    ZFCoreArrayPOD<_ZFP_ZFProtocolDataPrivateData *> allValue;
    _ZFP_ZFProtocolDataMap.allValue(allValue);
    for(zfindex i = 0; i < allValue.count(); ++i)
    {
        ZFProtocolImplInfoData data;
        _ZFP_ZFProtocolImplDataGet(data, allValue[i]->data);
        ret.add(data);
    }

    return ret;
}
ZFCoreArray<ZFProtocolImplInfoData> ZFProtocolImplInfoDataGetImplemented(void)
{
    ZFCoreArray<ZFProtocolImplInfoData> ret;

    ZFCoreArrayPOD<_ZFP_ZFProtocolDataPrivateData *> allValue;
    _ZFP_ZFProtocolDataMap.allValue(allValue);
    for(zfindex i = 0; i < allValue.count(); ++i)
    {
        ZFProtocolImplInfoData data;
        _ZFP_ZFProtocolData &implData = allValue[i]->data;
        if(implData.implTryAccessCallback() != zfnull)
        {
            _ZFP_ZFProtocolImplDataGet(data, implData);
            ret.add(data);
        }
    }

    return ret;
}
ZFCoreArray<ZFProtocolImplInfoData> ZFProtocolImplInfoDataGetNotImplemented(void)
{
    ZFCoreArray<ZFProtocolImplInfoData> ret;

    ZFCoreArrayPOD<_ZFP_ZFProtocolDataPrivateData *> allValue;
    _ZFP_ZFProtocolDataMap.allValue(allValue);
    for(zfindex i = 0; i < allValue.count(); ++i)
    {
        ZFProtocolImplInfoData data;
        _ZFP_ZFProtocolData &implData = allValue[i]->data;
        if(implData.implTryAccessCallback() == zfnull)
        {
            _ZFP_ZFProtocolImplDataGet(data, implData);
            ret.add(data);
        }
    }

    return ret;
}
static ZFCompareResult _ZFP_ZFProtocolImplInfoDataPrint_sortComparer(ZF_IN const ZFProtocolImplInfoData &e0,
                                                                     ZF_IN const ZFProtocolImplInfoData &e1)
{
    zfint t = zfscmp(e0.protocolName.cString(), e1.protocolName.cString());
    if(t < 0)
    {
        return ZFCompareSmaller;
    }
    else if(t > 0)
    {
        return ZFCompareGreater;
    }
    else
    {
        return ZFCompareTheSame;
    }
}
void ZFProtocolImplInfoDataPrint(ZF_IN const ZFProtocolImplInfoData &data,
                                 ZF_IN_OPT const ZFOutputCallback &callback /* = ZFOutputCallbackDefault */)
{
    callback.execute(data.protocolName.cString());
    if(data.protocolOptional)
    {
        callback.execute(zfText(" (optional)"));
    }
    if(data.protocolImpl != zfnull)
    {
        callback.execute(zfText(" "));
        callback.execute(data.protocolImpl->protocolImplementationName());
        callback.execute(zfText("("));
        callback.execute(ZFProtocolLevelEnumToString(data.protocolImpl->protocolImplementationLevel()).cString());
        callback.execute(zfText(")"));
        if(!zfscmpTheSame(data.protocolImpl->protocolImplementationPlatformHint(), zfText("")))
        {
            callback.execute(zfText(" "));
            callback.execute(data.protocolImpl->protocolImplementationPlatformHint());
        }
    }
}
void ZFProtocolImplInfoDataPrint(ZF_IN_OPT const ZFOutputCallback &callback /* = ZFOutputCallbackDefault */)
{
    if(!callback.callbackIsValid())
    {
        return ;
    }

    ZFCoreArray<ZFProtocolImplInfoData> notImplemented = ZFProtocolImplInfoDataGetNotImplemented();
    ZFCoreArray<ZFProtocolImplInfoData> implemented = ZFProtocolImplInfoDataGetImplemented();
    if(!notImplemented.isEmpty())
    {
        notImplemented.sort(_ZFP_ZFProtocolImplInfoDataPrint_sortComparer);

        callback.execute(zfText("not implemented:\n"));
        for(zfindex i = 0; i < notImplemented.count(); ++i)
        {
            const ZFProtocolImplInfoData &data = notImplemented[i];
            callback.execute(zfText("  "));
            ZFProtocolImplInfoDataPrint(data, callback);
            callback.execute(zfText("\n"));
        }
    }
    if(!implemented.isEmpty())
    {
        implemented.sort(_ZFP_ZFProtocolImplInfoDataPrint_sortComparer);

        callback.execute(zfText("implemented:\n"));
        for(zfindex i = 0; i < implemented.count(); ++i)
        {
            const ZFProtocolImplInfoData &data = implemented[i];
            callback.execute(zfText("  "));
            ZFProtocolImplInfoDataPrint(data, callback);
            callback.execute(zfText("\n"));
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

