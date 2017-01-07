/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFClassUtilDef.h"
#include "ZFObjectDef.h"
#include "ZFClassFilterDef.h"
#include "ZFMethodFilterDef.h"
#include "ZFPropertyFilterDef.h"
#include "ZFPropertyTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFClassUtil)

void allClassParent(ZF_OUT ZFCoreArray<const ZFClass *> &ret,
                    ZF_IN const ZFClass *cls,
                    ZF_IN_OPT const ZFClassFilter *filter /* = zfnull */)
{
    if(filter == zfnull)
    {
        while(cls != zfnull)
        {
            ret.add(cls);
            for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i)
            {
                ret.add(cls->implementedInterfaceAtIndex(i));
            }
            cls = cls->parentClass();
        }
    }
    else
    {
        while(cls != zfnull)
        {
            if(filter->filterCheckActive(cls))
            {
                ret.add(cls);
            }
            for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i)
            {
                if(filter->filterCheckActive(cls->implementedInterfaceAtIndex(i)))
                {
                    ret.add(cls->implementedInterfaceAtIndex(i));
                }
            }
            cls = cls->parentClass();
        }
    }
}

void allMethod(ZF_OUT ZFCoreArray<const ZFMethod *> &ret,
               ZF_IN const ZFClass *cls,
               ZF_IN_OPT const ZFMethodFilter *filter /* = zfnull */)
{
    ZFCoreArrayPOD<const ZFClass *> allClassParent = ZFClassUtil::allClassParent(cls, (filter == zfnull) ? zfnull : &(filter->classFilter));
    for(zfindex i = 0; i < allClassParent.count(); ++i)
    {
        const ZFClass *cls = allClassParent.get(i);
        for(zfindex j = 0; j < cls->methodCount(); ++j)
        {
            if(filter == zfnull || filter->filterCheckActive(cls->methodAtIndex(j)))
            {
                ret.add(cls->methodAtIndex(j));
            }
        }
    }
}

void allProperty(ZF_OUT ZFCoreArray<const ZFProperty *> &ret,
                 ZF_IN const ZFClass *cls,
                 ZF_IN_OPT const ZFPropertyFilter *filter /* = zfnull */)
{
    ZFCoreArrayPOD<const ZFClass *> allClassParent = ZFClassUtil::allClassParent(cls, (filter == zfnull) ? zfnull : &(filter->classFilter));
    for(zfindex i = 0; i < allClassParent.count(); ++i)
    {
        const ZFClass *cls = allClassParent.get(i);
        for(zfindex j = 0; j < cls->propertyCount(); ++j)
        {
            if(filter == zfnull || filter->filterCheckActive(cls->propertyAtIndex(j)))
            {
                ret.add(cls->propertyAtIndex(j));
            }
        }
    }
}

zfbool allPropertyIsEqual(ZF_IN ZFObject *obj0,
                          ZF_IN ZFObject *obj1,
                          ZF_IN_OPT const ZFPropertyFilter *filter /* = zfnull */)
{
    if(obj0 == obj1)
    {
        return zftrue;
    }
    if(obj0 == zfnull || obj1 == zfnull)
    {
        return zffalse;
    }
    const ZFClass *cls0 = obj0->classData();
    const ZFClass *cls1 = obj1->classData();

    ZFCoreArrayPOD<const ZFProperty *> allProperty = ZFClassUtil::allProperty(cls0, filter);
    for(zfindex i = allProperty.count() - 1; i != zfindexMax; --i)
    {
        if(cls1->classIsTypeOf(allProperty[i]->ownerClass())
            && allProperty[i]->callbackCompare(allProperty[i], obj0, obj1) != ZFCompareTheSame)
        {
            return zffalse;
        }
    }

    return zftrue;
}

void objectPropertyInfo(ZF_IN_OUT zfstring &ret,
                        ZF_IN ZFObject *obj,
                        ZF_IN_OPT zfindex maxCount /* = zfindexMax */,
                        ZF_IN_OPT const ZFTokenForKeyValueContainer &token /* = ZFTokenForKeyValueContainerDefault */)
{
    if(obj == zfnull)
    {
        return ;
    }

    ZFCoreArrayPOD<const ZFProperty *> allProperty = ZFClassUtil::allProperty(obj->classData());
    zfindex count = 0;
    zfindex index = 0;
    for( ; index < allProperty.count() && count < maxCount; ++index)
    {
        if(ZFPropertyIsInitValue(allProperty[index], obj))
        {
            continue;
        }

        if(count == 0)
        {
            ret += token.tokenLeft;
        }
        else
        {
            ret += token.tokenSeparator;
        }
        ++count;

        ret += token.tokenPairLeft;
        {
            ret += token.tokenKeyLeft;
            ret += allProperty[index]->propertyName();
            ret += token.tokenKeyRight;
        }
        ret += token.tokenPairSeparator;
        {
            ret += token.tokenValueLeft;
            ZFPropertyGetInfo(ret, allProperty[index], obj);
            ret += token.tokenValueRight;
        }
        ret += token.tokenPairRight;
    }
    if(index < allProperty.count())
    {
        if(count > 0)
        {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    if(count > 0)
    {
        ret += token.tokenRight;
    }
}

void objectInfoT(ZF_IN_OUT zfstring &ret,
                 ZF_IN ZFObject *obj)
{
    if(obj == zfnull)
    {
        ret += ZFTOKEN_zfnull;
        return ;
    }

    ret += ZFTOKEN_ZFObjectInfoLeft;
    ret += obj->classData()->className();
    ret += zfText("(");
    zfsFromPointerT(ret, obj);
    ret += zfText(")");

    ZFClassUtil::objectPropertyInfo(ret, obj);

    ret += ZFTOKEN_ZFObjectInfoRight;
}

ZF_NAMESPACE_END(ZFClassUtil)

ZF_NAMESPACE_GLOBAL_END

