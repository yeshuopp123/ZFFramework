/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFVarConvertDef.h"
#include "zfautoObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFVarConvertHolderFwdFix(ZF_OUT zfautoObject &ret, ZF_IN ZFObject *from)
{
    /*
     * zfautoObjectCreate depends on leak test,
     * wrap it in this cpp file to solve recursive header dependency
     */
    ret = zfautoObjectCreate(from);
}

ZF_NAMESPACE_GLOBAL_END

