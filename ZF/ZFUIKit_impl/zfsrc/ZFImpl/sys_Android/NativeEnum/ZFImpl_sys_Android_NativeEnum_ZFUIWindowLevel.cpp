/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFUIKit/ZFUIWindow.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFUIWindowLevel)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIWindowLevel ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFUIWindowLevel)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, ZFFrameworkBgLowest)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, ZFFrameworkBgLow)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, ZFFrameworkBgNormal)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, ZFFrameworkBgHigh)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, ZFFrameworkBgHighest)

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, AppLowest)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, AppLow)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, AppNormal)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, AppHigh)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, AppHighest)

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, ZFFrameworkFgLowest)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, ZFFrameworkFgLow)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, ZFFrameworkFgNormal)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, ZFFrameworkFgHigh)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel, ZFFrameworkFgHighest)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFUIWindowLevel, ZFUIWindowLevel)

#endif // #if ZF_ENV_sys_Android

