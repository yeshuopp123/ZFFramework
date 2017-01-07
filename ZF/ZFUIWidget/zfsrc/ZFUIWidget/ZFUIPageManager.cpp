/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIPageManager.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassPOD _ZFP_ZFUIPageManagerPageDelayDetachData
{
public:
    ZFUIPage *page;
    ZFUIPagePauseReasonEnum reason;
};
zfclassNotPOD _ZFP_ZFUIPageManagerPrivate
{
public:
    ZFUIPageManager *pimplOwner;
    ZFCoreArrayPOD<ZFUIPage *> pageList;
    zfbool managerResumed;
    zfindex managerUIBlocked;
    zfindex requestBlocked;
    ZFCoreQueuePOD<ZFUIPageRequest *> requestQueue;
    zfbool requestRunning;
    ZFListener requestOnResolveListener;
    ZFCoreArrayPOD<_ZFP_ZFUIPageManagerPageDelayDetachData> pageDelayDetachList;
    ZFCoreArrayPOD<ZFUIPage *> pageDelayDestroyList;
    zfbool movePageFlag;

public:
    void requestDoPost(ZF_IN ZFObject *owner)
    {
        zfsynchronizedObject(owner);
        if(this->requestBlocked > 0 || this->requestQueue.isEmpty() || this->requestRunning)
        {
            return ;
        }
        this->requestRunning = zftrue;
        ZFThreadTaskRequest(
            this->requestOnResolveListener,
            zfHint("userData")owner,
            zfHint("param0")zfnull,
            zfHint("param1")zfnull,
            zfHint("owner")owner,
            ZFThreadTaskRequestMergeCallbackIgnoreNewTask);
    }
private:
    static ZFLISTENER_PROTOTYPE_EXPAND(requestOnResolve)
    {
        ZFUIPageManager *manager = userData->to<ZFUIPageManager *>();
        zfsynchronizedObject(manager->toObject());
        if(manager->d->requestBlocked > 0 || manager->d->requestQueue.isEmpty())
        {
            manager->d->requestRunning = zffalse;
            return ;
        }

        manager->requestBlockedSet(zftrue);
        ZFUIPageRequest *request = manager->d->requestQueue.queueTake();

        if(!request->requestResolved())
        {
            manager->requestOnResolvePrepare(request);
        }
        if(!request->requestResolved())
        {
            manager->requestOnResolve(request);
        }
        manager->managerUIBlockedSet(zffalse);
        zfCoreAssertWithMessageTrim(request->requestResolved(), zfTextA("[ZFUIPageManager] unresolved request: %s"), zfsCoreZ2A(request->objectInfo().cString()));
        manager->requestOnResolveFinish(request);
        ZFLeakTestLogBeforeReleaseVerbose(request, ZF_CALLER_FILE, zfTextA("requestOnResolve"), ZF_CALLER_LINE);
        zfReleaseWithoutLeakTest(request);
        manager->requestBlockedSet(zffalse);
        manager->d->requestRunning = zffalse;

        if(manager->d->requestQueue.isEmpty())
        {
            ZFUIPage *foregroundPage = manager->pageForeground();
            if(!foregroundPage->pageResumed())
            {
                manager->d->pageResume(foregroundPage, ZFUIPageResumeReason::e_ByManagerResume);
            }
            if(manager->d->requestQueue.isEmpty())
            {
                manager->requestOnResolveFinishAll();
            }
        }
        else
        {
            manager->d->requestDoPost(manager->toObject());
        }
    }

public:
    void pageCreate(ZF_IN ZFUIPage *page, ZF_IN const ZFUIPageRequestPageCreateParam &createParam)
    {
        page->pageCreateParamSet(createParam.pageCreateParam());

        page->_ZFP_ZFUIPage_pageCreate();
        this->pimplOwner->pageOnCreate(page, createParam);
        page->pageAfterCreate();
        this->pimplOwner->pageAfterCreate(page);
    }
    void pageResume(ZF_IN ZFUIPage *page, ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->pimplOwner->pageBeforeResume(page, reason);
        page->pageBeforeResume(reason);
        this->pageAttach(page, reason);
        page->_ZFP_ZFUIPage_pageResume(reason);
        this->pimplOwner->pageOnResume(page, reason);
        page->pageAfterResume(reason);
        this->pimplOwner->pageAfterResume(page, reason);
    }
    void pageAttach(ZF_IN ZFUIPage *page, ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        if(!page->pageAttached())
        {
            page->_ZFP_ZFUIPage_pageAttach(reason);
            this->pimplOwner->pageOnAttach(page, reason);
            this->pimplOwner->toObject()->observerNotifyWithCustomSender(
                page->toObject(),
                ZFUIPageManager::EventPageOnAttach(),
                zflineAllocWithoutLeakTest(ZFUIPageResumeReason, reason));
        }
    }
    void pageDoDetach(ZF_IN ZFUIPage *page, ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        if(page->pageAttached() && this->pimplOwner->pageOnDetachCheckRequire(page, reason))
        {
            this->pimplOwner->pageOnDetach(page, reason);
            page->_ZFP_ZFUIPage_pageDetach(reason);
            this->pimplOwner->toObject()->observerNotifyWithCustomSender(
                page->toObject(),
                ZFUIPageManager::EventPageOnDetach(),
                zflineAllocWithoutLeakTest(ZFUIPagePauseReason, reason));
        }
    }
    void pageDetach(ZF_IN ZFUIPage *page, ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        page->pageDelayDetachOnCheck(reason);
        this->pimplOwner->pageDelayDetachOnCheck(page, reason);
        if(page->_ZFP_ZFUIPage_pageDelayDetachFlag > 0 || page->_ZFP_ZFUIPage_pageDelayDetachByManagerFlag > 0)
        {
            _ZFP_ZFUIPageManagerPageDelayDetachData data;
            data.page = page;
            data.reason = reason;
            this->pageDelayDetachList.add(data);
        }
        else
        {
            this->pageDoDetach(page, reason);
        }
    }
    void pagePause(ZF_IN ZFUIPage *page, ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->pimplOwner->pageBeforePause(page, reason);
        page->pageBeforePause(reason);
        this->pimplOwner->pageOnPause(page, reason);
        page->_ZFP_ZFUIPage_pagePause(reason);
        this->pageDetach(page, reason);
        page->pageAfterPause(reason);
        this->pimplOwner->pageAfterPause(page, reason);
    }
    void pageDestroy(ZF_IN ZFUIPage *page)
    {
        zfCoreAssert(page->_ZFP_ZFUIPage_pageDelayDetachFlag == 0 && page->_ZFP_ZFUIPage_pageDelayDetachByManagerFlag == 0);
        if(page->pageAttached())
        {
            this->pimplOwner->pageOnDetach(page, ZFUIPagePauseReason::e_BeforeDestroy);
            page->_ZFP_ZFUIPage_pageDetach(ZFUIPagePauseReason::e_BeforeDestroy);
        }

        this->pimplOwner->pageBeforeDestroy(page);
        page->pageBeforeDestroy();
        this->pimplOwner->pageOnDestroy(page);
        page->_ZFP_ZFUIPage_pageDestroy();
    }

public:
    void pageDelayDetachCheckFinish(ZF_IN ZFUIPage *page)
    {
        if(page->_ZFP_ZFUIPage_pageDelayDetachFlag > 0 || page->_ZFP_ZFUIPage_pageDelayDetachByManagerFlag > 0)
        {
            return ;
        }
        for(zfindex i = 0; i < this->pageDelayDetachList.count(); ++i)
        {
            _ZFP_ZFUIPageManagerPageDelayDetachData data = this->pageDelayDetachList[i];
            if(data.page == page)
            {
                this->pageDelayDetachList.remove(i);
                this->pageDoDetach(data.page, data.reason);
                break;
            }
        }
    }
    void pageDelayDestroyCheckFinish(ZF_IN ZFUIPage *page)
    {
        if(page->_ZFP_ZFUIPage_pageDelayDestroyFlag > 0 || page->_ZFP_ZFUIPage_pageDelayDestroyByManagerFlag > 0)
        {
            return ;
        }
        zfindex index = this->pageDelayDestroyList.find(page);
        if(index == zfindexMax)
        {
            return ;
        }
        this->pageDelayDestroyList.remove(index);
        this->pageDestroy(page);
        ZFLeakTestLogBeforeReleaseVerbose(page, ZF_CALLER_FILE, zfText("pageDestroy"), ZF_CALLER_LINE);
        zfReleaseWithoutLeakTest(page);
    }

public:
    _ZFP_ZFUIPageManagerPrivate(void)
    : pimplOwner(zfnull)
    , pageList()
    , managerResumed(zffalse)
    , managerUIBlocked(0)
    , requestBlocked(0)
    , requestQueue()
    , requestRunning(zffalse)
    , requestOnResolveListener()
    , pageDelayDetachList()
    , pageDelayDestroyList()
    , movePageFlag(zffalse)
    {
        this->requestOnResolveListener = ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerPrivate::requestOnResolve);
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIPageManager)

ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerAfterResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerBeforePause)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerUIBlockedOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, RequestBlockedOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, RequestOnResolvePrepare)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, RequestOnResolve)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, RequestOnResolveFinish)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, RequestOnResolveFinishAll)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, PageAfterCreate)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, PageBeforeResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, PageAfterResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, PageOnAttach)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, PageOnDetach)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, PageBeforePause)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, PageAfterPause)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, PageBeforeDestroy)

// ============================================================
// embeded logic
void ZFUIPageManager::embededCreate(void)
{
    zfCoreAssert(d == zfnull);
    d = zfpoolNew(_ZFP_ZFUIPageManagerPrivate);
    d->pimplOwner = this;
    this->managerOnCreate();
}
void ZFUIPageManager::embededResume(void)
{
    zfCoreAssert(!d->managerResumed);
    d->managerResumed = zftrue;

    this->managerOnResume();

    if(!d->pageList.isEmpty())
    {
        ZFUIPage *foregroundPage = d->pageList.getLast();
        if(!foregroundPage->pageResumed())
        {
            d->pageResume(foregroundPage, ZFUIPageResumeReason::e_ByManagerResume);
        }
    }

    this->managerAfterResume();
}
void ZFUIPageManager::embededPause(void)
{
    zfCoreAssert(d->managerResumed);
    d->managerResumed = zffalse;

    this->managerBeforePause();

    if(!d->pageList.isEmpty())
    {
        ZFUIPage *foregroundPage = d->pageList.getLast();
        if(foregroundPage->pageResumed())
        {
            d->pagePause(foregroundPage, ZFUIPagePauseReason::e_ByManagerPause);
        }
    }
    this->managerOnPause();
}
void ZFUIPageManager::embededDestroy(void)
{
    zfCoreAssert(d != zfnull);
    zfCoreAssert(!d->managerResumed);
    zfCoreAssertWithMessage(d->pageDelayDestroyList.isEmpty(), zfTextA("you must not destroy the manager if there are pages delaying detach or destroy"));

    if(!d->pageList.isEmpty())
    {
        ZFCoreArrayPOD<ZFUIPage *> pageList;
        pageList.copyFrom(d->pageList);
        d->pageList.removeAll();
        for(zfindex i = pageList.count() - 1; i != zfindexMax; --i)
        {
            ZFUIPage *page = pageList[i];
            d->pageDestroy(page);
            zfRelease(page);
        }
    }

    if(!d->requestQueue.isEmpty())
    {
        ZFCoreArrayPOD<ZFUIPageRequest *> tmp;
        while(!d->requestQueue.isEmpty())
        {
            tmp.add(d->requestQueue.queueTake());
        }
        for(zfindex i = 0; i < tmp.count(); ++i)
        {
            this->managerUIBlockedSet(zffalse);
            zfRelease(tmp[i]);
        }
    }

    this->managerOnDestroy();

    ZFThreadTaskCancelWithOwner(this->toObject());

    zfpoolDelete(d);
}

// ============================================================
// manager control
void ZFUIPageManager::managerUIBlockedSet(ZF_IN zfbool value)
{
    zfsynchronizedObjectLock(this->toObject());
    if(value)
    {
        ++(d->managerUIBlocked);
        if(d->managerUIBlocked == 1)
        {
            zfsynchronizedObjectUnlock(this->toObject());
            this->managerUIBlockedOnChange();
        }
        else
        {
            zfsynchronizedObjectUnlock(this->toObject());
        }
    }
    else
    {
        --(d->managerUIBlocked);
        if(d->managerUIBlocked == 0)
        {
            zfsynchronizedObjectUnlock(this->toObject());
            this->managerUIBlockedOnChange();
        }
        else
        {
            zfsynchronizedObjectUnlock(this->toObject());
        }
    }
}
zfindex ZFUIPageManager::managerUIBlocked(void)
{
    return d->managerUIBlocked;
}

// ============================================================
// page access
zfindex ZFUIPageManager::pageCount(void)
{
    return d->pageList.count();
}
ZFCoreArrayPOD<ZFUIPage *> &ZFUIPageManager::pageList(void)
{
    return d->pageList;
}

// ============================================================
// page request
void ZFUIPageManager::requestPageCreate(ZF_IN const ZFUIPageRequestPageCreateParam &createParam)
{
    zfblockedAlloc(ZFUIPageRequestPageCreate, request);
    request->createParam = createParam;
    this->requestPost(request);
}
void ZFUIPageManager::requestPageCreate(ZF_IN const ZFClass *pageClass,
                                        ZF_IN_OPT ZFObject *pageCreateParam /* = zfnull */,
                                        ZF_IN_OPT zfbool pageAutoResume /* = zftrue */)
{
    if(pageClass == zfnull)
    {
        return ;
    }
    zfblockedAlloc(ZFUIPageRequestPageCreate, request);
    request->createParam
        .pageClassSet(pageClass)
        .pageCreateParamSet(pageCreateParam)
        .pageAutoResumeSet(pageAutoResume);
    this->requestPost(request);
}
void ZFUIPageManager::requestPageResume(ZF_IN ZFUIPage *page)
{
    if(page == zfnull)
    {
        return ;
    }
    zfblockedAlloc(ZFUIPageRequestPageResume, request);
    request->pageSet(page);
    this->requestPost(request);
}
void ZFUIPageManager::requestPageGroupResume(ZF_IN const zfchar *pageGroupId)
{
    zfblockedAlloc(ZFUIPageRequestPageGroupResume, request);
    request->pageGroupIdSet(zfstring(pageGroupId));
    this->requestPost(request);
}
void ZFUIPageManager::requestPageDestroy(ZF_IN ZFUIPage *page)
{
    if(page == zfnull)
    {
        return ;
    }
    zfblockedAlloc(ZFUIPageRequestPageDestroy, request);
    request->pageSet(page);
    this->requestPost(request);
}
void ZFUIPageManager::requestPost(ZF_IN ZFUIPageRequest *request)
{
    if(request != zfnull)
    {
        zfsynchronizedObject(this->toObject());
        this->managerUIBlockedSet(zftrue);
        d->requestQueue.queuePut(zfRetain(request));
        d->requestDoPost(this->toObject());
    }
}
void ZFUIPageManager::requestBlockedSet(ZF_IN zfbool value)
{
    zfsynchronizedObjectLock(this->toObject());
    if(value)
    {
        ++(d->requestBlocked);
        if(d->requestBlocked == 1)
        {
            zfsynchronizedObjectUnlock(this->toObject());
            this->requestBlockedOnChange();
        }
        else
        {
            zfsynchronizedObjectUnlock(this->toObject());
        }
    }
    else
    {
        --(d->requestBlocked);
        if(d->requestBlocked == 0)
        {
            zfsynchronizedObjectUnlock(this->toObject());
            this->requestBlockedOnChange();
            d->requestDoPost(this->toObject());
        }
        else
        {
            zfsynchronizedObjectUnlock(this->toObject());
        }
    }
}
zfindex ZFUIPageManager::requestBlocked(void)
{
    return d->requestBlocked;
}

// ============================================================
// request resolve
void ZFUIPageManager::requestOnResolve(ZF_IN ZFUIPageRequest *request)
{
    this->toObject()->observerNotify(ZFUIPageManager::EventRequestOnResolve(), this->toObject());
    this->toObject()->observerNotifyWithCustomSender(this->toObject(), ZFUIPageManager::EventRequestOnResolve(), this->toObject());

    if(request->requestResolved())
    {
        return ;
    }

    const ZFClass *cls = request->classData();
    if(cls->classIsSubclassOf(ZFUIPageRequestPageResume::ClassData()))
    {
        this->requestOnResolvePageResume(request->to<ZFUIPageRequestPageResume *>());
    }
    else if(cls->classIsSubclassOf(ZFUIPageRequestPageCreate::ClassData()))
    {
        this->requestOnResolvePageCreate(request->to<ZFUIPageRequestPageCreate *>());
    }
    else if(cls->classIsSubclassOf(ZFUIPageRequestPageDestroy::ClassData()))
    {
        this->requestOnResolvePageDestroy(request->to<ZFUIPageRequestPageDestroy *>());
    }
    else if(cls->classIsSubclassOf(ZFUIPageRequestPageGroupResume::ClassData()))
    {
        this->requestOnResolvePageGroupResume(request->to<ZFUIPageRequestPageGroupResume *>());
    }
    else if(cls->classIsSubclassOf(ZFUIPageRequestCustom::ClassData()))
    {
        this->requestOnResolveCustom(request->to<ZFUIPageRequestCustom *>());
    }
}
void ZFUIPageManager::requestOnResolvePageCreate(ZF_IN ZFUIPageRequestPageCreate *request)
{
    this->resolvePageCreate(request);
}
void ZFUIPageManager::resolvePageCreate(ZF_IN ZFUIPageRequestPageCreate *request)
{
    const ZFUIPageRequestPageCreateParam &createParam = request->createParam;
    zfCoreAssertWithMessageTrim(createParam.pageClass() != zfnull && createParam.pageClass()->classIsTypeOf(ZFUIPage::ClassData()),
        zfTextA("[ZFUIPageManager] pageClass %s not type of %s"),
        zfsCoreZ2A((createParam.pageClass() == zfnull) ? ZFTOKEN_zfnull : createParam.pageClass()->objectInfo().cString()),
        zfsCoreZ2A(ZFUIPage::ClassData()->objectInfo().cString()));

    zfautoObject pageObj = createParam.pageClass()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    ZFUIPage *page = pageObj.to<ZFUIPage *>();
    page->_ZFP_ZFUIPage_pageManager = this;
    zfRetain(page);
    d->pageList.add(0, page);

    d->pageCreate(page, createParam);

    if(createParam.pageAutoResume())
    {
        this->requestPageResume(page);
    }

    request->requestResolvedSet(zftrue);
}
void ZFUIPageManager::requestOnResolvePageResume(ZF_IN ZFUIPageRequestPageResume *request)
{
    this->resolvePageResume(request);
}
void ZFUIPageManager::resolvePageResume(ZF_IN ZFUIPageRequestPageResume *request)
{
    ZFUIPage *page = request->page();
    zfindex index = d->pageList.find(page);
    if(index == zfindexMax || page->pageResumed())
    {
        return ;
    }
    if(index != d->pageList.count() - 1)
    {
        d->pageList.remove(index);
        d->pageList.add(page);
    }
    if(d->pageList.count() >= 2)
    {
        ZFUIPage *old = d->pageList[d->pageList.count() - 2];
        if(old->pageResumed())
        {
            d->pagePause(old, ZFUIPagePauseReason::e_ToBackground);
        }
    }
    d->pageResume(page, ZFUIPageResumeReason::e_ByRequest);

    request->requestResolvedSet(zftrue);
}
void ZFUIPageManager::requestOnResolvePageGroupResume(ZF_IN ZFUIPageRequestPageGroupResume *request)
{
    this->resolvePageGroupResume(request);
}
void ZFUIPageManager::resolvePageGroupResume(ZF_IN ZFUIPageRequestPageGroupResume *request)
{
    const zfchar *pageGroupId = request->pageGroupId();
    this->movePageBegin();
    ZFCoreArrayPOD<ZFUIPage *> &pageList = this->pageList();
    ZFCoreArrayPOD<ZFUIPage *> pageListTmp;
    for(zfindex i = 0; i < pageList.count(); ++i)
    {
        ZFUIPage *page = pageList[i];
        if(page->pageGroupId().compare(pageGroupId) == 0)
        {
            pageList.remove(i);
            --i;
            pageListTmp.add(page);
        }
    }
    pageList.addFrom(pageListTmp);
    this->movePageEnd();

    request->requestResolvedSet(zftrue);
}
void ZFUIPageManager::requestOnResolvePageDestroy(ZF_IN ZFUIPageRequestPageDestroy *request)
{
    this->resolvePageDestroy(request);
}
void ZFUIPageManager::resolvePageDestroy(ZF_IN ZFUIPageRequestPageDestroy *request)
{
    ZFUIPage *page = request->page();
    zfindex index = d->pageList.find(page);
    if(index == zfindexMax)
    {
        return ;
    }
    if(index == d->pageList.count() - 1)
    {
        d->pageList.remove(index);
        if(page->pageResumed())
        {
            d->pagePause(page, ZFUIPagePauseReason::e_BeforeDestroy);
        }

        if(!d->pageList.isEmpty())
        {
            ZFUIPage *foregroundPage = d->pageList.getLast();
            if(!foregroundPage->pageResumed())
            {
                d->pageResume(foregroundPage, ZFUIPageResumeReason::e_FromBackground);
            }
        }

        page->pageDelayDestroyOnCheck();
        this->pageDelayDestroyOnCheck(page);
        if(page->_ZFP_ZFUIPage_pageDelayDestroyFlag > 0 || page->_ZFP_ZFUIPage_pageDelayDestroyByManagerFlag > 0)
        {
            d->pageDelayDestroyList.add(page);
        }
        else
        {
            d->pageDestroy(page);
            zfRelease(page);
        }
    }
    else
    {
        d->pageList.remove(index);

        if(!d->pageList.isEmpty())
        {
            ZFUIPage *foregroundPage = d->pageList.getLast();
            if(!foregroundPage->pageResumed())
            {
                d->pageResume(foregroundPage, ZFUIPageResumeReason::e_FromBackground);
            }
        }

        d->pageDestroy(page);
        zfRelease(page);
    }

    request->requestResolvedSet(zftrue);
}

void ZFUIPageManager::requestOnResolveCustom(ZF_IN ZFUIPageRequestCustom *request)
{
    this->resolveCustom(request);
}
void ZFUIPageManager::resolveCustom(ZF_IN ZFUIPageRequestCustom *request)
{
    request->listener().execute(
        ZFListenerData(zfidentityInvalid, this->toObject(), request->param0(), request->param1()),
        request->userData());

    request->requestResolvedSet(zftrue);
}

// ============================================================
// pageDelayDetach
void ZFUIPageManager::pageDelayDetachOnCheck(ZF_IN ZFUIPage *page,
                                             ZF_IN ZFUIPagePauseReasonEnum reason)
{
    // nothing to do
}
void ZFUIPageManager::pageDelayDetachEnable(ZF_IN ZFUIPage *page)
{
    ++(page->_ZFP_ZFUIPage_pageDelayDetachByManagerFlag);
    this->pageDelayDestroyEnable(page);
}
void ZFUIPageManager::pageDelayDetachNotifyFinish(ZF_IN ZFUIPage *page)
{
    --(page->_ZFP_ZFUIPage_pageDelayDetachByManagerFlag);
    d->pageDelayDetachCheckFinish(page);
    this->pageDelayDestroyNotifyFinish(page);
}
void ZFUIPageManager::_ZFP_ZFUIPageManager_pageDelayDetachNotifyFinish(ZF_IN ZFUIPage *page)
{
    d->pageDelayDetachCheckFinish(page);
    this->pageDelayDestroyNotifyFinish(page);
}

// ============================================================
// pageDelayDestroy
void ZFUIPageManager::pageDelayDestroyOnCheck(ZF_IN ZFUIPage *page)
{
    // nothing to do
}
void ZFUIPageManager::pageDelayDestroyEnable(ZF_IN ZFUIPage *page)
{
    ++(page->_ZFP_ZFUIPage_pageDelayDestroyByManagerFlag);
}
void ZFUIPageManager::pageDelayDestroyNotifyFinish(ZF_IN ZFUIPage *page)
{
    --(page->_ZFP_ZFUIPage_pageDelayDestroyByManagerFlag);
    d->pageDelayDestroyCheckFinish(page);
}
void ZFUIPageManager::_ZFP_ZFUIPageManager_pageDelayDestroyNotifyFinish(ZF_IN ZFUIPage *page)
{
    d->pageDelayDestroyCheckFinish(page);
}

// ============================================================
// page move
void ZFUIPageManager::movePageBegin(void)
{
    zfCoreAssertWithMessageTrim(!d->movePageFlag, zfTextA("[ZFUIPageManager] move page not paired, have you forgot movePageEnd?"));
    d->movePageFlag = zftrue;
}
void ZFUIPageManager::movePageEnd(void)
{
    zfCoreAssertWithMessageTrim(d->movePageFlag, zfTextA("[ZFUIPageManager] move page not paired, have you forgot movePageBegin?"));
    d->movePageFlag = zffalse;

    for(zfindex i = d->pageList.count() - 1; i != zfindexMax; --i)
    {
        ZFUIPage *page = d->pageList[i];
        if(page->pageResumed())
        {
            if(i != d->pageList.count() - 1)
            {
                d->pagePause(page, ZFUIPagePauseReason::e_ToBackground);
            }
            break;
        }
    }
    if(!d->pageList.isEmpty())
    {
        ZFUIPage *page = d->pageList.getLast();
        if(!page->pageResumed())
        {
            d->pageResume(page, ZFUIPageResumeReason::e_ByRequest);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

