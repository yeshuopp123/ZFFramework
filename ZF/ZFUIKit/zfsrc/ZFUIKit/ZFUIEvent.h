/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIEvent.h
 * @brief base class of all ui event
 */

#ifndef _ZFI_ZFUIEvent_h_
#define _ZFI_ZFUIEvent_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief base class of all ui event
 */
zfclass ZF_ENV_EXPORT ZFUIEvent : zfextends ZFEvent
{
    ZFOBJECT_DECLARE(ZFUIEvent, ZFEvent)

public:
    inline void _ZFP_ZFUIEvent_eventOnApplyScale(ZF_IN zffloat scale)
    {
        this->eventOnApplyScale(scale);
    }
    inline void _ZFP_ZFUIEvent_eventOnApplyScaleReversely(ZF_IN zffloat scale)
    {
        this->eventOnApplyScaleReversely(scale);
    }
protected:
    /**
     * @brief see #ZFUIView::scaleOnChange, usually internal use only
     */
    virtual void eventOnApplyScale(ZF_IN zffloat scale)
    {
    }
    /**
     * @brief see #eventOnApplyScale
     */
    virtual void eventOnApplyScaleReversely(ZF_IN zffloat scale)
    {
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIEvent_h_

