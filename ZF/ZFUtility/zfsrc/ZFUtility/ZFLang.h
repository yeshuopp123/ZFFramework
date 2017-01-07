/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFLang.h
 * @brief language util for localization
 */

#ifndef _ZFI_ZFLang_h_
#define _ZFI_ZFLang_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * called when language changed
 */
ZFOBSERVER_EVENT_GLOBAL(LangOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
/**
 * @brief access localized string with key,
 *   or return the key itself if no such localized string or not loaded
 *
 * to use localized string, you must:
 * -  prepare your own language file and load it manually,
 *   then store language datas by #zfLangSet
 * -  use this method to access localized language
 *
 * it all depends on app to manage language files,
 * and decide which file to use
 */
extern ZF_ENV_EXPORT void zfLang(ZF_IN_OUT zfstring &ret,
                                 ZF_IN const zfchar *key,
                                 ZF_IN_OPT const zfchar *valueDefault = zfnull);
/** @brief see #zfLang */
inline zfstring zfLang(ZF_IN const zfchar *key,
                       ZF_IN_OPT const zfchar *valueDefault = zfnull)
{
    zfstring ret;
    zfLang(ret, key, valueDefault);
    return ret;
}
/**
 * @brief notify lang changed
 */
extern ZF_ENV_EXPORT void zfLangNotifyChanged(void);
/**
 * @brief manually change one language data, or remove if value is null
 *
 * no event would be fired when set by this method, no thread-safe is supplied\n
 * for implementation that supply load logic, you should:
 * -# load by your own logic, and save by #zfLangSet
 * -# notify data changed by #zfLangNotifyChanged
 */
extern ZF_ENV_EXPORT void zfLangSet(ZF_IN const zfchar *key, ZF_IN const zfchar *value);
/**
 * @brief similar to #zfLangSet, but applies only if not exist
 */
extern ZF_ENV_EXPORT void zfLangSetDefault(ZF_IN const zfchar *key, ZF_IN const zfchar *value);
/**
 * @brief unload all data (not recommended)
 */
extern ZF_ENV_EXPORT void zfLangUnload(void);

/**
 * @brief for debug use only
 */
extern ZF_ENV_EXPORT void zfLangDebug(ZF_IN_OPT const ZFOutputCallback &output = ZFOutputCallbackDefault);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLang_h_

#include "ZFLangUtil.h"

