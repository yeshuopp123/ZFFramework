/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSerializableDataIO_fileDescriptor.h
 * @brief util method to load #ZFSerializableDataFromIO
 *   as file descriptor
 */

#ifndef _ZFI_ZFSerializableDataIO_fileDescriptor_h_
#define _ZFI_ZFSerializableDataIO_fileDescriptor_h_

#include "ZFSerializableDataIO.h"
#include "ZFFileDescriptorDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief detect file extension accorrding to file descriptor
 *
 * detect by #ZFFile::fileExtOf
 */
extern ZF_ENV_EXPORT zfstring ZFSerializableDataIOFileDescriptorDetect(ZF_IN const zfchar *fileDescriptor);

/**
 * @brief see #ZFSerializableDataFromIO, #ZFInputCallbackForFileDescriptor
 *
 * this method would:
 * -# check ioType by #ZFSerializableDataIOFileDescriptorDetect
 * -# open input by #ZFInputCallbackForFileDescriptor
 * -# load serializableData by #ZFSerializableDataFromIO
 */
extern ZF_ENV_EXPORT zfbool ZFSerializableDataFromFileDescriptor(ZF_OUT ZFSerializableData &serializableData,
                                                                 ZF_IN const zfchar *fileDescriptor,
                                                                 ZF_OUT_OPT zfstring *errorMessage = zfnull);
/** @brief see #ZFSerializableDataFromFileDescriptor */
inline ZFSerializableData ZFSerializableDataFromFileDescriptor(ZF_IN const zfchar *fileDescriptor,
                                                               ZF_OUT_OPT zfstring *errorMessage = zfnull)
{
    ZFSerializableData ret;
    if(ZFSerializableDataFromFileDescriptor(ret, fileDescriptor, errorMessage))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}
/**
 * @brief see #ZFSerializableDataToFileDescriptor
 *
 * this method would:
 * -# check ioType by #ZFSerializableDataIOFileDescriptorDetect
 * -# open output by #ZFOutputCallbackForFileDescriptor
 * -# write serializableData to output by #ZFSerializableDataToIO
 */
extern ZF_ENV_EXPORT zfbool ZFSerializableDataToFileDescriptor(ZF_IN const zfchar *fileDescriptor,
                                                               ZF_IN const ZFSerializableData &serializableData,
                                                               ZF_OUT_OPT zfstring *errorMessage = zfnull);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDataIO_fileDescriptor_h_

