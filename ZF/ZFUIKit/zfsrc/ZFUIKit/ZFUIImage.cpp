/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIImage.h"
#include "protocol/ZFProtocolZFUIImage.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// serializabel data
zfclassPOD _ZFP_ZFUIImageSerializeTypeData
{
public:
    ZFUIImageSerializeFromCallback fromCallback;
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageSerializeTypeHolder, ZFLevelZFFrameworkNormal)
{
}
public:
    ZFCoreMap types; // _ZFP_ZFUIImageSerializeTypeData
ZF_GLOBAL_INITIALIZER_END(ZFUIImageSerializeTypeHolder)
void ZFUIImageSerializeTypeRegister(ZF_IN const zfchar *name,
                                    ZF_IN ZFUIImageSerializeFromCallback fromCallback)
{
    if(name != zfnull && fromCallback != zfnull)
    {
        _ZFP_ZFUIImageSerializeTypeData *typeData = (_ZFP_ZFUIImageSerializeTypeData *)zfmalloc(sizeof(_ZFP_ZFUIImageSerializeTypeData));
        typeData->fromCallback = fromCallback;
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageSerializeTypeHolder)->types.set(name,
            ZFCorePointerForPOD<_ZFP_ZFUIImageSerializeTypeData *>(typeData));
    }
}
void ZFUIImageSerializeTypeUnregister(ZF_IN const zfchar *name)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageSerializeTypeHolder)->types.remove(name);
}

void ZFUIImageSerializeTypeGetAll(ZF_OUT ZFCoreArray<const zfchar *> &ret)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageSerializeTypeHolder)->types.allKey(ret);
}

// ============================================================
// _ZFP_ZFUIImagePrivate
zfclassNotPOD _ZFP_ZFUIImagePrivate
{
public:
    ZFUIImage *pimplOwner;
    void *nativeImage;
    ZFUISize imageSizeFixed;
    ZFUISize imageSize;

public:
    zfchar *serializableType;
    const ZFSerializableData *serializableData;

public:
    void imageSizeUpdate(void)
    {
        if(this->nativeImage != zfnull)
        {
            this->imageSizeFixed = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageSize(this->nativeImage);
            this->imageSize = ZFUISizeApplyScaleReversely(this->imageSizeFixed, this->pimplOwner->imageScaleFixed());
        }
        else
        {
            this->imageSizeFixed = ZFUISizeZero;
            this->imageSize = ZFUISizeZero;
        }
    }
    void copyFrom(ZF_IN _ZFP_ZFUIImagePrivate *another)
    {
        if(this->nativeImage != zfnull)
        {
            ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(this->nativeImage);
            this->nativeImage = zfnull;
        }
        this->imageSizeFixed = ZFUISizeZero;
        this->imageSize = ZFUISizeZero;
        zfsChange(this->serializableType, (const zfchar *)zfnull);
        if(this->serializableData != zfnull)
        {
            zfdelete(this->serializableData);
            this->serializableData = zfnull;
        }

        if(another->nativeImage != zfnull)
        {
            this->nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRetain(another->nativeImage);
        }
        this->imageSizeUpdate();
        zfsChange(this->serializableType, another->serializableType);
        if(another->serializableData != zfnull)
        {
            this->serializableData = zfnew(ZFSerializableData, *(another->serializableData));
        }
    }

public:
    _ZFP_ZFUIImagePrivate(void)
    : pimplOwner(zfnull)
    , nativeImage(zfnull)
    , imageSizeFixed(ZFUISizeZero)
    , imageSize(ZFUISizeZero)
    , serializableType(zfnull)
    , serializableData(zfnull)
    {
    }
    ~_ZFP_ZFUIImagePrivate(void)
    {
        zffree(this->serializableType);
        zfdelete(this->serializableData);
    }
};

// ============================================================
// ZFUIImage
ZFOBJECT_REGISTER(ZFUIImage)

zfbool ZFUIImage::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                  ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                  ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHintToAppend, outErrorPos)) {return zffalse;}

    // imageScale
    zffloat imageScale = 0;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHintToAppend, outErrorPos,
        check, ZFSerializableKeyword_ZFUIImage_imageScale, zffloat, imageScale);
    this->imageScaleSet(imageScale);

    // imageBin
    const zfchar *imageBin = zfnull;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHintToAppend, outErrorPos,
        check, ZFSerializableKeyword_ZFUIImage_imageBin, zfstring, imageBin);
    if(imageBin != zfnull)
    {
        if(!ZFUIImageEncodeFromBase64(this, ZFInputCallbackForBuffer(imageBin)))
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                zfText("fail to load image from base64 data: \"%s\""), imageBin);
            return zffalse;
        }
        return zftrue;
    }

    // imageType
    const zfchar *typeName = zfnull;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHintToAppend, outErrorPos,
        require, ZFSerializableKeyword_ZFUIImage_imageType, zfstring, typeName);
    _ZFP_ZFUIImageSerializeTypeData *typeData = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageSerializeTypeHolder)->types.get<_ZFP_ZFUIImageSerializeTypeData *>(typeName);
    if(typeData == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("no such image serializable type registered: \"%s\""), typeName);
        return zffalse;
    }

    // imageData
    ZFSerializableData imageData;
    {
        const ZFSerializableData *categoryData = ZFSerializableUtil::requireElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImage_imageData);
        if(categoryData == zfnull)
        {
            return zffalse;
        }
        if(!typeData->fromCallback(this, *categoryData, outErrorHintToAppend, outErrorPos))
        {
            return zffalse;
        }

        imageData = categoryData->copy();
        imageData.categorySet(zfnull);

        categoryData->resolveMark();
    }

    // check
    if(d->nativeImage == zfnull)
    {
        d->imageSizeFixed = ZFUISizeZero;
        d->imageSize = ZFUISizeZero;
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData, zfText("nativeImage not set"));
        return zffalse;
    }

    this->imageSerializableTypeSet(typeName);
    this->imageSerializableDataSet(&imageData);

    return zftrue;
}
zfbool ZFUIImage::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHintToAppend)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    // check
    if(d->nativeImage == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, zfText("serialize an image whose nativeImage not set"));
        return zffalse;
    }

    if(this->imageSerializableType() != zfnull)
    { // custom serialize
        if(ref != zfnull
            && zfscmpTheSame(this->imageSerializableType(), ref->imageSerializableType())
            && this->imageSerializableData() != zfnull && ref->imageSerializableData() != zfnull
            && this->imageSerializableData()->objectCompare(*(ref->imageSerializableData())))
        {
            // all match, skip
        }
        else
        {
            // imageType
            ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHintToAppend, ref,
                ZFSerializableKeyword_ZFUIImage_imageType, zfstring, this->imageSerializableType(), ref->imageSerializableType(), zfText(""));

            { // data
                if(this->imageSerializableData() == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
                        zfText("missing image data for type: \"%s\""), this->imageSerializableType());
                    return zffalse;
                }

                ZFSerializableData categoryData = this->imageSerializableData()->copy();
                categoryData.categorySet(ZFSerializableKeyword_ZFUIImage_imageData);
                if(this->imageSerializableData()->referenceInfoExistRecursively())
                {
                    categoryData.referenceInfoRestore(*(this->imageSerializableData()));
                }
                serializableData.elementAdd(categoryData);
            }
        }
    }
    else
    { // imageBin
        zfstring imageBin;
        if(!ZFUIImageEncodeToBase64(ZFOutputCallbackForString(imageBin), this))
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, zfText("save image to base64 failed"));
            return zffalse;
        }
        zfstring imageBinRef;
        if(ref != zfnull)
        {
            ZFUIImageEncodeToBase64(ZFOutputCallbackForString(imageBinRef), ref);
        }
        ZFSerializableUtilSerializeCategoryToData(serializableData, outErrorHintToAppend, ref,
            ZFSerializableKeyword_ZFUIImage_imageBin, zfstring, imageBin, imageBinRef, zfstring());
    }

    // imageScale
    ZFSerializableUtilSerializeCategoryToData(serializableData, outErrorHintToAppend, ref,
        ZFSerializableKeyword_ZFUIImage_imageScale, zffloat, this->imageScale(), ref->imageScale(), 0);

    return zftrue;
}

void ZFUIImage::copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
{
    zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);

    d->copyFrom(ZFCastZFObjectUnchecked(zfself *, anotherObj)->d);
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIImage, zffloat, imageScale)
{
    this->imageScaleSetInternal(newValue);
    d->imageSizeUpdate();
    this->imageScaleOnChange();
}
void ZFUIImage::_ZFP_ZFUIImage_imageScaleOnChange(void)
{
    d->imageSizeUpdate();
    this->imageScaleOnChange();
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageScaleChangeListenerHolder, ZFLevelZFFrameworkHigh)
{
    this->globalImageScaleOnChangeListener = ZFCallbackForRawFunction(zfself::globalImageScaleOnChange);
}
ZFListener globalImageScaleOnChangeListener;
static ZFLISTENER_PROTOTYPE_EXPAND(globalImageScaleOnChange)
{
    const ZFProperty *property = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFProperty *>();
    if(property == ZFPropertyAccess(ZFUIGlobalStyle, imageScale))
    {
        ZFUIImage *image = userData->to<ZFObjectHolder *>()->holdedObj;
        image->_ZFP_ZFUIImage_imageScaleOnChange();
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIImageScaleChangeListenerHolder)

const ZFUISize &ZFUIImage::imageSize(void)
{
    return d->imageSize;
}
const ZFUISize &ZFUIImage::imageSizeFixed(void)
{
    return d->imageSizeFixed;
}

ZFObject *ZFUIImage::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIImagePrivate);
    d->pimplOwner = this;
    return this;
}
void ZFUIImage::objectOnDealloc(void)
{
    if(d != zfnull)
    {
        if(d->nativeImage != zfnull)
        {
            ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(d->nativeImage);
            d->nativeImage = zfnull;
        }
        zfpoolDelete(d);
        d = zfnull;
    }
    zfsuper::objectOnDealloc();
}
void ZFUIImage::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    ZFUIGlobalStyle::DefaultStyle()->observerAdd(
        ZFObject::EventObjectPropertyValueAfterChange(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageScaleChangeListenerHolder)->globalImageScaleOnChangeListener,
        this->objectHolder());
}
void ZFUIImage::objectOnDeallocPrepare(void)
{
    ZFUIGlobalStyle::DefaultStyle()->observerRemove(
        ZFObject::EventObjectPropertyValueAfterChange(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageScaleChangeListenerHolder)->globalImageScaleOnChangeListener);
    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFUIImage::objectHash(void)
{
    return zfidentityCalcPointer(d->nativeImage);
}
ZFCompareResult ZFUIImage::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(d->nativeImage == another->d->nativeImage
        && this->imageNinePatch() == another->imageNinePatch()
        && zfscmpTheSame(d->serializableType, another->d->serializableType)
        && ((d->serializableData == zfnull && another->d->serializableData == zfnull)
            || (d->serializableData != zfnull && another->d->serializableData != zfnull
                && d->serializableData->objectCompare(*(another->d->serializableData)) == ZFCompareTheSame)))
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}
void ZFUIImage::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    ret += this->classData()->className();
    ret += zfText(" ");
    zfsFromPointerT(ret, this);
    ret += zfText(" ");
    ZFUISizeToString(ret, this->imageSize());
}

void *ZFUIImage::nativeImage(void)
{
    return d->nativeImage;
}

void ZFUIImage::nativeImageSet(ZF_IN void *nativeImage)
{
    void *toRelease = d->nativeImage;

    if(nativeImage != zfnull)
    {
        d->nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRetain(nativeImage);
    }
    else
    {
        d->nativeImage = zfnull;
    }
    d->imageSizeUpdate();

    if(toRelease != zfnull)
    {
        ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(toRelease);
    }
}

void ZFUIImage::imageSerializableTypeSet(ZF_IN const zfchar *typeName)
{
    zfsChange(d->serializableType, typeName);
}
const zfchar *ZFUIImage::imageSerializableType(void)
{
    return d->serializableType;
}
void ZFUIImage::imageSerializableDataSet(ZF_IN const ZFSerializableData *serializableData)
{
    if(d->serializableData != zfnull)
    {
        zfdelete(d->serializableData);
        d->serializableData = zfnull;
    }
    if(serializableData != zfnull)
    {
        d->serializableData = zfnew(ZFSerializableData, *serializableData);
    }
}
const ZFSerializableData *ZFUIImage::imageSerializableData(void)
{
    return d->serializableData;
}

// ============================================================
zfbool ZFUIImageEncodeFromBase64(ZF_IN_OUT ZFUIImage *image,
                                 ZF_IN const ZFInputCallback &inputCallback)
{
    ZFIOBridgeCallbackUsingTmpFile io;
    if(image != zfnull && ZFBase64Decode(io.outputCallback(), inputCallback))
    {
        void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(io.inputCallback());
        if(nativeImage != zfnull)
        {
            image->imageSerializableTypeSet(zfnull);
            image->imageSerializableDataSet(zfnull);
            image->nativeImageSet(nativeImage);
            ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);
            return zftrue;
        }
    }
    return zffalse;
}
zfautoObject ZFUIImageEncodeFromBase64(ZF_IN const ZFInputCallback &inputCallback)
{
    zfautoObject ret = ZFUIImage::ClassData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    if(ZFUIImageEncodeFromBase64(ret, inputCallback))
    {
        return ret;
    }
    else
    {
        return zfautoObjectNull;
    }
}
zfbool ZFUIImageEncodeToBase64(ZF_OUT const ZFOutputCallback &outputCallback,
                               ZF_IN ZFUIImage *image)
{
    if(image != zfnull && image->nativeImage() != zfnull && outputCallback.callbackIsValid())
    {
        ZFIOBridgeCallbackUsingTmpFile io;
        if(!ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageToOutput(image->nativeImage(), io.outputCallback()))
        {
            return zffalse;
        }
        return ZFBase64Encode(outputCallback, io.inputCallback());
    }
    return zffalse;
}
zfbool ZFUIImageEncodeFromBinary(ZF_IN_OUT ZFUIImage *image,
                                 ZF_IN const ZFInputCallback &inputCallback)
{
    if(image != zfnull && inputCallback.callbackIsValid())
    {
        void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(inputCallback);
        if(nativeImage != zfnull)
        {
            image->imageSerializableTypeSet(zfnull);
            image->imageSerializableDataSet(zfnull);
            image->nativeImageSet(nativeImage);
            ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);
            return zftrue;
        }
    }
    return zffalse;
}
zfautoObject ZFUIImageEncodeFromBinary(ZF_IN const ZFInputCallback &inputCallback)
{
    zfautoObject ret = ZFUIImage::ClassData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    if(ZFUIImageEncodeFromBinary(ret, inputCallback))
    {
        return ret;
    }
    else
    {
        return zfautoObjectNull;
    }
}
zfbool ZFUIImageEncodeToBinary(ZF_OUT const ZFOutputCallback &outputCallback,
                               ZF_IN ZFUIImage *image)
{
    if(image != zfnull && image->nativeImage() != zfnull && outputCallback.callbackIsValid())
    {
        return ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageToOutput(image->nativeImage(), outputCallback);
    }
    return zffalse;
}

ZF_NAMESPACE_GLOBAL_END

