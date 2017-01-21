/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFPropertyFwdDef.h
 * @brief property for ZFObject type
 */

#ifndef _ZFI_ZFPropertyFwdDef_h_
#define _ZFI_ZFPropertyFwdDef_h_

#include "ZFObjectClassTypeFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFProperty;
zfclassFwd ZFObject;
// ============================================================
/**
 * @brief used to check whether the property has been accessed
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackIsValueAccessed(propertyInfo, ownerObj);
 * @endcode
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyIsValueAccessed if necessary
 */
typedef zfbool (*ZFPropertyCallbackIsValueAccessed)(ZF_IN const ZFProperty *property,
                                                    ZF_IN ZFObject *ownerObj);
/** @brief change default impl for #ZFPropertyCallbackIsValueAccessed, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackIsValueAccessedChange(ZF_IN const ZFProperty *property,
                                                                  ZF_IN ZFPropertyCallbackIsValueAccessed callback);

/**
 * @brief used to check whether the property is in init value state
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackIsInitValue(propertyInfo, ownerObj, zfnull);
 * @endcode
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyIsInitValue if necessary
 * @note the outInitValue can be set to access the init value (only if the checker returned false)\n
 *   for assign property, it should be a pointer of your property type\n
 *   for retain property, it should be a pointer to #zfautoObject
 */
typedef zfbool (*ZFPropertyCallbackIsInitValue)(ZF_IN const ZFProperty *property,
                                                ZF_IN ZFObject *ownerObj,
                                                ZF_OUT_OPT void *outInitValue);
/** @brief change default impl for #ZFPropertyCallbackIsInitValue, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackIsInitValueChange(ZF_IN const ZFProperty *property,
                                                              ZF_IN ZFPropertyCallbackIsInitValue callback);

/**
 * @brief used to reset the property to its init value state
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackResetInitValue(propertyInfo, ownerObj);
 * @endcode
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyResetInitValue if necessary
 */
typedef void (*ZFPropertyCallbackResetInitValue)(ZF_IN const ZFProperty *property,
                                                 ZF_IN ZFObject *ownerObj);
/** @brief change default impl for #ZFPropertyCallbackResetInitValue, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackResetInitValueChange(ZF_IN const ZFProperty *property,
                                                                 ZF_IN ZFPropertyCallbackResetInitValue callback);

/**
 * @brief used to compare two object's property
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackCompare(propertyInfo, obj0, obj1);
 * @endcode
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyCompare if necessary
 */
typedef ZFCompareResult (*ZFPropertyCallbackCompare)(ZF_IN const ZFProperty *property,
                                                     ZF_IN ZFObject *obj0,
                                                     ZF_IN ZFObject *obj1);
/** @brief change default impl for #ZFPropertyCallbackCompare, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackCompareChange(ZF_IN const ZFProperty *property,
                                                          ZF_IN ZFPropertyCallbackCompare callback);

/**
 * @brief used to copy property from different object
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackCopy(propertyInfo, dstObj, srcObj);
 * @endcode
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyCopyAll if necessary
 */
typedef void (*ZFPropertyCallbackCopy)(ZF_IN const ZFProperty *property,
                                       ZF_IN ZFObject *dstObj,
                                       ZF_IN ZFObject *srcObj);
/** @brief change default impl for #ZFPropertyCallbackCopy, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackCopyChange(ZF_IN const ZFProperty *property,
                                                       ZF_IN ZFPropertyCallbackCopy callback);

/**
 * @brief used to set retain property without knowing the type
 *
 * do nothing if error
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyRetainSet if necessary
 */
typedef void (*ZFPropertyCallbackRetainSet)(ZF_IN const ZFProperty *property,
                                            ZF_IN ZFObject *dstObj,
                                            ZF_IN ZFObject *src);
/** @brief change default impl for #ZFPropertyCallbackRetainSet, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackRetainSetChange(ZF_IN const ZFProperty *property,
                                                            ZF_IN ZFPropertyCallbackRetainSet callback);

/**
 * @brief used to get retain property without knowing the type
 *
 * return null if error
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyRetainGet if necessary
 */
typedef ZFObject *(*ZFPropertyCallbackRetainGet)(ZF_IN const ZFProperty *property,
                                                 ZF_IN ZFObject *ownerObj);
/** @brief change default impl for #ZFPropertyCallbackRetainSet, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackRetainGetChange(ZF_IN const ZFProperty *property,
                                                            ZF_IN ZFPropertyCallbackRetainGet callback);

/**
 * @brief used to set assign property without knowing the type
 *
 * you must ensure the src's type is correct
 * (or safe and valid to cast to desired type by reinterpret pointer cast),
 * otherwise, data may be invalid and app may crash
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyAssignSet if necessary
 */
typedef void (*ZFPropertyCallbackAssignSet)(ZF_IN const ZFProperty *property,
                                            ZF_IN ZFObject *dstObj,
                                            ZF_IN void *src);
/** @brief change default impl for #ZFPropertyCallbackAssignSet, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackAssignSetChange(ZF_IN const ZFProperty *property,
                                                            ZF_IN ZFPropertyCallbackAssignSet callback);

/**
 * @brief used to get assign property without knowing the type
 *
 * return the property's address as (const void *) pointer,
 * or null if error
 */
typedef const void *(*ZFPropertyCallbackAssignGet)(ZF_IN const ZFProperty *property,
                                                   ZF_IN ZFObject *ownerObj);
/** @brief change default impl for #ZFPropertyCallbackAssignGet, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackAssignGetChange(ZF_IN const ZFProperty *property,
                                                            ZF_IN ZFPropertyCallbackAssignGet callback);

/**
 * @brief used to get info of the property without knowing the type,
 *   usually for debug use
 */
typedef void (*ZFPropertyCallbackGetInfo)(ZF_IN const ZFProperty *property,
                                          ZF_IN ZFObject *ownerObj,
                                          ZF_IN_OUT zfstring &ret);
/** @brief change default impl for #ZFPropertyCallbackGetInfo, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackGetInfoChange(ZF_IN const ZFProperty *property,
                                                          ZF_IN ZFPropertyCallbackGetInfo callback);

// ============================================================
/**
 * @brief shows that the property has no type and can not be used in advanced property copy operation,
 *   ensured an empty string
 */
#define ZFPropertyTypeId_none zfText("")

#define _ZFP_ZFPropertyTypeIdForRetainProperty zfText("_ZFP_ZFPropertyTypeIdForRetainProperty")

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyFwdDef_h_

