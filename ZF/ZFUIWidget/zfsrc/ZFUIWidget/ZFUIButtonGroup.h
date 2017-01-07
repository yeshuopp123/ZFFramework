/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIButtonGroup.h
 * @brief abstract button group management
 */

#ifndef _ZFI_ZFUIButtonGroup_h_
#define _ZFI_ZFUIButtonGroup_h_

#include "ZFUIButton.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonGroupType
/**
 * @brief button group type
 */
ZFENUM_BEGIN(ZFUIButtonGroupType)
    /**
     * @brief normal, all buttons are added without any modification
     */
    ZFENUM_VALUE(Normal)
    /**
     * @brief act like tab management, all buttons are changed to checkable,
     *   and only one button can be checked at one time
     *
     * this type can be used as tab buttons and ratio buttons
     */
    ZFENUM_VALUE(Tab)
ZFENUM_SEPARATOR(ZFUIButtonGroupType)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Tab)
ZFENUM_END_WITH_DEFAULT(ZFUIButtonGroupType, Normal)

// ============================================================
// ZFUIButtonGroup
/**
 * @brief abstract button group management
 *
 * you have two basic ways to use a button group to manage button views:
 * -  implements from #ZFUIButtonGroup,
 *   override #ZFUIButtonGroup::buttonOnAdd and #ZFUIButtonGroup::buttonOnRemove
 *   to manage button views
 * -  use #ZFUIButtonGroupArray to manage buttons,
 *   and observe #ZFUIButtonGroup::EventButtonOnAdd and #ZFUIButtonGroup::EventButtonOnRemove
 *   to manage button views
 *
 * after buttons added, you may observe #ZFUIButton's button event to achieve your logic\n
 * see #ZFUIButtonGroup::buttonGroupOnEvent for more info
 */
zfinterface ZF_ENV_EXPORT ZFUIButtonGroup : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIButtonGroup, ZFInterface)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button added,
     * sender is the button,
     * param0 is this button group itself,
     * param1 is a #ZFValue::indexValue which shows the button's index
     */
    ZFOBSERVER_EVENT(ButtonOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button removed,
     * sender is the button,
     * param0 is this button group itself,
     * param1 is a #ZFValue::indexValue which shows the button's index
     */
    ZFOBSERVER_EVENT(ButtonOnRemove)

public:
    /**
     * @brief button group's type, see #ZFUIButtonGroupType::e_Normal for more info
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIButtonGroupTypeEnum, buttonGroupType,
                                ZFPropertyInitValue(ZFUIButtonGroupType::EnumDefault()))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUIButtonGroupTypeEnum, buttonGroupType);

private:
    ZFPROPERTY_RETAIN_WITH_INIT(ZFArrayEditable *, _ZFP_ZFUIButtonGroup_buttons, ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFArrayEditable)))
public:
    /**
     * @brief button count
     */
    virtual zfindex buttonCount(void);
    /**
     * @brief find the button's index or return zfindexMax if not exist
     */
    virtual zfindex buttonFind(ZF_IN ZFUIButton *button);
    /**
     * @brief get button at index
     */
    virtual ZFUIButton *buttonAtIndex(ZF_IN zfindex buttonIndex);
    /**
     * @brief add button
     *
     * newly added button's setting would be changed accorrding #buttonGroupType,
     * before #buttonOnAdd
     */
    virtual void buttonAdd(ZF_IN ZFUIButton *button,
                           ZF_IN_OPT zfindex atIndex = zfindexMax);
    /**
     * @brief remove button or do nothing if not in this button group
     *
     * removed button's setting won't be reset by this method
     */
    virtual void buttonRemove(ZF_IN ZFUIButton *button);
    /**
     * @brief remove button at index
     *
     * removed button's setting won't be reset by this method
     */
    virtual void buttonRemoveAtIndex(ZF_IN zfindex buttonIndex);
    /**
     * @brief remove all button
     */
    virtual void buttonRemoveAll(void);

public:
    zffinal inline void _ZFP_ZFUIButtonGroup_buttonGroupOnEvent(ZF_IN ZFUIButton *button,
                                                                ZF_IN zfindex buttonIndex,
                                                                ZF_IN zfidentity eventId)
    {
        this->buttonGroupOnEvent(button, buttonIndex, eventId);
    }
protected:
    /**
     * @brief called when any of button event are notified
     *   to the buttons managed by this group
     *
     * here's a list of button event that would be fired:
     * -  #ZFUIButton::EventButtonOnClick
     * -  #ZFUIButton::EventButtonStateOnChange
     * -  #ZFUIButton::EventButtonMouseOnDown
     * -  #ZFUIButton::EventButtonMouseOnMoveEnter
     * -  #ZFUIButton::EventButtonMouseOnMoveExit
     * -  #ZFUIButton::EventButtonMouseOnMoveInside
     * -  #ZFUIButton::EventButtonMouseOnMoveOutside
     * -  #ZFUIButton::EventButtonMouseOnUpInside
     * -  #ZFUIButton::EventButtonMouseOnUp
     *
     * for example:
     * @code
     *   ZFUIButtonGroup *buttonGroup = ...;
     *   buttonGroup->toObject()->observerAdd(ZFUIButton::EventButtonOnClick(), myObserver);
     * @endcode
     * note:
     * sender is the button that fired the button event,
     * param0 is the button group itself,
     * param1 is a #ZFValue::indexValue which shows the button's index
     */
    virtual inline void buttonGroupOnEvent(ZF_IN ZFUIButton *button,
                                           ZF_IN zfindex buttonIndex,
                                           ZF_IN zfidentity eventId)
    {
        this->toObject()->observerNotifyWithCustomSender(
            button,
            eventId,
            this->toObject(),
            ZFValue::indexValueCreate(buttonIndex).toObject());
    }
    /** @brief see #EventButtonOnAdd */
    virtual inline void buttonOnAdd(ZF_IN ZFUIButton *button,
                                    ZF_IN zfindex buttonIndex)
    {
        this->toObject()->observerNotifyWithCustomSender(
            button,
            ZFUIButtonGroup::EventButtonOnAdd(),
            this->toObject(),
            ZFValue::indexValueCreate(buttonIndex).toObject());
    }
    /** @brief see #EventButtonOnRemove */
    virtual inline void buttonOnRemove(ZF_IN ZFUIButton *button,
                                       ZF_IN zfindex buttonIndex)
    {
        this->toObject()->observerNotifyWithCustomSender(
            button,
            ZFUIButtonGroup::EventButtonOnRemove(),
            this->toObject(),
            ZFValue::indexValueCreate(buttonIndex).toObject());
    }

    // ============================================================
    // for ZFUIButtonGroupType::e_Tab type
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * for #ZFUIButtonGroupType::e_Tab type only\n
     * called when #buttonTabChecked changed,
     * sender is the newly clicked button,
     * param0 is the button group itself,
     * param1 is a #ZFValue::indexValue which shows the previous checked button index
     * (may be #zfindexMax if nothing checked previously)
     */
    ZFOBSERVER_EVENT(ButtonTabOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * for #ZFUIButtonGroupType::e_Tab type only\n
     * called when checked button clicked,
     * sender is the clicked button,
     * param0 is the button group itself,
     * param1 is a #ZFValue::indexValue which shows the button's index
     */
    ZFOBSERVER_EVENT(ButtonTabOnClickChecked)

    /**
     * @brief for #ZFUIButtonGroupType::e_Tab type only, whether allow uncheck all button, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, buttonTabAllowUnchecked, ZFPropertyInitValue(zffalse))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfbool, buttonTabAllowUnchecked);

    /**
     * @brief for #ZFUIButtonGroupType::e_Tab type only, the checked tab index, zfindexMax by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, buttonTabChecked, ZFPropertyInitValue(zfindexMax))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfindex, buttonTabChecked);

public:
    zffinal inline void _ZFP_ZFUIButtonGroup_buttonTabOnChange(ZF_IN ZFUIButton *button,
                                                               ZF_IN zfindex buttonIndexPrev)
    {
        this->buttonTabOnChange(button, buttonIndexPrev);
    }
    zffinal inline void _ZFP_ZFUIButtonGroup_buttonTabOnClickChecked(ZF_IN ZFUIButton *button,
                                                                     ZF_IN zfindex buttonIndex)
    {
        this->buttonTabOnClickChecked(button, buttonIndex);
    }
protected:
    /** @brief see #EventButtonTabOnChange */
    virtual inline void buttonTabOnChange(ZF_IN ZFUIButton *button,
                                          ZF_IN zfindex buttonIndexPrev)
    {
        this->toObject()->observerNotifyWithCustomSender(
            button,
            ZFUIButtonGroup::EventButtonTabOnChange(),
            this->toObject(),
            ZFValue::indexValueCreate(buttonIndexPrev).toObject());
    }
    /** @brief see #EventButtonTabOnClickChecked */
    virtual inline void buttonTabOnClickChecked(ZF_IN ZFUIButton *button,
                                                ZF_IN zfindex buttonIndex)
    {
        this->toObject()->observerNotifyWithCustomSender(
            button,
            ZFUIButtonGroup::EventButtonTabOnClickChecked(),
            this->toObject(),
            ZFValue::indexValueCreate(buttonIndex).toObject());
    }
};

// ============================================================
// ZFUIButtonGroupArray
/**
 * @brief simply button group that only holds buttons
 */
zfclass ZF_ENV_EXPORT ZFUIButtonGroupArray : zfextends ZFObject, zfimplements ZFUIButtonGroup
{
    ZFOBJECT_DECLARE(ZFUIButtonGroupArray, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIButtonGroup)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonGroup_h_

