/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFLangUtil.h
 * @brief language util for localization
 */

#ifndef _ZFI_ZFLangUtil_h_
#define _ZFI_ZFLangUtil_h_

#include "ZFLang.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief see #zfLangApply
 */
typedef void (*ZFLangOnChangeCallback)(ZF_IN ZFObject *obj,
                                       ZF_IN const zfchar *objIdentity,
                                       ZF_IN const zfchar *langKey,
                                       ZF_IN const zfchar *langValue,
                                       ZF_IN_OPT ZFObject *userData);
/**
 * @brief util method to register an object that would be automatically updated
 *   when language changed (see #ZFGlobalEvent::EventLangOnChange)
 *
 * note, an object with the same objIdentity can only be applied once,
 * newly applied one would override old one\n
 * language auto update would be stopped automatically if the object would be deallocated,
 * or can be stopped manually by #zfLangApplyCancel
 * @note langChangeCallback would be called immediately to update for the first time
 */
extern ZF_ENV_EXPORT void zfLangApply(ZF_IN ZFObject *obj,
                                      ZF_IN const zfchar *objIdentity,
                                      ZF_IN const zfchar *langKey,
                                      ZF_IN ZFLangOnChangeCallback langChangeCallback,
                                      ZF_IN_OPT ZFObject *userData = zfnull);
/**
 * @brief cancel language auto update started by #zfLangApply
 *
 * by default, the language auto update would be stopped if the object would be deallocated,
 * there's no need to call this method for most cases
 */
extern ZF_ENV_EXPORT void zfLangApplyCancel(ZF_IN ZFObject *obj,
                                            ZF_IN const zfchar *objIdentity);

// ============================================================
/**
 * @brief util macro to declare #zfLangApply's implementation
 *
 * usage:
 * @code
 *   / ** you may add docs here * /
 *   ZFLANGAPPLY_IMPL(YourObjectType, YourTypeName, {obj->applyYourLang(objIdentity, langKey, langValue, userData);})
 *
 *   // impl function's proto type:
 *   //   void action(ZF_IN YourObjectType *obj,
 *   //       ZF_IN const zfchar *objIdentity,
 *   //       ZF_IN const zfchar *langKey,
 *   //       ZF_IN const zfchar *langValue,
 *   //       ZF_IN_OPT ZFObject *userData);
 * @endcode
 * expand this method for you:
 * @code
 *   void zfLangApplyYourTypeName(ZF_IN YourObjectType *,
 *                                ZF_IN const zfchar *langKey,
 *                                ZF_IN_OPT ZFObject *userData = zfnull);
 *   void zfLangApplyCancelYourTypeName(ZF_IN YourObjectType *);
 * @endcode
 */
#define ZFLANGAPPLY_IMPL(T_ZFObject, YourTypeName, T_action) \
    zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfLangApplyImpl_##YourTypeName \
    { \
    public: \
        static void _ZFP_action(ZF_IN ZFObject *_obj, \
                                ZF_IN const zfchar *objIdentity, \
                                ZF_IN const zfchar *langKey, \
                                ZF_IN const zfchar *langValue, \
                                ZF_IN_OPT ZFObject *userData) \
        { \
            T_ZFObject *obj = _obj->to<T_ZFObject *>(); \
            T_action \
        } \
    }; \
    /** \n apply language for T_ZFObject */ \
    inline void zfLangApply##YourTypeName(ZF_IN T_ZFObject *obj, \
                                          ZF_IN const zfchar *langKey, \
                                          ZF_IN_OPT ZFObject *userData = zfnull) \
    { \
        zfLangApply(ZFObjectToObject(obj), ZFM_TOSTRING(YourTypeName), langKey, _ZFP_zfLangApplyImpl_##YourTypeName::_ZFP_action, userData); \
    } \
    /** @brief see #zfLangApplyCancel */ \
    inline void zfLangApplyCancel##YourTypeName(ZF_IN T_ZFObject *obj) \
    { \
        zfLangApplyCancel(ZFObjectToObject(obj), ZFM_TOSTRING(YourTypeName)); \
    }

// ============================================================
/**
 * @brief auto apply localized string to propery when #ZFGlobalEvent::EventLangOnChange
 */
extern ZF_ENV_EXPORT void zfLangApplyProperty_zfstring(ZF_IN ZFObject *obj,
                                                       ZF_IN const ZFProperty *property,
                                                       ZF_IN const zfchar *langKey);
/** @brief see #zfLangApplyProperty_zfstring, #zfLangApplyCancel */
extern ZF_ENV_EXPORT void zfLangApplyCancelProperty_zfstring(ZF_IN ZFObject *obj,
                                                             ZF_IN const ZFProperty *property);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLangUtil_h_

