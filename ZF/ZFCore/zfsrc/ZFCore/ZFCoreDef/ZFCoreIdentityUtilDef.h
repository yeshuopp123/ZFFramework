/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreIdentityUtilDef.h
 * @brief identity generator utility
 */

#ifndef _ZFI_ZFCoreIdentityUtilDef_h_
#define _ZFI_ZFCoreIdentityUtilDef_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreArrayDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFIdentityGeneratorPrivate;
/**
 * @brief identity generator utility
 *
 * used to generate an unused identity
 * @warning this class is not thread-safe,
 *   you must achieve thread-safe manually,
 *   such as #ZFCoreMutexLock or #zfsynchronize
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFIdentityGenerator
{
public:
    /** @cond ZFPrivateDoc */
    ZFIdentityGenerator(void);
    ZFIdentityGenerator(ZF_IN ZFIdentityGenerator const &ref);
    ZFIdentityGenerator &operator = (ZF_IN ZFIdentityGenerator const &ref);
    ~ZFIdentityGenerator(void);
    /** @endcond */

public:
    /**
     * @brief get current identity,
     *   may be invalid or used,
     *   typically you should use #next
     */
    zfidentity current(void) const;
    /**
     * @brief get next identity,
     *   ensured valid and ensured unused
     */
    zfidentity next(void);
    /**
     * @brief get next identity,
     *   ensured valid and ensured unused,
     *   and auto mark it as used,
     *   see #next, #markUsed
     */
    zfidentity nextMarkUsed(void);
    /**
     * @brief true if the identity is not used
     */
    zfbool used(ZF_IN zfidentity identity) const;
    /**
     * @brief mark the identity used
     *
     * by default, #zfidentityInvalid would be marked as used
     */
    void markUsed(ZF_IN zfidentity identity);
    /**
     * @brief mark the identity unused
     *
     * by default, #zfidentityInvalid would be marked as used
     */
    void markUnused(ZF_IN zfidentity identity);

    /**
     * @brief remove all data
     */
    void removeAll(void);

public:
    /**
     * @brief get all identity currently used, for debug use only
     */
    void allUsed(ZF_IN_OUT ZFCoreArray<zfidentity> &ret) const;
    /** @brief see #allUsed */
    inline ZFCoreArrayPOD<zfidentity> allUsed(void) const
    {
        ZFCoreArrayPOD<zfidentity> ret;
        this->allUsed(ret);
        return ret;
    }

private:
    _ZFP_ZFIdentityGeneratorPrivate *d;
};

// ============================================================
/**
 * @brief calculate identity from string
 */
extern ZF_ENV_EXPORT zfidentity zfidentityCalcString(ZF_IN const zfchar *start);
/**
 * @brief calculate identity from raw buffer
 */
extern ZF_ENV_EXPORT zfidentity zfidentityCalcBuf(ZF_IN const void *start, ZF_IN zfindex byteSize);
/**
 * @brief calculate identity from POD object
 */
template<typename T_POD>
zfidentity zfidentityCalcPOD(ZF_IN T_POD const &value)
{
    return zfidentityCalcBuf(&value, sizeof(T_POD));
}

/**
 * @brief make a identity from pointer value
 */
extern ZF_ENV_EXPORT zfidentity zfidentityCalcPointer(ZF_IN const void *p);
/**
 * @brief make a identity from pointer value
 */
extern ZF_ENV_EXPORT zfidentity zfidentityCalcPointer(ZF_IN ZFFuncAddrType p);
/**
 * @brief make a identity from pointer value
 */
extern ZF_ENV_EXPORT zfidentity zfidentityCalcPointer(ZF_IN ZFMemberFuncAddrType p);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreIdentityUtilDef_h_

