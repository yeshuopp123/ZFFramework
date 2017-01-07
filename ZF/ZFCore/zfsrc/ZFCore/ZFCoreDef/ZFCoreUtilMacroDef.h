/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreUtilMacroDef.h
 * @brief macro utils
 */
#ifndef _ZFI_ZFCoreUtilMacroDef_h_
#define _ZFI_ZFCoreUtilMacroDef_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief macro to detect whether bit is set, zftrue only if all the bits in bit is set
 *
 * note that test by 0 is always zftrue
 */
#define ZFBitTest(var, bit) (((var) & (bit)) == (bit))
/**
 * @brief macro to get the value at bit
 */
#define ZFBitGet(var, bit) ((var) & (bit))
/**
 * @brief macro to set at bit
 */
#define ZFBitSet(var, bit) ((var) |= (bit))
/**
 * @brief clear at bit
 */
#define ZFBitUnset(var, bit) ((var) &= (~(bit)))

// ============================================================
/**
 * @brief dummy macro to show a hint
 *
 * used to make some code more human readable, no actual use\n
 * for example, add to function call's param list to make params more readable:
 * @code
 *   void yourFunc(zfbool clickable, zfbool moveable) {...}
 *   ...
 *   yourFunc(
 *       zfHint("clickable?")zftrue,
 *       zfHint("moveable?")zffalse);
 * @endcode
 */
#define zfHint(hint)

// ============================================================
#define _ZFP_ZFM_REPEAT_0(Type, LeftCommaFix)
#define _ZFP_ZFM_REPEAT_1(Type, LeftCommaFix) LeftCommaFix() Type(0)
#define _ZFP_ZFM_REPEAT_2(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_1(Type, LeftCommaFix), Type(1)
#define _ZFP_ZFM_REPEAT_3(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_2(Type, LeftCommaFix), Type(2)
#define _ZFP_ZFM_REPEAT_4(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_3(Type, LeftCommaFix), Type(3)
#define _ZFP_ZFM_REPEAT_5(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_4(Type, LeftCommaFix), Type(4)
#define _ZFP_ZFM_REPEAT_6(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_5(Type, LeftCommaFix), Type(5)
#define _ZFP_ZFM_REPEAT_7(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_6(Type, LeftCommaFix), Type(6)
#define _ZFP_ZFM_REPEAT_8(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_7(Type, LeftCommaFix), Type(7)
#define _ZFP_ZFM_REPEAT_9(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_8(Type, LeftCommaFix), Type(8)
#define _ZFP_ZFM_REPEAT_10(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_9(Type, LeftCommaFix), Type(9)
#define _ZFP_ZFM_REPEAT_11(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_10(Type, LeftCommaFix), Type(10)
#define _ZFP_ZFM_REPEAT_12(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_11(Type, LeftCommaFix), Type(11)
#define _ZFP_ZFM_REPEAT_13(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_12(Type, LeftCommaFix), Type(12)
#define _ZFP_ZFM_REPEAT_14(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_13(Type, LeftCommaFix), Type(13)
#define _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_14(Type, LeftCommaFix), Type(14)
#define _ZFP_ZFM_REPEAT_16(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(15)
#define _ZFP_ZFM_REPEAT_17(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(16)
#define _ZFP_ZFM_REPEAT_18(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(17)
#define _ZFP_ZFM_REPEAT_19(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(18)
#define _ZFP_ZFM_REPEAT_20(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(19)
#define _ZFP_ZFM_REPEAT_21(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(20)
#define _ZFP_ZFM_REPEAT_22(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(21)
#define _ZFP_ZFM_REPEAT_23(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(22)
#define _ZFP_ZFM_REPEAT_24(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(23)
#define _ZFP_ZFM_REPEAT_25(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(24)
#define _ZFP_ZFM_REPEAT_26(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(25)
#define _ZFP_ZFM_REPEAT_27(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(26)
#define _ZFP_ZFM_REPEAT_28(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(27)
#define _ZFP_ZFM_REPEAT_29(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(28)
#define _ZFP_ZFM_REPEAT_30(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(29)
#define _ZFP_ZFM_REPEAT_31(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(30)
#define _ZFP_ZFM_REPEAT_32(Type, LeftCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix), Type(31)
#define _ZFP_ZFM_REPEAT(N, Type, LeftCommaFix) _ZFP_ZFM_REPEAT_##N(Type, LeftCommaFix)
/**
 * @brief macro to repeat something
 *
 * for example\n
 *   ZFM_REPEAT(2, ZFMARCO_REPEAT_TEMPLATE, ZFM_EMPTY)\n
 * would be expand as:\n
 *   typename Type0, typename Type1\n
 * and
 *   ZFM_REPEAT(3, ZFM_REPEAT_PARAM, ZFM_COMMA)\n
 * would be expand as:\n
 *   , Type0 param0, Type1 param1, Type2 param2
 */
#define ZFM_REPEAT(N, Type, LeftCommaFix) _ZFP_ZFM_REPEAT(N, Type, LeftCommaFix)

#define _ZFP_ZFMARCO_REPEAT_TEMPLATE(N) typename Type##N
#define _ZFP_ZFM_REPEAT_TYPE(N) Type##N
#define _ZFP_ZFM_REPEAT_TYPE_CONST(N) Type##N const &
#define _ZFP_ZFM_REPEAT_NAME(N) param##N
#define _ZFP_ZFM_REPEAT_PARAM(N) Type##N param##N

/**
 * @brief usually used by #ZFM_REPEAT
 */
#define ZFMARCO_REPEAT_TEMPLATE(N) _ZFP_ZFMARCO_REPEAT_TEMPLATE(N)
/**
 * @brief usually used by #ZFM_REPEAT
 */
#define ZFM_REPEAT_TYPE(N) _ZFP_ZFM_REPEAT_TYPE(N)
/**
 * @brief usually used by #ZFM_REPEAT
 */
#define ZFM_REPEAT_TYPE_CONST(N) _ZFP_ZFM_REPEAT_TYPE_CONST(N)
/**
 * @brief usually used by #ZFM_REPEAT
 */
#define ZFM_REPEAT_NAME(N) _ZFP_ZFM_REPEAT_NAME(N)
/**
 * @brief usually used by #ZFM_REPEAT
 */
#define ZFM_REPEAT_PARAM(N) _ZFP_ZFM_REPEAT_PARAM(N)

// ============================================================
/**
 * @brief macro to (
 */
#define ZFM_BRACKET_L() (
/**
 * @brief macro to )
 */
#define ZFM_BRACKET_R() )

/**
 * @brief macro to s space
 */
#define ZFM_EMPTY(...)
/**
 * @brief macro to a comma
 */
#define ZFM_COMMA() ,

/**
 * @brief macro to expand a macro
 */
#define ZFM_EXPAND(...) __VA_ARGS__

/**
 * @brief concatenate a and b to ab without expand a or b
 */
#define ZFM_CAT_DIRECTLY(a, b) a##b
/**
 * @brief expand a and b, then concatenate to ab
 */
#define ZFM_CAT(a, b) ZFM_CAT_DIRECTLY(a, b)

/**
 * @brief convert a to zfcharA * without expand
 */
#define ZFM_TOSTRING_A_DIRECTLY(a) #a
/**
 * @brief expand a, then convert to zfcharA *
 */
#define ZFM_TOSTRING_A(a) ZFM_TOSTRING_A_DIRECTLY(a)

/**
 * @brief convert a to zfcharW * without expand
 */
#define ZFM_TOSTRING_W_DIRECTLY(a) zfTextW(#a)
/**
 * @brief expand a, then convert to zfcharW *
 */
#define ZFM_TOSTRING_W(a) ZFM_TOSTRING_W_DIRECTLY(a)

/**
 * @brief zfchar version of #ZFM_TOSTRING_A_DIRECTLY
 */
#define ZFM_TOSTRING_DIRECTLY(a) zfText(#a)
/**
 * @brief zfchar version of #ZFM_TOSTRING_A
 */
#define ZFM_TOSTRING(a) ZFM_TOSTRING_DIRECTLY(a)

// ============================================================
#define _ZFP_ZFM_N_INC_T(n) _ZFP_ZFM_N_INC_##n
#define _ZFP_ZFM_N_INC_0 1
#define _ZFP_ZFM_N_INC_1 2
#define _ZFP_ZFM_N_INC_2 3
#define _ZFP_ZFM_N_INC_3 4
#define _ZFP_ZFM_N_INC_4 5
#define _ZFP_ZFM_N_INC_5 6
#define _ZFP_ZFM_N_INC_6 7
#define _ZFP_ZFM_N_INC_7 8
#define _ZFP_ZFM_N_INC_8 9
#define _ZFP_ZFM_N_INC_9 10
#define _ZFP_ZFM_N_INC_10 11
#define _ZFP_ZFM_N_INC_11 12
#define _ZFP_ZFM_N_INC_12 13
#define _ZFP_ZFM_N_INC_13 14
#define _ZFP_ZFM_N_INC_14 15
#define _ZFP_ZFM_N_INC_15 16
#define _ZFP_ZFM_N_INC_16 17
#define _ZFP_ZFM_N_INC_17 18
#define _ZFP_ZFM_N_INC_18 19
#define _ZFP_ZFM_N_INC_19 20
#define _ZFP_ZFM_N_INC_20 21
#define _ZFP_ZFM_N_INC_21 22
#define _ZFP_ZFM_N_INC_22 23
#define _ZFP_ZFM_N_INC_23 24
#define _ZFP_ZFM_N_INC_24 25
#define _ZFP_ZFM_N_INC_25 26
#define _ZFP_ZFM_N_INC_26 27
#define _ZFP_ZFM_N_INC_27 28
#define _ZFP_ZFM_N_INC_28 29
#define _ZFP_ZFM_N_INC_29 30
#define _ZFP_ZFM_N_INC_30 31
#define _ZFP_ZFM_N_INC_31 32
/**
 * @brief increase n by macro expand
 */
#define ZFM_N_INC(n) _ZFP_ZFM_N_INC_T(n)

#define _ZFP_ZFM_N_DEC_T(n) _ZFP_ZFM_N_DEC_##n
#define _ZFP_ZFM_N_DEC_0 -1
#define _ZFP_ZFM_N_DEC_1 0
#define _ZFP_ZFM_N_DEC_2 1
#define _ZFP_ZFM_N_DEC_3 2
#define _ZFP_ZFM_N_DEC_4 3
#define _ZFP_ZFM_N_DEC_5 4
#define _ZFP_ZFM_N_DEC_6 5
#define _ZFP_ZFM_N_DEC_7 6
#define _ZFP_ZFM_N_DEC_8 7
#define _ZFP_ZFM_N_DEC_9 8
#define _ZFP_ZFM_N_DEC_10 9
#define _ZFP_ZFM_N_DEC_11 10
#define _ZFP_ZFM_N_DEC_12 11
#define _ZFP_ZFM_N_DEC_13 12
#define _ZFP_ZFM_N_DEC_14 13
#define _ZFP_ZFM_N_DEC_15 14
#define _ZFP_ZFM_N_DEC_16 15
#define _ZFP_ZFM_N_DEC_17 16
#define _ZFP_ZFM_N_DEC_18 17
#define _ZFP_ZFM_N_DEC_19 18
#define _ZFP_ZFM_N_DEC_20 19
#define _ZFP_ZFM_N_DEC_21 20
#define _ZFP_ZFM_N_DEC_22 21
#define _ZFP_ZFM_N_DEC_23 22
#define _ZFP_ZFM_N_DEC_24 23
#define _ZFP_ZFM_N_DEC_25 24
#define _ZFP_ZFM_N_DEC_26 25
#define _ZFP_ZFM_N_DEC_27 26
#define _ZFP_ZFM_N_DEC_28 27
#define _ZFP_ZFM_N_DEC_29 28
#define _ZFP_ZFM_N_DEC_30 29
#define _ZFP_ZFM_N_DEC_31 30
#define _ZFP_ZFM_N_DEC_32 31
/**
 * @brief decrease n by macro expand
 */
#define ZFM_N_DEC(n) _ZFP_ZFM_N_DEC_T(n)

ZF_ENV_SENSITIVE
/**
 * @brief get the param num of a certain sequence
 *
 * usage:
 * @code
 *   #define _EMPTY
 *   zfindex n = 0;
 *   n = ZFM_PARAM_NUM(a, b, c); // n would be 3
 *   n = ZFM_PARAM_NUM(); // n would be 0
 *   n = ZFM_PARAM_NUM(_EMPTY); // n would be 1
 * @endcode
 * this macro could calculate 0 ~ 32 param num
 */
#if defined(_MSC_VER)
    #define ZFM_PARAM_NUM(...) \
        _ZFP_ZFM_PARAM_NUM_TMP0 ZFM_BRACKET_L() \
            dummy, ##__VA_ARGS__, _ZFP_ZFM_PARAM_NUM_TMP2() \
        ZFM_BRACKET_R()
    #define _ZFP_ZFM_PARAM_NUM_TMP0(...) \
        _ZFP_ZFM_PARAM_NUM_TMP1 ZFM_BRACKET_L() __VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_PARAM_NUM_TMP1(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, N, ...) \
        N
    #define _ZFP_ZFM_PARAM_NUM_TMP2() \
        32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#else // #if defined(_MSC_VER)
    #define ZFM_PARAM_NUM(...) \
        _ZFP_ZFM_PARAM_NUM_TMP0(dummy, ##__VA_ARGS__, _ZFP_ZFM_PARAM_NUM_TMP2())
    #define _ZFP_ZFM_PARAM_NUM_TMP0(...) \
        _ZFP_ZFM_PARAM_NUM_TMP1(__VA_ARGS__)
    #define _ZFP_ZFM_PARAM_NUM_TMP1(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, N, ...) \
        N
    #define _ZFP_ZFM_PARAM_NUM_TMP2() \
        32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#endif // #if defined(_MSC_VER)

ZF_ENV_SENSITIVE
/**
 * @brief fix each param in the __VA_ARGS__
 *
 * usage:
 * @code
 *   // define your own expand method
 *   #define EXPAND(arg, index, total) #arg " "
 *
 *   // result is:
 *   //   "1" " " "2" " "  "3" " "
 *   // which is same as:
 *   //   "1 2 3 "
 *   ZFM_FIX_PARAM(EXPAND, ZFM_EMPTY, 1, 2, 3)
 * @endcode
 */
#if defined(_MSC_VER)
    #define ZFM_FIX_PARAM(ParamFix, CommaFix, ...) _ZFP_ZFM_FIX_PARAM ZFM_BRACKET_L() ParamFix, CommaFix, ZFM_PARAM_NUM(__VA_ARGS__), 0, ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_0(ParamFix, CommaFix, total, n, index, ...)
    #define _ZFP_ZFM_FIX_PARAM_1(ParamFix, CommaFix, total, n, index, arg) ParamFix(arg, index, total)
    #define _ZFP_ZFM_FIX_PARAM_2(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_3(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_4(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_5(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_6(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_7(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_8(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_9(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_10(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_11(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_12(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_13(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_14(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_15(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_16(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_17(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_18(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_19(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_20(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_21(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_22(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_23(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_24(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_25(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_26(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_27(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_28(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_29(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_30(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_31(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM_32(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ ZFM_BRACKET_R()
    #define _ZFP_ZFM_FIX_PARAM(ParamFix, CommaFix, n, index, ...) ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) ZFM_BRACKET_L() ParamFix, CommaFix, n, ZFM_N_DEC(n), index, ##__VA_ARGS__ ZFM_BRACKET_R()
#else // #if defined(_MSC_VER)
    #define ZFM_FIX_PARAM(ParamFix, CommaFix, ...) _ZFP_ZFM_FIX_PARAM(ParamFix, CommaFix, ZFM_PARAM_NUM(__VA_ARGS__), 0, ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_0(ParamFix, CommaFix, total, n, index, ...)
    #define _ZFP_ZFM_FIX_PARAM_1(ParamFix, CommaFix, total, n, index, arg) ParamFix(arg, index, total)
    #define _ZFP_ZFM_FIX_PARAM_2(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_3(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_4(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_5(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_6(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_7(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_8(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_9(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_10(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_11(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_12(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_13(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_14(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_15(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_16(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_17(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_18(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_19(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_20(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_21(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_22(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_23(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_24(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_25(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_26(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_27(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_28(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_29(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_30(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_31(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM_32(ParamFix, CommaFix, total, n, index, arg, ...) ParamFix(arg, index, total) CommaFix() ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, total, ZFM_N_DEC(n), ZFM_N_INC(index), ##__VA_ARGS__ )
    #define _ZFP_ZFM_FIX_PARAM(ParamFix, CommaFix, n, index, ...) ZFM_CAT(_ZFP_ZFM_FIX_PARAM_, n) (ParamFix, CommaFix, n, ZFM_N_DEC(n), index, ##__VA_ARGS__ )
#endif // #if defined(_MSC_VER)

// ============================================================
#define _ZFP_zfidentityHash_EXPAND(v, index, total) ^ (zfidentity)(v)
/**
 * @brief util method to connect multiple hash value into one hash value
 */
#define zfidentityHash(hash, ...) \
    ((zfidentity)(hash) ZFM_FIX_PARAM(_ZFP_zfidentityHash_EXPAND, ZFM_EMPTY, ##__VA_ARGS__))

// ============================================================
/**
 * @brief explicit declare v is not used
 */
#define ZFUNUSED(v) ((void)(v))

// ============================================================
/**
 * @brief calculate the size of an array
 */
#define ZFM_ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

// ============================================================
/**
 * @brief declare a unique name
 *
 * typical usage:
 * @code
 *   // assume this line is line 1
 *   int ZFUniqueName(a) = 0; // same as "int _ZFP_zUniqueName_a_2 = 0;"
 *   int ZFUniqueName(a) = 0; // same as "int _ZFP_zUniqueName_a_3 = 0;"
 * @endcode
 * this is usually used to declare a tmp variable, such as:
 * @code
 *   class MyClass
 *   {
 *   public:
 *       zfbool flag;
 *       MyClass() : flag(zftrue) {}
 *   };
 *   #define MyClassBlock for(MyClass ZFUniqueName(cls); \\
 *       ZFUniqueName(cls).flag; \\
 *       ZFUniqueName(cls).flag = zffalse)
 *
 *   // to use the blocked code
 *   MyClassBlock
 *   {
 *       // code block
 *       // there'll be a instance of MyClass named _ZFP_zUniqueName_cls_123
 *   } // and the MyClass's instance will be deconstructed after the brace
 * @endcode
 */
#define ZFUniqueName(name) _ZFP_ZFUniqueName2(name, __LINE__)
#define _ZFP_ZFUniqueName2(name, line) _ZFP_ZFUniqueName3(name, line)
#define _ZFP_ZFUniqueName3(name, line) _ZFP_zfunique_##name##_##line

// ============================================================
/**
 * @brief util macro to check whether class has desired member function,
 *   for advanced use only
 *
 * usage:
 * @code
 *   // declare before use
 *   ZFM_CLASS_HAS_MEMBER_DECLARE(
 *       YourNamespace, // namespace for avoid conflict
 *       yourMemberName, // member method name to check
 *       void (T::*F)(void) // member sig (T and F is reserved keyword for class and member type)
 *   )
 *
 *   // once declared, you may check like this
 *   // value would be true only if "ClassToCheck" has such member method:
 *   // void ClassToCheck::yourMethodName(void); // can be static or virtual
 *   zfbool value = ZFM_CLASS_HAS_MEMBER(YourNamespace, yourMethodName, ClassToCheck);
 * @endcode
 */
#define ZFM_CLASS_HAS_MEMBER_DECLARE(NameSpace, memberName, memberSig) \
    /** @cond ZFPrivateDoc */ \
    template<typename T_Owner> \
    zfclassNotPOD _ZFP_ZFM_CLASS_HAS_MEMBER_##NameSpace##_##memberName \
    { \
    private: \
        template <typename T, T> struct _Fix; \
        template <typename T> \
        zfclassNotPOD _Sig \
        { \
        public: \
            typedef memberSig; \
        }; \
        template<typename T> static char _has(_Fix<typename _Sig<T>::F, &T::memberName> *); \
        template<typename T> static int _has(...); \
    public: \
        enum { value = (sizeof(_has<T_Owner>(0)) == sizeof(char)) }; \
    }; \
    /** @endcond */
/**
 * @brief see #ZFM_CLASS_HAS_MEMBER_DECLARE
 */
#define ZFM_CLASS_HAS_MEMBER(NameSpace, memberName, ClassToCheck) \
    (_ZFP_ZFM_CLASS_HAS_MEMBER_##NameSpace##_##memberName<ClassToCheck>::value != 0)

// ============================================================
/**
 * @brief see #ZFCORE_PARAM
 */
#define ZFCORE_PARAM_DECLARE_SELF(T_self) \
    protected: \
        /** @cond ZFPrivateDoc */ \
        typedef T_self zfself; \
        /** @endcond */ \
    public:
/**
 * @brief a util macro to generate setter and getter for POD like object
 *
 * usage:
 * @code
 *   zfclassLikePOD YourType
 *   {
 *   protected:
 *       / **
 *        * @ brief must have self protected typedef-ed as zfself
 *        * /
 *       typedef YourType zfself;
 *       // or use this macro for short
 *       // ZFCORE_PARAM_DECLARE_SELF(YourType)
 *
 *       / **
 *        * @ brief you can add Doxygen docs here
 *        * /
 *       ZFCORE_PARAM(YourType, YourParamType, paramName)
 *   };
 * @endcode
 * once declared, you can access your param by the generated getter and setter
 * @code
 *   YourType v;
 *   v.paramNameSet(someValue);
 *   YourParamType paramValue = v.paramName();
 * @endcode
 */
#define ZFCORE_PARAM(T_ParamType, paramName) \
    ZFCORE_PARAM_WITH_INIT(T_ParamType, paramName, ZFM_EMPTY())
/**
 * @brief see #ZFCORE_PARAM
 */
#define ZFCORE_PARAM_WITH_INIT(T_ParamType, paramName, initValue) \
    public: \
        T_ParamType const &paramName(void) const \
        { \
            return this->_ZFP_##paramName.value; \
        } \
        /** @brief see @ref paramName */ \
        T_ParamType &paramName(void) \
        { \
            return this->_ZFP_##paramName.value; \
        } \
    private: \
        /** @cond ZFPrivateDoc */ \
        zfclassLikePOD ZF_ENV_EXPORT _ZFP_T_##paramName \
        { \
        public: \
            _ZFP_T_##paramName(void) \
            : value(initValue) \
            { \
            } \
            _ZFP_T_##paramName(ZF_IN const _ZFP_T_##paramName &ref) \
            : value(ref.value) \
            { \
            } \
            _ZFP_T_##paramName &operator = (ZF_IN const _ZFP_T_##paramName &ref) \
            { \
                this->value = ref.value; \
                return *this; \
            } \
        public: \
            T_ParamType value; \
        }; \
        _ZFP_T_##paramName _ZFP_##paramName; \
        /** @endcond */ \
    public: \
        /** @brief see @ref paramName */ \
        zfself &paramName##Set(ZF_IN T_ParamType const &value) \
        { \
            this->_ZFP_##paramName.value = value; \
            return *this; \
        }

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreUtilMacroDef_h_

