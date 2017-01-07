/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFUIKit/ZFUIKeyCode.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUIKeyAction ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFUIKeyAction)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIKeyAction ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFUIKeyAction)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIKeyAction, ZFUIKeyAction, KeyDown)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIKeyAction, ZFUIKeyAction, KeyRepeat)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIKeyAction, ZFUIKeyAction, KeyUp)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIKeyAction, ZFUIKeyAction, KeyCancel)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFUIKeyAction, ZFUIKeyAction)

#endif // #if ZF_ENV_sys_Android

