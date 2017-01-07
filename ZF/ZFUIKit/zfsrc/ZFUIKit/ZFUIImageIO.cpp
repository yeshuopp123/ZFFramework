/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIImageIO.h"
#include "protocol/ZFProtocolZFUIImage.h"
#include "protocol/ZFProtocolZFUIImageIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIImageScale
zfautoObject ZFUIImageScale(ZF_IN ZFUIImage *image,
                            ZF_IN const ZFUISize &newSize,
                            ZF_IN_OPT zfbool newSizeInPixelUnit /* = zffalse */)
{
    if(image == zfnull || image->nativeImage() == zfnull)
    {
        return zfautoObjectNull;
    }
    zffloat scale = image->imageScaleFixed();
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageApplyScale(
        scale,
        image->nativeImage(),
        newSizeInPixelUnit ? newSize : ZFUISizeApplyScale(newSize, scale),
        ZFUIMarginApplyScale(image->imageNinePatch(), scale));
    if(nativeImage == zfnull)
    {
        return zfautoObjectNull;
    }
    zfautoObject ret = ZFUIImage::ClassData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    ret.to<ZFUIImage *>()->nativeImageSet(nativeImage);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);
    return ret;
}

// ============================================================
// ZFUIImageLoadFromNativeImage
zfautoObject ZFUIImageLoadFromNativeImage(ZF_IN void *nativeImage)
{
    if(nativeImage == zfnull)
    {
        return zfautoObjectNull;
    }
    zfautoObject ret = ZFUIImage::ClassData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    ret.to<ZFUIImage *>()->nativeImageSet(nativeImage);
    return ret;
}

// ============================================================
// input
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(ZFUIImageSerializeType_input)
{
    ZFCallback input;
    if(!ZFCallbackFromSerializableData(input, serializableData, outErrorHintToAppend, outErrorPos))
    {
        return zffalse;
    }
    if(!input.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("invalid callback"));
        return zffalse;
    }
    if(!ZFUIImageEncodeFromBinary(result, input))
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("load image failed"));
        return zffalse;
    }
    return zftrue;
}

zfautoObject ZFUIImageLoadFromInput(ZF_IN const ZFInputCallback &input)
{
    zfautoObject ret = ZFUIImage::ClassData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    ZFUIImage *image = ret;
    if(!ZFUIImageEncodeFromBinary(image, input))
    {
        return zfautoObjectNull;
    }

    if(input.callbackSerializeCustomType() != zfnull)
    {
        ZFSerializableData inputData;
        if(ZFCallbackToSerializableData(inputData, input))
        {
            image->imageSerializableTypeSet(ZFUIImageSerializeTypeName_input);
            image->imageSerializableDataSet(&inputData);
        }
    }

    return ret;
}

// ============================================================
// color
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(ZFUIImageSerializeType_color)
{
    ZFUIColor color = ZFUIColorTransparent;
    { // color
        const ZFSerializableData *categoryData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImageIO_color);
        if(categoryData != zfnull)
        {
            if(!ZFUIColorFromSerializableData(color, *categoryData, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
        }
    }

    ZFUISize size = ZFUISizeMake(1);
    { // size
        const ZFSerializableData *categoryData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImageIO_color_size);
        if(categoryData != zfnull)
        {
            if(!ZFUISizeFromSerializableData(size, *categoryData, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            if(size.width <= 0 || size.height <= 0)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, *categoryData,
                    zfText("invalid size: %s"), ZFUISizeToString(size).cString());
                return zffalse;
            }
        }
    }

    ZFSerializableData imageData;
    {
        if(!ZFUIColorIsEqual(color, ZFUIColorTransparent))
        {
            ZFSerializableData categoryData;
            if(!ZFUIColorToSerializableData(categoryData, color, outErrorHintToAppend))
            {
                return zffalse;
            }
            categoryData.categorySet(ZFSerializableKeyword_ZFUIImageIO_color);
            imageData.elementAdd(categoryData);
        }
        if(!ZFUISizeIsEqual(size, ZFUISizeMake(1, 1)))
        {
            ZFSerializableData categoryData;
            if(!ZFUISizeToSerializableData(categoryData, size, outErrorHintToAppend))
            {
                return zffalse;
            }
            categoryData.categorySet(ZFSerializableKeyword_ZFUIImageIO_color_size);
            imageData.elementAdd(categoryData);
        }
    }

    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadFromColor(ZFUIGlobalStyle::DefaultStyle()->imageScale(), color, size);
    result->nativeImageSet(nativeImage);
    result->imageSerializableDataSet(&imageData);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);

    return zftrue;
}
zfautoObject ZFUIImageLoadFromColor(ZF_IN const ZFUIColor &color,
                                    ZF_IN_OPT const ZFUISize &size /* = ZFUISizeZero */)
{
    ZFUISize sizeTmp = ZFUISizeMake(zfmMax(1, size.width), zfmMax(1, size.height));
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadFromColor(
        ZFUIGlobalStyle::DefaultStyle()->imageScale(),
        color,
        ZFUISizeApplyScale(sizeTmp, ZFUIGlobalStyle::DefaultStyle()->imageScale()));
    if(nativeImage == zfnull)
    {
        return zfautoObjectNull;
    }

    zfautoObject ret = ZFUIImage::ClassData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    ZFUIImage *image = ret;
    image->nativeImageSet(nativeImage);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);

    ZFSerializableData imageData;

    do
    {
        // color
        if(color != ZFUIColorTransparent)
        {
            ZFSerializableData categoryData;
            if(!ZFUIColorToSerializableData(categoryData, color))
            {
                break;
            }
            categoryData.categorySet(ZFSerializableKeyword_ZFUIImageIO_color);
            imageData.elementAdd(categoryData);
        }

        // size
        if(!ZFUISizeIsEqual(sizeTmp, ZFUISizeMake(1, 1)))
        {
            ZFSerializableData categoryData;
            if(!ZFUISizeToSerializableData(categoryData, sizeTmp))
            {
                break;
            }
            categoryData.categorySet(ZFSerializableKeyword_ZFUIImageIO_color_size);
            imageData.elementAdd(categoryData);
        }

        imageData.itemClassSet(ZFSerializableKeyword_node);
    } while(zffalse);

    if(imageData.itemClass() != zfnull)
    {
        image->imageSerializableTypeSet(ZFUIImageSerializeTypeName_color);
        image->imageSerializableDataSet(&imageData);
    }

    return ret;
}

ZF_NAMESPACE_GLOBAL_END

