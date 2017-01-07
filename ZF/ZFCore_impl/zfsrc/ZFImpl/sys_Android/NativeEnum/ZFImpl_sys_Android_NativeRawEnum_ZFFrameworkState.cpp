/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFCore/ZFObject.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFFrameworkState ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFCore_ZFFrameworkState)
#define ZFImpl_sys_Android_JNI_NAME_ZFFrameworkState ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFCore.ZFFrameworkState)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFFrameworkState, ZFFrameworkStateNotAvailable)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFFrameworkState, ZFFrameworkStateInitProcessing)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFFrameworkState, ZFFrameworkStateAvailable)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFFrameworkState, ZFFrameworkStateCleanupProcessing)

#endif // #if ZF_ENV_sys_Android

