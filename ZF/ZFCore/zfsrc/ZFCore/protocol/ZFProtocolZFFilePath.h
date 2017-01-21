/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFFilePath.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFFilePath_h_
#define _ZFI_ZFProtocolZFFilePath_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFFilePath)
public:
    /**
     * @brief see #ZFFile::modulePath
     */
    virtual const zfchar *modulePath(void) zfpurevirtual;
    /**
     * @brief see #ZFFile::moduleFilePath
     */
    virtual const zfchar *moduleFilePath(void) zfpurevirtual;

    /**
     * @brief see #ZFFile::settingPath
     */
    virtual const zfchar *settingPath(void) zfpurevirtual;
    /**
     * @brief see #ZFFile::settingPathSet
     */
    virtual void settingPathSet(ZF_IN const zfchar *path = zfnull) zfpurevirtual;

    /**
     * @brief see #ZFFile::storagePath
     */
    virtual const zfchar *storagePath(void) zfpurevirtual;
    /**
     * @brief see #ZFFile::storagePathSet
     */
    virtual void storagePathSet(ZF_IN const zfchar *path = zfnull) zfpurevirtual;

    /**
     * @brief see #ZFFile::storageSharedPath
     */
    virtual const zfchar *storageSharedPath(void) zfpurevirtual;
    /**
     * @brief see #ZFFile::storageSharedPathSet
     */
    virtual void storageSharedPathSet(ZF_IN const zfchar *path = zfnull) zfpurevirtual;

    /**
     * @brief see #ZFFile::cachePath
     */
    virtual const zfchar *cachePath(void) zfpurevirtual;
    /**
     * @brief see #ZFFile::cachePathSet
     */
    virtual void cachePathSet(ZF_IN const zfchar *path = zfnull) zfpurevirtual;
    /**
     * @brief see #ZFFile::cachePathClear
     */
    virtual void cachePathClear(void) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFFilePath)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFFilePath_h_

