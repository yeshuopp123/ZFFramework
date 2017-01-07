/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFDebug.h
 * @brief debug utility, not thread-safe
 */

#ifndef _ZFI_ZFDebug_h_
#define _ZFI_ZFDebug_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFDebugSessionPrivate;
/**
 * @brief session used by ZFDebug
 *
 * this class is designed to be used by ZFDebug to output to
 * different position at the same time, typical usage:
 * @code
 *   ZFDebugSession session1, session2;
 *   zfDebugSessionStart(session1, aOutputCallback);
 *   zfDebugSessionStart(session2, aOutputCallback);
 *   zfDebugSessionT(session1) << something1;
 *   zfDebugSessionT(session2) << something2;
 * @endcode
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFDebugSession
{
public:
    /**
     * @brief main constructor
     */
    ZFDebugSession(void);
    /**
     * @brief retain from another session
     */
    ZFDebugSession(ZF_IN const ZFDebugSession &ref);
    /**
     * @brief retain from another session
     */
    zffinal ZFDebugSession &operator =(ZF_IN const ZFDebugSession &ref);
    ~ZFDebugSession(void);

public:
    /**
     * @brief see #ZFDebugAutoSpaceOn
     */
    zffinal void autoSpaceSet(ZF_IN_OPT zfbool autoSpace = zftrue);
    /**
     * @brief see #ZFDebugAutoSpaceOn
     */
    zffinal zfbool autoSpace(void) const;
    /**
     * @brief see #ZFDebugAutoEndlOn
     */
    zffinal void autoEndlSet(ZF_IN_OPT zfbool autoEndl = zftrue);
    /**
     * @brief see #ZFDebugAutoEndlOn
     */
    zffinal zfbool autoEndl(void) const;

    /** @cond ZFPrivateDoc */
    _ZFP_ZFDebugSessionPrivate *_ZFP_ZFDebugSession_d;
    /** @endcond */
};

/**
 * @brief internal class used by ZFDebug
 *
 * use zfDebugT or zfDebugSessionT series to access this class
 * @see zfDebugT, zfDebugSessionT
 * @warning you should not use this class directly,
 *   unless you know what you're doing
 *
 * ADVANCED\n
 * to output your own type, please refer to #ZFOUTPUT_TYPE
 */
ZFCALLBACK_DECLARE_BEGIN(ZFDebugCallback, ZFOutputCallback)
public:
    /**
     * @brief whether to auto add space during each output
     */
    virtual const ZFDebugCallback &autoSpaceSet(ZF_IN_OPT zfbool autoSpace = zftrue) const;
    /**
     * @brief whether to auto add endl during each output end
     */
    virtual const ZFDebugCallback &autoEndlSet(ZF_IN_OPT zfbool autoEndl = zftrue) const;

public:
    /**
     * @brief prefix before output anything
     */
    virtual const ZFDebugCallback &addPrefix(ZF_IN const zfchar *prefix) const;
    /**
     * @brief suffix output while dealloc of ZFDebugCallback
     */
    virtual const ZFDebugCallback &addSuffix(ZF_IN const zfchar *suffix) const;
    /**
     * @brief append format
     */
    virtual const ZFDebugCallback &append(ZF_IN const zfchar *format,
                                          ...) const;
    /**
     * @brief append format
     */
    virtual const ZFDebugCallback &appendV(ZF_IN const zfchar *format,
                                           ZF_IN va_list vaList) const;
ZFCALLBACK_DECLARE_END(ZFDebugCallback, ZFOutputCallback)

extern ZF_ENV_EXPORT ZFDebugCallback _ZFP_ZFDebugCallbackCreate(ZF_IN_OUT ZFDebugSession &session);

extern ZF_ENV_EXPORT ZFDebugCallback _ZFP_zfDebugTNull(void);

/**
 * @brief a null debug callback that do nothing while appending data
 */
#define zfDebugTNull (_ZFP_zfDebugTNull())

zfclassPOD ZF_ENV_EXPORT _ZFP_ZFDebugCallbackConvert
{
};
/**
 * @brief used to convert ZFOutputCallback to ZFDebugCallback
 *
 * usually you should not care about this command, use with causion
 * @see ZFDebugAutoSpaceOn
 */
extern ZF_ENV_EXPORT const _ZFP_ZFDebugCallbackConvert ZFDebugConvert;
/**
 * @brief convert ZFOutputCallback to ZFDebugCallback, see #ZFDebugConvert
 */
extern ZF_ENV_EXPORT const ZFDebugCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugCallbackConvert const &v);

zfclassPOD ZF_ENV_EXPORT _ZFP_ZFDebugAutoSpaceOn
{
};
/**
 * @brief used to set ZFDebugCallback's auto space enabled
 *
 * usage:
 * @code
 *   // will append space automatically between something0 and something1
 *   zfDebug() << ZFDebugAutoSpaceOn << something0 << something1;
 * @endcode
 *
 * @note there are more control types which can be added to ZFDebugCallback:
 *   -  ZFDebugConvert: convert ZFOutputCallback to ZFDebugCallback
 *   -  ZFDebugAutoSpaceOn: set auto space enabled to ZFDebugCallback
 *   -  ZFDebugAutoSpaceOff: set auto space disabled to ZFDebugCallback
 *   -  ZFDebugAutoEndlOn: set auto endl enabled to ZFDebugCallback
 *   -  ZFDebugAutoEndlOff: set auto endl disabled to ZFDebugCallback
 *
 * @warning you must make sure the object you call operator<< to is ZFDebugCallback,
 *   otherwise app may crash or throw exception
 */
extern ZF_ENV_EXPORT const _ZFP_ZFDebugAutoSpaceOn ZFDebugAutoSpaceOn;
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const ZFOutputCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugAutoSpaceOn const &v);

zfclassPOD ZF_ENV_EXPORT _ZFP_ZFDebugAutoSpaceOff
{
};
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const _ZFP_ZFDebugAutoSpaceOff ZFDebugAutoSpaceOff;
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const ZFOutputCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugAutoSpaceOff const &v);

zfclassPOD ZF_ENV_EXPORT _ZFP_ZFDebugAutoEndlOn
{
};
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const _ZFP_ZFDebugAutoEndlOn ZFDebugAutoEndlOn;
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const ZFOutputCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugAutoEndlOn const &v);

zfclassPOD ZF_ENV_EXPORT _ZFP_ZFDebugAutoEndlOff
{
};
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const _ZFP_ZFDebugAutoEndlOff ZFDebugAutoEndlOff;
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const ZFOutputCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugAutoEndlOff const &v);

// ============================================================
extern ZF_ENV_EXPORT zfstring _ZFP_ZFDebugMakeHeaderString(ZF_IN const zfcharA *callerFile,
                                                           ZF_IN const zfcharA *callerFunction,
                                                           ZF_IN zfindex callerLine);

/** @cond ZFPrivateDoc */
ZF_NAMESPACE_BEGIN(_ZFP_ZFDebugDummyNamespace)
    ZF_NAMESPACE_BEGIN(zfsuper)
        inline const ZFClass *ClassData(void)
        {
            return zfnull;
        }
    ZF_NAMESPACE_END(zfsuper)
    ZF_NAMESPACE_BEGIN(zfself)
        inline const ZFClass *ClassData(void)
        {
            return zfnull;
        }
    ZF_NAMESPACE_END(zfself)
ZF_NAMESPACE_END(_ZFP_ZFDebugDummyNamespace)
ZF_NAMESPACE_USE(_ZFP_ZFDebugDummyNamespace)
/** @endcond */
/**
 * @brief a convenient debug header string
 *
 * typically looks like this (not ensured):\n
 *   "[file function (line)]"
 */
#define ZFDEBUG_HEADER_STRING (_ZFP_ZFDebugMakeHeaderString(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE).cString())

// ============================================================
/**
 * @brief see #zfDebugSessionStart
 */
extern ZF_ENV_EXPORT zfbool zfDebugStart(ZF_IN_OPT const ZFOutputCallback &callback = ZFOutputCallbackDefault,
                                         ZF_IN_OPT zfbool isEnabled = zftrue);
/**
 * @brief see #zfDebugStart
 */
extern ZF_ENV_EXPORT const ZFOutputCallback &zfDebugOutputCallback(void);
/**
 * @brief see #zfDebugSessionEnableSet
 */
extern ZF_ENV_EXPORT zfbool zfDebugEnableSet(ZF_IN_OPT zfbool isEnabled = zftrue);
/**
 * @brief see #zfDebugSessionEnabled
 */
extern ZF_ENV_EXPORT zfbool zfDebugEnabled(void);
/**
 * @brief see #zfDebugSessionStop
 */
extern ZF_ENV_EXPORT void zfDebugStop(void);
/**
 * @brief see #zfDebugSessionT
 */
extern ZF_ENV_EXPORT ZFDebugCallback zfDebugT(void);
/**
 * @brief see #zfDebugSession
 */
extern ZF_ENV_EXPORT void zfDebug(ZF_IN const zfchar *format,
                                  ...);

/**
 * @brief active only if ZF_ENV_DEBUG is set to 1
 * @see ZF_ENV_DEBUG
 */
#if ZF_ENV_DEBUG
    #define ZFDEBUG(format, ...) zfDebug(format, ##__VA_ARGS__)
#else
    #define ZFDEBUG(format, ...) ((void)0)
#endif

/**
 * @brief init ZFDebug and set callback as output callback
 * @param session session used to specify the position to output
 * @param callback output callback
 * @param isEnabled zffalse to completely disable ZFDebug,
 *   ZFDebug won't active until next time you call init or enable
 */
extern ZF_ENV_EXPORT zfbool zfDebugSessionStart(ZF_IN_OUT ZFDebugSession &session,
                                                ZF_IN_OPT const ZFOutputCallback &callback = ZFOutputCallbackDefault,
                                                ZF_IN_OPT zfbool isEnabled = zftrue);
/**
 * @brief get the internal output callback
 */
extern ZF_ENV_EXPORT const ZFOutputCallback &zfDebugSessionOutputCallback(ZF_IN const ZFDebugSession &session);
/**
 * @brief temporarily enable/disable ZFDebug
 */
extern ZF_ENV_EXPORT zfbool zfDebugSessionEnableSet(ZF_IN_OUT ZFDebugSession &session,
                                                    ZF_IN_OPT zfbool isEnabled = zftrue);
/**
 * @brief return true if ZFDebug currently enabled
 *
 * always return false if isEnabled is set to zffalse when init
 */
extern ZF_ENV_EXPORT zfbool zfDebugSessionEnabled(ZF_IN_OUT ZFDebugSession &session);
/**
 * @brief cleanup and stop ZFDebug described by session
 */
extern ZF_ENV_EXPORT void zfDebugSessionStop(ZF_IN_OUT ZFDebugSession &session);
/**
 * @brief begin to debug
 *
 * typical usage:
 * @code
 *   zfDebugSessionT(session) << something1 << something2;
 * @endcode
 * by default, space would be auto added between "something1" and "something2",
 * and '\\n' would be auto added after this line
 * @see ZFDebugAutoSpaceOn, ZFDebugAutoEndlOn
 */
extern ZF_ENV_EXPORT ZFDebugCallback zfDebugSessionT(ZF_IN_OUT ZFDebugSession &session);
/**
 * @brief begin to debug
 *
 * typical usage:
 * @code
 *   zfDebugSession(session, format, paramList...);
 * @endcode
 * this function would automatically add a "\n" to tail
 * @see zfDebugSessionT
 */
extern ZF_ENV_EXPORT void zfDebugSession(ZF_IN_OUT ZFDebugSession &session,
                                         ZF_IN const zfchar *format,
                                         ...);

/**
 * @brief active only if ZF_ENV_DEBUG is set to 1
 * @see ZF_ENV_DEBUG
 */
#if ZF_ENV_DEBUG
    #define ZFDEBUGSESSION(session, format, ...) zfDebugSession(session, format, ##__VA_ARGS__)
#else
    #define ZFDEBUGSESSION(session, format, ...) ((void)0)
#endif

// ============================================================
/**
 * @brief log level
 *
 * similar to Android's Log.v() to Log.e(),
 * we have zfDebug V ~ E series: Verbose, Debug, Info, Warning, Error\n
 * you can use them by zfDebugV/zfDebugVT/zfDebugSessionV/zfDebugSessionVT series,
 * controlled by #zfDebugLevelSet/#zfDebugSessionLevelSet,
 * level state can be verified by #zfDebugIsActive/#zfDebugSessionIsActive
 */
ZFENUM_BEGIN(ZFDebugLevel)
    ZFENUM_VALUE(Verbose)
    ZFENUM_VALUE(Debug)
    ZFENUM_VALUE(Info)
    ZFENUM_VALUE(Warning)
    ZFENUM_VALUE(Error)
    ZFENUM_VALUE(Assert)
ZFENUM_SEPARATOR(ZFDebugLevel)
    ZFENUM_VALUE_REGISTER(Verbose)
    ZFENUM_VALUE_REGISTER(Debug)
    ZFENUM_VALUE_REGISTER(Info)
    ZFENUM_VALUE_REGISTER(Warning)
    ZFENUM_VALUE_REGISTER(Error)
    ZFENUM_VALUE_REGISTER(Assert)
ZFENUM_END(ZFDebugLevel)

/**
 * @brief see #ZFDebugLevel
 */
extern ZF_ENV_EXPORT void zfDebugLevelSet(ZF_IN ZFDebugLevelEnum level);
/**
 * @brief see #ZFDebugLevel
 */
extern ZF_ENV_EXPORT ZFDebugLevelEnum zfDebugLevelGet(void);
/**
 * @brief see #ZFDebugLevel
 */
extern ZF_ENV_EXPORT zfbool zfDebugIsActive(ZF_IN ZFDebugLevelEnum level);

/**
 * @brief see #ZFDebugLevel
 */
extern ZF_ENV_EXPORT void zfDebugSessionLevelSet(ZF_IN_OUT ZFDebugSession &session,
                                                 ZF_IN ZFDebugLevelEnum level);
/**
 * @brief see #ZFDebugLevel
 */
extern ZF_ENV_EXPORT ZFDebugLevelEnum zfDebugSessionLevelGet(ZF_IN_OUT ZFDebugSession &session);
/**
 * @brief see #ZFDebugLevel
 */
extern ZF_ENV_EXPORT zfbool zfDebugSessionIsActive(ZF_IN_OUT ZFDebugSession &session,
                                                   ZF_IN ZFDebugLevelEnum level);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFDebug_h_

