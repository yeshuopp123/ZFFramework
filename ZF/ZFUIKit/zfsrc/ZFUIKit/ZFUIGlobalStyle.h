/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIGlobalStyle.h
 * @brief global style for ui elements
 */

#ifndef _ZFI_ZFUIGlobalStyle_h_
#define _ZFI_ZFUIGlobalStyle_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief global style for ui elements
 */
zfclass ZF_ENV_EXPORT ZFUIGlobalStyle : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUIGlobalStyle, ZFStyleableObject)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIGlobalStyle)

public:
    // ============================================================
    // image scale
    /**
     * @brief resource image's base scale, 2 by default to suit most case
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, imageScale, ZFPropertyInitValue(2.0f))

    // ============================================================
    // common size hints
    /**
     * @brief space between inner items
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSpace, ZFPropertyInitValue(8))
    /**
     * @brief space to outer containers
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemMargin, ZFPropertyInitValue(8))

    // ============================================================
    // item size hints
    /**
     * @brief text item size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeText, ZFPropertyInitValue(21))
    /**
     * @brief small control item size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeControl, ZFPropertyInitValue(32))
    /**
     * @brief button size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeButton, ZFPropertyInitValue(44))
    /**
     * @brief button size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeButtonWidth, ZFPropertyInitValue(68))
    /**
     * @brief icon size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeIcon, ZFPropertyInitValue(40))
    /**
     * @brief icon size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeIconSmall, ZFPropertyInitValue(32))
    /**
     * @brief list cell size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeListCell, ZFPropertyInitValue(48))
    /**
     * @brief dialog item size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeDialog, ZFPropertyInitValue(80))
    /**
     * @brief dialog item size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeDialogWidth, ZFPropertyInitValue(200))

    // ============================================================
    // text size hints
    /**
     * @brief extra tiny text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeExtraTiny, ZFPropertyInitValue(8))
    /**
     * @brief tiny text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeTiny, ZFPropertyInitValue(10))
    /**
     * @brief small text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeSmall, ZFPropertyInitValue(12))
    /**
     * @brief normal text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeNormal, ZFPropertyInitValue(14))
    /**
     * @brief big text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeBig, ZFPropertyInitValue(16))
    /**
     * @brief large text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeLarge, ZFPropertyInitValue(18))
    /**
     * @brief extra large text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeExtraLarge, ZFPropertyInitValue(20))

    // ============================================================
    // text settings
    /**
     * @brief default text appearance
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUITextAppearanceEnum, textAppearance, ZFPropertyInitValue(ZFUITextAppearance::EnumDefault()))
    /**
     * @brief default text align
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIAlignFlags, textAlign, ZFPropertyInitValue(ZFUIAlign::e_LeftInner))
    /**
     * @brief default text truncate mode
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUITextTruncateModeEnum, textTruncateMode, ZFPropertyInitValue(ZFUITextTruncateMode::EnumDefault()))

    // ============================================================
    // generanl colors
    /**
     * @brief default background color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, colorBgDefault, ZFPropertyInitValue(ZFUIColorGrayLight))
    /**
     * @brief item background color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, colorBgItem, ZFPropertyInitValue(ZFUIColorWhite))
    /**
     * @brief separator color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, colorSeparator, ZFPropertyInitValue(ZFUIColorGrayLight))
    /**
     * @brief shadow color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, colorShadow, ZFPropertyInitValue(ZFUIColorChangeA(ZFUIColorBlack, 128)))
    /**
     * @brief mask color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, colorMask, ZFPropertyInitValue(ZFUIColorChangeA(ZFUIColorBlack, 128)))
    /**
     * @brief tint color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, colorTint, ZFPropertyInitValue(ZFUIColorGrayLight))

    // ============================================================
    // text colors
    /**
     * @brief default text color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColorDefault, ZFPropertyInitValue(ZFUIColorBlack))
    /**
     * @brief secondary default text color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColorSecondary, ZFPropertyInitValue(ZFUIColorGray))
    /**
     * @brief hint text color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColorHint, ZFPropertyInitValue(ZFUIColorGrayLight))
    /**
     * @brief link text color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColorLink, ZFPropertyInitValue(ZFUIColorBlue))
    /**
     * @brief visited link text color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColorLinkVisited, ZFPropertyInitValue(ZFUIColorMagenta))

    // ============================================================
    // animations
    /**
     * @brief short animation delay in miliseconds
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniDelayShort, ZFPropertyInitValue(100))
    /**
     * @brief normal animation delay in miliseconds
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniDelayNormal, ZFPropertyInitValue(250))
    /**
     * @brief long animation delay in miliseconds
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniDelayLong, ZFPropertyInitValue(500))

    /**
     * @brief short animation duration in miliseconds
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniDurationShort, ZFPropertyInitValue(100))
    /**
     * @brief normal animation duration in miliseconds
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniDurationNormal, ZFPropertyInitValue(250))
    /**
     * @brief long animation duration in miliseconds
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniDurationLong, ZFPropertyInitValue(500))

    // ============================================================
    // control
    /**
     * @brief normal control color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, controlColorNormal, ZFPropertyInitValue(ZFUIColorBlack))
    /**
     * @brief highlighted control color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, controlColorHighlighted, ZFPropertyInitValue(ZFUIColorBlack))
    /**
     * @brief checked control color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, controlColorChecked, ZFPropertyInitValue(ZFUIColorBlack))
    /**
     * @brief checked highlighted control color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, controlColorCheckedHighlighted, ZFPropertyInitValue(ZFUIColorBlack))
    /**
     * @brief disabled control color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, controlColorDisabled, ZFPropertyInitValue(ZFUIColorGray))
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIGlobalStyle_h_

