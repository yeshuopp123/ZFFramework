/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFLog.h"
#include "ZFTime.h"
#include "ZFMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass _ZFP_I_ZFLogMutex : zfextends ZFMutex
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFLogMutex, ZFMutex)
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLogDataHolder, ZFLevelZFFrameworkHigh)
{
    this->logMutex = zfnull;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLogDataHolder)
{
    if(this->logMutex != zfnull)
    {
        zfReleaseWithoutLeakTest(this->logMutex);
        this->logMutex = zfnull;
    }
}
private:
    ZFMutex *logMutex;
public:
    ZFMutex *getLogMutex(void)
    {
        if(this->logMutex == zfnull)
        {
            this->logMutex = zfAllocWithoutLeakTest(_ZFP_I_ZFLogMutex,
                zfHint("ignore if impl not available")zftrue);
            this->logMutex->objectDebugInfoSet(zfText("ZFLogMutex"));
        }
        return this->logMutex;
    }
    ZFDebugSession globalSession;
ZF_GLOBAL_INITIALIZER_END(ZFLogDataHolder)
#define _ZFP_ZFLogGlobalSession (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLogDataHolder)->globalSession)
#define _ZFP_ZFLogGlobalMutex (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLogDataHolder)->getLogMutex())

ZFMutex *_ZFP_ZFLogMutex(void)
{
    return _ZFP_ZFLogGlobalMutex;
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLogInitDataHolder, ZFLevelZFFrameworkNormal)
{
    this->ZFOutputCallbackDefaultOnChangeListener = ZFCallbackForRawFunction(zfself::ZFOutputCallbackDefaultOnChange);
    ZFGlobalEventCenter::instance()->observerAdd(ZFGlobalEvent::EventZFOutputCallbackDefaultOnChange(), this->ZFOutputCallbackDefaultOnChangeListener);

    zfDebugSessionStart(_ZFP_ZFLogGlobalSession);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLogInitDataHolder)
{
    ZFGlobalEventCenter::instance()->observerRemove(ZFGlobalEvent::EventZFOutputCallbackDefaultOnChange(), this->ZFOutputCallbackDefaultOnChangeListener);
    zfDebugSessionStop(_ZFP_ZFLogGlobalSession);
}
private:
    ZFListener ZFOutputCallbackDefaultOnChangeListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(ZFOutputCallbackDefaultOnChange)
    {
        zfDebugSessionStop(_ZFP_ZFLogGlobalSession);
        zfDebugSessionStart(_ZFP_ZFLogGlobalSession);
    }
ZF_GLOBAL_INITIALIZER_END(ZFLogInitDataHolder)

// ============================================================
zfclassNotPOD _ZFP_ZFLogPrivate
{
public:
    zfindex refCount;
    ZFDebugCallback debugCallback;

public:
    _ZFP_ZFLogPrivate(void)
    : refCount(1)
    , debugCallback()
    {
    }
    _ZFP_ZFLogPrivate(ZF_IN const ZFDebugCallback &debugCallback)
    : refCount(1)
    , debugCallback(debugCallback)
    {
    }
};

// ============================================================
_ZFP_zfLogTAccess::_ZFP_zfLogTAccess(void)
: debugCallback(zfnull)
{
    _ZFP_ZFLogGlobalMutex->mutexLock();
    this->debugCallback = zfnew(ZFDebugCallback, zfDebugSessionT(_ZFP_ZFLogGlobalSession));
}
_ZFP_zfLogTAccess::~_ZFP_zfLogTAccess(void)
{
    // cleanup
    zfdelete(this->debugCallback);
    this->debugCallback = zfnull;

    // restore global settings
    // must after callback's dealloc
    _ZFP_ZFLogGlobalSession.autoSpaceSet(zftrue);
    _ZFP_ZFLogGlobalSession.autoEndlSet(zftrue);

    _ZFP_ZFLogGlobalMutex->mutexUnlock();
}

// ============================================================
// other convenient method
zfstring _ZFP_zfLogCurTimeString(void)
{
    zfstring s;
    ZFTimeInfo ti = ZFTime::currentTimeInfo();
    zfstringAppend(s,
        zfText("%02d:%02d:%02d.%03d"),
        ti.hour,
        ti.minute,
        ti.second,
        ti.miliSecond);
    return s;
}

// ============================================================
void zfLogLevelSet(ZF_IN ZFDebugLevelEnum level)
{
    zfDebugSessionLevelSet(_ZFP_ZFLogGlobalSession, level);
}
ZFDebugLevelEnum zfLogLevelGet(void)
{
    return zfDebugSessionLevelGet(_ZFP_ZFLogGlobalSession);
}
zfbool zfLogIsActive(ZF_IN ZFDebugLevelEnum level)
{
    return zfDebugSessionIsActive(_ZFP_ZFLogGlobalSession, level);
}

ZF_NAMESPACE_GLOBAL_END

