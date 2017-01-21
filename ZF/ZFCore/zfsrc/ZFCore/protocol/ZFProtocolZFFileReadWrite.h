/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFFileReadWrite.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFFileReadWrite_h_
#define _ZFI_ZFProtocolZFFileReadWrite_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFFileReadWrite)
public:
    /** @brief see #ZFFile::fileOpen */
    virtual ZFFileToken fileOpen(ZF_IN const zfchar *filePath,
                                 ZF_IN_OPT ZFFileOpenOptionFlags flag = ZFFileOpenOption::e_Read) zfpurevirtual;
    /** @brief see #ZFFile::fileClose */
    virtual zfbool fileClose(ZF_IN ZFFileToken token) zfpurevirtual;

    /** @brief see #ZFFile::fileTell */
    virtual zfindex fileTell(ZF_IN ZFFileToken token) zfpurevirtual;
    /** @brief see #ZFFile::fileSeek */
    virtual zfbool fileSeek(ZF_IN ZFFileToken token,
                            ZF_IN zfindex byteSize,
                            ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin) zfpurevirtual;

    /** @brief see #ZFFile::fileRead */
    virtual zfindex fileRead(ZF_IN ZFFileToken token,
                             ZF_IN void *buf,
                             ZF_IN zfindex maxByteSize) zfpurevirtual;

    /** @brief see #ZFFile::fileWrite */
    virtual zfindex fileWrite(ZF_IN ZFFileToken token,
                              ZF_IN const void *src,
                              ZF_IN zfindex maxByteSize) zfpurevirtual;
    /** @brief see #ZFFile::fileFlush */
    virtual void fileFlush(ZF_IN ZFFileToken token) zfpurevirtual;
    /** @brief see #ZFFile::fileEof */
    virtual zfbool fileEof(ZF_IN ZFFileToken token) zfpurevirtual;
    /** @brief see #ZFFile::fileError */
    virtual zfbool fileError(ZF_IN ZFFileToken token) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFFileReadWrite)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFFileReadWrite_h_

