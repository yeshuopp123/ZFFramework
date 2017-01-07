/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFFilePath.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Qt
#include <QCoreApplication>
#include <QStandardPaths>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFilePathImpl_sys_Qt, ZFFilePath, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt::applicationDirPath"))
public:
    virtual const zfchar *modulePath(void)
    {
        if(this->_modulePath.isEmpty())
        {
            this->_modulePath = QCoreApplication::applicationDirPath().toStdString().c_str();
            zfCoreAssert(!this->_modulePath.isEmpty());
        }
        return this->_modulePath;
    }
    virtual const zfchar *moduleFilePath(void)
    {
        if(this->_moduleFilePath.isEmpty())
        {
            this->_moduleFilePath = QCoreApplication::applicationFilePath().toStdString().c_str();
            zfCoreAssert(!this->_moduleFilePath.isEmpty());
        }
        return this->_moduleFilePath;
    }

    virtual const zfchar *settingPath(void)
    {
        if(this->_settingPath.isEmpty())
        {
            ZFFile::filePathFormat(this->_settingPath, QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).toStdString().c_str());
            if(this->_settingPath.isEmpty())
            {
                this->_settingPath = this->modulePath();
            }
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
            ZFFile::filePathFormat(this->_storagePath, QStandardPaths::writableLocation(QStandardPaths::DataLocation).toStdString().c_str());
            if(this->_storagePath.isEmpty())
            {
                this->_storagePath = this->modulePath();
            }
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
            ZFFile::filePathFormat(this->_storageSharedPath, QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString().c_str());
            if(this->_storageSharedPath.isEmpty())
            {
                this->_storageSharedPath = this->modulePath();
                this->_storageSharedPath += ZFFile::fileSeparator;
                this->_storageSharedPath += zfText("zfstorage");
            }
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
            ZFFile::filePathFormat(this->_cachePath, QStandardPaths::writableLocation(QStandardPaths::CacheLocation).toStdString().c_str());
            if(this->_cachePath.isEmpty())
            {
                this->_cachePath = this->modulePath();
            }
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
ZFPROTOCOL_IMPLEMENTATION_END(ZFFilePathImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFilePathImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

