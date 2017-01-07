/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_iOS
#import <UIKit/UIKit.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageImpl_sys_iOS, ZFUIImage, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UIImage"))
public:
    virtual void *nativeImageFromInput(ZF_IN const ZFInputCallback &inputCallback)
    {
        ZFBuffer dataBuf = ZFInputCallbackReadToBuffer(inputCallback);
        if(dataBuf.buffer() == zfnull)
        {
            return zfnull;
        }
        NSData *nsData = [NSData dataWithBytesNoCopy:dataBuf.buffer() length:(NSUInteger)dataBuf.bufferSize() freeWhenDone:YES];
        if(nsData == nil)
        {
            return zfnull;
        }
        dataBuf.bufferGiveUp(); // should be free-ed by nsData
        UIImage *uiImage = [UIImage imageWithData:nsData scale:ZFUIGlobalStyle::DefaultStyle()->imageScale()];
        if(uiImage == nil)
        {
            return zfnull;
        }

        return (__bridge_retained void *)uiImage;
    }
    virtual zfbool nativeImageToOutput(ZF_IN void *nativeImage,
                                       ZF_OUT const ZFOutputCallback &outputCallback)
    {
        UIImage *uiImage = (__bridge UIImage *)nativeImage;
        NSData *nsData = UIImagePNGRepresentation(uiImage);
        if(nsData == nil)
        {
            return zffalse;
        }

        const zfbyte *buf = ZFCastStatic(const zfbyte *, nsData.bytes);
        zfindex size = nsData.length;
        static const zfindex blockSize = 10240;
        const zfbyte *bufEnd = buf + size;
        const zfbyte *bufEndTmp = buf + size - blockSize;
        while(buf < bufEndTmp)
        {
            if(outputCallback.execute(buf, blockSize) != blockSize)
            {
                return zffalse;
            }
            buf += blockSize;
        }
        if(buf < bufEnd)
        {
            zfindex tmpSize = bufEnd - buf;
            if(outputCallback.execute(buf, tmpSize) != tmpSize)
            {
                return zffalse;
            }
        }

        return zftrue;
    }

    virtual void *nativeImageCopy(ZF_IN void *nativeImage)
    {
        UIImage *uiImage = (__bridge UIImage *)nativeImage;
        CGImageRef cgImageNew = CGImageCreateCopy(uiImage.CGImage);
        UIImage *uiImageNew = [UIImage imageWithCGImage:cgImageNew scale:uiImage.scale orientation:uiImage.imageOrientation];
        CGImageRelease(cgImageNew);
        return (__bridge_retained void *)uiImageNew;
    }

    virtual void *nativeImageRetain(ZF_IN void *nativeImage)
    {
        UIImage *tmp = (__bridge UIImage *)nativeImage;
        return (__bridge_retained void *)tmp;
    }
    virtual void nativeImageRelease(ZF_IN void *nativeImage)
    {
        UIImage *tmp = (__bridge_transfer UIImage *)nativeImage;
        tmp = nil;
    }

    virtual ZFUISize nativeImageSize(ZF_IN void *nativeImage)
    {
        UIImage *uiImage = (__bridge UIImage *)nativeImage;
        return ZFUISizeMake(zfmRound(uiImage.size.width * uiImage.scale), zfmRound(uiImage.size.height * uiImage.scale));
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIImageImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

