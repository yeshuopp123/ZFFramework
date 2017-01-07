/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreStatisticDef.h"
#include "ZFCoreStringUtilDef.h"
#include "ZFCoreMapDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

static ZFCoreMap &_ZFP_ZFCoreStatisticDataMap(void)
{
    static ZFCoreMap d;
    return d;
}

void invokeCountLog(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticDataMap();
    zfindex *count = m.get<zfindex *>(key);
    if(count != zfnull)
    {
        ++(*count);
    }
    else
    {
        m.set(key, ZFCorePointerForObject<zfindex *>(zfnew(zfindex, 1)));
    }
}
void invokeCountRemove(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    _ZFP_ZFCoreStatisticDataMap().remove(key);
}
void invokeCountRemoveAll(void)
{
    _ZFP_ZFCoreStatisticDataMap().removeAll();
}
zfindex invokeCountGet(ZF_IN_OPT const zfchar *key /* = zfnull */)
{
    zfindex *count = _ZFP_ZFCoreStatisticDataMap().get<zfindex *>(key);
    return ((count != zfnull) ? *count : 0);
}

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END

