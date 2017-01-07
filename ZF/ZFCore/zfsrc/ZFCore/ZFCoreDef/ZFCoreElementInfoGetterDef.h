/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreElementInfoGetterDef.h
 * @brief template type util for element access
 */

#ifndef _ZFI_ZFCoreElementInfoGetterDef_h_
#define _ZFI_ZFCoreElementInfoGetterDef_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief used to add extra token while output element info
 */
template<typename T_Element, typename T_ReservedFix = void>
zfclassNotPOD ZF_ENV_EXPORT ZFCoreElementTokenGetter
{
public:
    /** @brief see #ZFCoreElementTokenGetter */
    static void elementTokenGetterLeft(ZF_IN_OUT zfstring &ret, ZF_IN T_Element const &v)
    {
    }
    /** @brief see #ZFCoreElementTokenGetter */
    static void elementTokenGetterRight(ZF_IN_OUT zfstring &ret, ZF_IN T_Element const &v)
    {
    }
};

// ============================================================
/**
 * @brief content info getter for template type
 *
 * you should specialize this class template to output your custom type:
 * @code
 *   template<>
 *   zfclassNotPOD ZFCoreElementInfoGetter<YourType>
 *   {
 *   public:
 *       static zfstring elementInfoGetter(ZF_IN YourType const &v)
 *       {
 *           // return your own type's info
 *       }
 *   };
 * @endcode
 */
template<typename T_Element, typename T_ReservedFix = void>
zfclassNotPOD ZF_ENV_EXPORT ZFCoreElementInfoGetter
{
public:
    /** @brief see #ZFCoreElementInfoGetter */
    static void elementInfoGetter(ZF_IN_OUT zfstring &ret, ZF_IN T_Element const &v)
    {
        ret += ZFTOKEN_ZFCoreInfoGetterNotAvailable;
    }
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreElementInfoGetterDef_h_

#include "ZFCoreElementInfoGetterDef_common.h"

