/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFThreadTaskRequest.h"
#include "ZFCore/ZFThread.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadTaskRequestImpl_default, ZFThreadTaskRequest, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("ZFFramework:ZFThread"))

public:
    virtual void taskRequest(ZF_IN const ZFListener &task,
                             ZF_IN ZFObject *param0,
                             ZF_IN ZFObject *param1)
    {
        ZFLISTENER_LOCAL(threadCallback, {
            ZFLISTENER_LOCAL(mainThreadCallback, {
                ZFListenerHolder *listenerHolder = ZFCastZFObjectUnchecked(ZFListenerHolder *, userData);
                listenerHolder->runnableExecute();
            })
            ZFThreadExecuteInMainThread(mainThreadCallback, userData);
        })
        ZFThreadExecuteInNewThread(threadCallback,
            zflineAllocWithoutLeakTest(ZFListenerHolder, task, ZFListenerData(zfidentityInvalid, zfnull, param0, param1)));
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadTaskRequestImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFThreadTaskRequestImpl_default)

ZF_NAMESPACE_GLOBAL_END

