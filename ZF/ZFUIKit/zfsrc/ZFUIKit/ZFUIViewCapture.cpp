/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIViewCapture.h"
#include "protocol/ZFProtocolZFUIViewCapture.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfautoObject ZFUIViewCapture(ZF_IN ZFUIView *view)
{
    if(view == zfnull)
    {
        return zfautoObjectNull;
    }
    zfautoObject ret = ZFUIImage::ClassData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    if(!ZFPROTOCOL_ACCESS(ZFUIViewCapture)->viewCapture(view, ret.to<ZFUIImage *>()))
    {
        return zfautoObjectNull;
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

