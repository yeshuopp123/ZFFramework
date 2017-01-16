/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFFilterDef.h
 * @brief filters in ZFFramework
 */

#ifndef _ZFI_ZFFilterDef_h_
#define _ZFI_ZFFilterDef_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreUtilStringConvertDef.h"
#include "ZFCoreUtilComparerDef.h"
#include "ZFCoreArrayDef.h"
#include "ZFCoreSPrintfDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief filter type
 */
typedef enum {
    ZFFilterTypeInclude = 1,
    ZFFilterTypeExclude = 2,
} ZFFilterType;
/** @brief string tokens */
#define ZFTOKEN_ZFFilterTypeInclude zfText("Include")
/** @brief string tokens */
#define ZFTOKEN_ZFFilterTypeExclude zfText("Exclude")

ZFCOMPARER_DEFAULT_DECLARE(ZFFilterType, ZFFilterType, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFFilterType, ZFFilterType)

/**
 * @brief filter result for custom filter callback
 */
typedef enum {
    ZFFilterCallbackResultNotSpecified,
    ZFFilterCallbackResultActive,
    ZFFilterCallbackResultNotActive,
} ZFFilterCallbackResult;
/** @brief string tokens */
#define ZFTOKEN_ZFFilterCallbackResultNotSpecified zfText("NotSpecified")
/** @brief string tokens */
#define ZFTOKEN_ZFFilterCallbackResultActive zfText("Active")
/** @brief string tokens */
#define ZFTOKEN_ZFFilterCallbackResultNotActive zfText("NotActive")

ZFCOMPARER_DEFAULT_DECLARE(ZFFilterCallbackResult, ZFFilterCallbackResult, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFFilterCallbackResult, ZFFilterCallbackResult)

/**
 * @brief declare a filter class
 *
 * since filter usually a template class,
 * it needs some trick to declare a filter
 * @code
 *   ZFFILTER_DECLARE(
 *       ChildClass<T_Public ZFM_COMMA() T_Internal>,
 *       SuperClass<T_Public ZFM_COMMA() T_Internal>)
 * @endcode
 */
#define ZFFILTER_DECLARE(ChildClass, SuperClass) \
    protected: \
        /** @brief class ref to super */ \
        typedef SuperClass zfsuper; \
        /** @brief class ref to self */ \
        typedef ChildClass zfself; \
    public:

// ============================================================
template<typename T_Element>
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFFilterBaseFilterData
{
public:
    T_Element element;
    ZFFilterType filterType;
};

/**
 * @brief base class of filter
 */
template<typename T_Public, typename T_Internal = T_Public>
zfclassLikePOD ZF_ENV_EXPORT ZFFilterBase
{
protected:
    /** @brief typedef for self */
    typedef ZFFilterBase<T_Public, T_Internal> zfself;

public:
    /**
     * @brief custom filter callback for ZFFilter
     */
    typedef ZFFilterCallbackResult (*CustomFilterCallback)(ZF_IN T_Public const &e);

public:
    /**
     * @brief main constructor
     */
    ZFFilterBase(void)
    : _ZFP_ZFFilterBase_filters()
    , _ZFP_ZFFilterBase_customFilterCallbacks(zfnull)
    {
    }
    /**
     * @brief construct from another filter, retain only, you should use #copyFrom to copy contents
     */
    ZFFilterBase(ZF_IN const ZFFilterBase<T_Public, T_Internal> &ref)
    : _ZFP_ZFFilterBase_filters(ref._ZFP_ZFFilterBase_filters)
    , _ZFP_ZFFilterBase_customFilterCallbacks(zfnull)
    {
        if(ref._ZFP_ZFFilterBase_customFilterCallbacks != zfnull)
        {
            this->_ZFP_ZFFilterBase_customFilterCallbacks = zfnew(
                _ZFP_ZFFilterBase_CustomFilterCallbacksType,
                *ref._ZFP_ZFFilterBase_customFilterCallbacks);
        }
    }
    /**
     * @brief retain only
     */
    virtual ZFFilterBase<T_Public, T_Internal> &operator =(ZF_IN const ZFFilterBase<T_Public, T_Internal> &ref)
    {
        if(this != &ref)
        {
            this->_ZFP_ZFFilterBase_filters = ref._ZFP_ZFFilterBase_filters;
            if(this->_ZFP_ZFFilterBase_customFilterCallbacks != zfnull)
            {
                zfdelete(this->_ZFP_ZFFilterBase_customFilterCallbacks);
                this->_ZFP_ZFFilterBase_customFilterCallbacks = zfnull;
            }
            if(ref._ZFP_ZFFilterBase_customFilterCallbacks != zfnull)
            {
                this->_ZFP_ZFFilterBase_customFilterCallbacks = zfnew(
                    _ZFP_ZFFilterBase_CustomFilterCallbacksType,
                    *ref._ZFP_ZFFilterBase_customFilterCallbacks);
            }
        }
        return *this;
    }
    virtual ~ZFFilterBase(void)
    {
    }
public:
    /**
     * @brief copy from another filter
     */
    virtual void copyFrom(ZF_IN ZFFilterBase<T_Public, T_Internal> const &ref)
    {
        this->_ZFP_ZFFilterBase_filters.copyFrom(ref._ZFP_ZFFilterBase_filters);
        if(ref._ZFP_ZFFilterBase_customFilterCallbacks != zfnull)
        {
            if(this->_ZFP_ZFFilterBase_customFilterCallbacks == zfnull)
            {
                this->_ZFP_ZFFilterBase_customFilterCallbacks = zfnew(_ZFP_ZFFilterBase_CustomFilterCallbacksType);
            }
            this->_ZFP_ZFFilterBase_customFilterCallbacks->copyFrom(*ref._ZFP_ZFFilterBase_customFilterCallbacks);
        }
        else
        {
            if(this->_ZFP_ZFFilterBase_customFilterCallbacks != zfnull)
            {
                zfdelete(this->_ZFP_ZFFilterBase_customFilterCallbacks);
            }
        }
    }

public:
    /**
     * @brief add a element
     *
     * note this method won't check duplicated data for performance
     */
    virtual ZFFilterBase<T_Public, T_Internal> &filterAdd(ZF_IN T_Public const &e,
                                                          ZF_IN_OPT ZFFilterType filterType = ZFFilterTypeExclude)
    {
        if(this->filterOnCheckValid(e))
        {
            _ZFP_ZFFilterBaseFilterData<T_Internal> filterData;
            this->filterOnStore(filterData.element, e);
            filterData.filterType = filterType;
            this->_ZFP_ZFFilterBase_filters.add(filterData);
        }
        return *this;
    }
    /**
     * @brief remove a element
     */
    virtual ZFFilterBase<T_Public, T_Internal> &filterRemove(ZF_IN T_Public const &e,
                                                             ZF_IN_OPT ZFFilterType filterType = ZFFilterTypeExclude)
    {
        if(this->filterOnCheckValid(e))
        {
            for(zfindex i = 0; i < this->_ZFP_ZFFilterBase_filters.count(); ++i)
            {
                if(this->filterOnCheckEqual(this->_ZFP_ZFFilterBase_filters.get(i).element, e)
                    && this->_ZFP_ZFFilterBase_filters.get(i).filterType == filterType)
                {
                    this->filterOnRemove(this->_ZFP_ZFFilterBase_filters.get(i).element);
                    this->_ZFP_ZFFilterBase_filters.remove(i);
                    break;
                }
            }
        }
        return *this;
    }
    /**
     * @brief remove a element
     */
    virtual ZFFilterBase<T_Public, T_Internal> &filterRemoveAtIndex(ZF_IN zfindex index)
    {
        this->filterOnRemove(this->_ZFP_ZFFilterBase_filters.get(index).element);
        this->_ZFP_ZFFilterBase_filters.remove(index);
        return *this;
    }
    /**
     * @brief remove all contents of this filter
     */
    virtual void filterRemoveAll(void)
    {
        for(zfindex i = 0; i < this->_ZFP_ZFFilterBase_filters.count(); ++i)
        {
            this->filterOnRemove(this->_ZFP_ZFFilterBase_filters.get(i).element);
        }
        this->_ZFP_ZFFilterBase_filters.removeAll();
        zfdelete(this->_ZFP_ZFFilterBase_customFilterCallbacks);
        this->_ZFP_ZFFilterBase_customFilterCallbacks = zfnull;
    }
    /**
     * @brief get count of element
     */
    virtual zfindex filterCount(void) const
    {
        return this->_ZFP_ZFFilterBase_filters.count();
    }
    /**
     * @brief get element at index
     */
    virtual T_Public filterGet(ZF_IN zfindex index) const
    {
        T_Public t;
        this->filterOnAccess(t, this->_ZFP_ZFFilterBase_filters.get(index).element);
        return t;
    }
    /**
     * @brief get element at index with internal type
     */
    virtual T_Internal const &filterGetInternal(ZF_IN zfindex index) const
    {
        return this->_ZFP_ZFFilterBase_filters.get(index).element;
    }
    /**
     * @brief get filter type for filter data at index
     */
    virtual ZFFilterType filterGetFilterType(ZF_IN zfindex index) const
    {
        return this->_ZFP_ZFFilterBase_filters.get(index).filterType;
    }
    /**
     * @brief add a custom filter callback
     */
    virtual ZFFilterBase<T_Public, T_Internal> &customFilterCallbackAdd(ZF_IN typename ZFFilterBase<T_Public, T_Internal>::CustomFilterCallback customFilterCallback)
    {
        if(this->_ZFP_ZFFilterBase_customFilterCallbacks == zfnull)
        {
            this->_ZFP_ZFFilterBase_customFilterCallbacks = zfnew(_ZFP_ZFFilterBase_CustomFilterCallbacksType);
        }
        this->_ZFP_ZFFilterBase_customFilterCallbacks->add(customFilterCallback);
        return *this;
    }
    /**
     * @brief remove custom filter callback
     */
    virtual ZFFilterBase<T_Public, T_Internal> &customFilterCallbackRemove(ZF_IN typename ZFFilterBase<T_Public, T_Internal>::CustomFilterCallback customFilterCallback)
    {
        if(this->_ZFP_ZFFilterBase_customFilterCallbacks != zfnull)
        {
            for(zfiterator it = this->_ZFP_ZFFilterBase_customFilterCallbacks->iterator();
                this->_ZFP_ZFFilterBase_customFilterCallbacks->iteratorIsValid(it);
                this->_ZFP_ZFFilterBase_customFilterCallbacks->iteratorNext(it))
            {
                if(this->_ZFP_ZFFilterBase_customFilterCallbacks->iteratorGet(it) == customFilterCallback)
                {
                    this->_ZFP_ZFFilterBase_customFilterCallbacks->iteratorRemove(it);
                    break;
                }
            }
        }
        return *this;
    }
    /**
     * @brief remove custom filter callback
     */
    virtual ZFFilterBase<T_Public, T_Internal> &customFilterCallbackRemove(ZF_IN zfindex index)
    {
        if(this->_ZFP_ZFFilterBase_customFilterCallbacks != zfnull)
        {
            this->_ZFP_ZFFilterBase_customFilterCallbacks->remove(index);
        }
        return *this;
    }
    /**
     * @brief get custom filter callback count
     */
    virtual zfindex customFilterCallbackCount(void) const
    {
        if(this->_ZFP_ZFFilterBase_customFilterCallbacks != zfnull)
        {
            return this->_ZFP_ZFFilterBase_customFilterCallbacks->count();
        }
        return 0;
    }
    /**
     * @brief get custom filter callback
     */
    virtual typename ZFFilterBase<T_Public, T_Internal>::CustomFilterCallback customFilterCallbackAtIndex(ZF_IN zfindex index) const
    {
        return this->_ZFP_ZFFilterBase_customFilterCallbacks->get(index);
    }
    /**
     * @brief return true if e is not filtered by this filter, see #ZFFilterType
     */
    virtual zfbool filterCheckActive(ZF_IN T_Public const &e) const
    {
        if(!this->filterOnCheckValid(e))
        {
            return zffalse;
        }
        if(this->_ZFP_ZFFilterBase_customFilterCallbacks != zfnull)
        {
            for(zfindex i = 0; i < this->_ZFP_ZFFilterBase_customFilterCallbacks->count(); ++i)
            {
                switch(this->_ZFP_ZFFilterBase_customFilterCallbacks->get(i)(e))
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
        }
        return this->filterOnCheckActive(e);
    }

private:
    static void _ZFP_ZFFilterBase_contentInfoGetter(ZF_IN_OUT zfstring &ret, ZF_IN const _ZFP_ZFFilterBaseFilterData<T_Internal> &v)
    {
        ret += '(';
        switch(v.filterType)
        {
            case ZFFilterTypeInclude:
                ret += zfText("include ");
                break;
            case ZFFilterTypeExclude:
                ret += zfText("exclude ");
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
        zfstringAppend(ret, zfText("%p"), &(v.element));
        ret += ')';
    }
public:
    /**
     * @brief get a short info
     */
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) const
    {
        this->_ZFP_ZFFilterBase_filters.objectInfoOfContentT(ret,
            _ZFP_ZFFilterBase_contentInfoGetter,
            zfHint("max count")5,
            ZFTokenForContainerDefault);
    }
    /** @brief see #objectInfoT */
    virtual inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

protected:
    /**
     * @brief return true if element is valid for a filter
     *
     * if false, won't be added to content by #filterAdd or compared by #filterOnCheckEqual
     */
    virtual zfbool filterOnCheckValid(ZF_IN T_Public const &e) const
    {
        return zftrue;
    }
    /**
     * @brief store public type to internal storage type
     */
    virtual void filterOnStore(ZF_IN_OUT T_Internal &to,
                                ZF_IN T_Public const &from) const = 0;
    /**
     * @brief called before value removed from filter
     */
    virtual void filterOnRemove(ZF_IN_OUT T_Internal &value) const {}
    /**
     * @brief access public type from internal storage type
     */
    virtual void filterOnAccess(ZF_IN_OUT T_Public &to,
                                 ZF_IN T_Internal const &from) const = 0;
    /**
     * @brief true if e1 is regarded as equal to e2
     *
     * subclass may override this to supply custom comparation
     */
    virtual zfbool filterOnCheckEqual(ZF_IN T_Internal const &e1,
                                        ZF_IN T_Public const &e2) const = 0;
    /**
     * @brief called by #filterCheckActive to check whether the element is filtered out
     */
    virtual zfbool filterOnCheckActive(ZF_IN T_Public const &e) const
    {
        zfbool hasIncludeMode = zffalse;
        zfbool included = zffalse;
        for(zfindex i = 0; i < this->_ZFP_ZFFilterBase_filters.count(); ++i)
        {
            const _ZFP_ZFFilterBaseFilterData<T_Internal> &filter = this->_ZFP_ZFFilterBase_filters.get(i);
            switch(filter.filterType)
            {
                case ZFFilterTypeInclude:
                    hasIncludeMode = zftrue;
                    if(this->filterOnCheckEqual(filter.element, e))
                    {
                        included = zftrue;
                    }
                    break;
                case ZFFilterTypeExclude:
                    if(this->filterOnCheckEqual(filter.element, e))
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

private:
    typedef ZFCoreArray<_ZFP_ZFFilterBaseFilterData<T_Internal> > _ZFP_ZFFilterBase_FiltersType;
    typedef ZFCoreArrayPOD<typename ZFFilterBase<T_Public, T_Internal>::CustomFilterCallback> _ZFP_ZFFilterBase_CustomFilterCallbacksType;
    _ZFP_ZFFilterBase_FiltersType _ZFP_ZFFilterBase_filters;
    _ZFP_ZFFilterBase_CustomFilterCallbacksType *_ZFP_ZFFilterBase_customFilterCallbacks;
};

// ============================================================
/**
 * @brief basic filter
 *
 * a basic filter is:
 * -  support operator = to assign value between public type and internal storage type
 * -  support operator == to compare whether public type is equal to internal storage type
 */
template<typename T_Public, typename T_Internal = T_Public>
zfclassLikePOD ZF_ENV_EXPORT ZFFilterBasic : zfextendsLikePOD ZFFilterBase<T_Public, T_Internal>
{
    ZFFILTER_DECLARE(
        ZFFilterBasic<T_Public ZFM_COMMA() T_Internal>,
        ZFFilterBase<T_Public ZFM_COMMA() T_Internal>)

public:
    /**
     * @brief see #ZFFilterBasic
     */
    ZFFilterBasic(void)
    : zfsuper()
    {
    }
    /**
     * @brief see #ZFFilterBasic
     */
    ZFFilterBasic(ZF_IN const ZFFilterBasic<T_Public, T_Internal> &ref)
    : zfsuper(ref)
    {
    }
    /** @cond ZFPrivateDoc */
    virtual ZFFilterBasic<T_Public, T_Internal> &operator =(ZF_IN const ZFFilterBasic<T_Public, T_Internal> &ref)
    {
        zfsuper::operator =(ref);
        return *this;
    }
    /** @endcond */
    virtual ~ZFFilterBasic(void) {}

protected:
    /** @cond ZFPrivateDoc */
    zfoverride
    virtual void filterOnStore(ZF_IN_OUT T_Internal &to,
                               ZF_IN T_Public const &from) const
    {
        to = from;
    }
    zfoverride
    virtual void filterOnAccess(ZF_IN_OUT T_Public &to,
                                ZF_IN T_Internal const &from) const
    {
        to = from;
    }
    zfoverride
    virtual zfbool filterOnCheckEqual(ZF_IN T_Internal const &e1,
                                      ZF_IN T_Public const &e2) const
    {
        return (e1 == e2);
    }
    /** @endcond */
};

/**
 * @brief number filter with zfint as filter type
 */
typedef ZFFilterBasic<zfint> ZFFilterForNumber;

/**
 * @brief number filter with zfint as filter type
 */
typedef ZFFilterBasic<zfindex> ZFFilterForIndex;

// ============================================================
/**
 * @brief base class of string filter
 */
template<typename T_Public, typename T_Internal>
zfclassLikePOD ZF_ENV_EXPORT ZFFilterForStringBase : zfextendsLikePOD ZFFilterBase<T_Public, T_Internal>
{
    ZFFILTER_DECLARE(
        ZFFilterForStringBase<T_Public ZFM_COMMA() T_Internal>,
        ZFFilterBase<T_Public ZFM_COMMA() T_Internal>)

public:
    /**
     * @brief main constructor
     */
    ZFFilterForStringBase(void)
    : zfsuper()
    {
    }
    /**
     * @brief see #ZFFilterBase
     */
    ZFFilterForStringBase(ZF_IN const ZFFilterForStringBase<T_Public, T_Internal> &ref)
    : zfsuper(ref)
    {
    }
    /** @cond ZFPrivateDoc */
    virtual ZFFilterForStringBase<T_Public, T_Internal> &operator =(ZF_IN const ZFFilterForStringBase<T_Public, T_Internal> &ref)
    {
        zfsuper::operator =(ref);
        return *this;
    }
    /** @endcond */
    virtual ~ZFFilterForStringBase(void) {}

protected:
    zfoverride
    virtual zfbool filterOnCheckValid(ZF_IN T_Public const &e) const
    {
        return (e != zfnull);
    }
    zfoverride
    virtual void filterOnStore(ZF_IN_OUT T_Internal &to,
                                ZF_IN T_Public const &from) const
    {
        to = from;
    }
    zfoverride
    virtual void filterOnAccess(ZF_IN_OUT T_Public &to,
                                 ZF_IN T_Internal const &from) const
    {
        to = from.cString();
    }
    zfoverride
    virtual zfbool filterOnCheckEqual(ZF_IN T_Internal const &e1,
                                       ZF_IN T_Public const &e2) const
    {
        return (e1.compare(e2) == 0);
    }
};

/**
 * @brief string filter with const zfchar * as public type and zfstring as internal storage type
 */
typedef ZFFilterForStringBase<const zfchar *, zfstring> ZFFilterForString;

/**
 * @brief string filter with const zfcharA * as public type and zfstringA as internal storage type
 */
typedef ZFFilterForStringBase<const zfcharA *, zfstringA> ZFFilterForStringA;

/**
 * @brief string filter with const zfcharW * as public type and zfstringW as internal storage type
 */
typedef ZFFilterForStringBase<const zfcharW *, zfstringW> ZFFilterForStringW;

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFFilterDef_h_

