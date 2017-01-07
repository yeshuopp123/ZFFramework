/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFString.h"
#include "protocol/ZFProtocolZFString.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)

ZFENUM_DEFINE(ZFStringEncoding)

ZFENUM_DEFINE(ZFStringStorageType)

#define _ZFP_ZFStringImpl ZFPROTOCOL_ACCESS(ZFString)

// ============================================================
// _ZFP_ZFStringPrivate
zfclassNotPOD _ZFP_ZFStringPrivate
{
public:
    zfindex refCount;
    zfindex cachedLength;
    zfindex cachedWordCount;
    ZFStringStorageTypeEnum storageType;

    // used for ZFStringStorageType::e_NativeString
    void *nativeString;
    const zfchar *nativeStringBuf;
    void *nativeStringBufToken;

    // used for ZFStringStorageType::e_Copy
    zfchar *copyBuf;

    // used for ZFStringStorageType::e_CharBufferRef
    const zfchar *refBuf;

public:
    void clear(void)
    {
        this->cachedLength = zfindexMax;
        this->cachedWordCount = zfindexMax;

        switch(this->storageType)
        {
            case ZFStringStorageType::e_NativeString:
                break;
            case ZFStringStorageType::e_Copy:
                zffree(this->copyBuf);
                this->copyBuf = zfnull;
                break;
            case ZFStringStorageType::e_CharBufferRef:
                this->refBuf = zfnull;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
        if(this->nativeString != zfnull)
        {
            if(this->nativeStringBuf != zfnull)
            {
                _ZFP_ZFStringImpl->stringValueAccessCleanup(this->nativeString, this->nativeStringBuf, this->nativeStringBufToken);
                this->nativeStringBuf = zfnull;
            }
            _ZFP_ZFStringImpl->nativeStringRelease(this->nativeString);
            this->nativeString = zfnull;
        }
        this->storageType = ZFStringStorageType::e_CharBufferRef;
        this->refBuf = zfText("");
    }

public:
    _ZFP_ZFStringPrivate(void)
    : refCount(1)
    , cachedLength(zfindexMax)
    , cachedWordCount(zfindexMax)
    , storageType(ZFStringStorageType::e_CharBufferRef)
    , nativeString(zfnull)
    , nativeStringBuf(zfnull)
    , nativeStringBufToken(zfnull)
    , copyBuf(zfnull)
    , refBuf(zfText(""))
    {
    }
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStringDataHolder, ZFLevelZFFrameworkEssential)
{
    nullData = zfpoolNew(_ZFP_ZFStringPrivate);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFStringDataHolder)
{
    nullData->clear();
    zfpoolDelete(nullData);
}
public:
    _ZFP_ZFStringPrivate *nullData;
ZF_GLOBAL_INITIALIZER_END(ZFStringDataHolder)
#define _ZFP_ZFStringEmptyData (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStringDataHolder)->nullData)

static void _ZFP_ZFString_cleanup(_ZFP_ZFStringPrivate *old)
{
    if(old != zfnull)
    {
        --old->refCount;
        if(old->refCount == 0)
        {
            zfCoreAssertWithMessage(old != _ZFP_ZFStringEmptyData, zfTextA("over release ZFStringEmpty"));
            old->clear();
            zfpoolDelete(old);
        }
    }
}
static void _ZFP_ZFString_readyUpdate(_ZFP_ZFStringPrivate *&old)
{
    if(old == zfnull)
    {
        old = zfpoolNew(_ZFP_ZFStringPrivate);
    }
    else if(old->refCount > 1)
    {
        --old->refCount;
        old = zfpoolNew(_ZFP_ZFStringPrivate);
    }
    else if(old != _ZFP_ZFStringEmptyData)
    {
        old->clear();
    }
}

// ============================================================
// ZFString
ZFOBJECT_REGISTER(ZFString)

zfbool ZFString::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                 ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                 ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHintToAppend, outErrorPos)) {return zffalse;}

    const zfchar *value = zfnull;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHintToAppend, outErrorPos,
        check, ZFSerializableKeyword_value, zfstring, value);
    this->stringValueSet(value);

    return zftrue;
}
zfbool ZFString::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                               ZF_IN ZFSerializable *referencedOwnerOrNull,
                                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHintToAppend)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHintToAppend, ref,
        ZFSerializableKeyword_value, zfstring, this->stringValue(), ref->stringValue(), zfText(""));

    return zftrue;
}

// ============================================================
// conversion
zfbool ZFString::toUTF8(ZF_OUT zfstringA &result,
                        ZF_IN const void *s,
                        ZF_IN ZFStringEncodingEnum srcEncoding)
{
    if(s == zfnull) {return zftrue;}
    return _ZFP_ZFStringImpl->toUTF8(result, s, srcEncoding);
}
zfstringA ZFString::toUTF8(ZF_IN const void *s,
                           ZF_IN ZFStringEncodingEnum srcEncoding,
                           ZF_OUT_OPT zfbool *success /* = zfnull */)
{
    zfstringA result;
    zfbool ret = zfself::toUTF8(result, s, srcEncoding);
    if(success != zfnull) {*success = ret;}
    return result;
}
zfbool ZFString::toUTF16(ZF_OUT zfstringW &result,
                         ZF_IN const void *s,
                         ZF_IN ZFStringEncodingEnum srcEncoding)
{
    if(s == zfnull) {return zftrue;}
    return _ZFP_ZFStringImpl->toUTF16(result, s, srcEncoding);
}
zfstringW ZFString::toUTF16(ZF_IN const void *s,
                            ZF_IN ZFStringEncodingEnum srcEncoding,
                            ZF_OUT_OPT zfbool *success /* = zfnull */)
{
    zfstringW result;
    zfbool ret = zfself::toUTF16(result, s, srcEncoding);
    if(success != zfnull) {*success = ret;}
    return result;
}
zfbool ZFString::toUTF16BE(ZF_OUT zfstringW &result,
                           ZF_IN const void *s,
                           ZF_IN ZFStringEncodingEnum srcEncoding)
{
    if(s == zfnull) {return zftrue;}
    return _ZFP_ZFStringImpl->toUTF16BE(result, s, srcEncoding);
}
zfstringW ZFString::toUTF16BE(ZF_IN const void *s,
                              ZF_IN ZFStringEncodingEnum srcEncoding,
                              ZF_OUT_OPT zfbool *success /* = zfnull */)
{
    zfstringW result;
    zfbool ret = zfself::toUTF16BE(result, s, srcEncoding);
    if(success != zfnull) {*success = ret;}
    return result;
}
zfindex ZFString::wordCountOfUTF8(ZF_IN const zfcharA *utf8String)
{
    if(utf8String == zfnull) {return 0;}
    return _ZFP_ZFStringImpl->wordCountOfUTF8(utf8String);
}

// ============================================================
// instance method
ZFObject *ZFString::objectOnInit(ZF_IN const zfchar *s,
                                 ZF_IN_OPT ZFStringStorageTypeEnum storageType /* = ZFStringStorageType::EnumDefault() */)
{
    this->objectOnInit();
    zfself::stringValueSet(s, storageType);
    return this;
}
ZFObject *ZFString::objectOnInit(ZF_IN ZFString *another)
{
    this->objectOnInit();
    zfself::stringValueSet(another);
    return this;
}
ZFObject *ZFString::objectOnInit(ZF_IN void *nativeString)
{
    this->objectOnInit();
    zfself::nativeStringSet(nativeString);
    return this;
}
ZFObject *ZFString::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = _ZFP_ZFStringEmptyData;
    ++(d->refCount);
    return this;
}
void ZFString::objectOnDealloc(void)
{
    _ZFP_ZFString_cleanup(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfidentity ZFString::objectHash(void)
{
    return zfidentityCalcString(this->stringValue());
}
ZFCompareResult ZFString::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    zfint n = zfscmp(this->stringValue(), another->stringValue());
    if(n < 0)
    {
        return ZFCompareSmaller;
    }
    else if(n == 0)
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareGreater;
    }
}

ZFStringStorageTypeEnum ZFString::storageType(void)
{
    return d->storageType;
}
void ZFString::charBufferCheckCopy(void)
{
    if(d != _ZFP_ZFStringEmptyData && d->storageType == ZFStringStorageType::e_CharBufferRef)
    {
        this->stringValueSet(d->refBuf);
    }
}

zfbool ZFString::isEmpty(void)
{
    return (this->length() == 0);
}
zfindex ZFString::length(void)
{
    if(d->cachedLength == zfindexMax)
    {
        d->cachedLength = zfslen(this->stringValue());
    }
    return d->cachedLength;
}
zfindex ZFString::wordCount(void)
{
    if(d->cachedWordCount == zfindexMax)
    {
        ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
        d->cachedWordCount = _ZFP_ZFStringImpl->wordCountOfUTF8(this->stringValue());
    }
    return d->cachedWordCount;
}
const zfchar *ZFString::stringValue(void)
{
    switch(d->storageType)
    {
        case ZFStringStorageType::e_NativeString:
            if(d->nativeStringBuf == zfnull)
            {
                _ZFP_ZFStringImpl->stringValueAccess(d->nativeString, d->nativeStringBuf, d->nativeStringBufToken);
            }
            return d->nativeStringBuf;
        case ZFStringStorageType::e_Copy:
            return d->copyBuf;
        case ZFStringStorageType::e_CharBufferRef:
            return d->refBuf;
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}
void *ZFString::nativeString(void)
{
    if(d->nativeString == zfnull)
    {
        if(d == _ZFP_ZFStringEmptyData)
        {
            _ZFP_ZFString_readyUpdate(d);
            d->nativeString = _ZFP_ZFStringImpl->nativeStringCreate(zfText(""));
        }
        else
        {
            d->nativeString = _ZFP_ZFStringImpl->nativeStringCreate(this->stringValue());
        }
    }
    return d->nativeString;
}

void ZFString::stringValueSet(ZF_IN_OPT const zfchar *s /* = zfnull */,
                              ZF_IN_OPT ZFStringStorageTypeEnum storageType /* = ZFStringStorageType::EnumDefault() */)
{
    if(s == zfnull || *s == '\0')
    {
        _ZFP_ZFStringPrivate *dTmp = d;
        d = _ZFP_ZFStringEmptyData;
        ++(d->refCount);
        _ZFP_ZFString_cleanup(dTmp);
    }
    else
    {
        _ZFP_ZFString_readyUpdate(d);
        d->storageType = storageType;
        switch(storageType)
        {
            case ZFStringStorageType::e_NativeString:
                d->nativeString = _ZFP_ZFStringImpl->nativeStringCreate(s);
                break;
            case ZFStringStorageType::e_Copy:
            {
                zfindex tmpSize = sizeof(zfchar) * (zfslen(s) + 1);
                d->copyBuf = (zfchar *)zfmalloc(tmpSize);
                zfmemcpy(d->copyBuf, s, tmpSize);
            }
                break;
            case ZFStringStorageType::e_CharBufferRef:
                d->refBuf = s;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
}
void ZFString::nativeStringSet(ZF_IN void *nativeString)
{
    if(nativeString == zfnull)
    {
        _ZFP_ZFStringPrivate *dTmp = d;
        d = _ZFP_ZFStringEmptyData;
        ++(d->refCount);
        _ZFP_ZFString_cleanup(dTmp);
    }
    else
    {
        _ZFP_ZFString_readyUpdate(d);
        d->nativeString = _ZFP_ZFStringImpl->nativeStringRetain(nativeString);
        d->storageType = ZFStringStorageType::e_NativeString;
    }
}
void ZFString::stringValueSet(ZF_IN ZFString *another)
{
    _ZFP_ZFStringPrivate *dTmp = d;
    if(another == zfnull)
    {
        d = _ZFP_ZFStringEmptyData;
    }
    else
    {
        d = another->d;
    }
    ++(d->refCount);
    _ZFP_ZFString_cleanup(dTmp);
}

// ============================================================
static ZFString *_ZFP_ZFStringEmptyInstance = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStringEmptyHolder, ZFLevelZFFrameworkNormal)
{
    _ZFP_ZFStringEmptyInstance = zfAlloc(ZFString);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFStringEmptyHolder)
{
    zfRelease(_ZFP_ZFStringEmptyInstance);
    _ZFP_ZFStringEmptyInstance = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFStringEmptyHolder)
ZFString *_ZFP_ZFStringEmpty(void)
{
    return _ZFP_ZFStringEmptyInstance;
}

ZF_NAMESPACE_GLOBAL_END

