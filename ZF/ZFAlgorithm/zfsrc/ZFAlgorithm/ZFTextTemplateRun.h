/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTextTemplateRun.h
 * @brief text template util
 */

#ifndef _ZFI_ZFTextTemplateRun_h_
#define _ZFI_ZFTextTemplateRun_h_

#include "ZFTextTemplate.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief param for #ZFTextTemplateRun
 *
 * while running the #ZFTextTemplateRun,
 * these things can be applied with #ZFTextTemplateApply:
 * -  folder name
 * -  folder content, i.e. all the files and sub folders within it
 * -  file name
 * -  file content
 *
 * if folder or file name matches,
 * they would be renamed\n
 * if folder or file content matches,
 * the content would be applied with #ZFTextTemplateApply\n
 * \n
 * the filter param can be used to filter out something you don't want to be
 * applied with #ZFTextTemplateApply\n
 * \n
 * by default, all setting would be copied from #ZFTextTemplateRunParamDefault
 */
zfclassLikePOD ZF_ENV_EXPORT ZFTextTemplateRunParam
{
public:
    ZFFilterForString folderNameFilter; /**< @brief see #ZFTextTemplateRunParam */
    ZFFilterForString folderContentFilter; /**< @brief see #ZFTextTemplateRunParam */
    ZFFilterForString fileNameFilter; /**< @brief see #ZFTextTemplateRunParam */
    ZFFilterForString fileContentFilter; /**< @brief see #ZFTextTemplateRunParam */

public:
    /**
     * @brief copy from another param
     */
    void copyFrom(ZF_IN ZFTextTemplateRunParam &ref)
    {
        this->folderNameFilter.copyFrom(ref.folderNameFilter);
        this->folderContentFilter.copyFrom(ref.folderContentFilter);
        this->fileNameFilter.copyFrom(ref.fileNameFilter);
        this->fileContentFilter.copyFrom(ref.fileContentFilter);
    }
};

extern ZF_ENV_EXPORT ZFTextTemplateRunParam &_ZFP_ZFTextTemplateRunParamDefault(void);
/**
 * @brief default param for #ZFTextTemplateRun
 *
 * this setting change be changed at runtime,
 * however, it would affect the entire runtime,
 * change with caution
 * @note this setting would be cleared during #ZFFrameworkCleanup as #ZFLevelZFFrameworkNormal
 */
#define ZFTextTemplateRunParamDefault _ZFP_ZFTextTemplateRunParamDefault()

/**
 * @brief util method to apply #ZFTextTemplateApply to a given path recursively
 */
extern ZF_ENV_EXPORT zfbool ZFTextTemplateRun(ZF_IN const zfchar *path,
                                              ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                              ZF_IN_OPT const ZFTextTemplateRunParam &runParam = ZFTextTemplateRunParamDefault,
                                              ZF_OUT_OPT zfstring *errorMessage = zfnull);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTextTemplateRun_h_

