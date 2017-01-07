/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFCore/ZFDebug.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFDebugLevel ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFCore_ZFDebugLevel)
#define ZFImpl_sys_Android_JNI_NAME_ZFDebugLevel ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFCore.ZFDebugLevel)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevel, Verbose)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevel, Debug)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevel, Info)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevel, Warning)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevel, Error)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevel, Assert)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevel)

#endif // #if ZF_ENV_sys_Android

