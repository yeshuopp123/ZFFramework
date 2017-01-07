/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIButtonGroup.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIButtonGroupType)

ZFOBJECT_REGISTER(ZFUIButtonGroup)

ZFOBSERVER_EVENT_REGISTER(ZFUIButtonGroup, ButtonOnAdd)
ZFOBSERVER_EVENT_REGISTER(ZFUIButtonGroup, ButtonOnRemove)
ZFOBSERVER_EVENT_REGISTER(ZFUIButtonGroup, ButtonTabOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIButtonGroup, ButtonTabOnClickChecked)

// ============================================================
// common
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIButtonGroupListenerHolder, ZFLevelZFFrameworkNormal)
{
    this->buttonEventListener = ZFCallbackForRawFunction(zfself::buttonEvent);
}
public:
    ZFListener buttonEventListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(buttonEvent)
    {
        ZFUIButtonGroup *buttonGroup = userData->to<ZFObjectHolder *>()->holdedObj;
        ZFUIButton *button = listenerData.sender->to<ZFUIButton *>();
        zfindex buttonIndex = buttonGroup->buttonFind(button);
        zfCoreAssert(buttonIndex != zfindexMax);
        buttonGroup->_ZFP_ZFUIButtonGroup_buttonGroupOnEvent(button, buttonIndex, listenerData.eventId);
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIButtonGroupListenerHolder)
static void _ZFP_ZFUIButtonGroup_setup_common(ZF_IN ZFUIButtonGroup *buttonGroup,
                                              ZF_IN ZFUIButton *button,
                                              ZF_IN zfindex buttonIndex)
{
    const ZFListener &buttonEventListener = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIButtonGroupListenerHolder)->buttonEventListener;
    ZFObject *buttonGroupHolder = buttonGroup->toObject()->objectHolder();
    button->observerAdd(ZFUIButton::EventButtonOnClick(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonStateOnChange(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnDown(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnMoveEnter(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnMoveExit(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnMoveInside(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnMoveOutside(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnUpInside(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnUp(), buttonEventListener, buttonGroupHolder);
}
static void _ZFP_ZFUIButtonGroup_cleanup_common(ZF_IN ZFUIButtonGroup *buttonGroup,
                                                ZF_IN ZFUIButton *button)
{
    const ZFListener &buttonEventListener = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIButtonGroupListenerHolder)->buttonEventListener;
    button->observerRemove(ZFUIButton::EventButtonOnClick(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonStateOnChange(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnDown(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnMoveEnter(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnMoveExit(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnMoveInside(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnMoveOutside(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnUpInside(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnUp(), buttonEventListener);
}

// ============================================================
// ZFUIButtonGroupType::e_Normal
static void _ZFP_ZFUIButtonGroup_setup_Normal(ZF_IN ZFUIButtonGroup *buttonGroup,
                                              ZF_IN ZFUIButton *button,
                                              ZF_IN zfindex buttonIndex)
{
    _ZFP_ZFUIButtonGroup_setup_common(buttonGroup, button, buttonIndex);
}
static void _ZFP_ZFUIButtonGroup_cleanup_Normal(ZF_IN ZFUIButtonGroup *buttonGroup,
                                                ZF_IN ZFUIButton *button)
{
    _ZFP_ZFUIButtonGroup_cleanup_common(buttonGroup, button);
}

// ============================================================
// ZFUIButtonGroupType::e_Tab
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIButtonGroupListenerHolder_Tab, ZFLevelZFFrameworkNormal)
{
    this->buttonOnClickListener = ZFCallbackForRawFunction(zfself::buttonOnClick);
}
public:
    ZFListener buttonOnClickListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(buttonOnClick)
    {
        ZFUIButtonGroup *buttonGroup = userData->to<ZFObjectHolder *>()->holdedObj;
        ZFUIButton *button = listenerData.sender->to<ZFUIButton *>();
        zfindex buttonIndex = buttonGroup->buttonFind(button);
        zfCoreAssert(buttonIndex != zfindexMax);
        if(buttonIndex == buttonGroup->buttonTabChecked())
        {
            if(buttonGroup->buttonTabAllowUnchecked())
            {
                buttonGroup->buttonTabCheckedSet(zfindexMax);
                buttonGroup->_ZFP_ZFUIButtonGroup_buttonTabOnChange(button, buttonIndex);
            }
            else
            {
                button->buttonCheckedSet(zftrue);
            }
            buttonGroup->_ZFP_ZFUIButtonGroup_buttonTabOnClickChecked(button, buttonIndex);
        }
        else
        {
            zfindex buttonIndexPrev = buttonGroup->buttonTabChecked();
            buttonGroup->buttonTabCheckedSet(buttonIndex);
            buttonGroup->_ZFP_ZFUIButtonGroup_buttonTabOnChange(button, buttonIndexPrev);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIButtonGroupListenerHolder_Tab)
static void _ZFP_ZFUIButtonGroup_setup_Tab(ZF_IN ZFUIButtonGroup *buttonGroup,
                                           ZF_IN ZFUIButton *button,
                                           ZF_IN zfindex buttonIndex)
{
    _ZFP_ZFUIButtonGroup_setup_common(buttonGroup, button, buttonIndex);

    button->buttonCheckableSet(zftrue);
    button->buttonCheckedSet(zffalse);

    button->observerAdd(
        ZFUIButton::EventButtonOnClick(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIButtonGroupListenerHolder_Tab)->buttonOnClickListener,
        buttonGroup->toObject()->objectHolder());

    if(!buttonGroup->buttonTabAllowUnchecked() && buttonGroup->buttonTabChecked() == zfindexMax)
    {
        buttonGroup->buttonTabCheckedSet(buttonIndex);
    }
}
static void _ZFP_ZFUIButtonGroup_cleanup_Tab(ZF_IN ZFUIButtonGroup *buttonGroup,
                                             ZF_IN ZFUIButton *button)
{
    _ZFP_ZFUIButtonGroup_cleanup_common(buttonGroup, button);

    button->observerRemove(
        ZFUIButton::EventButtonOnClick(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIButtonGroupListenerHolder_Tab)->buttonOnClickListener);
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIButtonGroup, zfbool, buttonTabAllowUnchecked)
{
    this->buttonTabAllowUncheckedSetInternal(newValue);

    if(!this->buttonTabAllowUnchecked() && this->buttonCount() > 0 && this->buttonTabChecked() == zfindexMax)
    {
        this->buttonTabCheckedSet(0);
    }
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIButtonGroup, zfindex, buttonTabChecked)
{
    zfindex newValueTmp = ((newValue >= this->buttonCount()) ? zfindexMax : newValue);
    if(newValueTmp == this->buttonTabChecked())
    {
        return ;
    }
    if(!this->buttonTabAllowUnchecked() && newValueTmp >= this->buttonCount())
    {
        return ;
    }
    if(this->buttonTabChecked() != zfindexMax)
    {
        this->buttonAtIndex(this->buttonTabChecked())->buttonCheckedSet(zffalse);
    }
    if(newValueTmp != zfindexMax)
    {
        this->buttonAtIndex(newValueTmp)->buttonCheckedSet(zftrue);
    }
    this->buttonTabCheckedSetInternal(newValueTmp);
}

// ============================================================
static void _ZFP_ZFUIButtonGroup_setup(ZF_IN ZFUIButtonGroup *buttonGroup,
                                       ZF_IN ZFUIButton *button,
                                       ZF_IN zfindex buttonIndex)
{
    switch(buttonGroup->buttonGroupType())
    {
        case ZFUIButtonGroupType::e_Normal:
            _ZFP_ZFUIButtonGroup_setup_Normal(buttonGroup, button, buttonIndex);
            break;
        case ZFUIButtonGroupType::e_Tab:
            _ZFP_ZFUIButtonGroup_setup_Tab(buttonGroup, button, buttonIndex);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
static void _ZFP_ZFUIButtonGroup_cleanup(ZF_IN ZFUIButtonGroup *buttonGroup,
                                         ZF_IN ZFUIButton *button)
{
    switch(buttonGroup->buttonGroupType())
    {
        case ZFUIButtonGroupType::e_Normal:
            _ZFP_ZFUIButtonGroup_cleanup_Normal(buttonGroup, button);
            break;
        case ZFUIButtonGroupType::e_Tab:
            _ZFP_ZFUIButtonGroup_cleanup_Tab(buttonGroup, button);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIButtonGroup, ZFUIButtonGroupTypeEnum, buttonGroupType)
{
    if(this->buttonGroupType() == newValue)
    {
        return ;
    }
    for(zfindex i = 0; i < this->buttonCount(); ++i)
    {
        _ZFP_ZFUIButtonGroup_cleanup(this, this->buttonAtIndex(i));
    }
    this->buttonGroupTypeSetInternal(newValue);
    for(zfindex i = 0; i < this->buttonCount(); ++i)
    {
        _ZFP_ZFUIButtonGroup_setup(this, this->buttonAtIndex(i), i);
    }
}

zfindex ZFUIButtonGroup::buttonCount(void)
{
    return this->_ZFP_ZFUIButtonGroup_buttons()->count();
}
zfindex ZFUIButtonGroup::buttonFind(ZF_IN ZFUIButton *button)
{
    return this->_ZFP_ZFUIButtonGroup_buttons()->find(button);
}
ZFUIButton *ZFUIButtonGroup::buttonAtIndex(ZF_IN zfindex buttonIndex)
{
    return this->_ZFP_ZFUIButtonGroup_buttons()->get<ZFUIButton *>(buttonIndex);
}
void ZFUIButtonGroup::buttonAdd(ZF_IN ZFUIButton *button,
                                ZF_IN_OPT zfindex atIndex /* = zfindexMax */)
{
    if(atIndex == zfindexMax)
    {
        atIndex = this->buttonCount();
    }
    else if(atIndex >= this->buttonCount())
    {
        zfCoreCriticalIndexOutOfRange(atIndex, this->buttonCount());
        return ;
    }
    this->_ZFP_ZFUIButtonGroup_buttons()->add(atIndex, button);
    _ZFP_ZFUIButtonGroup_setup(this, button, atIndex);
    this->buttonOnAdd(button, atIndex);
}
void ZFUIButtonGroup::buttonRemove(ZF_IN ZFUIButton *button)
{
    this->buttonRemoveAtIndex(this->_ZFP_ZFUIButtonGroup_buttons()->find(button));
}
void ZFUIButtonGroup::buttonRemoveAtIndex(ZF_IN zfindex buttonIndex)
{
    if(buttonIndex == zfindexMax)
    {
        return ;
    }
    if(buttonIndex >= this->buttonCount())
    {
        zfCoreCriticalIndexOutOfRange(buttonIndex, this->buttonCount());
        return ;
    }
    ZFUIButton *button = this->buttonAtIndex(buttonIndex);
    zfRetainWithoutLeakTest(button);
    _ZFP_ZFUIButtonGroup_cleanup(this, button);
    this->_ZFP_ZFUIButtonGroup_buttons()->remove(buttonIndex);
    this->buttonOnRemove(button, buttonIndex);
    zfReleaseWithoutLeakTest(button);
}
void ZFUIButtonGroup::buttonRemoveAll(void)
{
    while(this->buttonCount() > 0)
    {
        this->buttonRemoveAtIndex(this->buttonCount() - 1);
    }
}

// ============================================================
// ZFUIButtonGroupArray
ZFOBJECT_REGISTER(ZFUIButtonGroupArray)

ZF_NAMESPACE_GLOBAL_END

