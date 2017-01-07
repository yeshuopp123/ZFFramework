/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIDialogBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIDialogButtonType)

ZFOBJECT_REGISTER(ZFUIDialogContent)

ZFOBSERVER_EVENT_REGISTER(ZFUIDialogContent, DialogButtonOnAdd)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialogContent, DialogButtonOnRemove)

static const ZFClass *_ZFP_ZFUIDialogContentClass = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogContentClassInit, ZFLevelZFFrameworkNormal)
{
    _ZFP_ZFUIDialogContentClass = ZFUIDialogContentBasic::ClassData();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogContentClassInit)
void ZFUIDialogContentClassSet(ZF_IN const ZFClass *cls)
{
    if(cls == zfnull)
    {
        _ZFP_ZFUIDialogContentClass = ZFUIDialogContentBasic::ClassData();
    }
    else if(!cls->classIsTypeOf(ZFUIDialogContent::ClassData()))
    {
        zfCoreCriticalClassNotTypeOf(cls, ZFUIDialogContent::ClassData());
        return ;
    }
    else
    {
        _ZFP_ZFUIDialogContentClass = cls;
    }
}
const ZFClass *ZFUIDialogContentClass(void)
{
    return _ZFP_ZFUIDialogContentClass;
}

ZFSTYLE_DEFAULT_DEFINE(ZFUIDialogBasic)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogBasicDataHolder, ZFLevelZFFrameworkNormal)
{
    this->dialogButtonOnAddListener = ZFCallbackForRawFunction(zfself::dialogButtonOnAdd);
    this->dialogButtonOnRemoveListener = ZFCallbackForRawFunction(zfself::dialogButtonOnRemove);
}
public:
    ZFListener dialogButtonOnAddListener;
    ZFListener dialogButtonOnRemoveListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(dialogButtonOnAdd)
    {
        userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIDialogBasic *>()
            ->_ZFP_ZFUIDialogBasic_dialogButtonOnAdd(listenerData.param0->to<ZFUIButton *>());
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(dialogButtonOnRemove)
    {
        userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIDialogBasic *>()
            ->_ZFP_ZFUIDialogBasic_dialogButtonOnRemove(listenerData.param0->to<ZFUIButton *>());
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogBasicDataHolder)

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIDialogBasic, ZFUIDialogContent *, dialogContent)
{
    if(newValue == zfnull)
    {
        zfCoreCriticalMessage(zfTextA("change dialogContent to null is not allowed"));
        return ;
    }
    else if(!newValue->classData()->classIsTypeOf(ZFUIDialogContent::ClassData()))
    {
        zfCoreCriticalClassNotTypeOf(newValue->classData(), ZFUIDialogContent::ClassData());
        return ;
    }

    if(this->dialogContent() != zfnull)
    {
        this->dialogContent()->toObject()->observerRemove(
            ZFUIDialogContent::EventDialogButtonOnAdd(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogBasicDataHolder)->dialogButtonOnAddListener);
        this->dialogContent()->toObject()->observerRemove(
            ZFUIDialogContent::EventDialogButtonOnRemove(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogBasicDataHolder)->dialogButtonOnRemoveListener);
        this->dialogContent()->to<ZFUIView *>()->viewRemoveFromParent();
    }

    this->dialogContentSetInternal(newValue);

    this->dialogInternalContainer()->childAdd(this->dialogContent()->to<ZFUIView *>());
    this->dialogContent()->to<ZFUIView *>()->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);

    this->dialogContent()->toObject()->observerAdd(
        ZFUIDialogContent::EventDialogButtonOnAdd(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogBasicDataHolder)->dialogButtonOnAddListener,
        this->objectHolder());
    this->dialogContent()->toObject()->observerAdd(
        ZFUIDialogContent::EventDialogButtonOnRemove(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogBasicDataHolder)->dialogButtonOnRemoveListener,
        this->objectHolder());
}

ZFObject *ZFUIDialogBasic::objectOnInit(void)
{
    zfsuper::objectOnInit();

    this->dialogContentSet(this->dialogContent());

    return this;
}
void ZFUIDialogBasic::objectOnDeallocPrepare(void)
{
    this->dialogContent()->dialogButtonRemoveAll();
    this->dialogContent()->dialogButtonRemove(ZFUIDialogButtonType::e_Yes);
    this->dialogContent()->dialogButtonRemove(ZFUIDialogButtonType::e_No);
    this->dialogContent()->dialogButtonRemove(ZFUIDialogButtonType::e_Cancel);
    this->dialogContent()->dialogButtonRemove(ZFUIDialogButtonType::e_Destructive);

    zfsuper::objectOnDeallocPrepare();
}
void ZFUIDialogBasic::objectOnDealloc(void)
{
    this->dialogContent()->toObject()->observerRemove(
        ZFUIDialogContent::EventDialogButtonOnAdd(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogBasicDataHolder)->dialogButtonOnAddListener);
    this->dialogContent()->toObject()->observerRemove(
        ZFUIDialogContent::EventDialogButtonOnRemove(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogBasicDataHolder)->dialogButtonOnRemoveListener);

    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

