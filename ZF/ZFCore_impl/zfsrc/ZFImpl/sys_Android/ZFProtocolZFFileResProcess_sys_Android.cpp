/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFFileResProcess.h"
#include "ZFCore/ZFString.h"
#include "ZFImpl/sys_Android/ZFMainEntry_sys_Android.h"

#if ZF_ENV_sys_Android
#define _ZFP_ZFProtocolZFFileResProcess_sys_Android_autoPostfix zfText(".mp2")
// need Android 2.3 or above for asset RW for JNI
// need -landroid
// all resource files are located under "project_dir/assets/zfres/"
//
// due to the limitation of Android asset (non-media file type must not exceeds 1M bytes),
// we introduced _ZFP_ZFProtocolZFFileResProcess_sys_Android_autoPostfix,
// which is a media file type's extension
// all res files with this extension would have its file extension to be removed
// (e.g. "file.ext.mp2" would have "file.ext" as the final file name,
// if no such extension found, the original file name would be used)
// so, if you have res file that exceed 1M bytes,
// you may simply append ".mp2" as file extension
// and access as normal
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

ZF_NAMESPACE_GLOBAL_BEGIN
#define ZFImpl_sys_Android_JNI_ID_ZFFileResProcess ZFImpl_sys_Android_JNI_ID(ZFCore_ZFFileResProcess)
#define ZFImpl_sys_Android_JNI_NAME_ZFFileResProcess ZFImpl_sys_Android_JNI_NAME(ZFCore.ZFFileResProcess)

zfclassPOD _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken
{
public:
    AAssetManager *assetManager;
    AAsset *token;
    zfbool isEof;
    zfbool isError;
};

zfclassNotPOD _ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData
{
public:
    jobjectArray files; // jstring[]
    zfindex curFileIndex;
public:
    _ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData(void)
    : files(zfnull)
    , curFileIndex(0)
    {
    }
    virtual ~_ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData(void)
    {
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileResProcessImpl_sys_Android, ZFFileResProcess, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:AssetsFile"))
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFFileResProcess).c_str());
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
    virtual zfbool resCopy(ZF_IN const zfchar *resPath,
                           ZF_IN const zfchar *dstPath,
                           ZF_IN_OPT zfbool isRecursive = zftrue,
                           ZF_IN_OPT zfbool isForce = zffalse,
                           ZF_IN_OPT zfstring *errPos = zfnull)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_resCp"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
                .add(JNIType::S_boolean)
            ).c_str());
        jobject resPathJ = ZFImpl_sys_Android_zfstringToString(resPath);
        jobject errPosJ = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            JNILineDeleteLocalRefWithEnv(ZFImpl_sys_Android_zfstringToString(resPath), jniEnv),
            JNILineDeleteLocalRefWithEnv(ZFImpl_sys_Android_zfstringToString(dstPath), jniEnv),
            isRecursive,
            isForce);
        if(errPosJ == NULL)
        {
            return zftrue;
        }
        else
        {
            if(errPos != zfnull)
            {
                ZFImpl_sys_Android_zfstringFromString(*errPos, errPosJ);
            }
            return zffalse;
        }
    }

    // ============================================================
    // res RW
    virtual ZFFileToken resOpen(ZF_IN const zfchar *resPath)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = zfnew(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken);
        zfmemset(d, 0, sizeof(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken));
        d->assetManager = AAssetManager_fromJava(jniEnv, ZFImpl_sys_Android_assetManager());
        zfCoreAssert(d->assetManager != zfnull);

        d->token = AAssetManager_open(d->assetManager,
            ZFStringZ2A(this->resGetFilePath(resPath).cString()),
            AASSET_MODE_RANDOM);
        if(d->token == zfnull)
        {
            d->token = AAssetManager_open(d->assetManager,
                ZFStringZ2A(this->resGetFilePathWithoutFix(resPath).cString()),
                AASSET_MODE_RANDOM);
            if(d->token == zfnull)
            {
                zfdelete(d);
                d = ZFFileTokenInvalid;
            }
        }
        return d;
    }
    virtual zfbool resClose(ZF_IN ZFFileToken token)
    {
        if(token == ZFFileTokenInvalid)
        {
            return zffalse;
        }

        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *, token);
        AAsset_close(d->token);
        zfdelete(d);
        return zftrue;
    }
    virtual zfindex resTell(ZF_IN ZFFileToken token)
    {
        if(token == ZFFileTokenInvalid)
        {
            return zfindexMax;
        }
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *, token);
        return (AAsset_getLength(d->token) - AAsset_getRemainingLength(d->token));
    }
    virtual zfbool resSeek(ZF_IN ZFFileToken token,
                           ZF_IN zfindex byteSize,
                           ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin)
    {
        if(token == ZFFileTokenInvalid)
        {
            return zffalse;
        }
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *, token);
        zfint seekPos = SEEK_SET;
        long seekSize = (long)byteSize;
        switch(position)
        {
            case ZFSeekPosBegin:
                seekPos = SEEK_SET;
                break;
            case ZFSeekPosCur:
                seekPos = SEEK_CUR;
                break;
            case ZFSeekPosCurReversely:
                seekPos = SEEK_CUR;
                seekSize = -seekSize;
                break;
            case ZFSeekPosEnd:
                seekPos = SEEK_END;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return (AAsset_seek(d->token, seekSize, seekPos) != -1);
    }
    virtual zfindex resRead(ZF_IN ZFFileToken token,
                            ZF_IN void *buf,
                            ZF_IN zfindex maxByteSize)
    {
        if(token == ZFFileTokenInvalid || maxByteSize == 0)
        {
            return 0;
        }
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *, token);
        if(d->isEof || d->isError)
        {
            return 0;
        }
        zfint ret = AAsset_read(d->token, buf, maxByteSize);
        if(ret < 0)
        {
            d->isError = zftrue;
            ret = 0;
        }
        else if(ret == 0 || ret < maxByteSize)
        {
            d->isEof = zftrue;
        }
        return ret;
    }
    virtual zfbool resEof(ZF_IN ZFFileToken token)
    {
        if(token == ZFFileTokenInvalid)
        {
            return zffalse;
        }
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *, token);
        return d->isEof;
    }
    virtual zfbool resError(ZF_IN ZFFileToken token)
    {
        if(token == ZFFileTokenInvalid)
        {
            return zffalse;
        }
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *, token);
        return d->isError;
    }

    // ============================================================
    // res find
    virtual zfbool resFindFirst(ZF_IN const zfchar *resPath,
                                ZF_IN_OUT ZFFileFindDataContainer &fd)
    {
        fd.parentPath = resPath;
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData *d = zfnew(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData);
        fd.nativeFd = d;
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_listAssets"),
            JNIGetMethodSig(JNIType::S_array(JNIType::S_object(zfTextA("java.lang.String"))), JNIParamTypeContainer()
                .add(JNIType::S_object(zfTextA("java.lang.String")))
            ).c_str());
        jstring param = JNIUtilNewStringUTF(jniEnv, ZFString::toUTF8(
            this->resGetDirPath(fd.parentPath.cString()).cString(),
            ZFStringEncodingForZFChar).cString());
        JNIBlockedDeleteLocalRef(param);
        jobjectArray files = (jobjectArray)JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId, param);
        JNIBlockedDeleteLocalRef(files);
        zfbool success = zffalse;
        if(files != zfnull)
        {
            d->files = (jobjectArray)JNIUtilNewGlobalRef(jniEnv, files);
            success = this->resFindNext(fd);
        }
        if(!success)
        {
            this->resFindClose(fd);
        }
        return success;
    }
    virtual zfbool resFindNext(ZF_IN_OUT ZFFileFindDataContainer &fd)
    {
        if(fd.nativeFd == zfnull)
        {
            return zffalse;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData *, fd.nativeFd);
        if(d->curFileIndex >= JNIUtilGetArrayLength(jniEnv, d->files))
        {
            return zffalse;
        }
        jstring jsPath = (jstring)JNIUtilGetObjectArrayElement(jniEnv, d->files, d->curFileIndex);
        ++d->curFileIndex;
        const zfcharA *sName = JNIUtilGetStringUTFChars(jniEnv, jsPath, zfnull);
        zfstring refName = ZFStringA2Z(sName);
        JNIUtilReleaseStringUTFChars(jniEnv, jsPath, sName);
        zfstring absPath = this->resGetDirPath(zfsConnectLineFree(fd.parentPath.cString(), ZFFile::fileSeparatorString, refName.cString()));

        AAsset *asset = AAssetManager_open(
            AAssetManager_fromJava(jniEnv, ZFImpl_sys_Android_assetManager()),
            ZFStringZ2A(absPath.cString()),
            AASSET_MODE_STREAMING);
        if(asset == zfnull)
        {
            fd.fileIsFolder = zftrue;
            ZFFile::fileParentPathOf(fd.parentPath, refName.cString());
            ZFFile::fileNameOf(fd.name, refName.cString());
        }
        else
        {
            AAsset_close(asset);
            fd.fileIsFolder = zffalse;
            ZFFile::fileParentPathOf(fd.parentPath, refName.cString());
            if(zfstringFindReversely(refName, _ZFP_ZFProtocolZFFileResProcess_sys_Android_autoPostfix)
                == refName.length() - zfslen(_ZFP_ZFProtocolZFFileResProcess_sys_Android_autoPostfix))
            {
                fd.name = ZFFile::fileNameOfWithoutExt(refName.cString());
            }
            else
            {
                fd.name = refName;
            }
        }
        fd.path.removeAll();
        if(fd.parentPath.length() > 0)
        {
            fd.path += fd.parentPath;
            fd.path += ZFFile::fileSeparator;
        }
        fd.path += fd.name;

        return zftrue;
    }
    virtual void resFindClose(ZF_IN_OUT ZFFileFindDataContainer &fd)
    {
        if(fd.nativeFd == zfnull)
        {
            return ;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData *, fd.nativeFd);
        if(d->files != zfnull)
        {
            JNIUtilDeleteGlobalRef(jniEnv, d->files);
            d->files = zfnull;
        }
        zfdelete(d);
        fd.nativeFd = zfnull;
    }

private:
    jclass jclsOwner;
    zfstring resGetDirPath(ZF_IN const zfchar *src)
    {
        zfindex srcLen = ((src == zfnull) ? 0 : zfslen(src));
        zfstring ret(srcLen + 8);
        ret += zfText("zfres");
        if(srcLen > 0)
        {
            ret += ZFFile::fileSeparator;
            ret += src;
        }
        return ret;
    }
    zfstring resGetFilePathWithoutFix(ZF_IN const zfchar *src)
    {
        zfindex srcLen = ((src == zfnull) ? 0 : zfslen(src));
        zfstring ret(srcLen + 16);
        ret += zfText("zfres");
        if(srcLen > 0)
        {
            ret += ZFFile::fileSeparator;
            ret += src;
        }
        return ret;
    }
    zfstring resGetFilePath(ZF_IN const zfchar *src)
    {
        zfindex srcLen = ((src == zfnull) ? 0 : zfslen(src));
        zfstring ret(srcLen + 16);
        ret += zfText("zfres");
        if(srcLen > 0)
        {
            ret += ZFFile::fileSeparator;
            ret += src;
        }
        ret += _ZFP_ZFProtocolZFFileResProcess_sys_Android_autoPostfix;
        return ret;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileResProcessImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFileResProcessImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif

