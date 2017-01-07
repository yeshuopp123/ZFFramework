/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZF2048App_h_
#define _ZFI_ZF2048App_h_

#include "ZF2048Game.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZF2048AppPrivate;
zfclass ZF2048App : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZF2048App, ZFUIView)

    ZFPROPERTY_RETAIN_READONLY(ZF2048Game *, game, ZFPropertyInitValue(zflineAlloc(ZF2048Game)))

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void viewOnAddToParent(ZF_IN ZFUIView *parent)
    {
        zfsuper::viewOnAddToParent(parent);
        this->game()->gameFocus();
    }

private:
    _ZFP_ZF2048AppPrivate *d;
    friend zfclassFwd _ZFP_ZF2048AppPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048App_h_

