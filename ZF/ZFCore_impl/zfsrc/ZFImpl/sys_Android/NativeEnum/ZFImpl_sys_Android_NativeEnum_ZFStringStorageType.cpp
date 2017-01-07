/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFStringStorageType ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFCore_ZFStringStorageType)
#define ZFImpl_sys_Android_JNI_NAME_ZFStringStorageType ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFCore.ZFStringStorageType)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFStringStorageType, ZFStringStorageType, NativeString)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFStringStorageType, ZFStringStorageType, Copy)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFStringStorageType, ZFStringStorageType, CharBufferRef)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFStringStorageType, ZFStringStorageType)

#endif // #if ZF_ENV_sys_Android

