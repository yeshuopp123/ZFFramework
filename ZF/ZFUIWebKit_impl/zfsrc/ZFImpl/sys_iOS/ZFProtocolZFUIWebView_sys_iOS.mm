/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIWebKit.h"
#include "ZFUIWebKit/protocol/ZFProtocolZFUIWebView.h"

#if ZF_ENV_sys_iOS

#import <UIKit/UIKit.h>

@interface _ZFP_ZFUIWebViewImpl_sys_iOS_View : UIWebView<UIWebViewDelegate>
@property (nonatomic, assign) ZFUIWebView *_ZFP_ownerZFUIWebView;
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUIWebView) *_ZFP_impl;
@end
@implementation _ZFP_ZFUIWebViewImpl_sys_iOS_View
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    return YES;
}
- (void)webViewDidStartLoad:(UIWebView *)webView
{
    self._ZFP_impl->notifyWebLoadStateChanged(self._ZFP_ownerZFUIWebView);
}
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    self._ZFP_impl->notifyWebLoadStateChanged(self._ZFP_ownerZFUIWebView);
}
- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    self._ZFP_impl->notifyWebLoadStateChanged(self._ZFP_ownerZFUIWebView);
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIWebViewImpl_sys_iOS, ZFUIWebView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UIWebView"))
public:
    virtual void *nativeWebViewCreate(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = [_ZFP_ZFUIWebViewImpl_sys_iOS_View new];
        nativeWebView._ZFP_ownerZFUIWebView = webView;
        nativeWebView._ZFP_impl = this;
        return (__bridge_retained void *)nativeWebView;
    }
    virtual void nativeWebViewDestroy(ZF_IN ZFUIWebView *webView,
                                      ZF_IN void *nativeWebView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebViewTmp = (__bridge_transfer _ZFP_ZFUIWebViewImpl_sys_iOS_View *)nativeWebView;
        // load empty data to reduce memory usage
        [nativeWebViewTmp loadHTMLString:@"" baseURL:nil];
        nativeWebViewTmp = nil;
    }

    virtual void webLoadUrl(ZF_IN ZFUIWebView *webView,
                            ZF_IN const zfchar *url)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
        [nativeWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]]]];
    }
    virtual void webLoadHtml(ZF_IN ZFUIWebView *webView,
                             ZF_IN const zfchar *html,
                             ZF_IN_OPT const zfchar *baseUrl = zfnull)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
        NSURL *baseUrlTmp = nil;
        if(baseUrl != zfnull)
        {
            baseUrlTmp = [NSURL URLWithString:[NSString stringWithCString:baseUrl encoding:NSUTF8StringEncoding]];
        }
        [nativeWebView loadHTMLString:[NSString stringWithCString:html encoding:NSUTF8StringEncoding] baseURL:baseUrlTmp];
    }
    virtual void webReload(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        [nativeWebView reload];
    }
    virtual void webLoadStop(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        [nativeWebView stopLoading];
    }

    virtual void webGoBack(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        [nativeWebView goBack];
    }
    virtual void webGoForward(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        [nativeWebView goForward];
    }

    virtual zfbool webLoading(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        return nativeWebView.loading;
    }
    virtual zfbool webGoBackAvailable(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        return [nativeWebView canGoBack];
    }
    virtual zfbool webGoForwardAvailable(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        return [nativeWebView canGoForward];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIWebViewImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIWebViewImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

