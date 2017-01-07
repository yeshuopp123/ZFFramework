/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFFilePath.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN
#define ZFImpl_sys_Android_JNI_ID_ZFFilePath ZFImpl_sys_Android_JNI_ID(ZFCore_ZFFilePath)
#define ZFImpl_sys_Android_JNI_NAME_ZFFilePath ZFImpl_sys_Android_JNI_NAME(ZFCore.ZFFilePath)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFilePathImpl_sys_Android, ZFFilePath, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:FileSystem"))
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFFilePath).c_str());
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
    virtual const zfchar *modulePath(void)
    {
        if(this->_modulePath.isEmpty())
        {
            (void)this->moduleFilePath();
            zfindex pos = zfstringFindReversely(this->_moduleFilePath, ZFFile::fileSeparator);
            zfCoreAssert(pos != zfindexMax);
            this->_modulePath.assign(this->_moduleFilePath, 0, pos);
        }
        return this->_modulePath;
    }
    virtual const zfchar *moduleFilePath(void)
    {
        if(this->_moduleFilePath.isEmpty())
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_moduleFilePath"),
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
            ZFImpl_sys_Android_zfstringFromString(this->_moduleFilePath, path);
            zfCoreAssert(!this->_moduleFilePath.isEmpty());
        }
        return this->_moduleFilePath;
    }

    virtual const zfchar *settingPath(void)
    {
        if(this->_settingPath.isEmpty())
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_settingPath"),
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
            ZFImpl_sys_Android_zfstringFromString(this->_settingPath, path);
            zfCoreAssert(!this->_settingPath.isEmpty());
        }
        return this->_settingPath;
    }
    virtual void settingPathSet(ZF_IN const zfchar *path = zfnull)
    {
        this->_settingPath = path;
    }

    virtual const zfchar *storagePath(void)
    {
        if(this->_storagePath.isEmpty())
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_storagePath"),
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
            ZFImpl_sys_Android_zfstringFromString(this->_storagePath, path);
            zfCoreAssert(!this->_storagePath.isEmpty());
        }
        return this->_storagePath;
    }
    virtual void storagePathSet(ZF_IN const zfchar *path = zfnull)
    {
        this->_storagePath = path;
    }

    virtual const zfchar *storageSharedPath(void)
    {
        if(this->_storageSharedPath.isEmpty())
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_storageSharedPath"),
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
            ZFImpl_sys_Android_zfstringFromString(this->_storageSharedPath, path);
            zfCoreAssert(!this->_storageSharedPath.isEmpty());
        }
        return this->_storageSharedPath;
    }
    virtual void storageSharedPathSet(ZF_IN const zfchar *path = zfnull)
    {
        this->_storageSharedPath = path;
    }

    virtual const zfchar *cachePath(void)
    {
        if(this->_cachePath.isEmpty())
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_cachePath"),
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
            ZFImpl_sys_Android_zfstringFromString(this->_cachePath, path);
            zfCoreAssert(!this->_cachePath.isEmpty());
        }
        return this->_cachePath;
    }
    virtual void cachePathSet(ZF_IN const zfchar *path = zfnull)
    {
        this->_cachePath = path;
    }
    virtual void cachePathClear(void)
    {
        ZFFile::fileRemove(this->_cachePath, zfHint("isRecursive")zftrue, zfHint("isForce")zftrue);
    }

private:
    jclass jclsOwner;
private:
    zfstring _modulePath;
    zfstring _moduleFilePath;
    zfstring _settingPath;
    zfstring _storagePath;
    zfstring _storageSharedPath;
    zfstring _cachePath;
ZFPROTOCOL_IMPLEMENTATION_END(ZFFilePathImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFilePathImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif

