/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFUtility_ZFTestCase_debug_LogEvent)
{
    ZFLISTENER_LOCAL(testCaseOnOutput, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << zfstringWithFormat(zfText("[%s]"), listenerData.sender->classData()->className())
            << listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const zfchar *>();
    })
    this->testCaseOnOutputListener = testCaseOnOutput;
    ZFObjectGlobalEventObserverAdd(ZFTestCase::EventTestCaseOnOutput(), this->testCaseOnOutputListener);

    ZFLISTENER_LOCAL(testCaseOnStart, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << zfstringWithFormat(zfText("[%s]"), listenerData.sender->classData()->className())
            << zfText("========================== start ===========================");
    })
    this->testCaseOnStartListener = testCaseOnStart;
    ZFObjectGlobalEventObserverAdd(ZFTestCase::EventTestCaseOnStart(), this->testCaseOnStartListener);

    ZFLISTENER_LOCAL(testCaseOnProgress, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << zfstringWithFormat(zfText("[%s]"), listenerData.sender->classData()->className())
            << zfText("progress updated");
    })
    this->testCaseOnProgressListener = testCaseOnProgress;
    ZFObjectGlobalEventObserverAdd(ZFTestCase::EventTestCaseOnProgress(), this->testCaseOnProgressListener);

    ZFLISTENER_LOCAL(testCaseOnStop, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << zfstringWithFormat(zfText("[%s]"), listenerData.sender->classData()->className())
            << zfText("-------------------------- stop ----------------------------");
    })
    this->testCaseOnStopListener = testCaseOnStop;
    ZFObjectGlobalEventObserverAdd(ZFTestCase::EventTestCaseOnStop(), this->testCaseOnStopListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUtility_ZFTestCase_debug_LogEvent)
{
    ZFObjectGlobalEventObserverRemove(ZFTestCase::EventTestCaseOnOutput(), this->testCaseOnOutputListener);
    ZFObjectGlobalEventObserverRemove(ZFTestCase::EventTestCaseOnStart(), this->testCaseOnStartListener);
    ZFObjectGlobalEventObserverRemove(ZFTestCase::EventTestCaseOnProgress(), this->testCaseOnProgressListener);
    ZFObjectGlobalEventObserverRemove(ZFTestCase::EventTestCaseOnStop(), this->testCaseOnStopListener);
}
private:
    ZFListener testCaseOnOutputListener;
    ZFListener testCaseOnStartListener;
    ZFListener testCaseOnProgressListener;
    ZFListener testCaseOnStopListener;
ZF_GLOBAL_INITIALIZER_END(ZFUtility_ZFTestCase_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

