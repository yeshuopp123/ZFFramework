/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIImage_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImage_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIImageView, view);
        container->childAdd(view);
        view->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);

        // encode image to binary and load it again to test
        zfautoObject imageHolder = ZFUIImageRes(zfText("test_normal.png"));
        ZFIOBridgeCallbackUsingBuffer io;
        ZFUIImageEncodeToBinary(io, imageHolder.toAny());
        zfautoObject imageNew = ZFUIImageEncodeFromBinary(io);
        view->imageContentSet(imageNew.toAny());

        this->prepareSettingButton(window, view);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIImageView *view)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIImage_test)

ZF_NAMESPACE_GLOBAL_END

