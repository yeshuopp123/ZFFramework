/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFUIKit/ZFUITextEdit.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnType ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFUITextEditKeyboardReturnType)
#define ZFImpl_sys_Android_JNI_NAME_ZFUITextEditKeyboardReturnType ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFUITextEditKeyboardReturnType)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnType, ZFUITextEditKeyboardReturnType, Normal)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnType, ZFUITextEditKeyboardReturnType, Next)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnType, ZFUITextEditKeyboardReturnType, Search)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnType, ZFUITextEditKeyboardReturnType, Done)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnType, ZFUITextEditKeyboardReturnType, Go)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnType, ZFUITextEditKeyboardReturnType, Send)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnType, ZFUITextEditKeyboardReturnType)

#endif // #if ZF_ENV_sys_Android

