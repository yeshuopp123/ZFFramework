/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFCore/ZFFile.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFFileOpenOption ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFCore_ZFFileOpenOption)
#define ZFImpl_sys_Android_JNI_NAME_ZFFileOpenOption ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFCore.ZFFileOpenOption)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFFileOpenOption, ZFFileOpenOption, Create)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFFileOpenOption, ZFFileOpenOption, Read)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFFileOpenOption, ZFFileOpenOption, Write)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFFileOpenOption, ZFFileOpenOption, Append)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFFileOpenOption, ZFFileOpenOption)

#endif // #if ZF_ENV_sys_Android

