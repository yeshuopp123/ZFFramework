/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreTypeDef_CoreIntType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_CoreIntType_h_
#define _ZFI_ZFCoreTypeDef_CoreIntType_h_

#include "ZFCoreTypeDef_CorePrimitiveType.h"

ZF_ENV_SENSITIVE("whether we have stdint")
#if defined(__GNUC__)
    #define _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint 1
#elif defined(_MSC_VER)
    #if _MSC_VER >= 1600
        #define _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint 1
    #else
        #define _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint 0
    #endif
#elif defined(__BORLANDC__)
    #if __BORLANDC__ >= 0x0560
        #define _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint 1
    #else
        #define _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint 0
    #endif
#else
    #define _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint 1
#endif

#if _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint
    #include <stdint.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @typedef zft_zfint8
 * @brief primitive number type, use only if necessary, see #zfindex
 * @typedef zft_zfuint8
 * @brief primitive number type, use only if necessary, see #zfindex
 *
 * @typedef zft_zfint16
 * @brief primitive number type, use only if necessary, see #zfindex
 * @typedef zft_zfuint16
 * @brief primitive number type, use only if necessary, see #zfindex
 *
 * @typedef zft_zfint32
 * @brief primitive number type, use only if necessary, see #zfindex
 * @typedef zft_zfuint32
 * @brief primitive number type, use only if necessary, see #zfindex
 *
 * @typedef zft_zfint64
 * @brief primitive number type, use only if necessary, see #zfindex
 * @typedef zft_zfuint64
 * @brief primitive number type, use only if necessary, see #zfindex
 * */
#if _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint
    typedef int8_t zft_zfint8;
    typedef uint8_t zft_zfuint8;

    typedef int16_t zft_zfint16;
    typedef uint16_t zft_zfuint16;

    typedef int32_t zft_zfint32;
    typedef uint32_t zft_zfuint32;

    typedef int64_t zft_zfint64;
    typedef uint64_t zft_zfuint64;
#else
    typedef signed char zft_zfint8;
    typedef unsigned char zft_zfuint8;

    typedef signed short int zft_zfint16;
    typedef unsigned short int zft_zfuint16;

    typedef signed long int zft_zfint32;
    typedef unsigned long int zft_zfuint32;

    typedef signed long long int zft_zfint64;
    typedef unsigned long long int zft_zfuint64;
#endif // #if _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint
#undef _ZFP_ZFCoreTypeDef_CoreIntType_has_stdint

// ============================================================
/**
 * @brief wrapper to #ZFT_INT_STRONG
 */
#define ZFT_INT_WEAK(T, D) \
    typedef T D; \
    /** @brief primitive number type, use only if necessary, for app, use @ref D instead */ \
    typedef T zft_##D;

// ============================================================
/**
 * @brief declare a strong wrapper type for integer types, similar to boost's BOOST_STRONG_TYPEDEF
 */
#define ZFT_INT_STRONG(T, D) \
    ZFT_INT_STRONG_DETAIL(T, D, \
        ZFT_INT_STRONG_BIT_NOT_SUPPORT, \
        ZFT_INT_STRONG_MATH_SUPPORT)
/** @brief see #ZFT_INT_STRONG */
#define ZFT_INT_STRONG_WITH_BIT(T, D) \
    ZFT_INT_STRONG_DETAIL(T, D, \
        ZFT_INT_STRONG_BIT_SUPPORT, \
        ZFT_INT_STRONG_MATH_SUPPORT)
/** @brief see #ZFT_INT_STRONG */
#define ZFT_INT_STRONG_DETAIL(T, D, \
        ZFT_INT_STRONG_BIT_SUPPORT_OR_NOT, \
        ZFT_INT_STRONG_MATH_SUPPORT_OR_NOT) \
    _ZFP_ZFT_INT_STRONG_DETAIL(T, D, \
        ZFT_INT_STRONG_BIT_SUPPORT_OR_NOT, \
        ZFT_INT_STRONG_MATH_SUPPORT_OR_NOT)
#define _ZFP_ZFT_INT_STRONG_DETAIL(T, D, \
        ZFT_INT_STRONG_BIT_SUPPORT_OR_NOT, \
        ZFT_INT_STRONG_MATH_SUPPORT_OR_NOT) \
    /** @cond ZFPrivateDoc */ \
    class _zft_##D \
    { \
    protected: \
        typedef _zft_##D D; \
    public: \
        T t; \
        _zft_##D(void) : t() {} \
        _zft_##D(const D &t_) : t(t_.t) {} \
        explicit _zft_##D(const T t_) : t(t_) {} \
        D &operator = (const D &t_) {t = t_.t; return *this;} \
        template<typename T2> D &operator = (const T2 &t_) {t = t_; return *this;} \
        operator const T &(void) const {return t;} \
        operator T &(void) {return t;} \
        _ZFP_ZFT_INT_STRONG_BIT_MEMBER(ZFT_INT_STRONG_BIT_SUPPORT_OR_NOT, T, D) \
        _ZFP_ZFT_INT_STRONG_MATH_MEMBER(ZFT_INT_STRONG_MATH_SUPPORT_OR_NOT, T, D) \
    }; \
    /** @endcond */ \
    typedef _zft_##D D; \
    /** @brief primitive number type, use only if necessary, for app, use @ref D instead */ \
    typedef T zft_##D; \
    /** @cond ZFPrivateDoc */ \
    inline bool operator == (const D &v, const D &t_) {return (v.t == t_.t);} \
    inline bool operator != (const D &v, const D &t_) {return (v.t != t_.t);} \
    inline bool operator <  (const D &v, const D &t_) {return (v.t <  t_.t);} \
    inline bool operator <= (const D &v, const D &t_) {return (v.t <= t_.t);} \
    inline bool operator >  (const D &v, const D &t_) {return (v.t >  t_.t);} \
    inline bool operator >= (const D &v, const D &t_) {return (v.t >= t_.t);} \
    template<typename T2> bool operator == (const D &v, const T2 &t_) {return (v.t == t_);} \
    template<typename T2> bool operator != (const D &v, const T2 &t_) {return (v.t != t_);} \
    template<typename T2> bool operator <  (const D &v, const T2 &t_) {return (v.t <  t_);} \
    template<typename T2> bool operator <= (const D &v, const T2 &t_) {return (v.t <= t_);} \
    template<typename T2> bool operator >  (const D &v, const T2 &t_) {return (v.t >  t_);} \
    template<typename T2> bool operator >= (const D &v, const T2 &t_) {return (v.t >= t_);} \
    template<typename T2> bool operator == (const T2 &t_, const D &v) {return (t_ == v.t);} \
    template<typename T2> bool operator != (const T2 &t_, const D &v) {return (t_ != v.t);} \
    template<typename T2> bool operator <  (const T2 &t_, const D &v) {return (t_ <  v.t);} \
    template<typename T2> bool operator <= (const T2 &t_, const D &v) {return (t_ <= v.t);} \
    template<typename T2> bool operator >  (const T2 &t_, const D &v) {return (t_ >  v.t);} \
    template<typename T2> bool operator >= (const T2 &t_, const D &v) {return (t_ >= v.t);} \
    /** @endcond */ \
    _ZFP_ZFT_INT_STRONG_BIT(ZFT_INT_STRONG_BIT_SUPPORT_OR_NOT, T, D) \
    _ZFP_ZFT_INT_STRONG_MATH(ZFT_INT_STRONG_MATH_SUPPORT_OR_NOT, T, D)

/** @brief see #ZFT_INT_STRONG_DETAIL */
#define ZFT_INT_STRONG_BIT_NOT_SUPPORT bit_not_support
/** @brief see #ZFT_INT_STRONG_DETAIL */
#define ZFT_INT_STRONG_BIT_SUPPORT bit_support

/** @brief see #ZFT_INT_STRONG_DETAIL */
#define ZFT_INT_STRONG_MATH_NOT_SUPPORT math_not_support
/** @brief see #ZFT_INT_STRONG_DETAIL */
#define ZFT_INT_STRONG_MATH_SUPPORT math_support

// ============================================================
#define _ZFP_ZFT_INT_STRONG_BIT_MEMBER(bit_support_or_not, T, D) \
    _ZFP_ZFT_INT_STRONG_BIT_MEMBER_##bit_support_or_not(T, D)
#define _ZFP_ZFT_INT_STRONG_BIT_MEMBER_bit_not_support(T, D)
#define _ZFP_ZFT_INT_STRONG_BIT_MEMBER_bit_support(T, D) \
    /** @cond ZFPrivateDoc */ \
    D operator ~ (void) const {return (D)(~t);} \
    D &operator <<= (int n) {t <<= n; return *this;} \
    D &operator >>= (int n) {t >>= n; return *this;} \
    D &operator |= (const D &t_) {t |= t_.t; return *this;} \
    D &operator &= (const D &t_) {t &= t_.t; return *this;} \
    D &operator ^= (const D &t_) {t ^= t_.t; return *this;} \
    template<typename T2> D &operator |= (const T2 &t_) {t |= t_; return *this;} \
    template<typename T2> D &operator &= (const T2 &t_) {t &= t_; return *this;} \
    template<typename T2> D &operator ^= (const T2 &t_) {t ^= t_; return *this;} \
    /** @endcond */
#define _ZFP_ZFT_INT_STRONG_BIT(bit_support_or_not, T, D) \
    _ZFP_ZFT_INT_STRONG_BIT_##bit_support_or_not(T, D)
#define _ZFP_ZFT_INT_STRONG_BIT_bit_not_support(T, D)
#define _ZFP_ZFT_INT_STRONG_BIT_bit_support(T, D) \
    /** @cond ZFPrivateDoc */ \
    inline D operator << (const D &v, int n) {return (D)(v.t << n);} \
    inline D operator >> (const D &v, int n) {return (D)(v.t >> n);} \
    inline D operator | (const D &v, const D &t_) {return (D)(v.t | t_.t);} \
    inline D operator & (const D &v, const D &t_) {return (D)(v.t & t_.t);} \
    inline D operator ^ (const D &v, const D &t_) {return (D)(v.t ^ t_.t);} \
    template<typename T2> D operator | (const D &v, const T2 &t_) {return (D)(v.t | t_);} \
    template<typename T2> D operator & (const D &v, const T2 &t_) {return (D)(v.t & t_);} \
    template<typename T2> D operator ^ (const D &v, const T2 &t_) {return (D)(v.t ^ t_);} \
    template<typename T2> D operator | (const T2 &t_, const D &v) {return (D)(t_ | v.t);} \
    template<typename T2> D operator & (const T2 &t_, const D &v) {return (D)(t_ & v.t);} \
    template<typename T2> D operator ^ (const T2 &t_, const D &v) {return (D)(t_ ^ v.t);} \
    /** @endcond */

// ============================================================
#define _ZFP_ZFT_INT_STRONG_MATH_MEMBER(math_support_or_not, T, D) \
    _ZFP_ZFT_INT_STRONG_MATH_MEMBER_##math_support_or_not(T, D)
#define _ZFP_ZFT_INT_STRONG_MATH_MEMBER_math_not_support(T, D)
#define _ZFP_ZFT_INT_STRONG_MATH_MEMBER_math_support(T, D) \
    /** @cond ZFPrivateDoc */ \
    bool operator ! (void) const {return (t != 0);} \
    D &operator ++ (void) {++t; return *this;} \
    D &operator -- (void) {--t; return *this;} \
    D operator ++ (int) {return (D)(t++);} \
    D operator -- (int) {return (D)(t--);} \
    D &operator += (const D &t_) {t += t_.t; return *this;} \
    D &operator -= (const D &t_) {t -= t_.t; return *this;} \
    D &operator *= (const D &t_) {t *= t_.t; return *this;} \
    D &operator /= (const D &t_) {t /= t_.t; return *this;} \
    template<typename T2> D &operator %= (const T2 &t_) {t %= t_; return *this;} \
    template<typename T2> D &operator += (const T2 &t_) {t += t_; return *this;} \
    template<typename T2> D &operator -= (const T2 &t_) {t -= t_; return *this;} \
    template<typename T2> D &operator *= (const T2 &t_) {t *= t_; return *this;} \
    template<typename T2> D &operator /= (const T2 &t_) {t /= t_; return *this;} \
    /** @endcond */
#define _ZFP_ZFT_INT_STRONG_MATH(math_support_or_not, T, D) \
    _ZFP_ZFT_INT_STRONG_MATH_##math_support_or_not(T, D)
#define _ZFP_ZFT_INT_STRONG_MATH_math_not_support(T, D)
#define _ZFP_ZFT_INT_STRONG_MATH_math_support(T, D) \
    /** @cond ZFPrivateDoc */ \
    inline bool operator || (const D &v, const D &t_) {return ((v.t != 0) || (t_.t != 0));} \
    inline bool operator && (const D &v, const D &t_) {return ((v.t != 0) && (t_.t != 0));} \
    inline D operator + (const D &v, const D &t_) {return (D)(v.t + t_.t);} \
    inline D operator - (const D &v, const D &t_) {return (D)(v.t - t_.t);} \
    inline D operator / (const D &v, const D &t_) {return (D)(v.t / t_.t);} \
    template<typename T2> bool operator || (const D &v, const T2 &t_) {return ((v.t != 0) || t_);} \
    template<typename T2> bool operator && (const D &v, const T2 &t_) {return ((v.t != 0) && t_);} \
    template<typename T2> T2 operator % (const D &v, const T2 &t_) {return (T2)(v.t % t_);} \
    template<typename T2> bool operator || (const T2 &t_, const D &v) {return (t_ || (v.t != 0));} \
    template<typename T2> bool operator && (const T2 &t_, const D &v) {return (t_ && (v.t != 0));} \
    /** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_CoreIntType_h_

