/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIWebView.h
 * @brief web view
 */

#ifndef _ZFI_ZFUIWebView_h_
#define _ZFI_ZFUIWebView_h_

#include "ZFUIWebKitDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIWebView
/**
 * @brief web view
 */
zfclass ZF_ENV_EXPORT ZFUIWebView : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIWebView, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIWebView)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when load start or stop,
     * new state can be checked by #webLoading
     */
    ZFOBSERVER_EVENT(WebLoadStateOnChange)

public:
    // ============================================================
    // properties
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(zfbool, viewFocusable, zftrue)

public:
    /**
     * @brief load from url
     */
    virtual void webLoadUrl(ZF_IN const zfchar *url);
    /**
     * @brief load from html string
     */
    virtual void webLoadHtml(ZF_IN const zfchar *html,
                             ZF_IN_OPT const zfchar *baseUrl = zfnull);
    /**
     * @brief reload current web data
     */
    virtual void webReload(void);
    /**
     * @brief stop loading
     */
    virtual void webLoadStop(void);

    /**
     * @brief go back, see #webGoBackAvailable
     */
    virtual void webGoBack(void);
    /**
     * @brief go forward, see #webGoForwardAvailable
     */
    virtual void webGoForward(void);

public:
    /**
     * @brief true if web is loading
     */
    virtual zfbool webLoading(void);

    /**
     * @brief whether the web can go back, see #webGoBack
     */
    virtual zfbool webGoBackAvailable(void);
    /**
     * @brief whether the web can go forward, see #webGoForward
     */
    virtual zfbool webGoForwardAvailable(void);

public:
    zffinal void _ZFP_ZFUIWebView_notifyWebLoadStateChanged(void)
    {
        this->webLoadStateOnChange();
    }

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    /** @brief see #EventWebLoadStateOnChange */
    virtual inline void webLoadStateOnChange(void)
    {
        this->observerNotify(ZFUIWebView::EventWebLoadStateOnChange());
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIWebView_h_

