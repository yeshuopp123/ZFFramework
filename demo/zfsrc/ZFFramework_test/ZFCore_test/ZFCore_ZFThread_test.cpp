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

#if 1
    #define _ZFP_ZFCore_ZFThread_test_outputDetailSteps(format, ...) \
        zfLogTrim(format, ##__VA_ARGS__)
#else
    #define _ZFP_ZFCore_ZFThread_test_outputDetailSteps(format, ...)
#endif

// ============================================================
zfclass ZFCore_ZFThread_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFThread_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFThread)

        zfLogTrim(zfText("============================================================"));
        zfLogTrim(zfText("ZFThread"));
        zfidentity taskId = zfidentityInvalid;

#if 1
        zfLogTrim(zfText("execute main and thread without lock (async):"));
        ZFLISTENER_LOCAL(asyncFunc, {
            zfLogTrim(zfText("async thread begin"));
            for(zfindex i = 0; i < 5; ++i)
            {
                for(zfindex j = 0; j < 10; ++j)
                {
                    _ZFP_ZFCore_ZFThread_test_outputDetailSteps(zfText("  async thread: %zi %zi"), i, j);
                    ZFThread::sleep((zftimet)20);
                }
                ZFThread::sleep((zftimet)300);
            }
            zfLogTrim(zfText("async thread end"));
        })
        taskId = ZFThreadExecuteInNewThread(asyncFunc);
        for(zfindex i = 0; i < 5; ++i)
        {
            for(zfindex j = 0; j < 10; ++j)
            {
                _ZFP_ZFCore_ZFThread_test_outputDetailSteps(zfText("  main: %zi %zi"), i, j);
                ZFThread::sleep((zftimet)20);
            }
            ZFThread::sleep((zftimet)190);
        }
        zfLogTrim(zfText("main thread wait async thread begin"));
        ZFThreadExecuteWait(taskId);
        zfLogTrim(zfText("main thread wait async thread complete"));
#endif

#if 1
        zfLogTrim(zfText("============================================================"));
        zfLogTrim(zfText("execute main and thread with lock (sync):"));
        ZFLISTENER_LOCAL(syncFunc, {
            zfLogTrim(zfText("sync thread begin"));
            for(zfindex i = 0; i < 5; ++i)
            {
                zfsynchronizeLock();
                for(zfindex j = 0; j < 10; ++j)
                {
                    _ZFP_ZFCore_ZFThread_test_outputDetailSteps(zfText("  sync thread: %zi %zi"), i, j);
                    ZFThread::sleep((zftimet)20);
                }
                zfsynchronizeUnlock();
                ZFThread::sleep((zftimet)200);
            }
            zfLogTrim(zfText("sync thread end"));
        })
        taskId = ZFThreadExecuteInNewThread(syncFunc);
        for(zfindex i = 0; i < 5; ++i)
        {
            zfsynchronizeLock();
            for(zfindex j = 0; j < 10; ++j)
            {
                _ZFP_ZFCore_ZFThread_test_outputDetailSteps(zfText("  main:   %zi %zi"), i, j);
                ZFThread::sleep((zftimet)20);
            }
            zfsynchronizeUnlock();
            ZFThread::sleep((zftimet)190);
        }
        zfLogTrim(zfText("main thread wait sync thread begin"));
        ZFThreadExecuteWait(taskId);
        zfLogTrim(zfText("main thread wait sync thread complete"));
#endif

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFThread_test)

ZF_NAMESPACE_GLOBAL_END

