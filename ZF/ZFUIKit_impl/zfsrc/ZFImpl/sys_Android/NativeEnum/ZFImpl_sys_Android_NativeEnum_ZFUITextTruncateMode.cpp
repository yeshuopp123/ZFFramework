/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFUIKit/ZFUITypeDef.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUITextTruncateMode ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFUITextTruncateMode)
#define ZFImpl_sys_Android_JNI_NAME_ZFUITextTruncateMode ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFUITextTruncateMode)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextTruncateMode, ZFUITextTruncateMode, Disable)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextTruncateMode, ZFUITextTruncateMode, Head)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextTruncateMode, ZFUITextTruncateMode, Middle)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextTruncateMode, ZFUITextTruncateMode, Tail)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFUITextTruncateMode, ZFUITextTruncateMode)

#endif // #if ZF_ENV_sys_Android

