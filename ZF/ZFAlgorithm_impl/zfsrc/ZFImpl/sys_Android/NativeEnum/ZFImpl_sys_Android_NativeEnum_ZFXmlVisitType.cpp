/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFAlgorithm/ZFXml.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFXmlVisitType ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFAlgorithm_ZFXmlVisitType)
#define ZFImpl_sys_Android_JNI_NAME_ZFXmlVisitType ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFAlgorithm.ZFXmlVisitType)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFXmlVisitType, ZFXmlVisitType, Enter)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFXmlVisitType, ZFXmlVisitType, Exit)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFXmlVisitType, ZFXmlVisitType, ExitChildren)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFXmlVisitType, ZFXmlVisitType)

#endif // #if ZF_ENV_sys_Android

