/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFDebug.h"
#include "ZFString.h"
#include "protocol/ZFProtocolZFOutput.h"
#include <stdio.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFDebugLevel)

// ============================================================
static zfindex _ZFP_ZFDebugWrappedZFCoreLog(ZF_IN const zfcharA *s)
{
    if(ZFPROTOCOL_TRY_ACCESS(ZFOutput) != zfnull)
    {
        return ZFPROTOCOL_ACCESS(ZFOutput)->outputCoreLog(s);
    }
    else
    {
        // try to print to std output
        fprintf(stderr, zfTextA("%s"), s);
    }
    return zfindexMax;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFDebugDataHolder, ZFLevelZFFrameworkNormal)
{
    zfCoreLogOutputCallbackSet(_ZFP_ZFDebugWrappedZFCoreLog);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFDebugDataHolder)
{
}
public:
    ZFDebugSession globalSession;
    ZFDebugSession globalNullSession;
ZF_GLOBAL_INITIALIZER_END(ZFDebugDataHolder)
#define _ZFP_ZFDebugGlobalSession (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFDebugDataHolder)->globalSession)
#define _ZFP_ZFDebugGlobalNullSession (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFDebugDataHolder)->globalNullSession)

// ============================================================
ZFDebugCallback _ZFP_zfDebugTNull(void)
{
    return zfDebugSessionT(_ZFP_ZFDebugGlobalNullSession);
}

// ============================================================
zfclassNotPOD _ZFP_ZFDebugSessionPrivate
{
public:
    zfindex refCount;
    zfbool isEnabled;
    ZFDebugLevelEnum level;
    ZFOutputCallback callback;
    zfbool autoSpace;
    zfbool autoEndl;
public:
    _ZFP_ZFDebugSessionPrivate(void)
    : refCount(1)
    , isEnabled(zffalse)
    , level(ZFDebugLevel::e_Verbose)
    , callback()
    , autoSpace(zftrue)
    , autoEndl(zftrue)
    {
    }
};
ZFDebugSession::ZFDebugSession(void)
{
    _ZFP_ZFDebugSession_d = zfpoolNew(_ZFP_ZFDebugSessionPrivate);
}
ZFDebugSession::ZFDebugSession(ZF_IN const ZFDebugSession &ref)
{
    _ZFP_ZFDebugSession_d = ref._ZFP_ZFDebugSession_d;
    ++(_ZFP_ZFDebugSession_d->refCount);
}
ZFDebugSession &ZFDebugSession::operator =(ZF_IN const ZFDebugSession &ref)
{
    ++(ref._ZFP_ZFDebugSession_d->refCount);

    --(_ZFP_ZFDebugSession_d->refCount);
    if(_ZFP_ZFDebugSession_d->refCount == 0)
    {
        zfpoolDelete(_ZFP_ZFDebugSession_d);
    }

    _ZFP_ZFDebugSession_d = ref._ZFP_ZFDebugSession_d;
    return *this;
}
ZFDebugSession::~ZFDebugSession(void)
{
    --(_ZFP_ZFDebugSession_d->refCount);
    if(_ZFP_ZFDebugSession_d->refCount == 0)
    {
        zfpoolDelete(_ZFP_ZFDebugSession_d);
    }
    _ZFP_ZFDebugSession_d = zfnull;
}

void ZFDebugSession::autoSpaceSet(ZF_IN_OPT zfbool autoSpace /* = zftrue */)
{
    _ZFP_ZFDebugSession_d->autoSpace = autoSpace;
}
zfbool ZFDebugSession::autoSpace(void) const
{
    return _ZFP_ZFDebugSession_d->autoSpace;
}
void ZFDebugSession::autoEndlSet(ZF_IN_OPT zfbool autoEndl /* = zftrue */)
{
    _ZFP_ZFDebugSession_d->autoEndl = autoEndl;
}
zfbool ZFDebugSession::autoEndl(void) const
{
    return _ZFP_ZFDebugSession_d->autoEndl;
}

// ============================================================
const _ZFP_ZFDebugCallbackConvert ZFDebugConvert = {};
const ZFDebugCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugCallbackConvert const &v)
{
    return *ZFCastStatic(const ZFDebugCallback *, &d);
}

const _ZFP_ZFDebugAutoSpaceOn ZFDebugAutoSpaceOn = {};
const ZFOutputCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugAutoSpaceOn const &v)
{
    ZFCastStatic(const ZFDebugCallback *, &d)->autoSpaceSet(zftrue);
    return d;
}

const _ZFP_ZFDebugAutoSpaceOff ZFDebugAutoSpaceOff = {};
const ZFOutputCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugAutoSpaceOff const &v)
{
    ZFCastStatic(const ZFDebugCallback *, &d)->autoSpaceSet(zffalse);
    return d;
}

const _ZFP_ZFDebugAutoEndlOn ZFDebugAutoEndlOn = {};
const ZFOutputCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugAutoEndlOn const &v)
{
    ZFCastStatic(const ZFDebugCallback *, &d)->autoEndlSet(zftrue);
    return d;
}

const _ZFP_ZFDebugAutoEndlOff ZFDebugAutoEndlOff = {};
const ZFOutputCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugAutoEndlOff const &v)
{
    ZFCastStatic(const ZFDebugCallback *, &d)->autoEndlSet(zffalse);
    return d;
}

// ============================================================
zfclass _ZFP_I_ZFDebugCallbackOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFDebugCallbackOwner, ZFObject)

public:
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->firstTimeOutput = zftrue;
        this->prefix = zfnull;
        this->suffix = zfnull;
        return this;
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        if(this->prefix != zfnull)
        {
            session._ZFP_ZFDebugSession_d->callback.execute(this->prefix);
            zfsChange(this->prefix, (const zfchar *)zfnull);
        }
        if(session._ZFP_ZFDebugSession_d->isEnabled && session._ZFP_ZFDebugSession_d->autoEndl)
        {
            session._ZFP_ZFDebugSession_d->callback.execute(zfText("\n"));
        }
        if(this->suffix != zfnull)
        {
            session._ZFP_ZFDebugSession_d->callback.execute(this->suffix);
        }
        zfsuper::objectOnDeallocPrepare();
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfsChange(this->prefix, (const zfchar *)zfnull);
        zfsChange(this->suffix, (const zfchar *)zfnull);
        zfsuper::objectOnDealloc();
    }

public:
    void addPrefix(ZF_IN const zfchar *prefix)
    {
        this->prefix = zfsAppend(this->prefix, prefix);
    }
    void addSuffix(ZF_IN const zfchar *suffix)
    {
        this->suffix = zfsAppend(this->suffix, suffix);
    }

public:
    ZFMETHOD_DECLARE_2(zfindex, onOutput, const void *, src, zfindex, count)
    {
        if(!session._ZFP_ZFDebugSession_d->isEnabled)
        {
            return 0;
        }
        if(this->firstTimeOutput)
        {
            this->firstTimeOutput = zffalse;
            if(this->prefix != zfnull)
            {
                session._ZFP_ZFDebugSession_d->callback.execute(this->prefix);
                zfsChange(this->prefix, (const zfchar *)zfnull);
            }
        }
        else if(session._ZFP_ZFDebugSession_d->autoSpace)
        {
            session._ZFP_ZFDebugSession_d->callback.execute(zfText(" "));
        }

        session._ZFP_ZFDebugSession_d->callback.execute(src, count);
        return zfindexMax;
    }

public:
    ZFDebugSession session;
    zfbool firstTimeOutput;
    zfchar *prefix;
    zfchar *suffix;
};

// ============================================================
const ZFDebugCallback &ZFDebugCallback::autoSpaceSet(ZF_IN_OPT zfbool autoSpace /* = zftrue */) const
{
    _ZFP_I_ZFDebugCallbackOwner *owner = ZFCastZFObjectUnchecked(_ZFP_I_ZFDebugCallbackOwner *, this->callbackOwnerObject());
    if(owner != zfnull)
    {
        owner->session._ZFP_ZFDebugSession_d->autoSpace = autoSpace;
    }
    return *this;
}
const ZFDebugCallback &ZFDebugCallback::autoEndlSet(ZF_IN_OPT zfbool autoEndl /* = zftrue */) const
{
    _ZFP_I_ZFDebugCallbackOwner *owner = ZFCastZFObjectUnchecked(_ZFP_I_ZFDebugCallbackOwner *, this->callbackOwnerObject());
    if(owner != zfnull)
    {
        owner->session._ZFP_ZFDebugSession_d->autoEndl = autoEndl;
    }
    return *this;
}
const ZFDebugCallback &ZFDebugCallback::addPrefix(ZF_IN const zfchar *prefix) const
{
    _ZFP_I_ZFDebugCallbackOwner *owner = ZFCastZFObjectUnchecked(_ZFP_I_ZFDebugCallbackOwner *, this->callbackOwnerObject());
    if(owner != zfnull)
    {
        owner->addPrefix(prefix);
    }
    return *this;
}
const ZFDebugCallback &ZFDebugCallback::addSuffix(ZF_IN const zfchar *suffix) const
{
    _ZFP_I_ZFDebugCallbackOwner *owner = ZFCastZFObjectUnchecked(_ZFP_I_ZFDebugCallbackOwner *, this->callbackOwnerObject());
    if(owner != zfnull)
    {
        owner->addSuffix(suffix);
    }
    return *this;
}
const ZFDebugCallback &ZFDebugCallback::append(ZF_IN const zfchar *format,
                                               ...) const
{
    va_list vaList;
    va_start(vaList, format);
    this->appendV(format, vaList);
    va_end(vaList);
    return *this;
}
const ZFDebugCallback &ZFDebugCallback::appendV(ZF_IN const zfchar *format,
                                                ZF_IN va_list vaList) const
{
    zfstring tmp(zfslen(format));
    zfstringAppendV(tmp, format, vaList);
    this->execute(tmp.cString(), tmp.length());
    return *this;
}

// ============================================================
ZFDebugCallback _ZFP_ZFDebugCallbackCreate(ZF_IN_OUT ZFDebugSession &session)
{
    _ZFP_I_ZFDebugCallbackOwner *owner = zfAllocWithoutLeakTest(_ZFP_I_ZFDebugCallbackOwner);
    owner->session = session;
    ZFDebugCallback callback = ZFCallbackForMemberMethod(owner, ZFMethodAccess(_ZFP_I_ZFDebugCallbackOwner, onOutput));
    callback.callbackOwnerObjectRetain();
    zfReleaseWithoutLeakTest(owner);
    return callback;
}

// ============================================================
zfbool zfDebugStart(ZF_IN_OPT const ZFOutputCallback &callback /* = ZFOutputCallbackDefault */,
                    ZF_IN_OPT zfbool isEnabled /* = zftrue */)
{
    return zfDebugSessionStart(_ZFP_ZFDebugGlobalSession, callback, isEnabled);
}
const ZFOutputCallback &zfDebugOutputCallback(void)
{
    return zfDebugSessionOutputCallback(_ZFP_ZFDebugGlobalSession);
}
zfbool zfDebugEnableSet(ZF_IN_OPT zfbool isEnabled /* = zftrue */)
{
    return zfDebugSessionEnableSet(_ZFP_ZFDebugGlobalSession, isEnabled);
}
zfbool zfDebugEnabled(void)
{
    return zfDebugSessionEnabled(_ZFP_ZFDebugGlobalSession);
}
void zfDebugStop(void)
{
    zfDebugSessionStop(_ZFP_ZFDebugGlobalSession);
}
ZFDebugCallback zfDebugT(void)
{
    return zfDebugSessionT(_ZFP_ZFDebugGlobalSession);
}
void zfDebug(ZF_IN const zfchar *format,
             ...)
{
    if(!_ZFP_ZFDebugGlobalSession._ZFP_ZFDebugSession_d->isEnabled) {return ;}

    va_list vaList;
    va_start(vaList, format);
    (zfDebugSessionT(_ZFP_ZFDebugGlobalSession) << ZFDebugConvert).appendV(format, vaList);
    va_end(vaList);
}

// ============================================================
zfbool zfDebugSessionStart(ZF_IN_OUT ZFDebugSession &session,
                           ZF_IN_OPT const ZFOutputCallback &callback /* = ZFOutputCallbackDefault */,
                           ZF_IN_OPT zfbool isEnabled /* = zftrue */)
{
    zfDebugSessionStop(session);
    if(isEnabled)
    {
        if(callback.callbackIsValid())
        {
            session._ZFP_ZFDebugSession_d->callback = callback;
            session._ZFP_ZFDebugSession_d->isEnabled = zftrue;
        }
        else
        {
            return zffalse;
        }
    }
    return zftrue;
}
const ZFOutputCallback &zfDebugSessionOutputCallback(ZF_IN const ZFDebugSession &session)
{
    return session._ZFP_ZFDebugSession_d->callback;
}
zfbool zfDebugSessionEnableSet(ZF_IN_OUT ZFDebugSession &session,
                               ZF_IN_OPT zfbool isEnabled /* = zftrue */)
{
    if(isEnabled && session._ZFP_ZFDebugSession_d->callback.callbackIsValid())
    {
        session._ZFP_ZFDebugSession_d->isEnabled = zftrue;
    }
    else
    {
        session._ZFP_ZFDebugSession_d->isEnabled = zffalse;
    }
    return session._ZFP_ZFDebugSession_d->isEnabled;
}
zfbool zfDebugSessionEnabled(ZF_IN_OUT ZFDebugSession &session)
{
    return session._ZFP_ZFDebugSession_d->isEnabled;
}
void zfDebugSessionStop(ZF_IN_OUT ZFDebugSession &session)
{
    session._ZFP_ZFDebugSession_d->callback.callbackClear();
    session._ZFP_ZFDebugSession_d->isEnabled = zffalse;
}
ZFDebugCallback zfDebugSessionT(ZF_IN_OUT ZFDebugSession &session)
{
    return _ZFP_ZFDebugCallbackCreate(session);
}
void zfDebugSession(ZF_IN_OUT ZFDebugSession &session,
                    ZF_IN const zfchar *format,
                    ...)
{
    if(!session._ZFP_ZFDebugSession_d->isEnabled) {return ;}

    va_list vaList;
    va_start(vaList, format);
    (zfDebugSessionT(session) << ZFDebugConvert).appendV(format, vaList);
    va_end(vaList);
}

// ============================================================
zfstring _ZFP_ZFDebugMakeHeaderString(ZF_IN const zfcharA *callerFile,
                                      ZF_IN const zfcharA *callerFunction,
                                      ZF_IN zfindex callerLine)
{
    zfstring ret;
    ZF_CALLER_INFO_DETAIL_REF(ret, callerFile, callerFunction, callerLine);
    return ret;
}

// ============================================================
void zfDebugLevelSet(ZF_IN ZFDebugLevelEnum level)
{
    zfDebugSessionLevelSet(_ZFP_ZFDebugGlobalSession, level);
}
ZFDebugLevelEnum zfDebugLevelGet(void)
{
    return zfDebugSessionLevelGet(_ZFP_ZFDebugGlobalSession);
}
zfbool zfDebugIsActive(ZF_IN ZFDebugLevelEnum level)
{
    return zfDebugSessionIsActive(_ZFP_ZFDebugGlobalSession, level);
}

void zfDebugSessionLevelSet(ZF_IN_OUT ZFDebugSession &session,
                            ZF_IN ZFDebugLevelEnum level)
{
    session._ZFP_ZFDebugSession_d->level = level;
}
ZFDebugLevelEnum zfDebugSessionLevelGet(ZF_IN_OUT ZFDebugSession &session)
{
    return session._ZFP_ZFDebugSession_d->level;
}
zfbool zfDebugSessionIsActive(ZF_IN_OUT ZFDebugSession &session,
                              ZF_IN ZFDebugLevelEnum level)
{
    return (session._ZFP_ZFDebugSession_d->level <= level);
}

ZF_NAMESPACE_GLOBAL_END

