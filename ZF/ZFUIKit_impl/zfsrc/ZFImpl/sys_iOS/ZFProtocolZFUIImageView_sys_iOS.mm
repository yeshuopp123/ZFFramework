/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageView.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUIImageViewImpl_sys_iOS_ImageView : UIImageView
@property (nonatomic, assign) ZFUIImage *ownerZFUIImage;
- (void)updateNinePatch;
@end
@implementation _ZFP_ZFUIImageViewImpl_sys_iOS_ImageView
// ============================================================
// init and dealloc
- (id)init
{
    self = [super init];
    self.autoresizesSubviews = NO;
    self.contentMode = UIViewContentModeScaleToFill;
    return self;
}
// ============================================================
// nine patch logic
- (void)updateNinePatch
{
    if(self.ownerZFUIImage == zfnull)
    {
        self.image = nil;
    }
    else
    {
        UIImage *nativeImage = (__bridge UIImage *)self.ownerZFUIImage->nativeImage();
        UIImage *tmpImage = [UIImage imageWithCGImage:nativeImage.CGImage scale:nativeImage.scale orientation:UIImageOrientationUp];
        UIEdgeInsets ninePatch = UIEdgeInsetsMake(
            self.ownerZFUIImage->imageNinePatch().top,
            self.ownerZFUIImage->imageNinePatch().left,
            self.ownerZFUIImage->imageNinePatch().bottom,
            self.ownerZFUIImage->imageNinePatch().right);
        if(ninePatch.left + ninePatch.right >= tmpImage.size.width)
        {
            if(ninePatch.left + ninePatch.right == tmpImage.size.width)
            {
                --(ninePatch.right);
            }
            else
            {
                ninePatch.left = 0;
                ninePatch.right = 0;
            }
        }
        if(ninePatch.top + ninePatch.bottom >= tmpImage.size.height)
        {
            if(ninePatch.top + ninePatch.bottom == tmpImage.size.height)
            {
                --(ninePatch.bottom);
            }
            else
            {
                ninePatch.left = 0;
                ninePatch.right = 0;
            }
        }
        self.image = [tmpImage resizableImageWithCapInsets:ninePatch resizingMode:UIImageResizingModeStretch];
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageViewImpl_sys_iOS, ZFUIImageView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UIImageView"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, zfText("iOS:UIImage"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void *nativeImageViewCreate(ZF_IN ZFUIImageView *imageView)
    {
        return (__bridge_retained void *)[_ZFP_ZFUIImageViewImpl_sys_iOS_ImageView new];
    }
    virtual void nativeImageViewDestroy(ZF_IN ZFUIImageView *imageView,
                                        ZF_IN void *nativeImageView)
    {
        UIView *tmp = (__bridge_transfer UIView *)nativeImageView;
        tmp = nil;
    }

    virtual void imageContentSet(ZF_IN ZFUIImageView *imageView,
                                 ZF_IN ZFUIImage *image)
    {
        _ZFP_ZFUIImageViewImpl_sys_iOS_ImageView *nativeImplView = (__bridge _ZFP_ZFUIImageViewImpl_sys_iOS_ImageView *)imageView->nativeImplView();
        nativeImplView.ownerZFUIImage = image;
    }
    virtual void imageNinePatchChanged(ZF_IN ZFUIImageView *imageView,
                                       ZF_IN zffloat imageScale,
                                       ZF_IN const ZFUIMargin &imageNinePatch)
    {
        _ZFP_ZFUIImageViewImpl_sys_iOS_ImageView *nativeImplView = (__bridge _ZFP_ZFUIImageViewImpl_sys_iOS_ImageView *)imageView->nativeImplView();
        [nativeImplView updateNinePatch];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageViewImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIImageViewImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

