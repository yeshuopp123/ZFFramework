/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFAlgorithm/ZFRegExp.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFRegExpOption ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFAlgorithm_ZFRegExpOption)
#define ZFImpl_sys_Android_JNI_NAME_ZFRegExpOption ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFAlgorithm.ZFRegExpOption)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFRegExpOption, ZFRegExpOption, None)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFRegExpOption, ZFRegExpOption, IgnoreCase)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFRegExpOption, ZFRegExpOption, SingleLine)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFRegExpOption, ZFRegExpOption, MultiLine)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFRegExpOption, ZFRegExpOption)

#endif // #if ZF_ENV_sys_Android

