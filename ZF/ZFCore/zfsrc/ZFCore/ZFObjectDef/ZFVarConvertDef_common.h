/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFVarConvertDef_common.h
 * @brief common types for #ZFVAR_CONVERT_DECLARE
 */

#ifndef _ZFI_ZFVarConvertDef_common_h_
#define _ZFI_ZFVarConvertDef_common_h_

#include "ZFVarConvertDef.h"
#include "ZFObjectDef.h"
#include "zfautoObjectDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief dummy base class defined for #ZFVAR_CONVERT_DECLARE and #ZFVAR_CONVERT_DECLARE_BY_WRAPPER
 */
zfclass ZF_ENV_EXPORT ZFVarConvertBase : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFVarConvertBase, ZFObject)
};

/**
 * @brief util macro to declare wrapper class for #ZFVAR_CONVERT_DECLARE
 *
 * the following wrapper class would be declared and used to achieve the wrap
 * @code
 *   zfclass ZF_ENV_EXPORT v_MyTypeName : zfextends ZFVarConvertBase
 *   {
 *       ZFOBJECT_DECLARE(v_MyTypeName, ZFVarConvertBase)
 *   public:
 *       MyType value;
 *   public:
 *       zfoverride
 *       virtual ZFObject *objectOnInit(void) {return zfsuper::objectOnInit();}
 *       zfoverride
 *       virtual ZFObject *objectOnInit(ZF_IN MyType const &v) {this->objectOnInit(); this->value = v; return this;}
 *   };
 * @endcode
 */
#define ZFVAR_CONVERT_WRAPPER_DECLARE(Type, TypeName) \
    /** @brief see #ZFVAR_CONVERT_DECLARE_BY_WRAPPER */ \
    zfclass ZF_ENV_EXPORT v_##TypeName : zfextends ZFVarConvertBase \
    { \
        ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(v_##TypeName, ZFVarConvertBase) \
    protected: \
        /** @cond ZFPrivateDoc */ \
        v_##TypeName(void) : value() {} \
        /** @endcond */ \
    public: \
        /** @brief the value to wrap, see #ZFVAR_CONVERT_DECLARE_BY_WRAPPER */ \
        Type value; \
    public: \
        zfoverride \
        virtual ZFObject *objectOnInit(void) {return zfsuper::objectOnInit();} \
        /** @brief construct from value */ \
        virtual ZFObject *objectOnInit(ZF_IN Type const &v) {this->objectOnInit(); this->value = v; return this;} \
    };
/**
 * @brief see #ZFVAR_CONVERT_WRAPPER_DECLARE
 */
#define ZFVAR_CONVERT_WRAPPER_DEFINE(TypeName) \
    ZFOBJECT_REGISTER(v_##TypeName)

// ============================================================
/**
 * @brief util macro to declare #ZFVAR_CONVERT_DECLARE
 *
 * usage:
 * @code
 *   // in header file
 *   ZFVAR_CONVERT_WRAPPER_DECLARE(MyType, MyTypeName)
 *   ZFVAR_CONVERT_DECLARE_BY_WRAPPER(MyType, MyTypeName)
 *
 *   // in source file
 *   ZFVAR_CONVERT_WRAPPER_DEFINE(MyType, MyTypeName)
 * @endcode
 */
#define ZFVAR_CONVERT_DECLARE_BY_WRAPPER(Type, TypeName) \
    ZFVAR_CONVERT_DECLARE(Type, { \
            v_##TypeName *wrap = ZFCastZFObject(v_##TypeName *, obj); \
            if(wrap == zfnull) \
            { \
                return (obj == zfnull); \
            } \
            v = wrap->value; \
        }, { \
            v_##TypeName *wrap = zfAlloc(v_##TypeName); \
            wrap->value = v; \
            obj = zfautoObjectCreate(wrap); \
            zfRelease(wrap); \
        })

// ============================================================
// basic type
ZFVAR_CONVERT_WRAPPER_DECLARE(zfbool, zfbool)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(zfbool, zfbool)

ZFVAR_CONVERT_WRAPPER_DECLARE(zfchar, zfchar)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(zfchar, zfchar)

// ============================================================
// string type
ZFVAR_CONVERT_WRAPPER_DECLARE(zfstring, zfstring)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(zfstring, zfstring)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(const zfchar *, zfstring)

// ============================================================
// number type
// all use zfdouble as raw type
// and treat -1 as a special type for unsigned type
ZFVAR_CONVERT_WRAPPER_DECLARE(zfdouble, zfnumber)

#define _ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER(Type, TypeName) \
    ZFVAR_CONVERT_DECLARE(Type, { \
            v_zfnumber *wrap = ZFCastZFObject(v_zfnumber *, obj); \
            if(wrap == zfnull) \
            { \
                return (obj == zfnull); \
            } \
            v = (Type)wrap->value; \
        }, { \
            v_zfnumber *wrap = zfAlloc(v_zfnumber); \
            wrap->value = (zfdouble)v; \
            obj = zfautoObjectCreate(wrap); \
            zfRelease(wrap); \
        })
#define _ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_INTEGER(Type, TypeName) \
    ZFVAR_CONVERT_DECLARE(Type, { \
            v_zfnumber *wrap = ZFCastZFObject(v_zfnumber *, obj); \
            if(wrap == zfnull) \
            { \
                return (obj == zfnull); \
            } \
            v = (Type)zfmRound(wrap->value); \
        }, { \
            v_zfnumber *wrap = zfAlloc(v_zfnumber); \
            wrap->value = (zfdouble)v; \
            obj = zfautoObjectCreate(wrap); \
            zfRelease(wrap); \
        })
#define _ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_UNSIGNED(Type, TypeName) \
    ZFVAR_CONVERT_DECLARE(Type, { \
            v_zfnumber *wrap = ZFCastZFObject(v_zfnumber *, obj); \
            if(wrap == zfnull) \
            { \
                return (obj == zfnull); \
            } \
            if(zffloatIsEqual<zfdouble>(wrap->value, -1)) \
            { \
                v = (Type)-1; \
            } \
            else \
            { \
                v = (Type)wrap->value; \
            } \
        }, { \
            v_zfnumber *wrap = zfAlloc(v_zfnumber); \
            if(v == (Type)-1) \
            { \
                wrap->value = -1; \
            } \
            else \
            { \
                wrap->value = (zfdouble)v; \
            } \
            obj = zfautoObjectCreate(wrap); \
            zfRelease(wrap); \
        })

_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_INTEGER(zfbyte, zfbyte)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_INTEGER(zfint, zfint)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_UNSIGNED(zfuint, zfuint)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_UNSIGNED(zfindex, zfindex)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_INTEGER(zfint8, zfint8)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_UNSIGNED(zfuint8, zfuint8)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_INTEGER(zfint16, zfint16)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_UNSIGNED(zfuint16, zfuint16)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_INTEGER(zfint32, zfint32)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_UNSIGNED(zfuint32, zfuint32)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_INTEGER(zfint64, zfint64)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_UNSIGNED(zfuint64, zfuint64)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER(zffloat, zffloat)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER(zfdouble, zfdouble)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER(zflongdouble, zflongdouble)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_INTEGER(zftimet, zftimet)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_UNSIGNED(zfflags, zfflags)
_ZFP_ZFVAR_CONVERT_DECLARE_BY_WRAPPER_NUMBER_UNSIGNED(zfidentity, zfidentity)

// ============================================================
// core type
ZFVAR_CONVERT_WRAPPER_DECLARE(ZFLevel, ZFLevel)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFLevel, ZFLevel)

ZFVAR_CONVERT_WRAPPER_DECLARE(ZFCompareResult, ZFCompareResult)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFCompareResult, ZFCompareResult)

ZFVAR_CONVERT_WRAPPER_DECLARE(ZFSeekPos, ZFSeekPos)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFSeekPos, ZFSeekPos)

ZFVAR_CONVERT_WRAPPER_DECLARE(zfindexRange, zfindexRange)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(zfindexRange, zfindexRange)

ZFVAR_CONVERT_WRAPPER_DECLARE(ZFFilterType, ZFFilterType)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFFilterType, ZFFilterType)

ZFVAR_CONVERT_WRAPPER_DECLARE(ZFFilterCallbackResult, ZFFilterCallbackResult)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFFilterCallbackResult, ZFFilterCallbackResult)

ZFVAR_CONVERT_WRAPPER_DECLARE(ZFCallbackType, ZFCallbackType)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFCallbackType, ZFCallbackType)

ZFVAR_CONVERT_WRAPPER_DECLARE(ZFClassFilterType, ZFClassFilterType)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFClassFilterType, ZFClassFilterType)

ZFVAR_CONVERT_WRAPPER_DECLARE(ZFMethodPrivilegeType, ZFMethodPrivilegeType)
ZFVAR_CONVERT_DECLARE_BY_WRAPPER(ZFMethodPrivilegeType, ZFMethodPrivilegeType)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFVarConvertDef_common_h_

