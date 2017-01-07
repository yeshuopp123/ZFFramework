/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPropertyFwdDef.h"
#include "ZFObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFPropertyCallbackIsValueAccessedChange(ZF_IN const ZFProperty *property,
                                             ZF_IN ZFPropertyCallbackIsValueAccessed callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackIsValueAccessed = callback;
}
void ZFPropertyCallbackIsInitValueChange(ZF_IN const ZFProperty *property,
                                         ZF_IN ZFPropertyCallbackIsInitValue callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackIsInitValue = callback;
}
void ZFPropertyCallbackResetInitValueChange(ZF_IN const ZFProperty *property,
                                            ZF_IN ZFPropertyCallbackResetInitValue callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackResetInitValue = callback;
}
void ZFPropertyCallbackCompareChange(ZF_IN const ZFProperty *property,
                                     ZF_IN ZFPropertyCallbackCompare callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackCompare = callback;
}
void ZFPropertyCallbackCopyChange(ZF_IN const ZFProperty *property,
                                  ZF_IN ZFPropertyCallbackCopy callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackCopy = callback;
}
void ZFPropertyCallbackRetainSetChange(ZF_IN const ZFProperty *property,
                                       ZF_IN ZFPropertyCallbackRetainSet callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackRetainSet = callback;
}
void ZFPropertyCallbackRetainGetChange(ZF_IN const ZFProperty *property,
                                       ZF_IN ZFPropertyCallbackRetainGet callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackRetainGet = callback;
}
void ZFPropertyCallbackAssignSetChange(ZF_IN const ZFProperty *property,
                                       ZF_IN ZFPropertyCallbackAssignSet callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackAssignSet = callback;
}
void ZFPropertyCallbackAssignGetChange(ZF_IN const ZFProperty *property,
                                       ZF_IN ZFPropertyCallbackAssignGet callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackAssignGet = callback;
}
void ZFPropertyCallbackGetInfoChange(ZF_IN const ZFProperty *property,
                                     ZF_IN ZFPropertyCallbackGetInfo callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackGetInfo = callback;
}

ZF_NAMESPACE_GLOBAL_END

