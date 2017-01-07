/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZFFramework_test_global_h_
#define _ZFI_ZFFramework_test_global_h_

#include "ZFUtility/ZFTestCase.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfabstract ZFFramework_test_TestCase : zfextends ZFTestCase
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFFramework_test_TestCase, ZFTestCase)

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);
protected:
    zfoverride
    virtual void testCaseOnStop(ZF_IN ZFResultTypeEnum testCaseResult);

public:
    virtual void testCaseOutputSeparator(void);
    virtual const zfchar *testCaseTmpPath(void);
    virtual zfstring testCaseUseTmpFile(ZF_IN const zfchar *fileName);
    virtual void testCaseAddFileToRemove(ZF_IN const zfchar *filePath);

private:
    ZFCoreArray<zfstring> _filesToRemove;
    zfstring _testCaseTmpPath;
};

#define ZFFramework_test_protocolCheck(ProtocolName) \
    if(!ZFProtocolIsAvailable(#ProtocolName)) \
    { \
        this->testCaseOutput(#ProtocolName zfText(" not available, skip test case")); \
        this->testCaseStop(); \
        return ; \
    }

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFFramework_test_global_h_

