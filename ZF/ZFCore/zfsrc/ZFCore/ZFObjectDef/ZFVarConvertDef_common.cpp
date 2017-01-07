/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFVarConvertDef_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFVarConvertBase)

// ============================================================
// basic type
ZFVAR_CONVERT_WRAPPER_DEFINE(zfbool)
ZFVAR_CONVERT_WRAPPER_DEFINE(zfchar)

// ============================================================
// string type
ZFVAR_CONVERT_WRAPPER_DEFINE(zfstring)

// ============================================================
// number type
ZFVAR_CONVERT_WRAPPER_DEFINE(zfnumber)

// ============================================================
// core type
ZFVAR_CONVERT_WRAPPER_DEFINE(ZFLevel)
ZFVAR_CONVERT_WRAPPER_DEFINE(ZFCompareResult)
ZFVAR_CONVERT_WRAPPER_DEFINE(ZFSeekPos)
ZFVAR_CONVERT_WRAPPER_DEFINE(zfindexRange)
ZFVAR_CONVERT_WRAPPER_DEFINE(ZFFilterType)
ZFVAR_CONVERT_WRAPPER_DEFINE(ZFFilterCallbackResult)
ZFVAR_CONVERT_WRAPPER_DEFINE(ZFCallbackType)
ZFVAR_CONVERT_WRAPPER_DEFINE(ZFClassFilterType)
ZFVAR_CONVERT_WRAPPER_DEFINE(ZFMethodPrivilegeType)

ZF_NAMESPACE_GLOBAL_END

