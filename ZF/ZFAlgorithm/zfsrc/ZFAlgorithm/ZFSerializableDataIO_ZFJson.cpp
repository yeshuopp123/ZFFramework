/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFSerializableDataIO_ZFJson.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSERIALIZABLEDATAIO_DEFINE(ZFSerializableDataIOTypeId_json, {
        if(!ZFJsonParse(serializableData, input))
        {
            zfstringAppend(outErrorHint, zfText("unable to parse json"));
            return zffalse;
        }
        else
        {
            return zftrue;
        }
    }, {
        ZFJsonPrint(serializableData, output);
        return zftrue;
    })

ZF_NAMESPACE_GLOBAL_END

