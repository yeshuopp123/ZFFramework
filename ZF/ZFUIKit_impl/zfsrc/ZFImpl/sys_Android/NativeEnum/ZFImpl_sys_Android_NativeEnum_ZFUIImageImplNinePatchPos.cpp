/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUIImageImplNinePatchPos ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIKit_ZFUIImageImplNinePatchPos)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIImageImplNinePatchPos ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIKit.ZFUIImageImplNinePatchPos)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIImageImplNinePatchPos, ZFUIImageImplNinePatchPos, TopLeft)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIImageImplNinePatchPos, ZFUIImageImplNinePatchPos, TopCenter)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIImageImplNinePatchPos, ZFUIImageImplNinePatchPos, TopRight)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIImageImplNinePatchPos, ZFUIImageImplNinePatchPos, CenterLeft)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIImageImplNinePatchPos, ZFUIImageImplNinePatchPos, CenterCenter)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIImageImplNinePatchPos, ZFUIImageImplNinePatchPos, CenterRight)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIImageImplNinePatchPos, ZFUIImageImplNinePatchPos, BottomLeft)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIImageImplNinePatchPos, ZFUIImageImplNinePatchPos, BottomCenter)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIImageImplNinePatchPos, ZFUIImageImplNinePatchPos, BottomRight)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFUIImageImplNinePatchPos, ZFUIImageImplNinePatchPos)

#endif // #if ZF_ENV_sys_Android

