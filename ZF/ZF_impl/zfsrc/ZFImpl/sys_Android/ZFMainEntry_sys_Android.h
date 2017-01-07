/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZFMainEntry_sys_Android_h_
#define _ZFI_ZFMainEntry_sys_Android_h_

#include "ZFCore.h"
#include "ZFImpl_sys_Android_ZF.h"

/*
 * main entry steps:
 * -# prepare your loader Activity
 * -# load all so files in your Activity as static init step
 * -# start ZFMainEntry as a Activity
 * -# finish your loader Activity
 */
#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

// return android.content.Context
extern ZF_ENV_EXPORT JNIObjectHolder ZFImpl_sys_Android_appContext(void);
// return android.content.res.AssetManager
extern ZF_ENV_EXPORT JNIObjectHolder ZFImpl_sys_Android_assetManager(void);
// return android.app.Activity
extern ZF_ENV_EXPORT JNIObjectHolder ZFImpl_sys_Android_mainEntryActivity(void);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFMainEntry_sys_Android_h_

