/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFKeyValuePair.h
 * @brief ZFObject key value pair
 */

#ifndef _ZFI_ZFKeyValuePair_h_
#define _ZFI_ZFKeyValuePair_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief key value pair as POD type, no auto retain logic, designed for performance
 */
zffinal zfclassPOD ZF_ENV_EXPORT ZFKeyValuePair
{
public:
    ZFObject *key; /**< @brief key, no auto retain */
    ZFObject *value; /**< @brief value, no auto retain */
};
/**
 * @brief make a ZFKeyValuePair
 */
inline ZFKeyValuePair ZFKeyValuePairMake(ZF_IN ZFObject *key,
                                         ZF_IN ZFObject *value)
{
    ZFKeyValuePair ret = {key, value};
    return ret;
}
/**
 * @brief an empty pair
 */
extern ZF_ENV_EXPORT const ZFKeyValuePair ZFKeyValuePairZero;

/**
 * @brief key value pair with auto retain logic
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFKeyValuePairAutoRelease
{
public:
    zfautoObject key; /**< @brief key */
    zfautoObject value; /**< @brief value */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFKeyValuePair_h_

