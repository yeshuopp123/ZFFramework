/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreStaticRegisterDef.h
 * @brief static register util
 */
#ifndef _ZFI_ZFCoreStaticRegisterDef_h_
#define _ZFI_ZFCoreStaticRegisterDef_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief static register step that ensured won't be stripped by compiler
 *
 * usage:
 * @code
 *   ZF_STATIC_REGISTER_INIT(YourName)
 *   {
 *       // your register code
 *   }
 *   ZF_STATIC_REGISTER_DESTROY(YourName) // destroy step is optional
 *   {
 *       // your unregister code
 *   }
 *   ZF_STATIC_REGISTER_END(YourName)
 * @endcode
 * @note for different register step, the order is not ensured
 * @note for internal use only, for app level,
 *   use #ZF_GLOBAL_INITIALIZER_INIT or #ZF_STATIC_INITIALIZER_INIT instead
 */
#define ZF_STATIC_REGISTER_INIT(Name) \
    /** @cond ZFPrivateDoc */ \
    template<typename D> \
    zfclassNotPOD _ZFP_SR_##Name \
    { \
    public: \
        zfclassNotPOD exec_register \
        { \
        public: \
            exec_register(void)
/** @brief see ZF_STATIC_REGISTER_INIT */
#define ZF_STATIC_REGISTER_DESTROY(Name) \
            ~exec_register(void)
/** @brief see ZF_STATIC_REGISTER_INIT */
#define ZF_STATIC_REGISTER_END(Name) \
        }; \
        template<exec_register &> class ref_it {}; \
        static exec_register register_object; \
        static ref_it<register_object> referrer; \
    }; \
    template<typename D> \
    typename _ZFP_SR_##Name<D>::exec_register _ZFP_SR_##Name<D>::register_object; \
    zfclassNotPOD _ZFP_SR_D_##Name : _ZFP_SR_##Name<_ZFP_SR_D_##Name> \
    {}; \
    /** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreStaticRegisterDef_h_

