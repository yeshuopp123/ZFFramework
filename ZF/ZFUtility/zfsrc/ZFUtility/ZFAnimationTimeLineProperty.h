/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFAnimationTimeLineProperty.h
 * @brief animated update object's property base on time line
 */

#ifndef _ZFI_ZFAnimationTimeLineProperty_h_
#define _ZFI_ZFAnimationTimeLineProperty_h_

#include "ZFAnimationTimeLine.h"
#include "ZFTimeLineProperty.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFAnimationTimeLineProperty_step zfText("step")

// ============================================================
zfclassFwd _ZFP_ZFAnimationTimeLinePropertyPrivate;
/**
 * @brief animated update object's property base on time line
 *
 * serializable data:
 * @code
 *   <ZFAnimationTimeLineProperty>
 *       // all property change steps
 *       <ZFTimeLineProperty_XXX category="step" ... />
 *   </ZFAnimationTimeLineProperty>
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFAnimationTimeLineProperty : zfextends ZFAnimationTimeLine
{
    ZFOBJECT_DECLARE(ZFAnimationTimeLineProperty, ZFAnimationTimeLine)

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
    // property change step
public:
    /**
     * @brief add a property change step,
     *   #ZFAnimation::aniTarget's property would be change accorrding this step
     */
    virtual void stepAdd(ZF_IN ZFTimeLineProperty *step);
    /** @brief see #stepAdd */
    virtual void stepRemove(ZF_IN ZFTimeLineProperty *step);
    /** @brief see #stepAdd */
    virtual void stepRemoveAtIndex(ZF_IN zfindex index);
    /** @brief see #stepAdd */
    virtual void stepRemoveAll(void);
    /** @brief see #stepAdd */
    virtual zfindex stepCount(void);
    /** @brief see #stepAdd */
    virtual ZFTimeLineProperty *stepAtIndex(ZF_IN zfindex index);

    // ============================================================
    // start stop
protected:
    zfoverride
    virtual zfbool aniImplCheckValid(void);

protected:
    zfoverride
    virtual void aniOnStart(void);
    zfoverride
    virtual void aniOnStop(void);
    zfoverride
    virtual void aniTimeLineOnUpdate(ZF_IN zffloat progress);

private:
    _ZFP_ZFAnimationTimeLinePropertyPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAnimationTimeLineProperty_h_

