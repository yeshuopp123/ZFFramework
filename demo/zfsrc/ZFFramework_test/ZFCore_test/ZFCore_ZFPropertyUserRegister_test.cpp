/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCore_test.h"

#include "ZFAlgorithm/ZFXml.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass _ZFP_ZFCore_ZFPropertyUserRegister_test_Object : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFPropertyUserRegister_test_Object, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)
public:
    ZFPROPERTY_ASSIGN(zfindex, valueNormal)
};
zfclass ZFCore_ZFPropertyUserRegister_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFPropertyUserRegister_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        {
            zfblockedAlloc(_ZFP_ZFCore_ZFPropertyUserRegister_test_Object, obj);
            obj->valueNormalSet(1);

            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("try modify and serialize a dynamically registered property, result"));

            ZFPropertyUserRegisterAssign(propertyInfo, _ZFP_ZFCore_ZFPropertyUserRegister_test_Object::ClassData(),
                zfstring, zfText("valueDynamic"), ZFPropertyNoInitValue,
                public, public,
                ZFPropertyUserRegisterDefaultImplAssign<zfstring>::callbackSet,
                ZFPropertyUserRegisterDefaultImplAssign<zfstring>::callbackGet,
                ZFPropertyUserRegisterDefaultImplAssign<zfstring>::callbackIsValueAccessed,
                ZFPropertyUserRegisterDefaultImplAssign<zfstring>::callbackIsInitValue,
                ZFPropertyUserRegisterDefaultImplAssign<zfstring>::callbackResetInitValue,
                ZFPropertyUserRegisterDefaultImplAssign<zfstring>::callbackCompare,
                ZFPropertyUserRegisterDefaultImplAssign<zfstring>::callbackCopy,
                ZFPropertyUserRegisterDefaultImplAssign<zfstring>::callbackGetInfo);
            obj->classData()->propertyForName(zfText("valueDynamic"))->setterMethod()
                ->execute<void, zfstring const &>(obj, zfText("value"));
            ZFXmlPrint(obj);
            ZFPropertyUserUnregister(_ZFP_ZFCore_ZFPropertyUserRegister_test_Object::ClassData(), zfText("valueDynamic"));

            this->testCaseOutput(zfText("after unregister:"));
            ZFXmlPrint(obj);
        }

        {
            zfblockedAlloc(_ZFP_ZFCore_ZFPropertyUserRegister_test_Object, obj);
            obj->valueNormalSet(1);

            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("try modify and serialize a statically registered property, result"));

            obj->classData()->propertyForName(zfText("valueStatic"))->setterMethod()
                ->execute<void, ZFString * const &>(obj, zflineAlloc(ZFString, zfText("value")));
            ZFXmlPrint(obj);
        }

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFPropertyUserRegister_test)

ZFPROPERTY_USER_REGISTER_RETAIN(ZFCore_ZFPropertyUserRegister_test, _ZFP_ZFCore_ZFPropertyUserRegister_test_Object::ClassData(),
    ZFString *, zfText("valueStatic"), ZFPropertyNoInitValue,
    public, public,
    ZFPropertyUserRegisterDefaultImplRetain<ZFString *>::callbackSet,
    ZFPropertyUserRegisterDefaultImplRetain<ZFString *>::callbackGet,
    ZFPropertyUserRegisterDefaultImplRetain<ZFString *>::callbackIsValueAccessed,
    ZFPropertyUserRegisterDefaultImplRetain<ZFString *>::callbackIsInitValue,
    ZFPropertyUserRegisterDefaultImplRetain<ZFString *>::callbackResetInitValue,
    ZFPropertyUserRegisterDefaultImplRetain<ZFString *>::callbackCompare,
    ZFPropertyUserRegisterDefaultImplRetain<ZFString *>::callbackCopy,
    ZFPropertyUserRegisterDefaultImplRetain<ZFString *>::callbackGetInfo)

ZF_NAMESPACE_GLOBAL_END

