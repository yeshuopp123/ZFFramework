/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewPositionOnScreen.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIViewPositionOnScreen ZFImpl_sys_Android_JNI_ID(ZFUIKit_ZFUIViewPositionOnScreen)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIViewPositionOnScreen ZFImpl_sys_Android_JNI_NAME(ZFUIKit.ZFUIViewPositionOnScreen)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewPositionOnScreenImpl_sys_Android, ZFUIViewPositionOnScreen, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:View"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("Android:View"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIViewPositionOnScreen).c_str());
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
    virtual void viewPositionOnScreen(ZF_IN ZFUIView *view,
                                      ZF_OUT ZFUIRect &rect)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_viewPositionOnScreen"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, view->nativeView()));
        ZFImpl_sys_Android_ZFUIRectFromZFAndroidRect(rect, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewPositionOnScreenImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewPositionOnScreenImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android

