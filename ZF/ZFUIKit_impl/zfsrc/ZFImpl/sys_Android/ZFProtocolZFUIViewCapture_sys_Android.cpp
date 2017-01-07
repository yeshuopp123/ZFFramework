/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewCapture.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIViewCapture ZFImpl_sys_Android_JNI_ID(ZFUIKit_ZFUIViewCapture)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIViewCapture ZFImpl_sys_Android_JNI_NAME(ZFUIKit.ZFUIViewCapture)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewCaptureImpl_sys_Android, ZFUIViewCapture, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:View"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("Android:View"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, zfText("Android:BitmapDrawable"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIViewCapture).c_str());
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
    virtual zfbool viewCapture(ZF_IN ZFUIView *view,
                               ZF_IN_OUT ZFUIImage *image)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_viewCapture"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, view->nativeView()));
        image->nativeImageSet(tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return zftrue;
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewCaptureImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewCaptureImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android

