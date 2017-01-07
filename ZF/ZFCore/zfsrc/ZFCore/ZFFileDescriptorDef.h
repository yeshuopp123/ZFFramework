/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFFileDescriptorDef.h
 * @brief file io from a string descriptor
 */

#ifndef _ZFI_ZFFileDescriptorDef_h_
#define _ZFI_ZFFileDescriptorDef_h_

#include "ZFFile.h"
#include "ZFFileCallbackDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT ZFInputCallback _ZFP_ZFInputCallbackForFileDescriptor(ZF_IN const zfcharA *callerFile,
                                                                           ZF_IN const zfcharA *callerFunction,
                                                                           ZF_IN zfindex callerLine,
                                                                           ZF_IN const zfchar *fileDescriptor,
                                                                           ZF_IN_OPT zfindex fileDescriptorLen = zfindexMax,
                                                                           ZF_IN_OPT ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read,
                                                                           ZF_IN_OPT const ZFFileBOM *autoSkipBOMTable = &ZFFileBOMUTF8,
                                                                           ZF_IN_OPT zfindex autoSkipBOMTableCount = 1);
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <zfstring category="fileDescriptor" ... />
 *       <ZFFileOpenOptionFlags category="flags" ... /> // optional, ZFFileOpenOption::e_Read by default
 *       <zfstring category="autoSkipBOMTable" ... /> // optional, ZFFileBOMUTF8 by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFInputCallbackForFileDescriptor ZFInputCallbackForFileDescriptor
/** @brief see #ZFCallbackSerializeCustomTypeRegister */
#define ZFCallbackSerializeCustomTypeName_ZFInputCallbackForFileDescriptor ZFM_TOSTRING(ZFCallbackSerializeCustomType_ZFInputCallbackForFileDescriptor)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputCallbackForFileDescriptor_fileDescriptor zfText("fileDescriptor")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputCallbackForFileDescriptor_flags zfText("flags")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputCallbackForFileDescriptor_autoSkipBOMTable zfText("autoSkipBOMTable")
/**
 * @brief create a input callback from file descriptor, see #ZFFileDescriptorTypeRegister
 *
 * see #ZFFileDescriptorTypeRegister for more info\n
 * params:
 * -  (const zfchar *)fileDescriptor file's descriptor type, such as "Res:path/file.ext"
 * -  (zfindex)fileDescriptorLen length of fileDescriptor
 * -  (ZFFileOpenOptionFlags)flags optional, file's open flag
 * -  (const ZFFileBOM *)autoSkipBOMTable: BOM to skip,
 *   if not null, BOM would be discarded and BOM's size would be ignored while calculating the file's size
 * -  (zfindex)autoSkipBOMTableCount autoSkipBOMTable's count
 */
#define ZFInputCallbackForFileDescriptor(fileDescriptor, ...) \
    _ZFP_ZFInputCallbackForFileDescriptor(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, \
        fileDescriptor, ##__VA_ARGS__)

extern ZF_ENV_EXPORT ZFOutputCallback _ZFP_ZFOutputCallbackForFileDescriptor(ZF_IN const zfcharA *callerFile,
                                                                             ZF_IN const zfcharA *callerFunction,
                                                                             ZF_IN zfindex callerLine,
                                                                             ZF_IN const zfchar *fileDescriptor,
                                                                             ZF_IN_OPT zfindex fileDescriptorLen = zfindexMax,
                                                                             ZF_IN_OPT ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Create);
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <zfstring category="fileDescriptor" ... />
 *       <ZFFileOpenOptionFlags category="flags" ... /> // optional, ZFFileOpenOption::e_Create by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputCallbackForFileDescriptor ZFOutputCallbackForFileDescriptor
/** @brief see #ZFCallbackSerializeCustomTypeRegister */
#define ZFCallbackSerializeCustomTypeName_ZFOutputCallbackForFileDescriptor ZFM_TOSTRING(ZFCallbackSerializeCustomType_ZFOutputCallbackForFileDescriptor)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFOutputCallbackForFileDescriptor_fileDescriptor zfText("fileDescriptor")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFOutputCallbackForFileDescriptor_flags zfText("flags")
/**
 * @brief create a output callback from file descriptor, see #ZFFileDescriptorTypeRegister
 *
 * see #ZFFileDescriptorTypeRegister for more info\n
 * params:
 * -  (const zfchar *)fileDescriptor file's descriptor type, such as "Res:path/file.ext"
 * -  (zfindex)fileDescriptorLen length of fileDescriptor
 * -  (ZFFileOpenOptionFlags)flags optional, file's open flag
 */
#define ZFOutputCallbackForFileDescriptor(fileDescriptor, ...) \
    _ZFP_ZFOutputCallbackForFileDescriptor(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, \
        fileDescriptor, ##__VA_ARGS__)

/**
 * @brief see #ZFFileDescriptorTypeRegister
 */
typedef ZFInputCallback (*ZFFileDescriptorInputCallbackGetter)(ZF_IN const zfchar *fileDescriptorData,
                                                               ZF_IN_OPT zfindex dataLen /* = zfindexMax */,
                                                               ZF_IN_OPT ZFFileOpenOptionFlags flags /* = ZFFileOpenOption::e_Read */,
                                                               ZF_IN_OPT const ZFFileBOM *autoSkipBOMTable /* = &ZFFileBOMUTF8 */,
                                                               ZF_IN_OPT zfindex autoSkipBOMTableCount /* = 1 */);
/**
 * @brief see #ZFFileDescriptorTypeRegister
 */
typedef ZFOutputCallback (*ZFFileDescriptorOutputCallbackGetter)(ZF_IN const zfchar *fileDescriptorData,
                                                                 ZF_IN_OPT zfindex dataLen /* = zfindexMax */,
                                                                 ZF_IN_OPT ZFFileOpenOptionFlags flags /* = ZFFileOpenOption::e_Create */);
/**
 * @brief register a file descriptor
 *
 * a file descriptor is encoded in this format:
 * @code
 *   Type:typeData
 * @endcode
 * such as "Res:file.txt"\n
 * \n
 * with file descriptor, you may easily create input and output callback
 * without knowing the file's absolute path or the file's type (resource file or normal file or binary data)\n
 * \n
 * you may register your own types by this method,
 * if you do,
 * you should supply callbacks to create input and output callback for the type,
 * and call #ZFFileDescriptorTypeUnregister if want to unregister
 */
extern ZF_ENV_EXPORT void ZFFileDescriptorTypeRegister(ZF_IN const zfchar *descriptorType,
                                                       ZF_IN ZFFileDescriptorInputCallbackGetter inputCallbackGetter,
                                                       ZF_IN ZFFileDescriptorOutputCallbackGetter outputCallbackGetter);

/**
 * @brief see #ZFFileDescriptorTypeRegister
 */
extern ZF_ENV_EXPORT void ZFFileDescriptorTypeUnregister(ZF_IN const zfchar *descriptorType);

/**
 * @brief usually for debug use only
 */
extern ZF_ENV_EXPORT void ZFFileDescriptorTypeGetAll(ZF_OUT ZFCoreArray<const zfchar *> &ret);
/** @brief see #ZFFileDescriptorTypeGetAll */
inline ZFCoreArrayPOD<const zfchar *> ZFFileDescriptorTypeGetAll(void)
{
    ZFCoreArrayPOD<const zfchar *> ret;
    ZFFileDescriptorTypeGetAll(ret);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFileDescriptorDef_h_

