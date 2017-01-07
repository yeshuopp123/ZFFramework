/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFCore_ZFSerializable_test_TestClass : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFSerializable_test_TestClass, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

public:
    ZFPROPERTY_ASSIGN(zfstring, stringInParent)

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }
};

zfclass _ZFP_ZFCore_ZFSerializable_test_TestClassChild : zfextends _ZFP_ZFCore_ZFSerializable_test_TestClass
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFSerializable_test_TestClassChild, _ZFP_ZFCore_ZFSerializable_test_TestClass)

    ZFPROPERTY_ASSIGN(zfstring, stringInChild)
};

zfclass _ZFP_ZFCore_ZFSerializable_test_TestClassContainer : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFSerializable_test_TestClassContainer, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

public:
    ZFPROPERTY_RETAIN(ZFObject *, serializableMember)

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        ret += this->classData()->className();
        ZFClassUtil::objectPropertyInfo(ret, this);
    }
};

// ============================================================
zfclass ZFCore_ZFSerializable_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFSerializable_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFSerializable: normal serializable object"));
        this->test(this->obj);

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFSerializable: inherit serializable object"));
        this->test(this->objChild);

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFSerializable: serializable object that contains another serializable object"));
        this->test(this->objContainer);

        this->testCaseStop();
    }
public:
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->memberPrepare();
        return this;
    }
    virtual void objectOnDealloc(void)
    {
        this->memberDestroy();
        zfsuper::objectOnDealloc();
    }
private:
    _ZFP_ZFCore_ZFSerializable_test_TestClass *obj;
    _ZFP_ZFCore_ZFSerializable_test_TestClassChild *objChild;
    _ZFP_ZFCore_ZFSerializable_test_TestClassContainer *objContainer;
    void memberPrepare(void)
    {
        this->obj = zfAlloc(_ZFP_ZFCore_ZFSerializable_test_TestClass);
        this->obj->stringInParentSet(zfText("base's string, with unicode chars: \"啊哦\""));

        this->objChild = zfAlloc(_ZFP_ZFCore_ZFSerializable_test_TestClassChild);
        this->objChild->stringInParentSet(zfText("child's string"));
        this->objChild->stringInChildSet(zfText("child's string in child"));

        this->objContainer = zfAlloc(_ZFP_ZFCore_ZFSerializable_test_TestClassContainer);
        _ZFP_ZFCore_ZFSerializable_test_TestClassChild *objTmp = zfAlloc(_ZFP_ZFCore_ZFSerializable_test_TestClassChild);
        objTmp->stringInParentSet(zfText("container's string"));
        objTmp->stringInChildSet(zfText("container's string"));
        this->objContainer->serializableMemberSet(objTmp);
        zfRelease(objTmp);
    }
    void memberDestroy(void)
    {
        zfRelease(this->obj);
        this->obj = zfnull;
        zfRelease(this->objChild);
        this->objChild = zfnull;
        zfRelease(this->objContainer);
        this->objContainer = zfnull;
    }
    void test(ZFSerializable *serializableObj)
    {
        zfstringA encodedData;

        this->testCaseOutput(zfText("object:\n%s\n"), serializableObj->toObject()->objectInfo().cString());

        ZFObjectToString(encodedData, serializableObj->toObject());
        this->testCaseOutput(zfText("encodedData:\n%s\n"), encodedData.cString());

        ZFSerializableData serializableData;
        serializableObj->serializeToData(serializableData);
        this->testCaseOutput(zfText("serializableData:\n%s\n"), serializableData.objectInfo().cString());

        zfautoObject newSerializableObj = ZFObjectFromString(encodedData.cString());
        this->testCaseOutput(zfText("re-serialize from encodedData, result:\n%s\n"), newSerializableObj.toObject()->objectInfo().cString());
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFSerializable_test)

ZF_NAMESPACE_GLOBAL_END

