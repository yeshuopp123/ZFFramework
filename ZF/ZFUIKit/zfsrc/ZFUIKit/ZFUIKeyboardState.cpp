/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIKeyboardState.h"
#include "protocol/ZFProtocolZFUIKeyboardState.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIKeyboardState)

ZFOBSERVER_EVENT_REGISTER(ZFUIKeyboardState, KeyPressedOnChange)

ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFUIKeyboardState, ZFUIKeyboardState, ZFUIKeyboardState, instance, ZFLevelZFFrameworkNormal)

zfbool ZFUIKeyboardState::keyPressed(ZF_IN ZFUIKeyCodeEnum keyCode)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIKeyboardState) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIKeyboardState);
    if(impl != zfnull)
    {
        return impl->keyPressed(keyCode);
    }
    return zffalse;
}
zfbool ZFUIKeyboardState::keyPressedRaw(ZF_IN zfuint32 keyCodeRaw)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIKeyboardState) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIKeyboardState);
    if(impl != zfnull)
    {
        return impl->keyPressedRaw(keyCodeRaw);
    }
    return zffalse;
}

void ZFUIKeyboardState::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    zfstring tmp;
    for(zfindex i = 0; i < ZFUIKeyCode::EnumCount(); ++i)
    {
        if(this->keyPressed((ZFUIKeyCodeEnum)ZFUIKeyCode::EnumValueAtIndex(i)))
        {
            if(!tmp.isEmpty())
            {
                tmp += zfText(", ");
            }
            tmp += ZFUIKeyCode::EnumNameAtIndex(i);
        }
    }
    if(!tmp.isEmpty())
    {
        ret += zfText(", pressed: ");
        ret += tmp;
    }
}

ZF_NAMESPACE_GLOBAL_END

