/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIListView.h
 * @brief list view
 */

#ifndef _ZFI_ZFUIListView_h_
#define _ZFI_ZFUIListView_h_

#include "ZFUIWidgetDef.h"
#include "ZFUIListAdapter.h"
#include "ZFUIListCellUpdater.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIListView
zfclassFwd _ZFP_ZFUIListViewPrivate;
/**
 * @brief linear container view that layout children in linear by specified orientation
 *
 * a list layout is a scroll view, you may manage the scroll logic by #ZFUIScrollView's methods,
 * while scroll view's content size would be updated automatically
 * and you should not modify it manually
 * @warning you must not add or remove normal child to list view,
 *   only internal background or foreground view is allowed
 *
 * to use a list view, you must supply a list adapter which would supply all of list cells,
 * see #ZFUIListAdapter for more info\n
 * list view itself won't supplly any separator or margins or border radius,
 * you must supply by the list adapter,
 * see #ZFUIListCell for more info
 */
zfclass ZF_ENV_EXPORT ZFUIListView : zfextends ZFUIScrollView
{
    ZFOBJECT_DECLARE(ZFUIListView, ZFUIScrollView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIListView)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when list cell attached to this list view,
     * param0 is the list cell
     * @note you must not access #listVisibleCell or #listVisibleCellIndexRange
     *   during this event,
     *   you can only access them during #EventListVisibleCellOnChange
     */
    ZFOBSERVER_EVENT(ListCellOnAttach)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when list cell detached from this list view,
     * param0 is the list cell
     * @note you must not access #listVisibleCell or #listVisibleCellIndexRange
     *   during this event,
     *   you can only access them during #EventListVisibleCellOnChange
     */
    ZFOBSERVER_EVENT(ListCellOnDetach)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when list visible cell changed
     */
    ZFOBSERVER_EVENT(ListVisibleCellOnChange)

public:
    // ============================================================
    // properties
    /**
     * @brief list adapter, no auto retain
     */
    ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE(ZFUIListAdapter *, listAdapter)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUIListAdapter *, listAdapter);
    /**
     * @brief set and retain the list adapter
     *
     * by default, list adapter won't be retained by this list view to prevent recursive retain,
     * you may use this method to retain it automatically
     */
    virtual void listAdapterSetAutoRetain(ZF_IN ZFUIListAdapter *listAdapter);

    /**
     * @brief list updater to update list cells, holds #ZFUIListCellUpdater
     */
    ZFPROPERTY_RETAIN_READONLY(ZFArrayEditable *, cellUpdater, ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFArrayEditable)))

    /**
     * @brief direction to layout children, #ZFUIOrientation::e_Top by default
     *
     * #ZFUIOrientation::e_Left means layout children from left to right,
     * while #ZFUIOrientation::e_Bottom means layout children from bottom to top
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIOrientationEnum, listOrientation,
                                ZFPropertyInitValue(ZFUIOrientation::e_Top))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUIOrientationEnum, listOrientation);

    /**
     * @brief list cell's hint size which would be passed to #ZFUIListAdapter,
     *   use -1 to measure by list cell,
     *   #ZFUIGlobalStyle::itemSizeListCell by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, cellSizeHint,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->itemSizeListCell()))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfint, cellSizeHint);

    /**
     * @brief whether auto update #ZFUIScrollView::scrollBounceVertical series accorrding to #listOrientation,
     *   true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, listBounce, ZFPropertyInitValue(zftrue))
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(zfbool, listBounce);

    // ============================================================
    // override
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    /**
     * @brief see #ZFSerializable::serializableOnCheckPropertyType
     *
     * we will ignore:
     * -  #ZFUIScrollView::scrollContentFrame
     * -  #ZFUIScrollView::scrollBounceVertical
     * -  #ZFUIScrollView::scrollBounceHorizontal
     * -  #ZFUIScrollView::scrollBounceVerticalAlways
     * -  #ZFUIScrollView::scrollBounceHorizontalAlways
     */
    zfoverride
    virtual ZFSerializable::PropertyType serializableOnCheckPropertyType(ZF_IN const ZFProperty *property);
    /**
     * @brief all child views would be ignored from serializable
     */
    zfoverride
    virtual zfbool serializableOnCheckNeedSerializeChildren(void)
    {
        return zffalse;
    }

protected:
    zfoverride
    virtual void layoutOnLayoutRequest(ZF_IN ZFUIView *causedByThisView);
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);

    zfoverride
    virtual void viewChildOnAdd(ZF_IN ZFUIView *child,
                                ZF_IN ZFUIViewChildLayerEnum layer);
    zfoverride
    virtual void viewChildOnRemove(ZF_IN ZFUIView *child,
                                   ZF_IN ZFUIViewChildLayerEnum layer);

    zfoverride
    virtual void scrollAreaMarginOnChange(void);
    zfoverride
    virtual void scrollContentFrameOnChange(void);

protected:
    zfoverride
    virtual void scrollOnScrolledByUser(void);
    zfoverride
    virtual void scrollOnScrollEnd(void);

    // ============================================================
    // list control
public:
    /**
     * @brief reload entire list cells
     *
     * @note this method would only schedule an update step,
     * when it would be reloaded is not ensured\n
     * @note this method would reload the entire list,
     *   use #listReloadCellAtIndex for performance
     */
    virtual void listReload(void);
    /**
     * @brief true if list need reload
     */
    virtual zfbool listReloadRequested(void);
    /**
     * @brief reload cell at index immediately, do nothing if index not in visible range
     *   or #listReloadRequested
     *
     * this method would have higher performance if you want to update specified cell only
     */
    virtual void listReloadCellAtIndex(ZF_IN zfindex index);

public:
    /**
     * @brief return a list of current visible cells, valid only if #listReloadRequested is not true
     *
     * returned value should not be stored,
     * since visible cell may change frequently
     */
    virtual ZFCoreArrayPOD<ZFUIListCell *> listVisibleCell(void);
    /**
     * @brief return first visible cell's index, valid only if #listReloadRequested is not true
     *
     * index is ordered by #ZFUIListAdapter,
     * so first cell may positioned at bottom accorrding to layout orientation
     */
    virtual const zfindexRange &listVisibleCellIndexRange(void);

public:
    /**
     * @brief scroll cell to top, do nothing if invalid or no need to scroll
     *
     * if animated, scroll task would be canceled if scrolled by user (#ZFUIScrollView::scrollOnScrolledByUser)
     * @note since list cell may load dynamically,
     *   this method would recursively scroll until reached desired position,
     *   you should prevent doing other load logic during scroll events,
     *   otherwise, dead loop may occurred
     */
    virtual void scrollListCellToHead(ZF_IN zfindex cellIndex,
                                      ZF_IN_OPT zfint offset = 0,
                                      ZF_IN_OPT zfbool animated = zftrue);
    /**
     * @brief scroll cell to bottom, see #scrollListCellToHead
     */
    virtual void scrollListCellToTail(ZF_IN zfindex cellIndex,
                                      ZF_IN_OPT zfint offset = 0,
                                      ZF_IN_OPT zfbool animated = zftrue);

    // ============================================================
    // events
protected:
    /** @brief see #EventListCellOnAttach */
    virtual inline void cellOnAttach(ZF_IN ZFUIListCell *cell)
    {
        this->observerNotify(ZFUIListView::EventListCellOnAttach(), cell);
    }
    /** @brief see #EventListCellOnDetach */
    virtual inline void cellOnDetach(ZF_IN ZFUIListCell *cell)
    {
        this->observerNotify(ZFUIListView::EventListCellOnDetach(), cell);
    }
    /** @brief see #EventListVisibleCellOnChange */
    virtual void listVisibleCellOnChange(void);

private:
    _ZFP_ZFUIListViewPrivate *d;
    friend zfclassFwd _ZFP_ZFUIListViewPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListView_h_

