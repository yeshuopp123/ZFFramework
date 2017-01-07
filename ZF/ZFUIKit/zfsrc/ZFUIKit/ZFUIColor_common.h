/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIColor_common.h
 * @brief common color
 */

#ifndef _ZFI_ZFUIColor_common_h_
#define _ZFI_ZFUIColor_common_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// common colors
/**
 * @brief create a random color without alpha
 */
inline ZFUIColor ZFUIColorRandom(void)
{
    return ZFUIColorMake(255, zfmRand(255), zfmRand(255), zfmRand(255));
}
/**
 * @brief create a random color with specified alpha
 */
inline ZFUIColor ZFUIColorRandom(ZF_IN zfint alpha)
{
    return ZFUIColorMake(alpha, zfmRand(255), zfmRand(255), zfmRand(255));
}

/** @brief 0x00000000 */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorTransparent;
/** @brief 0xFF000000 */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorBlack;
/** @brief 0xFF555555 */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorGrayDark;
/** @brief 0xFF808080 */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorGray;
/** @brief 0xFFAAAAAA */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorGrayLight;
/** @brief 0xFFFFFFFF */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorWhite;

/** @brief 0xFFFF0000 */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorRed;
/** @brief 0xFF00FF00 */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorGreen;
/** @brief 0xFF0000FF */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorBlue;
/** @brief 0xFFFFFF00 */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorYellow;
/** @brief 0xFFFF00FF */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorMagenta;
/** @brief 0xFF00FFFF */
extern ZF_ENV_EXPORT ZFUIColor ZFUIColorCyan;

// ============================================================
// web colors
/** @brief 0xFFF0F8FF */
#define ZFUIColor_webAliceBlue ZFUIColorMake(0xFF, 0xF0, 0xF8, 0xFF)
/** @brief 0xFFFAEBD7 */
#define ZFUIColor_webAntiqueWhite ZFUIColorMake(0xFF, 0xFA, 0xEB, 0xD7)
/** @brief 0xFF00FFFF */
#define ZFUIColor_webAqua ZFUIColorMake(0xFF, 0x00, 0xFF, 0xFF)
/** @brief 0xFF7FFFD4 */
#define ZFUIColor_webAquamarine ZFUIColorMake(0xFF, 0x7F, 0xFF, 0xD4)
/** @brief 0xFFF0FFFF */
#define ZFUIColor_webAzure ZFUIColorMake(0xFF, 0xF0, 0xFF, 0xFF)
/** @brief 0xFFF5F5DC */
#define ZFUIColor_webBeige ZFUIColorMake(0xFF, 0xF5, 0xF5, 0xDC)
/** @brief 0xFFFFE4C4 */
#define ZFUIColor_webBisque ZFUIColorMake(0xFF, 0xFF, 0xE4, 0xC4)
/** @brief 0xFF000000 */
#define ZFUIColor_webBlack ZFUIColorMake(0xFF, 0x00, 0x00, 0x00)
/** @brief 0xFFFFEBCD */
#define ZFUIColor_webBlanchedAlmond ZFUIColorMake(0xFF, 0xFF, 0xEB, 0xCD)
/** @brief 0xFF0000FF */
#define ZFUIColor_webBlue ZFUIColorMake(0xFF, 0x00, 0x00, 0xFF)
/** @brief 0xFF8A2BE2 */
#define ZFUIColor_webBlueViolet ZFUIColorMake(0xFF, 0x8A, 0x2B, 0xE2)
/** @brief 0xFFA52A2A */
#define ZFUIColor_webBrown ZFUIColorMake(0xFF, 0xA5, 0x2A, 0x2A)
/** @brief 0xFFDEB887 */
#define ZFUIColor_webBurlyWood ZFUIColorMake(0xFF, 0xDE, 0xB8, 0x87)
/** @brief 0xFF5F9EA0 */
#define ZFUIColor_webCadetBlue ZFUIColorMake(0xFF, 0x5F, 0x9E, 0xA0)
/** @brief 0xFF7FFF00 */
#define ZFUIColor_webChartreuse ZFUIColorMake(0xFF, 0x7F, 0xFF, 0x00)
/** @brief 0xFFD2691E */
#define ZFUIColor_webChocolate ZFUIColorMake(0xFF, 0xD2, 0x69, 0x1E)
/** @brief 0xFFFF7F50 */
#define ZFUIColor_webCoral ZFUIColorMake(0xFF, 0xFF, 0x7F, 0x50)
/** @brief 0xFF6495ED */
#define ZFUIColor_webCornflowerBlue ZFUIColorMake(0xFF, 0x64, 0x95, 0xED)
/** @brief 0xFFFFF8DC */
#define ZFUIColor_webCornsilk ZFUIColorMake(0xFF, 0xFF, 0xF8, 0xDC)
/** @brief 0xFFDC143C */
#define ZFUIColor_webCrimson ZFUIColorMake(0xFF, 0xDC, 0x14, 0x3C)
/** @brief 0xFF00FFFF */
#define ZFUIColor_webCyan ZFUIColorMake(0xFF, 0x00, 0xFF, 0xFF)
/** @brief 0xFF00008B */
#define ZFUIColor_webDarkBlue ZFUIColorMake(0xFF, 0x00, 0x00, 0x8B)
/** @brief 0xFF008B8B */
#define ZFUIColor_webDarkCyan ZFUIColorMake(0xFF, 0x00, 0x8B, 0x8B)
/** @brief 0xFFB8860B */
#define ZFUIColor_webDarkGoldenRod ZFUIColorMake(0xFF, 0xB8, 0x86, 0x0B)
/** @brief 0xFFA9A9A9 */
#define ZFUIColor_webDarkGray ZFUIColorMake(0xFF, 0xA9, 0xA9, 0xA9)
/** @brief 0xFF006400 */
#define ZFUIColor_webDarkGreen ZFUIColorMake(0xFF, 0x00, 0x64, 0x00)
/** @brief 0xFFBDB76B */
#define ZFUIColor_webDarkKhaki ZFUIColorMake(0xFF, 0xBD, 0xB7, 0x6B)
/** @brief 0xFF8B008B */
#define ZFUIColor_webDarkMagenta ZFUIColorMake(0xFF, 0x8B, 0x00, 0x8B)
/** @brief 0xFF556B2F */
#define ZFUIColor_webDarkOliveGreen ZFUIColorMake(0xFF, 0x55, 0x6B, 0x2F)
/** @brief 0xFFFF8C00 */
#define ZFUIColor_webDarkOrange ZFUIColorMake(0xFF, 0xFF, 0x8C, 0x00)
/** @brief 0xFF9932CC */
#define ZFUIColor_webDarkOrchid ZFUIColorMake(0xFF, 0x99, 0x32, 0xCC)
/** @brief 0xFF8B0000 */
#define ZFUIColor_webDarkRed ZFUIColorMake(0xFF, 0x8B, 0x00, 0x00)
/** @brief 0xFFE9967A */
#define ZFUIColor_webDarkSalmon ZFUIColorMake(0xFF, 0xE9, 0x96, 0x7A)
/** @brief 0xFF8FBC8F */
#define ZFUIColor_webDarkSeaGreen ZFUIColorMake(0xFF, 0x8F, 0xBC, 0x8F)
/** @brief 0xFF483D8B */
#define ZFUIColor_webDarkSlateBlue ZFUIColorMake(0xFF, 0x48, 0x3D, 0x8B)
/** @brief 0xFF2F4F4F */
#define ZFUIColor_webDarkSlateGray ZFUIColorMake(0xFF, 0x2F, 0x4F, 0x4F)
/** @brief 0xFF00CED1 */
#define ZFUIColor_webDarkTurquoise ZFUIColorMake(0xFF, 0x00, 0xCE, 0xD1)
/** @brief 0xFF9400D3 */
#define ZFUIColor_webDarkViolet ZFUIColorMake(0xFF, 0x94, 0x00, 0xD3)
/** @brief 0xFFFF1493 */
#define ZFUIColor_webDeepPink ZFUIColorMake(0xFF, 0xFF, 0x14, 0x93)
/** @brief 0xFF00BFFF */
#define ZFUIColor_webDeepSkyBlue ZFUIColorMake(0xFF, 0x00, 0xBF, 0xFF)
/** @brief 0xFF696969 */
#define ZFUIColor_webDimGray ZFUIColorMake(0xFF, 0x69, 0x69, 0x69)
/** @brief 0xFF1E90FF */
#define ZFUIColor_webDodgerBlue ZFUIColorMake(0xFF, 0x1E, 0x90, 0xFF)
/** @brief 0xFFB22222 */
#define ZFUIColor_webFireBrick ZFUIColorMake(0xFF, 0xB2, 0x22, 0x22)
/** @brief 0xFFFFFAF0 */
#define ZFUIColor_webFloralWhite ZFUIColorMake(0xFF, 0xFF, 0xFA, 0xF0)
/** @brief 0xFF228B22 */
#define ZFUIColor_webForestGreen ZFUIColorMake(0xFF, 0x22, 0x8B, 0x22)
/** @brief 0xFFFF00FF */
#define ZFUIColor_webFuchsia ZFUIColorMake(0xFF, 0xFF, 0x00, 0xFF)
/** @brief 0xFFDCDCDC */
#define ZFUIColor_webGainsboro ZFUIColorMake(0xFF, 0xDC, 0xDC, 0xDC)
/** @brief 0xFFF8F8FF */
#define ZFUIColor_webGhostWhite ZFUIColorMake(0xFF, 0xF8, 0xF8, 0xFF)
/** @brief 0xFFFFD700 */
#define ZFUIColor_webGold ZFUIColorMake(0xFF, 0xFF, 0xD7, 0x00)
/** @brief 0xFFDAA520 */
#define ZFUIColor_webGoldenRod ZFUIColorMake(0xFF, 0xDA, 0xA5, 0x20)
/** @brief 0xFF808080 */
#define ZFUIColor_webGray ZFUIColorMake(0xFF, 0x80, 0x80, 0x80)
/** @brief 0xFF008000 */
#define ZFUIColor_webGreen ZFUIColorMake(0xFF, 0x00, 0x80, 0x00)
/** @brief 0xFFADFF2F */
#define ZFUIColor_webGreenYellow ZFUIColorMake(0xFF, 0xAD, 0xFF, 0x2F)
/** @brief 0xFFF0FFF0 */
#define ZFUIColor_webHoneyDew ZFUIColorMake(0xFF, 0xF0, 0xFF, 0xF0)
/** @brief 0xFFFF69B4 */
#define ZFUIColor_webHotPink ZFUIColorMake(0xFF, 0xFF, 0x69, 0xB4)
/** @brief 0xFFCD5C5C */
#define ZFUIColor_webIndianRed ZFUIColorMake(0xFF, 0xCD, 0x5C, 0x5C)
/** @brief 0xFF4B0082 */
#define ZFUIColor_webIndigo ZFUIColorMake(0xFF, 0x4B, 0x00, 0x82)
/** @brief 0xFFFFFFF0 */
#define ZFUIColor_webIvory ZFUIColorMake(0xFF, 0xFF, 0xFF, 0xF0)
/** @brief 0xFFF0E68C */
#define ZFUIColor_webKhaki ZFUIColorMake(0xFF, 0xF0, 0xE6, 0x8C)
/** @brief 0xFFE6E6FA */
#define ZFUIColor_webLavender ZFUIColorMake(0xFF, 0xE6, 0xE6, 0xFA)
/** @brief 0xFFFFF0F5 */
#define ZFUIColor_webLavenderBlush ZFUIColorMake(0xFF, 0xFF, 0xF0, 0xF5)
/** @brief 0xFF7CFC00 */
#define ZFUIColor_webLawnGreen ZFUIColorMake(0xFF, 0x7C, 0xFC, 0x00)
/** @brief 0xFFFFFACD */
#define ZFUIColor_webLemonChiffon ZFUIColorMake(0xFF, 0xFF, 0xFA, 0xCD)
/** @brief 0xFFADD8E6 */
#define ZFUIColor_webLightBlue ZFUIColorMake(0xFF, 0xAD, 0xD8, 0xE6)
/** @brief 0xFFF08080 */
#define ZFUIColor_webLightCoral ZFUIColorMake(0xFF, 0xF0, 0x80, 0x80)
/** @brief 0xFFE0FFFF */
#define ZFUIColor_webLightCyan ZFUIColorMake(0xFF, 0xE0, 0xFF, 0xFF)
/** @brief 0xFFFAFAD2 */
#define ZFUIColor_webLightGoldenRodYellow ZFUIColorMake(0xFF, 0xFA, 0xFA, 0xD2)
/** @brief 0xFFD3D3D3 */
#define ZFUIColor_webLightGray ZFUIColorMake(0xFF, 0xD3, 0xD3, 0xD3)
/** @brief 0xFF90EE90 */
#define ZFUIColor_webLightGreen ZFUIColorMake(0xFF, 0x90, 0xEE, 0x90)
/** @brief 0xFFFFB6C1 */
#define ZFUIColor_webLightPink ZFUIColorMake(0xFF, 0xFF, 0xB6, 0xC1)
/** @brief 0xFFFFA07A */
#define ZFUIColor_webLightSalmon ZFUIColorMake(0xFF, 0xFF, 0xA0, 0x7A)
/** @brief 0xFF20B2AA */
#define ZFUIColor_webLightSeaGreen ZFUIColorMake(0xFF, 0x20, 0xB2, 0xAA)
/** @brief 0xFF87CEFA */
#define ZFUIColor_webLightSkyBlue ZFUIColorMake(0xFF, 0x87, 0xCE, 0xFA)
/** @brief 0xFF778899 */
#define ZFUIColor_webLightSlateGray ZFUIColorMake(0xFF, 0x77, 0x88, 0x99)
/** @brief 0xFFB0C4DE */
#define ZFUIColor_webLightSteelBlue ZFUIColorMake(0xFF, 0xB0, 0xC4, 0xDE)
/** @brief 0xFFFFFFE0 */
#define ZFUIColor_webLightYellow ZFUIColorMake(0xFF, 0xFF, 0xFF, 0xE0)
/** @brief 0xFF00FF00 */
#define ZFUIColor_webLime ZFUIColorMake(0xFF, 0x00, 0xFF, 0x00)
/** @brief 0xFF32CD32 */
#define ZFUIColor_webLimeGreen ZFUIColorMake(0xFF, 0x32, 0xCD, 0x32)
/** @brief 0xFFFAF0E6 */
#define ZFUIColor_webLinen ZFUIColorMake(0xFF, 0xFA, 0xF0, 0xE6)
/** @brief 0xFFFF00FF */
#define ZFUIColor_webMagenta ZFUIColorMake(0xFF, 0xFF, 0x00, 0xFF)
/** @brief 0xFF800000 */
#define ZFUIColor_webMaroon ZFUIColorMake(0xFF, 0x80, 0x00, 0x00)
/** @brief 0xFF66CDAA */
#define ZFUIColor_webMediumAquaMarine ZFUIColorMake(0xFF, 0x66, 0xCD, 0xAA)
/** @brief 0xFF0000CD */
#define ZFUIColor_webMediumBlue ZFUIColorMake(0xFF, 0x00, 0x00, 0xCD)
/** @brief 0xFFBA55D3 */
#define ZFUIColor_webMediumOrchid ZFUIColorMake(0xFF, 0xBA, 0x55, 0xD3)
/** @brief 0xFF9370DB */
#define ZFUIColor_webMediumPurple ZFUIColorMake(0xFF, 0x93, 0x70, 0xDB)
/** @brief 0xFF3CB371 */
#define ZFUIColor_webMediumSeaGreen ZFUIColorMake(0xFF, 0x3C, 0xB3, 0x71)
/** @brief 0xFF7B68EE */
#define ZFUIColor_webMediumSlateBlue ZFUIColorMake(0xFF, 0x7B, 0x68, 0xEE)
/** @brief 0xFF00FA9A */
#define ZFUIColor_webMediumSpringGreen ZFUIColorMake(0xFF, 0x00, 0xFA, 0x9A)
/** @brief 0xFF48D1CC */
#define ZFUIColor_webMediumTurquoise ZFUIColorMake(0xFF, 0x48, 0xD1, 0xCC)
/** @brief 0xFFC71585 */
#define ZFUIColor_webMediumVioletRed ZFUIColorMake(0xFF, 0xC7, 0x15, 0x85)
/** @brief 0xFF191970 */
#define ZFUIColor_webMidnightBlue ZFUIColorMake(0xFF, 0x19, 0x19, 0x70)
/** @brief 0xFFF5FFFA */
#define ZFUIColor_webMintCream ZFUIColorMake(0xFF, 0xF5, 0xFF, 0xFA)
/** @brief 0xFFFFE4E1 */
#define ZFUIColor_webMistyRose ZFUIColorMake(0xFF, 0xFF, 0xE4, 0xE1)
/** @brief 0xFFFFE4B5 */
#define ZFUIColor_webMoccasin ZFUIColorMake(0xFF, 0xFF, 0xE4, 0xB5)
/** @brief 0xFFFFDEAD */
#define ZFUIColor_webNavajoWhite ZFUIColorMake(0xFF, 0xFF, 0xDE, 0xAD)
/** @brief 0xFF000080 */
#define ZFUIColor_webNavy ZFUIColorMake(0xFF, 0x00, 0x00, 0x80)
/** @brief 0xFFFDF5E6 */
#define ZFUIColor_webOldLace ZFUIColorMake(0xFF, 0xFD, 0xF5, 0xE6)
/** @brief 0xFF808000 */
#define ZFUIColor_webOlive ZFUIColorMake(0xFF, 0x80, 0x80, 0x00)
/** @brief 0xFF6B8E23 */
#define ZFUIColor_webOliveDrab ZFUIColorMake(0xFF, 0x6B, 0x8E, 0x23)
/** @brief 0xFFFFA500 */
#define ZFUIColor_webOrange ZFUIColorMake(0xFF, 0xFF, 0xA5, 0x00)
/** @brief 0xFFFF4500 */
#define ZFUIColor_webOrangeRed ZFUIColorMake(0xFF, 0xFF, 0x45, 0x00)
/** @brief 0xFFDA70D6 */
#define ZFUIColor_webOrchid ZFUIColorMake(0xFF, 0xDA, 0x70, 0xD6)
/** @brief 0xFFEEE8AA */
#define ZFUIColor_webPaleGoldenRod ZFUIColorMake(0xFF, 0xEE, 0xE8, 0xAA)
/** @brief 0xFF98FB98 */
#define ZFUIColor_webPaleGreen ZFUIColorMake(0xFF, 0x98, 0xFB, 0x98)
/** @brief 0xFFAFEEEE */
#define ZFUIColor_webPaleTurquoise ZFUIColorMake(0xFF, 0xAF, 0xEE, 0xEE)
/** @brief 0xFFDB7093 */
#define ZFUIColor_webPaleVioletRed ZFUIColorMake(0xFF, 0xDB, 0x70, 0x93)
/** @brief 0xFFFFEFD5 */
#define ZFUIColor_webPapayaWhip ZFUIColorMake(0xFF, 0xFF, 0xEF, 0xD5)
/** @brief 0xFFFFDAB9 */
#define ZFUIColor_webPeachPuff ZFUIColorMake(0xFF, 0xFF, 0xDA, 0xB9)
/** @brief 0xFFCD853F */
#define ZFUIColor_webPeru ZFUIColorMake(0xFF, 0xCD, 0x85, 0x3F)
/** @brief 0xFFFFC0CB */
#define ZFUIColor_webPink ZFUIColorMake(0xFF, 0xFF, 0xC0, 0xCB)
/** @brief 0xFFDDA0DD */
#define ZFUIColor_webPlum ZFUIColorMake(0xFF, 0xDD, 0xA0, 0xDD)
/** @brief 0xFFB0E0E6 */
#define ZFUIColor_webPowderBlue ZFUIColorMake(0xFF, 0xB0, 0xE0, 0xE6)
/** @brief 0xFF800080 */
#define ZFUIColor_webPurple ZFUIColorMake(0xFF, 0x80, 0x00, 0x80)
/** @brief 0xFFFF0000 */
#define ZFUIColor_webRed ZFUIColorMake(0xFF, 0xFF, 0x00, 0x00)
/** @brief 0xFFBC8F8F */
#define ZFUIColor_webRosyBrown ZFUIColorMake(0xFF, 0xBC, 0x8F, 0x8F)
/** @brief 0xFF4169E1 */
#define ZFUIColor_webRoyalBlue ZFUIColorMake(0xFF, 0x41, 0x69, 0xE1)
/** @brief 0xFF8B4513 */
#define ZFUIColor_webSaddleBrown ZFUIColorMake(0xFF, 0x8B, 0x45, 0x13)
/** @brief 0xFFFA8072 */
#define ZFUIColor_webSalmon ZFUIColorMake(0xFF, 0xFA, 0x80, 0x72)
/** @brief 0xFFF4A460 */
#define ZFUIColor_webSandyBrown ZFUIColorMake(0xFF, 0xF4, 0xA4, 0x60)
/** @brief 0xFF2E8B57 */
#define ZFUIColor_webSeaGreen ZFUIColorMake(0xFF, 0x2E, 0x8B, 0x57)
/** @brief 0xFFFFF5EE */
#define ZFUIColor_webSeaShell ZFUIColorMake(0xFF, 0xFF, 0xF5, 0xEE)
/** @brief 0xFFA0522D */
#define ZFUIColor_webSienna ZFUIColorMake(0xFF, 0xA0, 0x52, 0x2D)
/** @brief 0xFFC0C0C0 */
#define ZFUIColor_webSilver ZFUIColorMake(0xFF, 0xC0, 0xC0, 0xC0)
/** @brief 0xFF87CEEB */
#define ZFUIColor_webSkyBlue ZFUIColorMake(0xFF, 0x87, 0xCE, 0xEB)
/** @brief 0xFF6A5ACD */
#define ZFUIColor_webSlateBlue ZFUIColorMake(0xFF, 0x6A, 0x5A, 0xCD)
/** @brief 0xFF708090 */
#define ZFUIColor_webSlateGray ZFUIColorMake(0xFF, 0x70, 0x80, 0x90)
/** @brief 0xFFFFFAFA */
#define ZFUIColor_webSnow ZFUIColorMake(0xFF, 0xFF, 0xFA, 0xFA)
/** @brief 0xFF00FF7F */
#define ZFUIColor_webSpringGreen ZFUIColorMake(0xFF, 0x00, 0xFF, 0x7F)
/** @brief 0xFF4682B4 */
#define ZFUIColor_webSteelBlue ZFUIColorMake(0xFF, 0x46, 0x82, 0xB4)
/** @brief 0xFFD2B48C */
#define ZFUIColor_webTan ZFUIColorMake(0xFF, 0xD2, 0xB4, 0x8C)
/** @brief 0xFF008080 */
#define ZFUIColor_webTeal ZFUIColorMake(0xFF, 0x00, 0x80, 0x80)
/** @brief 0xFFD8BFD8 */
#define ZFUIColor_webThistle ZFUIColorMake(0xFF, 0xD8, 0xBF, 0xD8)
/** @brief 0xFFFF6347 */
#define ZFUIColor_webTomato ZFUIColorMake(0xFF, 0xFF, 0x63, 0x47)
/** @brief 0xFF40E0D0 */
#define ZFUIColor_webTurquoise ZFUIColorMake(0xFF, 0x40, 0xE0, 0xD0)
/** @brief 0xFFEE82EE */
#define ZFUIColor_webViolet ZFUIColorMake(0xFF, 0xEE, 0x82, 0xEE)
/** @brief 0xFFF5DEB3 */
#define ZFUIColor_webWheat ZFUIColorMake(0xFF, 0xF5, 0xDE, 0xB3)
/** @brief 0xFFFFFFFF */
#define ZFUIColor_webWhite ZFUIColorMake(0xFF, 0xFF, 0xFF, 0xFF)
/** @brief 0xFFF5F5F5 */
#define ZFUIColor_webWhiteSmoke ZFUIColorMake(0xFF, 0xF5, 0xF5, 0xF5)
/** @brief 0xFFFFFF00 */
#define ZFUIColor_webYellow ZFUIColorMake(0xFF, 0xFF, 0xFF, 0x00)
/** @brief 0xFF9ACD32 */
#define ZFUIColor_webYellowGreen ZFUIColorMake(0xFF, 0x9A, 0xCD, 0x32)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIColor_common_h_

