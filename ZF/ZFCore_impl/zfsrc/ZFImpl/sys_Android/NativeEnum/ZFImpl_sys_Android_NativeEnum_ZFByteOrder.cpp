/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFCore/ZFByteOrder.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFByteOrder ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFCore_ZFByteOrder)
#define ZFImpl_sys_Android_JNI_NAME_ZFByteOrder ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFCore.ZFByteOrder)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFByteOrder, ZFByteOrder, LittleEndian)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFByteOrder, ZFByteOrder, BigEndian)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFByteOrder, ZFByteOrder)

#endif // #if ZF_ENV_sys_Android

