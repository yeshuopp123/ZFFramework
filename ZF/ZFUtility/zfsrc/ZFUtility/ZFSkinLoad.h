/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSkinLoad.h
 * @brief skin util
 */

#ifndef _ZFI_ZFSkinLoad_h_
#define _ZFI_ZFSkinLoad_h_

#include "ZFSkin.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief load skin data for #zfSkin, see #zfSkin
 *
 * can load from more than one file,
 * old one would be kept, and new one would be merged to current\n
 * the file's format should be like:
 * @code
 *   <skin>
 *       <s k="YourSkinKey1">
 *           <YourSkinValueType1 /> // must be type of ZFStyleable
 *       </s>
 *       <s k="YourSkinKey2">
 *           <YourSkinValueType2 />
 *       </s>
 *   </skin>
 * @endcode
 * equivalent json format:
 * @code
 *   {
 *     "@skin" : [
 *       {
 *         "@s" : [
 *           {
 *             "@YourSkinValueType1" : []
 *           }
 *         ],
 *         "k" : "YourSkinKey1"
 *       },
 *       {
 *         "@s" : [
 *           {
 *             "@YourSkinValueType2" : []
 *           }
 *         ],
 *         "k" : "YourSkinKey2"
 *       }
 *     ]
 *   }
 * @endcode
 */
extern ZF_ENV_EXPORT zfbool zfSkinLoad(ZF_IN const ZFSerializableData &serializableData,
                                       ZF_OUT_OPT zfstring *errorHint = zfnull);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSkinLoad_h_

