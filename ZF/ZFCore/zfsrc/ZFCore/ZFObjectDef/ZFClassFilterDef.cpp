/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFClassFilterDef.h"
#include "ZFObjectDef.h"
#include "ZFClassDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFClassFilterTypeToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFClassFilterType const &value)
{
    switch(value)
    {
        case ZFClassFilterTypeInclude:
            ret += ZFTOKEN_ZFClassFilterTypeInclude;
            return ;
        case ZFClassFilterTypeExclude:
            ret += ZFTOKEN_ZFClassFilterTypeExclude;
            return ;
        case ZFClassFilterTypeIncludeChildOf:
            ret += ZFTOKEN_ZFClassFilterTypeIncludeChildOf;
            return ;
        case ZFClassFilterTypeIncludeChildTypeOf:
            ret += ZFTOKEN_ZFClassFilterTypeIncludeChildTypeOf;
            return ;
        case ZFClassFilterTypeIncludeParentOf:
            ret += ZFTOKEN_ZFClassFilterTypeIncludeParentOf;
            return ;
        case ZFClassFilterTypeIncludeParentTypeOf:
            ret += ZFTOKEN_ZFClassFilterTypeIncludeParentTypeOf;
            return ;
        case ZFClassFilterTypeExcludeChildOf:
            ret += ZFTOKEN_ZFClassFilterTypeExcludeChildOf;
            return ;
        case ZFClassFilterTypeExcludeChildTypeOf:
            ret += ZFTOKEN_ZFClassFilterTypeExcludeChildTypeOf;
            return ;
        case ZFClassFilterTypeExcludeParentOf:
            ret += ZFTOKEN_ZFClassFilterTypeExcludeParentOf;
            return ;
        case ZFClassFilterTypeExcludeParentTypeOf:
            ret += ZFTOKEN_ZFClassFilterTypeExcludeParentTypeOf;
            return ;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
const zfchar *ZFClassFilterTypeFromString(ZF_OUT ZFClassFilterType &ret,
                                          ZF_IN const zfchar *src,
                                          ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFClassFilterTypeInclude,
        ZFTOKEN_ZFClassFilterTypeExclude,
        ZFTOKEN_ZFClassFilterTypeIncludeChildOf,
        ZFTOKEN_ZFClassFilterTypeIncludeChildTypeOf,
        ZFTOKEN_ZFClassFilterTypeIncludeParentOf,
        ZFTOKEN_ZFClassFilterTypeIncludeParentTypeOf,
        ZFTOKEN_ZFClassFilterTypeExcludeChildOf,
        ZFTOKEN_ZFClassFilterTypeExcludeChildTypeOf,
        ZFTOKEN_ZFClassFilterTypeExcludeParentOf,
        ZFTOKEN_ZFClassFilterTypeExcludeParentTypeOf,
    };
    zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
    ret = ZFClassFilterTypeInclude;
    switch(matched)
    {
        case 0:
            ret = ZFClassFilterTypeInclude;
            return zfnull;
        case 1:
            ret = ZFClassFilterTypeExclude;
            return zfnull;
        case 2:
            ret = ZFClassFilterTypeIncludeChildOf;
            return zfnull;
        case 3:
            ret = ZFClassFilterTypeIncludeChildTypeOf;
            return zfnull;
        case 4:
            ret = ZFClassFilterTypeIncludeParentOf;
            return zfnull;
        case 5:
            ret = ZFClassFilterTypeIncludeParentTypeOf;
            return zfnull;
        case 6:
            ret = ZFClassFilterTypeExcludeChildOf;
            return zfnull;
        case 7:
            ret = ZFClassFilterTypeExcludeChildTypeOf;
            return zfnull;
        case 8:
            ret = ZFClassFilterTypeExcludeParentOf;
            return zfnull;
        case 9:
            ret = ZFClassFilterTypeExcludeParentTypeOf;
            return zfnull;
        default:
            return src;
    }
}

// ============================================================
/** @cond ZFPrivateDoc */
ZFClassFilter::ZFClassFilter(void)
: _ZFP_ZFClassFilter_filters()
, _ZFP_ZFClassFilter_customFilterCallbacks()
{
}
ZFClassFilter::ZFClassFilter(ZF_IN const ZFClassFilter &ref)
: _ZFP_ZFClassFilter_filters(ref._ZFP_ZFClassFilter_filters)
, _ZFP_ZFClassFilter_customFilterCallbacks(ref._ZFP_ZFClassFilter_customFilterCallbacks)
{
}
ZFClassFilter &ZFClassFilter::operator = (ZF_IN const ZFClassFilter &ref)
{
    this->_ZFP_ZFClassFilter_filters = ref._ZFP_ZFClassFilter_filters;
    this->_ZFP_ZFClassFilter_customFilterCallbacks = ref._ZFP_ZFClassFilter_customFilterCallbacks;
    return *this;
}
/** @endcond */
ZFClassFilter::~ZFClassFilter(void)
{
}
void ZFClassFilter::copyFrom(ZF_IN ZFClassFilter const &ref)
{
    this->_ZFP_ZFClassFilter_filters.copyFrom(ref._ZFP_ZFClassFilter_filters);
    this->_ZFP_ZFClassFilter_customFilterCallbacks.copyFrom(ref._ZFP_ZFClassFilter_customFilterCallbacks);
}

ZFClassFilter &ZFClassFilter::filterAdd(ZF_IN const ZFClass * const &e,
                                        ZF_IN_OPT ZFClassFilterType filterType /* = ZFClassFilterTypeExclude */)
{
    if(e == zfnull)
    {
        return *this;
    }

    _ZFP_ZFClassFilterData filterData;
    filterData.filterClass = e;
    filterData.filterType = filterType;
    this->_ZFP_ZFClassFilter_filters.add(filterData);
    return *this;
}
ZFClassFilter &ZFClassFilter::filterRemove(ZF_IN const ZFClass * const &e,
                                           ZF_IN_OPT ZFClassFilterType filterType /* = ZFClassFilterTypeExclude */)
{
    for(zfindex i = 0; i < this->_ZFP_ZFClassFilter_filters.count(); ++i)
    {
        if(this->_ZFP_ZFClassFilter_filters.get(i).filterClass == e && this->_ZFP_ZFClassFilter_filters.get(i).filterType == filterType)
        {
            this->_ZFP_ZFClassFilter_filters.remove(i);
            break;
        }
    }
    return *this;
}
ZFClassFilter &ZFClassFilter::filterRemoveAtIndex(ZF_IN zfindex index)
{
    this->_ZFP_ZFClassFilter_filters.remove(index);
    return *this;
}

zfindex ZFClassFilter::filterCount(void) const
{
    return this->_ZFP_ZFClassFilter_filters.count();
}
const ZFClass *ZFClassFilter::filterGetFilterClass(ZF_IN zfindex index) const
{
    return this->_ZFP_ZFClassFilter_filters.get(index).filterClass;
}
ZFClassFilterType ZFClassFilter::filterGetFilterType(ZF_IN zfindex index) const
{
    return this->_ZFP_ZFClassFilter_filters.get(index).filterType;
}

ZFClassFilter &ZFClassFilter::customFilterCallbackAdd(ZF_IN ZFClassFilter::CustomFilterCallback customFilterCallback)
{
    this->_ZFP_ZFClassFilter_customFilterCallbacks.add(customFilterCallback);
    return *this;
}
ZFClassFilter &ZFClassFilter::customFilterCallbackRemove(ZF_IN ZFClassFilter::CustomFilterCallback customFilterCallback)
{
    for(zfiterator it = this->_ZFP_ZFClassFilter_customFilterCallbacks.iterator();
        this->_ZFP_ZFClassFilter_customFilterCallbacks.iteratorIsValid(it);
        this->_ZFP_ZFClassFilter_customFilterCallbacks.iteratorNext(it))
    {
        if(this->_ZFP_ZFClassFilter_customFilterCallbacks.iteratorGet(it) == customFilterCallback)
        {
            this->_ZFP_ZFClassFilter_customFilterCallbacks.iteratorRemove(it);
            break;
        }
    }
    return *this;
}
ZFClassFilter &ZFClassFilter::customFilterCallbackRemove(ZF_IN zfindex index)
{
    this->_ZFP_ZFClassFilter_customFilterCallbacks.remove(index);
    return *this;
}
zfindex ZFClassFilter::customFilterCallbackCount(void) const
{
    return this->_ZFP_ZFClassFilter_customFilterCallbacks.count();
}
ZFClassFilter::CustomFilterCallback ZFClassFilter::customFilterCallbackAtIndex(ZF_IN zfindex index) const
{
    return this->_ZFP_ZFClassFilter_customFilterCallbacks.get(index);
}

zfbool ZFClassFilter::filterCheckActive(ZF_IN const ZFClass * const &e) const
{
    if(e == zfnull)
    {
        return zffalse;
    }

    for(zfindex i = 0, count = this->_ZFP_ZFClassFilter_customFilterCallbacks.count(); i < count; ++i)
    {
        switch(this->_ZFP_ZFClassFilter_customFilterCallbacks.get(i)(e))
        {
            case ZFFilterCallbackResultNotSpecified:
                break;
            case ZFFilterCallbackResultActive:
                return zftrue;
            case ZFFilterCallbackResultNotActive:
                return zffalse;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
    }

    zfbool hasIncludeMode = zffalse;
    zfbool included = zffalse;
    for(zfindex i = 0, count = this->_ZFP_ZFClassFilter_filters.count(); i < count; ++i)
    {
        const _ZFP_ZFClassFilterData &filter = this->_ZFP_ZFClassFilter_filters.get(i);
        switch(filter.filterType)
        {
            case ZFClassFilterTypeInclude:
                hasIncludeMode = zftrue;
                if(filter.filterClass == e)
                {
                    included = zftrue;
                }
                break;
            case ZFClassFilterTypeExclude:
                if(filter.filterClass == e)
                {
                    return zffalse;
                }
                break;
            case ZFClassFilterTypeIncludeChildOf:
                if(!e->classIsSubclassOf(filter.filterClass))
                {
                    return zffalse;
                }
                break;
            case ZFClassFilterTypeIncludeChildTypeOf:
                if(!e->classIsTypeOf(filter.filterClass))
                {
                    return zffalse;
                }
                break;
            case ZFClassFilterTypeIncludeParentOf:
                if(!filter.filterClass->classIsSubclassOf(e))
                {
                    return zffalse;
                }
                break;
            case ZFClassFilterTypeIncludeParentTypeOf:
                if(!filter.filterClass->classIsTypeOf(e))
                {
                    return zffalse;
                }
                break;
            case ZFClassFilterTypeExcludeChildOf:
                if(e->classIsSubclassOf(filter.filterClass))
                {
                    return zffalse;
                }
                break;
            case ZFClassFilterTypeExcludeChildTypeOf:
                if(e->classIsTypeOf(filter.filterClass))
                {
                    return zffalse;
                }
                break;
            case ZFClassFilterTypeExcludeParentOf:
                if(filter.filterClass->classIsSubclassOf(e))
                {
                    return zffalse;
                }
                break;
            case ZFClassFilterTypeExcludeParentTypeOf:
                if(filter.filterClass->classIsTypeOf(e))
                {
                    return zffalse;
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    return (!hasIncludeMode || included);
}

static void _ZFP_ZFClassFilter_contentInfoGetter(ZF_IN_OUT zfstring &ret, ZF_IN const _ZFP_ZFClassFilterData &v)
{
    ret += '(';
    ZFClassFilterTypeToString(ret, v.filterType);
    ret += ' ';
    ret += v.filterClass->className();
    ret += ')';
}
void ZFClassFilter::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    this->_ZFP_ZFClassFilter_filters.objectInfoOfContentT(ret,
        _ZFP_ZFClassFilter_contentInfoGetter,
        zfHint("max count")5,
        ZFTokenForContainerDefault);
}

ZF_NAMESPACE_GLOBAL_END

