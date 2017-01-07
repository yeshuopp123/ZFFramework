/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIViewStateAni.h"

#include "ZFCore/protocol/ZFProtocolZFThread.h"
#include "ZFCore/protocol/ZFProtocolZFThreadTaskRequest.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFUIViewStateAni_DEBUG 0

// ============================================================
ZFOBSERVER_EVENT_REGISTER(ZFUIViewStateAniImpl, StateAniOnInit)
ZFOBSERVER_EVENT_REGISTER(ZFUIViewStateAniImpl, StateAniOnDealloc)
ZFOBSERVER_EVENT_REGISTER(ZFUIViewStateAniImpl, StateAniViewAttach)
ZFOBSERVER_EVENT_REGISTER(ZFUIViewStateAniImpl, StateAniViewDetach)
ZFOBSERVER_EVENT_REGISTER(ZFUIViewStateAniImpl, StateAniViewAniPrepare)
ZFOBSERVER_EVENT_REGISTER(ZFUIViewStateAniImpl, StateAniViewAniStart)
ZFOBSERVER_EVENT_REGISTER(ZFUIViewStateAniImpl, StateAniViewAniStop)

// ============================================================
// ZFUIViewStateAniFilter
ZFFilterForZFObject ZFUIViewStateAniFilter;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniFilterDataHolder, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniFilterDataHolder)
{
    ZFUIViewStateAniFilter.filterRemoveAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniFilterDataHolder)

// ============================================================
zfclassLikePOD _ZFP_ZFUIViewStateAniTaskData
{
public:
    ZFUIView *view;
    zfautoObject ani;
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniDataHolder, ZFLevelZFFrameworkNormal)
{
    this->taskStarted = zffalse;
    this->delayStartTaskId = zfidentityInvalid;
    this->delayStartActionListener = ZFCallbackForRawFunction(zfself::delayStartAction);
    this->viewOnDetachListener = ZFCallbackForRawFunction(zfself::viewOnDetach);
    this->viewTaskOnStartListener = ZFCallbackForRawFunction(zfself::viewTaskOnStart);
    this->viewAniOnStopListener = ZFCallbackForRawFunction(zfself::viewAniOnStop);
}
public:
    zfbool taskStarted;
    zfstlmap<ZFUIView *, _ZFP_ZFUIViewStateAniTaskData> tasks;
    zfidentity delayStartTaskId;
    ZFListener delayStartActionListener;
    ZFCoreArrayPOD<ZFUIView *> delayStartTasks;
    ZFListener viewOnDetachListener;
    ZFListener viewTaskOnStartListener;
    ZFListener viewAniOnStopListener;
public:
    static ZFLISTENER_PROTOTYPE_EXPAND(delayStartAction)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniDataHolder);
        d->delayStart();
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(viewOnDetach)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniDataHolder);
        d->viewDetach(listenerData.sender->to<ZFUIView *>());
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(viewTaskOnStart)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniDataHolder);
        d->viewAniStart(listenerData.sender->to<ZFUIView *>());
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(viewAniOnStop)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniDataHolder);
        d->viewDetach(userData->to<ZFObjectHolder *>()->holdedObj);
        #if _ZFP_ZFUIViewStateAni_DEBUG
            zfLogTrimT() << zfLogCurTimeString() << zfText("[ZFUIViewStateAni]")
                << zfText("aniStop") << userData->to<ZFObjectHolder *>()->holdedObj << listenerData.sender;
        #endif
    }

public:
    void taskStart(void)
    {
        if(this->taskStarted)
        {
            return ;
        }
        this->taskStarted = zftrue;
        ZFObjectGlobalEventObserverAdd(ZFUIView::EventViewOnRemoveFromParent(), this->viewOnDetachListener);
        ZFObjectGlobalEventObserverAdd(ZFUIView::EventViewLayoutOnLayoutFinish(), this->viewTaskOnStartListener);
        ZFGlobalEventCenter::instance()->observerNotify(ZFUIViewStateAniImpl::EventStateAniOnInit());
    }
    void taskStop(void)
    {
        ZFObjectGlobalEventObserverRemove(ZFUIView::EventViewOnRemoveFromParent(), this->viewOnDetachListener);
        ZFObjectGlobalEventObserverRemove(ZFUIView::EventViewLayoutOnLayoutFinish(), this->viewTaskOnStartListener);
        this->taskStarted = zffalse;
        ZFGlobalEventCenter::instance()->observerNotify(ZFUIViewStateAniImpl::EventStateAniOnDealloc());
    }

public:
    void viewAttach(ZF_IN ZFUIView *view)
    {
        #if _ZFP_ZFUIViewStateAni_DEBUG
            zfLogTrimT() << zfLogCurTimeString() << zfText("[ZFUIViewStateAni]")
                << zfText("viewAttach") << view;
        #endif
        _ZFP_ZFUIViewStateAniTaskData taskData;
        taskData.view = view;
        this->tasks[view] = taskData;
        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(view, ZFUIViewStateAniImpl::EventStateAniViewAttach());
    }
    void viewDetach(ZF_IN ZFUIView *view)
    {
        if(!this->delayStartTasks.isEmpty())
        {
            this->delayStartTasks.removeElement(view);
            if(this->delayStartTasks.isEmpty())
            {
                this->delayStop();
            }
        }

        zfstlmap<ZFUIView *, _ZFP_ZFUIViewStateAniTaskData>::iterator it = this->tasks.find(view);
        if(it == this->tasks.end())
        {
            return ;
        }
        #if _ZFP_ZFUIViewStateAni_DEBUG
            zfLogTrimT() << zfLogCurTimeString() << zfText("[ZFUIViewStateAni]")
                << zfText("viewDetach") << view;
        #endif
        _ZFP_ZFUIViewStateAniTaskData taskData = it->second;
        this->tasks.erase(it);
        ZFAnimation *ani = taskData.ani;
        if(ani != zfnull)
        {
            ani->observerRemove(ZFAnimation::EventAniOnStopOrOnInvalid(), this->viewAniOnStopListener);
            ani->aniStop();
        }
        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(view, ZFUIViewStateAniImpl::EventStateAniViewDetach());

        if(this->tasks.empty())
        {
            this->taskStop();
        }
    }
    void viewAniStart(ZF_IN ZFUIView *view)
    {
        this->delayStartTasks.add(view);
        if(this->delayStartTaskId == zfidentityInvalid)
        {
            this->delayStartTaskId = ZFThreadTaskRequest(this->delayStartActionListener);
        }
    }
private:
    void delayStart(void)
    {
        while(!this->delayStartTasks.isEmpty())
        {
            this->viewAniDoStart(this->delayStartTasks.removeLastAndGet());
        }
        this->delayStartTaskId = zfidentityInvalid;
    }
    void delayStop(void)
    {
        ZFThreadTaskCancel(this->delayStartTaskId);
        this->delayStartTasks.removeAll();
        this->delayStartTaskId = zfidentityInvalid;
    }
    void viewAniDoStart(ZF_IN ZFUIView *view)
    {
        if(view->viewParent() == zfnull)
        {
            return ;
        }
        ZFUIView *parent = view;
        do
        {
            if(parent->objectIsPrivate()
                || !parent->viewVisible()
                || !ZFUIViewStateAniFilter.filterCheckActive(parent))
            {
                return ;
            }
            zfstlmap<ZFUIView *, _ZFP_ZFUIViewStateAniTaskData>::iterator it = this->tasks.find(parent);
            if(it != this->tasks.end() && it->second.ani != zfautoObjectNull)
            {
                return ;
            }
            parent = parent->viewParent();
        } while(parent != zfnull);

        zfstlmap<ZFUIView *, _ZFP_ZFUIViewStateAniTaskData>::iterator it = this->tasks.find(view);
        if(it == this->tasks.end())
        {
            this->viewAttach(view);
        }
        _ZFP_ZFUIViewStateAniTaskData &taskData = this->tasks[view];
        if(taskData.ani.toObject() != zfnull)
        {
            return ;
        }

        zfblockedAlloc(ZFArrayEditable, aniList);
        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(view, ZFUIViewStateAniImpl::EventStateAniViewAniPrepare(), aniList);
        #if _ZFP_ZFUIViewStateAni_DEBUG
            zfLogTrimT() << zfLogCurTimeString() << zfText("[ZFUIViewStateAni]")
                << zfText("aniPrepare") << view << view->layoutedFramePrev() << aniList;
        #endif

        if(aniList->isEmpty())
        {
            this->viewDetach(view);
            return ;
        }

        ZFAnimation *ani = ZFCastZFObject(ZFAnimation *, aniList->getFirst());
        zfCoreAssertWithMessage(ani != zfnull,
            zfText("%s not type of %s"),
            aniList->getFirst()->objectInfoOfInstance().cString(), ZFAnimation::ClassData()->className());
        taskData.ani = zfautoObjectCreate(ani);
        ani->observerAdd(ZFAnimation::EventAniOnStopOrOnInvalid(), this->viewAniOnStopListener, view->objectHolder());

        #if _ZFP_ZFUIViewStateAni_DEBUG
            zfLogTrimT() << zfLogCurTimeString() << zfText("[ZFUIViewStateAni]")
                << zfText("aniStart") << view << ani;
        #endif
        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(view, ZFUIViewStateAniImpl::EventStateAniViewAniStart(), ani);
        ani->aniStart();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniDataHolder)

// ============================================================
/*
 * some impl doesn't support animation during app startup,
 * since state ani is an optional module,
 * delay it to start is the most easy and convenient way to go through this problem
 */
static zfbool _ZFP_ZFUIViewStateAniStart_available = zffalse;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniStart_availableDelay, ZFLevelAppLow)
{
    this->delayTaskId = zfidentityInvalid;
    if(ZFPROTOCOL_IS_AVAILABLE(ZFThread))
    {
        ZFLISTENER_LOCAL(delayAction, {
            _ZFP_ZFUIViewStateAniStart_available = zftrue;
            userData->to<ZFTypeHolder *>()->holdedDataRef<zfidentity &>() = zfidentityInvalid;
        })
        this->delayTaskId = ZFThreadExecuteInMainThreadAfterDelay(200, delayAction,
            zflineAlloc(ZFTypeHolder, &this->delayTaskId, ZFTypeHolder::DeletePointerRef));
    }
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniStart_availableDelay)
{
    ZFThreadExecuteCancel(this->delayTaskId);
    _ZFP_ZFUIViewStateAniStart_available = zffalse;
}
private:
    zfidentity delayTaskId;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniStart_availableDelay)
void ZFUIViewStateAniStart(void)
{
    if(_ZFP_ZFUIViewStateAniStart_available
        && ZFPROTOCOL_IS_AVAILABLE(ZFThread)
        && ZFPROTOCOL_IS_AVAILABLE(ZFThreadTaskRequest))
    {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniDataHolder)->taskStart();
    }
}
zfbool ZFUIViewStateAniStarted(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniDataHolder)->taskStarted;
}

// ============================================================
ZFOBJECT_REGISTER(ZFUIViewStateAniImplProtocol)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniImplDataHolder, ZFLevelZFFrameworkNormal)
{
    this->started = zffalse;
    this->stateAniOnInitListener = ZFCallbackForRawFunction(zfself::stateAniOnInit);
    this->stateAniOnDeallocListener = ZFCallbackForRawFunction(zfself::stateAniOnDealloc);
    this->stateAniViewAttachListener = ZFCallbackForRawFunction(zfself::stateAniViewAttach);
    this->stateAniViewDetachListener = ZFCallbackForRawFunction(zfself::stateAniViewDetach);
    this->stateAniViewAniPrepareListener = ZFCallbackForRawFunction(zfself::stateAniViewAniPrepare);
    this->stateAniViewAniStartListener = ZFCallbackForRawFunction(zfself::stateAniViewAniStart);
    this->stateAniViewAniStopListener = ZFCallbackForRawFunction(zfself::stateAniViewAniStop);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniImplDataHolder)
{
    if(!this->implList.isEmpty())
    {
        ZFCoreArrayPOD<ZFUIViewStateAniImplProtocol *> implListTmp;
        implListTmp.copyFrom(this->implList);
        this->implList.removeAll();
        for(zfindex i = 0; i < implListTmp.count(); ++i)
        {
            zfRelease(implListTmp[i]);
        }
    }
}
public:
    ZFCoreArrayPOD<ZFUIViewStateAniImplProtocol *> implList;
    zfbool started;
    ZFListener stateAniOnInitListener;
    ZFListener stateAniOnDeallocListener;
    ZFListener stateAniViewAttachListener;
    ZFListener stateAniViewDetachListener;
    ZFListener stateAniViewAniPrepareListener;
    ZFListener stateAniViewAniStartListener;
    ZFListener stateAniViewAniStopListener;
public:
    static ZFLISTENER_PROTOTYPE_EXPAND(stateAniOnInit)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniImplDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniImplDataHolder);
        for(zfindex i = 0; i < d->implList.count(); ++i)
        {
            d->implList[i]->stateAniOnInit();
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(stateAniOnDealloc)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniImplDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniImplDataHolder);
        for(zfindex i = 0; i < d->implList.count(); ++i)
        {
            d->implList[i]->stateAniOnDealloc();
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(stateAniViewAttach)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniImplDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniImplDataHolder);
        for(zfindex i = 0; i < d->implList.count(); ++i)
        {
            d->implList[i]->stateAniViewAttach(listenerData.sender->to<ZFUIView *>());
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(stateAniViewDetach)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniImplDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniImplDataHolder);
        for(zfindex i = 0; i < d->implList.count(); ++i)
        {
            d->implList[i]->stateAniViewDetach(listenerData.sender->to<ZFUIView *>());
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(stateAniViewAniPrepare)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniImplDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniImplDataHolder);
        for(zfindex i = 0; i < d->implList.count(); ++i)
        {
            d->implList[i]->stateAniViewAniPrepare(listenerData.sender->to<ZFUIView *>(), listenerData.param0->to<ZFArrayEditable *>());
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(stateAniViewAniStart)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniImplDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniImplDataHolder);
        for(zfindex i = 0; i < d->implList.count(); ++i)
        {
            d->implList[i]->stateAniViewAniStart(listenerData.sender->to<ZFUIView *>(), listenerData.param0->to<ZFAnimation *>());
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(stateAniViewAniStop)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniImplDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniImplDataHolder);
        for(zfindex i = 0; i < d->implList.count(); ++i)
        {
            d->implList[i]->stateAniViewAniStop(listenerData.sender->to<ZFUIView *>(), listenerData.param0->to<ZFAnimation *>());
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniImplDataHolder)

void ZFUIViewStateAniImplRegister(ZF_IN ZFUIViewStateAniImplProtocol *impl)
{
    if(impl == zfnull)
    {
        return ;
    }

    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniImplDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniImplDataHolder);
    d->implList.add(zfRetain(impl));
    impl->toObject()->objectCachedSet(zftrue);
    if(d->implList.count() == 1)
    {
        ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniOnInit(), d->stateAniOnInitListener);
        ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniOnDealloc(), d->stateAniOnDeallocListener);
        ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniViewAttach(), d->stateAniViewAttachListener);
        ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniViewDetach(), d->stateAniViewDetachListener);
        ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniViewAniPrepare(), d->stateAniViewAniPrepareListener);
        ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniViewAniStart(), d->stateAniViewAniStartListener);
        ZFGlobalEventCenter::instance()->observerAdd(ZFUIViewStateAniImpl::EventStateAniViewAniStop(), d->stateAniViewAniStopListener);
    }
}
void ZFUIViewStateAniImplUnregister(ZF_IN ZFUIViewStateAniImplProtocol *impl)
{
    if(impl == zfnull)
    {
        return ;
    }

    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniImplDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniImplDataHolder);
    zfindex index = d->implList.find(impl);
    if(index == zfindexMax)
    {
        return ;
    }
    zfblockedRelease(impl);
    impl->toObject()->objectCachedSet(zffalse);
    d->implList.remove(index);

    if(d->implList.isEmpty())
    {
        ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniOnInit(), d->stateAniOnInitListener);
        ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniOnDealloc(), d->stateAniOnDeallocListener);
        ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniViewAttach(), d->stateAniViewAttachListener);
        ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniViewDetach(), d->stateAniViewDetachListener);
        ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniViewAniPrepare(), d->stateAniViewAniPrepareListener);
        ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniViewAniStart(), d->stateAniViewAniStartListener);
        ZFGlobalEventCenter::instance()->observerRemove(ZFUIViewStateAniImpl::EventStateAniViewAniStop(), d->stateAniViewAniStopListener);
    }
}

ZF_NAMESPACE_GLOBAL_END

