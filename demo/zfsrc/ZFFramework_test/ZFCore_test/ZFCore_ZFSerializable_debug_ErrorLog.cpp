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

#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFCore_ZFSerializable_debug_ErrorLog)
{
    ZFCALLBACK_LOCAL_BEGIN_2(zfindex, tmp, const void *, param0, zfindex, param1)
    {
        zfLogTrimT() << zfstring((const zfchar *)param0, param1);
        return zfindexMax;
    }
    ZFCALLBACK_LOCAL_END(tmp)
    this->output = tmp;

    ZFSerializableUtilErrorOutputCallbacks.add(this->output);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFCore_ZFSerializable_debug_ErrorLog)
{
    ZFSerializableUtilErrorOutputCallbacks.removeElement(this->output);
}
private:
    ZFOutputCallback output;
ZF_GLOBAL_INITIALIZER_END(ZFCore_ZFSerializable_debug_ErrorLog)
#endif

ZF_NAMESPACE_GLOBAL_END

