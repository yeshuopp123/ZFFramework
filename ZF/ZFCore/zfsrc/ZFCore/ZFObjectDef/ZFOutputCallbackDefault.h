/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFOutputCallbackDefault.h
 * @brief default callback for output
 */

#ifndef _ZFI_ZFOutputCallbackDefault_h_
#define _ZFI_ZFOutputCallbackDefault_h_

#include "ZFObjectObserverDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFOutputCallbackDefault changed,
 * params and result has no use and is always null
 */
ZFOBSERVER_EVENT_GLOBAL(ZFOutputCallbackDefaultOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFOutputCallbackDefaultHolder
{
public:
    const ZFOutputCallback &operator = (ZF_IN const ZFOutputCallback &callback) const;
    operator const ZFOutputCallback &(void) const;
};
/**
 * @brief default output callback, source must be zfchar *string
 *
 * you may change this at runtime,
 * and #ZFGlobalEvent::EventZFOutputCallbackDefaultOnChange would be fired
 */
#define ZFOutputCallbackDefault _ZFP_ZFOutputCallbackDefaultHolder()

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputCallbackDefault_h_

