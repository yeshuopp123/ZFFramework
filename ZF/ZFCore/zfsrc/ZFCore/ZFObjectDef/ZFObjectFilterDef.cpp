/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFObjectFilterDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFFilterForZFObject::ZFFilterForZFObject(void)
: zfsuper()
, classFilter()
{
}
ZFFilterForZFObject::ZFFilterForZFObject(ZF_IN const ZFFilterForZFObject &ref)
: zfsuper(ref)
, classFilter(ref.classFilter)
{
}
ZFFilterForZFObject &ZFFilterForZFObject::operator =(ZF_IN const ZFFilterForZFObject &ref)
{
    zfsuper::operator =(ref);
    this->classFilter = ref.classFilter;
    return *this;
}
ZFFilterForZFObject::~ZFFilterForZFObject(void)
{
    for(zfindex i = 0; i < this->filterCount(); ++i)
    {
        zfRelease(this->filterGetInternal(i));
    }
}

/** @cond ZFPrivateDoc */
void ZFFilterForZFObject::copyFrom(ZF_IN ZFFilterForZFObject const &ref)
{
    zfsuper::copyFrom(ref);
    for(zfindex i = 0; i < this->filterCount(); ++i)
    {
        zfRetain(this->filterGetInternal(i));
    }
    this->classFilter.copyFrom(ref.classFilter);
}
/** @endcond */

/** @cond ZFPrivateDoc */ // ZFDoxygenBugFix: can't auto detect template specialization
zfbool ZFFilterForZFObject::filterOnCheckValid(ZF_IN ZFObject * const &e) const
{
    return (e != zfnull);
}
void ZFFilterForZFObject::filterOnStore(ZF_IN_OUT ZFObject * &to,
                                        ZF_IN ZFObject * const &from) const
{
    to = zfRetain(from);
}
void ZFFilterForZFObject::filterOnRemove(ZF_IN_OUT ZFObject * &value) const
{
    zfRelease(value);
}
zfbool ZFFilterForZFObject::filterOnCheckEqual(ZF_IN ZFObject * const &e1,
                                               ZF_IN ZFObject * const &e2) const
{
    return (ZFObjectCompare(e1, e2) == ZFCompareTheSame);
}
zfbool ZFFilterForZFObject::filterOnCheckActive(ZF_IN ZFObject * const &e) const
{
    if(!zfsuper::filterOnCheckActive(e))
    {
        return zffalse;
    }
    return this->classFilter.filterCheckActive(e->classData());
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

