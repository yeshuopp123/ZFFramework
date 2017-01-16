/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFLangUtil_ZFSerializableHelper.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(ZFSerializableDataRefType_lang_zfstring)
{
    serializableData.propertyValueSet(zfLang(refData));
    return zftrue;
}

ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(ZFSerializableDataRefType_lang_ZFString)
{
    serializableData.propertyValueSet(zfLang(refData));
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

