/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUINativeViewWrapper.h
 * @brief native view wrapper
 */

#ifndef _ZFI_ZFUINativeViewWrapper_h_
#define _ZFI_ZFUINativeViewWrapper_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUINativeViewWrapper
/**
 * @brief view container to hold a native view
 *
 * used to hold a native view in ZFUIKit environment,
 * what you should do is create a #ZFUINativeViewWrapper
 * and store native view by #wrappedNativeViewSet\n
 * the embeded native view would be layouted to fill #ZFUINativeViewWrapper's frame,
 * to change it's layout logic, you should use it as a #ZFUIView\n
 * the #ZFUINativeViewWrapper itself is serializable,
 * however the native view itself can not be serialized,
 * it should be initialized by app manually
 */
zffinal zfclass ZF_ENV_EXPORT ZFUINativeViewWrapper : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUINativeViewWrapper, ZFUIView)

public:
    /**
     * @brief init with native view, see #ZFUINativeViewWrapper
     */
    virtual ZFObject *objectOnInit(ZF_IN void *wrappedNativeView);
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief see #ZFUINativeViewWrapper
     */
    virtual void wrappedNativeViewSet(ZF_IN void *wrappedNativeView);
    /**
     * @brief see #ZFUINativeViewWrapper
     */
    virtual void *wrappedNativeView(void);

protected:
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUINativeViewWrapper_h_

