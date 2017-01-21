/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFFile.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFFile_h_
#define _ZFI_ZFProtocolZFFile_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief data container for ZFProtocolZFFile's implementation
 *   to pass result and temporary data when find file
 *
 * required fields for implementation to fill:
 * -  parentPath: current search's parent path
 * -  path: current search's result's path
 * -  name: current search's result's name
 *
 * optional fields:
 * -  nativeFd: find description used to store find data,
 *   you may allocate your own type when implements,
 *   but only when fileFindFirst would return zftrue,
 *   and, you must deallocate it when fileFindClose
 *
 * @warning this is used for implementations of ZFProtocolZFFile only
 * @warning when implement ZFProtocolZFFile's find operation
 *   (fileFindFirst, fileFindNext, fileFindClose),
 *   you must fill all the required fields
 */
zfclassLikePOD ZF_ENV_EXPORT ZFFileFindDataContainer
{
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFFileFindDataContainer)
public:
    /**
     * @brief see #ZFFileFindDataContainer
     */
    zfstring parentPath;
    /**
     * @brief see #ZFFileFindDataContainer
     */
    zfstring path;
    /**
     * @brief see #ZFFileFindDataContainer
     */
    zfstring name;
    /**
     * @brief true if current file is directory
     */
    zfbool fileIsFolder;
    /**
     * @brief see #ZFFileFindDataContainer
     */
    void *nativeFd;
public:
    /**
     * @brief main constructor
     */
    ZFFileFindDataContainer(void)
    : parentPath()
    , path()
    , name()
    , fileIsFolder(zffalse)
    , nativeFd(zfnull)
    {
    }
};

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFFile)
public:
    /**
     * @brief see #ZFFile::filePathFormat
     */
    virtual zfbool filePathFormat(ZF_OUT zfstring &ret,
                                  ZF_IN const zfchar *src) zfpurevirtual;
    /**
     * @brief see #ZFFile::fileIsExist
     */
    virtual zfbool fileIsExist(ZF_IN const zfchar *path) zfpurevirtual;
    /**
     * @brief see #ZFFile::fileIsFolder
     */
    virtual zfbool fileIsFolder(ZF_IN const zfchar *path) zfpurevirtual;

    /**
     * @brief see #ZFFile::filePathCreate
     */
    virtual zfbool filePathCreate(ZF_IN const zfchar *path,
                                  ZF_IN_OPT zfbool autoMakeParent = zffalse,
                                  ZF_IN_OPT zfstring *errPos = zfnull) zfpurevirtual;
    /**
     * @brief see #ZFFile::fileCopy
     */
    virtual zfbool fileCopy(ZF_IN const zfchar *srcPath,
                            ZF_IN const zfchar *dstPath,
                            ZF_IN_OPT zfbool isRecursive = zftrue,
                            ZF_IN_OPT zfbool isForce = zffalse,
                            ZF_IN_OPT zfstring *errPos = zfnull) zfpurevirtual;
    /**
     * @brief see #ZFFile::fileMove
     */
    virtual zfbool fileMove(ZF_IN const zfchar *srcPath,
                            ZF_IN const zfchar *dstPath,
                            ZF_IN_OPT zfbool isRecursive = zftrue,
                            ZF_IN_OPT zfbool isForce = zffalse,
                            ZF_IN_OPT zfstring *errPos = zfnull) zfpurevirtual;
    /**
     * @brief see #ZFFile::fileRemove
     */
    virtual zfbool fileRemove(ZF_IN const zfchar *path,
                              ZF_IN_OPT zfbool isRecursive = zftrue,
                              ZF_IN_OPT zfbool isForce = zffalse,
                              ZF_IN_OPT zfstring *errPos = zfnull) zfpurevirtual;

    /**
     * @brief see #ZFFile::fileFindFirst
     *
     * use ZFFileFindDataContainer.nativeFd to store find data (if necessary),
     * use ZFFileFindDataContainer to store find result\n
     * remember that you must remove the folder "." and "..",
     * and if there is only two folder named "." and "..",
     * you should return false for fileFindFirst\n
     * if returned zffalse, no fileFindClose would be called anymore
     * @warning it's the implementation's responsibility
     *   to allocate and deallocate the nativeFd,
     *   and, if fileFindFirst would return zffalse,
     *   you must deallocate the nativeFd (if allocated) before return
     * @see ZFFileFindDataContainer
     */
    virtual zfbool fileFindFirst(ZF_IN const zfchar *path,
                                 ZF_IN_OUT ZFFileFindDataContainer &fd) zfpurevirtual;
    /**
     * @brief see #ZFFile::fileFindNext, fileFindFirst
     */
    virtual zfbool fileFindNext(ZF_IN_OUT ZFFileFindDataContainer &fd) zfpurevirtual;
    /**
     * @brief see #ZFFile::fileFindClose, fileFindFirst
     */
    virtual void fileFindClose(ZF_IN_OUT ZFFileFindDataContainer &fd) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFFile)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFFile_h_

