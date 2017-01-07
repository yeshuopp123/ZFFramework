/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIMouseEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIMouseAction)
ZFENUM_DEFINE(ZFUIMouseButton)

ZFOBJECT_REGISTER(ZFUIMouseEvent)
ZFCACHEABLE_DEFINE(ZFUIMouseEvent, ZFUIMouseEvent)

void ZFUIMouseEvent::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    ret += ZFUIMouseAction::EnumNameForValue(this->mouseAction);
    ret += zfText(" ");
    ZFUIPointToString(ret, this->mousePoint);

    if(this->mouseButton != ZFUIMouseButton::e_MouseButtonLeft)
    {
        ret += zfText(" ");
        ret += ZFUIMouseButton::EnumNameForValue(this->mouseButton);
    }

    if(this->eventResolved())
    {
        ret += zfText(" (resolved)");
    }
}

void ZFUIMouseEvent::cacheableOnReset(void)
{
    zfsuper::cacheableOnReset();
    this->mouseAction = ZFUIMouseAction::e_MouseDown;
    this->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
}

void ZFUIMouseEvent::eventOnApplyScale(ZF_IN zffloat scale)
{
    zfsuper::eventOnApplyScale(scale);
    this->mousePoint = ZFUIPointApplyScale(this->mousePoint, scale);
}
void ZFUIMouseEvent::eventOnApplyScaleReversely(ZF_IN zffloat scale)
{
    zfsuper::eventOnApplyScale(scale);
    this->mousePoint = ZFUIPointApplyScaleReversely(this->mousePoint, scale);
}

ZF_NAMESPACE_GLOBAL_END

