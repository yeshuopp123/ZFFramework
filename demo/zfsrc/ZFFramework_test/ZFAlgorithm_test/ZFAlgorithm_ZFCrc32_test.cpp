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

zfclass ZFAlgorithm_ZFCrc32_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFCrc32_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        const zfchar *testString = zfText("123abc");
        zfuint32 testValue = 0x8DFE51D2; // testString's CRC32 to verify
        zfuint32 value = 0;

        value = zfCrc32Calc((const zfbyte *)testString, zfslen(testString) * sizeof(zfchar));
        this->testCaseOutput(zfText("CRC32 of array \"%s\": %x"), testString, (zfuint)value);
        ZFTestCaseAssert(value == testValue);

        zfstring tmpFilePath = this->testCaseUseTmpFile(zfText("ZFCrc32_Crc32.txt"));
        ZFFileToken fp = ZFFile::fileOpen(tmpFilePath, ZFFileOpenOption::e_Write);
        if(fp != ZFFileTokenInvalid)
        {
            ZFFile::fileWrite(fp, testString);
            ZFFile::fileClose(fp);
            fp = ZFFileTokenInvalid;
        }
        this->testCaseOutput(zfText("write it to file %s, file's CRC32: %x"),
                tmpFilePath.cString(),
                (zfuint)zfCrc32Calc(ZFInputCallbackForFile(tmpFilePath)));

        this->testCaseOutputSeparator();
        tmpFilePath = this->testCaseUseTmpFile(zfText("ZFCrc32_Crc32_big.txt"));
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
        zfuint32 CRC32BigFile = (zfuint32)zfCrc32Calc(ZFInputCallbackForFile(tmpFilePath.cString()));
        ZFTimeValue tv2 = ZFTimeValueDec(ZFTime::currentTimeValue(), tv1);
        this->testCaseOutput(zfText("write it 1000*1000 times to file %s, file's size: %zi, CRC32: %X, time: %s.%03s %03s"),
            tmpFilePath.cString(),
            fileSize,
            CRC32BigFile,
            zfsFromInt(tv2.sec).cString(),
            zfsFromInt(tv2.usec / 1000).cString(),
            zfsFromInt(tv2.usec % 1000).cString());

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFCrc32_test)

ZF_NAMESPACE_GLOBAL_END

