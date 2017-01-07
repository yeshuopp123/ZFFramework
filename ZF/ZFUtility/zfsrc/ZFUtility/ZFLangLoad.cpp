/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFLangLoad.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfbool zfLangLoad(ZF_IN const ZFSerializableData &serializableData,
                  ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &item = serializableData.elementAtIndex(i);
        if(zfscmpTheSame(item.itemClass(), zfText("l")))
        {
            const zfchar *k = item.attributeForName(zfText("k"));
            const zfchar *v = item.attributeForName(zfText("v"));
            if(k != zfnull && v != zfnull)
            {
                zfLangSet(k, v);
            }
        }
    }

    zfLangNotifyChanged();
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

