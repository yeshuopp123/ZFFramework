/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSerializableDataDef.h
 * @brief serializable data for #ZFSerializable
 */

#ifndef _ZFI_ZFSerializableDataDef_h_
#define _ZFI_ZFSerializableDataDef_h_

#include "ZFObjectDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief keyword for #ZFSerializable to hold a no name node,
 *   which means the data doesn't care about its class name
 *
 * @code
 *   <node key="value" />
 * @endcode
 */
#define ZFSerializableKeyword_node zfText("node")
/**
 * @brief keyword for #ZFSerializable to hold attribute or property's name,
 *   see #ZFSerializableData::propertyName
 */
#define ZFSerializableKeyword_name zfText("name")
/**
 * @brief keyword for #ZFSerializable to hold attribute or property's value,
 *   see #ZFSerializableData::propertyValue
 */
#define ZFSerializableKeyword_value zfText("value")
/**
 * @brief keyword for #ZFSerializable to hold the reference's type,
 *   see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 */
#define ZFSerializableKeyword_refType zfText("refType")
/**
 * @brief keyword for #ZFSerializable to hold the reference's data,
 *   see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 */
#define ZFSerializableKeyword_refData zfText("refData")
/**
 * @brief keyword for #ZFSerializable's to hold category,
 *   see #ZFSerializableData::category
 */
#define ZFSerializableKeyword_category zfText("category")
/**
 * @brief keyword for #ZFSerializable's to hold whether it's under editMode,
 *   see #ZFSerializable::editModeData
 */
#define ZFSerializableKeyword_editMode zfText("editMode")
/**
 * @brief used to declare a null object
 *
 * @code
 *   <null>
 * @endcode
 * serializing this would result a null with success state\n
 * the actual type for the null is determined by the owner
 */
#define ZFSerializableKeyword_null zfText("null")

// ============================================================
zfclassFwd ZFSerializable;
zfclassFwd _ZFP_ZFSerializableDataPrivate;
/**
 * @brief ZFSerializable's data container, see #ZFSerializable
 *
 * this is a data structure similar to xml,
 * designed to reduce xml dependency
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFSerializableData
{
public:
    /** @cond ZFPrivateDoc */
    ZFSerializableData(ZF_IN _ZFP_ZFSerializableDataPrivate *d);
    /** @endcond */
    /**
     * @brief construct an empty data
     */
    ZFSerializableData(void);
    /**
     * @brief retain from another ref
     */
    ZFSerializableData(ZF_IN const ZFSerializableData &ref);
    /**
     * @brief retain from another ref
     */
    ZFSerializableData &operator =(ZF_IN const ZFSerializableData &ref);
    ~ZFSerializableData(void);

public:
    /**
     * @brief copy all the contents
     */
    zffinal void copyFrom(ZF_IN const ZFSerializableData &ref);
    /**
     * @brief return a copy of this data
     */
    inline ZFSerializableData copy(void) const
    {
        ZFSerializableData ret;
        ret.copyFrom(*this);
        return ret;
    }

    /**
     * @brief get current retain count
     */
    zffinal zfindex objectRetainCount(void) const;

    // ============================================================
    // reference logic
public:
    /**
     * @brief store reference info of this node, see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
     */
    zffinal void referenceRefTypeSet(ZF_IN const zfchar *refType);
    /** @brief see #referenceRefTypeSet */
    zffinal const zfchar *referenceRefType(void) const;
    /** @brief see #referenceRefTypeSet */
    zffinal void referenceRefDataSet(ZF_IN const zfchar *refData);
    /** @brief see #referenceRefTypeSet */
    zffinal const zfchar *referenceRefData(void) const;

    /**
     * @brief remove reference info attached to this data
     */
    zffinal void referenceInfoRemove(void);
    /**
     * @brief remove reference info attached to this data and all child data
     */
    zffinal void referenceInfoRemoveRecursively(void);

    /**
     * @brief true if this node contains reference info
     */
    zffinal zfbool referenceInfoExist(void) const;
    /**
     * @brief true if this node or child node contains reference info
     */
    zffinal zfbool referenceInfoExistRecursively(void) const;

private:
    zffinal zfbool _ZFP_ZFSerializableData_referenceInfoLoad(ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                             ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zffinal zfbool _ZFP_ZFSerializableData_referenceInfoApply(ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                              ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
public:
    /**
     * @brief load data accorrding to reference info, see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
     *
     * this method should be called on root node for only once
     */
    zffinal zfbool referenceInfoLoad(ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                     ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    /**
     * @brief restore to original serializable data accorrding to refNode
     *   which holds reference info, see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
     *
     * this method would:
     * -  remove duplicated items in self, accorrding to refNode
     * -  restore refType/refData accorrding to refNode
     */
    zffinal void referenceInfoRestore(ZF_IN const ZFSerializableData &refNode);

    // ============================================================
    // parent
public:
    /**
     * @brief access parent of this node, return true if parent exists
     */
    zffinal zfbool serializableDataParent(ZF_OUT ZFSerializableData &ret) const;

    // ============================================================
    // class
public:
    /** @brief see #ZFSerializable */
    zffinal void itemClassSet(ZF_IN const zfchar *clsName);
    /** @brief see #ZFSerializable */
    zffinal const zfchar *itemClass(void) const;

    // ============================================================
    // other types
public:
    /** @brief see #ZFSerializable, same as set value of attribute "name" */
    inline void propertyNameSet(ZF_IN const zfchar *name)
    {
        this->attributeSet(ZFSerializableKeyword_name, name);
    }
    /** @brief see #propertyNameSet */
    inline const zfchar *propertyName(void) const
    {
        return this->attributeForName(ZFSerializableKeyword_name);
    }
    /** @brief see #ZFSerializable, same as set value of attribute "value" */
    inline void propertyValueSet(ZF_IN const zfchar *value)
    {
        this->attributeSet(ZFSerializableKeyword_value, value);
    }
    /** @brief see #propertyValueSet */
    inline const zfchar *propertyValue(void) const
    {
        return this->attributeForName(ZFSerializableKeyword_value);
    }
    /** @brief see #ZFSerializable, same as set value of attribute "category" */
    inline void categorySet(ZF_IN const zfchar *category)
    {
        this->attributeSet(ZFSerializableKeyword_category, category);
    }
    /** @brief see #categorySet */
    inline const zfchar *category(void) const
    {
        return this->attributeForName(ZFSerializableKeyword_category);
    }
    /** @brief see #ZFSerializable, same as set value of attribute "editMode" */
    inline void editModeSet(ZF_IN zfbool editMode)
    {
        this->attributeSet(ZFSerializableKeyword_editMode, editMode ? ZFTOKEN_zfbool_zftrue : zfnull);
    }
    /** @brief see #categorySet */
    inline zfbool editMode(void) const
    {
        return (this->attributeForName(ZFSerializableKeyword_editMode) != zfnull);
    }

    // ============================================================
public:
    /**
     * @brief clear all the contents
     */
    zffinal void removeAll(void);

    // ============================================================
public:
    /**
     * @brief used to store extra state for this serializable data
     *
     * typically used by impl to achieve additional features
     */
    zffinal void serializableDataTagSet(ZF_IN const zfchar *key,
                                        ZF_IN ZFObject *tag,
                                        ZF_IN_OPT zfbool autoMarkCached = zffalse);
    /**
     * @brief #serializableDataTagSet and automatically mark tag as #ZFObject::objectCached
     */
    zffinal inline void serializableDataTagSetMarkCached(ZF_IN const zfchar *key,
                                                         ZF_IN ZFObject *tag)
    {
        this->serializableDataTagSet(key, tag, zftrue);
    }
    /**
     * @brief see #serializableDataTagSet
     */
    zffinal ZFObject *serializableDataTagGet(ZF_IN const zfchar *key) const;
    /** @brief see #serializableDataTagSet */
    template<typename T_ZFObject>
    T_ZFObject serializableDataTagGet(ZF_IN const zfchar *key) const
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->serializableDataTagGet(key));
    }
    /** @brief see #serializableDataTagSet */
    zffinal void serializableDataTagGetAllKeyValue(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &allKey,
                                                   ZF_IN_OUT ZFCoreArrayPOD<ZFObject *> &allValue) const;
    /**
     * @brief remove tag, same as set tag to null
     */
    zffinal void serializableDataTagRemove(ZF_IN const zfchar *key);
    /**
     * @brief remove tag, return removed tag or #zfautoObjectNull if not exist
     */
    zffinal zfautoObject serializableDataTagRemoveAndGet(ZF_IN const zfchar *key);
    /**
     * @brief see #serializableDataTagSet
     *
     * @note it's unsafe to remove all tag manually,
     *   which may break unrelated modules' additional logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    zffinal void serializableDataTagRemoveAll(void);

    // ============================================================
    // attribute
public:
    /**
     * @brief see #ZFSerializable, remove if value is null (same as #attributeRemove)
     *
     * note that order of attributes is not ensured
     * after you add, remove or modify attributes
     */
    zffinal void attributeSet(ZF_IN const zfchar *name,
                             ZF_IN const zfchar *value);
    /** @brief see #attributeSet */
    zffinal const zfchar *attributeForName(ZF_IN const zfchar *name) const;

    /** @brief see #attributeSet */
    zffinal zfindex attributeCount(void) const;

    /**
     * @brief remove value with name, see #attributeSet
     */
    zffinal void attributeRemove(ZF_IN const zfchar *name);

    /**
     * @brief remove all attribute
     */
    zffinal void attributeRemoveAll(void);

    /** @brief see #zfiterator */
    zffinal zfiterator attributeIteratorForName(ZF_IN const zfchar *name) const;
    /** @brief see #zfiterator */
    zffinal zfiterator attributeIterator(void) const;
    /** @brief see #zfiterator */
    zffinal zfbool attributeIteratorIsValid(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal zfbool attributeIteratorIsEqual(ZF_IN const zfiterator &it0,
                                            ZF_IN const zfiterator &it1) const;
    /** @brief see #zfiterator */
    zffinal void attributeIteratorSet(ZF_IN_OUT zfiterator &it,
                                      ZF_IN const zfchar *value);
    /** @brief see #zfiterator */
    zffinal void attributeIteratorRemove(ZF_IN_OUT zfiterator &it);
    /** @brief see #zfiterator */
    zffinal const zfchar *attributeIteratorGetKey(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal const zfchar *attributeIteratorGet(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal const zfchar *attributeIteratorNextKey(ZF_IN_OUT zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal const zfchar *attributeIteratorNext(ZF_IN_OUT zfiterator &it) const;

    /** @brief see #zfiterator */
    zffinal zfbool attributeIteratorResolved(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal void attributeIteratorResolveMark(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal void attributeIteratorResolveUnmark(ZF_IN const zfiterator &it) const;

    // ============================================================
    // element
public:
    /**
     * @brief add child element
     *
     * note this method won't check duplicate data with same name
     */
    zffinal void elementAdd(ZF_IN const ZFSerializableData &element);
    /**
     * @brief add child element
     *
     * note this method won't check duplicate data with same name
     */
    zffinal void elementAdd(ZF_IN const ZFSerializableData &element,
                            ZF_IN zfindex atIndex);

    /**
     * @brief replace existing element at index or assert fail if index out of range
     */
    zffinal void elementSetAtIndex(ZF_IN zfindex index,
                                   ZF_IN const ZFSerializableData &element);

    /**
     * @brief find element with name or zfindexMax if not found
     */
    zffinal zfindex elementFindByName(ZF_IN const zfchar *name,
                                      ZF_IN_OPT zfbool skipResolved = zffalse) const;
    /**
     * @brief see #elementFindByName
     */
    zffinal zfindex elementFindByCategory(ZF_IN const zfchar *name,
                                          ZF_IN_OPT zfbool skipResolved = zffalse) const;

    /** @brief see #ZFSerializable */
    zffinal zfindex elementCount(void) const;
    /** @brief see #ZFSerializable */
    zffinal const ZFSerializableData &elementAtIndex(ZF_IN zfindex index) const;
    /** @brief see #ZFSerializable */
    zffinal ZFSerializableData &elementAtIndex(ZF_IN zfindex index);

    /**
     * @brief remove element at index
     */
    zffinal void elementRemoveAtIndex(ZF_IN zfindex index);
    /**
     * @brief remove all element
     */
    zffinal void elementRemoveAll(void);

    // ============================================================
    // resolved state
public:
    /**
     * @brief used to mark whether the data is resolved
     *
     * useful while serializing some type from serializable,
     * after operation's done,
     * you may check whether all data resolved
     * to see whether there's extra unrecognized data
     */
    zffinal zfbool resolved(void) const;
    /** @brief see #resolved */
    zffinal void resolveMark(void) const;
    /** @brief see #resolved */
    zffinal void resolveUnmark(void) const;

    /** @brief see #resolved */
    zffinal zfbool resolvedAttribute(ZF_IN const zfchar *name) const;
    /** @brief see #resolved */
    zffinal void resolveAttributeMark(ZF_IN const zfchar *name) const;
    /** @brief see #resolved */
    zffinal void resolveAttributeUnmark(ZF_IN const zfchar *name) const;

    /** @brief see #resolved */
    zffinal zfbool resolvedPropertyName(void) const
    {
        return this->resolvedAttribute(ZFSerializableKeyword_name);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyNameMark(void) const
    {
        this->resolveAttributeMark(ZFSerializableKeyword_name);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyNameUnmark(void) const
    {
        this->resolveAttributeUnmark(ZFSerializableKeyword_name);
    }

    /** @brief see #resolved */
    zffinal zfbool resolvedPropertyValue(void) const
    {
        return this->resolvedAttribute(ZFSerializableKeyword_value);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyValueMark(void) const
    {
        this->resolveAttributeMark(ZFSerializableKeyword_value);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyValueUnmark(void) const
    {
        this->resolveAttributeUnmark(ZFSerializableKeyword_value);
    }

    /** @brief see #resolved */
    zffinal zfbool resolvedCategory(void) const
    {
        return this->resolvedAttribute(ZFSerializableKeyword_category);
    }
    /** @brief see #resolved */
    zffinal void resolveCategoryMark(void) const
    {
        this->resolveAttributeMark(ZFSerializableKeyword_category);
    }
    /** @brief see #resolved */
    zffinal void resolveCategoryUnmark(void) const
    {
        this->resolveAttributeUnmark(ZFSerializableKeyword_category);
    }

    /**
     * @brief see #resolved, check whether self and all attribute and child element resolved
     */
    zffinal zfbool resolvedAll(ZF_OUT_OPT const ZFSerializableData **firstNotResolvedElement = zfnull,
                               ZF_OUT_OPT zfstring *firstNotResolvedAttribute = zfnull) const;
    /**
     * @brief see #resolved, mark self and all attribute and child element
     */
    zffinal void resolveMarkAll(void) const;
    /**
     * @brief see #resolved, unmark self and all attribute and child element
     */
    zffinal void resolveUnmarkAll(void) const;
    /**
     * @brief see #resolved, mark all attribute
     */
    zffinal void resolveAttributeMarkAll(void) const;
    /**
     * @brief see #resolved, unmark all attribute
     */
    zffinal void resolveAttributeUnmarkAll(void) const;

    // ============================================================
    // other functions
public:
    /**
     * @brief get a short info
     */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #objectInfoT */
    zffinal zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }
    /**
     * @brief compare another serializable data
     *
     * if two data considered as same,
     * attribute's order have no need to be same,
     * but child element's order must be same
     */
    zffinal ZFCompareResult objectCompare(ZF_IN const ZFSerializableData &another) const;

public:
    /**
     * @brief true if the data contains nothing
     */
    zffinal zfbool isEmpty(void) const;

private:
    _ZFP_ZFSerializableDataPrivate *d;
    friend zfclassFwd _ZFP_ZFSerializableDataPrivate;
};

ZFCOMPARER_DEFAULT_DECLARE_BEGIN(ZFSerializableData, e0, ZFSerializableData, e1)
{
    return e0.objectCompare(e1);
}
ZFCOMPARER_DEFAULT_DECLARE_END(ZFSerializableData, e0, ZFSerializableData, e1)

/**
 * @brief usually for debug use only
 *
 * macro names are recommended to be ZFSerializableDataRefType_XXX
 */
extern ZF_ENV_EXPORT void ZFSerializableDataRefTypeGetAll(ZF_OUT ZFCoreArray<const zfchar *> &ret);
/** @brief see #ZFSerializableDataRefTypeGetAll */
inline ZFCoreArrayPOD<const zfchar *> ZFSerializableDataRefTypeGetAll(void)
{
    ZFCoreArrayPOD<const zfchar *> ret;
    ZFSerializableDataRefTypeGetAll(ret);
    return ret;
}

typedef zfbool (*_ZFP_ZFSerializableDataReferenceCallback)(ZF_OUT ZFSerializableData &serializableData,
                                                           ZF_IN const zfchar *data,
                                                           ZF_OUT_OPT zfstring *outErrorHintToAppend,
                                                           ZF_OUT_OPT ZFSerializableData *outErrorPos);
extern ZF_ENV_EXPORT void _ZFP_ZFSerializableDataReferenceTypeRegister(ZF_IN const zfchar *referenceType,
                                                                       ZF_IN _ZFP_ZFSerializableDataReferenceCallback callback);
extern ZF_ENV_EXPORT void _ZFP_ZFSerializableDataReferenceTypeUnregister(ZF_IN const zfchar *referenceType);

#define _ZFP_ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(referenceTypeName, outSerializableDataName, inDataName, outErrorHintToAppend, outErrorPos) \
    static zfbool _ZFP_ZFSerializableDataReferenceCallback_##referenceTypeName( \
        ZF_OUT ZFSerializableData &outSerializableDataName, \
        ZF_IN const zfchar *inDataName, \
        ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull, \
        ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(_ZFP_ZFSerializableDataReferenceTypeRegister_##referenceTypeName, \
                                          ZFLevelZFFrameworkNormal) \
    { \
        _ZFP_ZFSerializableDataReferenceTypeRegister( \
            ZFM_TOSTRING(referenceTypeName), \
            _ZFP_ZFSerializableDataReferenceCallback_##referenceTypeName); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(_ZFP_ZFSerializableDataReferenceTypeRegister_##referenceTypeName) \
    { \
        _ZFP_ZFSerializableDataReferenceTypeUnregister(ZFM_TOSTRING(referenceTypeName)); \
    } \
    ZF_GLOBAL_INITIALIZER_END(_ZFP_ZFSerializableDataReferenceTypeRegister_##referenceTypeName) \
    static zfbool _ZFP_ZFSerializableDataReferenceCallback_##referenceTypeName( \
        ZF_OUT ZFSerializableData &outSerializableDataName, \
        ZF_IN const zfchar *inDataName, \
        ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */, \
        ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
/**
 * @brief advanced data reference logic for serializable
 *
 * used to reference another existing serializable data, typical example:
 * @code
 *   <OwnerType>
 *       <PropertyType refType="YourType" refData="YourType's reference data"
 *           yourExtraAttributeKey="yourExtraAttributeValue" >
 *           <YourExtraChildren />
 *       </PropertyType>
 *   </OwnerType>
 * @endcode
 * in this example, we declare a referenced node:
 * -  attribute "refType" shows the reference type,
 *   which was registered by #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 * -  attribute "refData" shows the data for the reference,
 *   may be file path or content data or others,
 *   which would be decoded by the method declared by #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 * -  you may add extra attributes or children to ref node,
 *   which would be appended to result decoded data
 *   (existing attributes would be replaced, and existing children would be appended normally)
 *
 * for example, assume we have two xml files:
 * @code
 *   // 1.xml
 *   <Holder>
 *       <MyProperty refType="myXmlParseLogic"
 *           refData="2.xml"
 *           myAttr1="myValue1" >
 *           <MyChild1 />
 *       </MyProperty>
 *   </Holder>
 *   // 2.xml
 *   <AnyType myAttr1="myValue1 replaced"
 *       myAttr2="myValue2" >
 *       <MyChild2 />
 *   </AnyType>
 * @endcode
 * then the decoded data would be like:
 * @code
 *   <Holder>
 *       <MyProperty myAttr1="myValue1 replaced"
 *           myAttr2="myValue2" >
 *           <MyChild1 />
 *           <MyChild2 />
 *       </MyProperty>
 *   </Holder>
 * @endcode
 * \n
 * to declare your own type:
 * @code
 *   // in cpp file only
 *   ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(yourType, serializableData, data, outErrorHintToAppend, outErrorPos)
 *   {
 *       // you decode operation here
 *       // decode (const zfchar *)data to (ZFSerializableData)serializableData
 *
 *       // you must call #ZFSerializableData::referenceInfoLoad to finish the decode step
 *       return serializableData.referenceInfoLoad();
 *   }
 * @endcode
 *
 * REMARKS:\n
 * typical serialize from data steps:
 * -# load serializable data from source, e.g. file or buffer
 * -# resolve reference by #ZFSerializableData::referenceInfoLoad
 * -# serialize each item for owner serializable, and store necessary reference info
 *
 * typical serialize to data step:
 * -# check whether your item has previous stored reference info, serialize to data accorrding it
 * -# restore reference info by #ZFSerializableData::referenceInfoRestore
 * -# save serializable data to result, e.g. file or buffer
 *
 * usually, the #ZFSerializable and #ZFSerializableUtilSerializeAttributeFromData series util macro would do the work for you
 */
#define ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(referenceTypeName, serializableData, data, outErrorHintToAppend, outErrorPos) \
    _ZFP_ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(referenceTypeName, serializableData, data, outErrorHintToAppend, outErrorPos)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDataDef_h_

#include "ZFSerializableDataStringConverterDef.h"

