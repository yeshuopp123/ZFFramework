/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFContainer.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFContainer_h_
#define _ZFI_ZFContainer_h_

#include "ZFObject.h"
#include "ZFIterable.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFContainer_element zfText("element")

/**
 * @brief container of ZFObject
 *
 * automatically retain the object when add,
 * and release it after remove\n
 * \n
 * a container can not contain null,
 * you may want to use #zfnullObject if necessary\n
 * \n
 * a container is serializable only if
 * all of it's content are serializable,
 * for performance, you should not check the serializable state too frequently\n
 * clear contents before serialize from data\n
 * whether contents of the container is sorted,
 * is depends on subclass\n
 * \n
 * serializable data:
 * @code
 *   <ContainerClass> // container class
 *       <ElementClass0 category="element" /> // each elements
 *       <ElementClass1 category="element" />
 *       ...
 *   </ContainerClass>
 * @endcode
 *
 * @note a container would have special serializing step with ref or style logic,
 *   see #serializableOnSerializeToData for more info
 */
zfabstract ZF_ENV_EXPORT ZFContainer : zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable, zfimplements ZFIterable
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFContainer, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable, ZFCopyable, ZFIterable)

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
     * param0 is the element added to this container
     */
    ZFOBSERVER_EVENT(ContentOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when element removed\n
     * param0 is the element removed from this container
     */
    ZFOBSERVER_EVENT(ContentOnRemove)

protected:
    zfoverride
    virtual zfbool serializableOnCheck(void);
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
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
                                                 ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
protected:
    /**
     * @brief see #serializableOnSerializeToData
     */
    virtual zfbool serializableOnSerializeToDataWithRef(ZF_IN_OUT ZFSerializableData &serializableData,
                                                        ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                        ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull) = 0;

public:
    /** @brief see #ZFObject::objectOnDeallocPrepare, remove all contents before dealloc */
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
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        this->objectInfoOfContentT(ret, 10);
    }

public:
    /**
     * @brief return a short string describe the content
     */
    virtual void objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                      ZF_IN_OPT zfindex maxCount = zfindexMax,
                                      ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault);
    /** @brief see #objectInfoOfContentT */
    virtual inline zfstring objectInfoOfContent(ZF_IN_OPT zfindex maxCount = zfindexMax,
                                                ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault)
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
        this->observerNotify(ZFContainer::EventContentOnChange());
    }
    /** @brief see #EventContentOnAdd */
    virtual inline void contentOnAdd(ZF_IN ZFObject *element)
    {
        if(this->objectCached())
        {
            element->objectCachedSet(zftrue);
        }
        this->observerNotify(ZFContainer::EventContentOnAdd(), element);
    }
    /** @brief see #EventContentOnRemove */
    virtual inline void contentOnRemove(ZF_IN ZFObject *element)
    {
        this->observerNotify(ZFContainer::EventContentOnRemove(), element);
        if(this->objectCached())
        {
            element->objectCachedSet(zffalse);
        }
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFContainer_h_

