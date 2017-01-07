/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFMd5_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFMd5_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        const zfchar *testString = zfText("123abc");
        zfstring testValue = zfText("A906449D5769FA7361D7ECC6AA3F6D28"); // testString's MD5 to verify
        zfstring value;

        value = zfMd5Calc((const zfbyte *)testString, zfslen(testString) * sizeof(zfchar));
        this->testCaseOutput(zfText("MD5 of array \"%s\": %s"), testString, value.cString());
        ZFTestCaseAssert(value == testValue);

        zfstring tmpFilePath = this->testCaseUseTmpFile(zfText("ZFMd5_Md5.txt"));
        ZFFileToken fp = ZFFile::fileOpen(tmpFilePath, ZFFileOpenOption::e_Write);
        if(fp != ZFFileTokenInvalid)
        {
            ZFFile::fileWrite(fp, testString);
            ZFFile::fileClose(fp);
            fp = ZFFileTokenInvalid;
        }
        this->testCaseOutput(zfText("write it to file %s, file's MD5: %s"),
                tmpFilePath.cString(),
                zfMd5Calc(ZFInputCallbackForFile(tmpFilePath)).cString());

        this->testCaseOutputSeparator();
        tmpFilePath = this->testCaseUseTmpFile(zfText("ZFMd5_Md5_big.txt"));
        fp = ZFFile::fileOpen(tmpFilePath, ZFFileOpenOption::e_Write);
        zfindex fileSize = 0;
        if(fp != zfnull)
        {
            for(zfindex i = 0; i < 1000; i++)
            {
                for(zfindex j = 0; j < 1000; j++)
                {
                    ZFFile::fileWrite(fp, testString);
                }
            }
            fileSize = ZFFile::fileTell(fp);
            ZFFile::fileClose(fp);
            fp = zfnull;
        }
        ZFTimeValue tv1 = ZFTime::currentTimeValue();
        zfstring MD5BigFile = zfMd5Calc(ZFInputCallbackForFile(tmpFilePath.cString()));
        ZFTimeValue tv2 = ZFTimeValueDec(ZFTime::currentTimeValue(), tv1);
        this->testCaseOutput(zfText("write it 1000*1000 times to file %s, file's size: %zi, MD5: %s, time: %s.%03s %03s"),
            tmpFilePath.cString(),
            fileSize,
            MD5BigFile.cString(),
            zfsFromInt(tv2.sec).cString(),
            zfsFromInt(tv2.usec / 1000).cString(),
            zfsFromInt(tv2.usec % 1000).cString());

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFMd5_test)

ZF_NAMESPACE_GLOBAL_END

