/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFSerializableDataIO_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSERIALIZABLEDATAIO_DEFINE(ZFSerializableDataIOTypeId_zfsd, {
        return ZFSerializableDataFromInput(serializableData, input, outErrorHint);
    }, {
        return ZFSerializableDataToOutput(output, serializableData, outErrorHint);
    })

ZF_NAMESPACE_GLOBAL_END

