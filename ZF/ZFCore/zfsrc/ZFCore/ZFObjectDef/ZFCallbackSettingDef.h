/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCallbackSettingDef.h
 * @brief additional setting storage for #ZFCallback
 */

#ifndef _ZFI_ZFCallbackSettingDef_h_
#define _ZFI_ZFCallbackSettingDef_h_

#include "ZFCallbackDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT void _ZFP_ZFCallbackSettingSet(ZF_IN_OUT const ZFCallback &o, ZF_IN const zfchar *name, ZF_IN const ZFCorePointerBase &sp);
extern ZF_ENV_EXPORT const ZFCorePointerBase *_ZFP_ZFCallbackSettingGet(ZF_IN const ZFCallback &o, ZF_IN const zfchar *name);
/**
 * @brief declare an additional setting that can be attached to #ZFCallback
 *
 * usage:
 * @code
 *   ZFCALLBACK_SETTING_DECLARE_BEGIN(MySetting)
 *   // you can declare your settings here,
 *   // only POD-like object can be used (objects that support operator =)
 *   zfstring myValue;
 *   // use #ZFCORE_PARAM_WITH_INIT is recommended
 *   ZFCORE_PARAM_WITH_INIT(zfstring, myValue2, zfText("initValue"))
 *
 *   // supply your custom constructor if necessary
 *   MySetting(void) : myValue() {}
 *   MySetting(zfstring const &v) : myValue(v) {}
 *   ZFCALLBACK_SETTING_DECLARE_END(MySetting)
 * @endcode
 * \n
 * once declared, you may attach it to any #ZFCallback:
 * @code
 *   MySetting mySetting(zfText("123"));
 *   ZFCallback callback;
 *   MySettingSet(callback, mySetting);
 * @endcode
 * and access it if necessary:
 * @code
 *   const MySetting &mySetting = MySettingGet(callback);
 *   // mySetting would have default setting if not exist
 * @endcode
 */
#define ZFCALLBACK_SETTING_DECLARE_BEGIN(T_SettingName) \
    /** \n see #ZFCALLBACK_SETTING_DECLARE_BEGIN */ \
    zfclassLikePOD ZF_ENV_EXPORT T_SettingName \
    { \
    public: \
        static const T_SettingName &_ZFP_ZFCallbackSettingDefault(void) \
        { \
            static T_SettingName d; \
            return d; \
        } \
    public:
/** @brief see #ZFCALLBACK_SETTING_DECLARE_BEGIN */
#define ZFCALLBACK_SETTING_DECLARE_END(T_SettingName) \
    }; \
    /** @brief see #ZFCALLBACK_SETTING_DECLARE_BEGIN */ \
    template<typename T_ZFCallback> \
    inline const T_ZFCallback &T_SettingName##Remove(ZF_IN_OUT const T_ZFCallback &o) \
    { \
        _ZFP_ZFCallbackSettingSet(o, #T_SettingName, ZFCorePointerForObject<T_SettingName *>()); \
        return o; \
    } \
    /** @brief see #ZFCALLBACK_SETTING_DECLARE_BEGIN */ \
    template<typename T_ZFCallback> \
    inline const T_ZFCallback &T_SettingName##Set(ZF_IN_OUT const T_ZFCallback &o, ZF_IN const T_SettingName &setting) \
    { \
        _ZFP_ZFCallbackSettingSet(o, #T_SettingName, ZFCorePointerForObject<T_SettingName *>(zfnew(T_SettingName, setting))); \
        return o; \
    } \
    /** @brief see #ZFCALLBACK_SETTING_DECLARE_BEGIN */ \
    inline const T_SettingName &T_SettingName##Get(ZF_IN const ZFCallback &o) \
    { \
        const ZFCorePointerBase *sp = _ZFP_ZFCallbackSettingGet(o, #T_SettingName); \
        return (sp ? *(sp->pointerValueT<const T_SettingName *>()) : T_SettingName::_ZFP_ZFCallbackSettingDefault()); \
    }

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallbackSettingDef_h_

