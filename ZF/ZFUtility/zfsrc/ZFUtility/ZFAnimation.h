/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFAnimation.h
 * @brief base class of all animation
 */

#ifndef _ZFI_ZFAnimation_h_
#define _ZFI_ZFAnimation_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFAnimationPrivate;
/**
 * @brief base class of all animation
 */
zfabstract ZF_ENV_EXPORT ZFAnimation : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFAnimation, ZFStyleableObject)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when start an invalid animation
     */
    ZFOBSERVER_EVENT(AniOnInvalid)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation delay finished
     */
    ZFOBSERVER_EVENT(AniOnDelayFinish)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation started
     * @note for delayed animation,
     *   this event would be fired before delay,
     *   use #EventAniOnDelayFinish if necessary
     */
    ZFOBSERVER_EVENT(AniOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation stopped
     */
    ZFOBSERVER_EVENT(AniOnStop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation stopped or invalid,
     * designed for convenient,
     * param0 is a #ZFValue containing a bool value to show whether the animation is valid
     */
    ZFOBSERVER_EVENT(AniOnStopOrOnInvalid)

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
    // property
public:
    /**
     * @brief animation's delay in miliseconds, 0 by default
     */
    ZFPROPERTY_ASSIGN(zftimet, aniDelay)
    /**
     * @brief animation's duration in miliseconds, 250 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniDuration,
                                ZFPropertyInitValue(250))

public:
    /**
     * @brief animation's target, assign only,
     *   but would be retain automatically while animation is running
     *
     * this is not necessary to be set during animation's running
     * (although most animation subclass need a target)
     */
    virtual void aniTargetSet(ZF_IN ZFObject *aniTarget);
    /**
     * @brief animation's target
     */
    virtual ZFObject *aniTarget(void);

    // ============================================================
    // start stop
public:
    /**
     * @brief start the animation
     * @note an animation would be retained automatically when it's running,
     *   and released after stopped
     */
    zffinal void aniStart(void);
    /**
     * @brief true if the animation is running (or delaying)
     */
    zffinal zfbool aniRunning(void);
    /**
     * @brief true if the animation is delaying
     */
    zffinal zfbool aniDelaying(void);
    /**
     * @brief stop the animation or do nothing if not started
     */
    zffinal void aniStop(void);
    /**
     * @brief true if the animation is stopped by calling #aniStop
     *
     * this state would be kept until next #aniStart called
     */
    zffinal zfbool aniStoppedByUser(void);

public:
    /**
     * @brief get current animation's id
     *
     * animation id would be updated automatically
     * each time animation is started and stopped
     */
    virtual zfidentity aniId(void);

public:
    /**
     * @brief check whether animation is valid, see #aniImplCheckValid
     */
    zffinal zfbool aniValid(void);

protected:
    /**
     * @brief called to check whether the animation is currently valid,
     *   an invalid animation is ensured can't be started
     *
     * by default, this method would only check animation's duration,
     * for common case, you should check whether the target is valid
     */
    virtual zfbool aniImplCheckValid(void);
public:
    zffinal void _ZFP_ZFAnimation_aniImplDelayNotifyFinish(ZF_IN zfidentity taskId);
protected:
    /**
     * @brief for subclass to achieve delay logic
     *
     * by default, this method would implement delay by #ZFThreadExecuteInMainThreadAfterDelay,
     * you may override and supply your own implementation without call super,
     * if you do, you must override or call all of these methods:
     * -  aniImplDelay: to achieve delay
     * -  aniImplDelayCancel: to achieve delay cancel
     * -  aniImplDelayNotifyFinish: to notify delay finished
     */
    virtual void aniImplDelay(void);
    /**
     * @brief see #aniImplDelay
     */
    virtual void aniImplDelayCancel(void);
    /**
     * @brief see #aniImplDelay
     */
    zffinal void aniImplDelayNotifyFinish(void);
    /**
     * @brief for subclass to achieve actual animation
     */
    virtual void aniImplStart(void) = 0;
    /**
     * @brief for subclass to stop actual animation
     */
    virtual void aniImplStop(void) = 0;
    /** @brief see #EventAniOnInvalid */
    virtual inline void aniOnInvalid(void)
    {
        this->observerNotify(ZFAnimation::EventAniOnInvalid());
    }
    /** @brief see #EventAniOnDelayFinish */
    virtual inline void aniOnDelayFinish(void)
    {
        this->observerNotify(ZFAnimation::EventAniOnDelayFinish());
    }
    /** @brief see #EventAniOnStart */
    virtual inline void aniOnStart(void)
    {
        this->observerNotify(ZFAnimation::EventAniOnStart());
    }
    /** @brief see #EventAniOnStop */
    virtual inline void aniOnStop(void)
    {
        this->observerNotify(ZFAnimation::EventAniOnStop());
    }
    /** @brief see #EventAniOnStopOrOnInvalid */
    virtual inline void aniOnStopOrOnInvalid(ZF_IN zfbool aniValid)
    {
        this->observerNotify(ZFAnimation::EventAniOnStopOrOnInvalid(), ZFValue::boolValueCreate(aniValid).toObject());
    }
    /**
     * @brief subclass must notify after the animation stop
     */
    zffinal void aniImplNotifyStop(void);

private:
    _ZFP_ZFAnimationPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAnimation_h_

