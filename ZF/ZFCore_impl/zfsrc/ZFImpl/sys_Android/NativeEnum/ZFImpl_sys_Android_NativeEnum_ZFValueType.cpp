/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFCore/ZFValue.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFValueType ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFCore_ZFValueType)
#define ZFImpl_sys_Android_JNI_NAME_ZFValueType ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFCore.ZFValueType)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType, bool)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType, char)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType, int)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType, unsignedInt)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType, index)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType, float)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType, double)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType, longDouble)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType, time)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType, flags)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType, identity)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType, serializableData)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFValueType, ZFValueType)

#endif // #if ZF_ENV_sys_Android

