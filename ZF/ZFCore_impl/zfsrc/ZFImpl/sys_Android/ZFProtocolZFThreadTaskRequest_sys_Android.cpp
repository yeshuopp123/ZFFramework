/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFThreadTaskRequest.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFThreadTaskRequest ZFImpl_sys_Android_JNI_ID(ZFCore_ZFThreadTaskRequest)
#define ZFImpl_sys_Android_JNI_NAME_ZFThreadTaskRequest ZFImpl_sys_Android_JNI_NAME(ZFCore.ZFThreadTaskRequest)

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadTaskRequestImpl_sys_Android, ZFThreadTaskRequest, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:Handler"))

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFThreadTaskRequest).c_str());
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
    virtual void taskRequest(ZF_IN const ZFListener &task,
                             ZF_IN ZFObject *param0,
                             ZF_IN ZFObject *param1)
    {
        ZFListenerHolder *listenerData = zfAllocWithoutLeakTest(ZFListenerHolder, task, ZFListenerData(zfidentityInvalid, zfnull, param0, param1));
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_taskRequest"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            JNIConvertZFObjectToJNIType(jniEnv, listenerData));
    }
private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadTaskRequestImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFThreadTaskRequestImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFThreadTaskRequest, native_1doExecute,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerToken)
{
    ZFListenerHolder *listenerData = ZFCastZFObject(ZFListenerHolder *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerToken));
    listenerData->runnableExecute();
    zfReleaseWithoutLeakTest(listenerData);
}

#endif // #if ZF_ENV_sys_Android

