/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFIdMapDef.h
 * @brief id to string map util, useful to define extendable string enum
 */

#ifndef _ZFI_ZFIdMapDef_h_
#define _ZFI_ZFIdMapDef_h_

#include "../ZFCoreDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
extern ZF_ENV_EXPORT const zfidentity *_ZFP_ZFIdMapRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN const zfchar *moduleName, ZF_IN const zfchar *idName);
extern ZF_ENV_EXPORT void _ZFP_ZFIdMapUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN const zfchar *moduleName, ZF_IN zfidentity idValue);
/**
 * @brief see #ZFIDMAP
 *
 * get id name from id value, or null if no such id
 * @note can be found only if accessed or registered by #ZFIDMAP_REGISTER
 */
extern ZF_ENV_EXPORT const zfchar *ZFIdMapGetName(ZF_IN const zfchar *moduleName, ZF_IN const zfidentity &idValue);
/**
 * @brief see #ZFIDMAP
 *
 * get id value from id name, or zfidentityInvalid if no such id name
 * @note can be found only if accessed or registered by #ZFIDMAP_REGISTER
 */
extern ZF_ENV_EXPORT zfidentity ZFIdMapGetId(ZF_IN const zfchar *moduleName, ZF_IN const zfchar *idName);

/**
 * @brief see #ZFIDMAP
 *
 * get all registered id data, for debug use only
 * @note can be found only if accessed or registered by #ZFIDMAP_REGISTER
 */
extern ZF_ENV_EXPORT void ZFIdMapGetAll(ZF_IN const zfchar *moduleName, ZF_OUT ZFCoreArrayPOD<zfidentity> &idValues, ZF_OUT ZFCoreArrayPOD<const zfchar *> &idNames);

zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFIdMapUnregisterHolder
{
public:
    _ZFP_ZFIdMapUnregisterHolder(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN const zfchar *moduleName, ZF_IN zfidentity idValue)
    : ZFCoreLibDestroyFlag(ZFCoreLibDestroyFlag)
    , moduleName(moduleName)
    , idValue(idValue)
    {
    }
    ~_ZFP_ZFIdMapUnregisterHolder(void)
    {
        if(!*(this->ZFCoreLibDestroyFlag))
        {
            _ZFP_ZFIdMapUnregister(this->ZFCoreLibDestroyFlag, this->moduleName, this->idValue);
        }
    }

private:
    zfbool *ZFCoreLibDestroyFlag;
    zfstring moduleName;
    zfidentity idValue;
};
/**
 * @brief declare an id within ZFObject class scope
 *
 * usage:
 * @code
 *   zfclass YourClass ...
 *   {
 *       ZFIDMAP(YourModuleName, YourSth)
 *   };
 *
 *   // optional, in cpp files only, required only if you need ZFIdMapGetId/ZFIdMapGetName
 *   ZFIDMAP_REGISTER(YourModuleName, YourClass, YourSth)
 * @endcode
 * declared id can be accessed by:
 * @code
 *   zfidentity idValue = YourClass::IdYourSth();
 *   const zfchar *idName = YourClass::IdYourSth_name();
 * @endcode
 * note that subclass may declare an id same as parent,
 * while the final id name is different:\n
 *   ParentClass::IdYourSth() => "ParentClass::IdYourSth"\n
 *   ChildClass::IdYourSth() => "ChildClass::IdYourSth"\n
 * \n
 * @note we declare the id as int types for performance,
 *   it's ensured each id has different value,
 *   but it's only ensured while app is running,
 *   after relaunching the app,
 *   the id value is not ensured the same,
 *   you should use the name of the id to store or pass between apps,
 *   and you can use #ZFIdMapGetId or #ZFIdMapGetName
 *   to convert them easily
 */
#define ZFIDMAP(YourModuleName, YourIdName) \
    ZFIDMAP_DETAIL(YourModuleName, Id, YourIdName)
/** @brief see #ZFIDMAP */
#define ZFIDMAP_DETAIL(YourModuleName, prefix, YourIdName) \
    public: \
        /** \n */ \
        static zfidentity prefix##YourIdName(void) \
        { \
            static zfbool ZFCoreLibDestroyFlag = zffalse; \
            static const zfidentity *idValue = _ZFP_ZFIdMapRegister( \
                &ZFCoreLibDestroyFlag, ZFM_TOSTRING(YourModuleName), zfself::prefix##YourIdName##_name()); \
            static _ZFP_ZFIdMapUnregisterHolder unregisterHolder(&ZFCoreLibDestroyFlag, ZFM_TOSTRING(YourModuleName), *idValue); \
            return *idValue; \
        } \
        /** @brief see @ref prefix##YourIdName */ \
        static const zfchar *prefix##YourIdName##_name(void) \
        { \
            static zfstring _s = zfsConnectLineFree(zfself::ClassData()->className(), zfText("::") zfText(#prefix) zfText(#YourIdName)); \
            return _s.cString(); \
        }

/**
 * @brief declare an id in global scope, see #ZFIDMAP
 *
 * usage:
 * @code
 *   // in header files
 *   ZF_NAMESPACE_BEGIN(YourNamespace)
 *   / ** @brief you can add doxygen docs here * /
 *   ZFIDMAP_GLOBAL(YourModuleName, YourNamespace, YourSth)
 *   ZF_NAMESPACE_END(YourNamespace)
 *
 *   // optional, in cpp files only, required only if you need ZFIdMapGetId/ZFIdMapGetName
 *   ZFIDMAP_REGISTER(YourModuleName, YourNamespace, YourSth)
 *
 *   // use the id
 *   zfidentity idValue = YourNamespace::IdYourSth();
 * @endcode
 * unlike #ZFIDMAP, this macro would declare id outside of class scope
 */
#define ZFIDMAP_GLOBAL(YourModuleName, YourNamespace, YourIdName) \
    ZFIDMAP_GLOBAL_DETAIL(YourModuleName, YourNamespace, Id, YourIdName)
/** @brief see #ZFIDMAP_GLOBAL */
#define ZFIDMAP_GLOBAL_DETAIL(YourModuleName, GlobalNamespace, prefix, YourIdName) \
    /** @cond ZFPrivateDoc */ \
    zfclass ZF_ENV_EXPORT _ZFP_ZFIdMapHolder_##YourModuleName##_##GlobalNamespace##_##prefix##_##YourIdName \
    { \
    public: \
        static zfidentity idValue(void) \
        { \
            static zfbool ZFCoreLibDestroyFlag = zffalse; \
            static const zfidentity *idValue = _ZFP_ZFIdMapRegister( \
                &ZFCoreLibDestroyFlag, \
                ZFM_TOSTRING(YourModuleName), \
                _ZFP_ZFIdMapHolder_##YourModuleName##_##GlobalNamespace##_##prefix##_##YourIdName::idName()); \
            static _ZFP_ZFIdMapUnregisterHolder unregisterHolder(&ZFCoreLibDestroyFlag, ZFM_TOSTRING(YourModuleName), *idValue); \
            return *idValue; \
        } \
        static const zfchar *idName(void) \
        { \
            return zfText(#GlobalNamespace) zfText("::") zfText(#prefix) zfText(#YourIdName); \
        } \
    }; \
    /** @endcond */ \
    /** \n see #ZFIDMAP_GLOBAL */ \
    inline zfidentity prefix##YourIdName(void) \
    { \
        return _ZFP_ZFIdMapHolder_##YourModuleName##_##GlobalNamespace##_##prefix##_##YourIdName::idValue(); \
    } \
    /** @brief see @ref prefix##YourIdName, @ref ZFIDMAP_GLOBAL */ \
    inline const zfchar *prefix##YourIdName##_name(void) \
    { \
        return _ZFP_ZFIdMapHolder_##YourModuleName##_##GlobalNamespace##_##prefix##_##YourIdName::idName(); \
    }

/** @brief see #ZFIDMAP */
#define ZFIDMAP_REGISTER(YourModuleName, Scope, YourIdName) \
    ZFIDMAP_DETAIL_REGISTER(YourModuleName, Scope, Id, YourIdName)
/** @brief see #ZFIDMAP */
#define ZFIDMAP_DETAIL_REGISTER(YourModuleName, Scope, prefix, YourIdName) \
    ZF_STATIC_INITIALIZER_INIT(ZFIdMap_##YourModuleName##_##Scope##_##YourIdName) \
    { \
        (void)Scope::prefix##YourIdName(); \
    } \
    ZF_STATIC_INITIALIZER_END(ZFIdMap_##YourModuleName##_##Scope##_##YourIdName)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIdMapDef_h_

