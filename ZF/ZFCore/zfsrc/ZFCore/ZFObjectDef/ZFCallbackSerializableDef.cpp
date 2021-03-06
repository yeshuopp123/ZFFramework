/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCallbackSerializableDef.h"
#include "ZFSerializableUtilDef.h"
#include "ZFMethodSerializableDef.h"
#include "ZFCoreType_IODef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// serialize routine
ZFPROPERTY_TYPE_DEFINE(ZFCallback, ZFCallback, {
        { // custom serialize logic
            const zfchar *customType = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFCallback_callbackType);
            if(customType != zfnull)
            {
                const ZFSerializableData *customData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFCallback_callbackData);
                if(customData == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                        zfText("missing %s"), ZFSerializableKeyword_ZFCallback_callbackData);
                    return zffalse;
                }

                ZFCallbackSerializeCustomSerializeCallback serializeCallback = ZFCallbackSerializeCustomTypeGet(customType);
                if(serializeCallback == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                        zfText("no such callback custom serialize type: %s"), customType);
                    return zffalse;
                }
                if(!serializeCallback(v, *customData, outErrorHint, outErrorPos))
                {
                    return zffalse;
                }
                v.callbackSerializeCustomTypeSet(customType);
                v.callbackSerializeCustomDataSet(customData);

                serializableData.resolveMark();
                return zftrue;
            }
        }

        if(zfscmpTheSame(serializableData.itemClass(), ZFSerializableKeyword_null))
        {
            v = ZFCallbackNull();
            serializableData.resolveMark();
            return zftrue;
        }
        if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZFCallback(), serializableData, outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        const ZFSerializableData *methodData = ZFSerializableUtil::requireElementByCategory(serializableData, ZFSerializableKeyword_ZFCallback_method, outErrorHint, outErrorPos);
        if(methodData == zfnull)
        {
            return zffalse;
        }
        const ZFMethod *method = zfnull;
        if(!ZFMethodFromSerializableData(method, *methodData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }

        if(method->ownerClass() == zfnull)
        {
            v = ZFCallbackForMethod(method);
        }
        else if(method->methodIsStatic())
        {
            v = ZFCallbackForMethod(method);
        }
        else
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                zfText("member method callback is not supported"));
            return zffalse;
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        if(v.callbackSerializeCustomType() != zfnull)
        {
            if(v.callbackSerializeCustomData() == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, zfText("missing callback serialize custom data"));
                return zffalse;
            }
            serializableData.itemClassSet(ZFPropertyTypeId_ZFCallback());

            serializableData.attributeSet(ZFSerializableKeyword_ZFCallback_callbackType, v.callbackSerializeCustomType());

            ZFSerializableData customData = v.callbackSerializeCustomData()->copy();
            customData.categorySet(ZFSerializableKeyword_ZFCallback_callbackData);
            serializableData.elementAdd(customData);

            return zftrue;
        }

        switch(v.callbackType())
        {
            case ZFCallbackTypeDummy:
                serializableData.itemClassSet(ZFSerializableKeyword_null);
                break;
            case ZFCallbackTypeMethod:
            {
                serializableData.itemClassSet(ZFPropertyTypeId_ZFCallback());
                ZFSerializableData methodData;
                if(!ZFMethodToSerializableData(methodData, v.callbackMethod(), outErrorHint))
                {
                    return zffalse;
                }
                methodData.categorySet(ZFSerializableKeyword_ZFCallback_method);
                serializableData.elementAdd(methodData);
            }
                break;
            case ZFCallbackTypeMemberMethod:
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    zfText("member method callback is not supported"));
                return zffalse;
            }
                break;
            case ZFCallbackTypeRawFunction:
                serializableData.itemClassSet(ZFPropertyTypeId_ZFCallback());
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    zfText("raw function is not supported"));
                return zffalse;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }

        return zftrue;
    })

// ============================================================
// custom serialize logic
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFCallbackSerializeCustomDataHolder, ZFLevelZFFrameworkNormal)
{
}
public:
    ZFCoreMap datas; // ZFCallbackSerializeCustomSerializeCallback *
ZF_GLOBAL_INITIALIZER_END(ZFCallbackSerializeCustomDataHolder)
void ZFCallbackSerializeCustomTypeRegister(ZF_IN const zfchar *customType,
                                           ZFCallbackSerializeCustomSerializeCallback serializeCallback)
{
    ZFCoreMap &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCallbackSerializeCustomDataHolder)->datas;
    zfCoreAssertWithMessage(m.get(customType) == zfnull, zfTextA("custom callback serialize type \"%s\" already registered"), zfsCoreZ2A(customType));
    zfCoreAssert(customType != zfnull && *customType != '\0' && serializeCallback != zfnull);

    m.set(customType, ZFCorePointerForObject<ZFCallbackSerializeCustomSerializeCallback *>(zfnew(ZFCallbackSerializeCustomSerializeCallback, serializeCallback)));
}
void ZFCallbackSerializeCustomTypeUnregister(ZF_IN const zfchar *customType)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCallbackSerializeCustomDataHolder)->datas.remove(customType);
}
ZFCallbackSerializeCustomSerializeCallback ZFCallbackSerializeCustomTypeGet(ZF_IN const zfchar *customType)
{
    ZFCorePointerBase *value = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCallbackSerializeCustomDataHolder)->datas.get(customType);
    if(value == zfnull)
    {
        return zfnull;
    }
    else
    {
        return *(value->pointerValueT<const ZFCallbackSerializeCustomSerializeCallback *>());
    }
}

ZF_NAMESPACE_GLOBAL_END

