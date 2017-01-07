/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPropertyFilterDef.h"
#include "ZFPropertyDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static void _ZFP_ZFPropertyFilterGetSettingInfo(ZF_OUT zfstring &ret,
                                                ZF_IN const ZFPropertyFilter *filter)
{
    ret += '(';
    zfindex flagsCount = 0;
    if(filter->ignorePublicSetter())
    {
        ++flagsCount;
        ret += zfText("ignorePublicSetter, ");
    }
    if(filter->ignoreProtectedSetter())
    {
        ++flagsCount;
        ret += zfText("ignoreProtectedSetter, ");
    }
    if(filter->ignorePrivateSetter())
    {
        ++flagsCount;
        ret += zfText("ignorePrivateSetter, ");
    }
    if(filter->ignorePublicGetter())
    {
        ++flagsCount;
        ret += zfText("ignorePublicGetter, ");
    }
    if(filter->ignoreProtectedGetter())
    {
        ++flagsCount;
        ret += zfText("ignoreProtectedGetter, ");
    }
    if(filter->ignorePrivateGetter())
    {
        ++flagsCount;
        ret += zfText("ignorePrivateGetter, ");
    }

    if(flagsCount == 0)
    {
        ret.remove(ret.length() - 1);
    }
    else
    {
        ret.remove(ret.length() - 2);
        ret += ')';
    }
}
void ZFPropertyFilter::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    _ZFP_ZFPropertyFilterGetSettingInfo(ret, this);
    zfsuper::objectInfoT(ret);
}

/** @cond ZFPrivateDoc */ // ZFDoxygenBugFix: can't auto detect template specialization
zfbool ZFPropertyFilter::filterOnCheckActive(ZF_IN const ZFProperty * const &e) const
{
    if(!zfsuper::filterOnCheckActive(e)
       || !this->classFilter.filterCheckActive(e->ownerClass()))
    {
        return zffalse;
    }

    if((e->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate && this->ignorePrivateSetter())
       || (e->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypeProtected && this->ignoreProtectedSetter())
       || (e->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePublic && this->ignorePublicSetter())
       || (e->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate && this->ignorePrivateGetter())
       || (e->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypeProtected && this->ignoreProtectedGetter())
       || (e->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePublic && this->ignorePublicGetter()))
    {
        return zffalse;
    }

    return zftrue;
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

