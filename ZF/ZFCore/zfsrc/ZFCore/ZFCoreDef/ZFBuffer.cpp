/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFBuffer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfbool ZFBuffer::bufferMalloc(ZF_IN zfindex bufferSize)
{
    this->bufferFree();
    d->buffer = zfmalloc(bufferSize);
    if(d->buffer != zfnull)
    {
        zfmemset(d->buffer, 0, bufferSize);
        d->bufferSize = bufferSize;
        d->bufferAutoFree = zftrue;
        return zftrue;
    }
    else
    {
        d->bufferSize = 0;
        d->bufferAutoFree = zffalse;
        return zffalse;
    }
}
zfbool ZFBuffer::bufferRealloc(ZF_IN zfindex bufferSize)
{
    void *bufferOld = d->buffer;

    if(d->bufferAutoFree)
    {
        d->buffer = zfrealloc(d->buffer, bufferSize);
        if(d->buffer == zfnull)
        {
            d->buffer = bufferOld;
            return zffalse;
        }
        if(bufferSize > d->bufferSize)
        {
            zfmemset((zfbyte *)d->buffer + d->bufferSize, 0, bufferSize - d->bufferSize);
        }
    }
    else
    {
        d->buffer = zfmalloc(bufferSize);
        if(d->buffer == zfnull)
        {
            d->buffer = bufferOld;
            return zffalse;
        }
        if(bufferSize > d->bufferSize)
        {
            zfmemcpy(d->buffer, bufferOld, d->bufferSize);
            zfmemset((zfbyte *)d->buffer + d->bufferSize, 0, bufferSize - d->bufferSize);
        }
        else
        {
            zfmemcpy(d->buffer, bufferOld, bufferSize);
        }
    }

    d->bufferSize = bufferSize;
    d->bufferAutoFree = zftrue;
    return zftrue;
}
void ZFBuffer::bufferFree(void)
{
    if(d->buffer != zfnull && d->bufferAutoFree)
    {
        zffree(d->buffer);
        d->buffer = zfnull;
        d->bufferSize = 0;
        d->bufferAutoFree = zffalse;
    }
}

ZF_NAMESPACE_GLOBAL_END

