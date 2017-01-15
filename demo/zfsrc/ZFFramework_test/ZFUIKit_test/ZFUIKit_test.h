/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZFUIKit_test_h_
#define _ZFI_ZFUIKit_test_h_

#include "../../ZFFramework_test_global.h"
#include "ZFUIKit.h"
#include "ZFUIWidget/ZFUIButtonBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_test_Window : zfextends ZFUIWindow
{
    ZFOBJECT_DECLARE(ZFUIKit_test_Window, ZFUIWindow)

public:
    zfoverride
    virtual void objectOnInitFinish(void)
    {
        zfsuper::objectOnInitFinish();
        this->viewBackgroundColorSet(ZFUIColorWhite);
    }
};

zfclass ZFUIKit_test_Button : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZFUIKit_test_Button, ZFUIButtonBasic)

public:
    zfoverride
    virtual void objectOnInitFinish(void)
    {
        zfsuper::objectOnInitFinish();
        this->buttonBackgroundStyle()->viewBackgroundColorSet(ZFUIColorGreen);
        this->buttonBackgroundStyle(ZFUIButtonState::e_Highlighted)->viewBackgroundColorSet(ZFUIColorBlue);
    }
};

zfclass ZFUIKit_test_ListView : zfextends ZFUIScrollView
{
    ZFOBJECT_DECLARE(ZFUIKit_test_ListView, ZFUIScrollView)

public:
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->scrollBounceVerticalAlwaysSet(zftrue);
        return this;
    }

protected:
    zfoverride
    virtual void layoutParamOnUpdate(ZF_IN ZFUIViewLayoutParam *layoutParam)
    {
        zfsuper::layoutParamOnUpdate(layoutParam);

        layoutParam->sizeParamSet(ZFUISizeParamFillWidthWrapHeight);
        layoutParam->layoutAlignSet(ZFUIAlign::e_TopInner);
    }
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
    {
        zfsuper::layoutOnLayoutPrepare(bounds);

        zfint contentHeight = 0;
        zfint space = 4;
        ZFUISize childMeasureSizeHint = ZFUISizeMake(bounds.size.width - space * 2, -1);
        ZFUISizeParam childMeasureSizeParam = ZFUISizeParamFillWidthWrapHeight;

        for(zfindex i = 0; i < this->childCount(); ++i)
        {
            contentHeight += space;

            ZFUIView *child = this->childAtIndex(i);
            child->layoutMeasure(childMeasureSizeHint, childMeasureSizeParam);
            child->layoutParam()->layoutMarginSet(ZFUIMarginMake(0, contentHeight, 0, 0));

            contentHeight += child->layoutMeasuredSize().height;
        }
        contentHeight += space;

        this->scrollContentFrameSetWhileAnimating(ZFUIRectMake(
            this->scrollContentFrame().point.x, this->scrollContentFrame().point.y,
            bounds.size.width, contentHeight));
    }
};

extern void ZFUIKit_test_prepareTestWindow(ZF_OUT ZFUIWindow *&window,
                                           ZF_OUT ZFUIView *&container,
                                           ZF_IN ZFTestCase *testCaseToStop);

zfclass ZFUIKit_test_SettingData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIKit_test_SettingData, ZFObject)

public:
    ZFOBSERVER_EVENT(SettingOnChange)

public:
    /**
     * param0 is a #ZFStringEditable to get the button's text
     */
    ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE(ZFListener, buttonTextGetter)
    ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE(ZFListener, buttonClickListener)
    ZFPROPERTY_RETAIN(ZFObject *, userData)

public:
    zffinal void settingUpdate(void)
    {
        this->observerNotify(zfself::EventSettingOnChange());
    }

public:
    virtual ZFObject *objectOnInit(ZF_IN const ZFListener &buttonTextGetter,
                                   ZF_IN const ZFListener &buttonClickListener,
                                   ZF_IN_OPT ZFObject *userData = zfnull)
    {
        this->objectOnInit();
        this->buttonTextGetterSet(buttonTextGetter);
        this->buttonClickListenerSet(buttonClickListener);
        this->userDataSet(userData);
        return this;
    }
    virtual ZFObject *objectOnInit(void)
    {
        return zfsuper::objectOnInit();
    }
};
extern zfautoObject ZFUIKit_test_prepareSettingButton(ZF_IN ZFArray *settings);
extern void ZFUIKit_test_prepareSettingButtonWithTestWindow(ZF_IN ZFUIWindow *window,
                                                            ZF_IN ZFArray *settings);
extern void ZFUIKit_test_prepareSettingForProperty(ZF_IN_OUT ZFArrayEditable *settings,
                                                   ZF_IN ZFObject *obj,
                                                   ZF_IN ZFTimeLineProperty *modifier,
                                                   ZF_IN zffloat step);

// ============================================================
extern void ZFUIKit_test_prepareSettingForBoolProperty(ZF_IN_OUT ZFArrayEditable *settings,
                                                       ZF_IN ZFObject *obj,
                                                       ZF_IN const ZFProperty *property);

zfclassNotPOD _ZFP_ZFUIKit_test_prepareSettingForNormalProperty_ProgressBase
{
public:
    virtual ~_ZFP_ZFUIKit_test_prepareSettingForNormalProperty_ProgressBase(void) {}

public:
    virtual void progressOnUpdate(ZF_IN const ZFProperty *ownerProperty,
                                  ZF_IN ZFObject *ownerObj,
                                  ZF_IN zffloat progress) = 0;
    virtual void stateOnSave(ZF_IN const ZFProperty *ownerProperty,
                             ZF_IN ZFObject *ownerObj) = 0;
    virtual void stateOnRestore(ZF_IN const ZFProperty *ownerProperty,
                                ZF_IN ZFObject *ownerObj) = 0;
};
zfclass _ZFP_ZFUIKit_test_prepareSettingForNormalProperty_TimeLine : zfextends ZFTimeLineProperty
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIKit_test_prepareSettingForNormalProperty_TimeLine, ZFTimeLineProperty)
protected:
    zfoverride
    virtual void progressOnUpdate(ZF_IN ZFObject *ownerObj,
                                  ZF_IN zffloat progress)
    {
        zfsuper::progressOnUpdate(ownerObj, progress);
        this->action->progressOnUpdate(this->ownerProperty(), ownerObj, progress);
    }
    zfoverride
    virtual void stateOnSave(ZF_IN ZFObject *ownerObj)
    {
        zfsuper::stateOnSave(ownerObj);
        this->action->stateOnSave(this->ownerProperty(), ownerObj);
    }
    zfoverride
    virtual void stateOnRestore(ZF_IN ZFObject *ownerObj)
    {
        zfsuper::stateOnRestore(ownerObj);
        this->action->stateOnRestore(this->ownerProperty(), ownerObj);
    }
public:
    _ZFP_ZFUIKit_test_prepareSettingForNormalProperty_ProgressBase *action;
public:
    virtual void objectOnDealloc(void)
    {
        zfdelete(this->action);
        zfsuper::objectOnDealloc();
    }
};
#define ZFUIKit_test_prepareSettingForNormalProperty(settings, obj, PropertyType, property, propertyValues_) \
    zfCoreAssert(!propertyValues_.isEmpty()); \
    zfclassNotPOD ZFUniqueName(_ZFP_ZFUIKit_test_prepareSettingForNormalProperty_Progress) : public _ZFP_ZFUIKit_test_prepareSettingForNormalProperty_ProgressBase \
    { \
    public: \
        virtual void progressOnUpdate(ZF_IN const ZFProperty *ownerProperty, \
                                      ZF_IN ZFObject *ownerObj, \
                                      ZF_IN zffloat progress) \
        { \
            zffloat step = 1.0f / propertyValues.count(); \
            zfindex index = (zfindex)((progress + step / 2) / step); \
            ownerProperty->setterMethod()->execute<void, PropertyType const &>(ownerObj, this->propertyValues[index]); \
        } \
        virtual void stateOnSave(ZF_IN const ZFProperty *ownerProperty, \
                                 ZF_IN ZFObject *ownerObj) \
        { \
            this->_savedState = ownerProperty->getterMethod()->execute<PropertyType const &>(ownerObj); \
        } \
        virtual void stateOnRestore(ZF_IN const ZFProperty *ownerProperty, \
                                    ZF_IN ZFObject *ownerObj) \
        { \
            ownerProperty->setterMethod()->execute<void, PropertyType const &>(ownerObj, this->_savedState); \
        } \
    public: \
        ZFCoreArray<PropertyType> propertyValues; \
    private: \
        PropertyType _savedState; \
    }; \
    zfblockedAlloc(_ZFP_ZFUIKit_test_prepareSettingForNormalProperty_TimeLine, ZFUniqueName(modifier)); \
    ZFUniqueName(modifier)->ownerPropertySet(property); \
    ZFUniqueName(modifier)->action = zfnew(ZFUniqueName(_ZFP_ZFUIKit_test_prepareSettingForNormalProperty_Progress)); \
    ((ZFUniqueName(_ZFP_ZFUIKit_test_prepareSettingForNormalProperty_Progress) *)(ZFUniqueName(modifier)->action))->propertyValues = propertyValues_; \
    ZFUIKit_test_prepareSettingForProperty(settings, obj, ZFUniqueName(modifier), 1.0f / propertyValues_.count())

extern void ZFUIKit_test_prepareSettingForLayoutRequest(ZF_IN_OUT ZFArrayEditable *settings,
                                                        ZF_IN ZFUIView *view);

extern void ZFUIKit_test_prepareSettingForResetProperty(ZF_IN_OUT ZFArrayEditable *settings,
                                                        ZF_IN ZFObject *obj,
                                                        ZF_IN const ZFCoreArrayPOD<const ZFProperty *> &propertyList);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFUIKit_test_h_

