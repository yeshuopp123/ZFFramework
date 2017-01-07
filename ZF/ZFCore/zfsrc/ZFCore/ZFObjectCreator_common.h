/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectCreator_common.h
 * @brief see #ZFOBJECT_CREATOR_DEFINE
 */

#ifndef _ZFI_ZFObjectCreator_common_h_
#define _ZFI_ZFObjectCreator_common_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFOBJECT_CREATOR_DEFINE
 *
 * data is a string containing class name and method name,
 * which should be like this:\n
 *   "YourClass::YourMethod"\n
 * \n
 * YourClass must be a ZFObject or ZFInterface type (accessed by #ZFClass::classForName)\n
 * YourMethod must be a #ZFMethod with this proto type:\n
 *   static zfautoObject yourMethod(void);\n
 * no method id supported
 */
#define ZFObjectCreatorType_ZFMethod memberMethod
/** @brief see #ZFObjectCreatorType_ZFMethod */
#define ZFObjectCreatorTypeName_ZFMethod ZFM_TOSTRING(ZFObjectCreatorType_ZFMethod)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectCreator_common_h_

