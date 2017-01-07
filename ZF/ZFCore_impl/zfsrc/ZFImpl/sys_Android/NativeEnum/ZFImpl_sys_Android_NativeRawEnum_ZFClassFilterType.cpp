/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_NativeEnum.h"
#include "ZFCore/ZFObject.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFClassFilterType ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFCore_ZFClassFilterType)
#define ZFImpl_sys_Android_JNI_NAME_ZFClassFilterType ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFCore.ZFClassFilterType)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFClassFilterType, ZFClassFilterTypeInclude)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFClassFilterType, ZFClassFilterTypeExclude)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFClassFilterType, ZFClassFilterTypeIncludeChildOf)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFClassFilterType, ZFClassFilterTypeIncludeChildTypeOf)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFClassFilterType, ZFClassFilterTypeIncludeParentOf)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFClassFilterType, ZFClassFilterTypeIncludeParentTypeOf)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFClassFilterType, ZFClassFilterTypeExcludeChildOf)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFClassFilterType, ZFClassFilterTypeExcludeChildTypeOf)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFClassFilterType, ZFClassFilterTypeExcludeParentOf)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFClassFilterType, ZFClassFilterTypeExcludeParentTypeOf)

#endif // #if ZF_ENV_sys_Android

