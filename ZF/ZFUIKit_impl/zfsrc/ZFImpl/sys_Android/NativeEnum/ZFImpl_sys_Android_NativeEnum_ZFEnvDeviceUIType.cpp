/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFUIKit/ZFEnvInfo_ZFUIKit.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFEnvDeviceUIType ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFEnvDeviceUIType)
#define ZFImpl_sys_Android_JNI_NAME_ZFEnvDeviceUIType ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFEnvDeviceUIType)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFEnvDeviceUIType, ZFEnvDeviceUIType, Desktop)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFEnvDeviceUIType, ZFEnvDeviceUIType, Handheld)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFEnvDeviceUIType, ZFEnvDeviceUIType, Wearable)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFEnvDeviceUIType, ZFEnvDeviceUIType, TV)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFEnvDeviceUIType, ZFEnvDeviceUIType)

#endif // #if ZF_ENV_sys_Android

