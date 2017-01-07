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

#define ZFImpl_sys_Android_JNI_ID_ZFUIOrientation ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFUIOrientation)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIOrientation ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFUIOrientation)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIOrientation, ZFUIOrientation, Left)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIOrientation, ZFUIOrientation, Top)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIOrientation, ZFUIOrientation, Right)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIOrientation, ZFUIOrientation, Bottom)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFUIOrientation, ZFUIOrientation)

#endif // #if ZF_ENV_sys_Android

