/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIImageRes.h
 * @brief utility to load image from resource
 */

#ifndef _ZFI_ZFUIImageRes_h_
#define _ZFI_ZFUIImageRes_h_

#include "ZFUIImage.h"
#include "ZFUIImageCache.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief load image from resource with cache logic
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIImageRes(ZF_IN const zfchar *resPath);
/**
 * @brief load image from resource xml files with cache logic
 *
 * the xml file should contain serializable data which is able to be serialized to an image
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIImageResXml(ZF_IN const zfchar *resPath);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageRes_h_

