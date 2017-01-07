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

#define ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnAction ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFUITextEditKeyboardReturnAction)
#define ZFImpl_sys_Android_JNI_NAME_ZFUITextEditKeyboardReturnAction ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFUITextEditKeyboardReturnAction)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnAction, ZFUITextEditKeyboardReturnAction, None)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnAction, ZFUITextEditKeyboardReturnAction, FocusNext)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnAction, ZFUITextEditKeyboardReturnAction, HideKeyboard)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFUITextEditKeyboardReturnAction, ZFUITextEditKeyboardReturnAction)

#endif // #if ZF_ENV_sys_Android

