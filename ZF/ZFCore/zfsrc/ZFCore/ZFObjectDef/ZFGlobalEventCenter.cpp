/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFGlobalEventCenter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFGlobalEventCenter)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFGlobalEventCenter, ZFGlobalEventCenter, ZFGlobalEventCenter, instance, ZFLevelZFFrameworkHigh)

ZF_NAMESPACE_GLOBAL_END

