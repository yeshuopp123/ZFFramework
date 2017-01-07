/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_Qt

#include <QImage>
#include <QByteArray>
#include <QBuffer>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageImpl_sys_Qt, ZFUIImage, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt:QImage"))

public:
    virtual void *nativeImageFromInput(ZF_IN const ZFInputCallback &inputCallback)
    {
        QImage *nativeImage = new QImage();
        ZFBuffer buf = ZFInputCallbackReadToBuffer(inputCallback);
        if(!nativeImage->loadFromData((const uchar *)buf.buffer(), buf.bufferSize()))
        {
            delete nativeImage;
            return zfnull;
        }
        return nativeImage;
    }
    virtual zfbool nativeImageToOutput(ZF_IN void *nativeImage,
                                       ZF_OUT const ZFOutputCallback &outputCallback)
    {
        QImage *nativeImageTmp = ZFCastStatic(QImage *, nativeImage);
        QByteArray buf;
        QBuffer buffer(&buf);
        buffer.open(QIODevice::ReadWrite);
        if(!nativeImageTmp->save(&buffer, "PNG"))
        {
            return zffalse;
        }
        outputCallback.execute(buffer.buffer().constData(), (zfindex)buffer.size());
        return zftrue;
    }

    virtual void *nativeImageCopy(ZF_IN void *nativeImage)
    {
        QImage *nativeImageTmp = ZFCastStatic(QImage *, nativeImage);
        QImage *ret = new QImage();
        *ret = nativeImageTmp->copy();
        return ret;
    }

    virtual void *nativeImageRetain(ZF_IN void *nativeImage)
    {
        QImage *nativeImageTmp = ZFCastStatic(QImage *, nativeImage);
        QImage *ret = new QImage();
        *ret = *nativeImageTmp;
        return ret;
    }
    virtual void nativeImageRelease(ZF_IN void *nativeImage)
    {
        QImage *nativeImageTmp = ZFCastStatic(QImage *, nativeImage);
        delete nativeImageTmp;
    }

    virtual ZFUISize nativeImageSize(ZF_IN void *nativeImage)
    {
        QImage *nativeImageTmp = ZFCastStatic(QImage *, nativeImage);
        return ZFImpl_sys_Qt_ZFUIKit_ZFUISizeFromQSize(nativeImageTmp->size());
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIImageImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

