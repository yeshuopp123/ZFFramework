/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFClassSerializableDef.h"
#include "ZFSerializableUtilDef.h"
#include "ZFCoreType_IODef.h"
#include "ZFPropertyTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZFPROPERTY_TYPE_DEFINE(ZFClass, const ZFClass *, {
        if(zfscmpTheSame(serializableData.itemClass(), ZFSerializableKeyword_null))
        {
            v = zfnull;
            serializableData.resolveMark();
            return zftrue;
        }
        if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZFClass(), serializableData, outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }
        const zfchar *className = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFClass_className, outErrorHint, outErrorPos);
        if(className == zfnull)
        {
            return zffalse;
        }
        v = ZFClass::classForName(className);
        if(v == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                zfText("no such class \"%s\""), className);
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

        serializableData.itemClassSet(ZFPropertyTypeId_ZFClass());
        serializableData.attributeSet(ZFSerializableKeyword_ZFClass_className, v->className());

        return zftrue;
    })

ZF_NAMESPACE_GLOBAL_END

