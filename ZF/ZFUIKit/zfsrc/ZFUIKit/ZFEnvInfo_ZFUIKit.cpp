/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFEnvInfo_ZFUIKit.h"
#include "protocol/ZFProtocolZFEnvInfo_ZFUIKit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFEnvDeviceUIType)

ZF_NAMESPACE_BEGIN(ZFEnvInfo)

// ============================================================
ZFEnvDeviceUITypeEnum deviceUIType(ZF_IN_OPT ZFEnvDeviceUITypeEnum defaultValue /* = ZFEnvDeviceUIType::e_Desktop */)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_deviceUIInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_deviceUIInfo);
    if(impl != zfnull)
    {
        return impl->deviceUIType();
    }
    else
    {
        return defaultValue;
    }
}
ZF_NAMESPACE_END(ZFEnvInfo)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFEnvSummary_ZFUIKit, ZFLevelZFFrameworkNormal)
{
    ZFEnvInfo::envSummaryCallbackRegister(zfText("deviceUIType"), zfself::deviceUITypeSummary);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFEnvSummary_ZFUIKit)
{
    ZFEnvInfo::envSummaryCallbackUnregister(zfText("deviceUIType"));
}
public:
    static void deviceUITypeSummary(ZF_IN_OUT zfstring &ret)
    {
        ret += ZFEnvDeviceUIType::EnumNameForValue(ZFEnvInfo::deviceUIType());
    }
ZF_GLOBAL_INITIALIZER_END(ZFEnvSummary_ZFUIKit)

ZF_NAMESPACE_GLOBAL_END

