/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIViewPrintViewTree.h"

#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static void _ZFP_ZFUIViewPrintViewTreeDelayedAction(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    ZFListenerHolder *data = ZFCastZFObjectUnchecked(ZFListenerHolder *, userData);
    ZFUIViewPrintViewTree(ZFCastZFObjectUnchecked(ZFUIView *, data->listenerData.param0), data->runnable);
}
void ZFUIViewPrintViewTreeDelayed(ZF_IN ZFUIView *view,
                                  ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */)
{
    ZFThreadTaskRequest(
        ZFCallbackForRawFunction(_ZFP_ZFUIViewPrintViewTreeDelayedAction),
        zflineAllocWithoutLeakTest(ZFListenerHolder, outputCallback, ZFListenerData(zfidentityInvalid, zfnull, view)));
}

void ZFUIViewPrintViewTreeDelayed(ZF_IN zftimet delay,
                                  ZF_IN ZFUIView *view,
                                  ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */)
{
    ZFThreadExecuteInMainThreadAfterDelay(
        delay,
        ZFCallbackForRawFunction(_ZFP_ZFUIViewPrintViewTreeDelayedAction),
        zflineAllocWithoutLeakTest(ZFListenerHolder, outputCallback, ZFListenerData(zfidentityInvalid, zfnull, view)));
}

// ============================================================
zfclassPOD _ZFP_ZFUIViewPrintViewTreeData
{
public:
    const ZFClass *viewClass;
    ZFUIViewPrintViewTreeInfoGetter viewInfoGetter;
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewPrintViewTreeDataHolder, ZFLevelZFFrameworkNormal)
{
}
public:
    // ordered by:
    // ZFUIView, classBaseA, classChildA, classBaseB, classChildB
    ZFCoreArrayPOD<_ZFP_ZFUIViewPrintViewTreeData> datas;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewPrintViewTreeDataHolder)

zfclassPOD _ZFP_ZFUIViewPrintViewTreePrintData
{
public:
    ZFUIView *view;
    zfindex depth;
    zfindex siblingIndex;
    ZFUIViewChildLayerEnum layer;
};
static ZFObject *_ZFP_ZFUIViewPrintViewTreeSyncObject = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewPrintViewTreeSyncObjectHolder, ZFLevelZFFrameworkNormal)
{
    _ZFP_ZFUIViewPrintViewTreeSyncObject = zfAllocWithoutLeakTest(ZFObject);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewPrintViewTreeSyncObjectHolder)
{
    zfReleaseWithoutLeakTest(_ZFP_ZFUIViewPrintViewTreeSyncObject);
    _ZFP_ZFUIViewPrintViewTreeSyncObject = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewPrintViewTreeSyncObjectHolder);
void ZFUIViewPrintViewTree(ZF_IN ZFUIView *view,
                           ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */)
{
    if(view == zfnull || !outputCallback.callbackIsValid())
    {
        return ;
    }

    zfsynchronizedObject(_ZFP_ZFUIViewPrintViewTreeSyncObject);

    ZFCoreArrayPOD<_ZFP_ZFUIViewPrintViewTreePrintData> printDatas;
    _ZFP_ZFUIViewPrintViewTreePrintData rootPrintData;
    rootPrintData.view = view;
    rootPrintData.depth = 0;
    rootPrintData.siblingIndex = 0;
    rootPrintData.layer = ZFUIViewChildLayer::e_Normal;
    printDatas.add(rootPrintData);

    outputCallback.execute(zfText("====================== view tree begin =====================\n"));
    do
    {
        _ZFP_ZFUIViewPrintViewTreePrintData printData = printDatas.getLast();
        printDatas.removeLast();

        // ignore delegate view
        if(printData.view->viewDelegateForParent())
        {
            continue;
        }

        // all children
        ZFCoreArrayPOD<ZFUIView *> implViews = printData.view->internalImplViewArray();
        ZFCoreArrayPOD<ZFUIView *> bgViews = printData.view->internalBackgroundViewArray();
        ZFCoreArrayPOD<ZFUIView *> normalViews = printData.view->childArray();
        ZFCoreArrayPOD<ZFUIView *> fgViews = printData.view->internalForegroundViewArray();
        for(zfindex i = fgViews.count() - 1; i != zfindexMax; --i)
        {
            _ZFP_ZFUIViewPrintViewTreePrintData printDataTmp;
            printDataTmp.view = fgViews.get(i)->to<ZFUIView *>();
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_Foreground;
            printDatas.add(printDataTmp);
        }
        for(zfindex i = normalViews.count() - 1; i != zfindexMax; --i)
        {
            _ZFP_ZFUIViewPrintViewTreePrintData printDataTmp;
            printDataTmp.view = normalViews.get(i)->to<ZFUIView *>();
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_Normal;
            printDatas.add(printDataTmp);
        }
        for(zfindex i = bgViews.count() - 1; i != zfindexMax; --i)
        {
            _ZFP_ZFUIViewPrintViewTreePrintData printDataTmp;
            printDataTmp.view = bgViews.get(i)->to<ZFUIView *>();
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_Background;
            printDatas.add(printDataTmp);
        }
        for(zfindex i = implViews.count() - 1; i != zfindexMax; --i)
        {
            _ZFP_ZFUIViewPrintViewTreePrintData printDataTmp;
            printDataTmp.view = implViews.get(i)->to<ZFUIView *>();
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_Impl;
            printDatas.add(printDataTmp);
        }

        outputCallback.execute(zfText("|"));
        outputCallback.execute(zfstringWithFormat(zfText("%2zi"), printData.siblingIndex).cString());
        for(zfindex i = printData.depth - 1; i != zfindexMax; --i)
        {
            outputCallback.execute(zfText(" |"));
        }

        switch(printData.layer)
        {
            case ZFUIViewChildLayer::e_Normal:
                outputCallback.execute(zfText(" "));
                break;
            case ZFUIViewChildLayer::e_Impl:
                outputCallback.execute(zfText(" impl "));
                break;
            case ZFUIViewChildLayer::e_Background:
                outputCallback.execute(zfText(" bg "));
                break;
            case ZFUIViewChildLayer::e_Foreground:
                outputCallback.execute(zfText(" fg "));
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }

        ZFCoreArrayPOD<_ZFP_ZFUIViewPrintViewTreeData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewPrintViewTreeDataHolder)->datas;
        zfbool exist = zffalse;
        for(zfindex i = datas.count() - 1; i != zfindexMax; --i)
        {
            if(printData.view->classData()->classIsSubclassOf(datas[i].viewClass))
            {
                datas[i].viewInfoGetter(printData.view, outputCallback);
                exist = zftrue;
                break;
            }
        }
        if(!exist)
        {
            zfstring tmp;
            printData.view->objectInfoT(tmp);
            zfindex startFix = 0;
            zfindex countFix = 0;
            zfindex tokenLeftLen = zfslen(ZFTOKEN_ZFObjectInfoLeft);
            zfindex tokenRightLen = zfslen(ZFTOKEN_ZFObjectInfoRight);
            if(tmp.length() >= tokenLeftLen
                && zfsncmp(tmp.cString(), ZFTOKEN_ZFObjectInfoLeft, tokenLeftLen) == 0)
            {
                startFix += tokenLeftLen;
                countFix += tokenLeftLen;
            }
            if(tmp.length() >= tokenRightLen
                && zfsncmp(tmp.cString() - tokenRightLen, ZFTOKEN_ZFObjectInfoRight, tokenRightLen) == 0)
            {
                countFix += tokenRightLen;
            }
            outputCallback.execute(tmp.cString() + startFix, tmp.length() - countFix);
        }
        outputCallback.execute(zfText("\n"));
    } while(!printDatas.isEmpty());
    outputCallback.execute(zfText("====================== view tree  end  =====================\n"));
}

void ZFUIViewPrintViewTreeInfoGetterSet(ZF_IN const ZFClass *viewClass,
                                        ZF_IN ZFUIViewPrintViewTreeInfoGetter viewInfoGetter)
{
    zfCoreAssert(viewClass != zfnull && viewClass->classIsSubclassOf(ZFUIView::ClassData()));
    ZFCoreArrayPOD<_ZFP_ZFUIViewPrintViewTreeData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewPrintViewTreeDataHolder)->datas;
    if(viewInfoGetter == zfnull)
    {
        for(zfindex i = 0; i < datas.count(); ++i)
        {
            const ZFClass *cls = datas[i].viewClass;
            if(viewClass == cls)
            {
                datas.remove(i);
                break;
            }
        }
        return ;
    }

    zfindex indexAddTo = zfindexMax;
    for(zfindex i = 0; i < datas.count(); ++i)
    {
        const ZFClass *cls = datas[i].viewClass;
        if(viewClass == cls)
        {
            datas[i].viewInfoGetter = viewInfoGetter;
            break;
        }
        else if(cls->classIsSubclassOf(viewClass))
        {
            indexAddTo = i;
            break;
        }
        else if(viewClass->classIsSubclassOf(cls))
        {
            indexAddTo = i + 1;
        }
    }

    _ZFP_ZFUIViewPrintViewTreeData data;
    data.viewClass = viewClass;
    data.viewInfoGetter = viewInfoGetter;
    if(indexAddTo == zfindexMax)
    {
        datas.add(data);
    }
    else
    {
        datas.add(indexAddTo, data);
    }
}
ZFUIViewPrintViewTreeInfoGetter ZFUIViewPrintViewTreeInfoGetterGet(ZF_IN const ZFClass *viewClass)
{
    ZFCoreArrayPOD<_ZFP_ZFUIViewPrintViewTreeData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewPrintViewTreeDataHolder)->datas;
    for(zfindex i = datas.count() - 1; i != zfindexMax; --i)
    {
        if(datas[i].viewClass == viewClass)
        {
            return datas[i].viewInfoGetter;
        }
    }
    return zfnull;
}

ZF_NAMESPACE_GLOBAL_END

