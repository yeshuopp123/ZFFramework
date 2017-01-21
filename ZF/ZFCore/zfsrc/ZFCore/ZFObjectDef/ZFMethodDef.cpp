/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFMethodDef.h"
#include "ZFObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFCORETYPE_STRING_CONVERTER_DEFINE(ZFMethodPrivilegeType, ZFMethodPrivilegeType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFMethodPrivilegeTypePublic,
            ZFTOKEN_ZFMethodPrivilegeTypeProtected,
            ZFTOKEN_ZFMethodPrivilegeTypePrivate,
        });
        zfindex matched = ZFCoreStringCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFMethodPrivilegeTypePublic;
        switch(matched)
        {
            case 0:
                v = ZFMethodPrivilegeTypePublic;
                return zfnull;
            case 1:
                v = ZFMethodPrivilegeTypeProtected;
                return zfnull;
            case 2:
                v = ZFMethodPrivilegeTypePrivate;
                return zfnull;
            default:
                return src;
        }
    }, {
        switch(v)
        {
            case ZFMethodPrivilegeTypePublic:
                s += ZFTOKEN_ZFMethodPrivilegeTypePublic;
                return ;
            case ZFMethodPrivilegeTypeProtected:
                s += ZFTOKEN_ZFMethodPrivilegeTypeProtected;
                return ;
            case ZFMethodPrivilegeTypePrivate:
                s += ZFTOKEN_ZFMethodPrivilegeTypePrivate;
                return ;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    })

// ============================================================
void ZFMethod::_ZFP_ZFMethod_init(ZF_IN zfbool methodIsUserRegister,
                                  ZF_IN ZFFuncAddrType invoker,
                                  ZF_IN ZFMethodGenericInvoker methodGenericInvoker,
                                  ZF_IN const zfchar *methodName,
                                  ZF_IN const zfchar *methodId,
                                  ZF_IN const zfchar *returnTypeName,
                                  ...)
{
    this->_ZFP_ZFMethod_methodIsUserRegister = methodIsUserRegister;
    this->_ZFP_ZFMethod_invoker = invoker;
    this->_ZFP_ZFMethod_invokerOrg = invoker;
    this->_ZFP_ZFMethod_methodGenericInvoker = methodGenericInvoker;
    this->_ZFP_ZFMethod_methodGenericInvokerOrg = methodGenericInvoker;
    this->_ZFP_ZFMethod_methodName = methodName;
    this->_ZFP_ZFMethod_methodId = methodId + zfslen(ZFM_TOSTRING(_ZFP_ZFMethodIdFix));
    this->_ZFP_ZFMethod_returnTypeName = returnTypeName;
    this->_ZFP_ZFMethod_paramNum = 0;

    va_list vaList;
    va_start(vaList, returnTypeName);
    const zfcharA *paramType = va_arg(vaList, const zfcharA *);
    while(paramType != zfnull && zfslen(paramType) > 0)
    {
        this->_ZFP_ZFMethod_paramTypeNameList[this->_ZFP_ZFMethod_paramNum++] = zfsCoreA2Z(paramType);
        paramType = va_arg(vaList, const zfcharA *);
    }
    va_end(vaList);
}
void ZFMethod::_ZFP_ZFMethod_initClassMemberType(ZF_IN const ZFClass *ownerClass,
                                                 ZF_IN ZFMethodPrivilegeType privilegeType,
                                                 ZF_IN zfbool isStatic,
                                                 ZF_IN zfbool isVirtual,
                                                 ZF_IN zfbool isConst)
{
    this->_ZFP_ZFMethod_ownerClass = ownerClass;
    this->_ZFP_ZFMethod_privilegeType = privilegeType;
    this->_ZFP_ZFMethod_methodIsStatic = isStatic;
    this->_ZFP_ZFMethod_methodIsVirtual = isVirtual;
    this->_ZFP_ZFMethod_methodIsConst = isConst;
}
void ZFMethod::_ZFP_ZFMethod_initFuncType(ZF_IN const zfchar *methodNamespace)
{
    this->_ZFP_ZFMethod_methodNamespace = methodNamespace;
}

/** @cond ZFPrivateDoc */
ZFMethod::ZFMethod(void)
: _ZFP_ZFMethodNeedInit(zftrue)
, _ZFP_ZFMethod_methodIsUserRegister(zffalse)
, _ZFP_ZFMethod_invoker(zfnull)
, _ZFP_ZFMethod_invokerOrg(zfnull)
, _ZFP_ZFMethod_methodGenericInvoker(zfnull)
, _ZFP_ZFMethod_methodGenericInvokerOrg(zfnull)
, _ZFP_ZFMethod_methodName()
, _ZFP_ZFMethod_methodId()
, _ZFP_ZFMethod_returnTypeName()
, _ZFP_ZFMethod_paramNum(0)
, _ZFP_ZFMethod_paramTypeNameList()
, _ZFP_ZFMethod_ownerClass(zfnull)
, _ZFP_ZFMethod_privilegeType(ZFMethodPrivilegeTypePrivate)
, _ZFP_ZFMethod_methodIsStatic(zffalse)
, _ZFP_ZFMethod_methodIsVirtual(zffalse)
, _ZFP_ZFMethod_methodIsConst(zffalse)
, _ZFP_ZFMethod_methodNamespace()
{
}
/** @endcond */
ZFMethod::~ZFMethod(void)
{
}

void ZFMethod::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    if(this->methodIsStatic())
    {
        ret += zfText("static ");
    }
    else if(this->methodIsVirtual())
    {
        ret += zfText("virtual ");
    }

    ret += this->methodReturnTypeName();
    ret += zfText(" ");
    ret += this->ownerClass()->className();
    ret += zfText("::");
    ret += this->methodName();
    if(!zfscmpTheSame(this->methodId(), zfText("")))
    {
        ret += zfText(":");
        ret += this->methodId();
    }

    if(this->methodParamCount() > 0)
    {
        ret += zfText("(");
        for(zfindex i = 0; i < this->methodParamCount(); ++i)
        {
            if(i != 0)
            {
                ret += zfText(", ");
            }
            ret += this->methodParamTypeNameAtIndex(i);
        }
        ret += zfText(")");
    }
    else
    {
        ret += zfText("(void)");
    }

    if(this->methodIsConst())
    {
        ret += zfText(" const");
    }
}

const zfchar *ZFMethod::methodPrivilegeTypeString(void) const
{
    switch(this->_ZFP_ZFMethod_privilegeType)
    {
        case ZFMethodPrivilegeTypePublic:
            return zfText("public");
        case ZFMethodPrivilegeTypeProtected:
            return zfText("protected");
        case ZFMethodPrivilegeTypePrivate:
            return zfText("private");
    }
    return zfText("<UnknownPrivilegeType>");
}

void ZFMethod::methodGenericInvokerSet(ZF_IN ZFMethodGenericInvoker methodGenericInvoker) const
{
    this->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_methodGenericInvoker =
        ((methodGenericInvoker != zfnull) ? methodGenericInvoker : this->_ZFP_ZFMethod_methodGenericInvokerOrg);
    this->_ZFP_ZFMethod_ownerClass->classDataChangeNotify();
}

void ZFMethod::methodInvokerSet(ZF_IN ZFFuncAddrType methodInvoker) const
{
    this->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_invoker =
        ((methodInvoker != zfnull) ? methodInvoker : this->_ZFP_ZFMethod_invokerOrg);
    this->_ZFP_ZFMethod_ownerClass->classDataChangeNotify();
}

// ============================================================
zfclassLikePOD _ZFP_ZFMethodMapData
{
public:
    zfindex refCount;
    ZFMethod method;
};
ZF_STATIC_INITIALIZER_INIT(ZFMethodDataHolder)
{
}
ZFCoreMap methodMap; // _ZFP_ZFMethodMapData *
ZF_STATIC_INITIALIZER_END(ZFMethodDataHolder)
#define _ZFP_ZFMethodMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFMethodDataHolder)->methodMap)

ZFMethod *_ZFP_ZFMethodInstanceFind(ZF_IN const zfchar *internalMethodId)
{
    ZFCoreMutexLocker();
    _ZFP_ZFMethodMapData *v = _ZFP_ZFMethodMap.get<_ZFP_ZFMethodMapData *>(internalMethodId);
    if(v == zfnull)
    {
        return zfnull;
    }
    else
    {
        return &(v->method);
    }
}
ZFMethod *_ZFP_ZFMethodInstanceAccess(ZF_IN const zfchar *internalMethodId)
{
    ZFCoreMutexLocker();
    _ZFP_ZFMethodMapData *v = _ZFP_ZFMethodMap.get<_ZFP_ZFMethodMapData *>(internalMethodId);
    if(v == zfnull)
    {
        v = zfnew(_ZFP_ZFMethodMapData);
        _ZFP_ZFMethodMap.set(internalMethodId, ZFCorePointerForObject<_ZFP_ZFMethodMapData *>(v));
    }
    else
    {
        ++(v->refCount);
    }
    return &(v->method);
}
zfbool _ZFP_ZFMethodInstanceCleanup(ZF_IN const zfchar *internalMethodId)
{
    ZFCoreMutexLocker();
    _ZFP_ZFMethodMapData *v = _ZFP_ZFMethodMap.get<_ZFP_ZFMethodMapData *>(internalMethodId);
    if(v == zfnull)
    {
        return zffalse;
    }
    --(v->refCount);
    if(v->refCount == 0)
    {
        _ZFP_ZFMethodMap.remove(internalMethodId);
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

