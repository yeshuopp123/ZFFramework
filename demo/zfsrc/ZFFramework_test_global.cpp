/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFFramework_test_global.h"

#include "ZFCore/ZFFile.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFFramework_test_TestCase)

ZFObject *ZFFramework_test_TestCase::objectOnInit(void)
{
    zfsuper::objectOnInit();
    this->_testCaseTmpPath = zfstringWithFormat(zfText("%s%cZFFramework_test%c%s"),
        ZFFile::cachePath(),
        ZFFile::fileSeparator,
        ZFFile::fileSeparator,
        this->classData()->className());
    return this;
}
void ZFFramework_test_TestCase::objectOnDealloc(void)
{
    zfsuper::objectOnDealloc();
}
void ZFFramework_test_TestCase::objectOnDeallocPrepare(void)
{
    ZFFile::fileRemove(this->testCaseTmpPath());
    zfsuper::objectOnDeallocPrepare();
}

void ZFFramework_test_TestCase::testCaseOnStop(ZF_IN ZFResultTypeEnum testCaseResult)
{
    for(zfindex i = 0; i < this->_filesToRemove.count(); ++i)
    {
        ZFFile::fileRemove(this->_filesToRemove[i]);
    }
    zfsuper::testCaseOnStop(testCaseResult);
}

void ZFFramework_test_TestCase::testCaseOutputSeparator(void)
{
    this->testCaseOutput(zfText("----------------------------------------"));
}
const zfchar *ZFFramework_test_TestCase::testCaseTmpPath(void)
{
    return this->_testCaseTmpPath;
}
zfstring ZFFramework_test_TestCase::testCaseUseTmpFile(ZF_IN const zfchar *fileName)
{
    zfstring ret = this->testCaseTmpPath();
    ret += ZFFile::fileSeparator;
    ret += fileName;
    this->testCaseAddFileToRemove(ret.cString());
    return ret;
}
void ZFFramework_test_TestCase::testCaseAddFileToRemove(ZF_IN const zfchar *filePath)
{
    this->_filesToRemove.add(filePath);
}

ZF_NAMESPACE_GLOBAL_END

