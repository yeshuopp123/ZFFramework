/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFFileResProcess.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_iOS
/*
 * res files must be placed at "YourApp.app/zfres/"
 *
 * recommended to use "Run Script" under "Build Phases" of XCode:
 * @code
 *   cp -rf "relative_path_to_res/." "${CONFIGURATION_BUILD_DIR}/${CONTENTS_FOLDER_PATH}/zfres/" >/dev/null 2>&1
 * @endcode
 */
#import <Foundation/Foundation.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileResProcessImpl_sys_iOS, ZFFileResProcess, ZFProtocolLevel::e_SystemNormal)
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        NSString *resRootDirTmp = [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:@"zfres"];
        ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
        this->_resRootPath = resRootDirTmp.UTF8String;
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
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileResProcessImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFileResProcessImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif

