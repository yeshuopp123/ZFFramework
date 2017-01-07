/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit.h"

#if ZF_ENV_sys_iOS
ZF_NAMESPACE_GLOBAL_BEGIN

// convert utility
void ZFImpl_sys_iOS_ZFUIKit_ZFUIPointToCGPoint(ZF_OUT CGPoint &ret, ZF_IN const ZFUIPoint &point)
{
    ret.x = point.x;
    ret.y = point.y;
}
void ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint(ZF_OUT ZFUIPoint &ret, ZF_IN const CGPoint &cgPoint)
{
    ret.x = zfmRound(cgPoint.x);
    ret.y = zfmRound(cgPoint.y);
}
void ZFImpl_sys_iOS_ZFUIKit_ZFUISizeToCGSize(ZF_OUT CGSize &ret, ZF_IN const ZFUISize &size)
{
    ret.width = size.width;
    ret.height = size.height;
}
void ZFImpl_sys_iOS_ZFUIKit_ZFUISizeFromCGSize(ZF_OUT ZFUISize &ret, ZF_IN const CGSize &cgSize)
{
    ret.width = zfmRound(cgSize.width);
    ret.height = zfmRound(cgSize.height);
}
void ZFImpl_sys_iOS_ZFUIKit_ZFUIRectToCGRect(ZF_OUT CGRect &ret, ZF_IN const ZFUIRect &rect)
{
    ret.origin.x = rect.point.x;
    ret.origin.y = rect.point.y;
    ret.size.width = rect.size.width;
    ret.size.height = rect.size.height;
}
void ZFImpl_sys_iOS_ZFUIKit_ZFUIRectFromCGRect(ZF_OUT ZFUIRect &ret, ZF_IN const CGRect &cgRect)
{
    ret.point.x = zfmRound(cgRect.origin.x);
    ret.point.y = zfmRound(cgRect.origin.y);
    ret.size.width = zfmRound(cgRect.size.width);
    ret.size.height = zfmRound(cgRect.size.height);
}

void ZFImpl_sys_iOS_ZFUIKit_ZFUIMarginToUIEdgeInsets(ZF_OUT UIEdgeInsets &ret, ZF_IN const ZFUIMargin &margin)
{
    ret.left = margin.left;
    ret.top = margin.top;
    ret.right = margin.right;
    ret.bottom = margin.bottom;
}
void ZFImpl_sys_iOS_ZFUIKit_ZFUIMarginFromUIEdgeInsets(ZF_OUT ZFUIMargin &ret, ZF_IN const UIEdgeInsets &edgeInsets)
{
    ret.left = zfmRound(edgeInsets.left);
    ret.top = zfmRound(edgeInsets.top);
    ret.right = zfmRound(edgeInsets.right);
    ret.bottom = zfmRound(edgeInsets.bottom);
}

UIColor *ZFImpl_sys_iOS_ZFUIKit_ZFUIColorToUIColor(ZF_IN const ZFUIColor &color)
{
    return [UIColor colorWithRed:(ZFUIColorGetR(color) / 255.0f) green:(ZFUIColorGetG(color) / 255.0f) blue:(ZFUIColorGetB(color) / 255.0f) alpha:(ZFUIColorGetA(color) / 255.0f)];
}
void ZFImpl_sys_iOS_ZFUIKit_ZFUIColorFromUIColor(ZF_OUT ZFUIColor &ret, ZF_IN UIColor *uiColor)
{
    zfindex argb[4] = {0};
    ZFImpl_sys_iOS_ZFUIKit_UIColorToARGB(argb, uiColor);
    ret = ZFUIColorMake(argb[0], argb[1], argb[2], argb[3]);
}

// ============================================================
// util method
zfbool ZFImpl_sys_iOS_ZFUIKit_UIColorToARGB(ZF_IN_OUT zfindex *pARGB, ZF_IN UIColor *color)
{
    for(zfindex i = 0; i < 4; ++i)
    {
        pARGB[i] = 0;
    }
    if(color == nil)
    {
        return zftrue;
    }
    zfindex numComponents = CGColorGetNumberOfComponents(color.CGColor);
    if(numComponents == 4)
    {
        const CGFloat *buf = CGColorGetComponents([color CGColor]);
        pARGB[0] = (zfindex)(buf[3] * 255);
        for(zfindex i = 1; i < 4; ++i)
        {
            pARGB[i] = (zfindex)(buf[i - 1] * 255);
        }
    }
    else
    {
        CGColorSpaceRef rgbColorSpace = CGColorSpaceCreateDeviceRGB();
        unsigned char buf[5] = {0};
        CGContextRef context = CGBitmapContextCreate(
            &buf,
            1,
            1,
            8,
            4,
            rgbColorSpace,
            kCGImageAlphaNoneSkipLast);
        CGContextSetFillColorWithColor(context, [color CGColor]);
        CGContextFillRect(context, CGRectMake(0, 0, 1, 1));
        CGContextRelease(context);
        CGColorSpaceRelease(rgbColorSpace);
        for (zfindex i = 0; i < 4; ++i)
        {
            pARGB[i] = buf[i];
        }
    }

    return zftrue;
}
void ZFImpl_sys_iOS_ZFUIKit_UIColorGetInfo(ZF_OUT zfstring &ret, ZF_IN UIColor *color)
{
    zfindex rgba[4] = {0};
    if(color != nil && ZFImpl_sys_iOS_ZFUIKit_UIColorToARGB(rgba, color))
    {
        zfstringAppend(ret, zfText("#%02X%02X%02X%02X"), (zfuint)rgba[0], (zfuint)rgba[1], (zfuint)rgba[2], (zfuint)rgba[3]);
    }
}

static void _ZFP_ZFImpl_sys_iOS_ZFUIKit_UIViewGetViewTree_recursive(ZF_IN_OUT zfstring &s, UIView *view, zfindex depth, zfindex siblingIndex)
{
    zfstringAppend(s, zfText("|%2d "), siblingIndex);
    for(zfindex i = 0; i < depth; ++i)
    {
        s += zfText("| ");
    }

    NSString *viewInfo = [NSString stringWithFormat:@"<%@ %08X>", [view class], (zfuint)[view hash]];
    viewInfo = [viewInfo stringByAppendingFormat:@" (%d, %d, %d, %d)",
        (int)view.frame.origin.x, (int)view.frame.origin.y, (int)view.frame.size.width, (int)view.frame.size.height];

#if 0 // test
#define _TEST_PROPERTY
    if([view respondsToSelector:@selector(_TEST_PROPERTY)])
    {
        id value = [view performSelector:@selector(_TEST_PROPERTY)];
        if(value != nil)
        {
            viewInfo = [viewInfo stringByAppendingFormat:@" %s:%@", ZFM_TOSTRING_A(_TEST_PROPERTY), value];
        }
    }
#endif // test

#if 1 // scroll content
    if([view isKindOfClass:[UIScrollView class]])
    {
        UIScrollView *scrollView = (UIScrollView *)view;
        zfstring info = zfstringWithFormat(zfText("(%d, %d, %d, %d)"),
            (zfint)scrollView.contentOffset.x, (zfint)scrollView.contentOffset.y, (zfint)scrollView.contentSize.width, (zfint)scrollView.contentSize.height);
        viewInfo = [viewInfo stringByAppendingFormat:@" scroll:%@", [NSString stringWithUTF8String:info]];
    }
#endif // scroll content

#if 1 // bg
    if(view.backgroundColor != nil)
    {
        zfstring colorInfo = ZFImpl_sys_iOS_ZFUIKit_UIColorGetInfo(view.backgroundColor);
        viewInfo = [viewInfo stringByAppendingFormat:@" bg:%@", [NSString stringWithUTF8String:colorInfo]];
    }
#endif // bg

#if 1 // hidden
    if(view.hidden)
    {
        viewInfo = [viewInfo stringByAppendingString:@" hidden"];
    }
#endif // hidden

#if 1 // UIDisabled
    if(!view.userInteractionEnabled)
    {
        viewInfo = [viewInfo stringByAppendingString:@" UIDisabled"];
    }
#endif // UIDisabled

#if 1 // text
    if([view respondsToSelector:@selector(text)])
    {
        id value = [view performSelector:@selector(text)];
        if(value != nil)
        {
            viewInfo = [viewInfo stringByAppendingFormat:@" text:%@", value];
        }
    }
#endif // text

#if 1 // title
    if([view respondsToSelector:@selector(currentTitle)])
    {
        id value = [view performSelector:@selector(currentTitle)];
        if(value != nil)
        {
            viewInfo = [viewInfo stringByAppendingFormat:@" currentTitle:%@", value];
        }
    }
    else if([view respondsToSelector:@selector(titleLabel)])
    {
        id value = ((UILabel *)[view performSelector:@selector(titleLabel)]).text;
        if(value != nil)
        {
            viewInfo = [viewInfo stringByAppendingFormat:@" titleLabel:%@", value];
        }
    }
#endif // title

    s += ZFStringA2Z([viewInfo UTF8String]);
    s += zfText("\n");

    NSArray *subviews = view.subviews;
    for(NSUInteger i = 0; i < [subviews count]; ++i)
    {
        _ZFP_ZFImpl_sys_iOS_ZFUIKit_UIViewGetViewTree_recursive(s, [subviews objectAtIndex:i], depth + 1, i);
    }
}
void ZFImpl_sys_iOS_ZFUIKit_UIViewGetViewTree(ZF_OUT zfstring &ret, ZF_IN UIView *view)
{
    ret += zfText("==================== UIView tree begin ====================\n");
    if(view != nil)
    {
        @autoreleasepool {
            _ZFP_ZFImpl_sys_iOS_ZFUIKit_UIViewGetViewTree_recursive(ret, view, 0, 0);
        }
    }
    ret += zfText("==================== UIView tree  end  ====================\n");
}

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

