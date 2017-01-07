/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFFilePath_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFFilePath_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutput(zfText("modulePath:"));
        this->testCaseOutput(ZFFile::modulePath());

        this->testCaseOutput(zfText("moduleFilePath:"));
        this->testCaseOutput(ZFFile::moduleFilePath());

        this->testCaseOutput(zfText("settingPath:"));
        this->testCaseOutput(ZFFile::settingPath());

        this->testCaseOutput(zfText("storagePath:"));
        this->testCaseOutput(ZFFile::storagePath());

        this->testCaseOutput(zfText("storageSharedPath:"));
        this->testCaseOutput(ZFFile::storageSharedPath());

        this->testCaseOutput(zfText("cachePath:"));
        this->testCaseOutput(ZFFile::cachePath());

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFFilePath_test)

ZF_NAMESPACE_GLOBAL_END

