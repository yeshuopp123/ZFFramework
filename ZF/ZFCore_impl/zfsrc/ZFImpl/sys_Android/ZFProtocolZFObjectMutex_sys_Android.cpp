/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFObjectMutex.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFObjectMutex ZFImpl_sys_Android_JNI_ID(ZFCore_ZFObjectMutex)
#define ZFImpl_sys_Android_JNI_NAME_ZFObjectMutex ZFImpl_sys_Android_JNI_NAME(ZFCore.ZFObjectMutex)

zfclassNotPOD _ZFP_ZFObjectMutexImpl_sys_Android_MutexImpl : zfextendsNotPOD ZFObjectMutexImpl
{
public:
    jclass jclsOwner;
    jobject nativeMutex;
public:
    _ZFP_ZFObjectMutexImpl_sys_Android_MutexImpl(ZF_IN jclass jclsOwner)
    : ZFObjectMutexImpl()
    , jclsOwner(jclsOwner)
    , nativeMutex(NULL)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeMutexCreate"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
        this->nativeMutex = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    virtual ~_ZFP_ZFObjectMutexImpl_sys_Android_MutexImpl(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->nativeMutex);
    }
public:
    virtual void mutexImplLock(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_mutexLock"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId, this->nativeMutex);
    }
    virtual void mutexImplUnlock(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_mutexUnlock"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId, this->nativeMutex);
    }
    virtual zfbool mutexImplTryLock(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_mutexTryLock"),
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return (JNIUtilCallStaticBooleanMethod(jniEnv, this->jclsOwner, jmId, this->nativeMutex) == JNI_TRUE);
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFObjectMutexImpl_sys_Android, ZFObjectMutex, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:ReentrantLock"))

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFObjectMutex).c_str());
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

    virtual ZFObjectMutexImpl *nativeMutexCreate(void)
    {
        return zfnew(_ZFP_ZFObjectMutexImpl_sys_Android_MutexImpl, this->jclsOwner);
    }
    virtual void nativeMutexDestroy(ZF_IN ZFObjectMutexImpl *nativeMutex)
    {
        zfdelete(nativeMutex);
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFObjectMutexImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFObjectMutexImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

