/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFLeakTestObjectFilterDef.h
 * @brief object filter for leak test
 */

#ifndef _ZFI_ZFLeakTestObjectFilterDef_h_
#define _ZFI_ZFLeakTestObjectFilterDef_h_

#include "ZFObjectClassTypeFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFObject;

// ============================================================
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFLeakTestObjectFilterData
{
public:
    zfstring name;
    ZFFilterType filterType;
};
/**
 * @brief filter used by #ZFLeakTestPrintStatus
 *
 * you may use it like this:
 * @code
 *   SomeClass *obj = ...;
 *   ZFLeakTestPrintStatus(ZFLeakTestObjectFilter()
 *       .filterAddName(zfText("SomeClass"))
 *       .filterAddName(SomeClass::ClassData()->className())
 *       .filterAdd(obj)
 *       );
 * @endcode
 * the object added to the filter won't be retained,
 * and is safe to dealloc even if in the filter
 */
zfclassLikePOD ZF_ENV_EXPORT ZFLeakTestObjectFilter : zfextendsLikePOD ZFFilterBasic<ZFObject *>
{
    ZFFILTER_DECLARE(ZFLeakTestObjectFilter, ZFFilterBasic<ZFObject *>)

public:
    /** @cond ZFPrivateDoc */
    ZFLeakTestObjectFilter(void);
    ZFLeakTestObjectFilter(ZF_IN const ZFLeakTestObjectFilter &ref);
    virtual ZFLeakTestObjectFilter &operator =(ZF_IN const ZFLeakTestObjectFilter &ref);
    /** @endcond */
    virtual ~ZFLeakTestObjectFilter(void);
    /**
     * @brief add a object name to the filter, no wild card support
     */
    virtual ZFLeakTestObjectFilter &filterAddName(ZF_IN const zfchar *name,
                                                  ZF_IN_OPT ZFFilterType filterType = ZFFilterTypeExclude);
    /**
     * @brief add a object to the filter
     *
     * the obj won't be retained
     */
    virtual ZFLeakTestObjectFilter &filterAdd(ZF_IN ZFObject * const &obj,
                                              ZF_IN_OPT ZFFilterType filterType = ZFFilterTypeExclude);

protected:
    /** @cond ZFPrivateDoc */ // ZFDoxygenBugFix: can't auto detect template specialization
    zfoverride
    virtual zfbool filterOnCheckActive(ZF_IN ZFObject * const &obj) const;
    /** @endcond */

private:
    ZFCoreArray<_ZFP_ZFLeakTestObjectFilterData> _ZFP_ZFLeakTestObjectFilter_nameFilters;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLeakTestObjectFilterDef_h_

