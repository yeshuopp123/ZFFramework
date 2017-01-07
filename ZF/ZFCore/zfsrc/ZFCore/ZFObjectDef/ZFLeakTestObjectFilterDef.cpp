/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFLeakTestObjectFilterDef.h"
#include "ZFObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFLeakTestObjectFilter

/** @cond ZFPrivateDoc */
ZFLeakTestObjectFilter::ZFLeakTestObjectFilter(void)
: zfsuper()
, _ZFP_ZFLeakTestObjectFilter_nameFilters()
{
}
ZFLeakTestObjectFilter::ZFLeakTestObjectFilter(ZF_IN const ZFLeakTestObjectFilter &ref)
: zfsuper(ref)
, _ZFP_ZFLeakTestObjectFilter_nameFilters()
{
    this->_ZFP_ZFLeakTestObjectFilter_nameFilters.copyFrom(ref._ZFP_ZFLeakTestObjectFilter_nameFilters);
}
ZFLeakTestObjectFilter &ZFLeakTestObjectFilter::operator =(ZF_IN const ZFLeakTestObjectFilter &ref)
{
    zfsuper::operator =(ref);
    this->_ZFP_ZFLeakTestObjectFilter_nameFilters.copyFrom(ref._ZFP_ZFLeakTestObjectFilter_nameFilters);
    return *this;
}
/** @endcond */
ZFLeakTestObjectFilter::~ZFLeakTestObjectFilter(void)
{
}
ZFLeakTestObjectFilter &ZFLeakTestObjectFilter::filterAddName(ZF_IN const zfchar *name,
                                                              ZF_IN_OPT ZFFilterType filterType /* = ZFFilterTypeExclude */)
{
    if(name != zfnull)
    {
        for(zfindex i = 0; i < this->_ZFP_ZFLeakTestObjectFilter_nameFilters.count(); ++i)
        {
            if(this->_ZFP_ZFLeakTestObjectFilter_nameFilters.get(i).name.compare(name) == 0)
            {
                return *this;
            }
        }
    }
    _ZFP_ZFLeakTestObjectFilterData nameFilterData;
    nameFilterData.name = name;
    nameFilterData.filterType = filterType;
    this->_ZFP_ZFLeakTestObjectFilter_nameFilters.add(nameFilterData);
    return *this;
}
ZFLeakTestObjectFilter &ZFLeakTestObjectFilter::filterAdd(ZF_IN ZFObject * const &obj,
                                                          ZF_IN_OPT ZFFilterType filterType /* = ZFFilterTypeExclude */)
{
    zfsuper::filterAdd(obj, filterType);
    return *this;
}

/** @cond ZFPrivateDoc */ // ZFDoxygenBugFix: can't auto detect template specialization
zfbool ZFLeakTestObjectFilter::filterOnCheckActive(ZF_IN ZFObject * const &obj) const
{
    if(!zfsuper::filterOnCheckActive(obj))
    {
        return zffalse;
    }
    zfbool hasIncludeMode = zffalse;
    zfbool included = zffalse;
    for(zfindex i = 0; i < this->_ZFP_ZFLeakTestObjectFilter_nameFilters.count(); ++i)
    {
        const _ZFP_ZFLeakTestObjectFilterData &nameFilter = this->_ZFP_ZFLeakTestObjectFilter_nameFilters.get(i);
        switch(nameFilter.filterType)
        {
            case ZFFilterTypeInclude:
                hasIncludeMode = zftrue;
                if(nameFilter.name.compare(obj->classData()->className()) == 0)
                {
                    included = zftrue;
                }
                break;
            case ZFFilterTypeExclude:
                if(nameFilter.name.compare(obj->classData()->className()) == 0)
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
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

