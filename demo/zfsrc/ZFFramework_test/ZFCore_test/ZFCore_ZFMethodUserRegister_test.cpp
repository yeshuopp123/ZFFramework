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

// ============================================================
zfclass ZFCore_ZFMethodUserRegister_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFMethodUserRegister_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        {
            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("try execute dynamically registered method"));

            ZFMethodUserRegister_1(method, zfself::wrapper, zfself::ClassData(),
                public, ZFMethodNotStatic, ZFMethodNotVirtual, ZFMethodIsConst,
                zfstring, zfText("func_dynamic"), zfnull,
                zfstring const &, param0, ZFMethodNoDefaultParam);

            zfstring ret = zfself::ClassData()->methodForName(zfText("func_dynamic"))->execute<zfstring, zfstring const &>(this, zfText("paramValue"));
            this->testCaseOutput(zfText("execute result: %s"), ret.cString());

            ZFMethodUserUnregister(zfself::ClassData(), zfText("func_dynamic"), zfnull);

            zfLogTrimT() << zfText("after unregister, method:") << zfself::ClassData()->methodForName(zfText("func_dynamic"));
        }

        {
            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("try execute statically registered method"));

            zfstring ret = zfself::ClassData()->methodForName(zfText("func_static"))->execute<zfstring, zfstring const &>(this, zfText("paramValue"));
            this->testCaseOutput(zfText("execute result: %s"), ret.cString());
        }

        this->testCaseStop();
    }

public:
    static zfstring wrapper(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj, ZF_IN zfstring const &param0)
    {
        zfLogTrimT().autoSpaceSet(zffalse)
            << zfText("method called: ") << method
            << zfText(", obj: ") << obj
            << zfText(", param: ") << param0;
        return zfText("returnValue");
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethodUserRegister_test)

ZFMETHOD_USER_REGISTER_1(ZFCore_ZFMethodUserRegister_test, ZFCore_ZFMethodUserRegister_test::wrapper, ZFCore_ZFMethodUserRegister_test::ClassData(),
    public, ZFMethodNotStatic, ZFMethodNotVirtual, ZFMethodIsConst,
    zfstring, zfText("func_static"), zfnull,
    zfstring const &, param0, ZFMethodNoDefaultParam)

ZF_NAMESPACE_GLOBAL_END

