/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCallbackDef_common.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFCallbackDef_common_h_
#define _ZFI_ZFCallbackDef_common_h_

#include "ZFCallbackDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief for advance user only
 *
 * for ZFOutputCallback and ZFInputCallback to achieve some additional operation\n
 * \n
 * usage:\n
 * set an auto released data by #ZFCallback::callbackTagSet,
 * whose key is #ZFCallbackTagKeyword_ioOwner and value is a ZFObject
 * that optionally implements those method (as #ZFMethod):
 * -  ioSeek, similar to FILE's fileSeek, proto type:\n
 *   zfindex ioSeek(ZF_IN zfindex byteSize,
 *                  ZF_IN ZFSeekPos pos);\n
 *   return newly seeked position or zfindexMax if the callback doesn't support seek or error occurred
 * -  ioTell, similar to FILE's fileTell, proto type:\n
 *   zfindex ioTell(void);\n
 *   return current's index or zfindexMax if the callback doesn't support seek
 * -  ioSize, calculate callback's size, proto type:\n
 *   zfindex ioSize(void);\n
 *   return size or zfindexMax if the callback doesn't support
 *   @note for input callbacks, the size shows the current available size, may change after a ioSeek or execute call
 *   @note for ouput callbacks, the size shows the contents outputed to the output callback
 */
#define ZFCallbackTagKeyword_ioOwner zfText("ZFCallbackTagKeyword_ioOwner")
/**
 * @brief util method for ZFIOCallback::ioSeek's implementations to calculate seek index
 *
 * param:\n
 * -  (zfindex)offset callback's initial offset
 * -  (zfindex)length callback's length including offset (i.e. starting from 0)
 * -  (zfindex)curPos callback's current position including offset (i.e. start from 0)
 *
 * return an index start from 0 (offset is included), ensured range in [offset, length]
 */
extern ZF_ENV_EXPORT zfindex ZFIOCallbackCalcFSeek(ZF_IN zfindex offset,
                                                   ZF_IN zfindex length,
                                                   ZF_IN zfindex curPos,
                                                   ZF_IN zfindex seekByteSize,
                                                   ZF_IN ZFSeekPos seekPos);

// ============================================================
/**
 * @brief abstract base class, use #ZFOutputCallback or #ZFInputCallback
 */
ZFCALLBACK_DECLARE_BEGIN(ZFIOCallback, ZFCallback)
public:
    /**
     * @brief similar to FILE's fileSeek
     *
     * if seek out of range, no error would occurred and seek would try to seek the proper direction to the end\n
     * for some output callbacks, seek would cause outputed data being removed\n
     * return newly seeked position or zfindexMax if the callback doesn't support seek
     */
    virtual zfindex ioSeek(ZF_IN zfindex byteSize,
                           ZF_IN_OPT ZFSeekPos pos = ZFSeekPosBegin) const;
    /**
     * @brief similar to FILE's fileTell, return current's index or zfindexMax if the callback doesn't support seek
     */
    virtual zfindex ioTell(void) const;
    /**
     * @brief calculate the callback's size or return zfindexMax if not supported
     */
    virtual zfindex ioSize(void) const;
ZFCALLBACK_DECLARE_END(ZFIOCallback, ZFCallback)

// ============================================================
// ZFOutputCallback
/**
 * @brief general output callback
 *
 * proto type:
 * -  zfindex output(const void *, zfindex);
 *
 * params:
 * -  (const void *) output buffer
 * -  (zfindex) count, or zfindexMax to calculate by src
 *  (src must be NULL-terminated)
 *
 * return:
 * -  char count written to output,
 *   or 0 if nothing to write or error occurred,
 *   or zfindexMax if the output doesn't care the write result,
 *   so that only (result < desired) is an error state
 *
 * ADVANCED:\n
 * for implementations, see #ZFCallbackTagKeyword_ioOwner if need support seek
 */
ZFCALLBACK_DECLARE_BEGIN(ZFOutputCallback, ZFIOCallback)
public:
    /** @brief see #ZFOutputCallback */
    inline zfindex execute(ZF_IN const void *src,
                           ZF_IN_OPT zfindex count = zfindexMax) const
    {
        return ZFCallback::executeExact<zfindex, const void *, zfindex>(src, count);
    }
    /** @brief see #ZFOutputCallback */
    inline zfindex operator()(ZF_IN const void *src,
                              ZF_IN_OPT zfindex count = zfindexMax) const
    {
        return ZFCallback::executeExact<zfindex, const void *, zfindex>(src, count);
    }
ZFCALLBACK_DECLARE_END(ZFOutputCallback, ZFIOCallback)

// ============================================================
// custom type output
#define _ZFP_ZFOUTPUT_EXPAND(T_Type) \
    template<> \
    zfclassNotPOD ZFCoreElementInfoGetter<T_Type> \
    { \
    public: \
        static void elementInfoGetter(ZF_IN_OUT zfstring &ret, ZF_IN T_Type const &v) \
        { \
            ZFCoreElementTokenGetter<T_Type>::elementTokenGetterLeft(ret, v); \
            { \
                ZFOutputCallbackForString(ret) << v; \
            } \
            ZFCoreElementTokenGetter<T_Type>::elementTokenGetterRight(ret, v); \
        } \
    };
/**
 * @brief declare your custom type to output to #ZFOutputCallback
 *
 * proto type:
 * @code
 *   const ZFOutputCallback &operator << (ZF_IN_OUT const ZFOutputCallback &output, ZF_IN YourType const &v);
 * @endcode
 * usage:
 * @code
 *   ZFOUTPUT_TYPE(YourType, {output.execute(v.toString());})
 * @endcode
 *
 * once declared, you may output your type to #ZFOutputCallback by:
 * @code
 *   output << yourObject;
 * @endcode
 */
#define ZFOUTPUT_TYPE(T_Type, outputAction) \
    /** @cond ZFPrivateDoc */ \
    inline const ZFOutputCallback &operator <<(const ZFOutputCallback &output, T_Type const &v) \
    { \
        if(output.callbackIsValid()) \
        outputAction \
        return output; \
    } \
    _ZFP_ZFOUTPUT_EXPAND(T_Type) \
    /** @endcond */
/**
 * @brief see #ZFOUTPUT_TYPE
 *
 * usage:
 * @code
 *   ZFOUTPUT_TYPE_TEMPLATE(typename T0 ZFM_COMMA() typename T1,
 *                          YourType<T0 ZFM_COMMA() T1>,
 *                          {output.execute(v.toString())})
 * @endcode
 */
#define ZFOUTPUT_TYPE_TEMPLATE(templateList, T_Type, outputAction) \
    /** @cond ZFPrivateDoc */ \
    template<templateList> \
    inline const ZFOutputCallback &operator <<(const ZFOutputCallback &output, T_Type const &v) \
    { \
        outputAction \
        return output; \
    } \
    template<templateList> \
    zfclassNotPOD ZFCoreElementInfoGetter<T_Type> \
    { \
    public: \
        static void elementInfoGetter(ZF_IN_OUT zfstring &ret, ZF_IN T_Type const &v) \
        { \
            ZFCoreElementTokenGetter<T_Type>::elementTokenGetterLeft(ret, v); \
            { \
                ZFOutputCallbackForString(ret) << v; \
            } \
            ZFCoreElementTokenGetter<T_Type>::elementTokenGetterRight(ret, v); \
        } \
    }; \
    /** @endcond */

/**
 * @brief see #ZFOUTPUT_TYPE
 *
 * usage:
 * @code
 *   // in header file
 *   ZFOUTPUT_TYPE_DECLARE(YourType)
 *   // in source file
 *   ZFOUTPUT_TYPE_DEFINE(YourType, {doYourStuff();})
 *
 *   // or, the inline version
 *   ZFOUTPUT_TYPE(YourType, {doYourStuff();})
 * @endcode
 */
#define ZFOUTPUT_TYPE_DECLARE(T_Type) \
    /** @cond ZFPrivateDoc */ \
    extern ZF_ENV_EXPORT const ZFOutputCallback &operator <<(const ZFOutputCallback &output, T_Type const &v); \
    _ZFP_ZFOUTPUT_EXPAND(T_Type) \
    /** @endcond */
/** @brief see #ZFOUTPUT_TYPE_DECLARE */
#define ZFOUTPUT_TYPE_DEFINE(T_Type, outputAction) \
    /** @cond ZFPrivateDoc */ \
    const ZFOutputCallback &operator <<(const ZFOutputCallback &output, T_Type const &v) \
    { \
        outputAction \
        return output; \
    } \
    /** @endcond */

// ============================================================
// common output callbacks
extern ZF_ENV_EXPORT ZFOutputCallback _ZFP_ZFOutputCallbackForString(ZF_IN const zfcharA *ownerFilePath,
                                                                     ZF_IN const zfcharA *ownerFunctionName,
                                                                     ZF_IN zfindex ownerFileLine,
                                                                     ZF_IN_OUT zfstring &s);
extern ZF_ENV_EXPORT ZFOutputCallback _ZFP_ZFOutputCallbackForBuffer(ZF_IN const zfcharA *ownerFilePath,
                                                                     ZF_IN const zfcharA *ownerFunctionName,
                                                                     ZF_IN zfindex ownerFileLine,
                                                                     ZF_IN void *buf,
                                                                     ZF_IN_OPT zfindex maxCount = zfindexMax,
                                                                     ZF_IN_OPT zfbool autoAppendNullToken = zftrue);
/**
 * @brief create a output callback to output to a zfstring
 *
 * params:
 * -  (zfstring &) a zfstring to output
 *
 * @note you must ensure the string to output is alive while the callback is still under use
 */
#define ZFOutputCallbackForString(s) \
    _ZFP_ZFOutputCallbackForString(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, s)
/**
 * @brief create a output callback to output to a buffer
 *
 * params:
 * -  (void *) buffer to write to
 * -  (zfindex) max buffer size or zfindexMax for no limit (you must make sure buffer is enough)
 * -  (zfbool) whether auto append '\0' to tail each time write
 */
#define ZFOutputCallbackForBuffer(buf, ...) \
    _ZFP_ZFOutputCallbackForBuffer(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, buf, ##__VA_ARGS__)

// ============================================================
// ZFInputCallback
/**
 * @brief general input callback
 *
 * proto type:
 * -  zfindex input(void *, zfindex);
 *
 * params:
 * -  (void *) buffer to write to,
 *   do nothing and return 0 if null
 * -  (zfindex) max read count (excluding '\0')
 *
 * return:
 * -  (zfindex) char count actually read (excluding '\0', may less than desired count such as EOF),
 *   or 0 if nothing to read (such as EOF) or error occurred
 *
 * ADVANCED:\n
 * for implementations, see #ZFCallbackTagKeyword_ioOwner if need support seek
 */
ZFCALLBACK_DECLARE_BEGIN(ZFInputCallback, ZFIOCallback)
public:
    /** @brief see #ZFInputCallback */
    inline zfindex execute(ZF_OUT void *buf,
                           ZF_IN zfindex count) const
    {
        return ZFCallback::executeExact<zfindex, void *, zfindex>(buf, count);
    }
    /** @brief see #ZFInputCallback */
    inline zfindex operator()(ZF_OUT void *buf,
                              ZF_IN zfindex count) const
    {
        return ZFCallback::executeExact<zfindex, void *, zfindex>(buf, count);
    }
ZFCALLBACK_DECLARE_END(ZFInputCallback, ZFIOCallback)

// ============================================================
// custom type input
/**
 * @brief declare your custom type to input from #ZFInputCallback
 *
 * proto type:
 * @code
 *   zfbool YourTypeNameFromInput(ZF_OUT YourType &v, ZF_IN const ZFInputCallback &input);
 * @endcode
 * usage:
 * @code
 *   ZFINPUT_TYPE(YourTypeName, YourType, {doYourStuff(); return successOrNot;})
 * @endcode
 *
 * once declared, you may read your type from #ZFInputCallback by:
 * @code
 *   input >> yourObject;
 *   YourTypeNameFromInput(yourObject, input);
 * @endcode
 */
#define ZFINPUT_TYPE(T_TypeName, T_Type, inputAction) \
    /** @brief see #ZFInputCallback */ \
    inline zfbool T_TypeName##FromInput(ZF_OUT T_Type &v, ZF_IN const ZFInputCallback &input) \
    { \
        if(!input.callbackIsValid()) \
        { \
            return zffalse; \
        } \
        inputAction \
    } \
    /** @cond ZFPrivateDoc */ \
    inline const ZFInputCallback &operator >> (ZF_IN_OUT const ZFInputCallback &input, ZF_OUT T_Type &v) \
    { \
        T_TypeName##FromInput(v, input); \
        return input; \
    } \
    /** @endcond */
/**
 * @brief see #ZFINPUT_TYPE
 *
 * usage:
 * @code
 *   // in header file
 *   ZFINPUT_TYPE_DECLARE(YourType)
 *   // in source file
 *   ZFINPUT_TYPE_DEFINE(YourType, {doYourStuff();})
 *
 *   // or, the inline version
 *   ZFINPUT_TYPE(YourType, {doYourStuff();})
 * @endcode
 */
#define ZFINPUT_TYPE_DECLARE(T_TypeName, T_Type) \
    /** @brief see #ZFInputCallback */ \
    extern ZF_ENV_EXPORT zfbool T_TypeName##FromInput(ZF_OUT T_Type &v, ZF_IN const ZFInputCallback &input); \
    /** @cond ZFPrivateDoc */ \
    inline const ZFInputCallback &operator >> (ZF_IN_OUT const ZFInputCallback &input, ZF_OUT T_Type &v) \
    { \
        T_TypeName##FromInput(v, input); \
        return input; \
    } \
    /** @endcond */
/** @brief see #ZFINPUT_TYPE_DECLARE */
#define ZFINPUT_TYPE_DEFINE(T_TypeName, T_Type, inputAction) \
    zfbool T_TypeName##FromInput(ZF_OUT T_Type &v, ZF_IN const ZFInputCallback &input) \
    { \
        if(!input.callbackIsValid()) \
        { \
            return zffalse; \
        } \
        inputAction \
    }

// ============================================================
/**
 * @brief util method to read all contents from input to a buffer
 *
 * returned buffer is ensured null-terminated,
 * and buffer's size shows the content's size excluding tail '\0'\n
 * return null buffer if input invalid or input error
 */
extern ZF_ENV_EXPORT ZFBuffer ZFInputCallbackReadToBuffer(ZF_IN_OUT const ZFInputCallback &input);

/**
 * @brief util method to read all contents of input to output
 *
 * return size already written to output even if error occurred
 */
extern ZF_ENV_EXPORT zfindex ZFInputCallbackReadToOutput(ZF_IN_OUT const ZFInputCallback &input,
                                                         ZF_IN_OUT const ZFOutputCallback &output);

/**
 * @brief util method to skip chars in set
 *
 * return first char that doesn't match charSet, or 0 if end before match or error occurred
 */
extern ZF_ENV_EXPORT zfchar ZFInputCallbackSkipChars(ZF_IN_OUT const ZFInputCallback &input,
                                                     ZF_IN_OPT const zfchar *charSet = zfText(" \t\r\n"));

/**
 * @brief read until any char in charSet shows up, or reached maxCount,
 *   return count read so far
 *
 * the first char matched charSet would be read and discarded,
 * and you may check it by firstCharMatchedCharSet,
 * if reached end or maxCount before matched charSet,
 * 0 would be returned to firstCharMatchedCharSet
 */
extern ZF_ENV_EXPORT zfindex ZFInputCallbackReadUntil(ZF_IN_OUT zfstring &ret,
                                                      ZF_IN_OUT const ZFInputCallback &input,
                                                      ZF_IN_OPT const zfchar *charSet = zfText(" \t\r\n"),
                                                      ZF_IN_OPT zfindex maxCount = zfindexMax,
                                                      ZF_OUT_OPT zfchar *firstCharMatchedCharSet = zfnull);

/**
 * @brief util method to check whether the input match the tokens
 *
 * return token's index if match or zfindexMax if no match or error,
 * header white spaces would be skipped automatically\n
 * if no match, this method would try to restore the callback state by ioSeek to original position
 */
extern ZF_ENV_EXPORT zfindex ZFInputCallbackCheckMatch(ZF_IN const zfchar **tokens,
                                                       ZF_IN zfindex tokenCount,
                                                       ZF_IN_OUT const ZFInputCallback &input);
/**
 * @brief input callback version of #zfCoreDataPairSplitString
 */
extern ZF_ENV_EXPORT zfbool ZFInputCallbackReadDataPair(ZF_OUT ZFCoreArray<zfstring> &outData,
                                                        ZF_IN_OUT const ZFInputCallback &input,
                                                        ZF_IN zfindex desiredCount = zfindexMax,
                                                        ZF_IN_OPT zfchar leftToken = '(',
                                                        ZF_IN_OPT zfchar rightToken = ')',
                                                        ZF_IN_OPT const zfstring &separatorTokens = zfText(","));
/**
 * @brief input callback version of #zfCoreDataPairSplitString
 */
template<typename T_Int>
ZF_ENV_EXPORT zfbool ZFInputCallbackReadDataPairInt(ZF_OUT ZFCoreArray<T_Int> &outData,
                                                    ZF_IN_OUT const ZFInputCallback &input,
                                                    ZF_IN zfindex desiredCount = zfindexMax,
                                                    ZF_IN_OPT zfchar leftToken = '(',
                                                    ZF_IN_OPT zfchar rightToken = ')',
                                                    ZF_IN_OPT const zfstring &separatorTokens = zfText(","))
{
    ZFCoreArray<zfstring> buf;
    zfbool ret = ZFInputCallbackReadDataPair(buf, input, desiredCount, leftToken, rightToken, separatorTokens);
    for(zfindex i = 0; i < buf.count(); ++i)
    {
        T_Int element = 0;
        if(zfsToInt(element, buf.get(i).cString()) != zfnull)
        {
            ret = zffalse;
            break;
        }
        outData.add(element);
    }
    return ret;
}
/**
 * @brief input callback version of #zfCoreDataPairSplitString
 */
template<typename T_Float>
ZF_ENV_EXPORT zfbool ZFInputCallbackReadDataPairFloat(ZF_OUT ZFCoreArray<T_Float> &outData,
                                                      ZF_IN_OUT const ZFInputCallback &input,
                                                      ZF_IN zfindex desiredCount = zfindexMax,
                                                      ZF_IN_OPT zfchar leftToken = '(',
                                                      ZF_IN_OPT zfchar rightToken = ')',
                                                      ZF_IN_OPT const zfstring &separatorTokens = zfText(","))
{
    ZFCoreArray<zfstring> buf;
    zfbool ret = ZFInputCallbackReadDataPair(buf, input, desiredCount, leftToken, rightToken, separatorTokens);
    for(zfindex i = 0; i < buf.count(); ++i)
    {
        T_Float element = 0;
        if(zfsToFloat(element, buf.get(i).cString()) != zfnull)
        {
            ret = zffalse;
            break;
        }
        outData.add(element);
    }
    return ret;
}

// ============================================================
// common input callbacks
extern ZF_ENV_EXPORT ZFInputCallback _ZFP_ZFInputCallbackForInputInRange(ZF_IN const zfcharA *ownerFilePath,
                                                                         ZF_IN const zfcharA *ownerFunctionName,
                                                                         ZF_IN zfindex ownerFileLine,
                                                                         ZF_IN const ZFInputCallback &inputCallback,
                                                                         ZF_IN_OPT zfindex start = 0,
                                                                         ZF_IN_OPT zfindex count = zfindexMax,
                                                                         ZF_IN_OPT zfbool autoRestorePos = zftrue);
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <something category="input" ... />
 *       <zfindex category="start" ... /> // optional, 0 by default
 *       <zfindex category="count" ... /> // optional, zfindexMax by default
 *       <zfbool category="autoRestorePos" ... /> // optional, zftrue by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFInputCallbackForInputInRange ZFInputCallbackForInputInRange
/** @brief see #ZFCallbackSerializeCustomTypeRegister */
#define ZFCallbackSerializeCustomTypeName_ZFInputCallbackForInputInRange ZFM_TOSTRING(ZFCallbackSerializeCustomType_ZFInputCallbackForInputInRange)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputCallbackForInputInRange_input zfText("input")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputCallbackForInputInRange_start zfText("start")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputCallbackForInputInRange_count zfText("count")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputCallbackForInputInRange_autoRestorePos zfText("autoRestorePos")
/**
 * @brief create a intput callback from another input callback, specified in range
 *
 * params:
 * -  (const ZFInputCallback &) input callback to use
 * -  (zfindex) src's start index
 * -  (zfindex) src's count or zfindexMax to use whole
 * -  (zfbool) whether to restore src input callback's position after result callback deleted
 *
 * seeking the result input callback would ensure in range [start, start + count]\n
 * src must support seek, otherwise a null callback would be returned
 */
#define ZFInputCallbackForInputInRange(anotherInputCallback, ...) \
    _ZFP_ZFInputCallbackForInputInRange(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, anotherInputCallback, ##__VA_ARGS__)

extern ZF_ENV_EXPORT ZFInputCallback _ZFP_ZFInputCallbackForBuffer(ZF_IN const zfcharA *ownerFilePath,
                                                                   ZF_IN const zfcharA *ownerFunctionName,
                                                                   ZF_IN zfindex ownerFileLine,
                                                                   ZF_IN zfbool copy,
                                                                   ZF_IN const void *src,
                                                                   ZF_IN_OPT zfindex count = zfindexMax);
/**
 * @brief create a intput callback input from a const void *,
 *   you must ensure the buffer is alive during the callback's life time
 *
 * params:
 * -  (const void *) src to use
 * -  (zfindex) src's count or zfindexMax to calculate automatically (treated as const zfchar *),
 *   zfindexMax by default
 */
#define ZFInputCallbackForBuffer(buf, ...) \
    _ZFP_ZFInputCallbackForBuffer(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, zffalse, buf, ##__VA_ARGS__)
/**
 * @brief see #ZFInputCallbackForBuffer,
 *   copy the contents and auto free it
 */
#define ZFInputCallbackForBufferCopy(buf, ...) \
    _ZFP_ZFInputCallbackForBuffer(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, zftrue, buf, ##__VA_ARGS__)

// ============================================================
// ZFIOBridgeCallbackAbs
/**
 * @brief a util class to make a buffer to bridge input and output
 *
 * usage:
 * @code
 *   void outputFunc(ZF_IN const ZFOutputCallback &callback) {...}
 *   void inputFunc(ZF_IN const ZFInputCallback &callback) {...}
 *
 *   ZFIOBridgeCallbackAbs bridge;
 *   outputFunc(bridge); // output data to bridge's internal buffer
 *   inputFunc(bridge); // input data from bridge's internal buffer
 * @endcode
 */
zfclassLikePOD ZF_ENV_EXPORT ZFIOBridgeCallbackAbs
{
public:
    /** @cond ZFPrivateDoc */
    ZFIOBridgeCallbackAbs(void) {}
    ZFIOBridgeCallbackAbs(ZF_IN const ZFIOBridgeCallbackAbs &ref) {}
    /** @endcond */
    virtual ~ZFIOBridgeCallbackAbs(void) {}

public:
    /**
     * @brief implicit convert to input callback
     */
    virtual operator ZFInputCallback(void)
    {
        return this->inputCallback();
    }
    /**
     * @brief implicit convert to output callback
     */
    virtual operator ZFOutputCallback(void)
    {
        return this->outputCallback();
    }

public:
    /**
     * @brief get input callback
     */
    virtual ZFInputCallback inputCallback(void) = 0;
    /**
     * @brief get input callback
     */
    virtual ZFOutputCallback outputCallback(void) = 0;

public:
    /**
     * @brief reset input so that next input operation would read from begin
     */
    virtual void resetInput(void) = 0;
    /**
     * @brief clear all outputed data so that next output operation would write from begin,
     *   note that input would also be reset
     */
    virtual void resetOutput(void) = 0;
};

// ============================================================
// ZFIOBridgeCallbackUsingBuffer
zfclassFwd _ZFP_ZFIOBridgeCallbackUsingBufferPrivate;
/**
 * @brief see #ZFIOBridgeCallbackAbs
 */
zfclassLikePOD ZF_ENV_EXPORT ZFIOBridgeCallbackUsingBuffer : zfextendsLikePOD ZFIOBridgeCallbackAbs
{
public:
    /** @cond ZFPrivateDoc */
    ZFIOBridgeCallbackUsingBuffer(void);
    ZFIOBridgeCallbackUsingBuffer(ZF_IN const ZFIOBridgeCallbackUsingBuffer &ref);
    ZFIOBridgeCallbackUsingBuffer &operator =(ZF_IN const ZFIOBridgeCallbackUsingBuffer &ref);
    /** @endcond */
    virtual ~ZFIOBridgeCallbackUsingBuffer(void);

public:
    zfoverride
    virtual ZFInputCallback inputCallback(void);
    zfoverride
    virtual ZFOutputCallback outputCallback(void);

public:
    zfoverride
    virtual void resetInput(void);
    zfoverride
    virtual void resetOutput(void);

private:
    _ZFP_ZFIOBridgeCallbackUsingBufferPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallbackDef_common_h_

