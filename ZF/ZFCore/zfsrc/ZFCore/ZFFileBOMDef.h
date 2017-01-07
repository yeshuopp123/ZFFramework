/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFFileBOMDef.h
 * @brief file BOM defines
 */

#ifndef _ZFI_ZFFileBOMDef_h_
#define _ZFI_ZFFileBOMDef_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief max BOM size
 */
#define ZFFileBOMMaxSize 8
/**
 * @brief file's BOM info, '\0' terminated
 */
zfclassPOD ZF_ENV_EXPORT ZFFileBOM
{
public:
    /**
     * @brief file's BOM info, '\0' terminated
     */
    zfbyte BOM[ZFFileBOMMaxSize];
};

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFFileBOM, ZFFileBOM)

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFFileBOMList, ZFCoreArrayPOD<ZFFileBOM>)
/**
 * @brief see #ZFFileBOMListToString
 */
extern ZF_ENV_EXPORT void ZFFileBOMListToString(ZF_IN_OUT zfstring &ret,
                                                ZF_IN const ZFFileBOM *BOMList,
                                                ZF_IN zfindex BOMListCount);
/** @brief see #ZFFileBOMListToString */
inline zfstring ZFFileBOMListToString(ZF_IN const ZFFileBOM *BOMList,
                                      ZF_IN zfindex BOMListCount)
{
    zfstring ret;
    ZFFileBOMListToString(ret, BOMList, BOMListCount);
    return ret;
}

ZFCORE_POD_COMPARER_DECLARE(ZFFileBOM)

extern ZF_ENV_EXPORT const ZFFileBOM &_ZFP_ZFFileBOMUTF8(void);
/**
 * @brief UTF8 BOM
 */
#define ZFFileBOMUTF8 (_ZFP_ZFFileBOMUTF8())

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFileBOMDef_h_

