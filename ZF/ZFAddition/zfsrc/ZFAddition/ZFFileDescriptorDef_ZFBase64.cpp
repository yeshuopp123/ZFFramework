/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFFileDescriptorDef_ZFBase64.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static ZFInputCallback _ZFP_ZFFileDescriptorInputCallbackGetter_Base64(ZF_IN const zfchar *fileDescriptorData,
                                                                       ZF_IN_OPT zfindex dataLen /* = zfindexMax */,
                                                                       ZF_IN_OPT ZFFileOpenOptionFlags flags /* = ZFFileOpenOption::e_Read */,
                                                                       ZF_IN_OPT const ZFFileBOM *autoSkipBOMTable /* = &ZFFileBOMUTF8 */,
                                                                       ZF_IN_OPT zfindex autoSkipBOMTableCount /* = 1 */)
{
    if(dataLen == zfindexMax)
    {
        dataLen = zfslen(fileDescriptorData);
    }
    zfindex base64Size = ZFBase64DecodeCalcSize(dataLen);
    if(base64Size == 0)
    {
        return ZFInputCallbackForBuffer(zfText(""), 0);
    }
    zfbyte *base64Buf = (zfbyte *)zfmalloc(base64Size);
    zfblockedAlloc(ZFTypeHolder, bufSaved, base64Buf, ZFTypeHolder::DeletePOD);
    zfindex decodedSize = 0;
    if(!ZFBase64Decode(base64Buf, fileDescriptorData, dataLen, &decodedSize))
    {
        return ZFCallbackNull();
    }
    ZFInputCallback ret = ZFInputCallbackForBuffer(base64Buf, decodedSize);
    ret.callbackTagSet(zfText("Base64Buffer"), bufSaved);
    return ret;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFFileDescriptorTypeRegister_Base64, ZFLevelZFFrameworkNormal)
{
    ZFFileDescriptorTypeRegister(
        ZFFileDescriptorType_Base64,
        _ZFP_ZFFileDescriptorInputCallbackGetter_Base64,
        zfnull);
}
ZF_GLOBAL_INITIALIZER_END(ZFFileDescriptorTypeRegister_Base64)

ZF_NAMESPACE_GLOBAL_END

