/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZF2048AppAutoMoveSettingDialog_h_
#define _ZFI_ZF2048AppAutoMoveSettingDialog_h_

#include "ZF2048AppDialog.h"
#include "ZF2048AppAutoMoveAction.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZF2048AppAutoMoveSettingDialogPrivate;
zfclass ZF2048AppAutoMoveSettingDialog : zfextends ZF2048AppDialog
{
    ZFOBJECT_DECLARE(ZF2048AppAutoMoveSettingDialog, ZF2048AppDialog)

    ZFOBSERVER_EVENT(AutoMoveSettingOnChange)

public:
    ZFCoreArrayPOD<ZF2048AppAutoMoveActionEnum> autoMoves;

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    virtual void autoMoveSettingOnChange(void)
    {
        this->observerNotify(zfself::EventAutoMoveSettingOnChange());
    }

private:
    _ZFP_ZF2048AppAutoMoveSettingDialogPrivate *d;
    friend zfclassFwd _ZFP_ZF2048AppAutoMoveSettingDialogPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048AppAutoMoveSettingDialog_h_

