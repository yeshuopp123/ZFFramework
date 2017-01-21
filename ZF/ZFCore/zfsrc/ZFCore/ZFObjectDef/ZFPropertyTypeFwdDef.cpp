/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPropertyTypeFwdDef.h"
#include "ZFPropertyTypeDef.h"
#include "zfautoObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFPropertyTypeIdData_zfautoObjectCreate(ZF_OUT zfautoObject &ret, ZF_IN ZFObject *obj)
{
    ret = zfautoObjectCreate(obj);
}

ZF_NAMESPACE_GLOBAL_END

