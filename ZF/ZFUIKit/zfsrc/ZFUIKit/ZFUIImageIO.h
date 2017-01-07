/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIImageIO.h
 * @brief common image load logic
 */

#ifndef _ZFI_ZFUIImageIO_h_
#define _ZFI_ZFUIImageIO_h_

#include "ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIImageScale
/**
 * @brief scale and return the newly created scaled image
 *
 * if the source image have nine patch,
 * scale would use the nine patch setting\n
 * return null if source image or newSize invalid\n
 * newSize is applied with #ZFUIGlobalStyle::imageScale\n
 * this method would create a new image even if size not changed
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIImageScale(ZF_IN ZFUIImage *image,
                                                 ZF_IN const ZFUISize &newSize,
                                                 ZF_IN_OPT zfbool newSizeInPixelUnit = zffalse);

// ============================================================
// ZFUIImageLoadFromNativeImage
/**
 * @brief create image from native image
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIImageLoadFromNativeImage(ZF_IN void *nativeImage);

// ============================================================
// fileDescriptor
/**
 * @brief see #ZFUIImageSerializeTypeRegister
 *
 * serializable data:
 * @code
 *   <ZFCallback ... />
 * @endcode
 */
#define ZFUIImageSerializeType_input input
/** @brief see #ZFUIImageSerializeTypeRegister */
#define ZFUIImageSerializeTypeName_input ZFM_TOSTRING(ZFUIImageSerializeType_input)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageIO_input zfText("input")

/**
 * @brief load image from input, input should contain the image's binary data
 * @note this method has no cache logic
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIImageLoadFromInput(ZF_IN const ZFInputCallback &input);

// ============================================================
// color
/**
 * @brief see #ZFUIImageSerializeTypeRegister
 *
 * serializable data:
 * @code
 *   <node>
 *       <ZFUIColor category="color" /> // optional, transparent by default
 *       <ZFUISize category="size" /> // optional, (1, 1) by default
 *   </node>
 * @endcode
 */
#define ZFUIImageSerializeType_color color
/** @brief see #ZFUIImageSerializeTypeRegister */
#define ZFUIImageSerializeTypeName_color ZFM_TOSTRING(ZFUIImageSerializeType_color)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageIO_color zfText("color")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageIO_color_size zfText("size")

/**
 * @brief load image from color
 *
 * size is applied with #ZFUIGlobalStyle::imageScale
 * @note this method has no cache logic
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIImageLoadFromColor(ZF_IN const ZFUIColor &color,
                                                         ZF_IN_OPT const ZFUISize &size = ZFUISizeZero);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageIO_h_

