/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFFilePath.h"

#if ZF_ENV_sys_iOS
#import <Foundation/Foundation.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFilePathImpl_sys_iOS, ZFFilePath, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:SandboxPath"))
public:
    virtual const zfchar *modulePath(void)
    {
        if(this->_modulePath.isEmpty())
        {
            (void)this->moduleFilePath();
            zfindex pos = zfstringFindReversely(this->_moduleFilePath, ZFFile::fileSeparator);
            if(pos != zfindexMax)
            {
                this->_modulePath.assign(this->_moduleFilePath, 0, pos);
            }
        }
        return this->_modulePath;
    }
    virtual const zfchar *moduleFilePath(void)
    {
        if(this->_moduleFilePath.isEmpty())
        {
            zfstring tmp;
            ZFImpl_sys_iOS_zfstringFromNSString(tmp, [[NSBundle mainBundle] bundlePath]);
            ZFFile::filePathFormat(this->_moduleFilePath, tmp.cString());
        }
        return this->_moduleFilePath;
    }

    virtual const zfchar *settingPath(void)
    {
        if(this->_settingPath.isEmpty())
        {
            this->_settingPath = this->nativeDocumentPath();
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
            this->_storagePath = this->nativeDocumentPath();
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
            this->_storageSharedPath = this->nativeDocumentPath();
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
            this->_cachePath = this->nativeCachePath();
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
    zfstring _nativeDocumentPath;
    const zfchar *nativeDocumentPath(void)
    {
        if(_nativeDocumentPath.isEmpty())
        {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            zfstring tmp;
            ZFImpl_sys_iOS_zfstringFromNSString(tmp, (NSString *)[paths objectAtIndex:0]);
            ZFFile::filePathFormat(_nativeDocumentPath, tmp.cString());
        }
        return _nativeDocumentPath;
    }
    zfstring _nativeCachePath;
    const zfchar *nativeCachePath(void)
    {
        if(_nativeCachePath.isEmpty())
        {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
            zfstring tmp;
            ZFImpl_sys_iOS_zfstringFromNSString(tmp, (NSString *)[paths objectAtIndex:0]);
            ZFFile::filePathFormat(_nativeCachePath, tmp.cString());
        }
        return _nativeCachePath;
    }

private:
    zfstring _modulePath;
    zfstring _moduleFilePath;
    zfstring _settingPath;
    zfstring _storagePath;
    zfstring _storageSharedPath;
    zfstring _cachePath;
ZFPROTOCOL_IMPLEMENTATION_END(ZFFilePathImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFilePathImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS

