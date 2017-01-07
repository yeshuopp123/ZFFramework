/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFFilePath.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
    #include <unistd.h>
#endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFilePathImpl_default, ZFFilePath, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("ModuleLocalPath"))
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
            zfstring tmp;
            #if ZF_ENV_sys_Windows
                zfcharW buf[1024] = {0};
                ::GetModuleFileNameW(zfnull, buf, 1024);
                ZFString::toZFChar(tmp, buf, ZFStringEncodingForZFCharW);
            #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
                zfcharA buf[1024] = {0};
                zfint len = (zfint)::readlink(zfTextA("/proc/self/exe"), buf, 1024);
                if(len > 0)
                {
                    buf[len] = '\0';
                }
                else
                {
                    zfscpyA(buf, zfstringWithFormat(zfTextA("./unknown")).cString());
                }
                ZFString::toZFChar(tmp, buf, ZFStringEncoding::e_UTF8);
            #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
            zfbool result = ZFFile::filePathFormat(this->_moduleFilePath, tmp.cString());
            zfCoreAssert(result);
        }
        return this->_moduleFilePath;
    }

    virtual const zfchar *settingPath(void)
    {
        if(this->_settingPath.isEmpty())
        {
            this->_settingPath = this->modulePath();
            this->_settingPath += ZFFile::fileSeparator;
            this->_settingPath += zfText("zfsetting");
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
            this->_storagePath = this->modulePath();
            this->_storagePath += ZFFile::fileSeparator;
            this->_storagePath += zfText("zfstorage");
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
            this->_storageSharedPath = this->modulePath();
            this->_storageSharedPath += ZFFile::fileSeparator;
            this->_storageSharedPath += zfText("zfstorageshared");
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
            this->_cachePath = this->modulePath();
            this->_cachePath += ZFFile::fileSeparator;
            this->_cachePath += zfText("zfcache");
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
    zfstring _modulePath;
    zfstring _moduleFilePath;
    zfstring _settingPath;
    zfstring _storagePath;
    zfstring _storageSharedPath;
    zfstring _cachePath;
ZFPROTOCOL_IMPLEMENTATION_END(ZFFilePathImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFilePathImpl_default)

ZF_NAMESPACE_GLOBAL_END

