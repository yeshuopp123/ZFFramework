/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFFileDescriptorDef_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static ZFInputCallback _ZFP_ZFFileDescriptorInputCallbackGetter_NativeLocal(ZF_IN const zfchar *fileDescriptorData,
                                                                            ZF_IN_OPT zfindex dataLen /* = zfindexMax */,
                                                                            ZF_IN_OPT ZFFileOpenOptionFlags flags /* = ZFFileOpenOption::e_Read */,
                                                                            ZF_IN_OPT const ZFFileBOM *autoSkipBOMTable /* = &ZFFileBOMUTF8 */,
                                                                            ZF_IN_OPT zfindex autoSkipBOMTableCount /* = 1 */)
{
    return ZFInputCallbackForFile(
        ((dataLen == zfindexMax) ? fileDescriptorData : zfstring(fileDescriptorData, dataLen).cString()),
        flags,
        autoSkipBOMTable, autoSkipBOMTableCount);
}
static ZFOutputCallback _ZFP_ZFFileDescriptorOutputCallbackGetter_NativeLocal(ZF_IN const zfchar *fileDescriptorData,
                                                                              ZF_IN_OPT zfindex dataLen /* = zfindexMax */,
                                                                              ZF_IN_OPT ZFFileOpenOptionFlags flags /* = ZFFileOpenOption::e_Create */)
{
    return ZFOutputCallbackForFile(
        ((dataLen == zfindexMax) ? fileDescriptorData : zfstring(fileDescriptorData, dataLen).cString()),
        flags);
}

// ============================================================
#define _ZFP_ZFFileDescriptorInputCallbackGetter(FileDescriptorType) \
    static ZFInputCallback _ZFP_ZFFileDescriptorInputCallbackGetter_##FileDescriptorType(ZF_IN const zfchar *fileDescriptorData, \
                                                                                         ZF_IN_OPT zfindex dataLen /* = zfindexMax */, \
                                                                                         ZF_IN_OPT ZFFileOpenOptionFlags flags /* = ZFFileOpenOption::e_Read */, \
                                                                                         ZF_IN_OPT const ZFFileBOM *autoSkipBOMTable /* = &ZFFileBOMUTF8 */, \
                                                                                         ZF_IN_OPT zfindex autoSkipBOMTableCount /* = 1 */) \
    { \
        return ZFInputCallbackForFile(zfsConnectLineFree( \
                ZFFile::FileDescriptorType(), \
                ZFFile::fileSeparatorString, \
                ((dataLen == zfindexMax) ? fileDescriptorData : zfstring(fileDescriptorData, dataLen).cString()) \
            ), \
            flags, \
            autoSkipBOMTable, autoSkipBOMTableCount); \
    }
#define _ZFP_ZFFileDescriptorOutputCallbackGetter(FileDescriptorType) \
    static ZFOutputCallback _ZFP_ZFFileDescriptorOutputCallbackGetter_##FileDescriptorType(ZF_IN const zfchar *fileDescriptorData, \
                                                                                           ZF_IN_OPT zfindex dataLen /* = zfindexMax */, \
                                                                                           ZF_IN_OPT ZFFileOpenOptionFlags flags /* = ZFFileOpenOption::e_Create */) \
    { \
        return ZFOutputCallbackForFile(zfsConnectLineFree( \
                ZFFile::FileDescriptorType(), \
                ZFFile::fileSeparatorString, \
                ((dataLen == zfindexMax) ? fileDescriptorData : zfstring(fileDescriptorData, dataLen).cString()) \
            ), \
            flags); \
    }

// ============================================================
static ZFInputCallback _ZFP_ZFFileDescriptorInputCallbackGetterForResFile(ZF_IN const zfchar *fileDescriptorData,
                                                                          ZF_IN_OPT zfindex dataLen /* = zfindexMax */,
                                                                          ZF_IN_OPT ZFFileOpenOptionFlags flags /* = ZFFileOpenOption::e_Read */,
                                                                          ZF_IN_OPT const ZFFileBOM *autoSkipBOMTable /* = &ZFFileBOMUTF8 */,
                                                                          ZF_IN_OPT zfindex autoSkipBOMTableCount /* = 1 */)
{
    return ZFInputCallbackForResFile(
        (dataLen == zfindexMax) ? fileDescriptorData : zfstring(fileDescriptorData, dataLen).cString(),
        autoSkipBOMTable, autoSkipBOMTableCount);
}

_ZFP_ZFFileDescriptorInputCallbackGetter(modulePath)
_ZFP_ZFFileDescriptorOutputCallbackGetter(modulePath)

_ZFP_ZFFileDescriptorInputCallbackGetter(settingPath)
_ZFP_ZFFileDescriptorOutputCallbackGetter(settingPath)

_ZFP_ZFFileDescriptorInputCallbackGetter(storagePath)
_ZFP_ZFFileDescriptorOutputCallbackGetter(storagePath)

_ZFP_ZFFileDescriptorInputCallbackGetter(storageSharedPath)
_ZFP_ZFFileDescriptorOutputCallbackGetter(storageSharedPath)

_ZFP_ZFFileDescriptorInputCallbackGetter(cachePath)
_ZFP_ZFFileDescriptorOutputCallbackGetter(cachePath)

static ZFInputCallback _ZFP_ZFFileDescriptorInputCallbackGetterForText(ZF_IN const zfchar *fileDescriptorData,
                                                                       ZF_IN_OPT zfindex dataLen /* = zfindexMax */,
                                                                       ZF_IN_OPT ZFFileOpenOptionFlags flags /* = ZFFileOpenOption::e_Read */,
                                                                       ZF_IN_OPT const ZFFileBOM *autoSkipBOMTable /* = &ZFFileBOMUTF8 */,
                                                                       ZF_IN_OPT zfindex autoSkipBOMTableCount /* = 1 */)
{
    return ZFInputCallbackForBuffer(fileDescriptorData, dataLen);
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFFileDescriptorTypeRegister_common, ZFLevelZFFrameworkNormal)
{
    ZFFileDescriptorTypeRegister(
        ZFFileDescriptorType_NativeLocal,
        _ZFP_ZFFileDescriptorInputCallbackGetter_NativeLocal,
        _ZFP_ZFFileDescriptorOutputCallbackGetter_NativeLocal);

    ZFFileDescriptorTypeRegister(
        ZFFileDescriptorType_Res,
        _ZFP_ZFFileDescriptorInputCallbackGetterForResFile,
        zfnull);

    ZFFileDescriptorTypeRegister(
        ZFFileDescriptorType_ModulePath,
        _ZFP_ZFFileDescriptorInputCallbackGetter_modulePath,
        _ZFP_ZFFileDescriptorOutputCallbackGetter_modulePath);

    ZFFileDescriptorTypeRegister(
        ZFFileDescriptorType_SettingPath,
        _ZFP_ZFFileDescriptorInputCallbackGetter_settingPath,
        _ZFP_ZFFileDescriptorOutputCallbackGetter_settingPath);

    ZFFileDescriptorTypeRegister(
        ZFFileDescriptorType_StoragePath,
        _ZFP_ZFFileDescriptorInputCallbackGetter_storagePath,
        _ZFP_ZFFileDescriptorOutputCallbackGetter_storagePath);

    ZFFileDescriptorTypeRegister(
        ZFFileDescriptorType_StorageSharedPath,
        _ZFP_ZFFileDescriptorInputCallbackGetter_storageSharedPath,
        _ZFP_ZFFileDescriptorOutputCallbackGetter_storageSharedPath);

    ZFFileDescriptorTypeRegister(
        ZFFileDescriptorType_CachePath,
        _ZFP_ZFFileDescriptorInputCallbackGetter_cachePath,
        _ZFP_ZFFileDescriptorOutputCallbackGetter_cachePath);

    ZFFileDescriptorTypeRegister(
        ZFFileDescriptorType_Text,
        _ZFP_ZFFileDescriptorInputCallbackGetterForText,
        zfnull);
}
ZF_GLOBAL_INITIALIZER_END(ZFFileDescriptorTypeRegister_common)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFFileCacheAutoCleanup, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFFileCacheAutoCleanup)
{
    ZFFileFindData fd;
    if(ZFFile::fileFindFirst(ZFFile::cachePath(), fd))
    {
        do {
            ZFFile::fileRemove(fd.filePath());
        } while(ZFFile::fileFindNext(fd));
        ZFFile::fileFindClose(fd);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFFileCacheAutoCleanup)

ZF_NAMESPACE_GLOBAL_END

