/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIPageRequest.h
 * @brief abstract page manager
 */

#ifndef _ZFI_ZFUIPageRequest_h_
#define _ZFI_ZFUIPageRequest_h_

#include "ZFUIWidgetDef.h"
#include "ZFUIPage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief base class of request for #ZFUIPageManager,
 *   you may subclass this and achieve custom request
 */
zfclass ZF_ENV_EXPORT ZFUIPageRequest : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIPageRequest, ZFObject)

public:
    /**
     * @brief whether the request has been resolved
     */
    ZFPROPERTY_ASSIGN(zfbool, requestResolved)
};

/**
 * @brief param for #ZFUIPageManager::requestPageCreate
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFUIPageRequestPageCreateParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFUIPageRequestPageCreateParam)

public:
    /**
     * @brief page's class, must be type of #ZFUIPage
     */
    ZFCORE_PARAM(const ZFClass *, pageClass)

    /**
     * @brief page create param which would be passed to #ZFUIPage::pageCreateParam,
     *   auto retained, null by default
     */
    ZFCORE_PARAM_RETAIN(ZFObject *, pageCreateParam)

    /**
     * @brief whether resume page automatically after page created, true by default
     *
     * if true, #ZFUIPageManager::requestPageResume would be called automatically after page created
     */
    ZFCORE_PARAM_WITH_INIT(zfbool, pageAutoResume, zftrue)
};
/**
 * @brief request for #ZFUIPageManager::requestPageCreate
 */
zfclass ZFUIPageRequestPageCreate : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(ZFUIPageRequestPageCreate, ZFUIPageRequest)

public:
    /** @brief page create param */
    ZFUIPageRequestPageCreateParam createParam;
};

/**
 * @brief request for #ZFUIPageManager::requestPageResume
 */
zfclass ZFUIPageRequestPageResume : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(ZFUIPageRequestPageResume, ZFUIPageRequest)

public:
    /** @brief the page to resume */
    ZFPROPERTY_RETAIN(ZFUIPage *, page)
};

/**
 * @brief request for #ZFUIPageManager::requestPageGroupResume
 */
zfclass ZFUIPageRequestPageGroupResume : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(ZFUIPageRequestPageGroupResume, ZFUIPageRequest)

public:
    /** @brief page group id to resume */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfstring, pageGroupId, ZFPropertyInitValue(ZFUIPageGroupIdInvalid))
};

/**
 * @brief request for #ZFUIPageManager::requestPageDestroy
 */
zfclass ZFUIPageRequestPageDestroy : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(ZFUIPageRequestPageDestroy, ZFUIPageRequest)

public:
    /** @brief the page to destroy */
    ZFPROPERTY_RETAIN(ZFUIPage *, page)
};

/**
 * @brief request for #ZFUIPageManager::requestPostCustom
 */
zfclass ZFUIPageRequestCustom : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(ZFUIPageRequestCustom, ZFUIPageRequest)

public:
    /** @brief the listener to execute */
    ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE(ZFListener, listener)
    /** @brief the param passed to the listener */
    ZFPROPERTY_RETAIN(ZFObject *, userData)
    /** @brief the param passed to the listener */
    ZFPROPERTY_RETAIN(ZFObject *, param0)
    /** @brief the param passed to the listener */
    ZFPROPERTY_RETAIN(ZFObject *, param1)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPageRequest_h_

