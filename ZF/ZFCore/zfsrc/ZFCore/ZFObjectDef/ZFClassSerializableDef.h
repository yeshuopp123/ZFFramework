/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFClassSerializableDef.h
 * @brief serialization for ZFClass
 */

#ifndef _ZFI_ZFClassSerializableDef_h_
#define _ZFI_ZFClassSerializableDef_h_

#include "ZFClassDef.h"
#include "ZFSerializableDef.h"
#include "ZFPropertyTypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFClass className="ClassName">
 *   </ZFClass>
 * @endcode
 */
#define ZFPropertyTypeId_ZFClass zfText("ZFClass")
ZFPROPERTY_TYPE_DECLARE(ZFClass, const ZFClass *)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFClass_className zfText("className")

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFClassSerializableDef_h_

