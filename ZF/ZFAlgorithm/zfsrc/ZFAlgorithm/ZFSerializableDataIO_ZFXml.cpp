/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFSerializableDataIO_ZFXml.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSERIALIZABLEDATAIO_DEFINE(ZFSerializableDataIOTypeId_xml, {
        if(!ZFXmlParse(serializableData, input))
        {
            zfstringAppend(outErrorHint, zfText("unable to parse xml"));
            return zffalse;
        }
        else
        {
            return zftrue;
        }
    }, {
        ZFXmlPrint(serializableData, output);
        return zftrue;
    })

ZF_NAMESPACE_GLOBAL_END

