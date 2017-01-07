/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFCore/ZFProtocol.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFProtocolLevel ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFCore_ZFProtocolLevel)
#define ZFImpl_sys_Android_JNI_NAME_ZFProtocolLevel ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFCore.ZFProtocolLevel)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFProtocolLevel, ZFProtocolLevel, Default)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFProtocolLevel, ZFProtocolLevel, SystemLow)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFProtocolLevel, ZFProtocolLevel, SystemNormal)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFProtocolLevel, ZFProtocolLevel, SystemHigh)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFProtocolLevel, ZFProtocolLevel, AppLow)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFProtocolLevel, ZFProtocolLevel, AppNormal)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFProtocolLevel, ZFProtocolLevel, AppHigh)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFProtocolLevel, ZFProtocolLevel)

#endif // #if ZF_ENV_sys_Android

