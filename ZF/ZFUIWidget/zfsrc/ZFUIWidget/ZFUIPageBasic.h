/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIPageBasic.h
 * @brief basic #ZFUIPage with animation logic
 */

#ifndef _ZFI_ZFUIPageBasic_h_
#define _ZFI_ZFUIPageBasic_h_

#include "ZFUIPage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFUIPageManagerBasic;
zfclassFwd _ZFP_ZFUIPageManagerBasicPrivate;
zfclassFwd _ZFP_ZFUIPageBasicPrivate;
/**
 * @brief basic page with animation logic,
 *   see #ZFUIPageManager and #ZFUIPageManagerBasic for how to use
 */
zfabstract ZF_ENV_EXPORT ZFUIPageBasic : zfextends ZFObject, zfimplements ZFUIPage
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFUIPageBasic, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIPage)

    // ============================================================
    // observers
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page about to start animation,
     * ensured called even if no animation to start for convenient\n
     * param0 is the animation to start or null if no animation to start,
     * param1 is the #ZFUIPagePauseReason or #ZFUIPageResumeReason
     */
    ZFOBSERVER_EVENT(PageAniOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page about to stop animation,
     * ensured called even if no animation to start for convenient\n
     * param0 is the animation to stop or null if no animation to stop,
     * param1 is the #ZFUIPagePauseReason or #ZFUIPageResumeReason
     */
    ZFOBSERVER_EVENT(PageAniOnStop)

    // ============================================================
    // pageAni
public:
    /**
     * @brief page's animation
     *
     * we have these animations for page to setup:
     * -  pageAniResumeByRequest / pagePauseAniToBackground:
     *   used when a page is resumed by user request,
     *   and the previous resume page would be sent to background
     * -  pageAniResumeFromBackground / pagePauseAniBeforeDestroy:
     *   used when a foreground page is destroyed by user request,
     *   and background page would result to be moved to foreground
     *
     * see #pageAniOnUpdateForResume for more info
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, pageAniResumeByRequest)
    /**
     * @brief page's animation, see #pageAniResumeByRequest
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, pageAniResumeFromBackground)
    /**
     * @brief page's animation, see #pageAniResumeByRequest
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, pageAniPauseToBackground)
    /**
     * @brief page's animation, see #pageAniResumeByRequest
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, pageAniPauseBeforeDestroy)

private:
    zfbool _ZFP_ZFUIPage_pageAniCanChange;
public:
    /**
     * @brief the final animation being used when page stack changed, null to disable animation
     *
     * by default, this value would be updated by #pageAniOnUpdateForSiblingPageResume and #pageAniOnUpdateForResume,
     * you may override default value whenever you can,
     * to override animation\n
     * the recommended way is to override #pageAniOnUpdateForSiblingPageResume and/or #pageAniOnUpdateForResume
     * to change this value\n
     * this value would be reset to null when animation stopped or page destroyed\n
     * see #pageAniOnUpdateForResume for more info
     */
    virtual void pageAniSet(ZF_IN ZFAnimation *pageAni);
    /**
     * @brief see #pageAniSet
     */
    virtual ZFAnimation *pageAni(void);

public:
    /**
     * @brief whether this page need higher priority for animation,
     *   typically a higher priority animation would have its view on the top
     */
    zfbool pageAniPriorityNeedHigher;
protected:
    /**
     * @brief used to update animation by priority, do nothing by default
     */
    virtual void pageAniPriorityOnUpdate(ZF_IN zfbool priorityHigher)
    {
    }
protected:
    /**
     * @brief see #pageAniOnUpdateForResume
     */
    virtual void pageAniOnUpdateForSiblingPageResume(ZF_IN ZFUIPageResumeReasonEnum reason,
                                                     ZF_IN ZFUIPageBasic *siblingResumePageOrNull);
    /**
     * @brief for page to update it's final animation
     *
     * the actual animation should be saved to #pageAniSet,
     * by this page or by sibling page\n
     * \n
     * the actual animation logic is achieved by this:
     * @code
     *   ZFUIPageBasic *pausePage = xxx; // page to pause
     *   ZFUIPageBasic *resumePage = xxx; // page to resume
     *   pausePage->pageAniOnUpdateForSiblingPageResume(reason, resumePage);
     *   resumePage->pageAniOnUpdateForResume(reason, pausePage, pauseAniHasHigherPriority);
     *   zfbool pausePageHasHigherPriority = (!resumePage->pageAniPriorityNeedHigher && pausePage->pageAniPriorityNeedHigher);
     *   pausePage->pageAniPriorityOnUpdate(pausePageHasHigherPriority);
     *   resumePage->pageAniPriorityOnUpdate(!pausePageHasHigherPriority);
     * @endcode
     * by default, #pageAniOnUpdateForSiblingPageResume and #pageAniOnUpdateForResume
     * would update page animation
     * by #pageAniResumeByRequest/#pageAniResumeFromBackground/#pageAniPauseToBackground/#pageAniPauseBeforeDestroy
     * accorrding to page's resume and pause reason,
     * and page requested by user would have higher animation priority (request resume or request destroy)
     */
    virtual void pageAniOnUpdateForResume(ZF_IN ZFUIPageResumeReasonEnum reason,
                                          ZF_IN ZFUIPageBasic *siblingPausePageOrNull);
    /**
     * @brief called to update animation's target, do nothing by default
     */
    virtual void pageAniOnUpdateAniTarget(ZF_IN ZFAnimation *pageAni)
    {
    }

    // ============================================================
    // event
protected:
    /** @brief see #EventPageAniOnStart */
    virtual void pageAniOnStart(ZF_IN ZFAnimation *pageAni,
                                ZF_IN ZFEnum *pagePauseReasonOrResumeReason)
    {
        this->observerNotify(ZFUIPageBasic::EventPageAniOnStart(),
                             pageAni, pagePauseReasonOrResumeReason);
    }
    /** @brief see #EventPageAniOnStop */
    virtual void pageAniOnStop(ZF_IN ZFAnimation *pageAni,
                               ZF_IN ZFEnum *pagePauseReasonOrResumeReason)
    {
        this->observerNotify(ZFUIPageBasic::EventPageAniOnStop(),
                             pageAni, pagePauseReasonOrResumeReason);
    }

    // ============================================================
    // override
protected:
    zfoverride
    virtual void pageOnCreate(void);
    zfoverride
    virtual void pageOnResume(ZF_IN ZFUIPageResumeReasonEnum reason);
    zfoverride
    virtual void pageOnPause(ZF_IN ZFUIPagePauseReasonEnum reason);
    zfoverride
    virtual void pageOnDestroy(void);

protected:
    zfoverride
    virtual void pageDelayDestroyOnCheck(void);

private:
    _ZFP_ZFUIPageBasicPrivate *d;
    friend zfclassFwd ZFUIPageManagerBasic;
    friend zfclassFwd _ZFP_ZFUIPageManagerBasicPrivate;
    friend zfclassFwd _ZFP_ZFUIPageBasicPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPageBasic_h_

