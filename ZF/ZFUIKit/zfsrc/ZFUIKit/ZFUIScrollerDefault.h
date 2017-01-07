/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIScrollerDefault.h
 * @brief default scroller of #ZFUIScroller
 */

#ifndef _ZFI_ZFUIScrollerDefault_h_
#define _ZFI_ZFUIScrollerDefault_h_

#include "ZFUIScroller.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFUIScrollerDefaultPrivate;
/**
 * @brief default scroller of #ZFUIScroller
 */
zfclass ZF_ENV_EXPORT ZFUIScrollerDefault : zfextends ZFObject, zfimplements ZFUIScroller
{
    ZFOBJECT_DECLARE(ZFUIScrollerDefault, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIScroller)

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    zfoverride
    virtual void scrollOwnerSizeChanged(ZF_IN const zfint &ownerSize);
    zfoverride
    virtual void scrollBounceChanged(ZF_IN zfbool scrollBounce,
                                     ZF_IN zfbool scrollBounceAlways);
    zfoverride
    virtual void scrollAlignToPageChanged(ZF_IN zfbool scrollAlignToPage);
    zfoverride
    virtual void scrollContentChanged(ZF_IN zfint contentOffset, ZF_IN zfint contentSize);

public:
    zfoverride
    virtual void scrollToFitRange(void);
    zfoverride
    virtual void scrollWithoutAnimation(ZF_IN zfint contentOffset);
    zfoverride
    virtual void scrollByPoint(ZF_IN zfint point);
    zfoverride
    virtual zfint scrollByPointEndPoint(void);
    zfoverride
    virtual void scrollBySpeed(ZF_IN zfint speed);
    zfoverride
    virtual zfint scrollBySpeedCurrentSpeed(void);
    zfoverride
    virtual zfint scrollBySpeedEndPointPredicted(void);

    zfoverride
    virtual zfint scrollEndPointPredicted(void);

    zfoverride
    virtual zfbool scrollRequireFocus(void);

public:
    zfoverride
    virtual zfint scrollContentOffset(void);
    zfoverride
    virtual zfint scrollContentSize(void);

public:
    zfoverride
    virtual void scrollOnDragBegin(ZF_IN zfint mousePos,
                                   ZF_IN const zftimet &mouseTime);
    zfoverride
    virtual void scrollOnDrag(ZF_IN zfint mousePos,
                              ZF_IN const zftimet &mouseTime);
    zfoverride
    virtual void scrollOnDragEnd(ZF_IN const zftimet &mouseTime,
                                 ZF_IN zfbool needScrollAni);

public:
    zfoverride
    virtual void scrollAniOnUpdate(ZF_IN const zftimet &time);

private:
    _ZFP_ZFUIScrollerDefaultPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIScrollerDefault_h_

