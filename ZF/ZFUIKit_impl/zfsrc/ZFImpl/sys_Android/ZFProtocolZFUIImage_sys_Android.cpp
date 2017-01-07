/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIImage ZFImpl_sys_Android_JNI_ID(ZFUIKit_ZFUIImage)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIImage ZFImpl_sys_Android_JNI_NAME(ZFUIKit.ZFUIImage)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageImpl_sys_Android, ZFUIImage, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:BitmapDrawable"))
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIImage).c_str());
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
    virtual void *nativeImageFromInput(ZF_IN const ZFInputCallback &inputCallback)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeImageFromInput"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        ZFBuffer buf = ZFInputCallbackReadToBuffer(inputCallback);
        if(buf.buffer() == zfnull)
        {
            return zfnull;
        }
        jobject byteBuf = ZFImpl_sys_Android_ZFAndroidBufferToJava(buf.buffer(), buf.bufferSize());
        JNIBlockedDeleteLocalRef(byteBuf);

        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId, byteBuf);
        if(tmp == NULL)
        {
            return zfnull;
        }
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ret;
    }
    virtual zfbool nativeImageToOutput(ZF_IN void *nativeImage,
                                       ZF_OUT const ZFOutputCallback &outputCallback)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeImageToOutput"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject jobjBuffer = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId, ZFCastStatic(jobject, nativeImage));
        if(jobjBuffer == zfnull)
        {
            return zffalse;
        }
        ZFImpl_sys_Android_Buffer buffer = ZFImpl_sys_Android_ZFAndroidBufferFromJava(jobjBuffer);
        if(buffer.buffer == zfnull)
        {
            return zffalse;
        }
        zfindex written = outputCallback.execute(buffer.buffer, buffer.bufferSize);
        JNIUtilDeleteLocalRef(jniEnv, jobjBuffer);
        return (written == buffer.bufferSize);
    }

    virtual void *nativeImageCopy(ZF_IN void *nativeImage)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeImageCopy"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId, ZFCastStatic(jobject, nativeImage));
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ret;
    }

    virtual void *nativeImageRetain(ZF_IN void *nativeImage)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        return JNIUtilNewGlobalRef(jniEnv, ZFCastStatic(jobject, nativeImage));
    }
    virtual void nativeImageRelease(ZF_IN void *nativeImage)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, ZFCastStatic(jobject, nativeImage));
    }

    virtual ZFUISize nativeImageSize(ZF_IN void *nativeImage)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeImageSize"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject jobjSize = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId, ZFCastStatic(jobject, nativeImage));
        JNIBlockedDeleteLocalRef(jobjSize);
        return ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(jobjSize);
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIImageImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

