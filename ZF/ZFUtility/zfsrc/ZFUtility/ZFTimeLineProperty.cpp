/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFTimeLineProperty.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFTimeLineProperty)

void ZFTimeLineProperty::progressOnUpdate(ZF_IN ZFObject *ownerObj,
                                          ZF_IN zffloat progress)
{
}
void ZFTimeLineProperty::stateOnSave(ZF_IN ZFObject *ownerObj)
{
}
void ZFTimeLineProperty::stateOnRestore(ZF_IN ZFObject *ownerObj)
{
}

ZF_NAMESPACE_GLOBAL_END

