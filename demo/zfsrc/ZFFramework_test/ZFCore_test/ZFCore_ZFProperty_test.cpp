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

zfclass _ZFP_ZFCore_ZFProperty_test_TestBase : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFProperty_test_TestBase, ZFObject)

public:
    // retain
    ZFPROPERTY_RETAIN(ZFObject *, propertyRetain)

    ZFPROPERTY_RETAIN(ZFObject *, propertyRetainCustomSetter)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFObject *, propertyRetainCustomSetter)
    {
        zfLogT() << newValue;
        this->propertyRetainCustomSetterSetInternal(newValue);
    }

    ZFPROPERTY_RETAIN_READONLY(ZFObject *, propertyRetainReadonly, ZFPropertyNoInitValue)

    ZFPROPERTY_RETAIN_READONLY(ZFObject *, propertyRetainReadonlyCustomSetter, ZFPropertyNoInitValue)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFObject *, propertyRetainReadonlyCustomSetter)
    {
        zfLogT() << newValue;
        this->propertyRetainReadonlyCustomSetterSetInternal(newValue);
    }

    // assign
    ZFPROPERTY_ASSIGN(zfstring, propertyAssign)

    ZFPROPERTY_ASSIGN(zfstring, propertyAssignCustomSetter)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfstring, propertyAssignCustomSetter)
    {
        zfLogT() << newValue;
        this->propertyAssignCustomSetterSetInternal(newValue);
    }

    ZFPROPERTY_ASSIGN_READONLY(zfstring, propertyAssignReadonly, ZFPropertyNoInitValue)

    ZFPROPERTY_ASSIGN_READONLY(zfstring, propertyAssignReadonlyCustomSetter, ZFPropertyNoInitValue)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfstring, propertyAssignReadonlyCustomSetter)
    {
        zfLogT() << newValue;
        this->propertyAssignReadonlyCustomSetterSetInternal(newValue);
    }

    // override
    ZFPROPERTY_RETAIN(ZFObject *, propertyOverride)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFObject *, propertyOverride)
    {
        zfLogT() << zfText("setterInBase") << newValue;
        this->propertyOverrideSetInternal(newValue);
    }
    ZFPROPERTY_CUSTOM_GETTER_DECLARE(ZFObject *, propertyOverride)
    {
        zfLogT() << zfText("getterInBase");
        return this->propertyOverrideInternal();
    }
};

zfclass _ZFP_ZFCore_ZFProperty_test_TestChild: zfextends _ZFP_ZFCore_ZFProperty_test_TestBase
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFProperty_test_TestChild, _ZFP_ZFCore_ZFProperty_test_TestBase)

public:
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, ZFObject *, propertyOverride)
    {
        zfLogT() << zfText("setterInChild") << newValue;
        zfsuper::propertyOverrideSet(newValue);
    }
    ZFPROPERTY_OVERRIDE_GETTER_DECLARE(public, ZFObject *, propertyOverride);

public:
    ZFPROPERTY_ASSIGN(zfstring, propertyInChild)
};
ZFPROPERTY_OVERRIDE_GETTER_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestChild, ZFObject *, propertyOverride)
{
    zfLogT() << zfText("getterInChild");
    return zfsuper::propertyOverride();
}

// ============================================================
zfclass ZFCore_ZFProperty_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFProperty_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        ZFLeakTestBegin();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFProperty"));
        {
            zfblockedAlloc(_ZFP_ZFCore_ZFProperty_test_TestChild, p);

            // property list
            const ZFClass *clsTmp = p->classData();
            zfLogT() << ZFDebugAutoEndlOff << zfText("list: ");
            for(zfindex i = 0; i < clsTmp->propertyCount(); ++i)
            {
                if(i > 0)
                {
                    zfLogTrimT() << ZFDebugAutoEndlOff << zfText(", ");
                }
                zfLogTrimT() << ZFDebugAutoSpaceOff << ZFDebugAutoEndlOff
                << clsTmp->propertyAtIndex(i)->propertyName();
            }
            zfLogTrimT();

            // reflect
            p->propertyAssignSet(zfText("oldValue"));
            zfLogT() << zfText("before:") << p->propertyAssign();
            p->classData()->methodForName(zfText("propertyAssignSet"))->execute<void, zfstring const &>(p, zfText("newValue"));
            zfLogT() << zfText("after:") << p->propertyAssign();
            zfLogT() << zfText("access by reflect:") << p->classData()->methodForName(zfText("propertyAssign"))->execute<zfstring const &>(p);

            // retain
            zfLogTrimT();
            zfLogTrimT() << zfText("retain");

            p->propertyRetainSet(zflineAlloc(ZFObject));
            zfLogT() << p->propertyRetain();

            p->propertyRetainCustomSetterSet(zflineAlloc(ZFObject));
            zfLogT() << p->propertyRetainCustomSetter();

#if 0 // this should not compile
            p->propertyRetainReadonlySet(zflineAlloc(ZFObject));
#endif
            zfLogT() << p->propertyRetainReadonly();

#if 0 // this should not compile
            p->propertyRetainReadonlyCustomSetterSet(zflineAlloc(ZFObject));
#endif
            zfLogT() << p->propertyRetainReadonlyCustomSetter();

            // assign
            zfLogTrimT();
            zfLogTrimT() << zfText("assign");

            p->propertyAssignSet(zfstring());
            zfLogT() << p->propertyAssign();

            p->propertyAssignCustomSetterSet(zfstring());
            zfLogT() << p->propertyAssignCustomSetter();

#if 0 // this should not compile
            p->propertyAssignReadonlySet(zfstring());
#endif
            zfLogT() << p->propertyAssignReadonly();

#if 0 // this should not compile
            p->propertyAssignReadonlyCustomSetterSet(zfstring());
#endif
            zfLogT() << p->propertyAssignReadonlyCustomSetter();

            // override
            zfLogTrimT();
            zfLogTrimT() << zfText("override property");
            p->propertyOverrideSet(zfnull);
            p->propertyOverride();

            // copy
            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("copy propertis"));
            zfblockedAlloc(_ZFP_ZFCore_ZFProperty_test_TestBase, pBase);
            zfblockedAlloc(_ZFP_ZFCore_ZFProperty_test_TestChild, pChild);
            pBase->propertyAssignSet(zfText("string set in another"));
            ZFPropertyCopyAll(pChild, pBase);
            this->testCaseOutputSeparator();
            zfLogTrimT() << zfText("after copy:") << pChild->propertyAssign();

            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("copy by ZFPropertyCopyAll"));
            pChild->propertyAssignSet(zfText(""));
            ZFPropertyCopyAll(pChild, pBase);
            zfLogTrimT() << zfText("after copy:") << pChild->propertyAssign();
        }

        ZFLeakTestPrintStatus();
        ZFLeakTestEnd();

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFProperty_test)

ZF_NAMESPACE_GLOBAL_END

