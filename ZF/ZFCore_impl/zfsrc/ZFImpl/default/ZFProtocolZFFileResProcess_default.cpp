/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFFileResProcess.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// res files must be placed at "zfres/" in the same folder of your executable

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileResProcessImpl_default, ZFFileResProcess, ZFProtocolLevel::e_Default)
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        this->_resRootPath = ZFFile::modulePath();
        this->_resRootPath += ZFFile::fileSeparator;
        #if ZF_ENV_sys_MacOS
            this->_resRootPath += zfText("..");
            this->_resRootPath += ZFFile::fileSeparator;
            this->_resRootPath += zfText("Resources");
            this->_resRootPath += ZFFile::fileSeparator;
        #endif
        this->_resRootPath += zfText("zfres");
        this->_resRootPath += ZFFile::fileSeparator;
    }

public:
    zfoverride
    virtual const zfstring &resRootPath(void)
    {
        return this->_resRootPath;
    }
private:
    zfstring _resRootPath;
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileResProcessImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFileResProcessImpl_default)

ZF_NAMESPACE_GLOBAL_END

