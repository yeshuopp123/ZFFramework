/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZF2048UIBlock_h_
#define _ZFI_ZF2048UIBlock_h_

#include "ZFUIWidget.h"
#include "../ZF2048Core/ZF2048Types.h"
#include "ZF2048UISkin.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZF2048UIBlock
/**
 * @brief linear container view that layout children in linear by specified orientation
 */
zfclass ZF2048UIBlock : zfextends ZFUIImageView, zfimplements ZFCacheable
{
    ZFOBJECT_DECLARE(ZF2048UIBlock, ZFUIImageView)
    ZFIMPLEMENTS_DECLARE(ZFCacheable)
    ZFSTYLE_DEFAULT_DECLARE(ZF2048UIBlock)
    ZFCACHEABLE_DECLARE(ZF2048UIBlock)

public:
    ZFPROPERTY_ASSIGN_WITH_INIT(ZF2048Value, blockValue,
                                ZFPropertyInitValue(0))
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN(ZF2048Value, blockValue)
    {
        propertyValue = 0;
    }
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZF2048Value, blockValue);

    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, blockTitle, ZFPropertyInitValue(zflineAlloc(ZFUITextView)))

protected:
    zfoverride
    virtual void cacheableOnReset(void)
    {
        zfsuperI(ZFCacheable)::cacheableOnReset();
        this->blockValueSet(0);
    }

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048UIBlock_h_

