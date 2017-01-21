/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIDialogBasic.h
 * @brief basic dialog
 */

#ifndef _ZFI_ZFUIDialogBasic_h_
#define _ZFI_ZFUIDialogBasic_h_

#include "ZFUIDialog.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDialogButtonType
/**
 * @brief button state
 */
ZFENUM_BEGIN(ZFUIDialogButtonType)
    ZFENUM_VALUE(Normal) /**< @brief normal button */
    ZFENUM_VALUE(Yes) /**< @brief yes button */
    ZFENUM_VALUE(No) /**< @brief no button */
    ZFENUM_VALUE(Cancel) /**< @brief cancel button */
    ZFENUM_VALUE(Destructive) /**< @brief for destructive operations */
ZFENUM_SEPARATOR(ZFUIDialogButtonType)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Yes)
    ZFENUM_VALUE_REGISTER(No)
    ZFENUM_VALUE_REGISTER(Cancel)
    ZFENUM_VALUE_REGISTER(Destructive)
ZFENUM_END(ZFUIDialogButtonType)

// ============================================================
/**
 * @brief abstract dialog content for #ZFUIDialogBasic
 */
zfinterface ZF_ENV_EXPORT ZFUIDialogContent : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIDialogContent, ZFInterface)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when dialog button added,
     * param0 is the dialog button
     */
    ZFOBSERVER_EVENT(DialogButtonOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when dialog button removed,
     * param0 is the dialog button
     */
    ZFOBSERVER_EVENT(DialogButtonOnRemove)

public:
    // ============================================================
    // title
    /**
     * @brief container to hold custom title views
     */
    virtual ZFUIView *dialogTitleContainer(void) zfpurevirtual;
    /**
     * @brief title text view
     */
    virtual ZFUITextView *dialogTitleView(void) zfpurevirtual;
    /**
     * @brief set the dialog's title text
     */
    virtual void dialogTitleTextSet(ZF_IN const zfchar *text)
    {
        this->dialogTitleView()->textContentStringSet(text);
    }
    /**
     * @brief get the dialog's title text
     */
    virtual const zfchar *dialogTitleText(void)
    {
        return this->dialogTitleView()->textContentString();
    }

    // ============================================================
    // content
    /**
     * @brief container to hold custom content views
     */
    virtual ZFUIView *dialogContentContainer(void) zfpurevirtual;
    /**
     * @brief content text view
     */
    virtual ZFUITextView *dialogContentView(void) zfpurevirtual;
    /**
     * @brief set the dialog's content text
     */
    virtual void dialogContentTextSet(ZF_IN const zfchar *text)
    {
        this->dialogContentView()->textContentStringSet(text);
    }
    /**
     * @brief get the dialog's content text
     */
    virtual const zfchar *dialogContentText(void)
    {
        return this->dialogContentView()->textContentString();
    }

    // ============================================================
    // button
    /**
     * @brief container to hold custom button views
     */
    virtual ZFUIView *dialogButtonContainer(void) zfpurevirtual;
    /**
     * @brief access dialog button with specifed type
     *
     * if autoCreateIfNotExist is not true,
     * button won't be created automatically,
     * and may return null if not exist,
     * see #dialogButtonRemove\n
     * access #ZFUIDialogButtonType::e_Normal would always return null
     */
    virtual ZFUIButton *dialogButton(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                     ZF_IN_OPT zfbool autoCreateIfNotExist = zftrue) zfpurevirtual;
    /**
     * @brief text of the button
     */
    virtual const zfchar *dialogButtonText(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType) zfpurevirtual;
    /**
     * @brief see #dialogButtonText
     */
    virtual void dialogButtonTextSet(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                     ZF_IN const zfchar *text) zfpurevirtual;
    /**
     * @brief remove specified button type, see #dialogButton
     */
    virtual void dialogButtonRemove(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType) zfpurevirtual;

    /** @brief util method to access #dialogButton */
    virtual inline ZFUIButton *dialogButton_Yes(ZF_IN_OPT zfbool autoCreateIfNotExist = zftrue)
    {
        return this->dialogButton(ZFUIDialogButtonType::e_Yes, autoCreateIfNotExist);
    }
    /** @brief util method to access #dialogButton */
    virtual inline ZFUIButton *dialogButton_No(ZF_IN_OPT zfbool autoCreateIfNotExist = zftrue)
    {
        return this->dialogButton(ZFUIDialogButtonType::e_No, autoCreateIfNotExist);
    }
    /** @brief util method to access #dialogButton */
    virtual inline ZFUIButton *dialogButton_Cancel(ZF_IN_OPT zfbool autoCreateIfNotExist = zftrue)
    {
        return this->dialogButton(ZFUIDialogButtonType::e_Cancel, autoCreateIfNotExist);
    }
    /** @brief util method to access #dialogButton */
    virtual inline ZFUIButton *dialogButton_Destructive(ZF_IN_OPT zfbool autoCreateIfNotExist = zftrue)
    {
        return this->dialogButton(ZFUIDialogButtonType::e_Destructive, autoCreateIfNotExist);
    }

    /** @brief util method to access #dialogButtonText */
    virtual inline const zfchar *dialogButtonText_Yes(void)
    {
        return this->dialogButtonText(ZFUIDialogButtonType::e_Yes);
    }
    /** @brief util method to access #dialogButtonTextSet */
    virtual inline void dialogButtonTextSet_Yes(ZF_IN const zfchar *text)
    {
        this->dialogButtonTextSet(ZFUIDialogButtonType::e_Yes, text);
    }
    /** @brief util method to access #dialogButtonText */
    virtual inline const zfchar *dialogButtonText_No(void)
    {
        return this->dialogButtonText(ZFUIDialogButtonType::e_No);
    }
    /** @brief util method to access #dialogButtonTextSet */
    virtual inline void dialogButtonTextSet_No(ZF_IN const zfchar *text)
    {
        this->dialogButtonTextSet(ZFUIDialogButtonType::e_No, text);
    }
    /** @brief util method to access #dialogButtonText */
    virtual inline const zfchar *dialogButtonText_Cancel(void)
    {
        return this->dialogButtonText(ZFUIDialogButtonType::e_Cancel);
    }
    /** @brief util method to access #dialogButtonTextSet */
    virtual inline void dialogButtonTextSet_Cancel(ZF_IN const zfchar *text)
    {
        this->dialogButtonTextSet(ZFUIDialogButtonType::e_Cancel, text);
    }
    /** @brief util method to access #dialogButtonText */
    virtual inline const zfchar *dialogButtonText_Destructive(void)
    {
        return this->dialogButtonText(ZFUIDialogButtonType::e_Destructive);
    }
    /** @brief util method to access #dialogButtonTextSet */
    virtual inline void dialogButtonTextSet_Destructive(ZF_IN const zfchar *text)
    {
        this->dialogButtonTextSet(ZFUIDialogButtonType::e_Destructive, text);
    }

    // ============================================================
    // button
    /**
     * @brief button count with #ZFUIDialogButtonType::e_Normal type
     */
    virtual zfindex dialogButtonCount(void) zfpurevirtual;
    /**
     * @brief access button at index, for #ZFUIDialogButtonType::e_Normal type only
     */
    virtual ZFUIButton *dialogButtonAtIndex(ZF_IN zfindex index) zfpurevirtual;
    /**
     * @brief find button's index, for #ZFUIDialogButtonType::e_Normal type only
     */
    virtual zfindex dialogButtonFind(ZF_IN ZFUIButton *dialogButton) zfpurevirtual;
    /**
     * @brief manually add a button with #ZFUIDialogButtonType::e_Normal type
     */
    virtual void dialogButtonAdd(ZF_IN ZFUIButton *button,
                                 ZF_IN_OPT zfindex atIndex = zfindexMax) zfpurevirtual;
    /**
     * @brief manually remove a specified button, which can be #dialogButton
     */
    virtual void dialogButtonRemove(ZF_IN ZFUIButton *button) zfpurevirtual;
    /**
     * @brief manually remove a specified button at index
     */
    virtual void dialogButtonRemoveAtIndex(ZF_IN zfindex index) zfpurevirtual;
    /**
     * @brief manually remove all button
     */
    virtual void dialogButtonRemoveAll(void) zfpurevirtual;

protected:
    /** @brief see #EventDialogButtonOnAdd */
    virtual inline void dialogButtonOnAdd(ZF_IN ZFUIButton *button)
    {
        this->toObject()->observerNotify(ZFUIDialogContent::EventDialogButtonOnAdd(), button);
    }
    /** @brief see #EventDialogButtonOnRemove */
    virtual inline void dialogButtonOnRemove(ZF_IN ZFUIButton *button)
    {
        this->toObject()->observerNotify(ZFUIDialogContent::EventDialogButtonOnRemove(), button);
    }
};

// ============================================================
/**
 * @brief set the default class of content view for #ZFUIDialogBasic
 *
 * #ZFUIDialogContentBasic by default,
 * set null to reset to default
 */
extern ZF_ENV_EXPORT void ZFUIDialogContentClassSet(ZF_IN const ZFClass *cls);
/**
 * @brief see #ZFUIDialogContentClassSet
 */
extern ZF_ENV_EXPORT const ZFClass *ZFUIDialogContentClass(void);

// ============================================================
// ZFUIDialogBasic
/**
 * @brief basic dialog with title, content and buttons
 *
 * actual dialog content is implemented by #ZFUIDialogContent,
 * you may change the default impl by #ZFUIDialogContentClassSet,
 * or directly change #ZFUIDialogBasic::dialogContent
 */
zfclass ZF_ENV_EXPORT ZFUIDialogBasic : zfextends ZFUIDialog, zfimplements ZFUIDialogContent
{
    ZFOBJECT_DECLARE(ZFUIDialogBasic, ZFUIDialog)
    ZFIMPLEMENTS_DECLARE(ZFUIDialogContent)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogBasic)

public:
    /**
     * @brief dialog content, create instance from #ZFUIDialogContentClass by default
     */
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIDialogContent *, dialogContent,
                                ZFPropertyInitValue(ZFUIDialogContentClass()->newInstance().to<ZFUIDialogContent *>()))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUIDialogContent *, dialogContent);

public:
    // ============================================================
    // title
    zfoverride
    virtual inline ZFUIView *dialogTitleContainer(void) {return this->dialogContent()->dialogTitleContainer();}
    zfoverride
    virtual inline ZFUITextView *dialogTitleView(void) {return this->dialogContent()->dialogTitleView();}

    // ============================================================
    // content
    zfoverride
    virtual inline ZFUIView *dialogContentContainer(void) {return this->dialogContent()->dialogContentContainer();}
    zfoverride
    virtual inline ZFUITextView *dialogContentView(void) {return this->dialogContent()->dialogContentView();}

    // ============================================================
    // button
    zfoverride
    virtual inline ZFUIView *dialogButtonContainer(void) {return this->dialogContent()->dialogButtonContainer();}
    zfoverride
    virtual inline ZFUIButton *dialogButton(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                            ZF_IN_OPT zfbool autoCreateIfNotExist = zftrue)
    {return this->dialogContent()->dialogButton(dialogButtonType, autoCreateIfNotExist);}
    zfoverride
    virtual inline const zfchar *dialogButtonText(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType)
    {return this->dialogContent()->dialogButtonText(dialogButtonType);}
    zfoverride
    virtual inline void dialogButtonTextSet(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                            ZF_IN const zfchar *text)
    {this->dialogContent()->dialogButtonTextSet(dialogButtonType, text);}
    zfoverride
    virtual inline void dialogButtonRemove(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType) {this->dialogContent()->dialogButtonRemove(dialogButtonType);}

    // ============================================================
    // button
    zfoverride
    virtual inline zfindex dialogButtonCount(void) {return this->dialogContent()->dialogButtonCount();}
    zfoverride
    virtual inline ZFUIButton *dialogButtonAtIndex(ZF_IN zfindex index) {return this->dialogContent()->dialogButtonAtIndex(index);}
    zfoverride
    virtual inline zfindex dialogButtonFind(ZF_IN ZFUIButton *dialogButton) {return this->dialogContent()->dialogButtonFind(dialogButton);}
    zfoverride
    virtual inline void dialogButtonAdd(ZF_IN ZFUIButton *button,
                                        ZF_IN_OPT zfindex atIndex = zfindexMax)
    {this->dialogContent()->dialogButtonAdd(button, atIndex);}
    zfoverride
    virtual inline void dialogButtonRemove(ZF_IN ZFUIButton *button) {this->dialogContent()->dialogButtonRemove(button);}
    zfoverride
    virtual inline void dialogButtonRemoveAtIndex(ZF_IN zfindex index) {this->dialogContent()->dialogButtonRemoveAtIndex(index);}
    zfoverride
    virtual inline void dialogButtonRemoveAll(void) {this->dialogContent()->dialogButtonRemoveAll();}

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zffinal void _ZFP_ZFUIDialogBasic_dialogButtonOnAdd(ZF_IN ZFUIButton *button)
    {
        this->dialogButtonOnAdd(button);
    }
    zffinal void _ZFP_ZFUIDialogBasic_dialogButtonOnRemove(ZF_IN ZFUIButton *button)
    {
        this->dialogButtonOnRemove(button);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialogBasic_h_

#include "ZFUIDialogContentBasic.h"

