/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTokenDef.h
 * @brief string token util
 */

#ifndef _ZFI_ZFTokenDef_h_
#define _ZFI_ZFTokenDef_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util class to hold string tokens
 */
zfclassLikePOD ZF_ENV_EXPORT ZFTokenForContainer
{
public:
    zfstring tokenLeft; /**< @brief "[" by default */
    zfstring tokenRight; /**< @brief "]" by default */
    zfstring tokenSeparator; /**< @brief ", " by default */
    zfstring tokenValueLeft; /**< @brief "" by default */
    zfstring tokenValueRight; /**< @brief "" by default */
    zfstring tokenEtc; /**< @brief "..." by default */

public:
    /** @cond ZFPrivateDoc */
    ZFTokenForContainer(void)
    : tokenLeft(zfText("["))
    , tokenRight(zfText("]"))
    , tokenSeparator(zfText(", "))
    , tokenValueLeft(zfText(""))
    , tokenValueRight(zfText(""))
    , tokenEtc(zfText("..."))
    {
    }
    ZFTokenForContainer(
          ZF_IN zfstring const &tokenLeft
        , ZF_IN zfstring const &tokenRight
        , ZF_IN zfstring const &tokenSeparator
        , ZF_IN zfstring const &tokenValueLeft
        , ZF_IN zfstring const &tokenValueRight
        , ZF_IN zfstring const &tokenEtc
        )
    : tokenLeft(tokenLeft)
    , tokenRight(tokenRight)
    , tokenSeparator(tokenSeparator)
    , tokenValueLeft(tokenValueLeft)
    , tokenValueRight(tokenValueRight)
    , tokenEtc(tokenEtc)
    {
    }
    /** @endcond */
};
/** @brief see #ZFTokenForContainer */
extern ZF_ENV_EXPORT const ZFTokenForContainer ZFTokenForContainerTrim;
/** @brief see #ZFTokenForContainer */
extern ZF_ENV_EXPORT const ZFTokenForContainer ZFTokenForContainerDetail;
/** @brief see #ZFTokenForContainer */
#define ZFTokenForContainerDefault ZFTokenForContainerTrim

// ============================================================
/**
 * @brief util class to hold string tokens
 */
zfclassLikePOD ZF_ENV_EXPORT ZFTokenForKeyValueContainer
{
public:
    zfstring tokenLeft; /**< @brief "{" by default */
    zfstring tokenRight; /**< @brief "}" by default */
    zfstring tokenSeparator; /**< @brief ", " by default */
    zfstring tokenPairLeft; /**< @brief "" by default */
    zfstring tokenPairRight; /**< @brief "" by default */
    zfstring tokenPairSeparator; /**< @brief " = " by default */
    zfstring tokenKeyLeft; /**< @brief "" by default */
    zfstring tokenKeyRight; /**< @brief "" by default */
    zfstring tokenValueLeft; /**< @brief "" by default */
    zfstring tokenValueRight; /**< @brief "" by default */
    zfstring tokenEtc; /**< @brief "..." by default */

public:
    /** @cond ZFPrivateDoc */
    ZFTokenForKeyValueContainer(void)
    : tokenLeft(zfText("{"))
    , tokenRight(zfText("}"))
    , tokenSeparator(zfText(", "))
    , tokenPairLeft(zfText(""))
    , tokenPairRight(zfText(""))
    , tokenPairSeparator(zfText(" = "))
    , tokenKeyLeft(zfText(""))
    , tokenKeyRight(zfText(""))
    , tokenValueLeft(zfText(""))
    , tokenValueRight(zfText(""))
    , tokenEtc(zfText("..."))
    {
    }
    ZFTokenForKeyValueContainer(
          ZF_IN zfstring const &tokenLeft
        , ZF_IN zfstring const &tokenRight
        , ZF_IN zfstring const &tokenSeparator
        , ZF_IN zfstring const &tokenPairLeft
        , ZF_IN zfstring const &tokenPairRight
        , ZF_IN zfstring const &tokenPairSeparator
        , ZF_IN zfstring const &tokenKeyLeft
        , ZF_IN zfstring const &tokenKeyRight
        , ZF_IN zfstring const &tokenValueLeft
        , ZF_IN zfstring const &tokenValueRight
        , ZF_IN zfstring const &tokenEtc
        )
    : tokenLeft(tokenLeft)
    , tokenRight(tokenRight)
    , tokenSeparator(tokenSeparator)
    , tokenPairLeft(tokenPairLeft)
    , tokenPairRight(tokenPairRight)
    , tokenPairSeparator(tokenPairSeparator)
    , tokenKeyLeft(tokenKeyLeft)
    , tokenKeyRight(tokenKeyRight)
    , tokenValueLeft(tokenValueLeft)
    , tokenValueRight(tokenValueRight)
    , tokenEtc(tokenEtc)
    {
    }
    /** @endcond */
};
/** @brief see #ZFTokenForKeyValueContainer */
extern ZF_ENV_EXPORT const ZFTokenForKeyValueContainer ZFTokenForKeyValueContainerTrim;
/** @brief see #ZFTokenForKeyValueContainer */
extern ZF_ENV_EXPORT const ZFTokenForKeyValueContainer ZFTokenForKeyValueContainerDetail;
/** @brief see #ZFTokenForKeyValueContainer */
#define ZFTokenForKeyValueContainerDefault ZFTokenForKeyValueContainerTrim

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFTokenDef_h_

