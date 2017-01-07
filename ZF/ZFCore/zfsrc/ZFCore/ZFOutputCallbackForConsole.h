/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFOutputCallbackForConsole.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFOutputCallbackForConsole_h_
#define _ZFI_ZFOutputCallbackForConsole_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
extern ZF_ENV_EXPORT const ZFOutputCallback &_ZFP_ZFOutputCallbackForConsole(void);
/**
 * @brief default output callback to output to console, source must be zfchar *string
 */
#define ZFOutputCallbackForConsole _ZFP_ZFOutputCallbackForConsole()

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputCallbackForConsole_h_

