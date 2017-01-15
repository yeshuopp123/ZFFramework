/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFMethodSerializableDef.h
 * @brief serialization for ZFMethod
 */

#ifndef _ZFI_ZFMethodSerializableDef_h_
#define _ZFI_ZFMethodSerializableDef_h_

#include "ZFMethodDef.h"
#include "ZFSerializableDef.h"
#include "ZFPropertyTypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFMethod owner="OwnerClassName" method="MethodName" methodId="methodId" /> // methodId is optional
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFMethod, const ZFMethod *)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFMethod_owner zfText("owner")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFMethod_method zfText("method")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFMethod_methodId zfText("methodId")

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodSerializableDef_h_

