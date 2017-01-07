/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCopyableDef.h
 * @brief interface shows a ZFObject is copyable
 */

#ifndef _ZFI_ZFCopyableDef_h_
#define _ZFI_ZFCopyableDef_h_

#include "ZFObjectDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief interface shows a ZFObject is copyable
 */
zfinterface ZF_ENV_EXPORT ZFCopyable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFCopyable, ZFInterface)

public:
    /**
     * @brief return a copy of this object
     */
    zffinal zfautoObject copy(void);
    /**
     * @brief see #copy, anotherObj must be same type as this object (by classData),
     *   otherwise, do nothing
     */
    zffinal void copyFrom(ZF_IN ZFObject *anotherObj);

protected:
    /**
     * @brief called by #copy to copy contents from anotherObj
     *
     * anotherObj is ensured the same type as self,
     * ensured not null,
     * and ensured not same instance of this\n
     * by default this method would do nothing for performance,
     * you may use #ZFPropertyCopyAll to copy all properties for most case
     */
    virtual inline void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCopyableDef_h_

