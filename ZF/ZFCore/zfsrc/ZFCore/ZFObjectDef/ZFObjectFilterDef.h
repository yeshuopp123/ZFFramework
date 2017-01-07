/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectFilterDef.h
 * @brief filter define for ZFObject
 */

#ifndef _ZFI_ZFObjectFilterDef_h_
#define _ZFI_ZFObjectFilterDef_h_

#include "ZFObjectDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFFilterForZFObject
/**
 * @brief object filter compared by #ZFObject::objectCompare
 */
zfclassLikePOD ZF_ENV_EXPORT ZFFilterForZFObject : zfextendsLikePOD ZFFilterBasic<ZFObject *>
{
    ZFFILTER_DECLARE(ZFFilterForZFObject, ZFFilterBasic<ZFObject *>)

public:
    /**
     * @brief see #ZFFilterBase
     */
    ZFFilterForZFObject(void);
    /**
     * @brief see #ZFFilterBase
     */
    ZFFilterForZFObject(ZF_IN const ZFFilterForZFObject &ref);
    /**
     * @brief see #ZFFilterBase
     */
    virtual ZFFilterForZFObject &operator =(ZF_IN const ZFFilterForZFObject &ref);
    virtual ~ZFFilterForZFObject(void);

    /** @cond ZFPrivateDoc */
    virtual void copyFrom(ZF_IN ZFFilterForZFObject const &ref);
    /** @endcond */

public:
    /**
     * @brief filter the object by class
     */
    ZFClassFilter classFilter;

protected:
    /** @cond ZFPrivateDoc */ // ZFDoxygenBugFix: can't auto detect template specialization
    zfoverride
    virtual zfbool filterOnCheckValid(ZF_IN ZFObject * const &e) const;
    zfoverride
    virtual void filterOnStore(ZF_IN_OUT ZFObject * &to,
                               ZF_IN ZFObject * const &from) const;
    zfoverride
    virtual void filterOnRemove(ZF_IN_OUT ZFObject * &to) const;
    zfoverride
    virtual zfbool filterOnCheckEqual(ZF_IN ZFObject * const &e1,
                                      ZF_IN ZFObject * const &e2) const;
    zfoverride
    virtual zfbool filterOnCheckActive(ZF_IN ZFObject * const &e) const;
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectFilterDef_h_

