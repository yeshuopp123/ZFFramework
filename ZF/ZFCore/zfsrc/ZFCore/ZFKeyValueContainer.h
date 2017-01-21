/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFKeyValueContainer.h
 * @brief key value container of ZFObject
 */

#ifndef _ZFI_ZFKeyValueContainer_h_
#define _ZFI_ZFKeyValueContainer_h_

#include "ZFObject.h"
#include "ZFIterable.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFKeyValueContainer_key zfText("key")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFKeyValueContainer_value zfText("value")

zfclassFwd _ZFP_ZFKeyValueContainerPrivate;
/**
 * @brief key value container of ZFObject
 *
 * automatically retain Key and Value when add,
 * and release it after remove
 * @note all Keys must be comparable and sortable
 *   (#ZFObject::objectCompare must not return ZFCompareUncomparable)
 * @see ZFObject::objectCompare
 *
 * to go through all the contents, you should:
 * @code
 *   ZFCoreArrayPOD<ZFKeyValuePair> pairs = container->allPair();
 *   for(zfindex i = 0; i < pairs.count(); ++i)
 *   {
 *       if(condition)
 *       {
 *           // safe to remove since keys and values is a copy of the contents
 *           // however, you must not access the removed elements after remove
 *           container->remove(keys[i]);
 *       }
 *   }
 * @endcode
 * or you may want to use iterator, see #zfiterator for more info
 * \n
 * a key value container is serializable only if
 * all of it's key and value are serializable\n
 * for performance, you should not check the serializable state too frequently\n
 * clear contents before serialize from data\n
 * \n
 * serializable data:
 * @code
 *   <ContainerClass>
 *       // each paired elements, must be paired
 *       <ElementKeyClass0 category="key" />
 *       <ElementValueClass0 category="value" />
 *
 *       <ElementKeyClass1 category="key" />
 *       <ElementValueClass1 category="value" />
 *
 *       ...
 *   </ContainerClass>
 * @endcode
 */
zfabstract ZF_ENV_EXPORT ZFKeyValueContainer: zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable, zfimplements ZFIterable, zfimplements ZFIterableKeyValue
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFKeyValueContainer, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable, ZFCopyable, ZFIterable, ZFIterableKeyValue)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when element added or removed or order changed
     */
    ZFOBSERVER_EVENT(ContentOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when element added\n
     * param0 is the element key,
     * param1 is the element value
     */
    ZFOBSERVER_EVENT(ContentOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when element removed\n
     * param0 is the element key,
     * param1 is the element value
     */
    ZFOBSERVER_EVENT(ContentOnRemove)

protected:
    zfoverride
    virtual zfbool serializableOnCheck(void);
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    /**
     * @brief see #ZFSerializable::serializableOnSerializeToData
     *
     * while serializing a container to data with ref or style logic,
     * it's hard to auto serialize all contents,
     * to workaround this problem,
     * we would serialize all contents normally if no ref logic found,
     * and would call #serializableOnSerializeToDataWithRef otherwise,
     * you should override #serializableOnSerializeToDataWithRef
     * if you have different ref logic
     */
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);
protected:
    /**
     * @brief see #ZFKeyValueContainer::serializableOnSerializeToData
     */
    virtual zfbool serializableOnSerializeToDataWithRef(ZF_IN_OUT ZFSerializableData &serializableData,
                                                        ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                        ZF_OUT_OPT zfstring *outErrorHint = zfnull) zfpurevirtual;

public:
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    zfoverride
    virtual inline void objectInfoOnAppendTokenLeft(ZF_IN_OUT zfstring &ret)
    {
    }
    zfoverride
    virtual inline void objectInfoOnAppendTokenRight(ZF_IN_OUT zfstring &ret)
    {
    }
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        this->objectInfoOfContentT(ret, 5);
    }

public:
    /**
     * @brief return a short string describe the content
     */
    virtual void objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                      ZF_IN_OPT zfindex maxCount = zfindexMax,
                                      ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault);
    /** @brief see #objectInfoOfContentT */
    virtual inline zfstring objectInfoOfContent(ZF_IN_OPT zfindex maxCount = zfindexMax,
                                                ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault)
    {
        zfstring ret;
        this->objectInfoOfContentT(ret, maxCount, token);
        return ret;
    }

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

protected:
    zfoverride
    virtual void objectCachedOnChange(void);

protected:
    /** @brief see #EventContentOnChange */
    virtual inline void contentOnChange(void)
    {
        this->observerNotify(ZFKeyValueContainer::EventContentOnChange());
    }
    /** @brief see #EventContentOnAdd */
    virtual inline void contentOnAdd(ZF_IN ZFObject *key, ZF_IN ZFObject *value)
    {
        if(this->objectCached())
        {
            key->objectCachedSet(zftrue);
            if(value)
            {
                value->objectCachedSet(zftrue);
            }
        }
        this->observerNotify(ZFKeyValueContainer::EventContentOnAdd(), key, value);
    }
    /** @brief see #EventContentOnRemove */
    virtual inline void contentOnRemove(ZF_IN ZFObject *key, ZF_IN ZFObject *value)
    {
        this->observerNotify(ZFKeyValueContainer::EventContentOnRemove(), key, value);
        if(this->objectCached())
        {
            key->objectCachedSet(zftrue);
            if(value)
            {
                value->objectCachedSet(zftrue);
            }
        }
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFKeyValueContainer_h_

