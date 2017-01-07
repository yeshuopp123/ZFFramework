/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFTextTemplateRun.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFTextTemplateRunParam &_ZFP_ZFTextTemplateRunParamDefault(void)
{
    static ZFTextTemplateRunParam d;
    return d;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTextTemplateRunParamCleaner, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFTextTemplateRunParamCleaner)
{
    ZFTextTemplateRunParam &d = ZFTextTemplateRunParamDefault;
    d.folderNameFilter = ZFFilterForString();
    d.folderContentFilter = ZFFilterForString();
    d.fileNameFilter = ZFFilterForString();
    d.fileContentFilter = ZFFilterForString();
}
ZF_GLOBAL_INITIALIZER_END(ZFTextTemplateRunParamCleaner)

// ============================================================
static zfbool _ZFP_ZFTextTemplateRun_applyName(ZF_IN_OUT zfstring &path,
                                               ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                               ZF_IN const ZFFilterForString &nameFilter,
                                               ZF_OUT zfstring *errorMessage);
static zfbool _ZFP_ZFTextTemplateRun_applyFolder(ZF_IN_OUT zfstring &path,
                                                 ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                                 ZF_IN const ZFTextTemplateRunParam &runParam,
                                                 ZF_OUT zfstring *errorMessage);
static zfbool _ZFP_ZFTextTemplateRun_applyFile(ZF_IN_OUT zfstring &path,
                                               ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                               ZF_IN const ZFTextTemplateRunParam &runParam,
                                               ZF_OUT zfstring *errorMessage);
zfbool ZFTextTemplateRun(ZF_IN const zfchar *path,
                         ZF_IN const ZFTextTemplateParam &textTemplateParam,
                         ZF_IN_OPT const ZFTextTemplateRunParam &runParam /* = ZFTextTemplateRunParam() */,
                         ZF_OUT_OPT zfstring *errorMessage /* = zfnull */)
{
    zfstring pathTmp;
    ZFFile::filePathFormat(pathTmp, path);
    if(!ZFFile::fileIsExist(pathTmp))
    {
        zfstringAppend(errorMessage, zfText("path not exist: \"%s\""), path);
        return zffalse;
    }

    if(ZFFile::fileIsFolder(pathTmp))
    {
        return _ZFP_ZFTextTemplateRun_applyFolder(pathTmp, textTemplateParam, runParam, errorMessage);
    }
    else
    {
        return _ZFP_ZFTextTemplateRun_applyFile(pathTmp, textTemplateParam, runParam, errorMessage);
    }
}

// ============================================================
static zfbool _ZFP_ZFTextTemplateRun_applyName(ZF_IN_OUT zfstring &path,
                                               ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                               ZF_IN const ZFFilterForString &nameFilter,
                                               ZF_OUT zfstring *errorMessage)
{
    if(!nameFilter.filterCheckActive(path))
    {
        return zftrue;
    }

    zfstring fileName = ZFFile::fileNameOf(path);
    zfstring fileNameNew;
    ZFTextTemplateApply(textTemplateParam, ZFOutputCallbackForString(fileNameNew), fileName);
    if(fileName.compare(fileNameNew) == 0)
    {
        return zftrue;
    }
    if(fileNameNew.isEmpty())
    {
        ZFFile::fileRemove(path);
        path.removeAll();
        return zftrue;
    }

    zfstring pathNew = ZFFile::fileParentPathOf(path);
    if(!pathNew.isEmpty())
    {
        pathNew += ZFFile::fileSeparator;
    }
    pathNew += fileNameNew;

    if(!ZFFile::fileMove(path, pathNew))
    {
        zfstringAppend(errorMessage,
            zfText("failed to move from \"%s\" to \"%s\""),
            path.cString(), pathNew.cString());
        return zffalse;
    }

    path = pathNew;
    return zftrue;
}
static zfbool _ZFP_ZFTextTemplateRun_applyFolder(ZF_IN_OUT zfstring &path,
                                                 ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                                 ZF_IN const ZFTextTemplateRunParam &runParam,
                                                 ZF_OUT zfstring *errorMessage)
{
    if(!_ZFP_ZFTextTemplateRun_applyName(path, textTemplateParam, runParam.folderNameFilter, errorMessage))
    {
        return zffalse;
    }
    if(path.isEmpty())
    {
        return zftrue;
    }
    if(!runParam.folderContentFilter.filterCheckActive(path))
    {
        return zftrue;
    }

    zfbool ret = zftrue;
    ZFFileFindData fd;
    if(ZFFile::fileFindFirst(path, fd))
    {
        do
        {
            if(fd.fileIsFolder())
            {
                zfstring folderPath = fd.filePath();
                if(!_ZFP_ZFTextTemplateRun_applyFolder(folderPath, textTemplateParam, runParam, errorMessage))
                {
                    ret = zffalse;
                    break;
                }
            }
            else
            {
                zfstring filePath = fd.filePath();
                if(!_ZFP_ZFTextTemplateRun_applyFile(filePath, textTemplateParam, runParam, errorMessage))
                {
                    ret = zffalse;
                    break;
                }
            }
        } while(ZFFile::fileFindNext(fd));
        ZFFile::fileFindClose(fd);
    }
    return ret;
}
static zfbool _ZFP_ZFTextTemplateRun_applyFile(ZF_IN_OUT zfstring &path,
                                               ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                               ZF_IN const ZFTextTemplateRunParam &runParam,
                                               ZF_OUT zfstring *errorMessage)
{
    if(!_ZFP_ZFTextTemplateRun_applyName(path, textTemplateParam, runParam.fileNameFilter, errorMessage))
    {
        return zffalse;
    }
    if(path.isEmpty())
    {
        return zftrue;
    }
    if(!runParam.fileContentFilter.filterCheckActive(path))
    {
        return zftrue;
    }

    // read entire file to buffer
    zfchar *buf = zfnull;
    zfchar *bufEnd = zfnull;
    {
        ZFFileToken token = ZFFile::fileOpen(path, ZFFileOpenOption::e_Read);
        if(token == ZFFileTokenInvalid)
        {
            zfstringAppend(errorMessage, zfText("failed to open file %s"), path.cString());
            return zffalse;
        }
        ZFFileBlockedClose(token);

        zfindex fileSize = ZFFile::fileSize(token);
        if(fileSize == 0)
        {
            return zftrue;
        }

        buf = (zfchar *)zfmalloc(fileSize);
        if(buf == zfnull)
        {
            zfstringAppend(errorMessage, zfText("failed to malloc buffer for size %zi"), fileSize);
            return zffalse;
        }

        if(ZFFile::fileRead(token, buf, fileSize) != fileSize)
        {
            zfstringAppend(errorMessage, zfText("failed to read file %s"), path.cString());
            zffree(buf);
            return zffalse;
        }
        bufEnd = buf + fileSize;
    }
    zfblockedFree(buf);

    if(ZFTextTemplateApply(textTemplateParam, ZFOutputCallbackForFile(path), buf, bufEnd - buf) == zfindexMax)
    {
        zfstringAppend(errorMessage, zfText("failed to update template for %s"), path.cString());
        return zffalse;
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

