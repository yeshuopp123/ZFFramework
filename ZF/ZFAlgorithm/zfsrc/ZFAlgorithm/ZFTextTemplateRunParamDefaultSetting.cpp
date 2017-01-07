/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFTextTemplateRun.h"

#include "ZFRegExp.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFTextTemplateRunParamDefault_DECLARE(typeName) \
    static ZFRegExp *_ZFP_ZFTextTemplateRunParamDefault_##typeName##FilterRule = zfnull; \
    static ZFFilterCallbackResult _ZFP_ZFTextTemplateRunParamDefault_##typeName##Filter(ZF_IN const zfchar *const &value) \
    { \
        if(_ZFP_ZFTextTemplateRunParamDefault_##typeName##FilterRule != zfnull) \
        { \
            ZFRegExpResult regExpResult; \
            _ZFP_ZFTextTemplateRunParamDefault_##typeName##FilterRule->regExpMatch(regExpResult, value); \
            if(regExpResult.matched) \
            { \
                return ZFFilterCallbackResultNotActive; \
            } \
        } \
        return ZFFilterCallbackResultNotSpecified; \
    }

_ZFP_ZFTextTemplateRunParamDefault_DECLARE(folderName)
_ZFP_ZFTextTemplateRunParamDefault_DECLARE(folderContent)
_ZFP_ZFTextTemplateRunParamDefault_DECLARE(fileName)
_ZFP_ZFTextTemplateRunParamDefault_DECLARE(fileContent)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTextTemplateRunParamDefaultSetting, ZFLevelZFFrameworkNormal)
{
    #if 0
    _ZFP_ZFTextTemplateRunParamDefault_folderNameFilterRule = zfAlloc(ZFRegExp);
    ZFTextTemplateRunParamDefault.folderNameFilter.customFilterCallbackAdd(_ZFP_ZFTextTemplateRunParamDefault_folderNameFilter);
    _ZFP_ZFTextTemplateRunParamDefault_folderNameFilterRule->regExpCompile(zfText(
            ""
        ));
    #endif

    _ZFP_ZFTextTemplateRunParamDefault_folderContentFilterRule = zfAlloc(ZFRegExp);
    ZFTextTemplateRunParamDefault.folderContentFilter.customFilterCallbackAdd(_ZFP_ZFTextTemplateRunParamDefault_folderContentFilter);
    _ZFP_ZFTextTemplateRunParamDefault_folderContentFilterRule->regExpCompile(zfText(
            "(^\\.git$)"
            "|(^\\.svn$)"
            "|(^\\.hg$)"
        ));

    #if 0
    _ZFP_ZFTextTemplateRunParamDefault_fileNameFilterRule = zfAlloc(ZFRegExp);
    ZFTextTemplateRunParamDefault.fileNameFilter.customFilterCallbackAdd(_ZFP_ZFTextTemplateRunParamDefault_fileNameFilter);
    _ZFP_ZFTextTemplateRunParamDefault_fileNameFilterRule->regExpCompile(zfText(
            ""
        ));
    #endif

    _ZFP_ZFTextTemplateRunParamDefault_fileContentFilterRule = zfAlloc(ZFRegExp);
    ZFTextTemplateRunParamDefault.fileContentFilter.customFilterCallbackAdd(_ZFP_ZFTextTemplateRunParamDefault_fileContentFilter);
    _ZFP_ZFTextTemplateRunParamDefault_fileContentFilterRule->regExpCompile(zfText(
            "(\\.ico$)"
            "|(\\.icns$)"

            "|(\\.png$)"
            "|(\\.jpg$)"
            "|(\\.jpeg$)"
            "|(\\.gif$)"

            "|(\\.mp2$)"
            "|(\\.mp3$)"
            "|(\\.mp4$)"
            "|(\\.wav$)"
            "|(\\.ogg$)"

            "|(\\.db$)"
            "|(\\.rar$)"
            "|(\\.zip$)"
            "|(\\.7z$)"

            "|(\\.a(\\..*)*$)"
            "|(\\.o(\\..*)*$)"
            "|(\\.so(\\..*)*$)"
            "|(\\.dll(\\..*)*$)"
            "|(\\.dylib(\\..*)*$)"
        ));
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFTextTemplateRunParamDefaultSetting)
{
    ZFTextTemplateRunParamDefault.folderNameFilter.customFilterCallbackRemove(_ZFP_ZFTextTemplateRunParamDefault_folderNameFilter);
    zfRelease(_ZFP_ZFTextTemplateRunParamDefault_folderNameFilterRule);
    _ZFP_ZFTextTemplateRunParamDefault_folderNameFilterRule = zfnull;

    ZFTextTemplateRunParamDefault.folderContentFilter.customFilterCallbackRemove(_ZFP_ZFTextTemplateRunParamDefault_folderContentFilter);
    zfRelease(_ZFP_ZFTextTemplateRunParamDefault_folderContentFilterRule);
    _ZFP_ZFTextTemplateRunParamDefault_folderContentFilterRule = zfnull;

    ZFTextTemplateRunParamDefault.fileNameFilter.customFilterCallbackRemove(_ZFP_ZFTextTemplateRunParamDefault_fileNameFilter);
    zfRelease(_ZFP_ZFTextTemplateRunParamDefault_fileNameFilterRule);
    _ZFP_ZFTextTemplateRunParamDefault_fileNameFilterRule = zfnull;

    ZFTextTemplateRunParamDefault.fileContentFilter.customFilterCallbackRemove(_ZFP_ZFTextTemplateRunParamDefault_fileContentFilter);
    zfRelease(_ZFP_ZFTextTemplateRunParamDefault_fileContentFilterRule);
    _ZFP_ZFTextTemplateRunParamDefault_fileContentFilterRule = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFTextTemplateRunParamDefaultSetting)

ZF_NAMESPACE_GLOBAL_END

