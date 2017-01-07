/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCallbackDef.h"
#include "ZFObjectDef.h"
#include "ZFObjectSmartPointerDef.h"
#include "ZFSerializableDataDef.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFCallback_executeNullCallback(ZF_IN const zfchar *createInfo)
{
    zfCoreCriticalMessageTrim(zfTextA("try execute a null callback, created at: %s"),
        zfsCoreZ2A(createInfo));
}

// ============================================================
void ZFCallbackTypeToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFCallbackType const &value)
{
    switch(value)
    {
        case ZFCallbackTypeDummy:
            ret += ZFTOKEN_ZFCallbackTypeDummy;
            return ;
        case ZFCallbackTypeMethod:
            ret += ZFTOKEN_ZFCallbackTypeMethod;
            return ;
        case ZFCallbackTypeMemberMethod:
            ret += ZFTOKEN_ZFCallbackTypeMemberMethod;
            return ;
        case ZFCallbackTypeRawFunction:
            ret += ZFTOKEN_ZFCallbackTypeRawFunction;
            return ;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
const zfchar *ZFCallbackTypeFromString(ZF_OUT ZFCallbackType &ret,
                                       ZF_IN const zfchar *src,
                                       ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFCallbackTypeDummy,
        ZFTOKEN_ZFCallbackTypeMethod,
        ZFTOKEN_ZFCallbackTypeMemberMethod,
        ZFTOKEN_ZFCallbackTypeRawFunction,
    };
    zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
    ret = ZFCallbackTypeDummy;
    switch(matched)
    {
        case 0:
            ret = ZFCallbackTypeDummy;
            return zfnull;
        case 1:
            ret = ZFCallbackTypeMethod;
            return zfnull;
        case 2:
            ret = ZFCallbackTypeMemberMethod;
            return zfnull;
        case 3:
            ret = ZFCallbackTypeRawFunction;
            return zfnull;
        default:
            return src;
    }
}

// ============================================================
// _ZFP_ZFCallbackPrivate
typedef zfstlmap<zfstlstringZ, zfautoObject> _ZFP_ZFCallbackTagMap;
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCallbackPrivate
{
public:
    zfindex refCount;
    zfchar *callbackId;
    _ZFP_ZFCallbackTagMap callbackTagMap;
    ZFCallbackType callbackType;
    ZFObject *callbackOwnerObj; // assign only
    zfbool callbackOwnerObjectRetainFlag;
    const ZFMethod *callbackMethod;
    ZFFuncAddrType callbackRawFunc;
    const zfcharA *createInfoFilePath;
    const zfcharA *createInfoFunctionName;
    zfindex createInfoFileLine;
    zfchar *serializableCustomType;
    ZFSerializableData *serializableCustomData;

public:
    _ZFP_ZFCallbackPrivate(void)
    : refCount(1)
    , callbackId(zfnull)
    , callbackTagMap()
    , callbackType(ZFCallbackTypeDummy)
    , callbackOwnerObj(zfnull)
    , callbackOwnerObjectRetainFlag(zffalse)
    , callbackMethod(zfnull)
    , callbackRawFunc(zfnull)
    , createInfoFilePath(zfnull)
    , createInfoFunctionName(zfnull)
    , createInfoFileLine(0)
    , serializableCustomType(zfnull)
    , serializableCustomData(zfnull)
    {
    }
    ~_ZFP_ZFCallbackPrivate(void)
    {
        zffree(this->callbackId);
        if(this->callbackOwnerObjectRetainFlag)
        {
            zfRelease(this->callbackOwnerObj);
        }
        zffree(this->serializableCustomType);
        zfdelete(this->serializableCustomData);
    }
};

// ============================================================
// global
ZF_STATIC_INITIALIZER_INIT(ZFCallbackDataHolder)
{
    nullData = zfnew(_ZFP_ZFCallbackPrivate);
}
ZF_STATIC_INITIALIZER_DESTROY(ZFCallbackDataHolder)
{
    zfdelete(nullData);
    nullData = zfnull;
}
/*
 * ensured no ZFObject types would be attached to null data
 * so it's safe to hold as static init step
 */
_ZFP_ZFCallbackPrivate *nullData;
ZF_STATIC_INITIALIZER_END(ZFCallbackDataHolder)
#define _ZFP_ZFCallbackNullPrivateData (ZF_STATIC_INITIALIZER_INSTANCE(ZFCallbackDataHolder)->nullData)

static void _ZFP_ZFCallbackPrivateDataChange(_ZFP_ZFCallbackPrivate *&oldData, _ZFP_ZFCallbackPrivate *newData)
{
    _ZFP_ZFCallbackPrivate *dTmp = oldData;
    oldData = newData;
    if(newData != zfnull)
    {
        ++newData->refCount;
    }
    if(dTmp != zfnull)
    {
        --dTmp->refCount;
        if(dTmp->refCount == 0)
        {
            zfdelete(dTmp);
        }
    }
}
static void _ZFP_ZFCallbackPrivateDataReadyUpdate(_ZFP_ZFCallbackPrivate *&d)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    else if(d->refCount != 1)
    {
        --d->refCount;
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
}

// ============================================================
// ZFCallback
/** @cond ZFPrivateDoc */
ZFCallback::ZFCallback(void)
: d(_ZFP_ZFCallbackNullPrivateData)
{
    ++(d->refCount);
    _ZFP_ZFCallbackCachedDataSetup(*this, d);
}
ZFCallback::ZFCallback(ZF_IN const ZFCallback &ref)
: d(zfnull)
{
    this->operator =(ref);
}
ZFCallback &ZFCallback::operator =(ZF_IN const ZFCallback &ref)
{
    _ZFP_ZFCallbackPrivateDataChange(d, ref.d);
    _ZFP_ZFCallbackCachedDataSetup(*this, d);
    return *this;
}
ZFCallback::~ZFCallback(void)
{
    _ZFP_ZFCallbackPrivateDataChange(d, zfnull);
}
ZFCallback ZFCallback::_ZFP_ZFCallbackCreate(ZF_IN ZFCallbackType callbackType,
                                             ZF_IN ZFObject *callbackOwnerObj,
                                             ZF_IN const ZFMethod *callbackMethod,
                                             ZF_IN ZFFuncAddrType callbackRawFunc,
                                             ZF_IN const zfcharA *createInfoFilePath,
                                             ZF_IN const zfcharA *createInfoFunctionName,
                                             ZF_IN zfindex createInfoFileLine)
{
    ZFCallback callback;
    _ZFP_ZFCallbackPrivateDataReadyUpdate(callback.d);
    callback.d->createInfoFilePath = createInfoFilePath;
    callback.d->createInfoFunctionName = createInfoFunctionName;
    callback.d->createInfoFileLine = createInfoFileLine;
    switch(callbackType)
    {
        case ZFCallbackTypeDummy:
            callback.d->callbackType = callbackType;
            break;
        case ZFCallbackTypeMethod:
            zfCoreAssertWithMessageTrim(callbackMethod != zfnull, zfTextA("%s method is null"),
                zfsCoreZ2A(callback.createInfo().cString()));
            zfCoreAssertWithMessageTrim(callbackMethod->methodIsStatic(),
                zfTextA("%s method \"%s\" is not class static member type"),
                zfsCoreZ2A(callback.createInfo().cString()),
                zfsCoreZ2A(callbackMethod->objectInfo().cString()));
            callback.d->callbackType = callbackType;
            callback.d->callbackMethod = callbackMethod;
            break;
        case ZFCallbackTypeMemberMethod:
            zfCoreAssertWithMessageTrim(callbackOwnerObj != zfnull && callbackMethod != zfnull,
                zfTextA("%s invalid callback, ownerObj: %s, method: %s"),
                zfsCoreZ2A(callback.createInfo().cString()),
                zfsFromPointer<zfstringA>(callbackOwnerObj).cString(),
                zfsFromPointer<zfstringA>(callbackMethod).cString());
            zfCoreAssertWithMessageTrim(!callbackMethod->methodIsStatic(),
                zfTextA("%s method \"%s\" is not class member type"),
                zfsCoreZ2A(callback.createInfo().cString()),
                zfsCoreZ2A(callbackMethod->objectInfo().cString()));
            zfCoreAssertWithMessageTrim(callbackOwnerObj->classData()->classIsTypeOf(callbackMethod->ownerClass()),
                zfTextA("%s object %s has no such method \"%s\""),
                zfsCoreZ2A(callback.createInfo().cString()),
                zfsCoreZ2A(callbackOwnerObj->objectInfoOfInstance().cString()),
                zfsCoreZ2A(callbackMethod->objectInfo().cString()));
            callback.d->callbackType = callbackType;
            callback.d->callbackOwnerObj = callbackOwnerObj;
            callback.d->callbackMethod = callbackMethod;
            break;
        case ZFCallbackTypeRawFunction:
            zfCoreAssertWithMessageTrim(callbackRawFunc != zfnull, zfTextA("%s invalid function address"),
                zfsCoreZ2A(callback.createInfo().cString()));
            callback.d->callbackType = callbackType;
            callback.d->callbackRawFunc = callbackRawFunc;
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
    _ZFP_ZFCallbackCachedDataSetup(callback, callback.d);
    return callback;
}
/** @endcond */

zfindex ZFCallback::objectRetainCount(void) const
{
    return d->refCount;
}

void ZFCallback::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    switch(this->callbackType())
    {
        case ZFCallbackTypeDummy:
            ret += zfText("ZFCallbackNull");
            break;
        case ZFCallbackTypeMethod:
        case ZFCallbackTypeMemberMethod:
            this->callbackMethod()->objectInfoT(ret);
            break;
        case ZFCallbackTypeRawFunction:
            ret += ZFTOKEN_ZFObjectInfoLeft;
            ret += zfText("ZFCallback func: ");
            zfsFromPointerT(ret, ZFCastReinterpret(const void *, this->callbackFunctionAddr()));
            ret += ZFTOKEN_ZFObjectInfoRight;
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

ZFCompareResult ZFCallback::objectCompare(ZF_IN const ZFCallback &ref) const
{
    return ((d == ref.d || (
            this->callbackType() == ref.callbackType()
            && this->callbackOwnerObject() == ref.callbackOwnerObject()
            && this->callbackMethod() == ref.callbackMethod()
            && this->callbackFunctionAddr() == ref.callbackFunctionAddr()
            ))
        ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCompareResult ZFCallback::objectCompareIncludingOwner(ZF_IN const ZFCallback &ref) const
{
    return ((this->objectCompare(ref) == ZFCompareTheSame && (
            zfscmpTheSame(this->createInfoFilePath(), ref.createInfoFilePath())
            && zfscmpTheSame(this->createInfoFunctionName(), ref.createInfoFunctionName())
            && (this->createInfoFileLine() == ref.createInfoFileLine())
        ))
        ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCompareResult ZFCallback::objectCompareByInstance(ZF_IN const ZFCallback &ref) const
{
    return ((d == ref.d) ? ZFCompareTheSame : ZFCompareUncomparable);
}

void ZFCallback::callbackIdSet(ZF_IN const zfchar *callbackId)
{
    if(callbackId != zfnull && *callbackId == '\0')
    {
        callbackId = zfnull;
    }
    zfsChange(d->callbackId, callbackId);
}
const zfchar *ZFCallback::callbackId(void) const
{
    return d->callbackId;
}

void ZFCallback::callbackTagSet(ZF_IN const zfchar *key,
                                ZF_IN ZFObject *tag,
                                ZF_IN_OPT zfbool autoMarkCached /* = zffalse */)
{
    if(key == zfnull)
    {
        return ;
    }
    // not allowed to add to global null callback,
    // check and create new one if necessary
    if(d == _ZFP_ZFCallbackNullPrivateData)
    {
        _ZFP_ZFCallbackPrivateDataReadyUpdate(d);
    }

    _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
    _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
    if(it == m.end())
    {
        if(tag != zfnull)
        {
            if(autoMarkCached)
            {
                m[key] = zfautoObjectCreateMarkCached(tag);
            }
            else
            {
                m[key] = zfautoObjectCreate(tag);
            }
        }
    }
    else
    {
        if(tag == zfnull)
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            m.erase(it);
        }
        else
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            if(autoMarkCached)
            {
                it->second = zfautoObjectCreateMarkCached(tag);
            }
            else
            {
                it->second = zfautoObjectCreate(tag);
            }
        }
    }
}
ZFObject *ZFCallback::callbackTagGet(ZF_IN const zfchar *key) const
{
    if(key != zfnull)
    {
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
        _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
        if(it != m.end())
        {
            return it->second.toObject();
        }
    }
    return zfnull;
}
void ZFCallback::callbackTagGetAllKeyValue(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &allKey,
                                           ZF_IN_OUT ZFCoreArrayPOD<ZFObject *> &allValue) const
{
    _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
    allKey.capacitySet(allKey.count() + m.size());
    allValue.capacitySet(allValue.count() + m.size());
    for(_ZFP_ZFCallbackTagMap::iterator it = m.begin(); it != m.end(); ++it)
    {
        allKey.add(it->first.c_str());
        allValue.add(it->second.toObject());
    }
}
zfautoObject ZFCallback::callbackTagRemoveAndGet(ZF_IN const zfchar *key)
{
    if(key != zfnull)
    {
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
        _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
        if(it != m.end())
        {
            zfautoObject ret = zfautoObjectCreate(it->second.toObject());
            m.erase(it);
            return ret;
        }
    }
    return zfautoObjectNull;
}
void ZFCallback::callbackTagRemoveAll(void)
{
    if(!d->callbackTagMap.empty())
    {
        _ZFP_ZFCallbackTagMap tmp;
        tmp.swap(d->callbackTagMap);
    }
}

ZFCallbackType ZFCallback::callbackType(void) const
{
    return d->callbackType;
}

ZFObject *ZFCallback::callbackOwnerObject(void) const
{
    return d->callbackOwnerObj;
}

const ZFMethod *ZFCallback::callbackMethod(void) const
{
    return d->callbackMethod;
}

ZFFuncAddrType ZFCallback::callbackFunctionAddr(void) const
{
    return d->callbackRawFunc;
}

void ZFCallback::createInfoSet(ZF_IN const zfcharA *createInfoFilePath,
                               ZF_IN const zfcharA *createInfoFunctionName,
                               ZF_IN zfindex createInfoFileLine)
{
    d->createInfoFilePath = createInfoFilePath;
    d->createInfoFunctionName = createInfoFunctionName;
    d->createInfoFileLine = createInfoFileLine;
}

void ZFCallback::createInfo(ZF_OUT zfstring &ret) const
{
    ret += zfsCoreA2Z(ZF_CALLER_INFO_DETAIL(
        d->createInfoFilePath,
        d->createInfoFunctionName,
        d->createInfoFileLine));
}

const zfcharA *ZFCallback::createInfoFilePath(void) const
{
    return d->createInfoFilePath;
}
const zfcharA *ZFCallback::createInfoFunctionName(void) const
{
    return d->createInfoFunctionName;
}
zfindex ZFCallback::createInfoFileLine(void) const
{
    return d->createInfoFileLine;
}

void ZFCallback::callbackClear(void)
{
    _ZFP_ZFCallbackPrivateDataChange(d, _ZFP_ZFCallbackNullPrivateData);
}

void ZFCallback::callbackOwnerObjectRetain(void) const
{
    if(d->callbackOwnerObj != zfnull && !d->callbackOwnerObjectRetainFlag)
    {
        zfRetain(d->callbackOwnerObj);
        d->callbackOwnerObjectRetainFlag = zftrue;
    }
}
void ZFCallback::callbackOwnerObjectRelease(void) const
{
    if(d->callbackOwnerObjectRetainFlag)
    {
        d->callbackOwnerObjectRetainFlag = zffalse;
        zfRelease(d->callbackOwnerObj);
    }
}

void ZFCallback::callbackSerializeCustomTypeSet(ZF_IN const zfchar *customType)
{
    zfsChange(d->serializableCustomType, customType);
}
const zfchar *ZFCallback::callbackSerializeCustomType(void) const
{
    return d->serializableCustomType;
}
void ZFCallback::callbackSerializeCustomDataSet(ZF_IN const ZFSerializableData *customData)
{
    if(customData == zfnull)
    {
        if(d->serializableCustomData != zfnull)
        {
            zfdelete(d->serializableCustomData);
            d->serializableCustomData = zfnull;
        }
    }
    else
    {
        if(d->serializableCustomData != zfnull)
        {
            *(d->serializableCustomData) = *customData;
        }
        else
        {
            d->serializableCustomData = zfnew(ZFSerializableData, *customData);
        }
    }
}
const ZFSerializableData *ZFCallback::callbackSerializeCustomData(void) const
{
    return d->serializableCustomData;
}

void ZFCallback::_ZFP_ZFCallbackCachedDataSetup(ZF_IN_OUT ZFCallback &c, _ZFP_ZFCallbackPrivate *d)
{
    switch(d->callbackType)
    {
        case ZFCallbackTypeDummy:
            c._ZFP_ZFCallbackCached_callbackType = _ZFP_ZFCallbackCachedTypeDummy;
            c._ZFP_ZFCallbackCached_callbackOwnerObj = zfnull;
            break;
        case ZFCallbackTypeMethod:
            c._ZFP_ZFCallbackCached_callbackType = _ZFP_ZFCallbackCachedTypeClassMember;
            c._ZFP_ZFCallbackCached_callbackOwnerObj = zfnull;
            break;
        case ZFCallbackTypeMemberMethod:
            c._ZFP_ZFCallbackCached_callbackType = _ZFP_ZFCallbackCachedTypeClassMember;
            c._ZFP_ZFCallbackCached_callbackOwnerObj = d->callbackOwnerObj;
            break;
        case ZFCallbackTypeRawFunction:
            c._ZFP_ZFCallbackCached_callbackType = _ZFP_ZFCallbackCachedTypeRawFunction;
            c._ZFP_ZFCallbackCached_callbackOwnerObj = zfnull;
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}
ZFFuncAddrType ZFCallback::_ZFP_ZFCallbackCached_callbackInvoker_method(void) const
{
    return d->callbackMethod->methodInvoker();
}
ZFFuncAddrType ZFCallback::_ZFP_ZFCallbackCached_callbackInvoker_rawFunction(void) const
{
    return d->callbackRawFunc;
}

// ============================================================
zfbool operator == (ZF_IN const ZFCallback &v0,
                    ZF_IN const ZFCallback &v1)
{
    return (v0.objectCompareByInstance(v1) == ZFCompareTheSame);
}
zfbool operator != (ZF_IN const ZFCallback &v0,
                    ZF_IN const ZFCallback &v1)
{
    return (v0.objectCompareByInstance(v1) != ZFCompareTheSame);
}

ZF_NAMESPACE_GLOBAL_END

