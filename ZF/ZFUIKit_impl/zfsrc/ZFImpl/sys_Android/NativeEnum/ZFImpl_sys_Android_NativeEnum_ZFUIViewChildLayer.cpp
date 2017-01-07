/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFUIKit/ZFUIWindow.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUIViewChildLayer ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFUIViewChildLayer)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIViewChildLayer ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFUIViewChildLayer)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIViewChildLayer, ZFUIViewChildLayer, Normal)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIViewChildLayer, ZFUIViewChildLayer, Impl)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIViewChildLayer, ZFUIViewChildLayer, Background)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIViewChildLayer, ZFUIViewChildLayer, Foreground)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFUIViewChildLayer, ZFUIViewChildLayer)

#endif // #if ZF_ENV_sys_Android

