/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIPage.h"
#include "ZFUIPageManager.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIPageResumeReason)
ZFENUM_DEFINE(ZFUIPagePauseReason)

ZFOBJECT_REGISTER(ZFUIPage)

ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAfterCreate)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageBeforeResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAfterResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageBeforePause)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAfterPause)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageBeforeDestroy)

void ZFUIPage::pageDestroy(void)
{
    this->pageManager()->requestPageDestroy(this);
}

void ZFUIPage::pageDelayDetachOnCheck(ZF_IN ZFUIPagePauseReasonEnum reason)
{
    // nothing to do
}
void ZFUIPage::pageDelayDetachEnable(void)
{
    ++(this->_ZFP_ZFUIPage_pageDelayDetachFlag);
}
void ZFUIPage::pageDelayDetachNotifyFinish(void)
{
    --(this->_ZFP_ZFUIPage_pageDelayDetachFlag);
    if(this->_ZFP_ZFUIPage_pageDelayDetachFlag == 0)
    {
        this->pageManager()->_ZFP_ZFUIPageManager_pageDelayDetachNotifyFinish(this);
    }
}

void ZFUIPage::pageDelayDestroyOnCheck(void)
{
    // nothing to do
}
void ZFUIPage::pageDelayDestroyEnable(void)
{
    ++(this->_ZFP_ZFUIPage_pageDelayDestroyFlag);
}
void ZFUIPage::pageDelayDestroyNotifyFinish(void)
{
    --(this->_ZFP_ZFUIPage_pageDelayDestroyFlag);
    if(this->_ZFP_ZFUIPage_pageDelayDestroyFlag == 0)
    {
        this->pageManager()->_ZFP_ZFUIPageManager_pageDelayDestroyNotifyFinish(this);
    }
}

void ZFUIPage::_ZFP_ZFUIPage_pageCreate(void)
{
    zfCoreAssert(!this->_ZFP_ZFUIPage_pageCreated);
    this->_ZFP_ZFUIPage_pageCreated = zftrue;

    this->_ZFP_ZFUIPage_pageOnCreateCalled = zffalse;
    this->pageOnCreate();
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnCreateCalled, zfTextA("[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnCreate"));
}
void ZFUIPage::_ZFP_ZFUIPage_pageResume(ZF_IN ZFUIPageResumeReasonEnum reason)
{
    zfCoreAssert(!this->_ZFP_ZFUIPage_pageResumed);
    this->_ZFP_ZFUIPage_pageResumed = zftrue;

    this->_ZFP_ZFUIPage_pageOnResumeCalled = zffalse;
    this->pageOnResume(reason);
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnResumeCalled, zfTextA("[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnResume"));
}
void ZFUIPage::_ZFP_ZFUIPage_pageAttach(ZF_IN ZFUIPageResumeReasonEnum reason)
{
    zfCoreAssert(!this->_ZFP_ZFUIPage_pageAttached);
    this->_ZFP_ZFUIPage_pageAttached = zftrue;

    this->_ZFP_ZFUIPage_pageOnAttachCalled = zffalse;
    this->pageOnAttach(reason);
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnAttachCalled, zfTextA("[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnAttach"));
}
void ZFUIPage::_ZFP_ZFUIPage_pageDetach(ZF_IN ZFUIPagePauseReasonEnum reason)
{
    zfCoreAssert(this->_ZFP_ZFUIPage_pageAttached);
    this->_ZFP_ZFUIPage_pageAttached = zffalse;

    this->_ZFP_ZFUIPage_pageOnDetachCalled = zffalse;
    this->pageOnDetach(reason);
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnDetachCalled, zfTextA("[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnDetach"));
}
void ZFUIPage::_ZFP_ZFUIPage_pagePause(ZF_IN ZFUIPagePauseReasonEnum reason)
{
    zfCoreAssert(this->_ZFP_ZFUIPage_pageResumed);
    this->_ZFP_ZFUIPage_pageResumed = zffalse;

    this->_ZFP_ZFUIPage_pageOnPauseCalled = zffalse;
    this->pageOnPause(reason);
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnPauseCalled, zfTextA("[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnPause"));
}
void ZFUIPage::_ZFP_ZFUIPage_pageDestroy(void)
{
    zfCoreAssert(this->_ZFP_ZFUIPage_pageCreated);
    this->_ZFP_ZFUIPage_pageCreated = zffalse;

    this->_ZFP_ZFUIPage_pageOnDestroyCalled = zffalse;
    this->pageOnDestroy();
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnDestroyCalled, zfTextA("[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnDestroy"));

    ZFPropertyChange(this->_ZFP_ZFUIPage_pageCreateParam, zfnull);
}

ZF_NAMESPACE_GLOBAL_END

