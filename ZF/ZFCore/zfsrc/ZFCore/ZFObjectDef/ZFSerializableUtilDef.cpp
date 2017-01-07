/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFSerializableUtilDef.h"
#include "ZFCoreType_IODef.h"
#include "ZFPropertyTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFSerializableUtilErrorOutputCallbacksHolder, ZFLevelZFFrameworkNormal)
{
}
public:
    ZFCoreArray<ZFOutputCallback> callbacks;
ZF_GLOBAL_INITIALIZER_END(ZFSerializableUtilErrorOutputCallbacksHolder)

ZF_NAMESPACE_BEGIN(ZFSerializableUtil)
ZFCoreArray<ZFOutputCallback> &_ZFP_ZFSerializableUtilErrorOutputCallbacks(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSerializableUtilErrorOutputCallbacksHolder)->callbacks;
}

void errorOccurred(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                   ZF_OUT_OPT ZFSerializableData *outErrorPos,
                   ZF_IN const ZFSerializableData &errorPos,
                   ZF_IN const zfchar *fmt,
                   ...)
{
    if(outErrorPos != zfnull)
    {
        *outErrorPos = errorPos;
    }

    zfstring errorString;
    va_list vaList;
    va_start(vaList, fmt);
    zfstringAppendV(errorString, fmt, vaList);
    va_end(vaList);
    zfstringAppend(errorString, zfText(", at:\n    %s"), errorPos.objectInfo().cString());

    if(outErrorHintToAppend != zfnull)
    {
        *outErrorHintToAppend += errorString;
    }
    ZFCoreArray<ZFOutputCallback> &callbacks = ZFSerializableUtilErrorOutputCallbacks;
    for(zfindex i = 0; i < callbacks.count(); ++i)
    {
        callbacks[i].execute(errorString.cString());
    }
}

void errorOccurred(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                   ZF_IN const zfchar *fmt,
                   ...)
{
    zfstring errorString;
    va_list vaList;
    va_start(vaList, fmt);
    zfstringAppendV(errorString, fmt, vaList);
    va_end(vaList);

    if(outErrorHintToAppend != zfnull)
    {
        *outErrorHintToAppend += errorString;
    }
    ZFCoreArray<ZFOutputCallback> &callbacks = ZFSerializableUtilErrorOutputCallbacks;
    for(zfindex i = 0; i < callbacks.count(); ++i)
    {
        callbacks[i].execute(errorString.cString());
    }
}

void errorOccurredWhile(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                        ZF_OUT_OPT ZFSerializableData *outErrorPos,
                        ZF_IN const ZFSerializableData &errorPos,
                        ZF_IN const zfchar *serializingName,
                        ZF_IN const zfchar *errorValue)
{
    if(outErrorPos != zfnull)
    {
        *outErrorPos = errorPos;
    }

    zfstring errorString;
    zfstringAppend(errorString, zfText("failed to serialize \"%s\" with value \"%s\""),
        serializingName, errorValue);
    zfstringAppend(errorString, zfText(", at:\n    %s"), errorPos.objectInfo().cString());

    if(outErrorHintToAppend != zfnull)
    {
        *outErrorHintToAppend += errorString;
    }
    ZFCoreArray<ZFOutputCallback> &callbacks = ZFSerializableUtilErrorOutputCallbacks;
    for(zfindex i = 0; i < callbacks.count(); ++i)
    {
        callbacks[i].execute(errorString.cString());
    }
}
void errorOccurredWhile(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                        ZF_IN const zfchar *serializingName,
                        ZF_IN const zfchar *errorValue)
{
    zfstring errorString;
    zfstringAppend(errorString, zfText("failed to serialize \"%s\" with value \"%s\""),
        serializingName, errorValue);

    if(outErrorHintToAppend != zfnull)
    {
        *outErrorHintToAppend += errorString;
    }
    ZFCoreArray<ZFOutputCallback> &callbacks = ZFSerializableUtilErrorOutputCallbacks;
    for(zfindex i = 0; i < callbacks.count(); ++i)
    {
        callbacks[i].execute(errorString.cString());
    }
}

const zfchar *checkSerializableClass(ZF_IN const zfchar *desiredClass,
                                     ZF_IN const ZFSerializableData &serializableData)
{
    const zfchar *serializableClass = serializableData.itemClass();
    if(desiredClass == zfnull || *desiredClass == '\0')
    {
        return serializableClass;
    }
    else
    {
        if(zfscmpTheSame(serializableClass, desiredClass))
        {
            return serializableClass;
        }
        else
        {
            return zfnull;
        }
    }
}
const zfchar *requireSerializableClass(ZF_IN const zfchar *desiredClass,
                                       ZF_IN const ZFSerializableData &serializableData,
                                       ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                       ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    const zfchar *ret = checkSerializableClass(desiredClass, serializableData);
    if(ret == zfnull)
    {
        if(desiredClass == zfnull || *desiredClass == '\0')
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                zfText("missing serializable class"));
        }
        else
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                zfText("serializable class must be \"%s\""), desiredClass);
        }
    }
    return ret;
}

const zfchar *checkAttribute(ZF_IN const ZFSerializableData &serializableData,
                             ZF_IN const zfchar *desiredAttribute)
{
    zfiterator it = serializableData.attributeIteratorForName(desiredAttribute);
    if(!serializableData.attributeIteratorIsValid(it))
    {
        return zfnull;
    }
    serializableData.attributeIteratorResolveMark(it);
    return serializableData.attributeIteratorGet(it);
}
const zfchar *requireAttribute(ZF_IN const ZFSerializableData &serializableData,
                               ZF_IN const zfchar *desiredAttribute,
                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    const zfchar *ret = checkAttribute(serializableData, desiredAttribute);
    if(ret == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("missing attribute \"%s\""), desiredAttribute);
    }
    return ret;
}

const ZFSerializableData *checkElementByName(ZF_IN const ZFSerializableData &serializableData,
                                             ZF_IN const zfchar *desiredElementName)
{
    zfindex index = serializableData.elementFindByName(desiredElementName, zfHint("skipResolved")zftrue);
    if(index == zfindexMax)
    {
        return zfnull;
    }
    serializableData.elementAtIndex(index).resolvePropertyNameMark();
    return &(serializableData.elementAtIndex(index));
}
const ZFSerializableData *requireElementByName(ZF_IN const ZFSerializableData &serializableData,
                                               ZF_IN const zfchar *desiredElementName,
                                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    const ZFSerializableData *ret = checkElementByName(serializableData, desiredElementName);
    if(ret == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("missing element with name \"%s\""),
            desiredElementName);
    }
    return ret;
}

const ZFSerializableData *checkElementByCategory(ZF_IN const ZFSerializableData &serializableData,
                                                 ZF_IN const zfchar *desiredElementCategory)
{
    zfindex index = serializableData.elementFindByCategory(desiredElementCategory);
    if(index == zfindexMax)
    {
        return zfnull;
    }
    serializableData.elementAtIndex(index).resolveCategoryMark();
    return &(serializableData.elementAtIndex(index));
}
const ZFSerializableData *requireElementByCategory(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_IN const zfchar *desiredElementCategory,
                                                   ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    const ZFSerializableData *ret = checkElementByCategory(serializableData, desiredElementCategory);
    if(ret == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("missing element with category \"%s\""),
            desiredElementCategory);
    }
    return ret;
}

static zfbool _ZFP_ZFSerializableUtilPrintResolveStatus(ZF_IN const ZFSerializableData &serializableData,
                                                        ZF_IN const ZFOutputCallback &outputCallback,
                                                        ZF_IN zfindex level)
{
    if(!serializableData.resolvedAll())
    {
        for(zfindex i = 0; i < level; ++i)
        {
            outputCallback << zfText("    ");
        }
        outputCallback << serializableData.itemClass();
        if(!serializableData.resolved())
        {
            outputCallback << zfText("(unresolved)");
        }

        zfbool hasUnresolvedAttribute = zffalse;
        for(zfiterator it = serializableData.attributeIterator();
            serializableData.attributeIteratorIsValid(it);
            serializableData.attributeIteratorNext(it))
        {
            if(!serializableData.attributeIteratorResolved(it))
            {
                if(!hasUnresolvedAttribute)
                {
                    hasUnresolvedAttribute = zftrue;
                    outputCallback << zfText(" < ");
                }
                else
                {
                    outputCallback << zfText("; ");
                }
                outputCallback
                    << serializableData.attributeIteratorGetKey(it)
                    << zfText("=")
                    << serializableData.attributeIteratorGet(it);
            }
        }
        if(hasUnresolvedAttribute)
        {
            outputCallback << zfText(" >");
        }

        outputCallback << zfText("\n");

        for(zfindex i = 0; i < serializableData.elementCount(); ++i)
        {
            _ZFP_ZFSerializableUtilPrintResolveStatus(serializableData.elementAtIndex(i), outputCallback, level + 1);
        }
        return zftrue;
    }
    return zffalse;
}
zfbool printResolveStatus(ZF_IN const ZFSerializableData &serializableData,
                          ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */)
{
    zfstring tmp = zfText("not all resolved:\n");
    zfbool ret = _ZFP_ZFSerializableUtilPrintResolveStatus(serializableData, ZFOutputCallbackForString(tmp), 1);
    if(ret)
    {
        outputCallback.execute(tmp.cString());
    }
    return ret;
}

ZF_NAMESPACE_END(ZFSerializableUtil)
ZF_NAMESPACE_GLOBAL_END

