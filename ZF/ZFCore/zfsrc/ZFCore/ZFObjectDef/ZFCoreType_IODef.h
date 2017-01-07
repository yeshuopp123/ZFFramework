/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreType_IODef.h
 * @brief type io with io callbacks
 */

#ifndef _ZFI_ZFCoreType_IODef_h_
#define _ZFI_ZFCoreType_IODef_h_

#include "ZFCallbackDef_common.h"
#include "ZFSerializableDef.h"
#include "ZFClassFilterDef.h"
#include "ZFMethodFilterDef.h"
#include "ZFPropertyFilterDef.h"
#include "zfautoObjectFwd.h"
#include "ZFAnyDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFCoreType_IODef_outputPointer() \
    {if(v) {output << v->objectInfo();} else {output << (const void *)zfnull;}}

// ============================================================
// common output for basic types
ZFOUTPUT_TYPE(const zfchar *, {output.execute(v ? v : ZFTOKEN_zfnull);})
ZFOUTPUT_TYPE(zfchar *, {output << (const zfchar *)v;})

#define _ZFP_ZFCoreType_IODef_int_DECLARE(T_Int) \
    ZFOUTPUT_TYPE_DECLARE(T_Int) \
    /** @brief see #ZFInputCallback */ \
    extern ZF_ENV_EXPORT zfbool zfintFromInput(ZF_OUT T_Int &v, \
                                               ZF_IN const ZFInputCallback &input, \
                                               ZF_IN_OPT zfindex radix = 10); \
    /** @cond ZFPrivateDoc */ \
    extern ZF_ENV_EXPORT const ZFInputCallback &operator >> (ZF_IN_OUT const ZFInputCallback &input, ZF_OUT T_Int &v); \
    /** @endcond */
_ZFP_ZFCoreType_IODef_int_DECLARE(short)
_ZFP_ZFCoreType_IODef_int_DECLARE(unsigned short)
_ZFP_ZFCoreType_IODef_int_DECLARE(int)
_ZFP_ZFCoreType_IODef_int_DECLARE(unsigned int)
_ZFP_ZFCoreType_IODef_int_DECLARE(long)
_ZFP_ZFCoreType_IODef_int_DECLARE(unsigned long)
_ZFP_ZFCoreType_IODef_int_DECLARE(long long)
_ZFP_ZFCoreType_IODef_int_DECLARE(unsigned long long)

/**
 * @brief settings for #zfintFromInput
 */
ZFCALLBACK_SETTING_DECLARE_BEGIN(zfintSettingForOutput)
public:
    ZFCORE_PARAM_DECLARE_SELF(zfintSettingForOutput)

public:
    /**
     * @brief radix to output integer values, 10 by default
     */
    ZFCORE_PARAM_WITH_INIT(zfindex, radix, 10)
    /**
     * @brief whether use upper case to output 'A' ~ 'Z' letters, true by default
     */
    ZFCORE_PARAM_WITH_INIT(zfbool, upperCase, zftrue)
ZFCALLBACK_SETTING_DECLARE_END(zfintSettingForOutput)
/** @cond ZFPrivateDoc */
inline const ZFOutputCallback &operator <<(ZF_IN_OUT const ZFOutputCallback &output, ZF_IN const zfintSettingForOutput &setting)
{
    zfintSettingForOutputSet(output, setting);
    return output;
}
/** @endcond */

/**
 * @brief settings for #zfintFromInput
 */
ZFCALLBACK_SETTING_DECLARE_BEGIN(zfintSettingForInput)
public:
    ZFCORE_PARAM_DECLARE_SELF(zfintSettingForInput)

public:
    /**
     * @brief radix to input integer values, 10 by default
     */
    ZFCORE_PARAM_WITH_INIT(zfindex, radix, 10)
ZFCALLBACK_SETTING_DECLARE_END(zfintSettingForInput)
/** @cond ZFPrivateDoc */
inline const ZFInputCallback &operator >>(ZF_IN_OUT const ZFInputCallback &input, ZF_IN const zfintSettingForInput &setting)
{
    zfintSettingForInputSet(input, setting);
    return input;
}
/** @endcond */

#define _ZFP_ZFCoreType_IODef_float_DECLARE(T_Float) \
    ZFOUTPUT_TYPE_DECLARE(T_Float) \
    /** @brief see #ZFInputCallback */ \
    extern ZF_ENV_EXPORT zfbool zffloatFromInput(ZF_OUT T_Float &v, \
                                               ZF_IN const ZFInputCallback &input); \
    /** @cond ZFPrivateDoc */ \
    extern ZF_ENV_EXPORT const ZFInputCallback &operator >> (ZF_IN_OUT const ZFInputCallback &input, ZF_OUT T_Float &v); \
    /** @endcond */
_ZFP_ZFCoreType_IODef_float_DECLARE(float)
_ZFP_ZFCoreType_IODef_float_DECLARE(double)
_ZFP_ZFCoreType_IODef_float_DECLARE(long double)

/**
 * @brief settings for #zffloatFromInput
 */
ZFCALLBACK_SETTING_DECLARE_BEGIN(zffloatSettingForOutput)
public:
    ZFCORE_PARAM_DECLARE_SELF(zffloatSettingForOutput)

public:
    /**
     * @brief precision to output float values, zfindexMax by default
     */
    ZFCORE_PARAM_WITH_INIT(zfindex, precision, zfindexMax)
ZFCALLBACK_SETTING_DECLARE_END(zffloatSettingForOutput)
/** @cond ZFPrivateDoc */
inline const ZFOutputCallback &operator <<(ZF_IN_OUT const ZFOutputCallback &output, ZF_IN const zffloatSettingForOutput &setting)
{
    zffloatSettingForOutputSet(output, setting);
    return output;
}
/** @endcond */

ZFOUTPUT_TYPE(const void *, {output.execute(v ? zfsFromPointer(v).cString() : ZFTOKEN_zfnull);})
ZFOUTPUT_TYPE(void *, {output << (const void *)v;})

ZFOUTPUT_TYPE(zfbool, {output << (v ? ZFTOKEN_zfbool_zftrue : ZFTOKEN_zfbool_zffalse);})
ZFOUTPUT_TYPE(const zfbool *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(zfbool *, {output << (const zfbool *)v;})
ZFINPUT_TYPE_DECLARE(zfbool, zfbool)

ZFOUTPUT_TYPE(zfchar, {output.execute(&v, 1);})
ZFINPUT_TYPE_DECLARE(zfchar, zfchar)

ZFOUTPUT_TYPE(zfstring, {output.execute(v.cString(), v.length());})
ZFOUTPUT_TYPE(const zfstring *, {if(v) {output.execute(v->cString(), v->length());} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(zfstring *, {output << (const zfstring *)v;})
ZFINPUT_TYPE_DECLARE(zfstring, zfstring)

/**
 * @brief settings for #zfstringFromInput
 */
ZFCALLBACK_SETTING_DECLARE_BEGIN(zfstringSettingForInput)
public:
    ZFCORE_PARAM_DECLARE_SELF(zfstringSettingForInput)

public:
    /**
     * @brief end input when reach any of these token, "CR, LF, space, tab, comma" by default
     */
    ZFCORE_PARAM_WITH_INIT(zfstring, endToken, zfText("\r\n \t"))
    /**
     * @brief escape token, backslash by default, 0 to disable
     */
    ZFCORE_PARAM_WITH_INIT(zfchar, escapeToken, '\\')
    /**
     * @brief left quote token, quote by default, 0 to disable
     */
    ZFCORE_PARAM_WITH_INIT(zfchar, quoteTokenLeft, '\"')
    /**
     * @brief right quote token, quote by default, 0 to disable
     */
    ZFCORE_PARAM_WITH_INIT(zfchar, quoteTokenRight, '\"')
    /**
     * @brief whether allow embeded quote with another quote, false by default
     */
    ZFCORE_PARAM_WITH_INIT(zfbool, quoteAllowEmbeded, zffalse)
    /**
     * @brief tokens to skip before read, "CR, LF, space, tab, comma" by default,
     *   typically this setting should be equal to endToken
     */
    ZFCORE_PARAM_WITH_INIT(zfstring, skipToken, zfText("\r\n \t"))
ZFCALLBACK_SETTING_DECLARE_END(zfstringSettingForInput)
/** @cond ZFPrivateDoc */
inline const ZFInputCallback &operator >>(ZF_IN_OUT const ZFInputCallback &input, ZF_IN const zfstringSettingForInput &setting)
{
    zfstringSettingForInputSet(input, setting);
    return input;
}
/** @endcond */

// ============================================================
// common output for core types
ZFOUTPUT_TYPE(ZFCompareResult, {output << ZFCompareResultToString(v);})
ZFOUTPUT_TYPE(const ZFCompareResult *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFCompareResult *, {output << (const ZFCompareResult *)v;})
ZFINPUT_TYPE_DECLARE(ZFCompareResult, ZFCompareResult)

ZFOUTPUT_TYPE(ZFSeekPos, {output << ZFSeekPosToString(v);})
ZFOUTPUT_TYPE(const ZFSeekPos *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFSeekPos *, {output << (const ZFSeekPos *)v;})
ZFINPUT_TYPE_DECLARE(ZFSeekPos, ZFSeekPos)

ZFOUTPUT_TYPE(zfindexRange, {output << zfindexRangeToString(v);})
ZFOUTPUT_TYPE(const zfindexRange *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(zfindexRange *, {output << (const zfindexRange *)v;})
ZFINPUT_TYPE_DECLARE(zfindexRange, zfindexRange)

ZFOUTPUT_TYPE_TEMPLATE(typename T_Public ZFM_COMMA() typename T_Internal,
                       ZFFilterBase<T_Public ZFM_COMMA() T_Internal>,
                       {output << v.objectInfo();})
ZFOUTPUT_TYPE_TEMPLATE(typename T_Public ZFM_COMMA() typename T_Internal,
                       const ZFFilterBase<T_Public ZFM_COMMA() T_Internal> *,
                       _ZFP_ZFCoreType_IODef_outputPointer())
ZFOUTPUT_TYPE_TEMPLATE(typename T_Public ZFM_COMMA() typename T_Internal,
                       ZFFilterBase<T_Public ZFM_COMMA() T_Internal> *,
                       {output << (const ZFFilterBase<T_Public, T_Internal> *)v;})

ZFOUTPUT_TYPE_TEMPLATE(typename T_Element,
                       ZFCoreArray<T_Element>,
                       {output << v.objectInfo();})
ZFOUTPUT_TYPE_TEMPLATE(typename T_Element,
                       const ZFCoreArray<T_Element> *,
                       _ZFP_ZFCoreType_IODef_outputPointer())
ZFOUTPUT_TYPE_TEMPLATE(typename T_Element,
                       ZFCoreArray<T_Element> *,
                       {output << (const ZFCoreArray<T_Element> *)v;})

ZFOUTPUT_TYPE_TEMPLATE(typename T_Element,
                       ZFCoreQueuePOD<T_Element>,
                       {output << v.objectInfo();})
ZFOUTPUT_TYPE_TEMPLATE(typename T_Element,
                       const ZFCoreQueuePOD<T_Element> *,
                       _ZFP_ZFCoreType_IODef_outputPointer())
ZFOUTPUT_TYPE_TEMPLATE(typename T_Element,
                       ZFCoreQueuePOD<T_Element> *,
                       {output << (const ZFCoreQueuePOD<T_Element> *)v;})

ZFOUTPUT_TYPE(ZFCoreMap, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFCoreMap *, _ZFP_ZFCoreType_IODef_outputPointer())
ZFOUTPUT_TYPE(ZFCoreMap *, {output << (const ZFCoreMap *)v;})

ZFOUTPUT_TYPE(ZFCorePointerBase, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFCorePointerBase *, _ZFP_ZFCoreType_IODef_outputPointer())
ZFOUTPUT_TYPE(ZFCorePointerBase *, {output << (const ZFCorePointerBase *)v;})

// ============================================================
// common output for ZFObject types
ZFOUTPUT_TYPE(ZFFilterType, {output << ZFFilterTypeToString(v);})
ZFOUTPUT_TYPE(const ZFFilterType *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFFilterType *, {output << (const ZFFilterType *)v;})
ZFINPUT_TYPE_DECLARE(ZFFilterType, ZFFilterType)

ZFOUTPUT_TYPE(ZFFilterCallbackResult, {output << ZFFilterCallbackResultToString(v);})
ZFOUTPUT_TYPE(const ZFFilterCallbackResult *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFFilterCallbackResult *, {output << (const ZFFilterCallbackResult *)v;})
ZFINPUT_TYPE_DECLARE(ZFFilterCallbackResult, ZFFilterCallbackResult)

ZFOUTPUT_TYPE(ZFClassFilterType, {output << ZFClassFilterTypeToString(v);})
ZFOUTPUT_TYPE(const ZFClassFilterType *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFClassFilterType *, {output << (const ZFClassFilterType *)v;})
ZFINPUT_TYPE_DECLARE(ZFClassFilterType, ZFClassFilterType)

ZFOUTPUT_TYPE(ZFClassFilter, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFClassFilter *, _ZFP_ZFCoreType_IODef_outputPointer())
ZFOUTPUT_TYPE(ZFClassFilter *, {output << (const ZFClassFilter *)v;})

ZFOUTPUT_TYPE(ZFMethodPrivilegeType, {output << ZFMethodPrivilegeTypeToString(v);})
ZFOUTPUT_TYPE(const ZFMethodPrivilegeType *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFMethodPrivilegeType *, {output << (const ZFMethodPrivilegeType *)v;})
ZFINPUT_TYPE_DECLARE(ZFMethodPrivilegeType, ZFMethodPrivilegeType)

ZFOUTPUT_TYPE(const ZFClass *, _ZFP_ZFCoreType_IODef_outputPointer())

ZFOUTPUT_TYPE(ZFObject *, _ZFP_ZFCoreType_IODef_outputPointer())

ZFOUTPUT_TYPE(const ZFMethod *, _ZFP_ZFCoreType_IODef_outputPointer())

ZFOUTPUT_TYPE(const ZFProperty *, _ZFP_ZFCoreType_IODef_outputPointer())

ZFOUTPUT_TYPE(ZFCallback, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFCallback *, _ZFP_ZFCoreType_IODef_outputPointer())
ZFOUTPUT_TYPE(ZFCallback *, {output << (const ZFCallback *)v;})

ZFOUTPUT_TYPE(ZFListenerData, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFListenerData *, _ZFP_ZFCoreType_IODef_outputPointer())
ZFOUTPUT_TYPE(ZFListenerData *, {output << (const ZFListenerData *)v;})

ZFOUTPUT_TYPE(ZFSerializableData, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFSerializableData *, _ZFP_ZFCoreType_IODef_outputPointer())
ZFOUTPUT_TYPE(ZFSerializableData *, {output << (const ZFSerializableData *)v;})

ZFOUTPUT_TYPE(ZFAny, {output << v.toObject();})
ZFOUTPUT_TYPE(const ZFAny *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFAny *, {output << (const ZFAny *)v;})

ZFOUTPUT_TYPE(zfautoObject, {output << v.toObject();})
ZFOUTPUT_TYPE(const zfautoObject *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(zfautoObject *, {output << (const zfautoObject *)v;})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreType_IODef_h_

