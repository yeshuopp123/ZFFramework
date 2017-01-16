/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFAnimationGroup.h
 * @brief animation group
 */

#ifndef _ZFI_ZFAnimationGroup_h_
#define _ZFI_ZFAnimationGroup_h_

#include "ZFAnimation.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief used to store child animation data for #ZFAnimationGroup
 *
 * serializable data:
 * @code
 *   <ZFAnimationGroupChildData>
 *       ... // all properties
 *   </ZFAnimationGroupChildData>
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFAnimationGroupChildData : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(ZFAnimationGroupChildData, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief child animation
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, childAni)
    /**
     * @brief delay after stop in miliseconds, 0 by default
     */
    ZFPROPERTY_ASSIGN(zftimet, childDelayBeforeStart)

    /**
     * @brief whether copy owner animation group's target to self, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, childAutoCopyTarget, ZFPropertyInitValue(zftrue))
};

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFAnimationGroup_child zfText("child")

// ============================================================
zfclassFwd _ZFP_ZFAnimationGroupPrivate;
/**
 * @brief animation group
 *
 * serializable data:
 * @code
 *   <ZFAnimationGroup>
 *       <ZFAnimationGroupChildData category="child" ... />
 *       ... // all child animations
 *   </ZFAnimationGroup>
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFAnimationGroup : zfextends ZFAnimation
{
    ZFOBJECT_DECLARE(ZFAnimationGroup, ZFAnimation)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the child animation
     */
    ZFOBSERVER_EVENT(AniGroupOnChildStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the child animation
     */
    ZFOBSERVER_EVENT(AniGroupOnChildStop)

    // ============================================================
    // serialize
protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);

    // ============================================================
    // object
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

    // ============================================================
    // child animation control
public:
    /**
     * @brief add child animation
     */
    virtual void childAniAdd(ZF_IN ZFAnimation *ani);
    /**
     * @brief add child animation
     */
    virtual void childAniAdd(ZF_IN ZFAnimation *ani,
                             ZF_IN zfbool childAutoCopyTarget);
    /**
     * @brief add child animation
     */
    virtual void childAniAdd(ZF_IN ZFAnimationGroupChildData *childData);
    /**
     * @brief child animation count
     */
    virtual zfindex childAniCount(void);
    /**
     * @brief get child animation
     */
    virtual ZFAnimation *childAniGet(ZF_IN zfindex index);
    /**
     * @brief get child animation data
     */
    virtual ZFAnimationGroupChildData *childAniDataGet(ZF_IN zfindex index);
    /**
     * @brief remove child animation
     */
    virtual void childAniRemove(ZF_IN zfindex index);
    /**
     * @brief remove all child animation
     */
    virtual void childAniRemoveAll(void);

    // ============================================================
    // property
public:
    /**
     * @brief whether to start child animations parallel(true) or serial(false), false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, aniParallel)

    // ============================================================
    // start stop
protected:
    zfoverride
    virtual zfbool aniImplCheckValid(void);
    zfoverride
    virtual void aniImplStart(void);
    zfoverride
    virtual void aniImplStop(void);

protected:
    /** @brief see #EventAniGroupOnChildStart */
    virtual inline void aniGroupOnChildStart(ZF_IN ZFAnimation *childAni)
    {
        this->observerNotify(ZFAnimationGroup::EventAniGroupOnChildStart(), childAni);
    }
    /** @brief see #EventAniGroupOnChildStop */
    virtual inline void aniGroupOnChildStop(ZF_IN ZFAnimation *childAni)
    {
        this->observerNotify(ZFAnimationGroup::EventAniGroupOnChildStop(), childAni);
    }

private:
    _ZFP_ZFAnimationGroupPrivate *d;
    friend zfclassFwd _ZFP_ZFAnimationGroupPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAnimationGroup_h_

