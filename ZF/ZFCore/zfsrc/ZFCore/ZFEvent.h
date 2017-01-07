/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFEvent.h
 * @brief event define
 */

#ifndef _ZFI_ZFEvent_h_
#define _ZFI_ZFEvent_h_

#include "ZFObject.h"
#include "ZFCacheable.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief base class of all event in ZFFramework
 */
zfclass ZF_ENV_EXPORT ZFEvent : zfextends ZFObject, zfimplements ZFCacheable
{
    ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(ZFEvent, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFCacheable)

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

    zfoverride
    virtual void cacheableOnReset(void)
    {
        zfsuperI(ZFCacheable)::cacheableOnReset();
        this->eventResolvedSet(zffalse);
    }

public:
    /**
     * @brief whether the event has been resolved
     */
    virtual void eventResolvedSet(ZF_IN zfbool const &value)
    {
        _ZFP_ZFEvent_eventResolved = value;
    }
    /**
     * @brief see #eventResolvedSet
     */
    virtual zfbool eventResolved(void)
    {
        return _ZFP_ZFEvent_eventResolved;
    }
private:
    zfbool _ZFP_ZFEvent_eventResolved;
protected:
    /** @cond ZFPrivateDoc */
    ZFEvent(void) : _ZFP_ZFEvent_eventResolved(zffalse) {}
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEvent_h_

