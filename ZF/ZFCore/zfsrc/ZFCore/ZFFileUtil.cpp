/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFFileUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFFileUtil)

static void _ZFP_fileList(ZF_IN zfbool resFile,
                          ZF_IN const zfchar *path,
                          ZF_IN const ZFOutputCallback &outputCallback,
                          ZF_IN const zfchar *headToken,
                          ZF_IN const zfchar *indentToken,
                          ZF_IN zfindex indentLevel)
{
    ZFFileFindData fd;
    if(resFile ? ZFFile::resFindFirst(path, fd) : ZFFile::fileFindFirst(path, fd))
    {
        do
        {
            if(headToken != zfnull)
            {
                outputCallback << headToken;
            }
            if(indentToken != zfnull)
            {
                for(zfindex i = 0; i < indentLevel; i++)
                {
                    outputCallback << indentToken;
                }
            }

            if(fd.fileIsFolder())
            {
                outputCallback << fd.fileName() << zfText("/\n");
                _ZFP_fileList(resFile, fd.filePath(), outputCallback, headToken, indentToken, indentLevel + 1);
            }
            else
            {
                outputCallback << fd.fileName();
                outputCallback << zfText("\n");
            }
        } while(resFile ? ZFFile::resFindNext(fd) : ZFFile::fileFindNext(fd));
        resFile ? ZFFile::resFindClose(fd) : ZFFile::fileFindClose(fd);
    }
}

void fileList(ZF_IN_OPT const zfchar *path /* = zfText(".") */,
              ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */,
              ZF_IN_OPT const zfchar *headToken /* = zfnull */,
              ZF_IN_OPT const zfchar *indentToken /* = zfText("  ") */)
{
    _ZFP_fileList(zffalse, path, outputCallback, headToken, indentToken, 0);
}

void resFileList(ZF_IN_OPT const zfchar *path /* = zfText(".") */,
                 ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */,
                 ZF_IN_OPT const zfchar *headToken /* = zfnull */,
                 ZF_IN_OPT const zfchar *indentToken /* = zfText("  ") */)
{
    _ZFP_fileList(zftrue, path, outputCallback, headToken, indentToken, 0);
}

ZF_NAMESPACE_END(ZFFileUtil)
ZF_NAMESPACE_GLOBAL_END

