/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZFImpl_sys_Android_NativeEnum_h_
#define _ZFI_ZFImpl_sys_Android_NativeEnum_h_

#include "ZFImpl_sys_Android_ZF.h"
#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_NativeEnum(clsId) ZFM_CAT(ZFM_CAT(ZFM_CAT(ZFImpl_sys_Android_ROOT_JNI_ID, _), NativeEnum_), clsId)
#define ZFImpl_sys_Android_JNI_NAME_NativeEnum(clsName) ZFImpl_sys_Android_ROOT_JNI_NAME ".NativeEnum." ZFM_TOSTRING_A(clsName)

#define ZFImpl_sys_Android_NativeRawEnum_EXPAND(EnumClassJniId, EnumValue) \
    _ZFP_ZFImpl_sys_Android_NativeRawEnum_EXPAND(EnumClassJniId, EnumValue)
#define _ZFP_ZFImpl_sys_Android_NativeRawEnum_EXPAND(EnumClassJniId, EnumValue) \
    JNI_METHOD_DECLARE(jint, EnumClassJniId, native_1##EnumValue, \
                       JNIEnv *jniEnv, jclass jniCls) \
    { \
        return EnumValue; \
    }

#define ZFImpl_sys_Android_NativeEnum_EXPAND(EnumClassJniId, EnumName, EnumValue) \
    JNI_METHOD_DECLARE(jint, EnumClassJniId, native_1e_1##EnumValue, \
                       JNIEnv *jniEnv, jclass jniCls) \
    { \
        return EnumName::e_##EnumValue; \
    }
#define ZFImpl_sys_Android_NativeEnum_EXPAND_Default(EnumClassJniId, EnumName) \
    JNI_METHOD_DECLARE(jint, EnumClassJniId, native_1EnumDefault, \
                       JNIEnv *jniEnv, jclass jniCls) \
    { \
        return EnumName::EnumDefault(); \
    }

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_NativeEnum_h_

