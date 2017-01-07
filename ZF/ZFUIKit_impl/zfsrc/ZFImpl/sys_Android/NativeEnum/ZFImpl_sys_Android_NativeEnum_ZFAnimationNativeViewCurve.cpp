/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFUIKit/ZFAnimationNativeView.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFAnimationNativeViewCurve ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFAnimationNativeViewCurve)
#define ZFImpl_sys_Android_JNI_NAME_ZFAnimationNativeViewCurve ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFAnimationNativeViewCurve)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFAnimationNativeViewCurve, ZFAnimationNativeViewCurve, Linear)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFAnimationNativeViewCurve, ZFAnimationNativeViewCurve, EaseIn)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFAnimationNativeViewCurve, ZFAnimationNativeViewCurve, EaseOut)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFAnimationNativeViewCurve, ZFAnimationNativeViewCurve, EaseInOut)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFAnimationNativeViewCurve, ZFAnimationNativeViewCurve)

#endif // #if ZF_ENV_sys_Android

