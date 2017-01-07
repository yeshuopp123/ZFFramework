/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIScrollThumb.h"
#include "ZFUIScrollThumbDefault.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIScrollThumb)

ZFOBSERVER_EVENT_REGISTER(ZFUIScrollThumb, ScrollThumbOnUpdate)

void ZFUIScrollThumb::scrollThumbPosCalc(ZF_OUT zfint &scrollThumbResultPos,
                                         ZF_OUT zfint &scrollThumbResultSize,
                                         ZF_IN zfint scrollViewSize,
                                         ZF_IN zfint scrollContentOffset,
                                         ZF_IN zfint scrollContentSize,
                                         ZF_IN zfint scrollThumbMinSize,
                                         ZF_IN_OPT zfint headMargin /* = 0 */,
                                         ZF_IN_OPT zfint tailMargin /* = 0 */)
{
    if(scrollViewSize <= 0
        || scrollContentSize <= 0)
    {
        scrollThumbResultSize = 0;
        return ;
    }

    zfint pos = 0;
    zfint size = 0;
    zfint fixedContentSize = zfmMax(scrollViewSize, scrollContentSize);
    zfint virtualContentSize = scrollViewSize - headMargin - tailMargin;

    if(scrollContentOffset > 0)
    { // bounce at head
        pos = 0;
        size = scrollViewSize - scrollContentOffset;
    }
    else if(scrollViewSize >= scrollContentSize && scrollContentOffset < 0)
    { // bounce at tail
        pos = -scrollContentOffset;
        size = scrollViewSize - pos;
    }
    else if(scrollContentOffset + fixedContentSize < scrollViewSize)
    { // bounce at tail
        pos = -scrollContentOffset;
        size = scrollContentOffset + fixedContentSize;
    }
    else
    { // no bounce
        pos = -scrollContentOffset;
        size = scrollViewSize;
    }

    scrollThumbResultPos = headMargin + pos * virtualContentSize / fixedContentSize;
    scrollThumbResultSize = size * virtualContentSize / fixedContentSize;

    if(scrollThumbResultSize < scrollThumbMinSize)
    {
        scrollThumbResultPos -= (scrollThumbMinSize - scrollThumbResultSize) / 2;
        scrollThumbResultSize = scrollThumbMinSize;
    }

    if(scrollThumbResultSize >= virtualContentSize - 2)
    {
        scrollThumbResultSize = virtualContentSize;
    }

    if(scrollThumbResultPos + scrollThumbResultSize + tailMargin > scrollViewSize)
    {
        scrollThumbResultPos = scrollViewSize - scrollThumbResultSize - tailMargin;
    }
    if(scrollThumbResultPos < headMargin)
    {
        scrollThumbResultPos = headMargin;
    }
}

static const ZFClass *_ZFP_ZFUIScrollThumbHorizontal_cls = zfnull;
static const ZFClass *_ZFP_ZFUIScrollThumbVertical_cls = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIScrollThumbRegister, ZFLevelZFFrameworkNormal)
{
    _ZFP_ZFUIScrollThumbHorizontal_cls = ZFUIScrollThumbDefault::ClassData();
    _ZFP_ZFUIScrollThumbVertical_cls = ZFUIScrollThumbDefault::ClassData();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIScrollThumbRegister)

void ZFUIScrollThumbHorizontalClassSet(ZF_IN const ZFClass *cls)
{
    if(cls != zfnull && !cls->classIsTypeOf(ZFUIScrollThumb::ClassData()))
    {
        zfCoreLog(zfTextA("class %s not type of %s"),
            zfsCoreZ2A(cls->className()),
            zfsCoreZ2A(ZFUIScrollThumb::ClassData()->className()));
        return ;
    }

    if(cls == zfnull)
    {
        cls = ZFUIScrollThumbDefault::ClassData();
    }
    _ZFP_ZFUIScrollThumbHorizontal_cls = cls;
}
const ZFClass *ZFUIScrollThumbHorizontalClass(void)
{
    return _ZFP_ZFUIScrollThumbHorizontal_cls;
}

void ZFUIScrollThumbVerticalClassSet(ZF_IN const ZFClass *cls)
{
    if(cls != zfnull && !cls->classIsTypeOf(ZFUIScrollThumb::ClassData()))
    {
        zfCoreLog(zfTextA("class %s not type of %s"),
            zfsCoreZ2A(cls->className()),
            zfsCoreZ2A(ZFUIScrollThumb::ClassData()->className()));
        return ;
    }

    if(cls == zfnull)
    {
        cls = ZFUIScrollThumbDefault::ClassData();
    }
    _ZFP_ZFUIScrollThumbVertical_cls = cls;
}
const ZFClass *ZFUIScrollThumbVerticalClass(void)
{
    return _ZFP_ZFUIScrollThumbVertical_cls;
}

ZF_NAMESPACE_GLOBAL_END

