/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZF2048AppSettingDialog_h_
#define _ZFI_ZF2048AppSettingDialog_h_

#include "ZF2048AppDialog.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZF2048AppSettingDialogPrivate;
zfclass ZF2048AppSettingDialog : zfextends ZF2048AppDialog
{
    ZFOBJECT_DECLARE(ZF2048AppSettingDialog, ZF2048AppDialog)

    ZFOBSERVER_EVENT(SettingOnChange)

public:
    zfindex dataWidth;
    zfindex dataHeight;

public:
    zfoverride
    virtual ZFObject *objectOnInit(ZF_IN zfindex dataWidth,
                                   ZF_IN zfindex dataHeight);
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    virtual void settingOnChange(void)
    {
        this->observerNotify(zfself::EventSettingOnChange());
    }

private:
    _ZFP_ZF2048AppSettingDialogPrivate *d;
    friend zfclassFwd _ZFP_ZF2048AppSettingDialogPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048AppSettingDialog_h_

