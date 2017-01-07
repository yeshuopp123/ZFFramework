/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSkin.h
 * @brief skin util for localization
 */

#ifndef _ZFI_ZFSkin_h_
#define _ZFI_ZFSkin_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * called when skin changed
 */
ZFOBSERVER_EVENT_GLOBAL(SkinOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
/**
 * @brief access localized skin with key,
 *   or return null if not exist
 *
 * a skin is a #ZFStyleable,
 * and skin logic is achieved by ##ZFStyleable::styleableCopyFrom\n
 * \n
 * to use localized skin, you must:
 * -  prepare your own skin file and load it manually,
 *   then store skin datas by #zfSkinSet
 * -  use this method to access localized skin
 *
 * it all depends on app to manage skin files,
 * and decide which file to use
 */
extern ZF_ENV_EXPORT zfautoObject zfSkin(ZF_IN const zfchar *key,
                                         ZF_IN_OPT ZFStyleable *valueDefault = zfnull);
/**
 * @brief notify skin changed
 */
extern ZF_ENV_EXPORT void zfSkinNotifyChanged(void);
/**
 * @brief manually change one skin data, or remove if value is null
 *
 * no event would be fired when set by this method, no thread-safe is supplied\n
 * for implementation that supply load logic, you should:
 * -# load by your own logic, and save by #zfSkinSet
 * -# notify data changed by #zfSkinNotifyChanged
 */
extern ZF_ENV_EXPORT void zfSkinSet(ZF_IN const zfchar *key, ZF_IN ZFStyleable *value);
/**
 * @brief similar to #zfSkinSet, but applies only if not exist
 */
extern ZF_ENV_EXPORT void zfSkinSetDefault(ZF_IN const zfchar *key, ZF_IN ZFStyleable *value);
/**
 * @brief unload all data (not recommended)
 */
extern ZF_ENV_EXPORT void zfSkinUnload(void);

/**
 * @brief for debug use only
 */
extern ZF_ENV_EXPORT void zfSkinDebug(ZF_IN_OPT const ZFOutputCallback &output = ZFOutputCallbackDefault);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSkin_h_

