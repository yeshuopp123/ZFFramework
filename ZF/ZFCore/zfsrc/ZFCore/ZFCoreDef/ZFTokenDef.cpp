/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFTokenDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFTokenForContainer ZFTokenForContainerTrim;
const ZFTokenForContainer ZFTokenForContainerDetail(
          zfText("[\n    ") // tokenLeft
        , zfText("\n]")     // tokenRight
        , zfText(",\n    ") // tokenSeparator
        , zfText("")        // tokenValueLeft
        , zfText("")        // tokenValueRight
        , zfText("...")     // tokenEtc
    );

const ZFTokenForKeyValueContainer ZFTokenForKeyValueContainerTrim;
const ZFTokenForKeyValueContainer ZFTokenForKeyValueContainerDetail(
          zfText("{\n    ") // tokenLeft
        , zfText("\n}")     // tokenRight
        , zfText(",\n    ") // tokenSeparator
        , zfText("")        // tokenPairLeft
        , zfText("")        // tokenPairRight
        , zfText(" = ")     // tokenPairSeparator
        , zfText("")        // tokenKeyLeft
        , zfText("")        // tokenKeyRight
        , zfText("")        // tokenValueLeft
        , zfText("")        // tokenValueRight
        , zfText("...")     // tokenEtc
    );

ZF_NAMESPACE_GLOBAL_END

