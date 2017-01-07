/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIPage.h
 * @brief abstract page
 */

#ifndef _ZFI_ZFUIPage_h_
#define _ZFI_ZFUIPage_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief page resume reason
 */
ZFENUM_BEGIN(ZFUIPageResumeReason)
    ZFENUM_VALUE(ByManagerResume)
    ZFENUM_VALUE(ByRequest)
    ZFENUM_VALUE(FromBackground)
ZFENUM_SEPARATOR(ZFUIPageResumeReason)
    ZFENUM_VALUE_REGISTER(ByManagerResume)
    ZFENUM_VALUE_REGISTER(ByRequest)
    ZFENUM_VALUE_REGISTER(FromBackground)
ZFENUM_END(ZFUIPageResumeReason)

/**
 * @brief page pause reason
 */
ZFENUM_BEGIN(ZFUIPagePauseReason)
    ZFENUM_VALUE(ByManagerPause)
    ZFENUM_VALUE(ToBackground)
    ZFENUM_VALUE(BeforeDestroy)
ZFENUM_SEPARATOR(ZFUIPagePauseReason)
    ZFENUM_VALUE_REGISTER(ByManagerPause)
    ZFENUM_VALUE_REGISTER(ToBackground)
    ZFENUM_VALUE_REGISTER(BeforeDestroy)
ZFENUM_END(ZFUIPagePauseReason)

/**
 * @brief invalid page group id, see #ZFUIPageManager::requestPageGroupResume for more info
 */
#define ZFUIPageGroupIdInvalid zfText("ZFUIPageGroupIdInvalid")

// ============================================================
zfclassFwd ZFUIPageManager;
zfclassFwd _ZFP_ZFUIPageManagerPrivate;
/**
 * @brief abstract page, see #ZFUIPageManager for how to use
 */
zfinterface ZF_ENV_EXPORT ZFUIPage : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(ZFUIPage, ZFInterface)

    // ============================================================
    // observers
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page created
     */
    ZFOBSERVER_EVENT(PageAfterCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPageResumeReason
     */
    ZFOBSERVER_EVENT(PageBeforeResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPageResumeReason
     */
    ZFOBSERVER_EVENT(PageAfterResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPagePauseReason
     */
    ZFOBSERVER_EVENT(PageBeforePause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPagePauseReason
     */
    ZFOBSERVER_EVENT(PageAfterPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page destroyed
     */
    ZFOBSERVER_EVENT(PageBeforeDestroy)

public:
    /**
     * @brief owner page manager
     */
    zffinal ZFUIPageManager *pageManager(void)
    {
        return this->_ZFP_ZFUIPage_pageManager;
    }
    /**
     * @brief owner page manager
     */
    template<typename T_ZFUIPageManager>
    T_ZFUIPageManager pageManager(void)
    {
        return ZFCastZFObjectUnchecked(T_ZFUIPageManager, this->pageManager());
    }

private:
    zffinal void pageCreateParamSet(ZF_IN ZFObject *pageCreateParam)
    {
        ZFPropertyChange(this->_ZFP_ZFUIPage_pageCreateParam, pageCreateParam);
    }
public:
    /**
     * @brief page create param passed from #ZFUIPageManager::requestPageCreate
     */
    zffinal ZFObject *pageCreateParam(void)
    {
        return _ZFP_ZFUIPage_pageCreateParam;
    }

public:
    /**
     * @brief page group id, see #ZFUIPageManager::requestPageGroupResume for more info
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfstring, pageGroupId, ZFPropertyInitValue(ZFUIPageGroupIdInvalid))

public:
    /**
     * @brief true if page created (true after #pageOnCreate called)
     */
    virtual zfbool pageCreated(void)
    {
        return _ZFP_ZFUIPage_pageCreated;
    }
    /**
     * @brief true if page resumed (true after #pageOnResume called)
     */
    virtual zfbool pageResumed(void)
    {
        return _ZFP_ZFUIPage_pageResumed;
    }
    /**
     * @brief true if page attached (true after #pageOnAttach and false before #pageOnDetach)
     */
    virtual zfbool pageAttached(void)
    {
        return _ZFP_ZFUIPage_pageAttached;
    }

    /**
     * @brief equal to invoke #ZFUIPageManager::requestPageDestroy
     */
    virtual void pageDestroy(void);

    // ============================================================
    // page life cycle
protected:
    /**
     * @brief see #ZFUIPageManager
     */
    virtual inline void pageOnCreate(void)
    {
        this->_ZFP_ZFUIPage_pageOnCreateCalled = zftrue;
    }
    /**
     * @brief see #ZFUIPageManager
     */
    virtual inline void pageOnResume(ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->_ZFP_ZFUIPage_pageOnResumeCalled = zftrue;
    }
    /**
     * @brief see #ZFUIPageManager
     */
    virtual inline void pageOnAttach(ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->_ZFP_ZFUIPage_pageOnAttachCalled = zftrue;
    }
    /**
     * @brief see #ZFUIPageManager
     */
    virtual inline void pageOnDetach(ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->_ZFP_ZFUIPage_pageOnDetachCalled = zftrue;
    }
    /**
     * @brief see #ZFUIPageManager
     */
    virtual inline void pageOnPause(ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->_ZFP_ZFUIPage_pageOnPauseCalled = zftrue;
    }
    /**
     * @brief see #ZFUIPageManager
     */
    virtual inline void pageOnDestroy(void)
    {
        this->_ZFP_ZFUIPage_pageOnDestroyCalled = zftrue;
    }

    // ============================================================
    // page events
protected:
    /** @brief see #EventPageAfterCreate */
    virtual inline void pageAfterCreate(void)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageAfterCreate());
    }
    /** @brief see #EventPageBeforeResume */
    virtual inline void pageBeforeResume(ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageBeforeResume(),
                                         zflineAllocWithoutLeakTest(ZFUIPageResumeReason, reason));
    }
    /** @brief see #EventPageAfterResume */
    virtual inline void pageAfterResume(ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageAfterResume(),
                                         zflineAllocWithoutLeakTest(ZFUIPageResumeReason, reason));
    }
    /** @brief see #EventPageBeforePause */
    virtual inline void pageBeforePause(ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageBeforePause(),
                                         zflineAllocWithoutLeakTest(ZFUIPagePauseReason, reason));
    }
    /** @brief see #EventPageAfterPause */
    virtual inline void pageAfterPause(ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageAfterPause(),
                                         zflineAllocWithoutLeakTest(ZFUIPagePauseReason, reason));
    }
    /** @brief see #EventPageBeforeDestroy */
    virtual inline void pageBeforeDestroy(void)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageBeforeDestroy());
    }

    // ============================================================
    // pageDelayDetach
protected:
    /** @brief see #ZFUIPageManager::pageDelayDetachOnCheck */
    virtual void pageDelayDetachOnCheck(ZF_IN ZFUIPagePauseReasonEnum reason);
    /** @brief see #ZFUIPageManager::pageDelayDetachOnCheck */
    zffinal void pageDelayDetachEnable(void);
    /** @brief see #ZFUIPageManager::pageDelayDetachOnCheck */
    zffinal void pageDelayDetachNotifyFinish(void);

    // ============================================================
    // pageDelayDestroy
protected:
    /** @brief see #ZFUIPageManager::pageDelayDestroyOnCheck */
    virtual void pageDelayDestroyOnCheck(void);
    /** @brief see #ZFUIPageManager::pageDelayDestroyOnCheck */
    zffinal void pageDelayDestroyEnable(void);
    /** @brief see #ZFUIPageManager::pageDelayDestroyOnCheck */
    zffinal void pageDelayDestroyNotifyFinish(void);

    // ============================================================
private:
    ZFUIPageManager *_ZFP_ZFUIPage_pageManager;
    ZFObject *_ZFP_ZFUIPage_pageCreateParam;
    zfbool _ZFP_ZFUIPage_pageCreated;
    zfbool _ZFP_ZFUIPage_pageResumed;
    zfbool _ZFP_ZFUIPage_pageAttached;
    zfbool _ZFP_ZFUIPage_pageOnCreateCalled;
    zfbool _ZFP_ZFUIPage_pageOnResumeCalled;
    zfbool _ZFP_ZFUIPage_pageOnAttachCalled;
    zfbool _ZFP_ZFUIPage_pageOnDetachCalled;
    zfbool _ZFP_ZFUIPage_pageOnPauseCalled;
    zfbool _ZFP_ZFUIPage_pageOnDestroyCalled;
    zfindex _ZFP_ZFUIPage_pageDelayDetachFlag;
    zfindex _ZFP_ZFUIPage_pageDelayDetachByManagerFlag;
    zfindex _ZFP_ZFUIPage_pageDelayDestroyFlag;
    zfindex _ZFP_ZFUIPage_pageDelayDestroyByManagerFlag;
protected:
    /** @cond ZFPrivateDoc */
    ZFUIPage(void)
    : _ZFP_ZFUIPage_pageManager(zfnull)
    , _ZFP_ZFUIPage_pageCreateParam(zfnull)
    , _ZFP_ZFUIPage_pageCreated(zffalse)
    , _ZFP_ZFUIPage_pageResumed(zffalse)
    , _ZFP_ZFUIPage_pageAttached(zffalse)
    , _ZFP_ZFUIPage_pageOnCreateCalled(zffalse)
    , _ZFP_ZFUIPage_pageOnResumeCalled(zffalse)
    , _ZFP_ZFUIPage_pageOnAttachCalled(zffalse)
    , _ZFP_ZFUIPage_pageOnDetachCalled(zffalse)
    , _ZFP_ZFUIPage_pageOnPauseCalled(zffalse)
    , _ZFP_ZFUIPage_pageOnDestroyCalled(zffalse)
    , _ZFP_ZFUIPage_pageDelayDetachFlag(0)
    , _ZFP_ZFUIPage_pageDelayDetachByManagerFlag(0)
    , _ZFP_ZFUIPage_pageDelayDestroyFlag(0)
    , _ZFP_ZFUIPage_pageDelayDestroyByManagerFlag(0)
    {
    }
    /** @endcond */
private:
    zffinal void _ZFP_ZFUIPage_pageCreate(void);
    zffinal void _ZFP_ZFUIPage_pageResume(ZF_IN ZFUIPageResumeReasonEnum reason);
    zffinal void _ZFP_ZFUIPage_pageAttach(ZF_IN ZFUIPageResumeReasonEnum reason);
    zffinal void _ZFP_ZFUIPage_pageDetach(ZF_IN ZFUIPagePauseReasonEnum reason);
    zffinal void _ZFP_ZFUIPage_pagePause(ZF_IN ZFUIPagePauseReasonEnum reason);
    zffinal void _ZFP_ZFUIPage_pageDestroy(void);
private:
    friend zfclassFwd ZFUIPageManager;
    friend zfclassFwd _ZFP_ZFUIPageManagerPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPage_h_

