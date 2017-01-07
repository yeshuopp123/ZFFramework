/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIButtonGroup_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIButtonGroup_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIButtonGroupBasic, layout);
        container->childAdd(layout);
        layout->layoutParam()->layoutMarginSet(ZFUIMarginMake(40));
        layout->buttonGroupTypeSet(ZFUIButtonGroupType::e_Tab);
        layout->layoutOrientationSet(ZFUIOrientation::e_Top);

        this->prepareChildren(layout);

        this->prepareSettingButton(window, layout);

        ZFLISTENER_LOCAL(buttonGroupAction, {
            zfLogTrimT()
                << listenerData.sender
                << ZFObserverEventGetName(listenerData.eventId)
                << zfText(", checked:") << listenerData.param1;
        })
        layout->observerAdd(ZFUIButtonGroup::EventButtonTabOnClickChecked(), buttonGroupAction);
        layout->observerAdd(ZFUIButtonGroup::EventButtonTabOnChange(), buttonGroupAction);
    }

private:
    void prepareChildren(ZF_IN ZFUIButtonGroup *layout)
    {
        for(zfindex i = 0; i < 4; ++i)
        {
            zfblockedAlloc(ZFUIButtonRatio, child);
            layout->buttonAdd(child);
            child->buttonLabelTextStringSet(zfstringWithFormat(zfText("btn %zi"), i));
        }
    }
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIButtonGroup *layout)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingForBoolProperty(settings, layout->toObject(), ZFPropertyAccess(ZFUIButtonGroup, buttonTabAllowUnchecked));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIButtonGroup_test)

ZF_NAMESPACE_GLOBAL_END

