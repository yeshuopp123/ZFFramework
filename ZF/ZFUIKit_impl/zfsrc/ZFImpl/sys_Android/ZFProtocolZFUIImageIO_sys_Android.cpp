/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageIO.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIImageIO ZFImpl_sys_Android_JNI_ID(ZFUIKit_ZFUIImageIO)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIImageIO ZFImpl_sys_Android_JNI_NAME(ZFUIKit.ZFUIImageIO)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageIOImpl_sys_Android, ZFUIImageIO, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:BitmapDrawable"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, zfText("Android:BitmapDrawable"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIImageIO).c_str());
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
    virtual void *imageApplyScale(ZF_IN zffloat imageScale,
                                  ZF_IN void *nativeImage,
                                  ZF_IN const ZFUISize &newSize,
                                  ZF_IN const ZFUIMargin &ninePatch)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_imageApplyScale"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_float)
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            imageScale,
            ZFCastStatic(jobject, nativeImage),
            JNILineDeleteLocalRef(ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(newSize)),
            JNILineDeleteLocalRef(ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(ninePatch)));
        JNIBlockedDeleteLocalRefWithEnv(tmp, jniEnv);
        return JNIUtilNewGlobalRef(jniEnv, tmp);
    }
    virtual void *imageLoadFromColor(ZF_IN zffloat imageScale,
                                     ZF_IN const ZFUIColor &color,
                                     ZF_IN const ZFUISize &size)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_imageLoadFromColor"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_int)
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            (jint)color,
            JNILineDeleteLocalRef(ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(size)));
        JNIBlockedDeleteLocalRefWithEnv(tmp, jniEnv);
        return JNIUtilNewGlobalRef(jniEnv, tmp);
    }
private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageIOImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIImageIOImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

