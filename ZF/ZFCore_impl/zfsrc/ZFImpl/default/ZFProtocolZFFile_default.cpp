/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFFile.h"
#include "ZFCore/ZFString.h"
#include "ZFCore/ZFFile.h"

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    #include <unistd.h>
    #include <dirent.h>
    #include <errno.h>
    #include <sys/stat.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileImpl_default, ZFFile, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("nativeAPI"))
public:
    virtual zfbool filePathFormat(ZF_OUT zfstring &ret,
                                  ZF_IN const zfchar *src)
    {
        if(src == zfnull) {return zffalse;}
        const zfchar *p = src;
        while(*p != '\0')
        {
            if(*p == '/')
            {
                do
                {
                    ++p;
                } while(*p != '\0'
                    && *p == '/');
                if(ret.isEmpty() || ret[ret.length() - 1] != ZFFile::fileSeparator)
                {
                    ret += ZFFile::fileSeparator;
                }
            }
            else if(*p == '\\')
            {
                ++p;
                if(*p == ' '
                    || *p == '"')
                {
                    ret += *p;
                    ++p;
                }
                else
                {
                    while(*p != '\0'
                        && (*p == '/' || *p == '\\'))
                    {
                        ++p;
                    }
                    if(ret.isEmpty() || ret[ret.length() - 1] != ZFFile::fileSeparator)
                    {
                        ret += ZFFile::fileSeparator;
                    }
                }
            }
            else
            {
                zfcharAppendAndMoveNext(ret, p);
            }
        }
        if(!ret.isEmpty())
        {
            if(ret[ret.length() - 1] == ZFFile::fileSeparator)
            {
                ret.remove(ret.length() - 1);
            }
        }
        return zftrue;
    }
    virtual zfbool fileIsExist(ZF_IN const zfchar *path)
    {
        if(path == zfnull) {return zffalse;}
        #if ZF_ENV_sys_Windows
            return (::GetFileAttributesW(ZFStringZ2W(path)) != 0xFFFFFFFF);
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            return (::access(ZFStringZ2A(path), F_OK) != -1);
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    }
    virtual zfbool fileIsFolder(ZF_IN const zfchar *path)
    {
        #if ZF_ENV_sys_Windows
            return ((::GetFileAttributesW(ZFStringZ2W(path)) & FILE_ATTRIBUTE_DIRECTORY) != 0);
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            struct stat statbuf;
            if(::lstat(ZFStringZ2A(path), &statbuf) <0) {return zffalse;}
            return S_ISDIR(statbuf.st_mode);
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    }

    virtual zfbool filePathCreate(ZF_IN const zfchar *path,
                                  ZF_IN_OPT zfbool autoMakeParent = zffalse,
                                  ZF_IN_OPT zfstring *errPos = zfnull)
    {
        if(autoMakeParent)
        {
            return this->makePath(path, errPos);
        }
        else
        {
            return this->makeDir(path, errPos);
        }
    }
private:
    zfbool cp_or_mv(ZF_IN zfbool isCopy,
                    ZF_IN const zfchar *srcPath,
                    ZF_IN const zfchar *dstPath,
                    ZF_IN_OPT zfbool isRecursive,
                    ZF_IN_OPT zfbool isForce,
                    ZF_IN_OPT zfstring *errPos)
    {
        if(srcPath == zfnull || dstPath == zfnull)
        {
            zfself::SetErrPos(errPos, zfnull);
            return zffalse;
        }
        if(!this->fileIsExist(srcPath))
        {
            zfself::SetErrPos(errPos, srcPath);
            return zffalse;
        }
        zfbool srcIsDir = this->fileIsFolder(srcPath);
        if(srcIsDir && !isRecursive)
        {
            zfself::SetErrPos(errPos, srcPath);
            return zffalse;
        }
        zfbool dstExist = this->fileIsExist(dstPath);
        zfbool dstIsDir = this->fileIsFolder(dstPath);
        if(dstExist && srcIsDir != dstIsDir)
        {
            zfself::SetErrPos(errPos, dstPath);
            return zffalse;
        }

        if(!this->makePath(dstPath, errPos, !srcIsDir))
        {
            return zffalse;
        }
        if(srcIsDir)
        {
            return this->copyOrMoveFolder(isCopy, srcPath, dstPath, isForce, errPos);
        }
        else
        {
            if(isCopy)
            {
                return this->copyFile(srcPath, dstPath, isForce, errPos);
            }
            else
            {
                return this->moveFile(srcPath, dstPath, isForce, errPos);
            }
        }
    }
public:
    virtual zfbool fileCopy(ZF_IN const zfchar *srcPath,
                            ZF_IN const zfchar *dstPath,
                            ZF_IN_OPT zfbool isRecursive = zftrue,
                            ZF_IN_OPT zfbool isForce = zffalse,
                            ZF_IN_OPT zfstring *errPos = zfnull)
    {
        ZFFile::filePathCreate(ZFFile::fileParentPathOf(dstPath));
        return this->cp_or_mv(zftrue, srcPath, dstPath, isRecursive, isForce, errPos);
    }
    virtual zfbool fileMove(ZF_IN const zfchar *srcPath,
                            ZF_IN const zfchar *dstPath,
                            ZF_IN_OPT zfbool isRecursive = zftrue,
                            ZF_IN_OPT zfbool isForce = zffalse,
                            ZF_IN_OPT zfstring *errPos = zfnull)
    {
        return this->cp_or_mv(zffalse, srcPath, dstPath, isRecursive, isForce, errPos);
    }
    virtual zfbool fileRemove(ZF_IN const zfchar *path,
                              ZF_IN_OPT zfbool isRecursive = zftrue,
                              ZF_IN_OPT zfbool isForce = zffalse,
                              ZF_IN_OPT zfstring *errPos = zfnull)
    {
        if(!this->fileIsExist(path)) {return zftrue;}
        if(this->fileIsFolder(path))
        {
            if(!isRecursive)
            {
                zfself::SetErrPos(errPos, path);
                return zffalse;
            }
            return this->removeFolder(path, isForce, errPos);
        }
        else
        {
            return this->removeFile(path, isForce, errPos);
        }
    }

    zfclassNotPOD _ZFP_ZFFileNativeFd
    {
    public:
    #if ZF_ENV_sys_Windows
        _ZFP_ZFFileNativeFd(void)
        {
            zfmemset(&this->fd, 0, sizeof(WIN32_FIND_DATAW));
            this->hFind = zfnull;
        }
        WIN32_FIND_DATAW fd;
        HANDLE hFind;
        void setup(ZFFileFindDataContainer &zfd)
        {
            zfd.name = ZFStringW2Z(fd.cFileName);

            zfd.path = zfd.parentPath;
            zfd.path += ZFFile::fileSeparator;
            zfd.path += zfd.name;

            zfd.fileIsFolder = ZFFile::fileIsFolder(zfd.path.cString());
        }
    #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
        _ZFP_ZFFileNativeFd(void)
        {
            this->pDir = zfnull;
            this->pDirent = zfnull;
            zfmemset(&this->fStat, 0, sizeof(struct stat));
        }
        DIR *pDir;
        struct dirent *pDirent;
        struct stat fStat;
        void setup(ZFFileFindDataContainer &zfd)
        {
            zfd.name = ZFStringA2Z(pDirent->d_name);

            zfd.path = zfd.parentPath;
            zfd.path += ZFFile::fileSeparator;
            zfd.path += zfd.name;

            zfd.fileIsFolder = ZFFile::fileIsFolder(zfd.path.cString());
        }
    #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    };

    virtual zfbool fileFindFirst(ZF_IN const zfchar *path,
                                 ZF_IN_OUT ZFFileFindDataContainer &fd)
    {
        if(path == zfnull) {return zffalse;}
        fd.parentPath = path;
        _ZFP_ZFFileNativeFd *nativeFd = zfnew(_ZFP_ZFFileNativeFd);
        fd.nativeFd = nativeFd;

        zfbool success = zffalse;
        do
        {
        #if ZF_ENV_sys_Windows
            zfstring tmp;
            tmp += fd.parentPath;
            tmp += ZFFile::fileSeparator;
            tmp += '*';
            nativeFd->hFind = ::FindFirstFileW(
                ZFStringZ2W(tmp.cString()),
                &(nativeFd->fd));
            if(nativeFd->hFind == INVALID_HANDLE_VALUE) {break;}

            nativeFd->setup(fd);
            success = zftrue;
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            nativeFd->pDir = ::opendir(ZFStringZ2A(fd.parentPath.cString()));
            if(nativeFd->pDir == zfnull) {break;}
            nativeFd->pDirent = ::readdir(nativeFd->pDir);
            if(nativeFd->pDirent == zfnull) {break;}

            nativeFd->setup(fd);
            success = zftrue;
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
        } while(zffalse);
        if(success)
        {
            while(zfscmpTheSame(fd.name.cString(), zfText("."))
                || zfscmpTheSame(fd.name.cString(), zfText("..")))
            {
                if(!this->fileFindNext(fd))
                {
                    this->fileFindClose(fd);
                    return zffalse;
                }
            }
            return zftrue;
        }
        else
        {
            this->fileFindClose(fd);
            return zffalse;
        }
    }
    virtual zfbool fileFindNext(ZF_IN_OUT ZFFileFindDataContainer &fd)
    {
        _ZFP_ZFFileNativeFd *nativeFd = (_ZFP_ZFFileNativeFd *)fd.nativeFd;
        #if ZF_ENV_sys_Windows
            if(!::FindNextFileW(nativeFd->hFind, &(nativeFd->fd))) {return zffalse;}
            nativeFd->setup(fd);
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            nativeFd->pDirent = ::readdir(nativeFd->pDir);
            if(nativeFd->pDirent == zfnull) {return zffalse;}
            nativeFd->setup(fd);
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
        if(zfscmpTheSame(fd.name.cString(), zfText("."))
            || zfscmpTheSame(fd.name.cString(), zfText("..")))
        {
            return this->fileFindNext(fd);
        }
        return zftrue;
    }
    virtual void fileFindClose(ZF_IN_OUT ZFFileFindDataContainer &fd)
    {
        _ZFP_ZFFileNativeFd *nativeFd = (_ZFP_ZFFileNativeFd *)fd.nativeFd;

        #if ZF_ENV_sys_Windows
            if(nativeFd->hFind != zfnull)
            {
                ::FindClose(nativeFd->hFind);
            }
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            if(nativeFd->pDir != zfnull)
            {
                ::closedir(nativeFd->pDir);
            }
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown

        zfdelete(nativeFd);
        fd.nativeFd = zfnull;
    }

private:
    static void SetErrPos(ZF_IN zfstring *errPos, ZF_IN const zfchar *s)
    {
        if(errPos != zfnull)
        {
            *errPos = ((s != zfnull) ? s : zfText("<null>"));
        }
    }
    zfbool makeDir(ZF_IN const zfchar *path,
                   ZF_IN_OPT zfstring *errPos)
    {
        if(this->fileIsExist(path))
        {
            if(!this->fileIsFolder(path))
            {
                zfself::SetErrPos(errPos, path);
                return zffalse;
            }
            return zftrue;
        }
        #if ZF_ENV_sys_Windows
            if(!::CreateDirectoryW(ZFStringZ2W(path), zfnull))
            {
                zfself::SetErrPos(errPos, path);
                return zffalse;
            }
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            if(::mkdir(ZFStringZ2A(path), 0777) != 0)
            {
                zfself::SetErrPos(errPos, path);
                return zffalse;
            }
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
        return zftrue;
    }
    zfbool makePath(ZF_IN const zfchar *path,
                    ZF_IN_OPT zfstring *errPos,
                    ZF_IN_OPT zfbool excludeLastLevel = zffalse)
    {
        zfstring pathTmp = path;
        zfindex indexL = zfstringFindReversely(pathTmp, pathTmp.length(), &ZFFile::fileSeparator, 1);
        if(indexL == zfindexMax)
        {
            if(!excludeLastLevel)
            {
                return this->makeDir(pathTmp, errPos);
            }
            return zftrue;
        }
        ZFCoreArray<zfstring> dirToMake;
        if(!excludeLastLevel)
        {
            if(this->fileIsExist(pathTmp))
            {
                if(!this->fileIsFolder(pathTmp))
                {
                    zfself::SetErrPos(errPos, pathTmp);
                    return zffalse;
                }
                return zftrue;
            }
            dirToMake.add(pathTmp);
        }
        pathTmp.remove(indexL);
        do
        {
            if(this->fileIsExist(pathTmp))
            {
                if(!this->fileIsFolder(pathTmp))
                {
                    zfself::SetErrPos(errPos, pathTmp);
                    return zffalse;
                }
                break;
            }
            dirToMake.add(pathTmp);
            if(indexL == zfindexMax)
            {
                break;
            }
            indexL = zfstringFindReversely(pathTmp, pathTmp.length(), &ZFFile::fileSeparator, 1);
            if(indexL != zfindexMax)
            {
                pathTmp.remove(indexL);
            }
        } while(zftrue);
        for(zfindex i = dirToMake.count() - 1; i != zfindexMax; --i)
        {
            if(!this->makeDir(dirToMake[i], errPos))
            {
                return zffalse;
            }
        }
        return zftrue;
    }
    zfbool copyFile(ZF_IN const zfchar *srcPath,
                    ZF_IN const zfchar *dstPath,
                    ZF_IN zfbool isForce,
                    ZF_IN_OPT zfstring *errPos)
    {
        if(isForce)
        {
            this->removeFile(dstPath, isForce, zfnull);
        }
        #if ZF_ENV_sys_Windows
            if(::CopyFileW(ZFStringZ2W(srcPath), ZFStringZ2W(dstPath), !isForce) != TRUE)
            {
                zfself::SetErrPos(errPos, dstPath);
                return zffalse;
            }
            return zftrue;
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
            if(this->fileIsExist(dstPath) && !isForce)
            {
                zfself::SetErrPos(errPos, dstPath);
                return zffalse;
            }

            FILE *fpSrc = fopen(ZFStringZ2A(srcPath), "rb");
            if(fpSrc == zfnull)
            {
                zfself::SetErrPos(errPos, srcPath);
                return zffalse;
            }
            FILE *fpDst = fopen(ZFStringZ2A(dstPath), "wb");
            if(fpDst == zfnull)
            {
                ::fclose(fpSrc);
                zfself::SetErrPos(errPos, dstPath);
                return zffalse;
            }

            size_t readSize = 0;
            #define _copyFile_bufSize 4096
            zfbyte readBuf[_copyFile_bufSize];
            while((readSize = ::fread(readBuf, 1, _copyFile_bufSize, fpSrc)) > 0)
            {
                ::fwrite(readBuf, 1, readSize, fpDst);
            }
            ::fclose(fpSrc);
            ::fclose(fpDst);
            #undef _copyFile_bufSize
            return zftrue;
        #endif // #if ZF_ENV_sys_Windows
    }
    zfbool moveFile(ZF_IN const zfchar *srcPath,
                    ZF_IN const zfchar *dstPath,
                    ZF_IN zfbool isForce,
                    ZF_IN_OPT zfstring *errPos)
    {
        if(isForce)
        {
            this->removeFile(dstPath, isForce, zfnull);
        }
        #if ZF_ENV_sys_Windows
            if(::MoveFileW(ZFStringZ2W(srcPath), ZFStringZ2W(dstPath)) != TRUE)
            {
                zfself::SetErrPos(errPos, dstPath);
                return zffalse;
            }
            return zftrue;
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            if(::rename(ZFStringZ2A(srcPath), ZFStringZ2A(dstPath)) != 0)
            {
                zfself::SetErrPos(errPos, dstPath);
                return zffalse;
            }
            return zftrue;
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    }
    zfbool copyOrMoveFolder(ZF_IN zfbool isCopy,
                            ZF_IN const zfchar *srcPath,
                            ZF_IN const zfchar *dstPath,
                            ZF_IN zfbool isForce,
                            ZF_IN_OPT zfstring *errPos)
    {
        ZFCoreArray<zfstring> stacksFolderSrc;
        ZFCoreArray<zfstring> stacksFolderDst;
        stacksFolderSrc.add(srcPath);
        stacksFolderDst.add(dstPath);

        while(stacksFolderSrc.count() > 0)
        {
            zfstring srcFolder = stacksFolderSrc.getLast();
            zfstring dstFolder = stacksFolderDst.getLast();
            stacksFolderSrc.removeLast();
            stacksFolderDst.removeLast();

            if(!this->makePath(dstFolder, errPos))
            {
                return zffalse;
            }

            ZFFileFindDataContainer fd;
            if(this->fileFindFirst(srcFolder.cString(), fd))
            {
                do
                {
                    zfstring dstTmp;
                    dstTmp += dstFolder;
                    dstTmp += ZFFile::fileSeparator;
                    dstTmp += fd.name.cString();
                    if(this->fileIsFolder(fd.path.cString()))
                    {
                        stacksFolderSrc.add(fd.path.cString());
                        stacksFolderDst.add(dstTmp.cString());
                    }
                    else
                    {
                        if(isCopy)
                        {
                            if(!this->copyFile(fd.path.cString(), dstTmp.cString(), isForce, errPos))
                            {
                                this->fileFindClose(fd);
                                return zffalse;
                            }
                        }
                        else
                        {
                            if(!this->moveFile(fd.path.cString(), dstTmp.cString(), isForce, errPos))
                            {
                                this->fileFindClose(fd);
                                return zffalse;
                            }
                        }
                    }
                } while(this->fileFindNext(fd));
                this->fileFindClose(fd);
            } // if(this->fileFindFirst(srcFolder.cString(), fd))
        } // while(!stacksFolderSrc.empty())

        if(!isCopy)
        {
            if(!this->removeFolder(srcPath, zffalse, errPos))
            {
                return zffalse;
            }
        }

        return zftrue;
    }
    zfbool removeFile(ZF_IN const zfchar *srcPath,
                      ZF_IN zfbool isForce,
                      ZF_IN_OPT zfstring *errPos)
    {
        #if ZF_ENV_sys_Windows
            if(isForce)
            {
                ::SetFileAttributesW(ZFStringZ2W(srcPath), FILE_ATTRIBUTE_NORMAL);
            }
            if(::DeleteFileW(ZFStringZ2W(srcPath)) != TRUE)
            {
                zfself::SetErrPos(errPos, srcPath);
                return zffalse;
            }
            return zftrue;
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
            if(isForce)
            {
                ::chmod(ZFStringZ2A(srcPath), 0777);
            }
            if(::remove(ZFStringZ2A(srcPath)) != 0)
            {
                zfself::SetErrPos(errPos, srcPath);
                return zffalse;
            }
            return zftrue;
        #endif
    }
    zfbool removeFolder(ZF_IN const zfchar *srcPath,
                        ZF_IN zfbool isForce,
                        ZF_IN_OPT zfstring *errPos)
    {
        ZFCoreArray<zfstring> foldersToCheck;
        foldersToCheck.add(srcPath);
        ZFCoreArray<zfstring> emptyFoldersToDel;

        // delete all child files
        while(foldersToCheck.count() > 0)
        {
            zfstring folderPath = foldersToCheck.getLast();
            foldersToCheck.removeLast();
            emptyFoldersToDel.add(folderPath);

            if(isForce)
            {
                #if ZF_ENV_sys_Windows
                    ::SetFileAttributesW(ZFStringZ2W(folderPath.cString()), FILE_ATTRIBUTE_NORMAL);
                #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
                    ::chmod(ZFStringZ2A(folderPath.cString()), 0777);
                #endif
            }

            ZFFileFindDataContainer fd;
            if(this->fileFindFirst(folderPath.cString(), fd))
            {
                do
                {
                    if(this->fileIsFolder(fd.path.cString()))
                    {
                        foldersToCheck.add(fd.path.cString());
                    }
                    else
                    {
                        if(!this->removeFile(fd.path.cString(), isForce, errPos))
                        {
                            return zffalse;
                        }
                    }
                } while(this->fileFindNext(fd));
                this->fileFindClose(fd);
            }
        } // while(!foldersToCheck.empty())

        // delete all empty folder
        while(emptyFoldersToDel.count() > 0)
        {
            zfstring pathTmp = emptyFoldersToDel.getLast();
            emptyFoldersToDel.removeLast();

            #if ZF_ENV_sys_Windows
                if(::RemoveDirectoryW(ZFStringZ2W(pathTmp.cString())) == 0)
                {
                    zfself::SetErrPos(errPos, pathTmp.cString());
                    return zffalse;
                }
            #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
                if(::rmdir(ZFStringZ2A(pathTmp.cString())) != 0)
                {
                    zfself::SetErrPos(errPos, pathTmp.cString());
                    return zffalse;
                }
            #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
        } // while(!stacksFolderToDel.empty())

        return zftrue;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFileImpl_default)

ZF_NAMESPACE_GLOBAL_END

