/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFFileCallbackDef_ZFIOBridgeCallbackUsingTmpFile.h
 * @brief callback for file
 */

#ifndef _ZFI_ZFFileCallbackDef_ZFIOBridgeCallbackUsingTmpFile_h_
#define _ZFI_ZFFileCallbackDef_ZFIOBridgeCallbackUsingTmpFile_h_

#include "ZFFileCallbackDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFIOBridgeCallbackUsingTmpFile
zfclassFwd _ZFP_ZFIOBridgeCallbackUsingTmpFilePrivate;
/**
 * @brief see #ZFIOBridgeCallbackAbs
 */
zfclassLikePOD ZF_ENV_EXPORT ZFIOBridgeCallbackUsingTmpFile : zfextendsLikePOD ZFIOBridgeCallbackAbs
{
public:
    /** @cond ZFPrivateDoc */
    ZFIOBridgeCallbackUsingTmpFile(void);
    ZFIOBridgeCallbackUsingTmpFile(ZF_IN const ZFIOBridgeCallbackUsingTmpFile &ref);
    ZFIOBridgeCallbackUsingTmpFile &operator =(ZF_IN const ZFIOBridgeCallbackUsingTmpFile &ref);
    /** @endcond */
    virtual ~ZFIOBridgeCallbackUsingTmpFile(void);

public:
    zfoverride
    virtual ZFInputCallback inputCallback(void);
    zfoverride
    virtual ZFOutputCallback outputCallback(void);

public:
    zfoverride
    virtual void resetInput(void);
    zfoverride
    virtual void resetOutput(void);

private:
    _ZFP_ZFIOBridgeCallbackUsingTmpFilePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFileCallbackDef_ZFIOBridgeCallbackUsingTmpFile_h_

