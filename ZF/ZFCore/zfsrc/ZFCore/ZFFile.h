/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFFile.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_h_
#define _ZFI_ZFFile_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFFileFindDataPrivate;
/**
 * @brief data used by ZFFile when finding files
 * @see ZFFile::fileFindFirst, ZFFile::fileFindNext, ZFFile::fileFindClose
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFFileFindData
{
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFFileFindData)
public:
    ZFFileFindData(void);
    ~ZFFileFindData(void);

public:
    /**
     * @brief return parent path
     *
     * no file separator will be added to tail
     */
    const zfchar *fileParentPath(void) const;
    /**
     * @brief return path of file
     */
    const zfchar *filePath(void) const;
    /**
     * @brief return file name of file
     */
    const zfchar *fileName(void) const;
    /**
     * @brief return true if is a directory
     */
    zfbool fileIsFolder(void) const;

public:
    /**
     * @brief get a short info
     */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #objectInfoT */
    inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

private:
    _ZFP_ZFFileFindDataPrivate *d;
    friend zfclass ZFFile;
};

ZFOUTPUT_TYPE(ZFFileFindData, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFFileFindData *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFFileFindData *, {output << (const ZFFileFindData *)v;})

// ============================================================
/**
 * @brief open falgs, similar to fopen's flags,
 *   while we have binary mode only
 *
 * usage:
 * -  ZFFileOpenOptionCreate\n
 *   create a new file for write only
 * -  ZFFileOpenOptionCreate | ZFFileOpenOption::e_Read\n
 *   create a new file for read/write
 * -  ZFFileOpenOption::e_Read\n
 *   open a existing file for read only, fail if file not exist
 * -  ZFFileOpenOption::e_Read | ZFFileOpenOptionWrite\n
 *   open a existing file for read/write, fail if file not exist
 * -  ZFFileOpenOptionWrite\n
 *   same as ZFFileOpenOptionCreate, create a new file for write only
 * -  ZFFileOpenOptionAppend\n
 *   open a file for append or create new if not exist, for write only
 * -  ZFFileOpenOptionAppend | ZFFileOpenOption::e_Read\n
 *   open a file for append or create new if not exist, for read/write
 */
ZFENUM_BEGIN(ZFFileOpenOption)
    ZFENUM_VALUE_WITH_INIT(Create, 1) /**< @brief create for read write */
    ZFENUM_VALUE_WITH_INIT(Read, 2) /**< @brief readonly */
    ZFENUM_VALUE_WITH_INIT(Write, 4) /**< @brief write only */
    ZFENUM_VALUE_WITH_INIT(Append, 8) /**< @brief append only */
ZFENUM_SEPARATOR(ZFFileOpenOption)
    ZFENUM_VALUE_REGISTER(Create)
    ZFENUM_VALUE_REGISTER(Read)
    ZFENUM_VALUE_REGISTER(Write)
    ZFENUM_VALUE_REGISTER(Append)
ZFENUM_END(ZFFileOpenOption)

ZFENUM_FLAGS_DECLARE(ZFFileOpenOption, ZFFileOpenOptionFlags)

/**
 * @brief similar to FILE in C world, see #ZFFile::fileOpen
 *
 * ensured void *, for implementations to store datas
 */
typedef void * ZFFileToken;
/**
 * @brief invalid token
 */
#define ZFFileTokenInvalid zfnull

/**
 * @brief file utility
 */
zfclass ZF_ENV_EXPORT ZFFile : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFFile, ZFObject)

    // ============================================================
    // common
public:
    /**
     * @brief file separator (ensured to be '/' for now)
     */
    static const zfchar fileSeparator;
    /**
     * @brief string version of #ZFFile::fileSeparator
     */
    static const zfchar *fileSeparatorString;

public:
    /**
     * @brief format path according ZFFile::fileSeparator
     *
     * it's ensured that no separator would be added to tail
     * (so if src is "/", result is just a empty string)\n
     * result would be appended to ret instead of clear and set\n
     * "\"" would be converted to single '"'\n
     * "\ " would be converted to single space ' '\n
     * null src is treated as error,
     * while empty string is not
     */
    static zfbool filePathFormat(ZF_OUT zfstring &ret,
                                 ZF_IN const zfchar *src);

    /**
     * @brief get file name from path or empty if error,
     *   e.g. "file.ext" from "/path/file.ext"
     *
     * path should be well formed, use #filePathFormat if necessary
     */
    static void fileNameOf(ZF_OUT zfstring &ret, ZF_IN const zfchar *src);
    /** @brief see #fileNameOf */
    static inline zfstring fileNameOf(ZF_IN const zfchar *src)
    {
        zfstring ret;
        ZFFile::fileNameOf(ret, src);
        return ret;
    }
    /**
     * @brief get file name without extension from path or empty if error,
     *   e.g. "file.ext0" from "/path/file.ext0.ext1"
     *
     * path should be well formed, use #filePathFormat if necessary
     */
    static void fileNameOfWithoutExt(ZF_OUT zfstring &ret, ZF_IN const zfchar *src);
    /** @brief see #fileNameOfWithoutExt */
    static inline zfstring fileNameOfWithoutExt(ZF_IN const zfchar *src)
    {
        zfstring ret;
        ZFFile::fileNameOfWithoutExt(ret, src);
        return ret;
    }
    /**
     * @brief get file extension from path or empty if error,
     *   e.g. "ext1" from "/path/file.ext0.ext1",
     *   or empty if no extension
     *
     * path should be well formed, use #filePathFormat if necessary
     */
    static void fileExtOf(ZF_OUT zfstring &ret, ZF_IN const zfchar *src);
    /** @brief see #fileExtOf */
    static inline zfstring fileExtOf(ZF_IN const zfchar *src)
    {
        zfstring ret;
        ZFFile::fileExtOf(ret, src);
        return ret;
    }
    /**
     * @brief get parent path
     *
     * return empty string if no parent found\n
     * path should be well formed, use #filePathFormat if necessary
     */
    static void fileParentPathOf(ZF_OUT zfstring &ret, ZF_IN const zfchar *src);
    /** @brief see #fileParentPathOf */
    static inline zfstring fileParentPathOf(ZF_IN const zfchar *src)
    {
        zfstring ret;
        ZFFile::fileParentPathOf(ret, src);
        return ret;
    }
    /**
     * @brief get each file components from path or empty if error,
     *   e.g. {"path", "file.ext"}(as array) from "/path/file.ext"
     *
     * path should be well formed, use #filePathFormat if necessary
     */
    static void filePathComponentsOf(ZF_OUT ZFCoreArray<zfstring> &ret, ZF_IN const zfchar *src);
    /** @brief see #filePathComponentsOf */
    static inline ZFCoreArray<zfstring> filePathComponentsOf(ZF_IN const zfchar *src)
    {
        ZFCoreArray<zfstring> ret;
        ZFFile::filePathComponentsOf(ret, src);
        return ret;
    }

    /**
     * @brief return true if file specified by path is exist
     */
    static zfbool fileIsExist(ZF_IN const zfchar *path);
    /**
     * @brief return true if file specified by path is a directory
     */
    static zfbool fileIsFolder(ZF_IN const zfchar *path);

    /**
     * @brief make directory
     */
    static zfbool filePathCreate(ZF_IN const zfchar *path,
                                 ZF_IN_OPT zfbool autoMakeParent = zftrue,
                                 ZF_IN_OPT zfstring *errPos = zfnull);

    /**
     * @brief copy a file or directory from srcPath to dstPath
     *
     * if both src and dst exist, but one is file and another is folder,
     * return zffalse\n
     * merge directory if dst is an existing folder
     * (if isForce not set,
     * would return false if dst has a child file with the same path in src)\n
     * override file if dst is an existing file and isForce is zftrue
     */
    static zfbool fileCopy(ZF_IN const zfchar *srcPath,
                           ZF_IN const zfchar *dstPath,
                           ZF_IN_OPT zfbool isRecursive = zftrue,
                           ZF_IN_OPT zfbool isForce = zffalse,
                           ZF_IN_OPT zfstring *errPos = zfnull);
    /**
     * @brief move a file or directory from srcPath to dstPath, similar to fileCopy
     */
    static zfbool fileMove(ZF_IN const zfchar *srcPath,
                           ZF_IN const zfchar *dstPath,
                           ZF_IN_OPT zfbool isRecursive = zftrue,
                           ZF_IN_OPT zfbool isForce = zffalse,
                           ZF_IN_OPT zfstring *errPos = zfnull);
    /**
     * @brief delete a file or directory from srcPath to dstPath
     *
     * fail if isRecursive is zffalse and dst is a folder
     */
    static zfbool fileRemove(ZF_IN const zfchar *path,
                             ZF_IN_OPT zfbool isRecursive = zftrue,
                             ZF_IN_OPT zfbool isForce = zffalse,
                             ZF_IN_OPT zfstring *errPos = zfnull);

    /**
     * @brief find file or directory, similar to FindFirstFile under Windows
     *
     * path supports path only, without wildcard support,
     * e.g. "/path/" or "/path",
     * "." and ".." won't be included\n
     * typical usage:
     * @code
     *   ZFFileFindData fd;
     *   if(ZFFile::fileFindFirst(path, fd))
     *   {
     *     do
     *     {
     *       // do something
     *     } while(ZFFile::fileFindNext(fd));
     *     ZFFile::fileFindClose(fd);
     *   }
     * @endcode
     * @return false if error or nothing found
     * @warning strings in #ZFFileFindData would be deleted
     *   after each findXXX call,
     *   you must save it if need future use
     * @see ZFFileFindData, fileFindFirst, fileFindNext, fileFindClose
     */
    static zfbool fileFindFirst(ZF_IN const zfchar *path,
                                ZF_IN_OUT ZFFileFindData &fd);

    /**
     * @see ZFFileFindData, fileFindFirst, fileFindClose
     */
    static zfbool fileFindNext(ZF_IN_OUT ZFFileFindData &fd);

    /**
     * @see ZFFileFindData, fileFindFirst, fileFindNext
     */
    static void fileFindClose(ZF_IN_OUT ZFFileFindData &fd);

    // ============================================================
    // file path
public:
    /**
     * @brief get module's parent's path, e.g. "/path"
     *
     * path is ensured to use ZFFile::fileSeparator as separator,
     * and no extra separator would be added to tail
     */
    static const zfchar *modulePath(void);
    /**
     * @brief get module's path, e.g. "/path/module.exe"
     *
     * path is ensured to use ZFFile::fileSeparator as separator\n
     * it's not ensured module path is actually executable's path,
     * use other path types for data storage such as settingPath
     */
    static const zfchar *moduleFilePath(void);

    /**
     * @brief get a proper data path that app can save settings to
     *
     * path is ensured to use ZFFile::fileSeparator as separator,
     * and no extra separator would be added to tail
     */
    static const zfchar *settingPath(void);
    /**
     * @brief change the data path, null to use defalut path
     */
    static void settingPathSet(ZF_IN const zfchar *path = zfnull);
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified after change, param0 is the old path value
     * (as #ZFPointerHolder of const zfchar *)\n
     * notified to #ZFGlobalEventCenter only
     */
    ZFOBSERVER_EVENT(SettingPathOnChange)

    /**
     * @brief get a proper storage path that app can write files to
     *
     * path is ensured to use ZFFile::fileSeparator as separator,
     * and no extra separator would be added to tail
     */
    static const zfchar *storagePath(void);
    /**
     * @brief change the storage path, null to use defalut path
     */
    static void storagePathSet(ZF_IN const zfchar *path = zfnull);
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified after change, param0 is the old path value
     * (as #ZFPointerHolder of const zfchar *)\n
     * notified to #ZFGlobalEventCenter only
     */
    ZFOBSERVER_EVENT(StoragePathOnChange)

    /**
     * @brief get a proper shared storage path that app can write files to,
     *   and other app can access
     *
     * note, this path may or may not be writable,
     * maybe external storage path or user's home path,
     * write with causion and only if necessary\n
     * path is ensured to use ZFFile::fileSeparator as separator,
     * and no extra separator would be added to tail
     */
    static const zfchar *storageSharedPath(void);
    /**
     * @brief change the storage path, null to use defalut path
     */
    static void storageSharedPathSet(ZF_IN const zfchar *path = zfnull);
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified after change, param0 is the old path value
     * (as #ZFPointerHolder of const zfchar *)\n
     * notified to #ZFGlobalEventCenter only
     */
    ZFOBSERVER_EVENT(StorageSharedPathOnChange)

    /**
     * @brief get a proper cache path that app can write files to
     *
     * cache is used for temp files only,
     * may or may not be deleted automatically\n
     * path is ensured to use ZFFile::fileSeparator as separator,
     * and no extra separator would be added to tail
     */
    static const zfchar *cachePath(void);
    /**
     * @brief change the cache path, null to use defalut path
     */
    static void cachePathSet(ZF_IN const zfchar *path = zfnull);
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified after change, param0 is the old path value
     * (as #ZFPointerHolder of const zfchar *)\n
     * notified to #ZFGlobalEventCenter only
     */
    ZFOBSERVER_EVENT(CachePathOnChange)
    /**
     * @brief clear all caches under #cachePath
     *
     * note, this method is ensured called during #ZFFrameworkCleanup
     * as level #ZFLevelAppNormal
     */
    static void cachePathClear(void);
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified to #ZFGlobalEventCenter only
     */
    ZFOBSERVER_EVENT(CachePathBeforeClear)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified to #ZFGlobalEventCenter only
     */
    ZFOBSERVER_EVENT(CachePathAfterClear)

    // ============================================================
    // file read write
public:
    /**
     * @brief open a file for read or write
     */
    static ZFFileToken fileOpen(ZF_IN const zfchar *filePath,
                                ZF_IN_OPT ZFFileOpenOptionFlags flag = ZFFileOpenOption::e_Read,
                                ZF_IN_OPT zfbool autoCreateParent = zftrue);
    /**
     * @brief close and save the file if need, return false if save failed
     */
    static zfbool fileClose(ZF_IN ZFFileToken token);

    /**
     * @brief get current file's position or zfindexMax if error
     */
    static zfindex fileTell(ZF_IN ZFFileToken token);
    /**
     * @brief similar to fileSeek, return false if seek out of range
     */
    static zfbool fileSeek(ZF_IN ZFFileToken token,
                           ZF_IN zfindex byteSize,
                           ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin);

    /**
     * @brief read file
     *
     * return size read, even if error occurred
     * (whether error occurred or eof,
     * could be accessed by #ZFFile::fileError and #ZFFile::fileEof)\n
     * typical usage:
     * @code
     *   zfindex sizeRead = 0;
     *   while((sizeRead = ZFFile::fileRead(token, buf)) > 0
     *       && !ZFFile::fileError(token))
     *   {
     *       // do something with buf
     *   }
     *   if(ZFFile::fileError(token))
     *   {
     *       // exit because of error occurred, read fail
     *   }
     *   else if(ZFFile::fileEof(token))
     *   {
     *       // exit because of eof, read success
     *   }
     * @endcode
     *
     * note that the tail '\0' won't be written to buf
     * @warning ZFFile always read files in binary mode,
     *   for text files,
     *   you must make sure the file is UTF8 encoded without BOM,
     *   otherwise, you must process the file's encoding and BOM manually
     */
    static zfindex fileRead(ZF_IN ZFFileToken token,
                            ZF_IN void *buf,
                            ZF_IN zfindex maxByteSize);

    /**
     * @brief write file, see #ZFFile::fileRead
     *
     * return size written, even if error occurred
     * (whether error occurred or not,
     * could be accessed by #ZFFile::fileError)\n
     * typical usage:
     * @code
     *   zfindex sizeWritten = ZFFile::fileWrite(token, src, size);
     *   if(ZFFile::fileError(token))
     *   {
     *       // write fail
     *   }
     *   if(sizeWritten != size)
     *   { // you can also check by sizeWritten
     *       // write fail
     *   }
     * @endcode
     * \n
     * maxByteSize could be zfindexMax,
     * which means fileWrite should be stopped when reached 0x00 in src,
     * usually to output a UTF8 string
     */
    static zfindex fileWrite(ZF_IN ZFFileToken token,
                             ZF_IN const void *src,
                             ZF_IN_OPT zfindex maxByteSize = zfindexMax);
    /**
     * @brief flush the file, useful only for files opened for write
     */
    static void fileFlush(ZF_IN ZFFileToken token);
    /**
     * @brief see #ZFFile::fileRead
     */
    static zfbool fileEof(ZF_IN ZFFileToken token);
    /**
     * @brief see #ZFFile::fileRead
     */
    static zfbool fileError(ZF_IN ZFFileToken token);

public:
    /**
     * @brief util method to get file size
     *
     * fileSeek to end, fileTell, then fileSeek to restore,
     * return zfindexMax if error\n
     * note that result is not ensured if file is opened in append mode
     */
    static zfindex fileSize(ZF_IN ZFFileToken token);

    // ============================================================
    // resource files
public:
    /**
     * @brief add an additional path to search as resource files
     *
     * by default, resource files are placed in a fixed builtin location,
     * you may add additional path to search as resource files\n
     * to make things clear, while accessing resource files using a res path,
     * we would:
     * -# search all additional resource path to find whether the file exists
     * -# if exist, use the additional resource file
     * -# if not exist, use builtin resource file
     * -# if builtin resource file not exist, fail
     *
     *
     * this method would affect all methods that process with resource files,
     * such as #ZFFile::resCopy, #ZFFile::resOpen, #ZFFile::resRead\n
     * typically, additional resource path should be modified during startup only
     */
    static void resAdditionalPathAdd(ZF_IN const zfchar *path);
    /** @brief see #resAdditionalPathAdd */
    static void resAdditionalPathRemove(ZF_IN const zfchar *path);
    /** @brief see #resAdditionalPathAdd */
    static ZFCoreArray<zfstring> resAdditionalPathList(void);
    /**
     * @brief see #resAdditionalPathAdd
     *
     * return the registered additional resource path if file exist,
     * or null if not exist\n
     * if return value is not null,
     * the final file path should be "return value + '/' + resPath"
     */
    static const zfchar *resAdditionalPathCheck(ZF_IN const zfchar *resPath);

public:
    /**
     * @brief res version of #fileCopy
     */
    static zfbool resCopy(ZF_IN const zfchar *resPath,
                          ZF_IN const zfchar *dstPath,
                          ZF_IN_OPT zfbool isRecursive = zftrue,
                          ZF_IN_OPT zfbool isForce = zffalse,
                          ZF_IN_OPT zfstring *errPos = zfnull);

    /**
     * @brief open a resource file for read only, see #fileOpen
     *
     * since the resource files may or may not be opened by normal method
     * (i.e. fileOpen), such as those in Android project which are located in
     * apk/assets, so use #resOpen and #resFindFirst to access it for cross-platform\n
     * also, since the resource files may or may not be compressed into app,
     * accessing them may or may not take much time
     * \n
     * a resource file is a file in "zfres" directory of native project,
     * such as "project_path/zfres/1.png" and "project_path/zfres/subdir/1.png",
     * which should be "1.png" and "subdir/1.png" when resOpen\n
     * file name could be the same if under different directories,
     * and is recommended to keep all lower case for cross-platform\n
     * \n
     * note that the token is defined same type as the one used by #ZFFile::fileOpen,
     * it's your responsibility to make sure not to misuse it
     */
    static ZFFileToken resOpen(ZF_IN const zfchar *resPath);
    /**
     * @brief see #resOpen #fileClose
     */
    static zfbool resClose(ZF_IN ZFFileToken token);

    /**
     * @brief see #resOpen #fileTell
     */
    static zfindex resTell(ZF_IN ZFFileToken token);
    /**
     * @brief see #resOpen #fileSeek
     */
    static zfbool resSeek(ZF_IN ZFFileToken token,
                          ZF_IN zfindex byteSize,
                          ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin);

    /**
     * @brief see #resOpen #fileRead
     */
    static zfindex resRead(ZF_IN ZFFileToken token,
                           ZF_IN void *buf,
                           ZF_IN zfindex maxByteSize);

    /**
     * @brief see #resOpen #fileEof
     */
    static zfbool resEof(ZF_IN ZFFileToken token);
    /**
     * @brief see #resOpen #fileEof
     */
    static zfbool resError(ZF_IN ZFFileToken token);

    /**
     * @see see #resOpen, #fileFindFirst
     *
     * use empty string to find resource's root directory
     */
    static zfbool resFindFirst(ZF_IN const zfchar *resPath,
                               ZF_IN_OUT ZFFileFindData &fd);
    /**
     * @see see #resOpen, #fileFindNext
     */
    static zfbool resFindNext(ZF_IN_OUT ZFFileFindData &fd);
    /**
     * @see see #resOpen, #fileFindClose
     */
    static void resFindClose(ZF_IN_OUT ZFFileFindData &fd);

public:
    /**
     * @brief see #resOpen, #fileSize
     */
    static zfindex resSize(ZF_IN ZFFileToken token);
};

// ============================================================
/**
 * @brief see #ZFFileBlockedClose
 */
zfclassLikePOD ZF_ENV_EXPORT ZFFileBlockedCloseHolder
{
public:
    /** @cond ZFPrivateDoc */
    ZFFileBlockedCloseHolder(ZF_IN ZFFileToken token)
    : token(token)
    {
    }
    ~ZFFileBlockedCloseHolder(void)
    {
        if(this->token != ZFFileTokenInvalid)
        {
            ZFFile::fileClose(this->token);
        }
    }
    /** @endcond */
private:
    ZFFileToken token;
};
/**
 * @brief util method to call #ZFFile::fileClose after code block
 */
#define ZFFileBlockedClose(token) ZFFileBlockedCloseHolder ZFUniqueName(ZFFileBlockedClose)(token)

// ============================================================
/**
 * @brief see #ZFFileBlockedResClose
 */
zfclassLikePOD ZF_ENV_EXPORT ZFFileBlockedResCloseHolder
{
public:
    /** @cond ZFPrivateDoc */
    ZFFileBlockedResCloseHolder(ZF_IN ZFFileToken token)
    : token(token)
    {
    }
    ~ZFFileBlockedResCloseHolder(void)
    {
        if(this->token != ZFFileTokenInvalid)
        {
            ZFFile::resClose(this->token);
        }
    }
    /** @endcond */
private:
    ZFFileToken token;
};
/**
 * @brief util method to call #ZFFile::fileClose after code block
 */
#define ZFFileBlockedResClose(token) ZFFileBlockedResCloseHolder ZFUniqueName(ZFFileBlockedResClose)(token)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_h_

#include "ZFFileCallbackDef.h"

