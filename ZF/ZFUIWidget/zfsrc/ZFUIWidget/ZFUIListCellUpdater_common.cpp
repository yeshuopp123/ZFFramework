/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIListCellUpdater_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIListCellUpdaterBasic)

#define _ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator zfText("_ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator")
#define _ZFP_ZFUIListCellUpdaterBasic_tag_separator zfText("_ZFP_ZFUIListCellUpdaterBasic_tag_separator")
#define _ZFP_ZFUIListCellUpdaterBasic_tag_separator_head zfText("_ZFP_ZFUIListCellUpdaterBasic_tag_separator_head")
static ZFUIView *_ZFP_ZFUIListCellUpdaterBasic_setupSeparator(ZF_IN ZFUIListCellUpdaterBasic *owner,
                                                              ZF_IN const ZFUIListCellUpdaterParam &updateParam)
{
    zfautoObject separatorViewHolder = owner->itemCacheAccess(_ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator);
    ZFUIView *separatorView = separatorViewHolder.to<ZFUIView *>();
    if(separatorView == zfnull)
    {
        separatorView = zfAlloc(ZFUIView);
        separatorViewHolder = zfautoObjectCreate(separatorView);
        zfRelease(separatorView);

        separatorView->layoutParamSet(zflineAllocWithoutLeakTest(ZFUIViewLayoutParam));
        separatorView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);

        separatorView->viewUIEnableTreeSet(zffalse);
    }

    updateParam.cell->internalForegroundViewAdd(separatorView);
    separatorView->viewBackgroundColorSet(owner->separatorColor());
    switch(updateParam.listOrientation)
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            separatorView->layoutParam()->sizeHintSet(ZFUISizeMake(owner->separatorSize(), -1));
            separatorView->layoutParam()->layoutMarginSet(ZFUIMarginMake(0, owner->separatorMargin().top, 0, owner->separatorMargin().bottom));
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            separatorView->layoutParam()->sizeHintSet(ZFUISizeMake(-1, owner->separatorSize()));
            separatorView->layoutParam()->layoutMarginSet(ZFUIMarginMake(owner->separatorMargin().left, 0, owner->separatorMargin().right, 0));
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
    return separatorView;
}
void ZFUIListCellUpdaterBasic::cellOnUpdate(ZF_IN const ZFUIListCellUpdaterParam &updateParam)
{
    zfsuperI(ZFUIListCellUpdater)::cellOnUpdate(updateParam);

    if(updateParam.cellIndex == 0)
    { // first cell
        if(this->separatorIncludingHead())
        {
            ZFUIView *separatorView = _ZFP_ZFUIListCellUpdaterBasic_setupSeparator(this, updateParam);
            updateParam.cell->tagSet(_ZFP_ZFUIListCellUpdaterBasic_tag_separator_head, separatorView);
            switch(updateParam.listOrientation)
            {
                case ZFUIOrientation::e_Left:
                    separatorView->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner);
                    break;
                case ZFUIOrientation::e_Right:
                    separatorView->layoutParam()->layoutAlignSet(ZFUIAlign::e_RightInner);
                    break;
                case ZFUIOrientation::e_Top:
                    separatorView->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner);
                    break;
                case ZFUIOrientation::e_Bottom:
                    separatorView->layoutParam()->layoutAlignSet(ZFUIAlign::e_BottomInner);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
        }
    }
    if(updateParam.cellIndex == updateParam.cellCount - 1)
    { // last cell
        if(!this->separatorIncludingTail())
        {
            return ;
        }
    }

    ZFUIView *separatorView = _ZFP_ZFUIListCellUpdaterBasic_setupSeparator(this, updateParam);
    updateParam.cell->tagSet(_ZFP_ZFUIListCellUpdaterBasic_tag_separator, separatorView);
    switch(updateParam.listOrientation)
    {
        case ZFUIOrientation::e_Left:
            separatorView->layoutParam()->layoutAlignSet(ZFUIAlign::e_RightInner);
            break;
        case ZFUIOrientation::e_Right:
            separatorView->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner);
            break;
        case ZFUIOrientation::e_Top:
            separatorView->layoutParam()->layoutAlignSet(ZFUIAlign::e_BottomInner);
            break;
        case ZFUIOrientation::e_Bottom:
            separatorView->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
void ZFUIListCellUpdaterBasic::cellOnRecycle(ZF_IN ZFUIListCell *cell)
{
    zfsuperI(ZFUIListCellUpdater)::cellOnRecycle(cell);

    ZFObject *separatorHead = cell->tagGet(_ZFP_ZFUIListCellUpdaterBasic_tag_separator_head);
    if(separatorHead != zfnull)
    {
        this->itemCacheRecycle(_ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator, separatorHead);
        cell->internalForegroundViewRemove(separatorHead->to<ZFUIView *>());
        cell->tagRemove(_ZFP_ZFUIListCellUpdaterBasic_tag_separator_head);
    }
    ZFObject *separatorTail = cell->tagGet(_ZFP_ZFUIListCellUpdaterBasic_tag_separator);
    if(separatorTail != zfnull)
    {
        this->itemCacheRecycle(_ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator, separatorTail);
        cell->internalForegroundViewRemove(separatorTail->to<ZFUIView *>());
        cell->tagRemove(_ZFP_ZFUIListCellUpdaterBasic_tag_separator);
    }
}

ZF_NAMESPACE_GLOBAL_END

