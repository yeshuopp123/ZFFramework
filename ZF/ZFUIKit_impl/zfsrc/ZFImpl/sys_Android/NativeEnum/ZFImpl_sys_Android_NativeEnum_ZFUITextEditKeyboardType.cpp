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

#define ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardType ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFUITextEditKeyboardType)
#define ZFImpl_sys_Android_JNI_NAME_ZFUITextEditKeyboardType ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFUITextEditKeyboardType)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardType, ZFUITextEditKeyboardType, Normal)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardType, ZFUITextEditKeyboardType, CharBased)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardType, ZFUITextEditKeyboardType, PhonePad)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardType, ZFUITextEditKeyboardType, NumberPad)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardType, ZFUITextEditKeyboardType)

#endif // #if ZF_ENV_sys_Android

