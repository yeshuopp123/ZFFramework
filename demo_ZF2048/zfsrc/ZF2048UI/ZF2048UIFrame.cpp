/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZF2048UIFrame.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZF2048UIFrame)

zfclass _ZFP_ZF2048UIBlockBackgroundView : zfextends ZFUIImageView, zfimplements ZFCacheable
{
    ZFOBJECT_DECLARE(_ZFP_ZF2048UIBlockBackgroundView, ZFUIImageView)
    ZFIMPLEMENTS_DECLARE(ZFCacheable)

    ZFCACHEABLE_DECLARE(_ZFP_ZF2048UIBlockBackgroundView)

public:
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();

        zfSkinApplyZFStyleable(this, zfText("ZF2048_block_bg"));

        return this;
    }
};
zfclassNotPOD _ZFP_ZF2048UIFramePrivate
{
public:
    ZFUIImageView *backgroundView;
    ZFArrayEditable *blocks; // ZF2048UIBlock or zfnullObject
    ZFArrayEditable *blocksHolder; // ZF2048UIBlock
    ZFArrayEditable *blockBackgrounds; // _ZFP_ZF2048UIBlockBackgroundView
    zfindex dataWidth;
    zfindex dataHeight;
    ZFUIRect cachedBlockFrame;
    zfint cachedBlockSize;

    ZFUIPoint mouseDownPoint;

public:
    zfindex posToIndex(ZF_IN zfindex posX,
                       ZF_IN zfindex posY)
    {
        return ZF2048PointToIndex(posX, posY, this->dataWidth, this->dataHeight);
    }
    ZF2048Point posFromIndex(ZF_IN zfindex index)
    {
        return ZF2048PointFromIndex(index, this->dataWidth, this->dataHeight);
    }

public:
    _ZFP_ZF2048UIFramePrivate(void)
    : backgroundView(zfnull)
    , blocks(zfnull)
    , blocksHolder(zfnull)
    , blockBackgrounds(zfnull)
    , dataWidth(0)
    , dataHeight(0)
    , cachedBlockFrame(ZFUIRectZero)
    , cachedBlockSize(0)
    , mouseDownPoint(ZFUIPointZero)
    {
    }
};
ZFCACHEABLE_DEFINE_WITH_MAX(_ZFP_ZF2048UIBlockBackgroundView, _ZFP_ZF2048UIBlockBackgroundView, 16)

ZFOBJECT_REGISTER(ZF2048UIFrame)

ZFOBSERVER_EVENT_REGISTER(ZF2048UIFrame, FrameOnMove)

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZF2048UIFrame, ZFUIImage *, frameBackgroundImage)
{
    this->frameBackgroundImageSetInternal(newValue);
    d->backgroundView->imageContentSet(this->frameBackgroundImage());
}
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZF2048UIFrame, ZFUIMargin, frameMargin)
{
    this->frameMarginSetInternal(newValue);
    this->layoutRequest();
}

void ZF2048UIFrame::update(ZF_IN const ZF2048Value *data,
                           ZF_IN zfindex dataWidth,
                           ZF_IN zfindex dataHeight)
{
    d->dataWidth = dataWidth;
    d->dataHeight = dataHeight;

    zfindex bgCount = dataWidth * dataHeight;
    while(d->blockBackgrounds->count() < bgCount)
    {
        _ZFP_ZF2048UIBlockBackgroundView *blockBg = _ZFP_ZF2048UIBlockBackgroundView::cacheAccess();
        d->blockBackgrounds->add(blockBg);
        this->childAdd(blockBg, zfnull, 0);
    }
    while(bgCount < d->blockBackgrounds->count())
    {
        _ZFP_ZF2048UIBlockBackgroundView *blockBg = d->blockBackgrounds->getLast()->toAny();
        if(blockBg != zfnull)
        {
            _ZFP_ZF2048UIBlockBackgroundView::cacheRelease(blockBg);
            blockBg->viewRemoveFromParent();
        }
        d->blockBackgrounds->removeLast();
    }

    zfindex blockCount = 0;
    while(d->blocks->count() < bgCount)
    {
        d->blocks->add(zfnullObject);
    }
    while(bgCount < d->blocks->count())
    {
        d->blocks->removeLast();
    }
    for(zfindex i = 0; i < bgCount; ++i)
    {
        if(data[i] == 0)
        {
            d->blocks->set(i, zfnullObject);
        }
        else
        {
            ZF2048UIBlock *block = zfnull;
            if(blockCount < d->blocksHolder->count())
            {
                block = d->blocksHolder->get<ZF2048UIBlock *>(blockCount);
                ++blockCount;
                this->childMove(block, zfindexMax);
            }
            else
            {
                block = ZF2048UIBlock::cacheAccess();
                ++blockCount;
                this->childAdd(block);
                d->blocksHolder->add(block);
            }
            d->blocks->set(i, block);
            block->blockValueSet(data[i]);
        }
    }
    while(blockCount < d->blocksHolder->count())
    {
        ZF2048UIBlock *block = d->blocksHolder->getLast<ZF2048UIBlock *>();
        block->viewRemoveFromParent();
        ZF2048UIBlock::cacheRelease(block);
        d->blocksHolder->removeLast();
    }
    if(this->layoutedFrame().size.width > 0 && this->layoutedFrame().size.height > 0)
    {
        this->layoutIfNeed();
    }
}
ZF2048UIBlock *ZF2048UIFrame::blockAt(ZF_IN zfindex posX, ZF_IN zfindex posY)
{
    return d->blocks->get(d->posToIndex(posX, posY))->toAny();
}
ZFUIRect ZF2048UIFrame::blockRectAt(ZF_IN zfindex posX, ZF_IN zfindex posY)
{
    return ZFUIRectMake(
        (zfint)(d->cachedBlockFrame.point.x + posX * d->cachedBlockSize),
        (zfint)(d->cachedBlockFrame.point.y + posY * d->cachedBlockSize),
        (zfint)(d->cachedBlockSize),
        (zfint)(d->cachedBlockSize));
}

ZFObject *ZF2048UIFrame::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfnew(_ZFP_ZF2048UIFramePrivate);

    d->backgroundView = zfAlloc(ZFUIImageView);
    this->internalBackgroundViewAdd(d->backgroundView);

    d->blocks = zfAlloc(ZFArrayEditable);
    d->blocksHolder = zfAlloc(ZFArrayEditable);
    d->blockBackgrounds = zfAlloc(ZFArrayEditable);

    zfSkinApplyZFStyleable(this, zfText("ZF2048_frame"));

    return this;
}
void ZF2048UIFrame::objectOnDealloc(void)
{
    zfRelease(d->backgroundView);
    zfRelease(d->blocks);
    zfRelease(d->blocksHolder);
    zfRelease(d->blockBackgrounds);

    zfdelete(d);
    zfsuper::objectOnDealloc();
}

void ZF2048UIFrame::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayoutPrepare(bounds);
    if(d->dataWidth <= 0 || d->dataHeight <= 0)
    {
        return ;
    }

    ZFUIRect availableFrame = ZFUIRectApplyMargin(bounds, this->frameMargin());
    availableFrame.size.width = zfmMax(availableFrame.size.width, 0);
    availableFrame.size.height = zfmMax(availableFrame.size.height, 0);
    if(availableFrame.size.width * d->dataHeight >= d->dataWidth * availableFrame.size.height)
    {
        d->cachedBlockSize = availableFrame.size.height / d->dataHeight;
    }
    else
    {
        d->cachedBlockSize = availableFrame.size.width / d->dataWidth;
    }
    d->cachedBlockFrame = ZFUIAlignApply(ZFUIAlign::e_Center, availableFrame,
        ZFUISizeMake((zfint)(d->cachedBlockSize * d->dataWidth), (zfint)(d->cachedBlockSize * d->dataHeight)));
}
void ZF2048UIFrame::internalBackgroundViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalBackgroundViewOnLayout(bounds);
    d->backgroundView->layout(ZFUIRectApplyMarginReversely(d->cachedBlockFrame, this->frameMargin()));
}
void ZF2048UIFrame::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayout(bounds);

    for(zfindex i = 0; i < d->blocks->count(); ++i)
    {
        ZF2048Point pos = d->posFromIndex(i);
        ZFUIRect blockFrame = this->blockRectAt(pos.x, pos.y);

        _ZFP_ZF2048UIBlockBackgroundView *blockBg = d->blockBackgrounds->get(i)->toAny();
        blockBg->layout(blockFrame);

        ZF2048UIBlock *block = d->blocks->get(i)->toAny();
        if(block != zfnull)
        {
            block->layout(blockFrame);
        }
    }
}

void ZF2048UIFrame::viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent)
{
    zfsuper::viewEventOnMouseEvent(mouseEvent);
    if(mouseEvent->mouseAction == ZFUIMouseAction::e_MouseDown)
    {
        d->mouseDownPoint = mouseEvent->mousePoint;
        if(this->viewFocusable() && !this->viewFocused())
        {
            this->viewFocusRequest(zftrue);
        }
        return ;
    }
    else if(mouseEvent->mouseAction != ZFUIMouseAction::e_MouseUp)
    {
        return ;
    }

    const zfint tolerance = 30;
    if(zfmAbs(mouseEvent->mousePoint.x - d->mouseDownPoint.x) >= zfmAbs(mouseEvent->mousePoint.y - d->mouseDownPoint.y))
    {
        if(d->mouseDownPoint.x - mouseEvent->mousePoint.x >= tolerance)
        {
            this->frameOnMove(ZFUIOrientation::e_Left);
        }
        else if(mouseEvent->mousePoint.x - d->mouseDownPoint.x >= tolerance)
        {
            this->frameOnMove(ZFUIOrientation::e_Right);
        }
    }
    else
    {
        if(d->mouseDownPoint.y - mouseEvent->mousePoint.y >= tolerance)
        {
            this->frameOnMove(ZFUIOrientation::e_Top);
        }
        else if(mouseEvent->mousePoint.y - d->mouseDownPoint.y >= tolerance)
        {
            this->frameOnMove(ZFUIOrientation::e_Bottom);
        }
    }
}
void ZF2048UIFrame::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
{
    switch(keyEvent->keyCode)
    {
        case ZFUIKeyCode::e_kLeft:
            if(keyEvent->keyAction == ZFUIKeyAction::e_KeyDown)
            {
                this->frameOnMove(ZFUIOrientation::e_Left);
            }
            keyEvent->eventResolvedSet(zftrue);
            break;
        case ZFUIKeyCode::e_kUp:
            if(keyEvent->keyAction == ZFUIKeyAction::e_KeyDown)
            {
                this->frameOnMove(ZFUIOrientation::e_Top);
            }
            keyEvent->eventResolvedSet(zftrue);
            break;
        case ZFUIKeyCode::e_kRight:
            if(keyEvent->keyAction == ZFUIKeyAction::e_KeyDown)
            {
                this->frameOnMove(ZFUIOrientation::e_Right);
            }
            keyEvent->eventResolvedSet(zftrue);
            break;
        case ZFUIKeyCode::e_kDown:
            if(keyEvent->keyAction == ZFUIKeyAction::e_KeyDown)
            {
                this->frameOnMove(ZFUIOrientation::e_Bottom);
            }
            keyEvent->eventResolvedSet(zftrue);
            break;
        default:
            zfsuper::viewEventOnKeyEvent(keyEvent);
            break;
    }
}

ZF_GLOBAL_INITIALIZER_INIT(ZF2048UIFrame_ignoreFrom_ZFUIViewBlinkWhenFocus)
{
    ZFUIViewBlinkWhenFocusFilter.classFilter.filterAdd(ZF2048UIFrame::ClassData());
}
ZF_GLOBAL_INITIALIZER_END(ZF2048UIFrame_ignoreFrom_ZFUIViewBlinkWhenFocus)

ZF_NAMESPACE_GLOBAL_END

