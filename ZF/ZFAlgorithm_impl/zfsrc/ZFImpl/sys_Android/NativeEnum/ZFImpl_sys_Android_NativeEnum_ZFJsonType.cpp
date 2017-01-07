/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFAlgorithm/ZFJson.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFJsonType ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFAlgorithm_ZFJsonType)
#define ZFImpl_sys_Android_JNI_NAME_ZFJsonType ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFAlgorithm.ZFJsonType)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFJsonType, ZFJsonType, JsonNull)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFJsonType, ZFJsonType, JsonValue)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFJsonType, ZFJsonType, JsonObject)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFJsonType, ZFJsonType, JsonArray)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFJsonType, ZFJsonType)

#endif // #if ZF_ENV_sys_Android

