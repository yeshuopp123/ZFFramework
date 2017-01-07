/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZF2048UIBlock.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZF2048UIBlock)

ZFOBJECT_REGISTER(ZF2048UIBlock)
ZFCACHEABLE_DEFINE_WITH_MAX(ZF2048UIBlock, ZF2048UIBlock, 16)

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZF2048UIBlock, ZF2048Value, blockValue)
{
    this->blockValueSetInternal(newValue);

    if(newValue == 0)
    {
        zfSkinApplyCancelZFStyleable(this);
        this->blockTitle()->viewVisibleSet(zffalse);
        return ;
    }

    zfstring skinKey = zfstringWithFormat(zfText("ZF2048_block_%d"), newValue);
    zfautoObject skin = zfSkin(skinKey);
    if(skin == zfautoObjectNull)
    {
        zfSkinApplyZFStyleable(this, zfText("ZF2048_block_na"));
        this->blockTitle()->textContentStringSet(zfstringWithFormat(zfText("%d"), this->blockValue()));
        this->blockTitle()->viewVisibleSet(zftrue);
    }
    else
    {
        zfSkinApplyZFStyleable(this, skinKey);
        this->blockTitle()->viewVisibleSet(zffalse);
    }
}

ZFObject *ZF2048UIBlock::objectOnInit(void)
{
    zfsuper::objectOnInit();
    this->internalForegroundViewAdd(this->blockTitle());
    this->blockTitle()->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
    this->blockTitle()->viewVisibleSet(zffalse);
    this->blockTitle()->textAlignSet(ZFUIAlign::e_Center);
    this->blockTitle()->textColorSet(ZFUIColorWhite);
    this->blockTitle()->textShadowColorSet(ZFUIColorGrayDark);
    this->blockTitle()->textAppearanceSet(ZFUITextAppearance::e_Bold);
    this->blockTitle()->textSizeSet(24);
    return this;
}

ZF_NAMESPACE_GLOBAL_END

