/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZF2048AppDialog_h_
#define _ZFI_ZF2048AppDialog_h_

#include "ZFUIWidget.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZF2048AppDialogPrivate;
zfclass ZF2048AppDialog : zfextends ZFUIDialog
{
    ZFOBJECT_DECLARE(ZF2048AppDialog, ZFUIDialog)

    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(zfbool, dialogHideWhenTouchOutside, zftrue)
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(zfbool, dialogHideWhenClickBack, zftrue)

    ZFPROPERTY_ASSIGN(zfstring, dialogTitle)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfstring, dialogTitle);

    ZFPROPERTY_RETAIN(ZFUIView *, dialogContent)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUIView *, dialogContent);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZF2048AppDialogPrivate *d;
    friend zfclassFwd _ZFP_ZF2048AppDialogPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048AppDialog_h_

