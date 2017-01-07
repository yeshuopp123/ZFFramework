/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFThreadTaskRequest.h"

#if ZF_ENV_sys_Qt

#include <QCoreApplication>
#include <QEvent>

#define _ZFP_ZFThreadTaskRequestImpl_sys_Qt_TaskEvent ((QEvent::Type)(QEvent::User + 4000))
class _ZFP_ZFThreadTaskRequestImpl_sys_Qt_TaskHolder : public QObject
{
    Q_OBJECT
public:
    void taskRequest(ZF_IN const ZFListener &runnable, ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        ZFListenerHolder *task = zfAllocWithoutLeakTest(ZFListenerHolder);
        task->runnable = runnable;
        task->listenerData.param0Set(param0);
        task->listenerData.param1Set(param1);
        this->tasks.queuePut(task);
        QCoreApplication::instance()->postEvent(this, new QEvent(_ZFP_ZFThreadTaskRequestImpl_sys_Qt_TaskEvent), Qt::LowEventPriority);
    }
    void cleanup(void)
    {
        this->tasks.removeAll();
        this->deleteLater();
    }

private:
    ZFCoreQueuePOD<ZFListenerHolder *> tasks;
public:
    virtual bool event(QEvent *event)
    {
        if(event->type() == _ZFP_ZFThreadTaskRequestImpl_sys_Qt_TaskEvent)
        {
            if(!this->tasks.isEmpty())
            {
                ZFListenerHolder *task = this->tasks.queueTake();
                task->runnableExecute();
                zfReleaseWithoutLeakTest(task);
            }
            return true;
        }
        return QObject::event(event);
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadTaskRequestImpl_sys_Qt_TaskHolderInstance, ZFLevelZFFrameworkHigh)
{
    this->taskHolder = new _ZFP_ZFThreadTaskRequestImpl_sys_Qt_TaskHolder();
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadTaskRequestImpl_sys_Qt_TaskHolderInstance)
{
    this->taskHolder->cleanup();
}
public:
    _ZFP_ZFThreadTaskRequestImpl_sys_Qt_TaskHolder *taskHolder;
ZF_GLOBAL_INITIALIZER_END(ZFThreadTaskRequestImpl_sys_Qt_TaskHolderInstance)

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadTaskRequestImpl_sys_Qt, ZFThreadTaskRequest, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt:QueuedConnection"))

public:
    virtual void taskRequest(ZF_IN const ZFListener &task,
                             ZF_IN ZFObject *param0,
                             ZF_IN ZFObject *param1)
    {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadTaskRequestImpl_sys_Qt_TaskHolderInstance)->taskHolder->taskRequest(task, param0, param1);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadTaskRequestImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFThreadTaskRequestImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFThreadTaskRequest_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

