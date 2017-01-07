/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFResCache_ZFJson.h
 * @brief utility to load serializable from resource with cache logic
 */

#ifndef _ZFI_ZFResCache_ZFJson_h_
#define _ZFI_ZFResCache_ZFJson_h_

#include "ZFAdditionDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief load serializable in json format from resource
 *
 * it's your responsibility to ensure the result serializable can be used more than once,
 * otherwise, you should copy the result (typically #ZFCopyable)
 */
extern ZF_ENV_EXPORT zfautoObject ZFResJson(ZF_IN const zfchar *resPath);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFResCache_ZFJson_h_

