/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIDialogForChoice.h
 * @brief basic dialog for user to choose items
 */

#ifndef _ZFI_ZFUIDialogForChoice_h_
#define _ZFI_ZFUIDialogForChoice_h_

#include "ZFUIDialogBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief byte order define for ZFFramework
 */
ZFENUM_BEGIN(ZFUIDialogForChoiceMode)
    /**
     * @brief choose only one, click confirm to finish
     */
    ZFENUM_VALUE(ChoiceSingle)
    /**
     * @brief choose only one, single one tap to finish
     */
    ZFENUM_VALUE(ChoiceSingleQuickly)
    /**
     * @brief choose multiple
     */
    ZFENUM_VALUE(ChoiceMultiple)
ZFENUM_SEPARATOR(ZFUIDialogForChoiceMode)
    ZFENUM_VALUE_REGISTER(ChoiceSingle)
    ZFENUM_VALUE_REGISTER(ChoiceSingleQuickly)
    ZFENUM_VALUE_REGISTER(ChoiceMultiple)
ZFENUM_END_WITH_DEFAULT(ZFUIDialogForChoiceMode, ChoiceSingle)

// ============================================================
// ZFUIDialogForChoiceData
/**
 * @brief choice item data for #ZFUIDialogForChoice
 */
zfclass ZF_ENV_EXPORT ZFUIDialogForChoiceData : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(ZFUIDialogForChoiceData, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

    /**
     * @brief id of the item
     */
    ZFPROPERTY_ASSIGN(zfstring, choiceId)
    /**
     * @brief name of the item
     */
    ZFPROPERTY_ASSIGN(zfstring, choiceName)
};

// ============================================================
// ZFUIDialogForChoice
zfclassFwd _ZFP_ZFUIDialogForChoicePrivate;
/**
 * @brief basic dialog for input
 */
zfclass ZF_ENV_EXPORT ZFUIDialogForChoice : zfextends ZFUIDialogBasic
{
    ZFOBJECT_DECLARE(ZFUIDialogForChoice, ZFUIDialogBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogForChoice)

    // ============================================================
    // events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when choice confirmed, either by:
     * -  click choice if choiceMode is #ZFUIDialogForChoiceMode::e_ChoiceSingleQuickly,
     *   implemented by default
     * -  click #ZFUIDialogContent::dialogButton_Yes,
     *   implemented by default
     * -  manually notify by #choiceConfirmNotify
     */
    ZFOBSERVER_EVENT(ChoiceOnConfirm)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when choice changed
     */
    ZFOBSERVER_EVENT(ChoiceOnChange)

public:
    /**
     * @brief choice mode, #ZFUIDialogForChoiceMode::EnumDefault by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIDialogForChoiceModeEnum, choiceMode,
                                ZFPropertyInitValue(ZFUIDialogForChoiceMode::EnumDefault()))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUIDialogForChoiceModeEnum, choiceMode)
    {
        if(newValue != this->choiceMode())
        {
            this->choiceModeSetInternal(newValue);
            this->choiceListOnChange();
            this->_ZFP_choiceStateUpdate();
        }
    }

    /**
     * @brief list of choice items, empty by default,
     *   use #choiceAdd/#choiceRemove to access the choice list
     */
    ZFPROPERTY_RETAIN_DETAIL(ZFArrayEditable *, list, ZFPropertyInitValue(zflineAlloc(ZFArrayEditable)),
                             private, protected)

    /**
     * @brief list of selected items' index, empty by default,
     *   holds #ZFValue::indexValue
     */
    ZFPROPERTY_RETAIN_DETAIL(ZFArrayEditable *, selected, ZFPropertyInitValue(zflineAlloc(ZFArrayEditable)),
                             private, protected)

public:
    /** @brief get choice count */
    zffinal zfindex choiceCount(void)
    {
        return this->list()->count();
    }
    /** @brief get choice at index */
    zffinal ZFUIDialogForChoiceData *choiceAtIndex(ZF_IN zfindex index)
    {
        return this->list()->get<ZFUIDialogForChoiceData *>(index);
    }
    /** @brief get choice id at index */
    zffinal const zfchar *choiceIdAtIndex(ZF_IN zfindex index)
    {
        return this->list()->get<ZFUIDialogForChoiceData *>(index)->choiceId();
    }
    /** @brief get choice name at index */
    zffinal const zfchar *choiceNameAtIndex(ZF_IN zfindex index)
    {
        return this->list()->get<ZFUIDialogForChoiceData *>(index)->choiceName();
    }
    /**
     * @brief add choice data
     */
    zffinal void choiceAdd(ZF_IN ZFUIDialogForChoiceData *choiceData)
    {
        if(choiceData)
        {
            this->list()->add(choiceData);
            this->choiceListOnChange();
            this->_ZFP_choiceStateUpdate();
        }
    }
    /** @brief see #choiceAdd */
    zffinal void choiceAdd(ZF_IN const zfchar *choiceId, ZF_IN const zfchar *choiceName)
    {
        zfblockedAlloc(ZFUIDialogForChoiceData, choiceData);
        choiceData->choiceIdSet(choiceId);
        choiceData->choiceNameSet(choiceName);
        this->choiceAdd(choiceData);
    }
    /** @brief remove choice by id */
    zffinal void choiceRemove(ZF_IN const zfchar *choiceId)
    {
        zfindex index = this->_ZFP_indexForChoiceId(choiceId);
        if(index != zfindexMax)
        {
            this->choiceRemoveAtIndex(index);
        }
    }
    /** @brief remove choice at index */
    zffinal void choiceRemoveAtIndex(ZF_IN zfindex index)
    {
        this->list()->remove(index);
        for(zfindex i = 0; i < this->selected()->count(); ++i)
        {
            zfindex t = this->selected()->get<ZFValue *>(i)->indexValue();
            if(t == index)
            {
                this->selected()->remove(i);
                --i;
            }
            else if(t > index)
            {
                this->selected()->set(i, ZFValue::indexValueCreate(t - 1).toObject());
            }
        }
        this->choiceListOnChange();
        this->_ZFP_choiceStateUpdate();
    }
    /** @brief remove all choice data */
    zffinal void choiceRemoveAll(void)
    {
        this->list()->removeAll();
        this->selected()->removeAll();
        this->choiceListOnChange();
    }

public:
    /** @brief select choice by id, automatically deselect previous for single choice mode */
    zffinal void choiceSelect(ZF_IN const zfchar *choiceId)
    {
        this->choiceSelectAtIndex(this->_ZFP_indexForChoiceId(choiceId));
    }
    /** @brief select choice at index, automatically deselect previous for single choice mode */
    zffinal void choiceSelectAtIndex(ZF_IN zfindex index)
    {
        if(index >= this->list()->count())
        {
            return ;
        }
        zfautoObject indexHolder = ZFValue::indexValueCreate(index);
        if(!this->selected()->isContain(indexHolder.toObject()))
        {
            if(this->_ZFP_singleChoiceMode())
            {
                this->selected()->removeAll();
            }
            this->selected()->add(indexHolder.toObject());
            if(this->selected()->count() > 1)
            {
                this->selected()->sort();
            }
            this->choiceSelectedListOnChange();
        }
    }
    /** @brief select all choice, or select first for single choice mode */
    zffinal void choiceSelectAll(void)
    {
        if(this->_ZFP_singleChoiceMode())
        {
            return ;
        }
        this->selected()->removeAll();
        for(zfindex i = 0; i < this->list()->count(); ++i)
        {
            this->selected()->add(ZFValue::indexValueCreate(i).toObject());
        }
        this->choiceSelectedListOnChange();
    }
    /** @brief deselect choice by id, do nothing for single choice mode */
    zffinal void choiceDeselect(ZF_IN const zfchar *choiceId)
    {
        if(this->_ZFP_singleChoiceMode())
        {
            return ;
        }
        this->choiceDeselectAtIndex(this->_ZFP_indexForChoiceId(choiceId));
    }
    /** @brief deselect choice at index, do nothing for single choice mode */
    zffinal void choiceDeselectAtIndex(ZF_IN zfindex index)
    {
        if(this->_ZFP_singleChoiceMode())
        {
            return ;
        }
        if(index >= this->list()->count())
        {
            return ;
        }
        if(this->selected()->removeElement(ZFValue::indexValueCreate(index).toObject()))
        {
            this->choiceSelectedListOnChange();
        }
    }
    /** @brief deselect all choice, do nothing for single choice mode */
    zffinal void choiceDeselectAll(void)
    {
        if(this->_ZFP_singleChoiceMode())
        {
            return ;
        }
        if(!this->selected()->isEmpty())
        {
            this->selected()->removeAll();
            this->choiceSelectedListOnChange();
        }
    }

public:
    /**
     * @brief true if choice selected
     */
    zffinal zfbool choiceSelected(ZF_IN const zfchar *choiceId)
    {
        zfindex index = this->_ZFP_indexForChoiceId(choiceId);
        if(index != zfindexMax)
        {
            return this->selected()->isContain(ZFValue::indexValueCreate(index).toObject());
        }
        else
        {
            return zffalse;
        }
    }
    /**
     * @brief true if choice selected
     */
    zffinal zfbool choiceSelectedAtIndex(ZF_IN zfindex index)
    {
        return this->selected()->isContain(ZFValue::indexValueCreate(index).toObject());
    }
    /**
     * @brief return all selected choice index
     */
    zffinal ZFCoreArrayPOD<zfindex> choiceSelectedIndexList(void)
    {
        ZFCoreArrayPOD<zfindex> ret;
        ret.capacitySet(this->selected()->count());
        for(zfindex i = 0; i < this->selected()->count(); ++i)
        {
            zfindex index = this->selected()->get<ZFValue *>(i)->indexValue();
            ret.add(index);
        }
        return ret;
    }
    /**
     * @brief return all selected choice id
     */
    zffinal ZFCoreArray<zfstring> choiceSelectedIdList(void)
    {
        ZFCoreArray<zfstring> ret;
        ret.capacitySet(this->selected()->count());
        for(zfindex i = 0; i < this->selected()->count(); ++i)
        {
            zfindex index = this->selected()->get<ZFValue *>(i)->indexValue();
            ret.add(this->choiceIdAtIndex(index));
        }
        return ret;
    }
    /**
     * @brief return all selected choice name
     */
    zffinal ZFCoreArray<zfstring> choiceSelectedNameList(void)
    {
        ZFCoreArray<zfstring> ret;
        ret.capacitySet(this->selected()->count());
        for(zfindex i = 0; i < this->selected()->count(); ++i)
        {
            zfindex index = this->selected()->get<ZFValue *>(i)->indexValue();
            ret.add(this->choiceNameAtIndex(index));
        }
        return ret;
    }

protected:
    /**
     * @brief called when choice list changed
     */
    virtual void choiceListOnChange(void);
    /**
     * @brief called when selected index list changed
     */
    virtual void choiceSelectedListOnChange(void);

private:
    zffinal void _ZFP_choiceStateUpdate(void)
    {
        switch(this->choiceMode())
        {
            case ZFUIDialogForChoiceMode::e_ChoiceSingle:
            case ZFUIDialogForChoiceMode::e_ChoiceSingleQuickly:
                if(this->selected()->count() > 1)
                {
                    this->selected()->remove(1, zfindexMax);
                    this->choiceSelectedListOnChange();
                }
                else if(this->selected()->count() == 0 && !this->list()->isEmpty())
                {
                    this->selected()->add(ZFValue::indexValueCreate(0).toObject());
                    this->choiceSelectedListOnChange();
                }
                break;
            case ZFUIDialogForChoiceMode::e_ChoiceMultiple:
                this->selected()->sort();
                this->choiceSelectedListOnChange();
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
    }
    zffinal zfindex _ZFP_indexForChoiceId(ZF_IN const zfchar *choiceId)
    {
        for(zfindex i = 0; i < this->list()->count(); ++i)
        {
            if(zfscmpTheSame(this->list()->get<ZFUIDialogForChoiceData *>(i)->choiceId().cString(), choiceId))
            {
                return i;
            }
        }
        return zfindexMax;
    }
    zffinal zfbool _ZFP_singleChoiceMode(void)
    {
        switch(this->choiceMode())
        {
            case ZFUIDialogForChoiceMode::e_ChoiceSingle:
            case ZFUIDialogForChoiceMode::e_ChoiceSingleQuickly:
                return zftrue;
            case ZFUIDialogForChoiceMode::e_ChoiceMultiple:
                return zffalse;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }

protected:
    /** @brief see #EventChoiceOnConfirm */
    virtual inline void choiceOnConfirm(void)
    {
        this->observerNotify(ZFUIDialogForChoice::EventChoiceOnConfirm());
    }
    /** @brief see #EventChoiceOnChange */
    virtual inline void choiceOnChange(void)
    {
        this->observerNotify(ZFUIDialogForChoice::EventChoiceOnChange());
    }

public:
    /**
     * @brief manually notify confirm the choice
     *
     * this method is useful if you have addition confirm button or action
     * to finish the choice, see #EventChoiceOnConfirm
     */
    zffinal void choiceConfirmNotify(void)
    {
        this->choiceOnConfirm();
    }
    /**
     * @brief manually notify confirm the choice
     *
     * this method is useful if you have addition change button or action
     * to change the choice, see #EventChoiceOnChange
     */
    zffinal void choiceChangeNotify(void)
    {
        this->choiceOnChange();
    }

public:
    zffinal void _ZFP_ZFUIDialogForChoice_choiceChange(ZF_IN zfindex index, ZF_IN zfbool selected)
    {
        this->selected()->removeElement(ZFValue::indexValueCreate(index).toObject());
        if(selected)
        {
            this->selected()->add(ZFValue::indexValueCreate(index).toObject());
            this->selected()->sort();
        }
    }

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull)
    {
        if(!zfsuper::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        this->choiceListOnChange();
        this->choiceSelectedListOnChange();
        return zftrue;
    }
    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
    {
        zfsuper::styleableOnCopyFrom(anotherStyleable);
        this->choiceListOnChange();
        this->choiceSelectedListOnChange();
    }

    zfoverride
    virtual void dialogButtonOnAdd(ZF_IN ZFUIButton *button);
    zfoverride
    virtual void dialogButtonOnRemove(ZF_IN ZFUIButton *button);

private:
    _ZFP_ZFUIDialogForChoicePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialogForChoice_h_

