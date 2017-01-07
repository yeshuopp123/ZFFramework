/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIPageManager.h
 * @brief abstract page manager
 */

#ifndef _ZFI_ZFUIPageManager_h_
#define _ZFI_ZFUIPageManager_h_

#include "ZFUIWidgetDef.h"
#include "ZFUIPage.h"
#include "ZFUIPageRequest.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFUIPageManagerPrivate;
/**
 * @brief abstract page manager to manage multiple #ZFUIPage
 *
 * a page is an abstract object with those life cycle:
 * -  #ZFUIPage::pageOnCreate, create necessary contents
 * -  #ZFUIPage::pageOnResume, before making the page active
 * -  #ZFUIPage::pageOnPause, before making the page deactive
 * -  #ZFUIPage::pageOnDestroy, to destroy the page
 *
 * and a page manager is an abstract object to manage multiple pages\n
 * \n
 * pages is managed by life cycle request, you must use #ZFUIPageManager's method to manage pages,
 * including creating pages or showing pages\n
 * a page manager can hold many pages, however, only one page at most is resumed at one time,
 * resuming another page would cause current page to be paused,
 * pausing page manager would also causing current page to be paused\n
 * page's life cycle is requested by app code,
 * and is ensured to be queued to run at future\n
 * \n
 * ADVANCED:\n
 * typically, a page is a normal view to hold UI elements to interact with user,
 * and a page manager is a native window or UI container to hold the pages,
 * however, you may embeded a page manager to another container,
 * even another page manager, for how to, please refer to #embededCreate
 */
zfinterface ZF_ENV_EXPORT ZFUIPageManager : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(ZFUIPageManager, ZFInterface)

    // ============================================================
    // observers
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager resumed
     */
    ZFOBSERVER_EVENT(ManagerAfterResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager resumed
     */
    ZFOBSERVER_EVENT(ManagerBeforePause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager's UIEnable changed,
     * see #managerUIBlockedSet
     */
    ZFOBSERVER_EVENT(ManagerUIBlockedOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #requestBlockedSet setting changed
     */
    ZFOBSERVER_EVENT(RequestBlockedOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the #ZFUIPageRequest,
     * called before resolve the request,
     * you may change the #ZFUIPageRequest::requestResolved state to override the request
     * @note this event would also be fired to the #ZFUIPageRequest
     */
    ZFOBSERVER_EVENT(RequestOnResolvePrepare)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the #ZFUIPageRequest,
     * called to resolve the request
     * @note this event would also be fired to the #ZFUIPageRequest
     */
    ZFOBSERVER_EVENT(RequestOnResolve)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the #ZFUIPageRequest,
     * called when a request resolved
     * @note this event would also be fired to the #ZFUIPageRequest
     */
    ZFOBSERVER_EVENT(RequestOnResolveFinish)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when all request resolved
     * @note it's allowed to post new request during activating this event,
     *   care about dead loop
     */
    ZFOBSERVER_EVENT(RequestOnResolveFinishAll)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page created,
     * sender is the page,
     * param0 is always null,
     * param1 is the page manager
     */
    ZFOBSERVER_EVENT(PageAfterCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * sender is the page,
     * param0 is #ZFUIPageResumeReason,
     * param1 is the page manager
     */
    ZFOBSERVER_EVENT(PageBeforeResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * sender is the page,
     * param0 is #ZFUIPageResumeReason,
     * param1 is the page manager
     */
    ZFOBSERVER_EVENT(PageAfterResume)
     /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page attached\n
     * sender is the page,
     * param0 is the #ZFUIPageResumeReason
     */
    ZFOBSERVER_EVENT(PageOnAttach)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page detached\n
     * sender is the page,
     * param0 is the #ZFUIPagePauseReason
     */
    ZFOBSERVER_EVENT(PageOnDetach)
   /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page paused,
     * sender is the page,
     * param0 is #ZFUIPagePauseReason,
     * param1 is the page manager
     */
    ZFOBSERVER_EVENT(PageBeforePause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page paused,
     * sender is the page,
     * param0 is #ZFUIPagePauseReason,
     * param1 is the page manager
     */
    ZFOBSERVER_EVENT(PageAfterPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page destroyed,
     * sender is the page,
     * param0 is always null,
     * param1 is the page manager
     */
    ZFOBSERVER_EVENT(PageBeforeDestroy)

    // ============================================================
    // embeded logic
public:
    /**
     * @brief called by implementation to achieve page manager's life cycle, see #ZFUIPageManager
     *
     * a page manager can be a window or native UI container,
     * or even embeded to a child page of another page manager,
     * so we supply direct life cycle manage method for implementation to achieve life cycle,
     * you must ensure it's logical valid while handling it
     */
    zffinal void embededCreate(void);
    /** @brief see #embededCreate */
    zffinal void embededResume(void);
    /** @brief see #embededCreate */
    zffinal void embededPause(void);
    /** @brief see #embededCreate */
    zffinal void embededDestroy(void);

    // ============================================================
    // manager control
public:
    /**
     * @brief enable or disable manager's user interaction
     *
     * it's useful to disable user interaction when manager is doing some heavy works,
     * such as animating switching pages\n
     * subclass should override #managerUIBlockedOnChange to achieve actual state change\n
     * this method can be embeded but must be paired,
     * last time you call this method to restore enable state would finally restore enable state
     */
    zffinal void managerUIBlockedSet(ZF_IN zfbool value);
    /**
     * @brief see #managerUIBlockedSet
     */
    zffinal zfindex managerUIBlocked(void);
protected:
    /** @brief see #EventManagerUIBlockedOnChange, subclass should implement and achieve actual action */
    virtual void managerUIBlockedOnChange(void)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventManagerUIBlockedOnChange());
    }

    // ============================================================
    // page access
public:
    /**
     * @brief page count of this page manager
     */
    zffinal zfindex pageCount(void);
    /**
     * @brief directly access the page list
     *
     * you must not store the list for future use,
     * since its content may change when resolving page request\n
     * you must not modify the list,
     * except when using with #movePageBegin
     */
    zffinal ZFCoreArrayPOD<ZFUIPage *> &pageList(void);
    /**
     * @brief page at index
     */
    inline ZFUIPage *pageAt(ZF_IN zfindex index)
    {
        return this->pageList().get(index);
    }
    /**
     * @brief page at index
     */
    template<typename T_ZFUIPage>
    inline T_ZFUIPage pageAt(ZF_IN zfindex index)
    {
        return ZFCastZFObjectUnchecked(T_ZFUIPage, this->pageAt(index));
    }
    /**
     * @brief return foreground page or null if no page
     */
    inline ZFUIPage *pageForeground(void)
    {
        if(!this->pageList().isEmpty())
        {
            return this->pageList().getLast();
        }
        return zfnull;
    }
    /**
     * @brief return foreground page or null if no page
     */
    template<typename T_ZFUIPage>
    inline T_ZFUIPage pageForeground(void)
    {
        return ZFCastZFObjectUnchecked(T_ZFUIPage, this->pageForeground());
    }

    // ============================================================
    // page request
public:
    /**
     * @brief request create page, see #ZFUIPageManager
     */
    zffinal void requestPageCreate(ZF_IN const ZFUIPageRequestPageCreateParam &createParam);
    /**
     * @brief request create page, see #ZFUIPageManager
     */
    zffinal void requestPageCreate(ZF_IN const ZFClass *pageClass,
                                   ZF_IN_OPT ZFObject *pageCreateParam = zfnull,
                                   ZF_IN_OPT zfbool pageAutoResume = zftrue);
    /**
     * @brief request resume page
     */
    zffinal void requestPageResume(ZF_IN ZFUIPage *page);
    /**
     * @brief request resume a group of page
     *
     * used to resume multiple pages,
     * all page with same group id would be reordered to page stack's front,
     * keeping their original relative stack order\n
     * null group id belongs to nothing
     */
    zffinal void requestPageGroupResume(ZF_IN const zfchar *pageGroupId);
    /**
     * @brief request destroy page
     */
    zffinal void requestPageDestroy(ZF_IN ZFUIPage *page);
    /**
     * @brief post a custom request to queue, which would be resolved by #requestOnResolve
     */
    zffinal void requestPost(ZF_IN ZFUIPageRequest *request);
    /**
     * @brief util method to post a listener to be execute at current request queue's end
     */
    zffinal void requestPostCustom(ZF_IN const ZFListener &listener,
                                   ZF_IN_OPT ZFObject *userData = zfnull,
                                   ZF_IN_OPT ZFObject *param0 = zfnull,
                                   ZF_IN_OPT ZFObject *param1 = zfnull)
    {
        zfblockedAlloc(ZFUIPageRequestCustom, request);
        request->listenerSet(listener);
        request->userDataSet(userData);
        request->param0Set(param0);
        request->param1Set(param1);
        this->requestPost(request);
    }

public:
    /**
     * @brief block request to prevent multiple request running at same time
     *
     * newly posted request would be queued, until restored\n
     * #requestBlockedSet can be embeded with more than one time,
     * but must be paired
     */
    zffinal void requestBlockedSet(ZF_IN zfbool value);
    /**
     * @brief see #requestBlockedSet
     */
    zffinal zfindex requestBlocked(void);
protected:
    /** @brief see #EventRequestBlockedOnChange */
    virtual inline void requestBlockedOnChange(void)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventRequestBlockedOnChange());
    }

    // ============================================================
    // request resolve
protected:
    /** @brief see #EventRequestOnResolvePrepare */
    virtual inline void requestOnResolvePrepare(ZF_IN ZFUIPageRequest *pageRequest)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventRequestOnResolvePrepare(), pageRequest);
        pageRequest->observerNotifyWithCustomSender(this->toObject(), ZFUIPageManager::EventRequestOnResolvePrepare(), pageRequest);
    }
    /**
     * @brief resolve request, #ZFUIPageRequest::requestResolved must be set to true
     *   if you have resolved the request
     *
     * builtin request would be dispatched by this method to its proper resolve method,
     * such as #requestOnResolvePageCreate\n
     * for custom request, you should check the request's class type, and do your own work
     */
    virtual void requestOnResolve(ZF_IN ZFUIPageRequest *request);
    /** @brief see #EventRequestOnResolveFinish */
    virtual inline void requestOnResolveFinish(ZF_IN ZFUIPageRequest *pageRequest)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventRequestOnResolveFinish(), pageRequest);
        pageRequest->observerNotifyWithCustomSender(this->toObject(), ZFUIPageManager::EventRequestOnResolveFinish(), pageRequest);
    }
    /** @brief see #EventRequestOnResolveFinishAll */
    virtual inline void requestOnResolveFinishAll(void)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventRequestOnResolveFinishAll());
    }

protected:
    /**
     * @brief resolve #requestPageCreate
     *
     * by default, this method would call #resolvePageCreate to create page,
     * you may override and change the behavior, such as delay some time then invoke #resolvePageCreate
     */
    virtual void requestOnResolvePageCreate(ZF_IN ZFUIPageRequestPageCreate *request);
    /** @brief see #requestOnResolvePageCreate */
    zffinal void resolvePageCreate(ZF_IN ZFUIPageRequestPageCreate *request);

    /** @brief see #requestOnResolvePageCreate */
    virtual void requestOnResolvePageResume(ZF_IN ZFUIPageRequestPageResume *request);
    /** @brief see #requestOnResolvePageCreate */
    zffinal void resolvePageResume(ZF_IN ZFUIPageRequestPageResume *request);

    /** @brief see #requestOnResolvePageCreate */
    virtual void requestOnResolvePageGroupResume(ZF_IN ZFUIPageRequestPageGroupResume *request);
    /** @brief see #requestOnResolvePageCreate */
    zffinal void resolvePageGroupResume(ZF_IN ZFUIPageRequestPageGroupResume *request);

    /** @brief see #requestOnResolvePageCreate */
    virtual void requestOnResolvePageDestroy(ZF_IN ZFUIPageRequestPageDestroy *request);
    /** @brief see #requestOnResolvePageCreate */
    zffinal void resolvePageDestroy(ZF_IN ZFUIPageRequestPageDestroy *request);

    /** @brief see #requestOnResolvePageCreate */
    virtual void requestOnResolveCustom(ZF_IN ZFUIPageRequestCustom *request);
    /** @brief see #requestOnResolvePageCreate */
    zffinal void resolveCustom(ZF_IN ZFUIPageRequestCustom *request);

    // ============================================================
    // pageDelayDetach
protected:
    /**
     * @brief check whether the page need delay to detach, do nothing by default
     *
     * for some case, page need to process additional works after pause,
     * and should not be detached immediately,
     * to achieve this, you may override this method and call #pageDelayDetachEnable,
     * then finally notify finish by #pageDelayDetachNotifyFinish
     * to finally detach the page\n
     * #pageDelayDetachEnable can be called multiple times,
     * bust must be paired with #pageDelayDetachNotifyFinish\n
     * all delay detach task must be finished before next attach task,
     * otherwise, assert fail\n
     * delaying detach page would also cause #pageDelayDestroyEnable to be set automatically
     * @see pageDelayDestroyOnCheck
     */
    virtual void pageDelayDetachOnCheck(ZF_IN ZFUIPage *page,
                                        ZF_IN ZFUIPagePauseReasonEnum reason);
    /** @brief see #pageDelayDetachOnCheck */
    zffinal void pageDelayDetachEnable(ZF_IN ZFUIPage *page);
    /** @brief see #pageDelayDetachOnCheck */
    zffinal void pageDelayDetachNotifyFinish(ZF_IN ZFUIPage *page);
private:
    zffinal void _ZFP_ZFUIPageManager_pageDelayDetachNotifyFinish(ZF_IN ZFUIPage *page);

    // ============================================================
    // pageDelayDestroy
protected:
    /**
     * @brief check whether the page need delay to destroy, do nothing by default
     *
     * for some case, page need to process additional works after pause,
     * and should not be destroyed immediately,
     * to achieve this, you may override this method and call #pageDelayDestroyEnable,
     * then finally notify finish by #pageDelayDestroyNotifyFinish
     * to finally destroy the page\n
     * #pageDelayDestroyEnable can be called multiple times,
     * bust must be paired with #pageDelayDestroyNotifyFinish\n
     * page that delaying to destroy would be removed from page stack,
     * and can't be accessed anymore
     * @see pageDelayDetachOnCheck
     */
    virtual void pageDelayDestroyOnCheck(ZF_IN ZFUIPage *page);
    /** @brief see #pageDelayDestroyOnCheck */
    zffinal void pageDelayDestroyEnable(ZF_IN ZFUIPage *page);
    /** @brief see #pageDelayDestroyOnCheck */
    zffinal void pageDelayDestroyNotifyFinish(ZF_IN ZFUIPage *page);
private:
    zffinal void _ZFP_ZFUIPageManager_pageDelayDestroyNotifyFinish(ZF_IN ZFUIPage *page);

    // ============================================================
    // page move
protected:
    /**
     * @brief for advanced page manage only, directly move the page stack,
     *   must be paired with #movePageEnd
     *
     * usage:
     * @code
     *   // mark to begin move page
     *   manager->movePageBegin();
     *
     *   // directly access the page list
     *   ZFCoreArrayPOD<ZFUIPage *> &pageList = manager->pageList();
     *   // modify pageList
     *   // you can only change the order of the pageList contents
     *   // adding or removing is not allowed
     *
     *   // finally mark move end, page's resume state would be updated directly
     *   manager->movePageEnd();
     * @endcode
     */
    zffinal void movePageBegin(void);
    /** @brief see #movePageBegin */
    zffinal void movePageEnd(void);

    // ============================================================
    // page manager life cycle
protected:
    /** @brief for subclass to do actual works */
    virtual void managerOnCreate(void)
    {
    }
    /** @brief for subclass to do actual works */
    virtual void managerOnResume(void)
    {
    }
    /** @brief for subclass to do actual works */
    virtual void managerOnPause(void)
    {
    }
    /** @brief for subclass to do actual works */
    virtual void managerOnDestroy(void)
    {
    }

    // ============================================================
    // page life cycle
protected:
    /** @brief for subclass to do actual works */
    virtual void pageOnCreate(ZF_IN ZFUIPage *page,
                              ZF_IN const ZFUIPageRequestPageCreateParam &createParam)
    {
    }
    /** @brief for subclass to do actual works */
    virtual void pageOnResume(ZF_IN ZFUIPage *page,
                              ZF_IN ZFUIPageResumeReasonEnum reason)
    {
    }
    /**
     * @brief called to attach page to manager, before #pageOnResume
     */
    virtual void pageOnAttach(ZF_IN ZFUIPage *page,
                              ZF_IN ZFUIPageResumeReasonEnum reason)
    {
    }
    /**
     * @brief called to detach page from manager, after #pageOnPause
     */
    virtual void pageOnDetach(ZF_IN ZFUIPage *page,
                              ZF_IN ZFUIPagePauseReasonEnum reason)
    {
    }
    /**
     * @brief called to check whether needs to #pageOnDetach page from manager
     *
     * for managers that needs to keep more than one page being attached,
     * you may simply override this method and return false\n
     * page would ensured to be detached before destroy\n
     * \n
     * typical usage:
     * -  always return false, to ensure all page attached, useful for tab-based manager
     * -  return false if current page count greater than 2, useful to implement "swipe to go back"
     */
    virtual zfbool pageOnDetachCheckRequire(ZF_IN ZFUIPage *page,
                                            ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        return zftrue;
    }
    /** @brief for subclass to do actual works */
    virtual void pageOnPause(ZF_IN ZFUIPage *page,
                             ZF_IN ZFUIPagePauseReasonEnum reason)
    {
    }
    /** @brief for subclass to do actual works */
    virtual void pageOnDestroy(ZF_IN ZFUIPage *page)
    {
    }

    // ============================================================
    // page manager events
protected:
    /** @brief see #EventManagerAfterResume */
    virtual void managerAfterResume(void)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventManagerAfterResume());
    }
    /** @brief see #EventManagerBeforePause */
    virtual void managerBeforePause(void)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventManagerBeforePause());
    }

    // ============================================================
    // page events
protected:
    /** @brief see #EventPageAfterCreate */
    virtual void pageAfterCreate(ZF_IN ZFUIPage *page)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPageManager::EventPageAfterCreate(),
            zfnull,
            this->toObject());
    }
    /** @brief see #EventPageBeforeResume */
    virtual void pageBeforeResume(ZF_IN ZFUIPage *page, ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPageManager::EventPageBeforeResume(),
            zflineAllocWithoutLeakTest(ZFUIPageResumeReason, reason),
            this->toObject());
    }
    /** @brief see #EventPageAfterResume */
    virtual void pageAfterResume(ZF_IN ZFUIPage *page, ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPageManager::EventPageAfterResume(),
            zflineAllocWithoutLeakTest(ZFUIPageResumeReason, reason),
            this->toObject());
    }
    /** @brief see #EventPageBeforePause */
    virtual void pageBeforePause(ZF_IN ZFUIPage *page, ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPageManager::EventPageBeforePause(),
            zflineAllocWithoutLeakTest(ZFUIPagePauseReason, reason),
            this->toObject());
    }
    /** @brief see #EventPageAfterPause */
    virtual void pageAfterPause(ZF_IN ZFUIPage *page, ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPageManager::EventPageAfterPause(),
            zflineAllocWithoutLeakTest(ZFUIPagePauseReason, reason),
            this->toObject());
    }
    /** @brief see #EventPageBeforeDestroy */
    virtual void pageBeforeDestroy(ZF_IN ZFUIPage *page)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPageManager::EventPageBeforeDestroy(),
            zfnull,
            this->toObject());
    }

private:
    _ZFP_ZFUIPageManagerPrivate *d;
    friend zfclassFwd _ZFP_ZFUIPageManagerPrivate;
    friend zfclassFwd ZFUIPage;
protected:
    /** @cond ZFPrivateDoc */
    ZFUIPageManager(void)
    : d(zfnull)
    {
    }
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPageManager_h_

