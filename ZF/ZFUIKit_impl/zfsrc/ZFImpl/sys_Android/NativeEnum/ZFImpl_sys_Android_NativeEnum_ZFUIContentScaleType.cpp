/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFUIKit/ZFUITypeDef.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUIContentScaleType ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFUIContentScaleType)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIContentScaleType ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFUIContentScaleType)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIContentScaleType, ZFUIContentScaleType, Fill)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIContentScaleType, ZFUIContentScaleType, Center)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIContentScaleType, ZFUIContentScaleType, FillCenter)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIContentScaleType, ZFUIContentScaleType, FillCenterClipped)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIContentScaleType, ZFUIContentScaleType, FitCenter)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIContentScaleType, ZFUIContentScaleType, FillX)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIContentScaleType, ZFUIContentScaleType, FitX)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIContentScaleType, ZFUIContentScaleType, FillY)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIContentScaleType, ZFUIContentScaleType, FitY)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFUIContentScaleType, ZFUIContentScaleType)

#endif // #if ZF_ENV_sys_Android

