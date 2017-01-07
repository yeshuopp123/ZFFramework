/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFSemaphore.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFSemaphore ZFImpl_sys_Android_JNI_ID(ZFCore_ZFSemaphore)
#define ZFImpl_sys_Android_JNI_NAME_ZFSemaphore ZFImpl_sys_Android_JNI_NAME(ZFCore.ZFSemaphore)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFSemaphoreImpl_sys_Android, ZFSemaphore, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:Object_Semaphore"))
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFSemaphore).c_str());
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

    virtual void *nativeSemaphoreCreate(ZF_IN ZFSemaphore *semaphore)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetMethodID(jniEnv, this->jclsOwner, JNIConstructorName,
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
            ).c_str());
        jobject tmp = JNIUtilNewObject(jniEnv, this->jclsOwner, jmId);
        jobject nativeSemaphore = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ZFCastStatic(void *, nativeSemaphore);
    }
    virtual void nativeSemaphoreDestroy(ZF_IN ZFSemaphore *semaphore,
                                        ZF_IN void *nativeSemaphore)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, ZFCastStatic(jobject, nativeSemaphore));
    }

    virtual void semaphoreLock(ZF_IN ZFSemaphore *semaphore)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetMethodID(jniEnv, this->jclsOwner, zfTextA("native_semaphoreLock"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
            ).c_str());
        JNIUtilCallVoidMethod(jniEnv, ZFCastStatic(jobject, semaphore->nativeSemaphore()), jmId);
    }
    virtual void semaphoreUnlock(ZF_IN ZFSemaphore *semaphore)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetMethodID(jniEnv, this->jclsOwner, zfTextA("native_semaphoreUnlock"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
            ).c_str());
        JNIUtilCallVoidMethod(jniEnv, ZFCastStatic(jobject, semaphore->nativeSemaphore()), jmId);
    }

    virtual void semaphoreSignal(ZF_IN ZFSemaphore *semaphore)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetMethodID(jniEnv, this->jclsOwner, zfTextA("native_semaphoreSignal"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
            ).c_str());
        JNIUtilCallVoidMethod(jniEnv, ZFCastStatic(jobject, semaphore->nativeSemaphore()), jmId);
    }
    virtual void semaphoreBroadcast(ZF_IN ZFSemaphore *semaphore)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetMethodID(jniEnv, this->jclsOwner, zfTextA("native_semaphoreBroadcast"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
            ).c_str());
        JNIUtilCallVoidMethod(jniEnv, ZFCastStatic(jobject, semaphore->nativeSemaphore()), jmId);
    }
    virtual void semaphoreWait(ZF_IN ZFSemaphore *semaphore)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetMethodID(jniEnv, this->jclsOwner, zfTextA("native_semaphoreWait"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
            ).c_str());
        JNIUtilCallVoidMethod(jniEnv, ZFCastStatic(jobject, semaphore->nativeSemaphore()), jmId);
    }
    virtual zfbool semaphoreWait(ZF_IN ZFSemaphore *semaphore,
                                 ZF_IN const zftimet &miliSecsTimeout)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetMethodID(jniEnv, this->jclsOwner, zfTextA("native_semaphoreWait"),
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
                .add(JNIType::S_long)
            ).c_str());
        return (JNIUtilCallBooleanMethod(jniEnv, ZFCastStatic(jobject, semaphore->nativeSemaphore()), jmId, ZFCastStatic(jlong, miliSecsTimeout)) == JNI_TRUE);
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFSemaphoreImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFSemaphoreImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

