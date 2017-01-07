/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFUIKit/ZFUIMouseEvent.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUIMouseAction ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFUIMouseAction)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIMouseAction ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFUIMouseAction)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIMouseAction, ZFUIMouseAction, MouseDown)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIMouseAction, ZFUIMouseAction, MouseMove)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIMouseAction, ZFUIMouseAction, MouseUp)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIMouseAction, ZFUIMouseAction, MouseCancel)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIMouseAction, ZFUIMouseAction, MouseHoverEnter)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIMouseAction, ZFUIMouseAction, MouseHover)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIMouseAction, ZFUIMouseAction, MouseHoverExit)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFUIMouseAction, ZFUIMouseAction)

#endif // #if ZF_ENV_sys_Android

