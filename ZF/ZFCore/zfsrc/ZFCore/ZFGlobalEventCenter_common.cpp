/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFGlobalEventCenter_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, AppOnCreate)
ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, AppOnDestroy)
ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, AppOnReceiveMemoryWarning)

static void _ZFP_ZFGlobalEventCenter_common_AppOnCreate(void)
{
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventAppOnCreate());
}
static void _ZFP_ZFGlobalEventCenter_common_AppOnDestroy(void)
{
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventAppOnDestroy());
}

ZF_STATIC_INITIALIZER_INIT(_ZFP_ZFGlobalEventCenter_common_register)
{
    ZFFrameworkInitFinishCallbacks.add(_ZFP_ZFGlobalEventCenter_common_AppOnCreate);
    ZFFrameworkCleanupPrepareCallbacks.add(_ZFP_ZFGlobalEventCenter_common_AppOnDestroy);
}
ZF_STATIC_INITIALIZER_DESTROY(_ZFP_ZFGlobalEventCenter_common_register)
{
    ZFFrameworkInitFinishCallbacks.removeElement(_ZFP_ZFGlobalEventCenter_common_AppOnCreate);
    ZFFrameworkCleanupPrepareCallbacks.removeElement(_ZFP_ZFGlobalEventCenter_common_AppOnDestroy);
}
ZF_STATIC_INITIALIZER_END(_ZFP_ZFGlobalEventCenter_common_register)

ZF_NAMESPACE_GLOBAL_END

