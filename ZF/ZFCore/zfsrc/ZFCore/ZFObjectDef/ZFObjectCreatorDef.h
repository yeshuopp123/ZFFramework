/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectCreatorDef.h
 * @brief serailizable way to create a ZFObject
 */

#ifndef _ZFI_ZFObjectCreatorDef_h_
#define _ZFI_ZFObjectCreatorDef_h_

#include "ZFObjectDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief function type to create an object type
 */
typedef zfautoObject (*ZFObjectCreator)(void);

/**
 * @brief create object from a descriptor, see #ZFOBJECT_CREATOR_DEFINE
 */
extern ZF_ENV_EXPORT zfautoObject ZFObjectCreate(ZF_IN const zfchar *type,
                                                 ZF_IN const zfchar *data);

/**
 * @brief usually for debug use only
 *
 * macro names are recommended to be ZFObjectCreatorType_XXX
 */
extern ZF_ENV_EXPORT void ZFObjectCreatorGetAllType(ZF_OUT ZFCoreArray<const zfchar *> &ret);
/** @brief see #ZFObjectCreatorGetAllType */
inline ZFCoreArrayPOD<const zfchar *> ZFObjectCreatorGetAllType(void)
{
    ZFCoreArrayPOD<const zfchar *> ret;
    ZFObjectCreatorGetAllType(ret);
    return ret;
}

typedef zfautoObject (*_ZFP_ZFObjectCreatorCallback)(ZF_IN const zfchar *data);
extern ZF_ENV_EXPORT void _ZFP_ZFObjectCreatorRegister(ZF_IN const zfchar *type,
                                                       ZF_IN _ZFP_ZFObjectCreatorCallback creatorCallback);
extern ZF_ENV_EXPORT void _ZFP_ZFObjectCreatorUnregister(ZF_IN const zfchar *type);

#define _ZFP_ZFOBJECT_CREATOR_DEFINE(typeName, data_) \
    static zfautoObject _ZFP_ZFObjectCreator_##typeName(ZF_IN const zfchar *data_); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObjectCreator_##typeName, ZFLevelZFFrameworkNormal) \
    { \
        _ZFP_ZFObjectCreatorRegister(#typeName, _ZFP_ZFObjectCreator_##typeName); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFObjectCreator_##typeName) \
    { \
        _ZFP_ZFObjectCreatorUnregister(#typeName); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFObjectCreator_##typeName) \
    static zfautoObject _ZFP_ZFObjectCreator_##typeName(ZF_IN const zfchar *data_)
/**
 * @brief macro to define and register a creator for #ZFObjectCreate, for advance user only
 *
 * usage:
 * @code
 *   ZFOBJECT_CREATOR_DEFINE(yourType, data)
 *   {
 *       // decode from data and return a newly created object
 *       return yourObject;
 *   }
 * @endcode
 */
#define ZFOBJECT_CREATOR_DEFINE(typeName, data_) \
    _ZFP_ZFOBJECT_CREATOR_DEFINE(typeName, data_)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectCreatorDef_h_

