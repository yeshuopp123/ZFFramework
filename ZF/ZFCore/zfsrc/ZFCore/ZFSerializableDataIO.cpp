/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFSerializableDataIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFSerializableDataIOData
{
public:
    ZFSerializableDataFromIOCallback fromCallback;
    ZFSerializableDataToIOCallback toCallback;
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFSerializableDataIODataHolder, ZFLevelZFFrameworkNormal)
{
}
public:
    ZFCoreMap dataMap; // _ZFP_ZFSerializableDataIOData
ZF_GLOBAL_INITIALIZER_END(ZFSerializableDataIODataHolder)

// ============================================================
zfbool ZFSerializableDataFromIO(ZF_OUT ZFSerializableData &serializableData,
                                ZF_IN const zfchar *ioType,
                                ZF_IN const ZFInputCallback &input,
                                ZF_OUT_OPT zfstring *errorMessage /* = zfnull */)
{
    if(!input.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(errorMessage, zfText("invalid callback"));
        return zffalse;
    }
    ZFSerializableDataFromIOCallback fromCallback = ZFSerializableDataFromIOCallbackGet(ioType);
    if(fromCallback == zfnull)
    {
        ZFSerializableUtil::errorOccurred(errorMessage, zfText("no such ioType: \"%s\""), ioType);
        return zffalse;
    }
    return fromCallback(serializableData, input, errorMessage);
}
zfbool ZFSerializableDataToIO(ZF_IN_OUT const ZFOutputCallback &output,
                              ZF_IN const zfchar *ioType,
                              ZF_IN const ZFSerializableData &serializableData,
                              ZF_OUT_OPT zfstring *errorMessage /* = zfnull */)
{
    if(!output.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(errorMessage, zfText("invalid callback"));
        return zffalse;
    }
    ZFSerializableDataToIOCallback toCallback = ZFSerializableDataToIOCallbackGet(ioType);
    if(toCallback == zfnull)
    {
        ZFSerializableUtil::errorOccurred(errorMessage, zfText("no such ioType: \"%s\""), ioType);
        return zffalse;
    }
    return toCallback(output, serializableData, errorMessage);
}

void ZFSerializableDataIORegister(ZF_IN const zfchar *ioType,
                                  ZF_IN ZFSerializableDataFromIOCallback fromCallback,
                                  ZF_IN ZFSerializableDataToIOCallback toCallback)
{
    zfCoreAssert(!zfscmpTheSame(ioType, zfText("")));
    zfCoreAssert(fromCallback != zfnull);
    zfCoreAssert(toCallback != zfnull);

    ZF_GLOBAL_INITIALIZER_CLASS(ZFSerializableDataIODataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSerializableDataIODataHolder);
    _ZFP_ZFSerializableDataIOData *data = zfnew(_ZFP_ZFSerializableDataIOData);
    data->fromCallback = fromCallback;
    data->toCallback = toCallback;
    d->dataMap.set(ioType, ZFCorePointerForObject<_ZFP_ZFSerializableDataIOData *>(data));
}
void ZFSerializableDataIOUnregister(ZF_IN const zfchar *ioType)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFSerializableDataIODataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSerializableDataIODataHolder);
    d->dataMap.remove(ioType);
}

ZFSerializableDataFromIOCallback ZFSerializableDataFromIOCallbackGet(ZF_IN const zfchar *ioType)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFSerializableDataIODataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSerializableDataIODataHolder);
    _ZFP_ZFSerializableDataIOData *data = d->dataMap.get<_ZFP_ZFSerializableDataIOData *>(ioType);
    if(data != zfnull)
    {
        return data->fromCallback;
    }
    else
    {
        return zfnull;
    }
}
ZFSerializableDataToIOCallback ZFSerializableDataToIOCallbackGet(ZF_IN const zfchar *ioType)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFSerializableDataIODataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSerializableDataIODataHolder);
    _ZFP_ZFSerializableDataIOData *data = d->dataMap.get<_ZFP_ZFSerializableDataIOData *>(ioType);
    if(data != zfnull)
    {
        return data->toCallback;
    }
    else
    {
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

