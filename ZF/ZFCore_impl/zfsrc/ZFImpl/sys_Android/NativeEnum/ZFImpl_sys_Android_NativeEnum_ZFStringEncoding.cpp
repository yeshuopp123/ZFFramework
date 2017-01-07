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

#define ZFImpl_sys_Android_JNI_ID_ZFStringEncoding ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFCore_ZFStringEncoding)
#define ZFImpl_sys_Android_JNI_NAME_ZFStringEncoding ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFCore.ZFStringEncoding)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFStringEncoding, ZFStringEncoding, UTF8)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFStringEncoding, ZFStringEncoding, UTF16LE)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFStringEncoding, ZFStringEncoding, UTF16BE)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFStringEncoding, ZFStringEncoding, UTF16)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFStringEncoding, ZFStringEncoding)

#endif // #if ZF_ENV_sys_Android

