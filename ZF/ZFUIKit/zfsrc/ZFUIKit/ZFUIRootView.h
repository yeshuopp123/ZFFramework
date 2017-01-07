/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIRootView.h
 * @brief root view for holding a ZFUIView's tree
 */

#ifndef _ZFI_ZFUIRootView_h_
#define _ZFI_ZFUIRootView_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief root view for holding a ZFUIView's tree
 *
 * internally used to manage view's,
 * usually you should not use this class
 */
zffinal zfclass ZF_ENV_EXPORT ZFUIRootView : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIRootView, ZFUIView)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when root view need layout
     */
    ZFOBSERVER_EVENT(RootViewOnRequestLayout)

public:
    /**
     * @brief set scale for root view, as well as all of its children
     */
    virtual void scaleSet(ZF_IN zffloat scale);

public:
    zfoverride
    virtual void layoutRequest(void);

protected:
    zfoverride
    virtual void viewOnAddToParent(ZF_IN ZFUIView *parent);
    zfoverride
    virtual void viewOnRemoveFromParent(ZF_IN ZFUIView *parent);

protected:
    /**
     * @brief for a root view, this method would always return sizeHint
     */
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);

protected:
    /** @brief see #EventRootViewOnRequestLayout */
    virtual inline void rootViewOnRequestLayout(void)
    {
        this->observerNotify(ZFUIRootView::EventRootViewOnRequestLayout());
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIRootView_h_

