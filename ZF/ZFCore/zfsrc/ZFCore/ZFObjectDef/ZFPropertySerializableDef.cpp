/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPropertySerializableDef.h"
#include "ZFSerializableUtilDef.h"
#include "ZFPropertyTypeDef.h"
#include "ZFCoreType_IODef.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZFPROPERTY_TYPE_DEFINE(ZFProperty, const ZFProperty *, {
        if(zfscmpTheSame(serializableData.itemClass(), ZFSerializableKeyword_null))
        {
            v = zfnull;
            serializableData.resolveMark();
            return zftrue;
        }
        if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZFProperty(), serializableData, outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        const zfchar *tmpValue = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFProperty_owner, outErrorHint, outErrorPos);
        if(tmpValue == zfnull)
        {
            return zffalse;
        }
        const ZFClass *ownerClass = ZFClass::classForName(tmpValue);
        if(ownerClass == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                zfText("no such class \"%s\""), tmpValue);
            return zffalse;
        }

        tmpValue = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFProperty_property, outErrorHint, outErrorPos);
        if(tmpValue == zfnull)
        {
            return zffalse;
        }
        v = ownerClass->propertyForName(tmpValue);
        if(v == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                zfText("no such property \"%s\" in class \"%s\""), tmpValue, ownerClass->className());
            return zffalse;
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        if(v == zfnull)
        {
            serializableData.itemClassSet(ZFSerializableKeyword_null);
            return zftrue;
        }

        serializableData.itemClassSet(ZFPropertyTypeId_ZFProperty());

        serializableData.attributeSet(ZFSerializableKeyword_ZFProperty_owner, v->ownerClass()->className());
        serializableData.attributeSet(ZFSerializableKeyword_ZFProperty_property, v->propertyName());

        return zftrue;
    })

ZF_NAMESPACE_GLOBAL_END

