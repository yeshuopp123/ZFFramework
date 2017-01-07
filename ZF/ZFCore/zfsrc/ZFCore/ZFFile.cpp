/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFFile.h"
#include "protocol/ZFProtocolZFFile.h"
#include "protocol/ZFProtocolZFFilePath.h"
#include "protocol/ZFProtocolZFFileReadWrite.h"
#include "protocol/ZFProtocolZFFileResProcess.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFFileFindDataPrivate
zfclassNotPOD _ZFP_ZFFileFindDataPrivate
{
public:
    typedef enum
    {
        NotStarted,
        NormalFileFind,
        ResFileFind,
        ResAdditionalFileFind,
    } FindStatus;
public:
    zfstring resFindFirstPathSaved;
    ZFFileFindDataContainer d;
    _ZFP_ZFFileFindDataPrivate::FindStatus findStatus;
    zfstring resAdditionalPathWithSeparator;
    ZFFileFindDataContainer dForResAdditionPath;

public:
    void resAdditionalPathCopy(void)
    {
        d.parentPath = dForResAdditionPath.parentPath;
        if(zfstringFind(d.parentPath, resAdditionalPathWithSeparator) == 0)
        {
            d.parentPath.remove(0, resAdditionalPathWithSeparator.length());
        }

        d.path = dForResAdditionPath.path;
        if(zfstringFind(d.path, resAdditionalPathWithSeparator) == 0)
        {
            d.path.remove(0, resAdditionalPathWithSeparator.length());
        }

        d.name = dForResAdditionPath.name;
        d.fileIsFolder = dForResAdditionPath.fileIsFolder;
        d.nativeFd = dForResAdditionPath.nativeFd;
    }

public:
    _ZFP_ZFFileFindDataPrivate(void)
    : resFindFirstPathSaved()
    , d()
    , findStatus(NotStarted)
    , resAdditionalPathWithSeparator()
    , dForResAdditionPath()
    {
    }
};

// ============================================================
// ZFFileFindData
ZFFileFindData::ZFFileFindData(void)
{
    d = zfpoolNew(_ZFP_ZFFileFindDataPrivate);
}
ZFFileFindData::~ZFFileFindData(void)
{
    zfpoolDelete(d);
    d = zfnull;
}
const zfchar *ZFFileFindData::fileParentPath(void) const
{
    return d->d.parentPath.cString();
}
const zfchar *ZFFileFindData::filePath(void) const
{
    return d->d.path.cString();
}
const zfchar *ZFFileFindData::fileName(void) const
{
    return d->d.name.cString();
}
zfbool ZFFileFindData::fileIsFolder(void) const
{
    return d->d.fileIsFolder;
}
void ZFFileFindData::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    if(this->fileIsFolder())
    {
        ret += zfText("(dir)");
    }
    ret += this->fileName();
    ret += ZFTOKEN_ZFObjectInfoRight;
}

// ============================================================
zfclassLikePOD _ZFP_ZFFileTokenForRes
{
public:
    ZFFileToken fd; // fd returned by ZFFile::fileOpen or ZFFile::resOpen
    zfstring resAdditionalPathWithSeparator; // not null if it's located in additional res path

public:
    _ZFP_ZFFileTokenForRes(void)
    : fd(ZFFileTokenInvalid)
    , resAdditionalPathWithSeparator()
    {
    }
};

// ============================================================
ZFENUM_DEFINE(ZFFileOpenOption)
ZFENUM_FLAGS_DEFINE(ZFFileOpenOption, ZFFileOpenOptionFlags)

// ============================================================
// implementation
#define _ZFP_ZFFileImpl ZFPROTOCOL_ACCESS(ZFFile)
#define _ZFP_ZFFilePathImpl ZFPROTOCOL_ACCESS(ZFFilePath)
#define _ZFP_ZFFileReadWriteImpl ZFPROTOCOL_ACCESS(ZFFileReadWrite)
#define _ZFP_ZFFileResProcessImpl ZFPROTOCOL_ACCESS(ZFFileResProcess)

// ============================================================
// ZFFile
const zfchar ZFFile::fileSeparator = '/';
const zfchar *ZFFile::fileSeparatorString = zfText("/");

ZFOBJECT_REGISTER(ZFFile)

ZFOBSERVER_EVENT_REGISTER(ZFFile, SettingPathOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFFile, StoragePathOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFFile, StorageSharedPathOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFFile, CachePathOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFFile, CachePathBeforeClear)
ZFOBSERVER_EVENT_REGISTER(ZFFile, CachePathAfterClear)

zfbool ZFFile::filePathFormat(ZF_OUT zfstring &ret,
                              ZF_IN const zfchar *src)
{
    if(src == zfnull)
    {
        return zffalse;
    }
    return _ZFP_ZFFileImpl->filePathFormat(ret, src);
}

void ZFFile::fileNameOf(ZF_OUT zfstring &ret, ZF_IN const zfchar *src)
{
    zfindex pos = zfstringFindReversely(src, zfindexMax, &ZFFile::fileSeparator, 1);
    if(pos != zfindexMax)
    {
        ret += (src + pos + 1);
    }
    else
    {
        ret += src;
    }
}
void ZFFile::fileNameOfWithoutExt(ZF_OUT zfstring &ret, ZF_IN const zfchar *src)
{
    zfindex len = zfslen(src);
    zfindex pos = zfstringFindReversely(src, len, &ZFFile::fileSeparator, 1);
    if(pos != zfindexMax)
    {
        ++pos;
    }
    else
    {
        pos = 0;
    }
    zfindex dotPos = zfstringFindReversely(src + pos, len - pos, zfText("."), 1);
    if(dotPos != zfindexMax)
    {
        ret.append(src + pos, dotPos - pos);
    }
    else
    {
        ret += (src + pos);
    }
}
void ZFFile::fileExtOf(ZF_OUT zfstring &ret, ZF_IN const zfchar *src)
{
    zfindex pos = zfstringFindReversely(src, zfindexMax, zfText("."), 1);
    if(pos != zfindexMax)
    {
        ret += (src + pos + 1);
    }
}
void ZFFile::fileParentPathOf(ZF_OUT zfstring &ret, ZF_IN const zfchar *src)
{
    zfindex pos = zfstringFindReversely(src, zfindexMax, &ZFFile::fileSeparator, 1);
    if(pos != zfindexMax)
    {
        ret.append(src, pos);
    }
}
void ZFFile::filePathComponentsOf(ZF_OUT ZFCoreArray<zfstring> &ret, ZF_IN const zfchar *src)
{
    zfindex len = zfslen(src);
    zfindex posL = 0;
    zfindex posR = len;
    do
    {
        posL = zfstringFindReversely(src, posR, &ZFFile::fileSeparator, 1);
        if(posL == zfindexMax)
        {
            if(posR > 0)
            {
                ret.add(zfstring(src, posR));
            }
            break;
        }
        else
        {
            if(posL != posR - 1)
            {
                ret.add(zfstring(src, posL + 1, posR - posL - 1));
            }
            posR = posL;
        }
    } while(zftrue);
}

zfbool ZFFile::fileIsExist(ZF_IN const zfchar *path)
{
    if(path == zfnull)
    {
        return zffalse;
    }
    return _ZFP_ZFFileImpl->fileIsExist(path);
}

zfbool ZFFile::fileIsFolder(ZF_IN const zfchar *path)
{
    if(path == zfnull)
    {
        return zffalse;
    }
    return _ZFP_ZFFileImpl->fileIsFolder(path);
}

zfbool ZFFile::filePathCreate(ZF_IN const zfchar *path,
                              ZF_IN_OPT zfbool autoMakeParent /* = zftrue */,
                              ZF_IN_OPT zfstring *errPos /* = zfnull */)
{
    if(path == zfnull)
    {
        return zffalse;
    }
    return _ZFP_ZFFileImpl->filePathCreate(path, autoMakeParent, errPos);
}

zfbool ZFFile::fileCopy(ZF_IN const zfchar *srcPath,
                        ZF_IN const zfchar *dstPath,
                        ZF_IN_OPT zfbool isRecursive /* = zftrue */,
                        ZF_IN_OPT zfbool isForce /* = zffalse */,
                        ZF_IN_OPT zfstring *errPos /* = zfnull */)
{
    if(srcPath == zfnull || dstPath == zfnull)
    {
        return zffalse;
    }
    return _ZFP_ZFFileImpl->fileCopy(srcPath, dstPath, isRecursive, isForce, errPos);
}

zfbool ZFFile::fileMove(ZF_IN const zfchar *srcPath,
                        ZF_IN const zfchar *dstPath,
                        ZF_IN_OPT zfbool isRecursive /* = zftrue */,
                        ZF_IN_OPT zfbool isForce /* = zffalse */,
                        ZF_IN_OPT zfstring *errPos /* = zfnull */)
{
    if(srcPath == zfnull || dstPath == zfnull)
    {
        return zffalse;
    }
    return _ZFP_ZFFileImpl->fileMove(srcPath, dstPath, isRecursive, isForce, errPos);
}
zfbool ZFFile::fileRemove(ZF_IN const zfchar *path,
                          ZF_IN_OPT zfbool isRecursive /* = zftrue */,
                          ZF_IN_OPT zfbool isForce /* = zffalse */,
                          ZF_IN_OPT zfstring *errPos /* = zfnull */)
{
    if(path == zfnull)
    {
        return zffalse;
    }
    return _ZFP_ZFFileImpl->fileRemove(path, isRecursive, isForce, errPos);
}

zfbool ZFFile::fileFindFirst(ZF_IN const zfchar *path,
                             ZF_IN_OUT ZFFileFindData &fd)
{
    if(path == zfnull)
    {
        return zffalse;
    }
    switch(fd.d->findStatus)
    {
        case _ZFP_ZFFileFindDataPrivate::NotStarted:
            break;
        case _ZFP_ZFFileFindDataPrivate::NormalFileFind:
            zfCoreCriticalMessage(zfTextA("have you forgot fileFindClose for last find operation?"));
            return zffalse;
        case _ZFP_ZFFileFindDataPrivate::ResFileFind:
        case _ZFP_ZFFileFindDataPrivate::ResAdditionalFileFind:
            zfCoreCriticalMessage(zfTextA("have you forgot resFindClose for last find operation?"));
            return zffalse;
        default:
            zfCoreCriticalShouldNotGoHere();
            return zffalse;
    }
    if(_ZFP_ZFFileImpl->fileFindFirst(path, fd.d->d))
    {
        fd.d->findStatus = _ZFP_ZFFileFindDataPrivate::NormalFileFind;
        return zftrue;
    }
    return zffalse;
}
zfbool ZFFile::fileFindNext(ZF_IN_OUT ZFFileFindData &fd)
{
    switch(fd.d->findStatus)
    {
        case _ZFP_ZFFileFindDataPrivate::NotStarted:
            zfCoreCriticalMessage(zfTextA("have you forgot fileFindFirst?"));
            return zffalse;
        case _ZFP_ZFFileFindDataPrivate::NormalFileFind:
            break;
        case _ZFP_ZFFileFindDataPrivate::ResFileFind:
        case _ZFP_ZFFileFindDataPrivate::ResAdditionalFileFind:
            zfCoreCriticalMessage(zfTextA("fileFindNext while started by resFindFirst"));
            return zffalse;
        default:
            zfCoreCriticalShouldNotGoHere();
            return zffalse;
    }
    return _ZFP_ZFFileImpl->fileFindNext(fd.d->d);
}
void ZFFile::fileFindClose(ZF_IN_OUT ZFFileFindData &fd)
{
    switch(fd.d->findStatus)
    {
        case _ZFP_ZFFileFindDataPrivate::NotStarted:
            zfCoreCriticalMessage(zfTextA("fileFindClose while not started by fileFindFirst or fileFindFirst failed"));
            return ;
        case _ZFP_ZFFileFindDataPrivate::NormalFileFind:
            break;
        case _ZFP_ZFFileFindDataPrivate::ResFileFind:
        case _ZFP_ZFFileFindDataPrivate::ResAdditionalFileFind:
            zfCoreCriticalMessage(zfTextA("fileFindClose while started by resFindFirst"));
            return ;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
    _ZFP_ZFFileImpl->fileFindClose(fd.d->d);
    fd.d->findStatus = _ZFP_ZFFileFindDataPrivate::NotStarted;
}

// ============================================================
const zfchar *ZFFile::modulePath(void)
{
    return _ZFP_ZFFilePathImpl->modulePath();
}
const zfchar *ZFFile::moduleFilePath(void)
{
    return _ZFP_ZFFilePathImpl->moduleFilePath();
}

const zfchar *ZFFile::settingPath(void)
{
    return _ZFP_ZFFilePathImpl->settingPath();
}
void ZFFile::settingPathSet(ZF_IN const zfchar *path /* = zfnull */)
{
    zfstring old = _ZFP_ZFFilePathImpl->settingPath();
    _ZFP_ZFFilePathImpl->settingPathSet(path);
    if(!old.isEmpty())
    {
        ZFPointerHolder *t = ZFPointerHolder::cacheAccess();
        t->holdedData = old.cString();
        ZFGlobalEventCenter::instance()->observerNotify(ZFFile::EventSettingPathOnChange(), t);
        ZFPointerHolder::cacheRelease(t);
    }
}

const zfchar *ZFFile::storagePath(void)
{
    return _ZFP_ZFFilePathImpl->storagePath();
}
void ZFFile::storagePathSet(ZF_IN const zfchar *path /* = zfnull */)
{
    zfstring old = _ZFP_ZFFilePathImpl->storagePath();
    _ZFP_ZFFilePathImpl->storagePathSet(path);
    if(!old.isEmpty())
    {
        ZFPointerHolder *t = ZFPointerHolder::cacheAccess();
        t->holdedData = old.cString();
        ZFGlobalEventCenter::instance()->observerNotify(ZFFile::EventStoragePathOnChange(), t);
        ZFPointerHolder::cacheRelease(t);
    }
}

const zfchar *ZFFile::storageSharedPath(void)
{
    return _ZFP_ZFFilePathImpl->storageSharedPath();
}
void ZFFile::storageSharedPathSet(ZF_IN const zfchar *path /* = zfnull */)
{
    zfstring old = _ZFP_ZFFilePathImpl->storageSharedPath();
    _ZFP_ZFFilePathImpl->storageSharedPathSet(path);
    if(!old.isEmpty())
    {
        ZFPointerHolder *t = ZFPointerHolder::cacheAccess();
        t->holdedData = old.cString();
        ZFGlobalEventCenter::instance()->observerNotify(ZFFile::EventStorageSharedPathOnChange(), t);
        ZFPointerHolder::cacheRelease(t);
    }
}

const zfchar *ZFFile::cachePath(void)
{
    return _ZFP_ZFFilePathImpl->cachePath();
}
void ZFFile::cachePathSet(ZF_IN const zfchar *path /* = zfnull */)
{
    zfstring old = _ZFP_ZFFilePathImpl->cachePath();
    _ZFP_ZFFilePathImpl->cachePathSet(path);
    if(!old.isEmpty())
    {
        ZFPointerHolder *t = ZFPointerHolder::cacheAccess();
        t->holdedData = old.cString();
        ZFGlobalEventCenter::instance()->observerNotify(ZFFile::EventCachePathOnChange(), t);
        ZFPointerHolder::cacheRelease(t);
    }
}

void ZFFile::cachePathClear(void)
{
    ZFGlobalEventCenter::instance()->observerNotify(ZFFile::EventCachePathBeforeClear());
    _ZFP_ZFFilePathImpl->cachePathClear();
    ZFGlobalEventCenter::instance()->observerNotify(ZFFile::EventCachePathAfterClear());
}

// ============================================================
ZFFileToken ZFFile::fileOpen(ZF_IN const zfchar *filePath,
                             ZF_IN_OPT ZFFileOpenOptionFlags flag /* = ZFFileOpenOption::e_Read */,
                             ZF_IN_OPT zfbool autoCreateParent /* = zftrue */)
{
    if(autoCreateParent && (zffalse
        || ZFBitTest(flag, ZFFileOpenOption::e_Create)
        || ZFBitTest(flag, ZFFileOpenOption::e_Write)
        || ZFBitTest(flag, ZFFileOpenOption::e_Append)
        ))
    {
        ZFFile::filePathCreate(ZFFile::fileParentPathOf(filePath).cString());
    }
    return _ZFP_ZFFileReadWriteImpl->fileOpen(filePath, flag);
}
zfbool ZFFile::fileClose(ZF_IN ZFFileToken token)
{
    return _ZFP_ZFFileReadWriteImpl->fileClose(token);
}
zfindex ZFFile::fileTell(ZF_IN ZFFileToken token)
{
    return _ZFP_ZFFileReadWriteImpl->fileTell(token);
}
zfbool ZFFile::fileSeek(ZF_IN ZFFileToken token,
                        ZF_IN zfindex byteSize,
                        ZF_IN_OPT ZFSeekPos position /* = ZFSeekPosBegin */)
{
    return _ZFP_ZFFileReadWriteImpl->fileSeek(token, byteSize, position);
}
zfindex ZFFile::fileRead(ZF_IN ZFFileToken token,
                         ZF_IN void *buf,
                         ZF_IN zfindex maxByteSize)
{
    return _ZFP_ZFFileReadWriteImpl->fileRead(token, buf, maxByteSize);
}
zfindex ZFFile::fileWrite(ZF_IN ZFFileToken token,
                          ZF_IN const void *src,
                          ZF_IN_OPT zfindex maxByteSize /* = zfindexMax */)
{
    if(src == zfnull)
    {
        return 0;
    }
    return _ZFP_ZFFileReadWriteImpl->fileWrite(token, src,
        (maxByteSize == zfindexMax) ? (sizeof(zfchar) * zfslen((const zfchar *)src)) : maxByteSize);
}
void ZFFile::fileFlush(ZF_IN ZFFileToken token)
{
    return _ZFP_ZFFileReadWriteImpl->fileFlush(token);
}
zfbool ZFFile::fileEof(ZF_IN ZFFileToken token)
{
    return _ZFP_ZFFileReadWriteImpl->fileEof(token);
}
zfbool ZFFile::fileError(ZF_IN ZFFileToken token)
{
    return _ZFP_ZFFileReadWriteImpl->fileError(token);
}
zfindex ZFFile::fileSize(ZF_IN ZFFileToken token)
{
    if(token == ZFFileTokenInvalid)
    {
        return zfindexMax;
    }
    zfindex saved = ZFFile::fileTell(token);
    if(saved == zfindexMax)
    {
        return zfindexMax;
    }
    ZFFile::fileSeek(token, 0, ZFSeekPosEnd);
    zfindex size = ZFFile::fileTell(token);
    ZFFile::fileSeek(token, saved, ZFSeekPosBegin);
    return size;
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFFileResAdditionalPathDataHolder, ZFLevelZFFrameworkHigh)
{
}
public:
    ZFCoreArray<zfstring> resAdditionalPathList;
ZF_GLOBAL_INITIALIZER_END(ZFFileResAdditionalPathDataHolder)
#define _ZFP_ZFFileResAdditionalPathList (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFFileResAdditionalPathDataHolder)->resAdditionalPathList)

void ZFFile::resAdditionalPathAdd(ZF_IN const zfchar *path)
{
    if(path == zfnull || *path == '\0')
    {
        return ;
    }
    _ZFP_ZFFileResAdditionalPathList.add(path);
}
void ZFFile::resAdditionalPathRemove(ZF_IN const zfchar *path)
{
    if(path == zfnull || *path == '\0')
    {
        return ;
    }
    _ZFP_ZFFileResAdditionalPathList.removeElement(path);
}
ZFCoreArray<zfstring> ZFFile::resAdditionalPathList(void)
{
    return _ZFP_ZFFileResAdditionalPathList;
}
const zfchar *ZFFile::resAdditionalPathCheck(ZF_IN const zfchar *resPath)
{
    for(zfindex i = 0; i < _ZFP_ZFFileResAdditionalPathList.count(); ++i)
    {
        zfstring t;
        t += _ZFP_ZFFileResAdditionalPathList[i];
        t += ZFFile::fileSeparator;
        t += resPath;
        if(ZFFile::fileIsExist(t))
        {
            return _ZFP_ZFFileResAdditionalPathList[i].cString();
        }
    }
    return zfnull;
}

zfbool ZFFile::resCopy(ZF_IN const zfchar *resPath,
                       ZF_IN const zfchar *dstPath,
                       ZF_IN_OPT zfbool isRecursive /* = zftrue */,
                       ZF_IN_OPT zfbool isForce /* = zffalse */,
                       ZF_IN_OPT zfstring *errPos /* = zfnull */)
{
    const zfchar *resAdditionalPath = ZFFile::resAdditionalPathCheck(resPath);
    if(resAdditionalPath == zfnull)
    {
        return _ZFP_ZFFileResProcessImpl->resCopy(resPath, dstPath, isRecursive, isForce, errPos);
    }
    else
    {
        zfstring resPathTmp;
        resPathTmp += resAdditionalPath;
        resPathTmp += ZFFile::fileSeparator;
        resPathTmp += resPath;
        zfstring errPosTmp;
        zfbool ret = _ZFP_ZFFileImpl->fileCopy(resPathTmp, dstPath, isRecursive, isForce, &errPosTmp);
        if(errPos != zfnull)
        {
            if(!ret)
            {
                zfstring header;
                header += resAdditionalPath;
                header += ZFFile::fileSeparator;
                if(zfstringFind(errPosTmp, header) == 0)
                {
                    errPosTmp.remove(0, header.length());
                }
            }
            *errPos += errPosTmp;
        }
        return ret;
    }
}
ZFFileToken ZFFile::resOpen(ZF_IN const zfchar *resPath)
{
    if(resPath == zfnull)
    {
        return ZFFileTokenInvalid;
    }

    _ZFP_ZFFileTokenForRes *ret = zfnew(_ZFP_ZFFileTokenForRes);
    const zfchar *resAdditionalPath = ZFFile::resAdditionalPathCheck(resPath);
    if(resAdditionalPath == zfnull)
    {
        ret->fd = _ZFP_ZFFileResProcessImpl->resOpen(resPath);
    }
    else
    {
        ret->resAdditionalPathWithSeparator = resAdditionalPath;
        ret->resAdditionalPathWithSeparator += ZFFile::fileSeparator;
        zfstring resPathTmp;
        resPathTmp += ret->resAdditionalPathWithSeparator;
        resPathTmp += resPath;
        ret->fd = _ZFP_ZFFileReadWriteImpl->fileOpen(resPathTmp);
    }
    if(ret->fd == ZFFileTokenInvalid)
    {
        zfdelete(ret);
        ret = zfnull;
    }
    return (ZFFileToken)ret;
}
zfbool ZFFile::resClose(ZF_IN ZFFileToken token)
{
    if(token == ZFFileTokenInvalid)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    zfblockedDelete(resToken);
    if(resToken->resAdditionalPathWithSeparator.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resClose(resToken->fd);
    }
    else
    {
        return _ZFP_ZFFileReadWriteImpl->fileClose(resToken->fd);
    }
}
zfindex ZFFile::resTell(ZF_IN ZFFileToken token)
{
    if(token == ZFFileTokenInvalid)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resAdditionalPathWithSeparator.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resTell(resToken->fd);
    }
    else
    {
        return _ZFP_ZFFileReadWriteImpl->fileTell(resToken->fd);
    }
}
zfbool ZFFile::resSeek(ZF_IN ZFFileToken token,
                       ZF_IN zfindex byteSize,
                       ZF_IN_OPT ZFSeekPos position /* = ZFSeekPosBegin */)
{
    if(token == ZFFileTokenInvalid)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resAdditionalPathWithSeparator.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resSeek(resToken->fd, byteSize, position);
    }
    else
    {
        return _ZFP_ZFFileReadWriteImpl->fileSeek(resToken->fd, byteSize, position);
    }
}
zfindex ZFFile::resRead(ZF_IN ZFFileToken token,
                        ZF_IN void *buf,
                        ZF_IN zfindex maxByteSize)
{
    if(token == ZFFileTokenInvalid)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resAdditionalPathWithSeparator.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resRead(resToken->fd, buf, maxByteSize);
    }
    else
    {
        return _ZFP_ZFFileReadWriteImpl->fileRead(resToken->fd, buf, maxByteSize);
    }
}
zfbool ZFFile::resEof(ZF_IN ZFFileToken token)
{
    if(token == ZFFileTokenInvalid)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resAdditionalPathWithSeparator.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resEof(resToken->fd);
    }
    else
    {
        return _ZFP_ZFFileReadWriteImpl->fileEof(resToken->fd);
    }
}
zfbool ZFFile::resError(ZF_IN ZFFileToken token)
{
    if(token == ZFFileTokenInvalid)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resAdditionalPathWithSeparator.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resError(resToken->fd);
    }
    else
    {
        return _ZFP_ZFFileReadWriteImpl->fileError(resToken->fd);
    }
}

zfbool ZFFile::resFindFirst(ZF_IN const zfchar *resPath,
                            ZF_IN_OUT ZFFileFindData &fd)
{
    if(resPath == zfnull)
    {
        return zffalse;
    }
    switch(fd.d->findStatus)
    {
        case _ZFP_ZFFileFindDataPrivate::NotStarted:
            break;
        case _ZFP_ZFFileFindDataPrivate::NormalFileFind:
            zfCoreCriticalMessage(zfTextA("have you forgot fileFindClose for last find operation?"));
            return zffalse;
        case _ZFP_ZFFileFindDataPrivate::ResFileFind:
        case _ZFP_ZFFileFindDataPrivate::ResAdditionalFileFind:
            zfCoreCriticalMessage(zfTextA("have you forgot resFindClose for last find operation?"));
            return zffalse;
        default:
            zfCoreCriticalShouldNotGoHere();
            return zffalse;
    }

    fd.d->resFindFirstPathSaved = resPath;
    const zfchar *resAdditionalPath = ZFFile::resAdditionalPathCheck(resPath);
    if(resAdditionalPath == zfnull)
    {
        fd.d->resAdditionalPathWithSeparator.removeAll();
        if(_ZFP_ZFFileResProcessImpl->resFindFirst(resPath, fd.d->d))
        {
            fd.d->findStatus = _ZFP_ZFFileFindDataPrivate::ResFileFind;
            return zftrue;
        }
        return zffalse;
    }
    else
    {
        fd.d->resAdditionalPathWithSeparator = resAdditionalPath;
        fd.d->resAdditionalPathWithSeparator += ZFFile::fileSeparator;
        zfstring resPathTmp;
        resPathTmp += fd.d->resAdditionalPathWithSeparator;
        resPathTmp += resPath;
        if(_ZFP_ZFFileImpl->fileFindFirst(resPathTmp, fd.d->dForResAdditionPath))
        {
            fd.d->findStatus = _ZFP_ZFFileFindDataPrivate::ResFileFind;
            fd.d->resAdditionalPathCopy();
            return zftrue;
        }
        else
        {
            fd.d->resAdditionalPathWithSeparator.removeAll();
            if(_ZFP_ZFFileResProcessImpl->resFindFirst(resPath, fd.d->d))
            {
                fd.d->findStatus = _ZFP_ZFFileFindDataPrivate::ResFileFind;
                return zftrue;
            }
            return zffalse;
        }
    }
}
zfbool ZFFile::resFindNext(ZF_IN_OUT ZFFileFindData &fd)
{
    switch(fd.d->findStatus)
    {
        case _ZFP_ZFFileFindDataPrivate::NotStarted:
            zfCoreCriticalMessage(zfTextA("have you forgot resFindFirst?"));
            return zffalse;
        case _ZFP_ZFFileFindDataPrivate::NormalFileFind:
            zfCoreCriticalMessage(zfTextA("resFindNext while started by fileFindFirst"));
            return zffalse;
        case _ZFP_ZFFileFindDataPrivate::ResFileFind:
        case _ZFP_ZFFileFindDataPrivate::ResAdditionalFileFind:
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return zffalse;
    }

    if(fd.d->resAdditionalPathWithSeparator.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resFindNext(fd.d->d);
    }
    else
    {
        if(_ZFP_ZFFileImpl->fileFindNext(fd.d->dForResAdditionPath))
        {
            fd.d->resAdditionalPathCopy();
            return zftrue;
        }
        else
        {
            fd.d->resAdditionalPathWithSeparator.removeAll();
            if(_ZFP_ZFFileResProcessImpl->resFindFirst(fd.d->resFindFirstPathSaved, fd.d->d))
            {
                return zftrue;
            }
            return zffalse;
        }
    }
}
void ZFFile::resFindClose(ZF_IN_OUT ZFFileFindData &fd)
{
    switch(fd.d->findStatus)
    {
        case _ZFP_ZFFileFindDataPrivate::NotStarted:
            zfCoreCriticalMessage(zfTextA("resFindClose while not started by resFindFirst or resFindFirst failed"));
            return ;
        case _ZFP_ZFFileFindDataPrivate::NormalFileFind:
            zfCoreCriticalMessage(zfTextA("resFindClose while started by fileFindFirst"));
            return ;
        case _ZFP_ZFFileFindDataPrivate::ResFileFind:
        case _ZFP_ZFFileFindDataPrivate::ResAdditionalFileFind:
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }

    if(fd.d->resAdditionalPathWithSeparator.isEmpty())
    {
        _ZFP_ZFFileResProcessImpl->resFindClose(fd.d->d);
    }
    else
    {
        _ZFP_ZFFileImpl->fileFindClose(fd.d->dForResAdditionPath);
    }
    fd.d->resAdditionalPathWithSeparator.removeAll();
    fd.d->findStatus = _ZFP_ZFFileFindDataPrivate::NotStarted;
}
zfindex ZFFile::resSize(ZF_IN ZFFileToken token)
{
    if(token == ZFFileTokenInvalid)
    {
        return zfindexMax;
    }
    zfindex saved = ZFFile::resTell(token);
    ZFFile::resSeek(token, 0, ZFSeekPosEnd);
    zfindex size = ZFFile::resTell(token);
    ZFFile::resSeek(token, saved, ZFSeekPosBegin);
    return size;
}

ZF_NAMESPACE_GLOBAL_END

