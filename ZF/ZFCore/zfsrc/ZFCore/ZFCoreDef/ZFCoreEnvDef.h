/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreEnvDef.h
 * @brief used to define the env used by ZFFramework
 */
#ifndef _ZFI_ZFCoreEnvDef_h_
#define _ZFI_ZFCoreEnvDef_h_

#include "ZFNamespaceDef.h"
#include "ZFCoreEnvConfig.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief dummy macro to show the code is env sensitive
 */
#define ZF_ENV_SENSITIVE

// ============================================================
/**
 * @brief true to enable RTTI, false by default
 *
 * add -DZF_ENV_RTTI=1 to compiler to make this macro take effect
 */
#ifndef ZF_ENV_RTTI
    #define ZF_ENV_RTTI 0
#endif

// ============================================================
/**
 * @brief used to export symbols
 *
 * for example, add -DZF_ENV_EXPORT=__declspec(dllexport)
 * to export symbols under Windows
 */
#ifndef ZF_ENV_EXPORT
    #define ZF_ENV_EXPORT
#endif

// ============================================================
/**
 * @brief whether in debug mode
 *
 * add -DZF_ENV_DEBUG=1 to compiler to make this macro take effect
 */
#ifndef ZF_ENV_DEBUG
    #if defined(DEBUG) || defined(_DEBUG)
        #define ZF_ENV_DEBUG 1
    #else
        #define ZF_ENV_DEBUG 0
    #endif
#endif

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreEnvDef_h_

