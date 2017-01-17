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

// ============================================================
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
 *
 *
 * REMARKS:
 * this macro can be used in header files,
 * but not recommended,
 * since it would create global static object to perform the static register,
 * which would easily cause "too many sections" "file too big" error under Windows,
 * may or may not have other further side effect on other condition,
 * try not to abuse it in header
 */
#define ZF_STATIC_REGISTER_INIT(Name) \
    /** @cond ZFPrivateDoc */ \
    template<typename D> \
    zfclassNotPOD _ZFP_SR_##Name \
    { \
    public: \
        zfclassNotPOD ExecInit \
        { \
        public: \
            ExecInit(void)
/** @brief see ZF_STATIC_REGISTER_INIT */
#define ZF_STATIC_REGISTER_DESTROY(Name) \
            ~ExecInit(void)
/** @brief see ZF_STATIC_REGISTER_INIT */
#define ZF_STATIC_REGISTER_END(Name) \
        }; \
        static ExecInit register_obj; \
        template<ExecInit &> class RefIt {}; \
        static RefIt<register_obj> ref_obj; \
    }; \
    template<typename D> \
    typename _ZFP_SR_##Name<D>::ExecInit _ZFP_SR_##Name<D>::register_obj; \
    zfclassNotPOD _ZFP_SR_D_##Name : _ZFP_SR_##Name<_ZFP_SR_D_##Name> \
    {}; \
    /** @endcond */

// ============================================================
template<typename Wrapper>
zfclassNotPOD _ZFP_SRIC
{
public:
    zfclassNotPOD ExecInit
    {
    public:
        ExecInit(void)
        {
            Wrapper::_ZFP_SRIC_init();
        }
        ~ExecInit(void)
        {
            Wrapper::_ZFP_SRIC_destroy();
        }
    };
    static ExecInit register_obj;
    template<ExecInit &> class RefIt {};
    static RefIt<register_obj> ref_obj;
};
template<typename Wrapper>
typename _ZFP_SRIC<Wrapper>::ExecInit _ZFP_SRIC<Wrapper>::register_obj;
zfclassNotPOD ZF_ENV_EXPORT _ZFP_SRICDummyBase
{
public:
    static void _ZFP_SRIC_init(void) {}
    static void _ZFP_SRIC_destroy(void) {}
};
zfclassNotPOD _ZFP_SRICDummy : zfextendsNotPOD _ZFP_SRICDummyBase, zfextendsNotPOD _ZFP_SRIC<_ZFP_SRICDummy>
{
};

/**
 * @brief static register step that are inside class scope
 *
 * usage:
 * @code
 *   class MyClass {
 *       ZF_STATIC_REGISTER_IN_CLASS_INIT(YourName)
 *       {
 *           // your register code
 *       }
 *       ZF_STATIC_REGISTER_IN_CLASS_DESTROY(YourName) // destroy step is optional
 *       {
 *           // your unregister code
 *       }
 *       ZF_STATIC_REGISTER_IN_CLASS_END(YourName)
 *   };
 * @endcode
 * @note for different register step, the order is not ensured
 * @note for internal use only, for app level,
 *   it's not recommended to register within class scope
 *
 *
 * REMARKS:
 * this macro can be used in header files,
 * but not recommended,
 * since it would create global static object to perform the static register,
 * which would easily cause "too many sections" "file too big" error under Windows,
 * may or may not have other further side effect on other condition,
 * try not to abuse it in header
 */
#define ZF_STATIC_REGISTER_IN_CLASS_INIT(Name) \
    public: \
        /** @cond ZFPrivateDoc */ \
        zfclassNotPOD _ZFP_SRIC_##Name : zfextendsNotPOD _ZFP_SRICDummyBase \
        { \
        public: \
            static void _ZFP_SRIC_init(void) \
            {
/** @brief see ZF_STATIC_REGISTER_INIT */
#define ZF_STATIC_REGISTER_IN_CLASS_DESTROY(Name) \
            } \
            static void _ZFP_SRIC_destroy(void) \
            {
/** @brief see ZF_STATIC_REGISTER_INIT */
#define ZF_STATIC_REGISTER_IN_CLASS_END(Name) \
            } \
        }; \
    public: \
        static _ZFP_SRIC<_ZFP_SRIC_##Name> &_ZFP_SRICRef_##Name(void) \
        { \
            static _ZFP_SRIC<_ZFP_SRIC_##Name> _ZFP_SRICObj_##Name; \
            return _ZFP_SRICObj_##Name; \
        } \
        /** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreStaticRegisterDef_h_

