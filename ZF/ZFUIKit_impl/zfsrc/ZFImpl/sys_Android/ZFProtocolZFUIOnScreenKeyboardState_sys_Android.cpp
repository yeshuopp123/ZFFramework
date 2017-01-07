/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIOnScreenKeyboardState.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIOnScreenKeyboardState ZFImpl_sys_Android_JNI_ID(ZFUIKit_ZFUIOnScreenKeyboardState)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIOnScreenKeyboardState ZFImpl_sys_Android_JNI_NAME(ZFUIKit.ZFUIOnScreenKeyboardState)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIOnScreenKeyboardStateImpl_sys_Android, ZFUIOnScreenKeyboardState, ZFProtocolLevel::e_SystemNormal)
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIOnScreenKeyboardState).c_str());
        this->jclsOwner = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsOwner);
        zfsuper::protocolOnDealloc();
    }

public:
    virtual zfbool keyboardShowing(ZF_IN ZFUIOnScreenKeyboardState *keyboardState)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_keyboardShowing"),
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, this->jclsOwner, jmId);
    }
    virtual ZFUIRect keyboardFrame(ZF_IN ZFUIOnScreenKeyboardState *keyboardState)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_keyboardFrame"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
            ).c_str());
        jobject jrect = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
        ZFUIRect ret = ZFImpl_sys_Android_ZFUIRectFromZFAndroidRect(jrect);
        JNIUtilDeleteLocalRef(jniEnv, jrect);
        return ret;
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIOnScreenKeyboardStateImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIOnScreenKeyboardStateImpl_sys_Android)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIOnScreenKeyboardStateImpl_sys_Android_init, ZFLevelZFFrameworkPostNormal)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();

    jclass jclsOwner = (jclass)JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIOnScreenKeyboardState).c_str());
    JNIBlockedDeleteLocalRef((jobject)jclsOwner);

    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jclsOwner, zfTextA("native_keyboardStaticInit"),
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
        ).c_str());
    JNIUtilCallStaticVoidMethod(jniEnv, jclsOwner, jmId);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardStateImpl_sys_Android_init)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();

    jclass jclsOwner = (jclass)JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIOnScreenKeyboardState).c_str());
    JNIBlockedDeleteLocalRef((jobject)jclsOwner);

    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jclsOwner, zfTextA("native_keyboardStaticCleanup"),
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
        ).c_str());
    JNIUtilCallStaticVoidMethod(jniEnv, jclsOwner, jmId);
}
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardStateImpl_sys_Android_init)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIOnScreenKeyboardState, native_1notifyKeyboardStateOnChange,
                   JNIEnv *jniEnv, jclass jniCls)
{
    if(ZFFrameworkStateCheck() != ZFFrameworkStateAvailable)
    {
        return ;
    }
    ZFPROTOCOL_ACCESS(ZFUIOnScreenKeyboardState)->notifyKeyboardStateOnChange(
        ZFUIOnScreenKeyboardState::instanceForSysWindow());
}

#endif // #if ZF_ENV_sys_Android

