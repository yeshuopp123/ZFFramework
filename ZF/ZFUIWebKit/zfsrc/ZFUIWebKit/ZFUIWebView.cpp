/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIWebView.h"
#include "protocol/ZFProtocolZFUIWebView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIWebView)

// ============================================================
// ZFUIWebView
ZFOBJECT_REGISTER(ZFUIWebView)

ZFOBSERVER_EVENT_REGISTER(ZFUIWebView, WebLoadStateOnChange)

void ZFUIWebView::webLoadUrl(ZF_IN const zfchar *url)
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->webLoadUrl(this, url);
}
void ZFUIWebView::webLoadHtml(ZF_IN const zfchar *html,
                              ZF_IN_OPT const zfchar *baseUrl /* = zfnull */)
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->webLoadHtml(this, html, baseUrl);
}
void ZFUIWebView::webReload(void)
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->webReload(this);
}
void ZFUIWebView::webLoadStop(void)
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->webLoadStop(this);
}
void ZFUIWebView::webGoBack(void)
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->webGoBack(this);
}
void ZFUIWebView::webGoForward(void)
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->webGoForward(this);
}

zfbool ZFUIWebView::webLoading(void)
{
    return ZFPROTOCOL_ACCESS(ZFUIWebView)->webLoading(this);
}
zfbool ZFUIWebView::webGoBackAvailable(void)
{
    return ZFPROTOCOL_ACCESS(ZFUIWebView)->webGoBackAvailable(this);
}
zfbool ZFUIWebView::webGoForwardAvailable(void)
{
    return ZFPROTOCOL_ACCESS(ZFUIWebView)->webGoForwardAvailable(this);
}

ZFObject *ZFUIWebView::objectOnInit(void)
{
    zfsuper::objectOnInit();

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD _ZFP_ZFUIWebView_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUIWebView)->nativeWebViewDestroy(view->to<ZFUIWebView *>(), nativeImplView);
        }
    };
    this->nativeImplViewSet(ZFPROTOCOL_ACCESS(ZFUIWebView)->nativeWebViewCreate(this),
                            _ZFP_ZFUIWebView_nativeImplViewDestroy::action);

    return this;
}
void ZFUIWebView::objectOnDealloc(void)
{
    zfsuper::objectOnDealloc();
}
void ZFUIWebView::objectOnDeallocPrepare(void)
{
    this->webLoadStop();
    zfsuper::objectOnDeallocPrepare();
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIWebView_ignoreFrom_ZFUIViewBlinkWhenFoocus, ZFLevelZFFrameworkNormal)
{
    ZFUIViewBlinkWhenFocusFilter.classFilter.filterAdd(ZFUIWebView::ClassData());
}
ZF_GLOBAL_INITIALIZER_END(ZFUIWebView_ignoreFrom_ZFUIViewBlinkWhenFoocus)

ZF_NAMESPACE_GLOBAL_END

