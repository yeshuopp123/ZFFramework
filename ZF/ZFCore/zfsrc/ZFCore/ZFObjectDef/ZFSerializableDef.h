/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSerializableDef.h
 * @brief serializable interface
 */

#ifndef _ZFI_ZFSerializableDef_h_
#define _ZFI_ZFSerializableDef_h_

#include "ZFSerializableDataDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief keyword for #ZFSerializable to hold styleable's type,
 *   see #ZFSerializable::serializableStyleableTypeSet
 */
#define ZFSerializableKeyword_styleableType zfText("styleableType")
/**
 * @brief keyword for #ZFSerializable to hold styleable's data,
 *   see #ZFSerializable::serializableStyleableTypeSet
 */
#define ZFSerializableKeyword_styleableData zfText("styleableData")

/**
 * @brief used for #ZFSerializable to override default constructor
 *
 * by default, serializable would be created by #ZFClass::newInstance while serializing from data,
 * you may supply this method to override:
 * -  static zfautoObject serializableNewInstance(void);
 *
 * the method should be supplied as #ZFMethod, and is recommended to register it statically by #ZFMETHOD_REGISTER\n
 * the method should return a newly created object, or retain your existing singleton instance\n
 * typically this method is used to achieve some singleton logic
 */
#define ZFSerializableKeyword_serializableNewInstanceSig serializableNewInstance
/**
 * @brief see #ZFSerializableKeyword_serializableNewInstanceSig
 */
#define ZFSerializableKeyword_serializableNewInstance ZFM_TOSTRING(ZFSerializableKeyword_serializableNewInstanceSig)

zfclassFwd _ZFP_I_ZFSerializable_PropertyTypeHolder;
// ============================================================
/**
 * @brief base class of call serializable object
 *
 * a serializable object can be encoded to and decoded from a string data,
 * use #ZFSerializableData to store necessary data\n
 * a ZFSerializableData can hold these datas:
 * -  serializable class:
 *   ZFObject's class name or other non-ZFObject's type name,
 *   such as "ZFString", "zfstring" and "zfint"
 * -  reference info:
 *   used to hold reference info,
 *   see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE for more info
 * -  property name:
 *   used only when the serializable belongs to another serializable,
 *   it's the property name,
 *   and is ensured stored in attributes with "name" as the attribute name
 * -  property value:
 *   used only when the serializable can be converted directly to a type,
 *   and is ensured stored in attributes with "value" as the attribute name
 * -  category:
 *   used to mark the node should be resolved specially,
 *   and is ensured stored in attributes with "category" as the attribute name
 * -  attributes:
 *   used to hold extra type datas for the owner
 * -  elements:
 *   used to hold properties of the serializable,
 *   it's a ZFSerializableData too
 *
 * ZFSerializableData can be converted from and to xml elements,
 * to make it easier to understand,
 * here's a typical serializable data in xml format that shows the types:
 * @code
 *   // assume we have a object hold a ZFArray as retain property:
 *   zfclass TestClass : zfextends ZFSerializable
 *   {
 *       ZFOBJECT_DECLARE(TestClass, ZFSerializable)
 *       ZFPROPERTY_RETAIN(ZFArray *, testProperty)
 *       ...
 *   };
 *
 *   // we have a ZFSerializableData like:
 *   <TestClass test="test">
 *       <ZFArray name="testProperty">
 *           <ZFString value="string content" />
 *       </ZFArray>
 *       <SomeType category="CategoryName" />
 *   </TestClass>
 * @endcode
 * in this example:
 * -  the "TestClass" in "<TestClass>" is a serializable class
 * -  the "testProperty" in "<ZFArray name="testProperty">" is a property name
 * -  the "string content" in "<ZFString value="string content" />" is a property value
 * -  the "test="test"" in "<TestClass test="test">" is a attribute
 * -  the "category" in "<SomeType category="CategoryName" />" is a category
 *   that should be resolved by subclass during #serializableOnSerializeFromData
 *
 * we have these builtin keywords for serializable data,
 * you should not use them as attribute name:
 * -  "name":
 *   shows the serializable is it's parent's property,
 *   and the name is the property's name
 * -  "value":
 *   for basic type only, the value for the basic type
 * -  "refType", "refData":
 *   for advanced reference logic,
 *   see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 * -  "category":
 *   if exist this attribute,
 *   ZFSerializable would ignore this node and leave it to subclass to decode
 * -  "editMode":
 *   if exist this attribute,
 *   ZFSerializable would ignore this node and store it as raw data for future process,
 *   see #ZFSerializable::editModeData
 * -  "styleableType", "styleableData"
 *   for advanced styleable logic, use #ZFObjectCreate, see #ZFSTYLE_DEFAULT_DECLARE for more info
 *
 * \n
 * a simplest way to implement ZFSerializable is:
 * -  declare your object, and make it implement ZFSerializable,
 *   by default, all #ZFProperty would be auto serialized
 *
 * \n
 * if your object has extra serialize step to do, you may want to:
 * -  override #serializableOnCheck to check serializable depending on current object state
 * -  override one or more of these methods to achieve custom serialize step:
 *   -  #serializableOnSerializePropertyFromData / #serializableOnSerializePropertyToData
 *   -  #serializableOnSerializeEmbededPropertyFromData / #serializableOnSerializeEmbededPropertyToData
 *   -  #serializableOnSerializeFromData / #serializableOnSerializeToData
 * -  take care of referencedOwnerOrNull while serialize to data,
 *   where you should implement reference logic for your custom serialize step\n
 *   by default, serializable property and embeded property's reference logic
 *   would be done by ZFSerializable automatically,
 *   but you should take care of category's reference logic manually
 *
 *
 * \n
 * typically you should override
 * #serializableOnSerializeFromData and #serializableOnSerializeToData
 * to supply custom serialize step\n
 * \n
 * ADVANCED:\n
 * serializable would be created by #ZFClass::newInstance while serializing from data,
 * you may supply your custom constructor,
 * see #ZFSerializableKeyword_serializableNewInstance\n
 * \n
 * ADVANCED:\n
 * serializable also supply styleable logic:
 * @code
 *   <YourSerializable styleableType="type" styleableData="data" />
 * @endcode
 * the reserved keyword "styleableType" and "styleableData" shows the styleable's source,
 * which would be created by #ZFObjectCreate,
 * created object must be #ZFSerializable and #ZFStyleable,
 * and would be copied to the object being serialized by #ZFStyleable::styleableCopyFrom\n
 * for typical styleable usage, see #ZFSTYLE_DEFAULT_DECLARE
 * @note styleable logic and reference logic can not be used together
 * @note comparing with reference logic,
 *   styleable logic would create a styleable object first then copy style from it,
 *   reference logic would load reference to serializable data then serialize from it,
 *   usually reference logic would have better performance,
 *   and styleable logic would have better flexibility
 */
zfinterface ZF_ENV_EXPORT ZFSerializable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(ZFSerializable, ZFInterface)

    // ============================================================
    // edit mode
public:
    /** @brief see #ZFSerializable::editModeData */
    zfclassLikePOD ZF_ENV_EXPORT EditModeData
    {
    public:
        /** @brief see #ZFSerializable::editModeData */
        const ZFClass *wrappedClass;
    };
    /**
     * @brief internal use only
     *
     * map of <className, #ZFSerializable::EditModeData>\n
     * used to store class data that currently not registered,
     * so that it can be serialized to data without data loss\n
     * \n
     * for normal serialize logic, we will reflect class type by #ZFClass::classForName,
     * so if the class is not registered currently,
     * we are unable to find it,
     * such as some plugin designed module,
     * can't be found until plugin has been loaded\n
     * to resolve the problem, we introduced this editMode,
     * which can map unknown type to existing class,
     * so that unknown type's serialize step can be done normally
     * with the logic of existing class\n
     * \n
     * edit mode data stores unresolved class name and serializable data to
     * #editModeWrappedClassName and #editModeWrappedElementDatas,
     * which should be resolved later
     */
    static ZFCoreMap &editModeData(void);
    /** @brief see #ZFSerializable::editModeData */
    static zfbool &editMode(void);
private:
    zfchar *_ZFP_ZFSerializable_editModeWrappedClassName;
    ZFCoreArray<ZFSerializableData> *_ZFP_ZFSerializable_editModeWrappedElementDatas;
public:
    /** @brief see #ZFSerializable::editModeData */
    virtual const zfchar *editModeWrappedClassName(void);
    /** @brief see #ZFSerializable::editModeData */
    virtual void editModeWrappedClassNameSet(ZF_IN const zfchar *value);
    /** @brief see #ZFSerializable::editModeData */
    virtual ZFCoreArray<ZFSerializableData> &editModeWrappedElementDatas(void);

    // ============================================================
protected:
    /** @cond ZFPrivateDoc */
    ZFSerializable(void)
    : _ZFP_ZFSerializable_editModeWrappedClassName(zfnull)
    , _ZFP_ZFSerializable_editModeWrappedElementDatas(zfnull)
    , _ZFP_ZFSerializable_referenceInfoMap(zfnull)
    , _ZFP_ZFSerializable_styleableType(zfnull)
    , _ZFP_ZFSerializable_styleableData(zfnull)
    {
    }
    /** @endcond */
    virtual ~ZFSerializable(void);

public:
    /**
     * @brief true if object is currently serializable, see #ZFSerializable
     *
     * subclass should override #serializableOnCheck to check whether serializable\n
     * some object may be serializable or not depends on content
     * @note you must check super's state first if override
     * @see ZFSerializable
     */
    zffinal zfbool serializable(void);
    /**
     * @brief serialize from data, see #ZFSerializable
     *
     * note that for performance, this method won't check whether serializable before execute
     */
    zffinal zfbool serializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                     ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                     ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    /**
     * @brief serialize to data, see #ZFSerializable
     *
     * note that for performance, this method won't check whether serializable before execute
     */
    zffinal zfbool serializeToData(ZF_OUT ZFSerializableData &serializableData,
                                   ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                   ZF_IN_OPT ZFSerializable *referencedObject = zfnull);

private:
    zffinal _ZFP_I_ZFSerializable_PropertyTypeHolder *_ZFP_ZFSerializable_getPropertyTypeHolder(void);
public:
    /**
     * @brief get all serializable property, usually for debug only, see #serializableOnCheckPropertyType
     */
    zffinal void serializableGetAllSerializableProperty(ZF_OUT ZFCoreArray<const ZFProperty *> &ret);
    /** @brief see #serializableGetAllSerializableProperty */
    inline ZFCoreArrayPOD<const ZFProperty *> serializableGetAllSerializableProperty(void)
    {
        ZFCoreArrayPOD<const ZFProperty *> ret;
        this->serializableGetAllSerializableProperty(ret);
        return ret;
    }
    /**
     * @brief get all serializable embeded property, usually for debug only, see #serializableOnCheckPropertyType
     */
    zffinal void serializableGetAllSerializableEmbededProperty(ZF_OUT ZFCoreArray<const ZFProperty *> &ret);
    /** @brief see #serializableGetAllSerializableEmbededProperty */
    inline ZFCoreArrayPOD<const ZFProperty *> serializableGetAllSerializableEmbededProperty(void)
    {
        ZFCoreArrayPOD<const ZFProperty *> ret;
        this->serializableGetAllSerializableEmbededProperty(ret);
        return ret;
    }

public:
    /**
     * @brief serializable property type, see #ZFSerializable::serializableOnCheckPropertyType
     */
    typedef enum {
        /**
         * @brief see #ZFSerializable::serializableOnCheckPropertyType
         */
        PropertyTypeNotSerializable,
        /**
         * @brief see #ZFSerializable::serializableOnCheckPropertyType
         */
        PropertyTypeSerializableProperty,
        /**
         * @brief see #ZFSerializable::serializableOnCheckPropertyType
         */
        PropertyTypeEmbededProperty,
    } PropertyType;
protected:
    /**
     * @brief check the property type that serializable should do what while serializing
     *
     * properties declared in ZFSerializalbe have these types:
     * -  not serializable:
     *   -  the property is not serializable and should be manually serialized if necessary
     * -  normal serializable property:
     *   -  the property would be serialized automatically
     *     during #serializableOnSerializePropertyFromData and #serializableOnSerializePropertyToData
     *   -  while serializing from data,
     *     ZFSerializable will serialize a property's new instance and then set to the property
     *   -  by default, a property is treated as normal serializable property if:
     *     -  the property's setter and getter is not private
     *     -  the property is retain property and its type is ZFSerializable
     *     -  the property is assign property and its type is registered by #ZFPROPERTY_TYPE_DECLARE
     * -  embeded serializable property:\n
     *   -  the property would be serialized automatically
     *     during #serializableOnSerializeEmbededPropertyFromData and #serializableOnSerializeEmbededPropertyToData
     *   -  while serializing from data,
     *     ZFSerializable will directly serialize the data to property instance
     *     (do nothing if property is null)
     *   -  by default, a property is treated as normal serializable property if:
     *     -  the property is retain property and its type is ZFSerializable
     *     -  the property's setter is private and getter is not private
     *   -  if a property is an embeded property,
     *     you must ensure it's not null while serializing,
     *     otherwise, serializing would fail
     *
     * \n
     * subclass may override this method to make ZFSerializable ignore or force serialize some property,
     * but you must make sure it's logical valid\n
     * ignored property (i.e. PropertyTypeNotSerializable) can be manually serialized
     * during #serializableOnSerializeFromData and #serializableOnSerializeToData
     */
    virtual ZFSerializable::PropertyType serializableOnCheckPropertyType(ZF_IN const ZFProperty *property);

protected:
    /**
     * @brief see #serializable
     */
    virtual zfbool serializableOnCheck(void)
    {
        return zftrue;
    }

    /**
     * @brief called to prepare serialize
     */
    virtual inline void serializableOnSerializeFromDataPrepare(ZF_IN const ZFSerializableData &serializableData)
    {
    }

    /**
     * @brief called to prepare serialize
     */
    virtual inline void serializableOnSerializeToDataPrepare(ZF_IN const ZFSerializableData &serializableData)
    {
    }

    /**
     * @brief for serializable data that has "category" attribute,
     *   ZFSerializable would ignore it and leave it to subclass to resolve,
     *   see #ZFSerializable
     *
     * while overriding this method, you should call super first,
     * and then check whether super has resolved the data\n
     * if subclass should resolve the category,
     * you should mark data as resolved and return whether resolve success\n
     * if not, subclass should leave the data unresoved and return true
     */
    virtual inline zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                          ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                          ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull)
    {
        return zftrue;
    }
    /**
     * @brief corresponding to #serializableOnSerializeFromData,
     *   return whether the task is success,
     *   see #ZFSerializable
     */
    virtual inline zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                        ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                        ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull)
    {
        return zftrue;
    }
    /**
     * @brief see #serializableOnCheckPropertyType, usually you have no need to override this method,
     *   see #ZFSerializable
     *
     * if subclass override this method, you should check whether it's resolved by parent,
     * and then mark data as resolved and return whether resolve success
     * @code
     *   zfbool YourType::serializableOnSerializePropertyFromData(...)
     *   {
     *       if(!SuperSerializable::serializableOnSerializePropertyFromData(...))
     *       {
     *           return zffalse;
     *       }
     *       if(categoryData.resolved())
     *       {
     *           return zftrue;
     *       }
     *
     *       // mark resolve if you have resolved
     *       // or don't mark to leave it to subclass
     *       propertyData.resolveMark();
     *
     *       return zftrue;
     *   }
     * @endcode
     */
    virtual zfbool serializableOnSerializePropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                           ZF_IN const ZFProperty *property,
                                                           ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                           ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    /**
     * @brief see #serializableOnCheckPropertyType, usually you have no need to override this method,
     *   see #ZFSerializable
     *
     * set serializable class to null to show the property is in init value state
     * and have no need to be serialized
     */
    virtual zfbool serializableOnSerializePropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                         ZF_IN const ZFProperty *property,
                                                         ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                         ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
    /**
     * @brief see #serializableOnCheckPropertyType, usually you have no need to override this method,
     *   see #ZFSerializable
     */
    virtual zfbool serializableOnSerializeEmbededPropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                                  ZF_IN const ZFProperty *property,
                                                                  ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                                  ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    /**
     * @brief see #serializableOnCheckPropertyType, usually you have no need to override this method,
     *   see #ZFSerializable
     *
     * set serializable class to null to show the property is in init value state
     * and have no need to be serialized
     */
    virtual zfbool serializableOnSerializeEmbededPropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                                ZF_IN const ZFProperty *property,
                                                                ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                                ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);

public:
    /**
     * @brief get info as a serializable
     */
    virtual void serializableGetInfoT(ZF_IN_OUT zfstring &ret);
    /** @brief see #serializableGetInfoT */
    virtual inline zfstring serializableGetInfo(void)
    {
        zfstring ret;
        this->serializableGetInfoT(ret);
        return ret;
    }

public:
    /**
     * @brief internal use only
     *
     * used to copy serializable info from another serializable,
     * so that this object can serialize to data with the same behavior
     * of the source serializable object\n
     * the anotherSerializable must be same as this object
     */
    virtual void serializableCopyInfoFrom(ZF_IN ZFSerializable *anotherSerializable);

private:
    ZFCoreMap *_ZFP_ZFSerializable_referenceInfoMap; // ZFSerializableData *
public:
    /**
     * @brief for impl to store reference info, so that it can be restored when serialize back to data
     */
    zffinal void referenceInfoSet(ZF_IN const zfchar *key, ZF_IN const ZFSerializableData *referenceInfo);
    /**
     * @brief see #referenceInfoSet
     */
    zffinal const ZFSerializableData *referenceInfo(ZF_IN const zfchar *key);
public:
    /** @brief see #referenceInfoSet */
    zffinal inline void referenceInfoForSelfSet(ZF_IN const ZFSerializableData *referenceInfo)
    {this->referenceInfoSet(zfText(""), referenceInfo);}
    /** @brief see #referenceInfoSet */
    zffinal inline const ZFSerializableData *referenceInfoForSelf(void)
    {return this->referenceInfo(zfText(""));}

    /** @brief see #referenceInfoSet */
    zffinal inline void referenceInfoForCategorySet(ZF_IN const zfchar *key, ZF_IN const ZFSerializableData *referenceInfo)
    {this->referenceInfoSet(zfstringWithFormat(zfText("c:%s"), key), referenceInfo);}
    /** @brief see #referenceInfoSet */
    zffinal inline const ZFSerializableData *referenceInfoForCategory(ZF_IN const zfchar *key)
    {return this->referenceInfo(zfstringWithFormat(zfText("c:%s"), key));}

private:
    zfchar *_ZFP_ZFSerializable_styleableType;
    zfchar *_ZFP_ZFSerializable_styleableData;
public:
    /**
     * @brief internal use only, store the styleable data of this serializable,
     *   see #ZFSerializable for more info
     */
    inline void serializableStyleableTypeSet(ZF_IN const zfchar *styleableType)
    {
        zfsChange(this->_ZFP_ZFSerializable_styleableType, styleableType);
    }
    /**
     * @brief see #serializableStyleableTypeSet
     */
    inline const zfchar *serializableStyleableTypeGet(void)
    {
        return this->_ZFP_ZFSerializable_styleableType;
    }
    /**
     * @brief see #serializableStyleableTypeSet
     */
    inline void serializableStyleableDataSet(ZF_IN const zfchar *styleableData)
    {
        zfsChange(this->_ZFP_ZFSerializable_styleableData, styleableData);
    }
    /**
     * @brief see #serializableStyleableTypeSet
     */
    inline const zfchar *serializableStyleableDataGet(void)
    {
        return this->_ZFP_ZFSerializable_styleableData;
    }
};

// ============================================================
/**
 * @brief true if object is serializable
 *
 * note that null is treated as serializable
 */
extern ZF_ENV_EXPORT zfbool ZFObjectIsSerializable(ZF_IN ZFObject *obj);

// ============================================================
/**
 * @brief convenient method to serialize from encoded data
 *
 * you should release the result object if not null
 */
extern ZF_ENV_EXPORT zfbool ZFObjectFromString(ZF_OUT zfautoObject &result,
                                               ZF_IN const zfchar *encodedData,
                                               ZF_IN_OPT zfindex encodedDataLen = zfindexMax,
                                               ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
/**
 * @brief convenient method to serialize from encoded data
 *
 * you should release the result object if not null
 * @note return null doesn't necessarily mean fail,
 *   if the input is ZFSerializableKeyword_null,
 *   which describe a null object,
 *   the result would be null
 */
extern ZF_ENV_EXPORT zfautoObject ZFObjectFromString(ZF_IN const zfchar *encodedData,
                                                     ZF_IN_OPT zfindex encodedDataLen = zfindexMax,
                                                     ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);

/**
 * @brief see #ZFObjectFromString
 */
extern ZF_ENV_EXPORT zfbool ZFObjectFromInput(ZF_OUT zfautoObject &result,
                                              ZF_IN const ZFInputCallback &input,
                                              ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
/**
 * @brief see #ZFObjectFromString
 */
extern ZF_ENV_EXPORT zfautoObject ZFObjectFromInput(ZF_IN const ZFInputCallback &input,
                                                    ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);

/**
 * @brief convenient method to serialize to encoded data
 */
extern ZF_ENV_EXPORT zfbool ZFObjectToString(ZF_OUT zfstring &encodedData,
                                             ZF_IN ZFObject *obj,
                                             ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
/**
 * @brief see #ZFObjectToString
 */
extern ZF_ENV_EXPORT zfstring ZFObjectToString(ZF_IN ZFObject *obj,
                                               ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
/**
 * @brief see #ZFObjectToString
 */
extern ZF_ENV_EXPORT zfbool ZFObjectToOutput(ZF_IN_OUT const ZFInputCallback &output,
                                             ZF_IN ZFObject *obj,
                                             ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);

// ============================================================
/**
 * @brief convenient method to serialize from serializable data
 *
 * you should release the result object if not null
 * @note return null doesn't necessarily mean fail,
 *   if the input is ZFSerializableKeyword_null,
 *   which describe a null object,
 *   the result would be null
 */
extern ZF_ENV_EXPORT zfautoObject ZFObjectFromSerializableData(ZF_IN const ZFSerializableData &serializableData,
                                                               ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                               ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
/**
 * @brief convenient method to serialize from serializable data
 *
 * you should release the result object if not null
 */
extern ZF_ENV_EXPORT zfbool ZFObjectFromSerializableData(ZF_OUT zfautoObject &result,
                                                         ZF_IN const ZFSerializableData &serializableData,
                                                         ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                         ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
/**
 * @brief convenient method to serialize to serializable data
 */
extern ZF_ENV_EXPORT zfbool ZFObjectToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                                                       ZF_IN ZFObject *obj,
                                                       ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                       ZF_IN_OPT ZFSerializable *referencedObject = zfnull);
/**
 * @brief convenient method to serialize to serializable data
 */
extern ZF_ENV_EXPORT ZFSerializableData ZFObjectToSerializableData(ZF_IN ZFObject *obj,
                                                                   ZF_OUT_OPT zfbool *outSuccess = zfnull,
                                                                   ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                                   ZF_IN_OPT ZFSerializable *referencedObject = zfnull);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDef_h_

#include "ZFSerializableUtilDef.h"

