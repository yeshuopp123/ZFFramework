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

#define ZFImpl_sys_Android_JNI_ID_ZFXmlType ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFAlgorithm_ZFXmlType)
#define ZFImpl_sys_Android_JNI_NAME_ZFXmlType ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFAlgorithm.ZFXmlType)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFXmlType, ZFXmlType, XmlNull)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFXmlType, ZFXmlType, XmlElement)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFXmlType, ZFXmlType, XmlAttribute)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFXmlType, ZFXmlType, XmlText)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFXmlType, ZFXmlType, XmlComment)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFXmlType, ZFXmlType, XmlDocument)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFXmlType, ZFXmlType, XmlDeclaration)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFXmlType, ZFXmlType, XmlDOCTYPE)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFXmlType, ZFXmlType, XmlProcessingInstruction)

ZFImpl_sys_Android_NativeEnum_EXPAND_Default(ZFImpl_sys_Android_JNI_ID_ZFXmlType, ZFXmlType)

#endif // #if ZF_ENV_sys_Android

