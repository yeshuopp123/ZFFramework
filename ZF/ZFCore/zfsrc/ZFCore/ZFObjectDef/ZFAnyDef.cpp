/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFAnyDef.h"
#include "ZFObjectDef.h"

#if ZF_ENV_DEBUG
    #include "ZFObjectSmartPointerDef.h"
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

#if ZF_ENV_DEBUG
zfclass _ZFP_ZFAnyTestParent : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFAnyTestParent, ZFObject)
};
zfclass _ZFP_ZFAnyTestChild : zfextends _ZFP_ZFAnyTestParent
{
    ZFOBJECT_DECLARE(_ZFP_ZFAnyTestChild, _ZFP_ZFAnyTestParent)
};

static void _ZFP_ZFAnyTest(void)
{
    zfblockedAlloc(_ZFP_ZFAnyTestChild, objChild);
    _ZFP_ZFAnyTestParent *objCenter = objChild;
    ZFAnyT<_ZFP_ZFAnyTestChild *> anyChild(objChild);
    ZFAnyT<_ZFP_ZFAnyTestParent *> anyCenter(objChild);
    ZFAny any(objChild);

    // ============================================================
    // ZFCastZFObject
    // any
    objChild = any;
    any = objChild;
    any = anyCenter;
    anyChild = any;

    objChild = ZFCastZFObject(_ZFP_ZFAnyTestChild *, any);
    any = ZFCastZFObject(ZFAny, objCenter);
    any = ZFCastZFObject(ZFAny, any);
    any = ZFCastZFObject(ZFAny, anyCenter);
    anyChild = ZFCastZFObject(ZFAnyT<_ZFP_ZFAnyTestChild *>, any);

    // anyT
    objChild = anyCenter;
//    anyChild = objCenter; // this should compile error
//    anyChild = anyCenter; // this should compile error

    objChild = ZFCastZFObject(_ZFP_ZFAnyTestChild *, anyCenter);
//    anyChild = ZFCastZFObject(ZFAnyT<_ZFP_ZFAnyTestChild *>, objCenter); // this should compile error
//    anyChild = ZFCastZFObject(ZFAnyT<_ZFP_ZFAnyTestChild *>, anyCenter); // this should compile error

    // ============================================================
    // ZFCastZFObjectUnchecked
    // any
    objChild = any;
    any = objChild;
    any = anyCenter;
    anyChild = any;

    objChild = ZFCastZFObjectUnchecked(_ZFP_ZFAnyTestChild *, any);
    any = ZFCastZFObjectUnchecked(ZFAny, objCenter);
    any = ZFCastZFObjectUnchecked(ZFAny, any);
    any = ZFCastZFObjectUnchecked(ZFAny, anyCenter);
    anyChild = ZFCastZFObjectUnchecked(ZFAnyT<_ZFP_ZFAnyTestChild *>, any);

    // anyT
    objChild = anyCenter;
//    anyChild = objCenter; // this should compile error
//    anyChild = anyCenter; // this should compile error

    objChild = ZFCastZFObjectUnchecked(_ZFP_ZFAnyTestChild *, anyCenter);
//    anyChild = ZFCastZFObjectUnchecked(ZFAnyT<_ZFP_ZFAnyTestChild *>, objCenter); // this should compile error
//    anyChild = ZFCastZFObjectUnchecked(ZFAnyT<_ZFP_ZFAnyTestChild *>, anyCenter); // this should compile error

    ZFUNUSED(objChild);
    ZFUNUSED(objCenter);
    ZFUNUSED(anyChild);
    ZFUNUSED(anyCenter);
    ZFUNUSED(any);
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAnyTest, ZFLevelAppLow)
{
    _ZFP_ZFAnyTest();
}
ZF_GLOBAL_INITIALIZER_END(ZFAnyTest)

#endif // #if ZF_ENV_DEBUG

ZF_NAMESPACE_GLOBAL_END

