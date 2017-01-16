/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFTextTemplateTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFTextTemplateIndexFlag)

// ============================================================
void ZFTextTemplateIndexData::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;

    zfstringAppend(ret, zfText("%%%zi[%zi%s]"),
        this->indexWidth,
        this->indexRadix,
        this->indexUpperCase ? zfText("x") : zfText("X"));

    if(this->indexOffset >= 0)
    {
        zfstringAppend(ret, zfText("[%zi, %d)"), this->indexStart, this->indexOffset);
    }
    else
    {
        zfstringAppend(ret, zfText("(%d, %zi]"), this->indexOffset, this->indexStart);
    }

    ret += zfText(" ");
    ZFTextTemplateIndexFlagEnumToString(ret, this->indexFlag);

    ret += ZFTOKEN_ZFObjectInfoRight;
}

// ============================================================
ZFPROPERTY_TYPE_DEFINE(ZFTextTemplateIndexData, ZFTextTemplateIndexData, {
        if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZFTextTemplateIndexData(), serializableData, outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        v.indexStart = 0;
        v.indexOffset = 1;
        v.indexRadix = 10;
        v.indexUpperCase = zftrue;
        v.indexWidth = 0;
        v.indexFlag = ZFTextTemplateIndexFlag::EnumDefault();

        const zfchar *element = zfnull;

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart);
        if(element != zfnull && zfindexFromString(v.indexStart, element) != zfnull)
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart, element);
            return zffalse;
        }

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset);
        if(element != zfnull && zfintFromString(v.indexOffset, element) != zfnull)
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset, element);
            return zffalse;
        }

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix);
        if(element != zfnull && zfindexFromString(v.indexRadix, element) != zfnull)
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix, element);
            return zffalse;
        }

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase);
        if(element != zfnull && zfboolFromString(v.indexUpperCase, element) != zfnull)
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase, element);
            return zffalse;
        }

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth);
        if(element != zfnull && zfindexFromString(v.indexWidth, element) != zfnull)
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth, element);
            return zffalse;
        }

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag);
        if(element != zfnull && ZFTextTemplateIndexFlagEnumFromString(v.indexFlag, element) != zfnull)
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag, element);
            return zffalse;
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        serializableData.itemClassSet(ZFPropertyTypeId_ZFTextTemplateIndexData());

        if(v.indexStart != 0)
        {
            serializableData.attributeSet(ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart, zfindexToString(v.indexStart));
        }

        if(v.indexOffset != 1)
        {
            serializableData.attributeSet(ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset, zfintToString(v.indexOffset));
        }

        if(v.indexRadix != 10)
        {
            serializableData.attributeSet(ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix, zfindexToString(v.indexRadix));
        }

        if(!v.indexUpperCase)
        {
            serializableData.attributeSet(ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase, zfboolToString(v.indexUpperCase));
        }

        if(v.indexWidth != 0)
        {
            serializableData.attributeSet(ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth, zfindexToString(v.indexWidth));
        }

        if(v.indexFlag != ZFTextTemplateIndexFlag::EnumDefault())
        {
            serializableData.attributeSet(ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag, ZFTextTemplateIndexFlagEnumToString(v.indexFlag));
        }

        return zftrue;
    })

// ============================================================
zfclassNotPOD _ZFP_ZFTextTemplateReplaceData
{
public:
    zfstring key;
    zfstring value;
};
zfclassNotPOD _ZFP_ZFTextTemplateEnableData
{
public:
    zfstring key;
    zfbool value;
};
zfclassNotPOD _ZFP_ZFTextTemplateIndexData
{
public:
    zfstring key;
    ZFTextTemplateIndexData value;
};
zfclassNotPOD _ZFP_ZFTextTemplateParamPrivate
{
public:
    ZFCoreArrayPOD<_ZFP_ZFTextTemplateReplaceData *> replaceDataList;
    ZFCoreMap replaceDataMap; // map to _ZFP_ZFTextTemplateReplaceData *

    zfbool enableDataDefault;
    ZFCoreArrayPOD<_ZFP_ZFTextTemplateEnableData *> enableDataList;
    ZFCoreMap enableDataMap; // map to _ZFP_ZFTextTemplateEnableData *

    ZFTextTemplateIndexData indexDataDefault;
    ZFCoreArrayPOD<_ZFP_ZFTextTemplateIndexData *> indexDataList;
    ZFCoreMap indexDataMap; // map to _ZFP_ZFTextTemplateIndexData *

public:
    _ZFP_ZFTextTemplateParamPrivate(void)
    : replaceDataList()
    , replaceDataMap()
    , enableDataDefault(zffalse)
    , enableDataList()
    , enableDataMap()
    , indexDataDefault()
    , indexDataList()
    , indexDataMap()
    {
    }
};
// ============================================================
ZFTextTemplateParam::ZFTextTemplateParam(void)
{
    d = zfnew(_ZFP_ZFTextTemplateParamPrivate);
}
ZFTextTemplateParam::~ZFTextTemplateParam(void)
{
    zfdelete(d);
    d = zfnull;
}

// ============================================================
void ZFTextTemplateParam::replaceDataAdd(ZF_IN const zfchar *key, ZF_IN const zfchar *value)
{
    if(key == zfnull)
    {
        return ;
    }
    zfiterator it = d->replaceDataMap.iteratorForKey(key);
    if(d->replaceDataMap.iteratorIsValid(it))
    {
        _ZFP_ZFTextTemplateReplaceData *replaceData = d->replaceDataMap.iteratorGetValue<_ZFP_ZFTextTemplateReplaceData *>(it);
        if(value == zfnull)
        {
            d->replaceDataList.removeElement(replaceData);
            d->replaceDataMap.iteratorRemove(it);
        }
        else
        {
            replaceData->value = value;
        }
    }
    else
    {
        if(value != zfnull)
        {
            _ZFP_ZFTextTemplateReplaceData *replaceData = zfnew(_ZFP_ZFTextTemplateReplaceData);
            replaceData->key = key;
            replaceData->value = value;
            d->replaceDataList.add(replaceData);
            d->replaceDataMap.set(key, ZFCorePointerForObject<_ZFP_ZFTextTemplateReplaceData *>(replaceData));
        }
    }
}
const zfchar *ZFTextTemplateParam::replaceData(ZF_IN const zfchar *key) const
{
    _ZFP_ZFTextTemplateReplaceData *replaceData = d->replaceDataMap.get<_ZFP_ZFTextTemplateReplaceData *>(key);
    if(replaceData != zfnull)
    {
        return replaceData->value;
    }
    else
    {
        return zfnull;
    }
}
zfindex ZFTextTemplateParam::replaceDataCount(void) const
{
    return d->replaceDataMap.count();
}
const zfchar *ZFTextTemplateParam::replaceDataNameAtIndex(ZF_IN zfindex index) const
{
    return d->replaceDataList.get(index)->key;
}
const zfchar *ZFTextTemplateParam::replaceDataAtIndex(ZF_IN zfindex index) const
{
    return d->replaceDataList.get(index)->value;
}
void ZFTextTemplateParam::replaceDataRemove(ZF_IN const zfchar *key)
{
    zfiterator it = d->replaceDataMap.iteratorForKey(key);
    if(d->replaceDataMap.iteratorIsValid(it))
    {
        d->replaceDataList.removeElement(d->replaceDataMap.iteratorGetValue<_ZFP_ZFTextTemplateReplaceData *>(it));
        d->replaceDataMap.iteratorRemove(it);
    }
}
void ZFTextTemplateParam::replaceDataRemoveAtIndex(ZF_IN zfindex index)
{
    const zfchar *key = d->replaceDataList.get(index)->key;
    d->replaceDataList.remove(index);
    d->replaceDataMap.remove(key);
}
void ZFTextTemplateParam::replaceDataRemoveAll(void)
{
    d->replaceDataList.removeAll();
    d->replaceDataMap.removeAll();
}

// ============================================================
void ZFTextTemplateParam::enableDataDefaultSet(ZF_IN zfbool enableDataDefault)
{
    d->enableDataDefault = enableDataDefault;
}
zfbool ZFTextTemplateParam::enableDataDefault(void) const
{
    return d->enableDataDefault;
}

void ZFTextTemplateParam::enableDataAdd(ZF_IN const zfchar *key, ZF_IN zfbool value)
{
    if(key == zfnull)
    {
        return ;
    }
    zfiterator it = d->enableDataMap.iteratorForKey(key);
    if(d->enableDataMap.iteratorIsValid(it))
    {
        _ZFP_ZFTextTemplateEnableData *enableData = d->enableDataMap.iteratorGetValue<_ZFP_ZFTextTemplateEnableData *>(it);
        enableData->value = value;
    }
    else
    {
        _ZFP_ZFTextTemplateEnableData *enableData = zfnew(_ZFP_ZFTextTemplateEnableData);
        enableData->key = key;
        enableData->value = value;
        d->enableDataList.add(enableData);
        d->enableDataMap.set(key, ZFCorePointerForObject<_ZFP_ZFTextTemplateEnableData *>(enableData));
    }
}
const zfbool *ZFTextTemplateParam::enableData(ZF_IN const zfchar *key) const
{
    _ZFP_ZFTextTemplateEnableData *enableData = d->enableDataMap.get<_ZFP_ZFTextTemplateEnableData *>(key);
    if(enableData != zfnull)
    {
        return &(enableData->value);
    }
    else
    {
        return zfnull;
    }
}
zfbool ZFTextTemplateParam::enableDataValue(ZF_IN const zfchar *key) const
{
    const zfbool *value = this->enableData(key);
    if(value == zfnull)
    {
        return this->enableDataDefault();
    }
    else
    {
        return *value;
    }
}
zfindex ZFTextTemplateParam::enableDataCount(void) const
{
    return d->enableDataMap.count();
}
const zfchar *ZFTextTemplateParam::enableDataNameAtIndex(ZF_IN zfindex index) const
{
    return d->enableDataList.get(index)->key;
}
zfbool ZFTextTemplateParam::enableDataAtIndex(ZF_IN zfindex index) const
{
    return d->enableDataList.get(index)->value;
}
void ZFTextTemplateParam::enableDataRemove(ZF_IN const zfchar *key)
{
    zfiterator it = d->enableDataMap.iteratorForKey(key);
    if(d->enableDataMap.iteratorIsValid(it))
    {
        d->enableDataList.removeElement(d->enableDataMap.iteratorGetValue<_ZFP_ZFTextTemplateEnableData *>(it));
        d->enableDataMap.iteratorRemove(it);
    }
}
void ZFTextTemplateParam::enableDataRemoveAtIndex(ZF_IN zfindex index)
{
    const zfchar *key = d->enableDataList.get(index)->key;
    d->enableDataList.remove(index);
    d->enableDataMap.remove(key);
}
void ZFTextTemplateParam::enableDataRemoveAll(void)
{
    d->enableDataList.removeAll();
    d->enableDataMap.removeAll();
}

// ============================================================
void ZFTextTemplateParam::indexDataDefaultSet(ZF_IN const ZFTextTemplateIndexData &indexDataDefault)
{
    d->indexDataDefault = indexDataDefault;
}
const ZFTextTemplateIndexData &ZFTextTemplateParam::indexDataDefault(void) const
{
    return d->indexDataDefault;
}

void ZFTextTemplateParam::indexDataAdd(ZF_IN const zfchar *key, ZF_IN const ZFTextTemplateIndexData &value)
{
    if(key == zfnull)
    {
        return ;
    }
    zfiterator it = d->indexDataMap.iteratorForKey(key);
    if(d->indexDataMap.iteratorIsValid(it))
    {
        _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataMap.iteratorGetValue<_ZFP_ZFTextTemplateIndexData *>(it);
        indexData->value = value;
    }
    else
    {
        _ZFP_ZFTextTemplateIndexData *indexData = zfnew(_ZFP_ZFTextTemplateIndexData);
        indexData->key = key;
        indexData->value = value;
        d->indexDataList.add(indexData);
        d->indexDataMap.set(key, ZFCorePointerForObject<_ZFP_ZFTextTemplateIndexData *>(indexData));
    }
}
const ZFTextTemplateIndexData *ZFTextTemplateParam::indexData(ZF_IN const zfchar *key) const
{
    _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataMap.get<_ZFP_ZFTextTemplateIndexData *>(key);
    if(indexData != zfnull)
    {
        return &(indexData->value);
    }
    else
    {
        return zfnull;
    }
}
ZFTextTemplateIndexData *ZFTextTemplateParam::indexData(ZF_IN const zfchar *key)
{
    _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataMap.get<_ZFP_ZFTextTemplateIndexData *>(key);
    if(indexData != zfnull)
    {
        return &(indexData->value);
    }
    else
    {
        return zfnull;
    }
}
const ZFTextTemplateIndexData *ZFTextTemplateParam::indexDataValue(ZF_IN const zfchar *key) const
{
    const ZFTextTemplateIndexData *value = this->indexData(key);
    if(value == zfnull)
    {
        return &(this->indexDataDefault());
    }
    else
    {
        return value;
    }
}
zfindex ZFTextTemplateParam::indexDataCount(void) const
{
    return d->indexDataMap.count();
}
const zfchar *ZFTextTemplateParam::indexDataNameAtIndex(ZF_IN zfindex index) const
{
    return d->indexDataList.get(index)->key;
}
const ZFTextTemplateIndexData *ZFTextTemplateParam::indexDataAtIndex(ZF_IN zfindex index) const
{
    return &(d->indexDataList.get(index)->value);
}
ZFTextTemplateIndexData *ZFTextTemplateParam::indexDataAtIndex(ZF_IN zfindex index)
{
    return &(d->indexDataList.get(index)->value);
}
void ZFTextTemplateParam::indexDataRemove(ZF_IN const zfchar *key)
{
    zfiterator it = d->indexDataMap.iteratorForKey(key);
    if(d->indexDataMap.iteratorIsValid(it))
    {
        d->indexDataList.removeElement(d->indexDataMap.iteratorGetValue<_ZFP_ZFTextTemplateIndexData *>(it));
        d->indexDataMap.iteratorRemove(it);
    }
}
void ZFTextTemplateParam::indexDataRemoveAtIndex(ZF_IN zfindex index)
{
    const zfchar *key = d->indexDataList.get(index)->key;
    d->indexDataList.remove(index);
    d->indexDataMap.remove(key);
}
void ZFTextTemplateParam::indexDataRemoveAll(void)
{
    d->indexDataList.removeAll();
    d->indexDataMap.removeAll();
}

// ============================================================
void ZFTextTemplateParam::copyFrom(ZF_IN const ZFTextTemplateParam &ref)
{
    if(d == ref.d)
    {
        return ;
    }

    d->replaceDataList.removeAll();
    d->replaceDataMap.removeAll();
    for(zfindex i = 0; i < ref.d->replaceDataList.count(); ++i)
    {
        const _ZFP_ZFTextTemplateReplaceData *replaceDataRef = ref.d->replaceDataList[i];
        _ZFP_ZFTextTemplateReplaceData *replaceData = zfnew(_ZFP_ZFTextTemplateReplaceData);
        replaceData->key = replaceDataRef->key;
        replaceData->value = replaceDataRef->value;
        d->replaceDataList.add(replaceData);
        d->replaceDataMap.set(replaceData->key, ZFCorePointerForObject<_ZFP_ZFTextTemplateReplaceData *>(replaceData));
    }

    d->enableDataDefault = ref.d->enableDataDefault;

    d->enableDataList.removeAll();
    d->enableDataMap.removeAll();
    for(zfindex i = 0; i < ref.d->enableDataList.count(); ++i)
    {
        const _ZFP_ZFTextTemplateEnableData *enableDataRef = ref.d->enableDataList[i];
        _ZFP_ZFTextTemplateEnableData *enableData = zfnew(_ZFP_ZFTextTemplateEnableData);
        enableData->key = enableDataRef->key;
        enableData->value = enableDataRef->value;
        d->enableDataList.add(enableData);
        d->enableDataMap.set(enableData->key, ZFCorePointerForObject<_ZFP_ZFTextTemplateEnableData *>(enableData));
    }

    d->indexDataDefault.copyFrom(ref.d->indexDataDefault);

    d->indexDataList.removeAll();
    d->indexDataMap.removeAll();
    for(zfindex i = 0; i < ref.d->indexDataList.count(); ++i)
    {
        const _ZFP_ZFTextTemplateIndexData *indexDataRef = ref.d->indexDataList[i];
        _ZFP_ZFTextTemplateIndexData *indexData = zfnew(_ZFP_ZFTextTemplateIndexData);
        indexData->key = indexDataRef->key;
        indexData->value.copyFrom(indexDataRef->value);
        d->indexDataList.add(indexData);
        d->indexDataMap.set(indexData->key, ZFCorePointerForObject<_ZFP_ZFTextTemplateIndexData *>(indexData));
    }
}

/** @cond ZFPrivateDoc */
zfbool ZFTextTemplateParam::operator == (ZF_IN const ZFTextTemplateParam &ref) const
{
    if(d == ref.d)
    {
        return zftrue;
    }
    if(d->replaceDataList.count() != ref.d->replaceDataList.count()
        || d->enableDataDefault != ref.d->enableDataDefault
        || d->enableDataList.count() != ref.d->enableDataList.count()
        || d->indexDataDefault != ref.d->indexDataDefault
        || d->indexDataList.count() != ref.d->indexDataList.count()
        )
    {
        return zffalse;
    }
    for(zfindex i = 0; i < d->replaceDataList.count(); ++i)
    {
        const _ZFP_ZFTextTemplateReplaceData *replaceData = d->replaceDataList[i];
        const _ZFP_ZFTextTemplateReplaceData *replaceDataRef = ref.d->replaceDataList[i];
        if(replaceData->key != replaceDataRef->key || replaceData->value != replaceDataRef->value)
        {
            return zffalse;
        }
    }
    for(zfindex i = 0; i < d->enableDataList.count(); ++i)
    {
        const _ZFP_ZFTextTemplateEnableData *enableData = d->enableDataList[i];
        const _ZFP_ZFTextTemplateEnableData *enableDataRef = ref.d->enableDataList[i];
        if(enableData->key != enableDataRef->key || enableData->value != enableDataRef->value)
        {
            return zffalse;
        }
    }
    for(zfindex i = 0; i < d->indexDataList.count(); ++i)
    {
        const _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataList[i];
        const _ZFP_ZFTextTemplateIndexData *indexDataRef = ref.d->indexDataList[i];
        if(indexData->key != indexDataRef->key || indexData->value != indexDataRef->value)
        {
            return zffalse;
        }
    }
    return zftrue;
}
/** @endcond */

void ZFTextTemplateParam::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    zfbool first = zftrue;

    if(!d->replaceDataList.isEmpty())
    {
        if(first) {first = zffalse;}
        else {ret += zfText(", ");}

        ret += zfText("replace: [");
        for(zfindex i = 0; i < d->replaceDataList.count(); ++i)
        {
            if(i > 0)
            {
                ret += zfText(", ");
            }
            const _ZFP_ZFTextTemplateReplaceData *replaceData = d->replaceDataList[i];
            zfstringAppend(ret, zfText("<%s, %s>"), replaceData->key.cString(), replaceData->value.cString());
        }
        ret += zfText("]");
    }

    if(!d->enableDataList.isEmpty())
    {
        if(first) {first = zffalse;}
        else {ret += zfText(", ");}

        ret += zfText("enable: [");
        for(zfindex i = 0; i < d->enableDataList.count(); ++i)
        {
            if(i > 0)
            {
                ret += zfText(", ");
            }
            const _ZFP_ZFTextTemplateEnableData *enableData = d->enableDataList[i];
            zfstringAppend(ret, zfText("<%s, %b>"), enableData->key.cString(), enableData->value);
        }
        ret += zfText("]");
    }

    {
        if(first) {first = zffalse;}
        else {ret += zfText(", ");}

        ret += zfText("enableDataDefault: ");
        zfboolToString(ret, this->enableDataDefault());
    }

    if(!d->indexDataList.isEmpty())
    {
        if(first) {first = zffalse;}
        else {ret += zfText(", ");}

        ret += zfText("index: ");
        for(zfindex i = 0; i < d->indexDataList.count(); ++i)
        {
            if(i > 0)
            {
                ret += zfText(", ");
            }
            const _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataList[i];
            ret += zfText("<");
            ret += indexData->key;
            ret += zfText(", ");
            indexData->value.objectInfoT(ret);
            ret += zfText(">");
        }
        ret += zfText("]");
    }

    {
        if(first) {first = zffalse;}
        else {ret += zfText(", ");}

        ret += zfText("indexDataDefault: ");
        d->indexDataDefault.objectInfoT(ret);
    }

    ret += ZFTOKEN_ZFObjectInfoRight;

    ZFUNUSED(first);
}

// ============================================================
ZFPROPERTY_TYPE_DEFINE(ZFTextTemplateParam, ZFTextTemplateParam, {
        if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZFTextTemplateParam(), serializableData, outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        v.replaceDataRemoveAll();
        v.enableDataRemoveAll();
        v.enableDataDefaultSet(zffalse);
        v.indexDataRemoveAll();
        v.indexDataDefaultSet(ZFTextTemplateIndexData());

        const ZFSerializableData *element = zfnull;

        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_replace);
        if(element != zfnull)
        {
            for(zfindex i = 0; i < element->elementCount(); ++i)
            {
                const ZFSerializableData &item = element->elementAtIndex(i);
                const zfchar *key = item.propertyName();
                if(key == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, item, zfText("missing item name"));
                    return zffalse;
                }
                zfstring value;
                if(!zfstringFromSerializableData(value, item, outErrorHint, outErrorPos))
                {
                    return zffalse;
                }
                v.replaceDataAdd(key, value);
            }
        }

        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_enable);
        if(element != zfnull)
        {
            for(zfindex i = 0; i < element->elementCount(); ++i)
            {
                const ZFSerializableData &item = element->elementAtIndex(i);
                const zfchar *key = item.propertyName();
                if(key == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, item, zfText("missing item name"));
                    return zffalse;
                }
                zfbool value = zffalse;
                if(!zfboolFromSerializableData(value, item, outErrorHint, outErrorPos))
                {
                    return zffalse;
                }
                v.enableDataAdd(key, value);
            }
        }

        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_enableDataDefault);
        if(element != zfnull)
        {
            zfbool enableDataDefault = zffalse;
            if(!zfboolFromSerializableData(enableDataDefault, *element, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            v.enableDataDefaultSet(enableDataDefault);
        }

        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_index);
        if(element != zfnull)
        {
            for(zfindex i = 0; i < element->elementCount(); ++i)
            {
                const ZFSerializableData &item = element->elementAtIndex(i);
                const zfchar *key = item.propertyName();
                if(key == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, item, zfText("missing item name"));
                    return zffalse;
                }
                ZFTextTemplateIndexData value;
                if(!ZFTextTemplateIndexDataFromSerializableData(value, item, outErrorHint, outErrorPos))
                {
                    return zffalse;
                }
                v.indexDataAdd(key, value);
            }
        }

        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_indexDataDefault);
        if(element != zfnull)
        {
            ZFTextTemplateIndexData indexDataDefault;
            if(!ZFTextTemplateIndexDataFromSerializableData(indexDataDefault, *element, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            v.indexDataDefaultSet(indexDataDefault);
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        serializableData.itemClassSet(ZFPropertyTypeId_ZFTextTemplateParam());

        if(v.replaceDataCount() > 0)
        {
            ZFSerializableData nodeData;
            for(zfindex i = 0; i < v.replaceDataCount(); ++i)
            {
                ZFSerializableData itemData;
                if(!zfstringToSerializableData(itemData, v.replaceDataAtIndex(i), outErrorHint))
                {
                    return zffalse;
                }
                itemData.propertyNameSet(v.replaceDataNameAtIndex(i));
                nodeData.elementAdd(itemData);
            }

            nodeData.itemClassSet(ZFSerializableKeyword_node);
            nodeData.categorySet(ZFSerializableKeyword_ZFTextTemplateParam_replace);
            serializableData.elementAdd(nodeData);
        }

        if(v.enableDataCount() > 0)
        {
            ZFSerializableData nodeData;
            for(zfindex i = 0; i < v.enableDataCount(); ++i)
            {
                ZFSerializableData itemData;
                if(!zfboolToSerializableData(itemData, v.enableDataAtIndex(i), outErrorHint))
                {
                    return zffalse;
                }
                itemData.propertyNameSet(v.enableDataNameAtIndex(i));
                nodeData.elementAdd(itemData);
            }

            nodeData.itemClassSet(ZFSerializableKeyword_node);
            nodeData.categorySet(ZFSerializableKeyword_ZFTextTemplateParam_enable);
            serializableData.elementAdd(nodeData);
        }

        if(v.enableDataDefault())
        {
            ZFSerializableData nodeData;
            if(!zfboolToSerializableData(nodeData, v.enableDataDefault(), outErrorHint))
            {
                return zffalse;
            }
            nodeData.categorySet(ZFSerializableKeyword_ZFTextTemplateParam_enableDataDefault);
            serializableData.elementAdd(nodeData);
        }

        if(v.indexDataCount() > 0)
        {
            ZFSerializableData nodeData;
            for(zfindex i = 0; i < v.indexDataCount(); ++i)
            {
                ZFSerializableData itemData;
                if(!ZFTextTemplateIndexDataToSerializableData(itemData, *(v.indexDataAtIndex(i)), outErrorHint))
                {
                    return zffalse;
                }
                itemData.propertyNameSet(v.indexDataNameAtIndex(i));
                nodeData.elementAdd(itemData);
            }

            nodeData.itemClassSet(ZFSerializableKeyword_node);
            nodeData.categorySet(ZFSerializableKeyword_ZFTextTemplateParam_index);
            serializableData.elementAdd(nodeData);
        }

        {
            ZFSerializableData nodeData;
            if(!ZFTextTemplateIndexDataToSerializableData(nodeData, v.indexDataDefault(), outErrorHint))
            {
                return zffalse;
            }
            if(nodeData.attributeCount() > 0 || nodeData.elementCount() > 0)
            {
                nodeData.categorySet(ZFSerializableKeyword_ZFTextTemplateParam_indexDataDefault);
                serializableData.elementAdd(nodeData);
            }
        }

        return zftrue;
    })

ZF_NAMESPACE_GLOBAL_END

