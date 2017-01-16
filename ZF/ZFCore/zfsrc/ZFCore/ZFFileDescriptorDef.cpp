/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFFileDescriptorDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassPOD _ZFP_ZFFileDescriptorCallbackData
{
public:
    ZFFileDescriptorInputCallbackGetter inputCallbackGetter;
    ZFFileDescriptorOutputCallbackGetter outputCallbackGetter;
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFFileDescriptorTypeHolder, ZFLevelZFFrameworkNormal)
{
}
public:
    ZFCoreMap fileDescriptorTypeMap; // _ZFP_ZFFileDescriptorCallbackData *
ZF_GLOBAL_INITIALIZER_END(ZFFileDescriptorTypeHolder)
#define _ZFP_ZFFileDescriptorTypeMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFFileDescriptorTypeHolder)->fileDescriptorTypeMap)

static _ZFP_ZFFileDescriptorCallbackData *_ZFP_ZFFileDescriptorTypeFind(ZF_IN const zfchar *fileDescriptorType)
{
    return _ZFP_ZFFileDescriptorTypeMap.get<_ZFP_ZFFileDescriptorCallbackData *>(fileDescriptorType);
}

ZFInputCallback _ZFP_ZFInputCallbackForFileDescriptor(ZF_IN const zfcharA *callerFile,
                                                      ZF_IN const zfcharA *callerFunction,
                                                      ZF_IN zfindex callerLine,
                                                      ZF_IN const zfchar *fileDescriptor,
                                                      ZF_IN_OPT zfindex fileDescriptorLen /* = zfindexMax */,
                                                      ZF_IN_OPT ZFFileOpenOptionFlags flags /* = ZFFileOpenOption::e_Read */,
                                                      ZF_IN_OPT const ZFFileBOM *autoSkipBOMTable /* = &ZFFileBOMUTF8 */,
                                                      ZF_IN_OPT zfindex autoSkipBOMTableCount /* = 1 */)
{
    ZFInputCallback ret = ZFCallbackNullDetail(callerFile, callerFunction, callerLine);
    if(fileDescriptor == zfnull)
    {
        return ret;
    }
    const zfchar *typeStart = zfnull;
    zfindex typeLength = 0;
    const zfchar *dataStart = zfnull;
    zfindex dataLength = 0;
    {
        const zfchar *p = fileDescriptor;
        const zfchar *pEnd = ((fileDescriptorLen == zfindexMax) ? p + zfslen(fileDescriptor) : p + fileDescriptorLen);
        for( ; p != pEnd; ++p)
        {
            if(*p == ':')
            {
                typeStart = fileDescriptor;
                typeLength = p - typeStart;
                dataStart = p + 1;
                dataLength = pEnd - dataStart;
                break;
            }
        }
        if(typeStart == zfnull)
        {
            return ret;
        }
    }
    const _ZFP_ZFFileDescriptorCallbackData *callbackData = _ZFP_ZFFileDescriptorTypeFind(zfstring(typeStart, typeLength).cString());
    if(callbackData == zfnull)
    {
        return ret;
    }
    ret = callbackData->inputCallbackGetter(
        dataStart, dataLength,
        flags,
        autoSkipBOMTable, autoSkipBOMTableCount);
    ret.createInfoSet(callerFile, callerFunction, callerLine);

    {
        zfstring callbackId;
        callbackId += zfText("ZFInputCallbackForFileDescriptor");
        callbackId += zfText("[");
        for(zfindex i = 0; i < autoSkipBOMTableCount; ++i)
        {
            if(i != 0)
            {
                callbackId += zfText(", ");
            }
            ZFFileBOMToString(callbackId, autoSkipBOMTable[i]);
        }
        callbackId += zfText("]:");
        callbackId.append(fileDescriptor, fileDescriptorLen);
        ret.callbackIdSet(callbackId);
    }

    {
        zfbool success = zffalse;
        ZFSerializableData customData;
        customData.itemClassSet(ZFSerializableKeyword_node);
        do
        {
            ZFSerializableData fileDescriptorData;
            if(!zfstringToSerializableData(fileDescriptorData, fileDescriptor))
            {
                break;
            }
            fileDescriptorData.categorySet(ZFSerializableKeyword_ZFInputCallbackForFileDescriptor_fileDescriptor);
            customData.elementAdd(fileDescriptorData);

            if(flags != ZFFileOpenOption::e_Read)
            {
                ZFSerializableData flagsData;
                if(!ZFFileOpenOptionFlagsToSerializableData(flagsData, flags))
                {
                    break;
                }
                flagsData.categorySet(ZFSerializableKeyword_ZFInputCallbackForFileDescriptor_flags);
                customData.elementAdd(flagsData);
            }

            if(autoSkipBOMTableCount != 1 || autoSkipBOMTable[0] != ZFFileBOMUTF8)
            {
                ZFSerializableData autoSkipBOMTableData;
                if(!zfstringToSerializableData(autoSkipBOMTableData, ZFFileBOMListToString(autoSkipBOMTable, autoSkipBOMTableCount)))
                {
                    break;
                }
                autoSkipBOMTableData.categorySet(ZFSerializableKeyword_ZFInputCallbackForFileDescriptor_autoSkipBOMTable);
                customData.elementAdd(autoSkipBOMTableData);
            }

            success = zftrue;
        } while(zffalse);
        if(success)
        {
            ret.callbackSerializeCustomTypeSet(ZFCallbackSerializeCustomTypeName_ZFInputCallbackForFileDescriptor);
            ret.callbackSerializeCustomDataSet(customData);
        }
    }

    return ret;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFCallbackSerializeCustomType_ZFInputCallbackForFileDescriptor)
{
    const ZFSerializableData *fileDescriptorData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFInputCallbackForFileDescriptor_fileDescriptor, outErrorHint, outErrorPos);
    if(fileDescriptorData == zfnull)
    {
        return zffalse;
    }
    const zfchar *fileDescriptor = zfnull;
    if(!zfstringFromSerializableData(fileDescriptor, *fileDescriptorData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read;
    {
        const ZFSerializableData *flagsData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFInputCallbackForFileDescriptor_flags);
        if(flagsData != zfnull && !ZFFileOpenOptionFlagsFromSerializableData(flags, *flagsData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
    }

    ZFCoreArrayPOD<ZFFileBOM> BOMList;
    BOMList.add(ZFFileBOMUTF8);
    {
        const ZFSerializableData *autoSkipBOMTableData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFInputCallbackForFileDescriptor_autoSkipBOMTable);
        zfstring BOMStringList;
        if(autoSkipBOMTableData != zfnull)
        {
            if(!zfstringFromSerializableData(BOMStringList, *autoSkipBOMTableData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            BOMList.removeAll();

            if(zfnull != ZFFileBOMListFromString(BOMList, BOMStringList))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, *autoSkipBOMTableData, zfText("format BOM list error"));
                return zffalse;
            }
        }
    }

    serializableData.resolveMark();

    result = ZFInputCallbackForFileDescriptor(fileDescriptor, zfindexMax, flags, BOMList.arrayBuf(), BOMList.count());
    return zftrue;
}

ZFOutputCallback _ZFP_ZFOutputCallbackForFileDescriptor(ZF_IN const zfcharA *callerFile,
                                                        ZF_IN const zfcharA *callerFunction,
                                                        ZF_IN zfindex callerLine,
                                                        ZF_IN const zfchar *fileDescriptor,
                                                        ZF_IN_OPT zfindex fileDescriptorLen /* = zfindexMax */,
                                                        ZF_IN_OPT ZFFileOpenOptionFlags flags /* = ZFFileOpenOption::e_Create */)
{
    ZFOutputCallback ret = ZFCallbackNullDetail(callerFile, callerFunction, callerLine);
    if(fileDescriptor == zfnull)
    {
        return ret;
    }
    const zfchar *typeStart = zfnull;
    zfindex typeLength = 0;
    const zfchar *dataStart = zfnull;
    zfindex dataLength = 0;
    {
        const zfchar *p = fileDescriptor;
        const zfchar *pEnd = ((fileDescriptorLen == zfindexMax) ? p + zfslen(fileDescriptor) : p + fileDescriptorLen);
        for( ; p != pEnd; ++p)
        {
            if(*p == ':')
            {
                typeStart = fileDescriptor;
                typeLength = p - typeStart;
                dataStart = p + 1;
                dataLength = pEnd - dataStart;
                break;
            }
        }
        if(typeStart == zfnull)
        {
            return ret;
        }
    }
    const _ZFP_ZFFileDescriptorCallbackData *callbackData = _ZFP_ZFFileDescriptorTypeFind(zfstring(typeStart, typeLength).cString());
    if(callbackData == zfnull)
    {
        return ret;
    }
    ret = callbackData->outputCallbackGetter(
        dataStart, dataLength,
        flags);
    ret.createInfoSet(callerFile, callerFunction, callerLine);

    {
        zfbool success = zffalse;
        ZFSerializableData customData;
        customData.itemClassSet(ZFSerializableKeyword_node);
        do
        {
            ZFSerializableData fileDescriptorData;
            if(!zfstringToSerializableData(fileDescriptorData, fileDescriptor))
            {
                break;
            }
            fileDescriptorData.categorySet(ZFSerializableKeyword_ZFOutputCallbackForFileDescriptor_fileDescriptor);
            customData.elementAdd(fileDescriptorData);

            if(flags != ZFFileOpenOption::e_Create)
            {
                ZFSerializableData flagsData;
                if(!ZFFileOpenOptionFlagsToSerializableData(flagsData, flags))
                {
                    break;
                }
                flagsData.categorySet(ZFSerializableKeyword_ZFOutputCallbackForFileDescriptor_flags);
                customData.elementAdd(flagsData);
            }

            success = zftrue;
        } while(zffalse);
        if(success)
        {
            ret.callbackSerializeCustomTypeSet(ZFCallbackSerializeCustomTypeName_ZFOutputCallbackForFileDescriptor);
            ret.callbackSerializeCustomDataSet(customData);
        }
    }

    return ret;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFCallbackSerializeCustomType_ZFOutputCallbackForFileDescriptor)
{
    const ZFSerializableData *fileDescriptorData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFOutputCallbackForFileDescriptor_fileDescriptor, outErrorHint, outErrorPos);
    if(fileDescriptorData == zfnull)
    {
        return zffalse;
    }
    const zfchar *fileDescriptor = zfnull;
    if(!zfstringFromSerializableData(fileDescriptor, *fileDescriptorData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Create;
    {
        const ZFSerializableData *flagsData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFOutputCallbackForFileDescriptor_flags);
        if(flagsData != zfnull && !ZFFileOpenOptionFlagsFromSerializableData(flags, *flagsData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
    }
    serializableData.resolveMark();

    result = ZFOutputCallbackForFileDescriptor(fileDescriptor, zfindexMax, flags);
    return zftrue;
}

void ZFFileDescriptorTypeRegister(ZF_IN const zfchar *descriptorType,
                                  ZF_IN ZFFileDescriptorInputCallbackGetter inputCallbackGetter,
                                  ZF_IN ZFFileDescriptorOutputCallbackGetter outputCallbackGetter)
{
    _ZFP_ZFFileDescriptorCallbackData *callbackData = zfnew(_ZFP_ZFFileDescriptorCallbackData);
    callbackData->inputCallbackGetter = inputCallbackGetter;
    callbackData->outputCallbackGetter = outputCallbackGetter;
    _ZFP_ZFFileDescriptorTypeMap.set(descriptorType, ZFCorePointerForObject<_ZFP_ZFFileDescriptorCallbackData *>(callbackData));
}
void ZFFileDescriptorTypeUnregister(ZF_IN const zfchar *descriptorType)
{
    _ZFP_ZFFileDescriptorTypeMap.remove(descriptorType);
}

void ZFFileDescriptorTypeGetAll(ZF_OUT ZFCoreArray<const zfchar *> &ret)
{
    _ZFP_ZFFileDescriptorTypeMap.allKey(ret);
}

ZF_NAMESPACE_GLOBAL_END

