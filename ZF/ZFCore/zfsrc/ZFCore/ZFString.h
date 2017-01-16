/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFString.h
 * @brief string container as ZFObject
 */

#ifndef _ZFI_ZFString_h_
#define _ZFI_ZFString_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief static assert supported encoding for zfchar
 */
#define ZFSTRINGENCODING_ASSERT(T_ZFStringEncodingEnum) \
    ZFASSERT_STATIC(ZFStringEncodingForZFChar == T_ZFStringEncodingEnum, encoding_not_supported)

// ============================================================
#if 1 // you must change these if change zfchar's type
/**
 * @brief encoding name for zfchar type
 */
#define ZFStringEncodingNameForZFChar UTF8
#endif // you must change these if change zfchar's type
// ============================================================

/**
 * @brief encoding name for zfcharA type, UTF8 for default
 */
#ifndef ZFStringEncodingNameForZFCharA
    #define ZFStringEncodingNameForZFCharA UTF8
#endif
/**
 * @brief encoding name for zfcharW type, UTF16 for default
 */
#ifndef ZFStringEncodingNameForZFCharW
    #define ZFStringEncodingNameForZFCharW UTF16
#endif

/**
 * @brief encoding used in zfchar, see #ZFStringEncoding
 */
#define ZFStringEncodingForZFChar ZFM_CAT(ZFStringEncoding::e_, ZFM_EXPAND(ZFStringEncodingNameForZFChar))

/**
 * @brief encoding used in zfcharA, see #ZFStringEncoding
 */
#define ZFStringEncodingForZFCharA ZFM_CAT(ZFStringEncoding::e_, ZFM_EXPAND(ZFStringEncodingNameForZFCharA))

/**
 * @brief encoding used in zfcharW, see #ZFStringEncoding
 */
#define ZFStringEncodingForZFCharW ZFM_CAT(ZFStringEncoding::e_, ZFM_EXPAND(ZFStringEncodingNameForZFCharW))

#define _ZFP_ZFStringCodeConv(s, fromType, toName, toCharType) \
    ((fromType == ZFStringEncoding::ZFM_CAT(e_, toName)) \
        ? ZFCastReinterpret(const toCharType *, s) \
        : (ZFM_CAT(ZFString::to, toName)(s, fromType).cString()))
/**
 * @brief convert types of: zfchar, zfcharA, zfcharW, TCHAR
 *
 * usually used under Windows, so that it's easy to pass string params to Windows API\n
 * call ZFString::toUTF8, etc, to convert encodings\n
 * the letters stands for:
 * -  Z: zfchar, ZFStringEncodingForZFChar encoded
 * -  A: zfcharA, ZFStringEncodingForZFCharA encoded
 * -  W: zfcharW, UTF16 encoded
 *
 * see #ZFStringEncoding for more info
 * @note the return value would be released automatically after expression's end,
 *   you should not save it as a zfcharA * string, use zfstringA instead
 */
#define ZFStringZ2A(s) _ZFP_ZFStringCodeConv(s, ZFStringEncodingForZFChar, ZFStringEncodingNameForZFCharA, zfcharA)
/** @brief see #ZFStringZ2A */
#define ZFStringA2Z(s) _ZFP_ZFStringCodeConv(s, ZFStringEncodingForZFCharA, ZFStringEncodingNameForZFChar, zfchar)
/** @brief see #ZFStringZ2A */
#define ZFStringZ2W(s) _ZFP_ZFStringCodeConv(s, ZFStringEncodingForZFChar, ZFStringEncodingNameForZFCharW, zfcharW)
/** @brief see #ZFStringZ2A */
#define ZFStringW2Z(s) _ZFP_ZFStringCodeConv(s, ZFStringEncodingForZFCharW, ZFStringEncodingNameForZFChar, zfchar)
/** @brief see #ZFStringZ2A */
#define ZFStringW2A(s) _ZFP_ZFStringCodeConv(s, ZFStringEncodingForZFCharW, ZFStringEncodingNameForZFCharA, zfcharA)
/** @brief see #ZFStringZ2A */
#define ZFStringA2W(s) _ZFP_ZFStringCodeConv(s, ZFStringEncodingForZFCharA, ZFStringEncodingNameForZFCharW, zfcharW)

/**
 * @brief supported string encoding
 *
 * we use internal string encoding for string processing,
 * and use native string type for string storage\n
 * \n
 * we have these encoding types:
 * -  zfchar: #ZFStringEncodingForZFChar encoded
 * -  zfcharA: usually used in io and net transfer,
 *   ensured 1 byte, #ZFStringEncodingForZFCharA encoded
 * -  zfcharW: usually used to adapt to Unicode version of Windows API,
 *   ensured no less than 2 bytes, #ZFStringEncodingForZFCharW encoded
 *
 * we have these string types:
 * -  cosnt zfchar *: C-style strings, simple and efficient
 * -  zfstring/zfstringA/zfstringW: same as #ZFCoreString, a wrapper for std::string
 * -  void *: very low level type to native string type,
 *   such as NSString, QString
 * -  ZFString: string as a ZFObject to contain the low level void * types,
 *   has interfaces to convert between different string types
 *
 * \n
 * keep it in short, use zfchar (or zfstring) to process strings,
 * and use #zfText to wrap strings in source code,
 * and use ZFString as ZFObject:
 * @code
 *   const zfchar *s = zfText("my string");
 *   zfstring str = s;
 *
 *   // for Windows API, convert to zfcharW and use WCS versions:
 *   WindowsWAPI(ZFStringZ2W(s));
 *
 *   // for net io, convert to UTF8:
 *   netTransfer(ZFString::toUTF8(s, ZFStringEncodingForZFChar).cString());
 * @endcode
 *
 * for more advanced string encoding operation,
 * you should consider third-party library
 */
ZFENUM_BEGIN(ZFStringEncoding)
    ZFENUM_VALUE(UTF8 = 1)
    ZFENUM_VALUE(UTF16LE = 2)
    ZFENUM_VALUE(UTF16BE = 3)
    ZFENUM_VALUE(UTF16 = e_UTF16LE)
ZFENUM_SEPARATOR_ALLOW_DUPLICATE_VALUE(ZFStringEncoding)
    ZFENUM_VALUE_REGISTER(UTF8)
    ZFENUM_VALUE_REGISTER_WITH_NAME(UTF16LE, zfText("UTF16"))
    ZFENUM_VALUE_REGISTER_WITH_NAME(UTF16BE, zfText("UTF16BE"))
    ZFENUM_VALUE_REGISTER_WITH_NAME(UTF16, zfText("UTF16"))
ZFENUM_END(ZFStringEncoding)

// ============================================================
/**
 * @brief storage type for a #ZFString,
 *   usually you should not concern this type
 */
ZFENUM_BEGIN(ZFStringStorageType)
    /**
     * @brief store to native string
     *
     * default storage type, need ZFString's implementation
     */
    ZFENUM_VALUE(NativeString)
    /**
     * @brief copy the string to internal storage
     *
     * have good performance and safety,
     * need no ZFString's implementation
     */
    ZFENUM_VALUE(Copy)
    /**
     * @brief reference to a char buffer (const zfchar *)
     *
     * have best performance,
     * but app must ensure the char buffer being alive
     * while using the #ZFString object,
     * use with extreme caution,
     * usually useful to create from static zfchar * strings for performance,
     * need no ZFString's implementation
     */
    ZFENUM_VALUE(CharBufferRef)
ZFENUM_SEPARATOR(ZFStringStorageType)
    ZFENUM_VALUE_REGISTER(NativeString)
    ZFENUM_VALUE_REGISTER(Copy)
    ZFENUM_VALUE_REGISTER(CharBufferRef)
ZFENUM_END_WITH_DEFAULT(ZFStringStorageType, NativeString)

// ============================================================
// ZFString
zfclassFwd _ZFP_ZFStringPrivate;
/**
 * @brief native string wrapper
 *
 * readonly string container\n
 * for advanced string operation,
 * you may want to:
 * -  get the string, encoded by ZFStringEncodingForZFChar, then do your work
 * -  write native code, using nativeString
 *   (deprecated, use only for performance)
 *
 * we use internal string encoding for string operation
 * (using zfchar and zfstring),
 * to communicate with UI, use nativeString,
 * to communicate with console, use ZFStringZ2A/ZFStringA2Z\n
 * \n
 * note, #ZFString is serializable,
 * but while serializing to a ZFString object,
 * storage type is ensured #ZFStringStorageType::EnumDefault\n
 * \n
 * serializable data:
 * @code
 *   <StringClass
 *       value="string content" // optional, empty by default
 *   />
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFString : zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFString, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable, ZFCopyable)

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->stringValueSet(ZFCastZFObjectUnchecked(zfself *, anotherObj)->stringValue());
    }

    // ============================================================
    // conversion
public:
    /**
     * @brief convert to UTF8 encoding, or empty string if failed
     *
     * result must be appended to result, instead clear and set
     */
    static zfbool toUTF8(ZF_OUT zfstringA &result,
                         ZF_IN const void *s,
                         ZF_IN ZFStringEncodingEnum srcEncoding);
    /**
     * @brief see #toUTF8
     */
    static zfstringA toUTF8(ZF_IN const void *s,
                            ZF_IN ZFStringEncodingEnum srcEncoding,
                            ZF_OUT_OPT zfbool *success = zfnull);
    /**
     * @brief convert to UTF16 little endian encoding, or empty string if failed
     *
     * result must be appended to result, instead clear and set
     */
    static zfbool toUTF16(ZF_OUT zfstringW &result,
                          ZF_IN const void *s,
                          ZF_IN ZFStringEncodingEnum srcEncoding);
    /**
     * @brief see #toUTF16
     */
    static zfstringW toUTF16(ZF_IN const void *s,
                             ZF_IN ZFStringEncodingEnum srcEncoding,
                             ZF_OUT_OPT zfbool *success = zfnull);
    /**
     * @brief convert to UTF16 big endian encoding, or empty string if failed
     *
     * result must be appended to result, instead clear and set
     */
    static zfbool toUTF16BE(ZF_OUT zfstringW &result,
                            ZF_IN const void *s,
                            ZF_IN ZFStringEncodingEnum srcEncoding);
    /**
     * @brief see #toUTF16BE
     */
    static zfstringW toUTF16BE(ZF_IN const void *s,
                               ZF_IN ZFStringEncodingEnum srcEncoding,
                               ZF_OUT_OPT zfbool *success = zfnull);
    /**
     * @brief to encoding specified by ZFStringEncodingForZFChar
     *
     * result must be appended to result, instead clear and set
     */
    static zfbool toZFChar(ZF_OUT zfstring &result,
                           ZF_IN const void *s,
                           ZF_IN ZFStringEncodingEnum srcEncoding)
    {
        return ZFM_CAT(ZFString::to, ZFStringEncodingNameForZFChar)(result, s, srcEncoding);
    }
    /**
     * @brief see #toZFChar
     */
    static zfstring toZFChar(ZF_IN const void *s,
                             ZF_IN ZFStringEncodingEnum srcEncoding,
                             ZF_OUT_OPT zfbool *success = zfnull)
    {
        return ZFM_CAT(ZFString::to, ZFStringEncodingNameForZFChar)(s, srcEncoding, success);
    }
    /**
     * @brief to encoding specified by ZFStringEncodingForZFCharA
     *
     * result must be appended to result, instead clear and set
     */
    static zfbool toZFCharA(ZF_OUT zfstringA &result,
                            ZF_IN const void *s,
                            ZF_IN ZFStringEncodingEnum srcEncoding)
    {
        return ZFM_CAT(ZFString::to, ZFStringEncodingNameForZFCharA)(result, s, srcEncoding);
    }
    /**
     * @brief see #toZFCharA
     */
    static zfstringA toZFCharA(ZF_IN const void *s,
                               ZF_IN ZFStringEncodingEnum srcEncoding,
                               ZF_OUT_OPT zfbool *success = zfnull)
    {
        return ZFM_CAT(ZFString::to, ZFStringEncodingNameForZFCharA)(s, srcEncoding, success);
    }
    /**
     * @brief to encoding specified by ZFStringEncodingForZFCharW
     *
     * result must be appended to result, instead clear and set
     */
    static zfbool toZFCharW(ZF_OUT zfstringW &result,
                            ZF_IN const void *s,
                            ZF_IN ZFStringEncodingEnum srcEncoding)
    {
        return ZFM_CAT(ZFString::to, ZFStringEncodingNameForZFCharW)(result, s, srcEncoding);
    }
    /**
     * @brief see #toZFCharW
     */
    static zfstringW toZFCharW(ZF_IN const void *s,
                               ZF_IN ZFStringEncodingEnum srcEncoding,
                               ZF_OUT_OPT zfbool *success = zfnull)
    {
        return ZFM_CAT(ZFString::to, ZFStringEncodingNameForZFCharW)(s, srcEncoding, success);
    }
    /**
     * @brief get utf8 string's logical word count or -1 if invalid letter exist
     */
    static zfindex wordCountOfUTF8(ZF_IN const zfcharA *utf8String);

    // ============================================================
    // instance method
public:
    /**
     * @brief init from zfchar string
     */
    virtual ZFObject *objectOnInit(ZF_IN const zfchar *s,
                                   ZF_IN_OPT ZFStringStorageTypeEnum storageType = ZFStringStorageType::EnumDefault());
    /**
     * @brief init from ZFString (no deep copy)
     */
    virtual ZFObject *objectOnInit(ZF_IN ZFString *another);
    /**
     * @brief init from native string
     */
    virtual ZFObject *objectOnInit(ZF_IN void *nativeString);
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual inline void objectInfoOnAppendTokenLeft(ZF_IN_OUT zfstring &ret)
    {
        ret += zfText("\"");
    }
    zfoverride
    virtual inline void objectInfoOnAppendTokenRight(ZF_IN_OUT zfstring &ret)
    {
        ret += zfText("\"");
    }
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        ret += this->stringValue();
    }
public:
    zfoverride
    virtual zfidentity objectHash(void);
    /**
     * @brief compare string value
     */
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief usually for debug or performance use only
     */
    virtual ZFStringStorageTypeEnum storageType(void);
    /**
     * @brief for reference storage type only,
     *   to ensure the internal referenced buffer is safe to release,
     *   see #ZFStringStorageType::e_CharBufferRef
     *
     * copy the buffer and change storage type if necessary
     */
    virtual void charBufferCheckCopy(void);

public:
    /**
     * @brief true if empty
     */
    virtual zfbool isEmpty(void);
    /**
     * @brief get the length of the string
     *
     * note that the length is the size of the zfcharA type,
     * may be or may not be the logical letter num,
     * the actual byte size can be calculated by
     * "sizeof(zfchar) * length()"
     * @see wordCount
     */
    virtual zfindex length(void);
    /**
     * @brief get the logical word count instead of byte size
     */
    virtual zfindex wordCount(void);
    /**
     * @brief get the string, ensured to be non-null
     *
     * result is cached so no need to worry about performance
     * @note the returned string is not ensured alive during owner ZFString's life-time,
     *   you should not store the returned pointer value
     */
    virtual const zfchar *stringValue(void);
    /**
     * @brief get the nativeString, you must not change its content,
     *   ensured to be non-null, create if necessary
     */
    virtual void *nativeString(void);

protected:
    /**
     * @brief clear and set by string
     *
     * store as #ZFStringStorageType::EnumDefault by default,
     * which need ZFString's implementation
     */
    virtual void stringValueSet(ZF_IN_OPT const zfchar *s = zfnull,
                                ZF_IN_OPT ZFStringStorageTypeEnum storageType = ZFStringStorageType::EnumDefault());
    /**
     * @brief clear and set by nativeString
     *
     * may or may not copy contents of the nativeString (depends on implementation),
     * so if you changed the contents of nativeString after set to another ZFString,
     * the behavior is undefined, use with caution
     */
    virtual void nativeStringSet(ZF_IN void *nativeString);
    /**
     * @brief set by ZFString, no deep copy
     */
    virtual void stringValueSet(ZF_IN ZFString *another);

private:
    _ZFP_ZFStringPrivate *d;
};

// ============================================================
// ZFStringEditable
/**
 * @brief editable version of ZFString
 */
zfclass ZF_ENV_EXPORT ZFStringEditable : zfextends ZFString
{
    ZFOBJECT_DECLARE(ZFStringEditable, ZFString)

public:
    zfoverride
    virtual ZFObject *objectOnInit(ZF_IN const zfchar *s,
                                   ZF_IN_OPT ZFStringStorageTypeEnum storageType = ZFStringStorageType::EnumDefault())
    {
        return zfsuper::objectOnInit(s, storageType);
    }
    zfoverride
    virtual ZFObject *objectOnInit(ZF_IN ZFString *another) {return zfsuper::objectOnInit(another);}
    zfoverride
    virtual ZFObject *objectOnInit(ZF_IN void *nativeString) {return zfsuper::objectOnInit(nativeString);}
public:
    zfoverride
    virtual ZFObject *objectOnInit(void) {return zfsuper::objectOnInit();}
    zfoverride
    virtual void objectOnDealloc(void) {zfsuper::objectOnDealloc();}

public:
    zfoverride
    virtual void stringValueSet(ZF_IN_OPT const zfchar *s = zfnull,
                                ZF_IN_OPT ZFStringStorageTypeEnum storageType = ZFStringStorageType::EnumDefault())
    {
        zfsuper::stringValueSet(s, storageType);
    }
    zfoverride
    virtual void nativeStringSet(ZF_IN void *nativeString)
    {
        zfsuper::nativeStringSet(nativeString);
    }
    zfoverride
    virtual void stringValueSet(ZF_IN ZFString *another)
    {
        zfsuper::stringValueSet(another);
    }
};

// ============================================================
extern ZF_ENV_EXPORT ZFString *_ZFP_ZFStringEmpty(void);
/**
 * @brief a global empty string
 */
#define ZFStringEmpty _ZFP_ZFStringEmpty()

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFString_h_

