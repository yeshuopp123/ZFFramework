/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFFileCallbackDef.h
 * @brief callback for file
 */

#ifndef _ZFI_ZFFileCallbackDef_h_
#define _ZFI_ZFFileCallbackDef_h_

#include "ZFFile.h"
#include "ZFFileBOMDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// keys
/**
 * @brief for io callback only, the file path of the target
 *
 * stored as #ZFString within #ZFCallback::callbackTagSet
 */
#define ZFCallbackTagKeyword_filePath zfText("ZFCallbackTagKeyword_filePath")
/**
 * @brief for io callback only, the resource file path of the target
 *
 * stored as #ZFString within #ZFCallback::callbackTagSet
 */
#define ZFCallbackTagKeyword_resPath zfText("ZFCallbackTagKeyword_resPath")

/**
 * @brief used to impl #ZFOutputCallbackForLocalFile/#ZFInputCallbackForLocalFile
 *
 * stored as #ZFString within #ZFSerializableData::serializableDataTagSet\n
 * holds the parent serializable data's logic file path,
 * while creating #ZFOutputCallbackForLocalFile/#ZFInputCallbackForLocalFile,
 * we will try to find the proper parent path recursively from child to parent,
 * and concatenate to make the final file path
 */
#define ZFSerializableDataTagKeyword_filePath zfText("ZFSerializableDataTagKeyword_filePath")
/**
 * @brief see #ZFSerializableDataTagKeyword_filePath
 */
#define ZFSerializableDataTagKeyword_resPath zfText("ZFSerializableDataTagKeyword_resPath")

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFFileCallback_filePath zfText("filePath")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFFileCallback_flags zfText("flags")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFFileCallback_autoFlushSize zfText("autoFlushSize")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFFileCallback_autoSkipBOMTable zfText("autoSkipBOMTable")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFFileCallback_localPath zfText("localPath")

// ============================================================
// ZFOutputCallbackForFile
extern ZF_ENV_EXPORT ZFOutputCallback _ZFP_ZFOutputCallbackForFile(ZF_IN const zfcharA *callerFile,
                                                                   ZF_IN const zfcharA *callerFunction,
                                                                   ZF_IN zfindex callerLine,
                                                                   ZF_IN const zfchar *filePath,
                                                                   ZF_IN_OPT ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Create,
                                                                   ZF_IN_OPT zfindex autoFlushSize = zfindexMax);
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <zfstring category="filePath" ... />
 *       <ZFFileOpenOptionFlags category="flags" ... /> // optional, ZFFileOpenOption::e_Create by default
 *       <zfindex category="autoFlushSize" ... /> // optional, zfindexMax by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputCallbackForFile ZFOutputCallbackForFile
/** @brief see #ZFCallbackSerializeCustomTypeRegister */
#define ZFCallbackSerializeCustomTypeName_ZFOutputCallbackForFile ZFM_TOSTRING(ZFCallbackSerializeCustomType_ZFOutputCallbackForFile)

/**
 * @brief util to create a file output callback
 *
 * param:
 * -  (const zfchar *)filePath: file path to use
 * -  (ZFFileOpenOption)flags: flags to open file
 * -  (zfindex)autoFlushSize: ensure to flush file after how much size written,
 *   use zfindexMax to disable or 0 to flush every time
 *
 * auto open and auto close files, may return a null callback if open file error
 */
#define ZFOutputCallbackForFile(filePath, ...) \
    _ZFP_ZFOutputCallbackForFile(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, filePath, ##__VA_ARGS__)

// ============================================================
// ZFInputCallbackForFile
extern ZF_ENV_EXPORT ZFInputCallback _ZFP_ZFInputCallbackForFile(ZF_IN const zfcharA *callerFile,
                                                                 ZF_IN const zfcharA *callerFunction,
                                                                 ZF_IN zfindex callerLine,
                                                                 ZF_IN const zfchar *filePath,
                                                                 ZF_IN_OPT ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read,
                                                                 ZF_IN_OPT const ZFFileBOM *autoSkipBOMTable = &ZFFileBOMUTF8,
                                                                 ZF_IN_OPT zfindex autoSkipBOMTableCount = 1);
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <zfstring category="filePath" ... />
 *       <ZFFileOpenOptionFlags category="flags" ... /> // optional, ZFFileOpenOption::e_Read by default
 *       <zfstring category="autoSkipBOMTable" ... /> // optional, ZFFileBOMUTF8 by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFInputCallbackForFile ZFInputCallbackForFile
/** @brief see #ZFCallbackSerializeCustomTypeRegister */
#define ZFCallbackSerializeCustomTypeName_ZFInputCallbackForFile ZFM_TOSTRING(ZFCallbackSerializeCustomType_ZFInputCallbackForFile)

/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const zfchar *)filePath: file path to use
 * -  (ZFFileOpenOption)flags: flags to open file
 * -  (const ZFFileBOM *)autoSkipBOMTable: BOM to skip,
 *   if not null, BOM would be discarded and BOM's size would be ignored while calculating the file's size
 * -  (zfindex)autoSkipBOMTableCount autoSkipBOMTable's count
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * auto setup callback cache id with res file path
 */
#define ZFInputCallbackForFile(filePath, ...) \
    _ZFP_ZFInputCallbackForFile(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, filePath, ##__VA_ARGS__)

// ============================================================
// ZFInputCallbackForResFile
extern ZF_ENV_EXPORT ZFInputCallback _ZFP_ZFInputCallbackForResFile(ZF_IN const zfcharA *callerFile,
                                                                    ZF_IN const zfcharA *callerFunction,
                                                                    ZF_IN zfindex callerLine,
                                                                    ZF_IN const zfchar *resFilePath,
                                                                    ZF_IN_OPT const ZFFileBOM *autoSkipBOMTable = &ZFFileBOMUTF8,
                                                                    ZF_IN_OPT zfindex autoSkipBOMTableCount = 1);
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <zfstring category="filePath" ... />
 *       <ZFFileOpenOptionFlags category="flags" ... /> // optional, ZFFileOpenOption::e_Read by default
 *       <zfstring category="autoSkipBOMTable" ... /> // optional, ZFFileBOMUTF8 by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFInputCallbackForResFile ZFInputCallbackForResFile
/** @brief see #ZFCallbackSerializeCustomTypeRegister */
#define ZFCallbackSerializeCustomTypeName_ZFInputCallbackForResFile ZFM_TOSTRING(ZFCallbackSerializeCustomType_ZFInputCallbackForResFile)

/**
 * @brief util to create a resource file input callback,
 *   see #ZFFile::resOpen for what resource file is
 *
 * param:
 * -  (const zfchar *)resFilePath: resource file path to use
 * -  (const ZFFileBOM *)autoSkipBOMTable: BOM to skip,
 *   if not null, BOM would be discarded and BOM's size would be ignored while calculating the file's size
 * -  (zfindex)autoSkipBOMTableCount autoSkipBOMTable's count
 *
 * auto open and auto close files, may return a null callback if open file error
 */
#define ZFInputCallbackForResFile(resFilePath, ...) \
    _ZFP_ZFInputCallbackForResFile(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, resFilePath, ##__VA_ARGS__)

// ============================================================
// ZFOutputCallbackForLocalFile
extern ZF_ENV_EXPORT ZFOutputCallback _ZFP_ZFOutputCallbackForLocalFile(ZF_IN const zfcharA *callerFile,
                                                                        ZF_IN const zfcharA *callerFunction,
                                                                        ZF_IN zfindex callerLine,
                                                                        ZF_IN const ZFSerializableData &dataToCheckParentPath,
                                                                        ZF_IN const zfchar *localPath,
                                                                        ZF_IN_OPT ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Create,
                                                                        ZF_IN_OPT zfindex autoFlushSize = zfindexMax);
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <zfstring category="localPath" ... />
 *       <ZFFileOpenOptionFlags category="flags" ... /> // optional, ZFFileOpenOption::e_Create by default
 *       <zfindex category="autoFlushSize" ... /> // optional, zfindexMax by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputCallbackForLocalFile ZFOutputCallbackForLocalFile
/** @brief see #ZFCallbackSerializeCustomTypeRegister */
#define ZFCallbackSerializeCustomTypeName_ZFOutputCallbackForLocalFile ZFM_TOSTRING(ZFCallbackSerializeCustomType_ZFOutputCallbackForLocalFile)

/**
 * @brief util to create a file output callback
 *
 * param:
 * -  (const ZFSerializableData &)dataToCheckParentPath: see #ZFSerializableDataTagKeyword_filePath
 * -  (const zfchar *)localPath: local file path to use
 * -  (ZFFileOpenOption)flags: flags to open file
 * -  (zfindex)autoFlushSize: ensure to flush file after how much size written,
 *   use zfindexMax to disable or 0 to flush every time
 *
 * auto open and auto close files, may return a null callback if open file error
 */
#define ZFOutputCallbackForLocalFile(dataToCheckParentPath, filePath, ...) \
    _ZFP_ZFOutputCallbackForLocalFile(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, dataToCheckParentPath, filePath, ##__VA_ARGS__)

// ============================================================
// ZFInputCallbackForLocalFile
extern ZF_ENV_EXPORT ZFInputCallback _ZFP_ZFInputCallbackForLocalFile(ZF_IN const zfcharA *callerFile,
                                                                      ZF_IN const zfcharA *callerFunction,
                                                                      ZF_IN zfindex callerLine,
                                                                      ZF_IN const ZFSerializableData &dataToCheckParentPath,
                                                                      ZF_IN const zfchar *filePath,
                                                                      ZF_IN_OPT ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read,
                                                                      ZF_IN_OPT const ZFFileBOM *autoSkipBOMTable = &ZFFileBOMUTF8,
                                                                      ZF_IN_OPT zfindex autoSkipBOMTableCount = 1);
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <zfstring category="localPath" ... />
 *       <ZFFileOpenOptionFlags category="flags" ... /> // optional, ZFFileOpenOption::e_Read by default
 *       <zfstring category="autoSkipBOMTable" ... /> // optional, ZFFileBOMUTF8 by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFInputCallbackForLocalFile ZFInputCallbackForLocalFile
/** @brief see #ZFCallbackSerializeCustomTypeRegister */
#define ZFCallbackSerializeCustomTypeName_ZFInputCallbackForLocalFile ZFM_TOSTRING(ZFCallbackSerializeCustomType_ZFInputCallbackForLocalFile)

/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const ZFSerializableData &)dataToCheckParentPath: see #ZFSerializableDataTagKeyword_filePath
 * -  (const zfchar *)localPath: local file path to use
 * -  (ZFFileOpenOption)flags: flags to open file
 * -  (const ZFFileBOM *)autoSkipBOMTable: BOM to skip,
 *   if not null, BOM would be discarded and BOM's size would be ignored while calculating the file's size
 * -  (zfindex)autoSkipBOMTableCount autoSkipBOMTable's count
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * auto setup callback cache id with res file path
 */
#define ZFInputCallbackForLocalFile(dataToCheckParentPath, filePath, ...) \
    _ZFP_ZFInputCallbackForLocalFile(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, dataToCheckParentPath, filePath, ##__VA_ARGS__)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFileCallbackDef_h_

#include "ZFFileCallbackDef_ZFIOBridgeCallbackUsingTmpFile.h"

