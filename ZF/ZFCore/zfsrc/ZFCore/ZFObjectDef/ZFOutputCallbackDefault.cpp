/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFOutputCallbackDefault.h"

#include "ZFGlobalEventCenter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, ZFOutputCallbackDefaultOnChange)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFOutputCallbackDefaultDataHolder, ZFLevelZFFrameworkNormal)
{
}
public:
    ZFOutputCallback outputCallbackDefault;
ZF_GLOBAL_INITIALIZER_END(ZFOutputCallbackDefaultDataHolder)

const ZFOutputCallback &_ZFP_ZFOutputCallbackDefaultHolder::operator = (ZF_IN const ZFOutputCallback &callback) const
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFOutputCallbackDefaultDataHolder)->outputCallbackDefault = callback;
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFOutputCallbackDefaultOnChange());
    return callback;
}
_ZFP_ZFOutputCallbackDefaultHolder::operator const ZFOutputCallback &(void) const
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFOutputCallbackDefaultDataHolder)->outputCallbackDefault;
}

ZF_NAMESPACE_GLOBAL_END

