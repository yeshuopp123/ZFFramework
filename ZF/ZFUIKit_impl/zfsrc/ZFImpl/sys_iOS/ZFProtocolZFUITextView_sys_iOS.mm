/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextView.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUITextViewImpl_sys_iOS_TextView : UILabel
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUITextView) *impl;
@property (nonatomic, assign) ZFUITextView *ownerZFUITextView;
@property (nonatomic, strong) NSString *fontName;
@property (nonatomic, assign) zfint textSize;
@end
@implementation _ZFP_ZFUITextViewImpl_sys_iOS_TextView
// ============================================================
// init and dealloc
- (id)init
{
    self = [super init];

    self.autoresizesSubviews = NO;
    self.backgroundColor = [UIColor clearColor];

    // property init
    self.fontName = [UIFont systemFontOfSize:[UIFont systemFontSize]].fontName;
    self.textColor = [UIColor blackColor];
    self.shadowColor = nil;
    self.numberOfLines = 1;
    self.lineBreakMode = NSLineBreakByClipping;

    return self;
}
- (void)dealloc
{
    self.fontName = nil;
}
- (void)setTextSize:(zfint)newTextSize ZFImpl_sys_iOS_overrideProperty
{
    if(self->_textSize != newTextSize)
    {
        self->_textSize = newTextSize;
        self.font = [UIFont fontWithName:self.fontName size:newTextSize];
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextViewImpl_sys_iOS, ZFUITextView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UILabel"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFString, zfText("iOS:NSString"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void *nativeTextViewCreate(ZF_IN ZFUITextView *textView)
    {
        return (__bridge_retained void *)[_ZFP_ZFUITextViewImpl_sys_iOS_TextView new];
    }
    virtual void nativeTextViewDestroy(ZF_IN ZFUITextView *textView,
                                       ZF_IN void *nativeTextView)
    {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *tmp = (__bridge_transfer _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)nativeTextView;
        tmp = nil;
    }

// ============================================================
// properties
public:
    virtual void textContentSet(ZF_IN ZFUITextView *textView,
                                ZF_IN ZFString *text)
    {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        if(text != zfnull)
        {
            NSString *nativeString = (__bridge NSString *)text->nativeString();
            nativeImplView.text = nativeString;
        }
        else
        {
            nativeImplView.text = nil;
        }
    }
    virtual void textAppearanceSet(ZF_IN ZFUITextView *textView,
                                   ZF_IN ZFUITextAppearanceEnum const &textAppearance)
    {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        switch(textAppearance)
        {
            case ZFUITextAppearance::e_Normal:
                nativeImplView.fontName = [UIFont systemFontOfSize:[UIFont systemFontSize]].fontName;
                break;
            case ZFUITextAppearance::e_Bold:
                nativeImplView.fontName = [UIFont boldSystemFontOfSize:[UIFont systemFontSize]].fontName;
                break;
            case ZFUITextAppearance::e_Italic:
                nativeImplView.fontName = [UIFont italicSystemFontOfSize:[UIFont systemFontSize]].fontName;
                break;
            case ZFUITextAppearance::e_BoldItalic:
                nativeImplView.fontName = @"Helvetica-BoldOblique";
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
    virtual void textAlignSet(ZF_IN ZFUITextView *textView,
                              ZF_IN ZFUIAlignFlags const &textAlign)
    {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        if(ZFBitTest(textAlign, ZFUIAlign::e_LeftInner))
        {
            nativeImplView.textAlignment = NSTextAlignmentLeft;
        }
        else if(ZFBitTest(textAlign, ZFUIAlign::e_RightInner))
        {
            nativeImplView.textAlignment = NSTextAlignmentRight;
        }
        else if(textAlign == ZFUIAlign::e_Center)
        {
            nativeImplView.textAlignment = NSTextAlignmentCenter;
        }
        else
        {
            nativeImplView.textAlignment = NSTextAlignmentLeft;
        }
    }
    virtual void textColorSet(ZF_IN ZFUITextView *textView,
                              ZF_IN ZFUIColor const &textColor)
    {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        nativeImplView.textColor = ZFImpl_sys_iOS_ZFUIKit_ZFUIColorToUIColor(textColor);
    }
    virtual void textShadowColorSet(ZF_IN ZFUITextView *textView,
                                    ZF_IN ZFUIColor const &textShadowColor)
    {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        nativeImplView.shadowColor = ZFImpl_sys_iOS_ZFUIKit_ZFUIColorToUIColor(textShadowColor);
    }
    virtual void textShadowOffsetSet(ZF_IN ZFUITextView *textView,
                                     ZF_IN ZFUISize const &textShadowOffset)
    {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        nativeImplView.shadowOffset = ZFImpl_sys_iOS_ZFUIKit_ZFUISizeToCGSize(textShadowOffset);
    }
    virtual void textSizeSet(ZF_IN ZFUITextView *textView,
                             ZF_IN zfint textSize)
    {
        // changed during layoutNativeTextView
    }
    virtual void textSizeAutoChangeMinSizeSet(ZF_IN ZFUITextView *textView,
                                              ZF_IN zfint textSizeAutoChangeMinSize)
    {
        // changed during layoutNativeTextView
    }
    virtual void textSizeAutoChangeMaxSizeSet(ZF_IN ZFUITextView *textView,
                                              ZF_IN zfint textSizeAutoChangeMaxSize)
    {
        // changed during layoutNativeTextView
    }
    virtual void textSingleLineSet(ZF_IN ZFUITextView *textView,
                                   ZF_IN zfbool textSingleLine)
    {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        nativeImplView.numberOfLines = (textSingleLine ? 1 : 0);
    }
    virtual void textTruncateModeSet(ZF_IN ZFUITextView *textView,
                                     ZF_IN ZFUITextTruncateModeEnum const &textTruncateMode)
    {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        switch(textTruncateMode)
        {
            case ZFUITextTruncateMode::e_Disable:
                nativeImplView.lineBreakMode = NSLineBreakByClipping;
                break;
            case ZFUITextTruncateMode::e_Head:
                nativeImplView.lineBreakMode = NSLineBreakByTruncatingHead;
                break;
            case ZFUITextTruncateMode::e_Middle:
                nativeImplView.lineBreakMode = NSLineBreakByTruncatingMiddle;
                break;
            case ZFUITextTruncateMode::e_Tail:
                nativeImplView.lineBreakMode = NSLineBreakByTruncatingTail;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }

    // ============================================================
    // layout
public:
    virtual ZFUISize measureNativeTextView(ZF_IN ZFUITextView *textView,
                                          ZF_IN const ZFUISize &sizeHint,
                                          ZF_IN zfint textSize)
    {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        zfint textSizeSaved = nativeImplView.textSize;
        nativeImplView.textSize = textSize;
        CGSize sizeTmp = ZFImpl_sys_iOS_ZFUIKit_ZFUISizeToCGSize(sizeHint);
        if(sizeTmp.width <= 0)
        {
            sizeTmp.width = 30000;
        }
        if(sizeTmp.height <= 0)
        {
            sizeTmp.height = 30000;
        }
        sizeTmp = [nativeImplView sizeThatFits:sizeTmp];
        nativeImplView.textSize = textSizeSaved;
        return ZFImpl_sys_iOS_ZFUIKit_ZFUISizeFromCGSize(sizeTmp);
    }

    virtual zfint textSizeCurrent(ZF_IN ZFUITextView *textView)
    {
        return ((__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView()).textSize;
    }

    virtual void layoutNativeTextView(ZF_IN ZFUITextView *textView,
                                      ZF_IN const ZFUISize &viewSize)
    {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        zfint fixedTextSize = this->calcTextSizeAutoChange(textView, viewSize);
        nativeImplView.textSize = fixedTextSize;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextViewImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUITextViewImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

