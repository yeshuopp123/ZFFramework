/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIWheelEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIWheelEvent)
ZFCACHEABLE_DEFINE(ZFUIWheelEvent, ZFUIWheelEvent)

void ZFUIWheelEvent::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    ret += zfText("wheel ");
    zfsFromIntT(ret, this->wheelX);
    ret += zfText(", ");
    zfsFromIntT(ret, this->wheelY);

    if(this->eventResolved())
    {
        ret += zfText(" (resolved)");
    }
}

void ZFUIWheelEvent::cacheableOnReset(void)
{
    zfsuper::cacheableOnReset();
    this->wheelX = 0;
    this->wheelY = 0;
}

ZF_NAMESPACE_GLOBAL_END

