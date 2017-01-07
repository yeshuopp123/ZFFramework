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

#define ZFImpl_sys_Android_JNI_ID_ZFFilterCallbackResult ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFCore_ZFFilterCallbackResult)
#define ZFImpl_sys_Android_JNI_NAME_ZFFilterCallbackResult ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFCore.ZFFilterCallbackResult)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFFilterCallbackResult, ZFFilterCallbackResultNotSpecified)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFFilterCallbackResult, ZFFilterCallbackResultActive)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFFilterCallbackResult, ZFFilterCallbackResultNotActive)

#endif // #if ZF_ENV_sys_Android

