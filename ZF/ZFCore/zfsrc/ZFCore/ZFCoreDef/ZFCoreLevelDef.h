/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreLevelDef.h
 * @brief level for logic which depends on order
 */
#ifndef _ZFI_ZFCoreLevelDef_h_
#define _ZFI_ZFCoreLevelDef_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreUtilStringConvertDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief level for logic which depends on order
 *
 * higher priority ensured to have smaller value
 */
typedef enum
{
    ZFLevelZFFrameworkStatic        = 0, /**< @brief ZFFramework static */
    ZFLevelZFFrameworkEssential     = 1, /**< @brief ZFFramework essential */
    ZFLevelZFFrameworkHigh          = 2, /**< @brief ZFFramework high */
    ZFLevelZFFrameworkNormal        = 3, /**< @brief ZFFramework */
    ZFLevelZFFrameworkLow           = 4, /**< @brief ZFFramework low */

    ZFLevelAppEssential             = 5, /**< @brief app essential */
    ZFLevelAppHigh                  = 6, /**< @brief app high */
    ZFLevelAppNormal                = 7, /**< @brief app */
    ZFLevelAppLow                   = 8, /**< @brief app low */

    ZFLevelZFFrameworkPostLow       = 9, /**< @brief ZFFramework post low */
    ZFLevelZFFrameworkPostNormal    = 10, /**< @brief ZFFramework post normal */
    ZFLevelZFFrameworkPostHigh      = 11, /**< @brief ZFFramework post high */
    ZFLevelZFFrameworkPostEssential = 12, /**< @brief ZFFramework post essential */
    ZFLevelZFFrameworkPostStatic    = 13, /**< @brief ZFFramework post static */
} ZFLevel;

/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkStatic zfText("LevelZFFrameworkStatic")
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkEssential zfText("LevelZFFrameworkEssential")
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkHigh zfText("LevelZFFrameworkHigh")
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkNormal zfText("LevelZFFrameworkNormal")
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkLow zfText("LevelZFFrameworkLow")

/** @brief string tokens */
#define ZFTOKEN_ZFLevelAppEssential zfText("LevelAppEssential")
/** @brief string tokens */
#define ZFTOKEN_ZFLevelAppHigh zfText("LevelAppHigh")
/** @brief string tokens */
#define ZFTOKEN_ZFLevelAppNormal zfText("LevelAppNormal")
/** @brief string tokens */
#define ZFTOKEN_ZFLevelAppLow zfText("LevelAppLow")

/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkPostLow zfText("LevelZFFrameworkPostLow")
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkPostNormal zfText("LevelZFFrameworkPostNormal")
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkPostHigh zfText("LevelZFFrameworkPostHigh")
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkPostEssential zfText("LevelZFFrameworkPostEssential")
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkPostStatic zfText("LevelZFFrameworkPostStatic")

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFLevel, ZFLevel)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreLevelDef_h_

