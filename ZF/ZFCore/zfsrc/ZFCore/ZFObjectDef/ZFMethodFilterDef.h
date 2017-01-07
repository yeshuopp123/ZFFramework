/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFMethodFilterDef.h
 * @brief filter for ZFMethod types
 */

#ifndef _ZFI_ZFMethodFilterDef_h_
#define _ZFI_ZFMethodFilterDef_h_

#include "ZFClassFilterDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFMethod;
// ============================================================
zfclassPOD ZF_ENV_EXPORT _ZFP_ZFMethodFilterSetting
{
public:
    zfbool ignorePublic;
    zfbool ignoreProtected;
    zfbool ignorePrivate;
};
/**
 * @brief util class to filter ZFMethod
 */
zfclassLikePOD ZF_ENV_EXPORT ZFMethodFilter : zfextendsLikePOD ZFFilterBasic<const ZFMethod *>
{
    ZFFILTER_DECLARE(ZFMethodFilter, ZFFilterBasic<const ZFMethod *>)

public:
    /**
     * @brief see #ZFFilterBase
     */
    ZFMethodFilter(void)
    : zfsuper()
    , classFilter()
    , d()
    {
        _ZFP_ZFMethodFilterSetting *setting = zfnew(_ZFP_ZFMethodFilterSetting);
        setting->ignorePublic = zffalse;
        setting->ignoreProtected = zffalse;
        setting->ignorePrivate = zffalse;
        d = setting;
    }
    /**
     * @brief see #ZFFilterBase
     */
    ZFMethodFilter(ZF_IN const ZFMethodFilter &ref)
    : zfsuper()
    , classFilter(ref.classFilter)
    , d(ref.d)
    {
    }
    /** @cond ZFPrivateDoc */
    virtual ZFMethodFilter &operator = (ZF_IN const ZFMethodFilter &ref)
    {
        zfsuper::operator = (ref);
        this->classFilter = ref.classFilter;
        this->d = ref.d;
        return *this;
    }
    virtual void copyFrom(ZF_IN ZFMethodFilter const &ref)
    {
        zfsuper::copyFrom(ref);
        this->classFilter.copyFrom(ref.classFilter);
        _ZFP_ZFMethodFilterSetting *setting = zfnew(_ZFP_ZFMethodFilterSetting);
        zfmemcpy(setting, ref.d.pointerValueGet(), sizeof(_ZFP_ZFMethodFilterSetting));
        d = setting;
    }
    /** @endcond */

public:
    /**
     * @brief filter the method's owner class
     */
    ZFClassFilter classFilter;

public:
    /**
     * @brief ignore public method, false by default
     */
    virtual void ignorePublicSet(ZF_IN zfbool value)
    {
        d->ignorePublic = value;
    }
    /**
     * @brief see #ignorePublicSet
     */
    virtual zfbool ignorePublic(void) const
    {
        return d->ignorePublic;
    }

    /**
     * @brief ignore protected method, false by default
     */
    virtual void ignoreProtectedSet(ZF_IN zfbool value)
    {
        d->ignoreProtected = value;
    }
    /**
     * @brief see #ignoreProtectedSet
     */
    virtual zfbool ignoreProtected(void) const
    {
        return d->ignoreProtected;
    }

    /**
     * @brief ignore private method, false by default
     */
    virtual void ignorePrivateSet(ZF_IN zfbool value)
    {
        d->ignorePrivate = value;
    }
    /**
     * @brief see #ignorePrivateSet
     */
    virtual zfbool ignorePrivate(void) const
    {
        return d->ignorePrivate;
    }

public:
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) const;

protected:
    /** @cond ZFPrivateDoc */ // ZFDoxygenBugFix: can't auto detect template specialization
    zfoverride
    virtual zfbool filterOnCheckValid(ZF_IN const ZFMethod * const &e) const
    {
        return (e != zfnull);
    }
    zfoverride
    virtual zfbool filterOnCheckActive(ZF_IN const ZFMethod * const &e) const;
    /** @endcond */

private:
    ZFCorePointerForPOD<_ZFP_ZFMethodFilterSetting *> d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodFilterDef_h_

