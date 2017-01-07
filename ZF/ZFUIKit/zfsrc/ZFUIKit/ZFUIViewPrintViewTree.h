/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIViewPrintViewTree.h
 * @brief print view tree for ZFUIView
 */

#ifndef _ZFI_ZFUIViewPrintViewTree_h_
#define _ZFI_ZFUIViewPrintViewTree_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief debug use only, to output human readable view tree info
 */
extern ZF_ENV_EXPORT void ZFUIViewPrintViewTree(ZF_IN ZFUIView *view,
                                                ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault);

/**
 * @brief delay to #ZFUIViewPrintViewTree using #ZFThreadTaskRequest
 */
extern ZF_ENV_EXPORT void ZFUIViewPrintViewTreeDelayed(ZF_IN ZFUIView *view,
                                                       ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault);
/**
 * @brief delay to #ZFUIViewPrintViewTree using #ZFThreadExecuteInMainThreadAfterDelay
 */
extern ZF_ENV_EXPORT void ZFUIViewPrintViewTreeDelayed(ZF_IN zftimet delay,
                                                       ZF_IN ZFUIView *view,
                                                       ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault);

/**
 * @brief info getter for #ZFUIViewPrintViewTree, see #ZFUIViewPrintViewTreeInfoGetterSet
 */
typedef void (*ZFUIViewPrintViewTreeInfoGetter)(ZF_IN ZFUIView *view,
                                                ZF_IN_OUT const ZFOutputCallback &outputCallback);
/**
 * @brief register a custom info getter for output info of a view type using #ZFUIViewPrintViewTree
 *
 * output should contain only infomations without extra new line or tokens,
 * newly set would override old one, set null to remove\n
 * you may add your own at runtime, using #ZF_GLOBAL_INITIALIZER_INIT is recommended\n
 * if a view class not registered, only #ZFObject::objectInfoOnAppend would be outputed
 * \n
 * if both base class and child class registered,
 * child class's one would have higher priority,
 * without parent's one being called
 */
extern ZF_ENV_EXPORT void ZFUIViewPrintViewTreeInfoGetterSet(ZF_IN const ZFClass *viewClass,
                                                             ZF_IN ZFUIViewPrintViewTreeInfoGetter viewInfoGetter);
/**
 * @brief see #ZFUIViewPrintViewTreeInfoGetterSet
 */
extern ZF_ENV_EXPORT ZFUIViewPrintViewTreeInfoGetter ZFUIViewPrintViewTreeInfoGetterGet(ZF_IN const ZFClass *viewClass);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewPrintViewTree_h_

