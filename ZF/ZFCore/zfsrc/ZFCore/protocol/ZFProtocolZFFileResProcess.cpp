/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFProtocolZFFileResProcess.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFFileResProcess)

// ============================================================
zfbool _ZFP_ZFImpl_ZFFileResProcess_resPathFix(ZF_IN const zfstring &resRootPath, ZF_IN_OUT zfstring &result, ZF_IN const zfstring &original)
{
    result.assign(original, resRootPath.length());
    return zftrue;
}

// ============================================================
const zfstring &ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resRootPath(void)
{
    static zfstring d = zfstringWithFormat(zfText(".%cres%c"), ZFFile::fileSeparator, ZFFile::fileSeparator);
    return d;
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resCopy(ZF_IN const zfchar *resPath,
                                                             ZF_IN const zfchar *dstPath,
                                                             ZF_IN_OPT zfbool isRecursive /* = zftrue */,
                                                             ZF_IN_OPT zfbool isForce /* = zffalse */,
                                                             ZF_IN_OPT zfstring *errPos /* = zfnull */)
{
    zfstring tmpPath;
    tmpPath += this->resRootPath();
    tmpPath += resPath;
    if(errPos == zfnull)
    {
        return ZFFile::fileCopy(tmpPath, dstPath, isRecursive, isForce, zfnull);
    }
    else
    {
        zfstring errPosTmp;
        zfbool ret = ZFFile::fileCopy(tmpPath, dstPath, isRecursive, isForce, &errPosTmp);
        if(!ret)
        {
            if(zfstringFind(errPosTmp, this->resRootPath()) == 0)
            {
                *errPos += errPosTmp.cString() + this->resRootPath().length();
            }
            else
            {
                *errPos += errPosTmp;
            }
        }
        return ret;
    }
}

// ============================================================
// res RW
ZFFileToken ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resOpen(ZF_IN const zfchar *resPath)
{
    zfstring tmpPath;
    tmpPath += this->resRootPath();
    tmpPath += resPath;
    return ZFFile::fileOpen(tmpPath.cString(), ZFFileOpenOption::e_Read);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resClose(ZF_IN ZFFileToken token)
{
    return ZFFile::fileClose(token);
}
zfindex ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resTell(ZF_IN ZFFileToken token)
{
    return ZFFile::fileTell(token);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resSeek(ZF_IN ZFFileToken token,
                                                             ZF_IN zfindex byteSize,
                                                             ZF_IN_OPT ZFSeekPos position /* = ZFSeekPosBegin */)
{
    return ZFFile::fileSeek(token, byteSize, position);
}
zfindex ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resRead(ZF_IN ZFFileToken token,
                                                              ZF_IN void *buf,
                                                              ZF_IN zfindex maxByteSize)
{
    return ZFFile::fileRead(token, buf, maxByteSize);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resEof(ZF_IN ZFFileToken token)
{
    return ZFFile::fileEof(token);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resError(ZF_IN ZFFileToken token)
{
    return ZFFile::fileError(token);
}

// ============================================================
// res find
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resFindFirst(ZF_IN const zfchar *resPath,
                                                                  ZF_IN_OUT ZFFileFindDataContainer &fd)
{
    ZFFileFindDataContainer *normalFd = zfnew(ZFFileFindDataContainer);
    zfstring findPath;
    findPath += this->resRootPath();
    findPath += resPath;
    if(!ZFPROTOCOL_ACCESS(ZFFile)->fileFindFirst(findPath, *normalFd)
       || !_ZFP_ZFImpl_ZFFileResProcess_resPathFix(this->resRootPath(), fd.parentPath, normalFd->parentPath)
       || !_ZFP_ZFImpl_ZFFileResProcess_resPathFix(this->resRootPath(), fd.path, normalFd->path))
    {
        zfdelete(normalFd);
        return zffalse;
    }
    fd.name = normalFd->name;
    fd.fileIsFolder = normalFd->fileIsFolder;
    fd.nativeFd = normalFd;
    return zftrue;
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resFindNext(ZF_IN_OUT ZFFileFindDataContainer &fd)
{
    ZFFileFindDataContainer *normalFd = ZFCastStatic(ZFFileFindDataContainer *, fd.nativeFd);
    if(!ZFPROTOCOL_ACCESS(ZFFile)->fileFindNext(*normalFd)
       || !_ZFP_ZFImpl_ZFFileResProcess_resPathFix(this->resRootPath(), fd.parentPath, normalFd->parentPath)
       || !_ZFP_ZFImpl_ZFFileResProcess_resPathFix(this->resRootPath(), fd.path, normalFd->path))
    {
        return zffalse;
    }
    fd.name = normalFd->name;
    fd.fileIsFolder = normalFd->fileIsFolder;
    return zftrue;
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resFindClose(ZF_IN_OUT ZFFileFindDataContainer &fd)
{
    ZFFileFindDataContainer *normalFd = ZFCastStatic(ZFFileFindDataContainer *, fd.nativeFd);
    ZFPROTOCOL_ACCESS(ZFFile)->fileFindClose(*normalFd);
    zfdelete(normalFd);
    fd.nativeFd = zfnull;
}

ZF_NAMESPACE_GLOBAL_END

