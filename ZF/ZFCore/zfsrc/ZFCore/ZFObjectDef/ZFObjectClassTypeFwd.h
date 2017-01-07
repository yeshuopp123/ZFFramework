/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectClassTypeFwd.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFObjectClassTypeFwd_h_
#define _ZFI_ZFObjectClassTypeFwd_h_

#include "../ZFCoreDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief same as class, shows that this class is a #ZFObject type
 *
 * we use some dummy macro to show the basic type of class:
 * -  zfclassFwd: forward declare for a type
 * -  zfclass: shows that it's a subclass of ZFObject,
 *   note that a ZFObject is always not a POD type
 *   and always can't be declared in stack
 * -  zfabstract: shows that it's a abstract class of ZFObject type
 * -  zfclassPOD: shows that it's a POD type
 * -  zfclassLikePOD: not POD type, but you may use it like POD,
 *   e.g. declare in stack, copy value by operator = (if available),
 *   except one limitation: must not memset to 0
 * -  zfclassNotPOD: shows that it's neither ZFObject nor POD type
 *   (or not designed as POD type),
 *   and usually should not be declared in stack
 *   or copy it by copy constructor or operator =
 *
 * @warning zfclassLikePOD is not actually a POD,
 *   usually has it's own constructor or destructor,
 *   so it's highly deprecated to declare a zfclassLikePOD as static variable,
 *   use ZF_GLOBAL_INITIALIZER_INIT or ZF_STATIC_INITIALIZER_INIT instead
 *   to declare in global
 */
#define zfclass class
/**
 * @brief typename for class showing that its abstract
 */
#define zfabstract class

/**
 * @brief declare a class inherit from other class
 */
#define zfextends public

// ============================================================
/**
 * @brief ZFObject auto retain version of #ZFCORE_PARAM
 */
#define ZFCORE_PARAM_RETAIN(T_ParamType, paramName) \
    ZFCORE_PARAM_RETAIN_WITH_INIT(T_ParamType, paramName, ZFM_EMPTY())
/**
 * @brief see #ZFCORE_PARAM_RETAIN
 */
#define ZFCORE_PARAM_RETAIN_WITH_INIT(T_ParamType, paramName, initValue) \
    public: \
        T_ParamType const &paramName(void) const \
        { \
            return this->paramName##_.value; \
        } \
    private: \
        /** @cond ZFPrivateDoc */ \
        zfclassLikePOD ZF_ENV_EXPORT paramName##_ZFCoreParam \
        { \
        public: \
            paramName##_ZFCoreParam(void) \
            : value(zfnull) \
            { \
                T_ParamType *t = zfnull; \
                ZFCoreMutexLock(); \
                this->value = zflockfree_zfRetainWithoutLeakTest(*(t = zfnew(T_ParamType, initValue))); \
                zflockfree_ZFLeakTestLogAfterRetainVerbose(this->value, ZF_CALLER_FILE, #paramName zfTextA(" init"), ZF_CALLER_LINE); \
                ZFCoreMutexUnlock(); \
                zfdelete(t); \
            } \
            paramName##_ZFCoreParam(ZF_IN const paramName##_ZFCoreParam &ref) \
            { \
                ZFCoreMutexLock(); \
                this->value = zflockfree_zfRetainWithoutLeakTest(ref.value); \
                zflockfree_ZFLeakTestLogAfterRetainVerbose(this->value, ZF_CALLER_FILE, #paramName zfTextA("Set"), ZF_CALLER_LINE); \
                ZFCoreMutexUnlock(); \
            } \
            ~paramName##_ZFCoreParam(void) \
            { \
                ZFCoreMutexLock(); \
                zflockfree_ZFLeakTestLogBeforeReleaseVerbose(this->value, ZF_CALLER_FILE, #paramName zfTextA(" cleanup"), ZF_CALLER_LINE); \
                zflockfree_zfReleaseWithoutLeakTest(this->value); \
                ZFCoreMutexUnlock(); \
            } \
            paramName##_ZFCoreParam &operator = (ZF_IN const paramName##_ZFCoreParam &ref) \
            { \
                ZFCoreMutexLock(); \
                zflockfree_zfRetainWithoutLeakTest(ref.value); \
                zflockfree_ZFLeakTestLogAfterRetainVerbose(ref.value, ZF_CALLER_FILE, #paramName zfTextA("Set"), ZF_CALLER_LINE); \
                zflockfree_ZFLeakTestLogBeforeReleaseVerbose(this->value, ZF_CALLER_FILE, #paramName zfTextA("Set"), ZF_CALLER_LINE); \
                zflockfree_zfReleaseWithoutLeakTest(this->value); \
                ZFCoreMutexUnlock(); \
                this->value = ref.value; \
                return *this; \
            } \
        public: \
            T_ParamType value; \
        }; \
        paramName##_ZFCoreParam paramName##_; \
        /** @endcond */ \
    public: \
        /** @brief see @ref paramName */ \
        zfself &paramName##Set(ZF_IN T_ParamType const &value) \
        { \
            ZFCoreMutexLock(); \
            zflockfree_zfRetainWithoutLeakTest(value); \
            zflockfree_ZFLeakTestLogAfterRetainVerbose(value, ZF_CALLER_FILE, #paramName zfTextA("Set"), ZF_CALLER_LINE); \
            zflockfree_ZFLeakTestLogBeforeReleaseVerbose(this->paramName##_.value, ZF_CALLER_FILE, #paramName zfTextA("Set"), ZF_CALLER_LINE); \
            zflockfree_zfReleaseWithoutLeakTest(this->paramName##_.value); \
            ZFCoreMutexUnlock(); \
            this->paramName##_.value = value; \
            return *this; \
        }

// ============================================================
#define _ZFP_ZFCORE_PARAM_RETAIN_DECLARE(T_ParamType, paramName) \
    public: \
        T_ParamType const &paramName(void) const; \
    private: \
        /** @cond ZFPrivateDoc */ \
        zfclassLikePOD ZF_ENV_EXPORT paramName##_ZFCoreParam \
        { \
        public: \
            paramName##_ZFCoreParam(void); \
            paramName##_ZFCoreParam(ZF_IN const paramName##_ZFCoreParam &ref); \
            ~paramName##_ZFCoreParam(void); \
            paramName##_ZFCoreParam &operator = (ZF_IN const paramName##_ZFCoreParam &ref); \
        public: \
            T_ParamType value; \
        public: \
            inline const zfcharA *_callerFile(void) const \
            { \
                return ZF_CALLER_FILE; \
            } \
            inline zfindex _callerLine(void) const \
            { \
                return ZF_CALLER_LINE; \
            } \
        }; \
        paramName##_ZFCoreParam paramName##_; \
        /** @endcond */ \
    public: \
        /** @brief see @ref paramName */ \
        zfself &paramName##Set(ZF_IN T_ParamType const &value);
#define _ZFP_ZFCORE_PARAM_RETAIN_DEFINE(T_Owner, T_ParamType, paramName, initValue) \
    T_ParamType const &T_Owner::paramName(void) const \
    { \
        return this->paramName##_.value; \
    } \
    /** @cond ZFPrivateDoc */ \
    T_Owner::paramName##_ZFCoreParam::paramName##_ZFCoreParam(void) \
    { \
        T_ParamType *t = zfnull; \
        ZFCoreMutexLock(); \
        this->value = zflockfree_zfRetainWithoutLeakTest(*(t = zfnew(T_ParamType, initValue))); \
        zflockfree_ZFLeakTestLogAfterRetainVerbose(this->value, this->_callerFile(), #paramName zfTextA(" init"), this->_callerLine()); \
        ZFCoreMutexUnlock(); \
        zfdelete(t); \
    } \
    T_Owner::paramName##_ZFCoreParam::paramName##_ZFCoreParam(ZF_IN const T_Owner::paramName##_ZFCoreParam &ref) \
    { \
        ZFCoreMutexLock(); \
        this->value = zflockfree_zfRetainWithoutLeakTest(ref.value); \
        ZFLeakTestLogAfterRetainVerbose(this->value, this->_callerFile(), #paramName zfTextA("Set"), this->_callerLine()); \
        ZFCoreMutexUnlock(); \
    } \
    T_Owner::paramName##_ZFCoreParam::~paramName##_ZFCoreParam(void) \
    { \
        ZFCoreMutexLock(); \
        zflockfree_ZFLeakTestLogBeforeReleaseVerbose(this->value, this->_callerFile(), #paramName zfTextA(" cleanup"), this->_callerLine()); \
        zflockfree_zfReleaseWithoutLeakTest(this->value); \
        ZFCoreMutexUnlock(); \
    } \
    T_Owner::paramName##_ZFCoreParam &T_Owner::paramName##_ZFCoreParam::operator = (ZF_IN const T_Owner::paramName##_ZFCoreParam &ref) \
    { \
        ZFCoreMutexLock(); \
        zflockfree_zfRetainWithoutLeakTest(ref.value); \
        zflockfree_ZFLeakTestLogAfterRetainVerbose(ref.value, this->_callerFile(), #paramName zfTextA("Set"), this->_callerLine()); \
        zflockfree_ZFLeakTestLogBeforeReleaseVerbose(this->value, this->_callerFile(), #paramName zfTextA("Set"), this->_callerLine()); \
        zflockfree_zfReleaseWithoutLeakTest(this->value); \
        ZFCoreMutexUnlock(); \
        this->value = ref.value; \
        return *this; \
    } \
    /** @endcond */ \
    T_Owner::zfself &T_Owner::paramName##Set(ZF_IN T_ParamType const &value) \
    { \
        ZFCoreMutexLock(); \
        zflockfree_zfRetainWithoutLeakTest(value); \
        zflockfree_ZFLeakTestLogAfterRetainVerbose(value, this->paramName##_._callerFile(), #paramName zfTextA("Set"), this->paramName##_._callerLine()); \
        zflockfree_ZFLeakTestLogBeforeReleaseVerbose(this->paramName##_.value, this->paramName##_._callerFile(), #paramName zfTextA("Set"), this->paramName##_._callerLine()); \
        zflockfree_zfReleaseWithoutLeakTest(this->paramName##_.value); \
        ZFCoreMutexUnlock(); \
        this->paramName##_.value = value; \
        return *this; \
    }

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectClassTypeFwd_h_

