/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFPropertyFilterDef.h
 * @brief filter for ZFProperty types
 */

#ifndef _ZFI_ZFPropertyFilterDef_h_
#define _ZFI_ZFPropertyFilterDef_h_

#include "ZFClassFilterDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFProperty;
// ============================================================
zfclassPOD ZF_ENV_EXPORT _ZFP_ZFPropertyFilterSetting
{
public:
    zfbool ignorePublicSetter;
    zfbool ignoreProtectedSetter;
    zfbool ignorePrivateSetter;
    zfbool ignorePublicGetter;
    zfbool ignoreProtectedGetter;
    zfbool ignorePrivateGetter;
};
/**
 * @brief util class to filter ZFProperty
 */
zfclassLikePOD ZF_ENV_EXPORT ZFPropertyFilter : zfextendsLikePOD ZFFilterBasic<const ZFProperty *>
{
    ZFFILTER_DECLARE(ZFPropertyFilter, ZFFilterBasic<const ZFProperty *>)

public:
    /**
     * @brief see #ZFFilterBase
     */
    ZFPropertyFilter(void)
    : zfsuper()
    , classFilter()
    , d()
    {
        _ZFP_ZFPropertyFilterSetting *setting = zfnew(_ZFP_ZFPropertyFilterSetting);
        setting->ignorePublicSetter = zffalse;
        setting->ignoreProtectedSetter = zffalse;
        setting->ignorePrivateSetter = zffalse;
        setting->ignorePublicGetter = zffalse;
        setting->ignoreProtectedGetter = zffalse;
        setting->ignorePrivateGetter = zffalse;
        d = setting;
    }
    /**
     * @brief see #ZFFilterBase
     */
    ZFPropertyFilter(ZF_IN const ZFPropertyFilter &ref)
    : zfsuper()
    , classFilter(ref.classFilter)
    , d(ref.d)
    {
    }

    /** @cond ZFPrivateDoc */
    virtual ZFPropertyFilter &operator = (ZF_IN const ZFPropertyFilter &ref)
    {
        zfsuper::operator = (ref);
        this->classFilter = ref.classFilter;
        this->d = ref.d;
        return *this;
    }
    virtual void copyFrom(ZF_IN ZFPropertyFilter const &ref)
    {
        zfsuper::copyFrom(ref);
        this->classFilter.copyFrom(ref.classFilter);
        _ZFP_ZFPropertyFilterSetting *setting = zfnew(_ZFP_ZFPropertyFilterSetting);
        zfmemcpy(setting, ref.d.pointerValueGet(), sizeof(_ZFP_ZFPropertyFilterSetting));
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
     * @brief ignore public setter, false by default
     */
    virtual void ignorePublicSetterSet(ZF_IN zfbool value)
    {
        d->ignorePublicSetter = value;
    }
    /**
     * @brief see #ignorePublicSetterSet
     */
    virtual zfbool ignorePublicSetter(void) const
    {
        return d->ignorePublicSetter;
    }

    /**
     * @brief ignore protected setter, false by default
     */
    virtual void ignoreProtectedSetterSet(ZF_IN zfbool value)
    {
        d->ignoreProtectedSetter = value;
    }
    /**
     * @brief see #ignoreProtectedSetterSet
     */
    virtual zfbool ignoreProtectedSetter(void) const
    {
        return d->ignoreProtectedSetter;
    }

    /**
     * @brief ignore private setter, false by default
     */
    virtual void ignorePrivateSetterSet(ZF_IN zfbool value)
    {
        d->ignorePrivateSetter = value;
    }
    /**
     * @brief see #ignorePrivateSetterSet
     */
    virtual zfbool ignorePrivateSetter(void) const
    {
        return d->ignorePrivateSetter;
    }

    /**
     * @brief ignore public getter, false by default
     */
    virtual void ignorePublicGetterSet(ZF_IN zfbool value)
    {
        d->ignorePublicGetter = value;
    }
    /**
     * @brief see #ignorePublicGetterSet
     */
    virtual zfbool ignorePublicGetter(void) const
    {
        return d->ignorePublicGetter;
    }

    /**
     * @brief ignore protected getter, false by default
     */
    virtual void ignoreProtectedGetterSet(ZF_IN zfbool value)
    {
        d->ignoreProtectedGetter = value;
    }
    /**
     * @brief see #ignoreProtectedGetterSet
     */
    virtual zfbool ignoreProtectedGetter(void) const
    {
        return d->ignoreProtectedGetter;
    }

    /**
     * @brief ignore private getter, false by default
     */
    virtual void ignorePrivateGetterSet(ZF_IN zfbool value)
    {
        d->ignorePrivateGetter = value;
    }
    /**
     * @brief see #ignorePrivateGetterSet
     */
    virtual zfbool ignorePrivateGetter(void) const
    {
        return d->ignorePrivateGetter;
    }

public:
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) const;

protected:
    /** @cond ZFPrivateDoc */ // ZFDoxygenBugFix: can't auto detect template specialization
    zfoverride
    virtual zfbool filterOnCheckValid(ZF_IN const ZFProperty * const &e) const
    {
        return (e != zfnull);
    }
    zfoverride
    virtual zfbool filterOnCheckActive(ZF_IN const ZFProperty * const &e) const;
    /** @endcond */

private:
    ZFCorePointerForPOD<_ZFP_ZFPropertyFilterSetting *> d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyFilterDef_h_

