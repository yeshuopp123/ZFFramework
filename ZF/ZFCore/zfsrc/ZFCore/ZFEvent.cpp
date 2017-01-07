/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZFOBJECT_REGISTER(ZFEvent)

void ZFEvent::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    if(this->eventResolved())
    {
        ret += zfText(" (resolved)");
    }
    ZFClassUtil::objectPropertyInfo(ret, this);
}

ZF_NAMESPACE_GLOBAL_END

