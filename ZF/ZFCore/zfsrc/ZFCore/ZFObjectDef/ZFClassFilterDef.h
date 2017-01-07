/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFClassFilterDef.h
 * @brief filter for ZFClass types
 */

#ifndef _ZFI_ZFClassFilterDef_h_
#define _ZFI_ZFClassFilterDef_h_

#include "ZFObjectClassTypeFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFClass;
// ============================================================
/**
 * @brief filter type for #ZFClassFilter
 */
typedef enum {
    /**
     * @brief exactly the same
     */
    ZFClassFilterTypeInclude,
    /**
     * @brief exactly not the same
     */
    ZFClassFilterTypeExclude,

    /**
     * @brief cls->classIsSubclassOf(clsToCheck)
     */
    ZFClassFilterTypeIncludeChildOf,
    /**
     * @brief cls->classIsTypeOf(clsToCheck)
     */
    ZFClassFilterTypeIncludeChildTypeOf,
    /**
     * @brief clsToCheck->classIsSubclassOf(cls)
     */
    ZFClassFilterTypeIncludeParentOf,
    /**
     * @brief clsToCheck->classIsTypeOf(cls)
     */
    ZFClassFilterTypeIncludeParentTypeOf,

    /**
     * @brief !cls->classIsSubclassOf(clsToCheck)
     */
    ZFClassFilterTypeExcludeChildOf,
    /**
     * @brief !cls->classIsTypeOf(clsToCheck)
     */
    ZFClassFilterTypeExcludeChildTypeOf,
    /**
     * @brief !clsToCheck->classIsSubclassOf(cls)
     */
    ZFClassFilterTypeExcludeParentOf,
    /**
     * @brief !clsToCheck->classIsTypeOf(cls)
     */
    ZFClassFilterTypeExcludeParentTypeOf,
} ZFClassFilterType;
/** @brief string tokens */
#define ZFTOKEN_ZFClassFilterTypeInclude zfText("Include")
/** @brief string tokens */
#define ZFTOKEN_ZFClassFilterTypeExclude zfText("Exclude")
/** @brief string tokens */
#define ZFTOKEN_ZFClassFilterTypeIncludeChildOf zfText("IncludeChildOf")
/** @brief string tokens */
#define ZFTOKEN_ZFClassFilterTypeIncludeChildTypeOf zfText("IncludeChildTypeOf")
/** @brief string tokens */
#define ZFTOKEN_ZFClassFilterTypeIncludeParentOf zfText("IncludeParentOf")
/** @brief string tokens */
#define ZFTOKEN_ZFClassFilterTypeIncludeParentTypeOf zfText("IncludeParentTypeOf")
/** @brief string tokens */
#define ZFTOKEN_ZFClassFilterTypeExcludeChildOf zfText("ExcludeChildOf")
/** @brief string tokens */
#define ZFTOKEN_ZFClassFilterTypeExcludeChildTypeOf zfText("ExcludeChildTypeOf")
/** @brief string tokens */
#define ZFTOKEN_ZFClassFilterTypeExcludeParentOf zfText("ExcludeParentOf")
/** @brief string tokens */
#define ZFTOKEN_ZFClassFilterTypeExcludeParentTypeOf zfText("ExcludeParentTypeOf")

ZFCOMPARER_DEFAULT_DECLARE_BEGIN(ZFClassFilterType, e0, ZFClassFilterType, e1)
{
    return ((e0 == e1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCOMPARER_DEFAULT_DECLARE_END(ZFClassFilterType, e0, ZFClassFilterType, e1)

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFClassFilterType, ZFClassFilterType)

// ============================================================
zfclassPOD ZF_ENV_EXPORT _ZFP_ZFClassFilterData
{
public:
    const ZFClass *filterClass;
    ZFClassFilterType filterType;
};
/**
 * @brief util class to filter ZFClass
 */
zfclassLikePOD ZF_ENV_EXPORT ZFClassFilter
{
public:
    /**
     * @brief see #ZFFilterBase::CustomFilterCallback
     */
    typedef ZFFilterCallbackResult (*CustomFilterCallback)(ZF_IN const ZFClass * const &e);

public:
    /**
     * @brief main constructor
     */
    ZFClassFilter(void);
    /**
     * @brief constructor from another filter, retain only, use #copyFrom if you want copy
     */
    ZFClassFilter(ZF_IN const ZFClassFilter &ref);
    /**
     * @brief retain only, use #copyFrom if you want copy
     */
    virtual ZFClassFilter &operator = (ZF_IN const ZFClassFilter &ref);
    virtual ~ZFClassFilter(void);
    /**
     * @brief copy from another filter
     */
    virtual void copyFrom(ZF_IN ZFClassFilter const &ref);

public:
    /**
     * @brief add a filter
     */
    virtual ZFClassFilter &filterAdd(ZF_IN const ZFClass * const &e,
                                     ZF_IN_OPT ZFClassFilterType filterType = ZFClassFilterTypeExclude);
    /**
     * @brief remove a filter (remove first only, if duplicated filter added)
     */
    virtual ZFClassFilter &filterRemove(ZF_IN const ZFClass * const &e,
                                        ZF_IN_OPT ZFClassFilterType filterType = ZFClassFilterTypeExclude);
    /**
     * @brief remove
     */
    virtual ZFClassFilter &filterRemoveAtIndex(ZF_IN zfindex index);

public:
    /**
     * @brief get filter count
     */
    virtual zfindex filterCount(void) const;
    /**
     * @brief get filter class at index
     */
    virtual const ZFClass *filterGetFilterClass(ZF_IN zfindex index) const;
    /**
     * @brief get filter type at index
     */
    virtual ZFClassFilterType filterGetFilterType(ZF_IN zfindex index) const;
    /**
     * @brief add a custom filter callback
     */
    virtual ZFClassFilter &customFilterCallbackAdd(ZF_IN ZFClassFilter::CustomFilterCallback customFilterCallback);
    /**
     * @brief remove custom filter callback
     */
    virtual ZFClassFilter &customFilterCallbackRemove(ZF_IN ZFClassFilter::CustomFilterCallback customFilterCallback);
    /**
     * @brief remove custom filter callback
     */
    virtual ZFClassFilter &customFilterCallbackRemove(ZF_IN zfindex index);
    /**
     * @brief get custom filter callback count
     */
    virtual zfindex customFilterCallbackCount(void) const;
    /**
     * @brief get custom filter callback
     */
    virtual ZFClassFilter::CustomFilterCallback customFilterCallbackAtIndex(ZF_IN zfindex index) const;

public:
    /**
     * @brief return true if e not filtered out
     */
    virtual zfbool filterCheckActive(ZF_IN const ZFClass * const &e) const;

public:
    /**
     * @brief get a short info about the filter
     */
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #objectInfoT */
    virtual inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

private:
    ZFCoreArrayPOD<_ZFP_ZFClassFilterData> _ZFP_ZFClassFilter_filters;
    ZFCoreArrayPOD<ZFClassFilter::CustomFilterCallback> _ZFP_ZFClassFilter_customFilterCallbacks;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFClassFilterDef_h_

