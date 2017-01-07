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

zfclass ZFUIKit_ZFUIViewPositionOnScreen_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIViewPositionOnScreen_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFFramework_test_protocolCheck(ZFUIViewPositionOnScreen)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIKit_test_Button, button);
        container->childAdd(button);
        button->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
        button->buttonLabelTextStringSet(zfText("click me"));

        ZFLISTENER_LOCAL(buttonOnClick, {
            zfLogTrimT() << zfText("window size:") << ZFUIViewUtil::viewRoot(listenerData.sender->to<ZFUIView *>())->layoutedFrame().size;
            zfLogTrimT() << zfText("clicked view's position:") << ZFUIViewPositionOnScreen(listenerData.sender->toAny());
        })
        button->observerAdd(ZFUIButton::EventButtonOnClick(), buttonOnClick);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIViewPositionOnScreen_test)

ZF_NAMESPACE_GLOBAL_END

