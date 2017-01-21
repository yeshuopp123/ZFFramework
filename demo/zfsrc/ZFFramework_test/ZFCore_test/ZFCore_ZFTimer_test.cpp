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

static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFCore_ZFTimer_test_timerEvent)
{
    ZFTimer *timer = ZFCastZFObject(ZFTimer *, listenerData.sender);
    zfLogTrim(zfText("timer event, current thread: %s"), ZFThread::currentThread()->objectInfo().cString());
    if(timer->timerActivatedCount() >= 3)
    {
        timer->timerStop();

        ZFLISTENER_LOCAL(action, {
            userData->to<ZFTestCase *>()->testCaseStop();
        })
        ZFThreadExecuteInMainThread(action, userData);
    }
}
zfclass ZFCore_ZFTimer_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFTimer_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFTimer)

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFTimer"));
        this->testCaseOutput(zfText("current thread: %s"), ZFThread::currentThread()->objectInfo().cString());

        zfblockedAlloc(ZFTimer, timer);
#if 0
        timer->timerActivateInMainThreadSet(zftrue);
#endif
        timer->observerAdd(ZFTimer::EventTimerOnActivate(), ZFCallbackForRawFunction(_ZFP_ZFCore_ZFTimer_test_timerEvent), this);
        timer->timerIntervalSet((zftimet)300);
        timer->timerStart();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFTimer_test)

ZF_NAMESPACE_GLOBAL_END

